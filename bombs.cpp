#include "bombs.h"

//konstruktor:
Bomb::Bomb(int nx,int ny,int _owner,int _flameLength,bool _trigerred,bool _jelly)
             {
              x=nx;
              y=ny;
              exploded=false;
              moveDelay=50;
              moveTime=moveDelay;
              fuse=fuseLength;
              if(players[_owner].hasDisease(DIS_SMALLFUSE))
               fuse=smallFuseLength;
              flameLength=_flameLength;
              owner=_owner;
              jelly=_jelly;
              trigerred=_trigerred;
              moveDirection=MOV_NONE;
             }
//dokoncit rozdelany pohyb (bomba se pohybuje po kopnuti)
void Bomb::finishMove()
{
                  int mx=x;
                  int my=y;
                  setPos(20,20); 
                  switch(moveDirection)
                  {
                        case MOV_NONE:
                            return;
                            break;
                        case MOV_LEFT:
                            mx=x-1;                            
                            break;
                        case MOV_RIGHT:
                            mx=x+1;
                            break;
                        case MOV_UP:
                            my=y-1;
                            break;
                        case MOV_DOWN:
                            my=y+1;
                            break;
                  }  
                    
                  
                  if(
                     (!isValidXY(mx,my))
                     ||
                     ((map[mx][my]!=FLD_NOTHING)&&(map[mx][my]!=FLD_EXPLOSION))
                     ||
                     (findPlayerAt(mx,my))
                     )
                   {
                        if(jelly)
                         {
                                moveDirection=invertDirection(moveDirection);
                                moveTime=moveDelay;
                         }
                        else
                         moveDirection=MOV_NONE;
                        return;
                   }
                                 
                  drawCharAtMapAt(patterns[patternIndex].charNothing,x,y,patterns[patternIndex].backColor*16+patterns[patternIndex].colorNothing);
                  if(bonusMap[x][y]>-1)
                   {
                        Bonus bonus=bonusList[bonusMap[x][y]];
                        drawCharAtMapAt(bonus.write,x,y,bonus.color);
                   }
                  map[x][y]=FLD_NOTHING;  
                  x=mx;
                  y=my;
                  if(trigerred)
                      drawCharAtMapAt(CHAR_TRIGGERBOMB,x,y,patterns[patternIndex].backColor*16+players[owner].color);
                  else
                    drawCharAtMapAt(CHAR_BOMB,x,y,patterns[patternIndex].backColor*16+players[owner].color);
                  if(map[mx][my]==FLD_EXPLOSION) 
                    {
                        explode();
                        return;
                    }
                  map[x][y]=FLD_BOMB;
                  moveTime=moveDelay;
}

//exploze
void Bomb::explode()
{
                    if(exploded) return;
                    exploded=true;
                     players[owner].bombCount++;
                     setExplosionAt(x,y,owner); 
                     bool fireLeft=true;
                     bool fireRight=true;
                     bool fireUp=true;
                     bool fireDown=true;
                     for(int i=1;i<=flameLength;i++)
                      {
                        if(fireLeft)
                        {
                            int nx=x-i;
                            int ny=y;
                            if(isValidXY(nx,ny))
                              {
                                if(map[nx][ny]==FLD_WALL) fireLeft=false;
                                if(map[nx][ny]==FLD_STONE) fireLeft=false;
                                if(map[nx][ny]!=FLD_STONE)
                                 setExplosionAt(nx,ny,owner);                                                 
                              }                                               
                        }
                        if(fireRight)
                        {
                            int nx=x+i;
                            int ny=y;
                            if(isValidXY(nx,ny))
                              {
                                if(map[nx][ny]==FLD_WALL) fireRight=false;
                                if(map[nx][ny]==FLD_STONE) fireRight=false;
                                if(map[nx][ny]!=FLD_STONE)
                                 setExplosionAt(nx,ny,owner);                                                 
                              }                                             
                        }
                        if(fireUp)
                        {
                            
                            int nx=x;
                            int ny=y-i;
                            if(isValidXY(nx,ny))
                              {
                                if(map[nx][ny]==FLD_WALL) fireUp=false;
                                if(map[nx][ny]==FLD_STONE) fireUp=false;
                                if(map[nx][ny]!=FLD_STONE)
                                 setExplosionAt(nx,ny,owner);                                                 
                              }
                        }
                        if(fireDown)
                        {
                            int nx=x;
                            int ny=y+i;
                            if(isValidXY(nx,ny))
                              {
                                if(map[nx][ny]==FLD_WALL) fireDown=false;
                                if(map[nx][ny]==FLD_STONE) fireDown=false;
                                if(map[nx][ny]!=FLD_STONE)
                                 setExplosionAt(nx,ny,owner);                                                 
                              }                                               
                        }
                      }
}

//vyhleda instanci bomby na dane pozici na mape
Bomb* findBombAt(int bx,int by)
{
    Bomb *b=NULL;
    BombItem *bi=blist.head;
    while(bi)
                    {
                        b=&bi->bomb;
                        if(b->x==bx)
                         if(b->y==by)
                          {                               
                                return b;
                                break;
                          }
                        
                        bi=bi->next;
                    }
    b=NULL;
    return NULL;
}
