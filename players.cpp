#include "players.h"

void Player::takeDisease()
{
    int newDisease=rand()%diseaseCount;                      
                            if(newDisease==DIS_SWAP)
                            {
                                swapPlayers();
                            }
                            else
                            {                                
                                diseaseTime=diseaseDelay;
                                if(disease<0) disease=0;
                                disease=disease | (1<<newDisease);
                            }
                            //printf("got disease:%d   \n",newDisease);
                            //printf("current disease:%d   ",disease);
}

void Player::takeBonus(int bonusIndex)
             {
                
                    
                    switch(bonusIndex)
                    {
                        case BNS_PLUSBOMB:
                            bombCount++;
                            maxBombCount++;
                            break;
                        case BNS_PLUSFLAME:
                            flameLength++;
                            break;    
                        case BNS_KICK:
                            kicking=true;
                            break;
                        case BNS_PLUSSPEED:
                            moveDelay-=5;
                            if(moveDelay<5) moveDelay=5;
                            break;    
                        case BNS_SPOOGER:
                            spooger=true;
                            break;
                        case BNS_GOLDFLAME:
                            flameLength=50;
                            break;            
                        case BNS_TRIGGER:
                            triggerCount=maxBombCount;
                            break;    
                        case BNS_DISEASE:
                            takeDisease();
                            break;     
                        case BNS_SUPERBADDISEASE:
                                takeDisease();
                                takeDisease();
                                takeDisease();
                                break;
                        case BNS_RANDOM:
                                int newBonus=0;
                                do
                                {
                                    newBonus=rand()%bonusCount;
                                }while(bonusList[newBonus].forbidden);
                                
                                takeBonus(newBonus);
                                break;
                                                                            
                    }
             }

void Player::placeBombAt(int bx,int by)
            {
                Bomb bomb(bx,by,index,hasDisease(DIS_MINFLAME)?1:flameLength,(triggerCount>0)&&(!hasDisease(DIS_SMALLFUSE)),jelly);
                if(!hasDisease(DIS_SMALLFUSE))
                  if(triggerCount>0) triggerCount--;
                blist.add(bomb);
                map[bx][by]=FLD_BOMB;
                bombCount--;
                if(!((bx==x)&&(by==y)))
                 {
                    if(bomb.trigerred)
                      drawCharAtMapAt(CHAR_TRIGGERBOMB,bx,by,patterns[patternIndex].backColor*16+color);
                    else
                      drawCharAtMapAt(CHAR_BOMB,bx,by,patterns[patternIndex].backColor*16+color);
                 }
                
            }

void Player::placeBomb()
            {
                if(!alive) return;
                if(bombCount==0) return;  
                if(hasDisease(DIS_NOBOMBS)) return;
                if(spooger&&(findBombAt(x,y))&&(!hasDisease(DIS_BOMBMANIA)))
                {          
                    int bc=bombCount;
                    for(int p=1;p<=bc;p++)
                    {
                        int nx=x+p*coordArr[direction].X;
                        int ny=y+p*coordArr[direction].Y;
                        
                        if(!isValidXY(nx,ny)) break;
                        if(map[nx][ny]!=FLD_NOTHING) break;
                        if(findPlayerAt(nx,ny)) break;
                        placeBombAt(nx,ny);                           
                    }
                }
                else
                {
                 if(findBombAt(x,y)) return;
                 placeBombAt(x,y);
                }
                
            }
void Player::finishMove()
             {
                  mx=x;
                  my=y;  
                  int mvd=moveDirection;
                  if(hasDisease(DIS_INVERTKEYS))
                    {
                      mvd=invertDirection(mvd);
                    }
                  switch(mvd)
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
                                    
                    
                  if(mx==-1) return;
                  if(!isValidXY(mx,my))return;
                  if(!alive) return;
                  
                  if(kicking)
                  {
                        Bomb *b=findBombAt(mx,my);
                        if(b)
                        {
                            b->moveDirection=direction;
                        }
                  }
                  
                  if(map[mx][my]==FLD_EXPLOSION) 
                    {
                        alive=false; 
                        drawCharAtMapAt(patterns[patternIndex].charNothing,x,y,patterns[patternIndex].backColor*16+patterns[patternIndex].colorNothing);                  
                        testWin();
                    }
                  if(map[mx][my]!=FLD_NOTHING) return;
                  
                  blist.reset();
                  drawCharAtMapAt(patterns[patternIndex].charNothing,x,y,patterns[patternIndex].backColor*16+patterns[patternIndex].colorNothing);                  
                  Bomb *bombat=findBombAt(x,y);
                  if(bombat)
                  {
                        if(bombat->trigerred)
                          drawCharAtMapAt(CHAR_TRIGGERBOMB,x,y,patterns[patternIndex].backColor*16+color);
                        else
                          drawCharAtMapAt(CHAR_BOMB,x,y,patterns[patternIndex].backColor*16+color);
                  }
                    
                  x=mx;
                  y=my;
                  if(bonusMap[x][y]>-1)
                   {
                        int bonus=bonusMap[x][y];
                        bonusMap[x][y]=-1;                        
                        takeBonus(bonus);
                        
                   }
                  drawCharAtMapAt(CHAR_PLAYER,x,y,patterns[patternIndex].backColor*16+color);
                  moveTime=moveDelay;
                  if(hasDisease(DIS_SLOWDOWN)) moveTime=moveDelay*2;
                  if(hasDisease(DIS_SPEEDUP)) moveTime=5;
                  if(disease!=DIS_NOTHING)
                  {
                        for(int p=0;p<playerCount;p++)
                        {
                            if(p!=index)
                            {
                                if(players[p].x==x)
                                 if(players[p].y==y)
                                  {
                                    if(players[p].disease==DIS_NOTHING)
                                     players[p].disease=0;
                                    players[p].disease=players[p].disease | disease;
                                    if(players[p].diseaseTime<diseaseTime)
                                     players[p].diseaseTime=diseaseTime;
                                  }
                            }
                        }
                  }
             }
