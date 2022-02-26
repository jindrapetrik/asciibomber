#ifndef __PLAYERS_H__
#define __PLAYERS_H__

class Player
{
      
      public:
             int x,y;  //pozice na mape  
             int mx,my; //pozice pro pohyb
             int color; //barva
             int shadowColor; //tmavsi barva
             bool alive; //zda je nazivu
             int index; //index v poli hracu
             int bombCount; //aktualni pocet bomb
             int flameLength; //delka plamene bomby
             int moveDelay; //Cas pohybu
             int moveTime; //Zbyvajici doba do dalsiho pohybu
             int moveDirection; //Smer pohybu
             int direction; //Natoceni
             bool kicking; //Zda umi kopat (bonus kopani)
             bool spooger; //Zda muze davat vice bomb v rade za sebou (bonus)
             int triggerCount; //Pocet bomb dalkove odpalovatelnych
             int maxBombCount; //Maximalni pocet bomb 
             bool jelly; //Zele bomby (bonus)
             int disease; //Cislo nemoci, kterou ma (viz konstanty DIS_*)
             int diseaseTime; //Doba ktera zbyva do uzdraveni
             int drawFrame; //cislo prekresleni pro vykresleni stavu nemocen
             Player()
             {
                    
             }
             Player(int _index,int _color,int _shadowColor)
             {              
              mx=-1;
              my=-1;
              drawFrame=0;     
              diseaseTime=0;                
              moveDelay=50-bonusList[BNS_PLUSSPEED].bornWith*5;
              if(moveDelay<5) moveDelay=5;
              disease=DIS_NOTHING;
              spooger=bonusList[BNS_SPOOGER].bornWith>=1;
              jelly=bonusList[BNS_JELLY].bornWith>=1;
              moveTime=moveDelay;    
              kicking=bonusList[BNS_KICK].bornWith>=1;
              moveDirection=MOV_NONE; 
              triggerCount=0;  
              
              maxBombCount=1+bonusList[BNS_PLUSBOMB].bornWith;
              bombCount=maxBombCount;
              
              if(bonusList[BNS_TRIGGER].bornWith>=1) 
                triggerCount=bombCount;
              alive=true;
              color=_color;
              index=_index;
              
              flameLength=1+bonusList[BNS_PLUSFLAME].bornWith;
              shadowColor=_shadowColor;
              direction=MOV_DOWN;
              //drawCharAtMapAt(CHAR_PLAYER,x,y,patterns[patternIndex].backColor*16+color);              
             }
             
             //vitezstvi:
             void victory()
             {
                    if(endit) return;
                    showJSC("screen\\victory.jsc",color);
                    enterToContinue();
                    endit=true;                    
             }
             
             //nastaveni pozice:
             void setPos(int nx,int ny)
             {
                    x=nx;
                    y=ny;
                    for(int p=0;p<=4;p++)
                     {
                         int fx=x+coordArr[p].X;
                         int fy=y+coordArr[p].Y;
                         if(isValidXY(fx,fy))
                         {
                                if(map[fx][fy]==FLD_WALL)
                                  map[fx][fy]=FLD_NOTHING;
                         }                         
                     }
                    drawCharAtMapAt(CHAR_PLAYER,x,y,patterns[patternIndex].backColor*16+color);                    
             }
             
             
             //testuje zda ma urcitou nemoc
             bool hasDisease(int hdisease)
             {
                    int dis=1<<hdisease;
                    int curDis=disease;
                    if(curDis==DIS_NOTHING) return false;
                    return (disease&dis)==dis;
             }
             
             //Nastaveni pohybu urcitym smerem:
             void setMove(int _moveDirection)
             {
                    
                    moveDirection=_moveDirection;
                    if(moveDirection!=MOV_NONE)
                      direction=_moveDirection;
             }
             
             //Vykresleni hrace:
             void draw()
             {
                    drawFrame++;
                    if(drawFrame==3) drawFrame=0;
                    
                    if((disease!=DIS_NOTHING)&&(drawFrame==0))
                      drawCharAtMapAt(CHAR_PLAYER,x,y,patterns[patternIndex].backColor*16+(rand()%16));
                    else
                      drawCharAtMapAt(CHAR_PLAYER,x,y,patterns[patternIndex].backColor*16+color);
             }
             
             void takeDisease();
             void takeBonus(int bonusIndex);
             
                          
             void finishMove();
             
             
             void move(int nx,int ny) 
             {                    
                                  
             }
             
            void placeBombAt(int bx,int by);
            void placeBomb();
            
            //dalkove odpaleni bomby:
            void triggerBomb()
            {
                BombItem *bi=blist.head;
                while(bi)
                {
                    Bomb b=bi->bomb;
                    if(b.owner==index)
                    if(b.trigerred) 
                    {
                        b.explode();
                        return;
                    }
                    bi=bi->next;
                }
            }
};

int playerCount=2;
const int maxPlayerCount=10;
Player players[maxPlayerCount];
COORD startLocations[maxPlayerCount];


//vrati hrace na urcite pozici:
Player* findPlayerAt(int px,int py)
{
 for(int p=0;p<playerCount;p++)
 {
        if(players[p].x==px)
         if(players[p].y==py)
           return &players[p];
 }
 return NULL;
}
#endif

