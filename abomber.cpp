/*
        ASCII Bomberman 
================================
         Verze 1.0

 Autor: Jindrich Petrik 
                petrij2
                  JPEXS

*/

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h> 
#include <windows.h>
#include <winuser.h>

//Poznámka: nìjak mi to includování pouze .h souborù nejelo, tak includuji celé .cpp

#include "const.h"   //konstanty
#include "pattern.cpp" //graficka schemata
#include "console.cpp" //prace s obrazovkou
#include "bonuses.cpp"
#include "bombs.h"
#include "func.cpp"  //funkce
#include "players.h" //hraci
#include "bombs.cpp" 
#include "players.cpp"
using namespace std;


//Prohodi pozice hracu:             
void swapPlayers()
{
    int alivePlayerCount=0;
    Player* alivePlayers[maxPlayerCount];
    for(int p=0;p<playerCount;p++)
     if(players[p].alive) //pokud je nazivu
      {
            alivePlayers[alivePlayerCount]=&players[p];
            alivePlayerCount++;
      }
    if(alivePlayerCount>0)
    {
      int fpx=alivePlayers[0]->x;
      int fpy=alivePlayers[0]->y;
      for(int q=1;q<alivePlayerCount;q++)
       {
            alivePlayers[q-1]->x=alivePlayers[q]->x;
            alivePlayers[q-1]->y=alivePlayers[q]->y;
            alivePlayers[q-1]->draw();
       }
      alivePlayers[alivePlayerCount-1]->x=fpx;
      alivePlayers[alivePlayerCount-1]->y=fpy;
      alivePlayers[alivePlayerCount-1]->draw();
    }
}








//Zkouska, zda zbyva 1 zivy hrac
void testWin()
{
    int aliveCount=0;
    int lastAlive=0;
    for(int p=0;p<playerCount;p++)
     if(players[p].alive)
     {
            lastAlive=p;
            aliveCount++;
    }
    
    if(aliveCount==1)
     players[lastAlive].victory();
}

//Nastavi explozi na urcite pozici
void setExplosionAt(int ex,int ey,int owner)
{
    if(endit) return;
    //Vykreslime explozi:
    drawCharAtMapAt(CHAR_EXPLOSION,ex,ey,16*patterns[patternIndex].backColor+players[owner].color); 
  
    //Zrusime pripadny bonus, ktery explodoval:
    if(map[ex][ey]==FLD_NOTHING)
     bonusMap[ex][ey]=-1;
    
    //Pokud tam prave eploduje, prodlouzime explozi:
    if(hideMap[ex][ey]<explosionDelay)
     hideMap[ex][ey]=explosionDelay;
     
    //Pokud je na tom miste bomba, pak take eploduje:
    Bomb *bomb=findBombAt(ex,ey);
    map[ex][ey]=FLD_EXPLOSION;
    if(bomb)
     if(!bomb->exploded)
       bomb->explode();

    //Zabije hrace:
    for(int p=0;p<playerCount;p++)
     if(players[p].alive)
     {          
     if((players[p].x==ex)&&(players[p].y==ey))
       {
            players[p].alive=false;
       };
     }
    
    //Zkusi, zda nekdo vyhral:
    testWin();

    
}








//Vrati pozici podretezce v retezci:
int strpos(const char *haystack,const char *needle)
{
    const char *r=strstr(haystack,needle);
    if(!r) return -1;
    return (r - haystack);
}

//Vrati podretezec:
char* substr(char* haystack,int pos,int length)
{
    char r2[255];
    char *ret=r2;
    for(int p=0;p<length;p++)
    {
        ret[p]=haystack[pos+p];
    }
    ret[length]=0;
    return ret;
}

//Orizne retezec:
char* trim(char* s)
{
    int length=strlen(s);
    for(int p=0;p<length;p++)
    {
        if(s[p]!=' ')
         {
                return substr(s,p,length-p);
         }
    }
    return "";
}



//Vytiskne obsah souboru
void printFile(char * filename)
{
    clrscr();
    FILE *f=fopen(filename,"rt");
    char read[255];
    char *ret;
    do
    {
      ret=fgets(read,255,f);
      if(ret)
        printf(ret);
    }while(ret);
    fclose(f);
}


//Nacte mapu ze souboru
void loadMap(char *filename)
{
    FILE *f=fopen(filename,"rt");
    if (f==NULL) { 
               puts("Chyba: Soubor se nepodaøilo otevøít.");
               exit(1);
             }    
    char read[255];
    while(true)
    {
        char *ret=fgets(read,255,f);
        if(!ret) break;
        if(strlen(read)>2)
        {
            if(strstr(read,"-S,")==read) //Startovni pozice
            {
                for(int p=0;p<maxPlayerCount;p++)
                {
                int sepPos[5]={0,0,0,0};
                int curSep=0;
                int sl=strlen(read);
                for(int q=0;q<sl;q++)
                {
                    if(read[q]==',')
                    {
                     sepPos[curSep]=q;
                     curSep++;
                    }
                }
                startLocations[p].X=atoi(substr(read,sepPos[1]+1,(sepPos[2]-sepPos[1]-1)));
                startLocations[p].Y=atoi(substr(read,sepPos[2]+1,(sepPos[3]-sepPos[2]-1)));
                ret=fgets(read,255,f);
                }
                
                //
            }
            if(strstr(read,"-R,")==read) //Dlazdice
            {
                for(int y=0;y<screenHeight;y++)
                {
                    for(int x=0;x<screenWidth;x++)
                    {
                      if(read[6+x]==':') map[x][y]=FLD_WALL;
                      if(read[6+x]=='#') map[x][y]=FLD_STONE;
                      if(read[6+x]=='.') map[x][y]=FLD_NOTHING;
                    }
                    ret=fgets(read,255,f);
                }
            }
            if(strstr(read,"-P,")==read) //Bonusy
            {
                for(int p=0;p<bonusCount;p++)
                {                    
                    bonusList[p].bornWith=atoi(trim(substr(ret,6,2)));
                    bonusList[p].hasOverride=1==atoi(substr(ret,9,1));
                    bonusList[p].override=atoi(trim(substr(ret,11,2)));
                    bonusList[p].forbidden=1==atoi(trim(substr(ret,15,1)));
                    ret=fgets(read,255,f);
                }
            }
        }
    }
    fclose(f);
    for(int y=0;y<screenHeight;y++)
  for(int x=0;x<screenWidth;x++)
   {
        hideMap[x][y]=0;
   } 
}

//Zjisti retezec ovladani hrace:
char* getStateStr(int state)
{
    switch(state)
    {
        case 0:
            return "off ";
            break;
        case 1:
            return "key1";
            break;
        case 2:
            return "key2";
            break;
        case 3:
            return "AI  ";  //neimplementovano
            break;
    }
    return NULL;
}


//Umisti nahodne bonusy na mapu
void placeBonuses()
{
    int wallcount=0;
  for(int y=0;y<screenHeight;y++)
	for(int x=0;x<screenWidth;x++)
	 {
      bonusMap[x][y]=-1;
	  if(map[y][x]==FLD_WALL) wallcount++;
     } 
  int curBonusId=0;
  int curBonusCount=1;
  int bx=0;
  int by=0;
  int totalBonusCount=1;
  int p=0;
  for(p=0;p<wallcount;p++)
		{
			do
			{
				do
				{
				  bx=(abs(rand()) % screenWidth);	
				  by=(abs(rand()) % screenHeight);			  			  
 				}while(bonusMap[bx][by]>-1);
			}while(map[bx][by]!=FLD_WALL); //dokud neni zed
		if(bonusList[curBonusId].getAppearCount()>=curBonusCount)
			{
				bonusMap[bx][by]=curBonusId;
				curBonusCount++;
				totalBonusCount++;
				if(totalBonusCount==wallcount) break;
			}
			else
			{
				curBonusId++;
				curBonusCount=1;
				totalBonusCount++;
				if(totalBonusCount==wallcount) break;
				if(curBonusId==12)
				 break;
			}
		}
	for(int q=0;q<bonusCount;q++)
	{
        if(bonusList[q].getAppearCount()<0)
        {
            int r=rand()%10+1;
            if(r<= -bonusList[q].getAppearCount())
            {
                do
    			{
    				do
    				{
    				  bx=(abs(rand()) % screenWidth);	
    				  by=(abs(rand()) % screenHeight);			  			  
     				}while(bonusMap[bx][by]>-1);
    			}while(map[bx][by]!=FLD_WALL);
    			bonusMap[bx][by]=q;
				totalBonusCount++;
				if(totalBonusCount==wallcount) break;
            }
        }
    }
}


//Vykresli mapu na obrazovku:
void drawMap()
{
     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     SetConsoleTextAttribute(hStdOut, 16*patterns[patternIndex].backColor);
     for(int y=0;y<screenHeight;y++)
     {     
      COORD coord={startX,startY+y}; 
      SetConsoleCursorPosition(hStdOut, coord);  
      for(int x=0;x<screenWidth;x++)
       {
              SetConsoleTextAttribute(hStdOut, 16*patterns[patternIndex].backColor+patterns[patternIndex].colorNothing);
              if(map[x][y]==FLD_NOTHING)
               cout<<patterns[patternIndex].charNothing;
              else if(map[x][y]==FLD_STONE)
               { 
                 SetConsoleTextAttribute(hStdOut, 16*patterns[patternIndex].backColor+patterns[patternIndex].colorStone);            
                 cout<<patterns[patternIndex].charStone;
               }
              else if(map[x][y]==FLD_WALL)
               {
                 SetConsoleTextAttribute(hStdOut, 16*patterns[patternIndex].backColor+patterns[patternIndex].colorWall);                                
                 cout<<patterns[patternIndex].charWall;//char(177);  
               }
       }                  
     }
}




//Metoda, ktera bezi neustale a zajistuje synchronizaci pohybu hracu:
void turn()
{
bool placingBomb0=false;    //hrac 0 macka davani bomby
bool placingBomb1=false;    //hrac 1 -||-
bool triggeringBomb0=false;    //hrac 0 dalkove odpaluje bombu
bool triggeringBomb1=false;   //hrac 1 -||-

//Smycka, koncici pri nastaveni endit
while(!endit)
{
  //pri stisku pohybove klavesy nastavit smer hrace:
  if(GetAsyncKeyState(VK_LEFT)<0) players[0].setMove(MOV_LEFT);
   else if(players[0].moveDirection==MOV_LEFT) players[0].setMove(MOV_NONE);
  if(GetAsyncKeyState(VK_RIGHT)<0) players[0].setMove(MOV_RIGHT);
   else if(players[0].moveDirection==MOV_RIGHT) players[0].setMove(MOV_NONE);
  if(GetAsyncKeyState(VK_UP)<0) players[0].setMove(MOV_UP);
   else if(players[0].moveDirection==MOV_UP) players[0].setMove(MOV_NONE);
  if(GetAsyncKeyState(VK_DOWN)<0) players[0].setMove(MOV_DOWN);
   else if(players[0].moveDirection==MOV_DOWN) players[0].setMove(MOV_NONE);
  if(GetAsyncKeyState('D')<0) players[1].setMove(MOV_LEFT);
   else if(players[1].moveDirection==MOV_LEFT) players[1].setMove(MOV_NONE);    
  if(GetAsyncKeyState('G')<0) players[1].setMove(MOV_RIGHT);
   else if(players[1].moveDirection==MOV_RIGHT) players[1].setMove(MOV_NONE); 
  if(GetAsyncKeyState('R')<0) players[1].setMove(MOV_UP);
   else if(players[1].moveDirection==MOV_UP) players[1].setMove(MOV_NONE); 
  if(GetAsyncKeyState('F')<0) players[1].setMove(MOV_DOWN);
   else if(players[1].moveDirection==MOV_DOWN) players[1].setMove(MOV_NONE); 
  
  //Pri nemoci "Bomboba manie" pokladej porad bomby:
  if(players[0].hasDisease(DIS_BOMBMANIA)) players[0].placeBomb();
  if(players[1].hasDisease(DIS_BOMBMANIA)) players[1].placeBomb();
  
  //Hrac 0 stiskl klavesu pro polozeni bomby:
  if(GetAsyncKeyState(32)<0)
    { 
    if(!placingBomb0) 
     {
            players[0].placeBomb();
            placingBomb0=true;
     }
    }
    else
    {
        placingBomb0=false;
    }
  //Hrac 1 stiskl klavesu pro polozeni bomby:
  if(GetAsyncKeyState('A')<0) 
  { 
    if(!placingBomb1) 
     {
            players[1].placeBomb();
            placingBomb1=true;
     }
    }
    else
    {
        placingBomb1=false;
    }
    
  //Hrac 0 stiskl klavesu pro dalkove odpaleni bomby:          
  if(GetAsyncKeyState(VK_RETURN)<0)
    { 
    if(!triggeringBomb0) 
     {
            players[0].triggerBomb();
            triggeringBomb0=true;
     }
    }
    else
    {
        triggeringBomb0=false;
    }
    
  //Hrac 1 stiskl klavesu pro dalkove odpaleni bomby:
  if(GetAsyncKeyState('S')<0) 
  { 
    if(!triggeringBomb1) 
     {
            players[1].triggerBomb();
            triggeringBomb1=true;
     }
    }
    else
    {
        triggeringBomb1=false;
    }  
  
  //Pri stisknuti ESC konec teto metody:
  if(GetAsyncKeyState(VK_ESCAPE)<0) 
   {    
    return;
    }
    
    //Provest tahy bomb:
blist.reset();
while(blist.current)
{
    BombItem *bi=blist.current;
    Bomb *b=&bi->bomb;
    b->doTurn();
    if(b->exploded)
     blist.remove(bi);
    blist.next();
}

//Vykresleni bonusu:    
for(int y=0;y<screenHeight;y++)
  for(int x=0;x<screenWidth;x++)
   {
        if(hideMap[x][y]>0)
        {
            hideMap[x][y]--;
            if(hideMap[x][y]==0)
            {
                drawCharAtMapAt(patterns[patternIndex].charNothing,x,y,patterns[patternIndex].backColor*16+patterns[patternIndex].colorNothing);                  
                map[x][y]=FLD_NOTHING;
                if(bonusMap[x][y]>-1)
                  {
                        Bonus bonus=bonusList[bonusMap[x][y]];
                        drawCharAtMapAt(bonus.write,x,y,bonus.color);
                  }
            }
        }
   } 
   
   //Pohyb hracu a nemoci:
   for(int p=0;p<playerCount;p++)
   {
        if(players[p].moveTime>0) players[p].moveTime--;
        if(players[p].moveTime==0)
          players[p].finishMove();
        if(players[p].diseaseTime>0)
        {
            players[p].diseaseTime--;
            if(players[p].diseaseTime==0)
              players[p].disease=DIS_NOTHING;
        }
        if(players[p].alive)
         {
              players[p].draw();  
         }
   }
//Tah trva urcitou dobu:
Sleep(turnTime);
}
endit=false;
}

//Vytvori novou hru:
void newGame()
{
    clrscr();
    drawFrame(); 
   char soubor[30]="SCHEMES\\"; //adresar s mapama
   strcat(soubor,schemeName);
   strcat(soubor,".SCH"); //pripona mapy
   loadMap(soubor);
  
  blist.clear(); //Vymaze bomby z minuleho kola
  
  //Nastaveni pozice hracu:
  bool posSet[maxPlayerCount];
  for(int p=0;p<maxPlayerCount;p++)
    posSet[p]=false;
  for(int p=0;p<playerCount;p++)
  {
        players[p].index=p;
        int newPos=0;
        do
        {
         newPos=rand()%maxPlayerCount;
        }while(posSet[newPos]);
        posSet[newPos]=true;
        players[p].setPos(startLocations[newPos].X,startLocations[newPos].Y);        
  }
  
  placeBonuses();//Umisti bonusy na mapu
  drawMap();  //vykresli mapu
  turn(); //skoci do provadeni tahu
}


//Prekresli v menu options volby:
void prekresliOpt()
{
    drawStringAt("                     ",18,11,15);
    drawStringAt(patterns[patternIndex].name,18,11,15);
    drawStringAt("                     ",17,10,15);
    drawStringAt(schemeName,17,10,15);
}

//Menu voleb:
void optionsMenu()
{
    int menuIndex=0;
    int menuCount=2;
    clrscr();

    setPos(9,9);
    setColors(15);
    printf("Game options:");
    
    
    drawStringAt("Scheme: ",9,10,15);
    drawStringAt("Pattern: ",9,11,15);
    prekresliOpt();
    drawCharAt(CHAR_BOMB,8,10+menuIndex,15);
    bool keyUp=false;
    bool keyDown=false;
    bool keyRight=false;
    bool keyLeft=false;
    
    while(true)
    {
        if(GetAsyncKeyState(VK_UP)<0) //klavesa nahoru
         {
                if(!keyUp)
                {
                    drawCharAt(' ',8,10+menuIndex,15);
                    menuIndex--;
                    if(menuIndex<0) menuIndex=0;
                    keyUp=true;
                    drawCharAt(CHAR_BOMB,8,10+menuIndex,15);
                }
         }
         else
          keyUp=false;
        if(GetAsyncKeyState(VK_DOWN)<0) //klavesa dolu
         {
                if(!keyDown)
                {
                    drawCharAt(' ',8,10+menuIndex,15);
                    menuIndex++;
                    if(menuIndex==menuCount) menuIndex=menuCount-1;
                    keyDown=true;
                    drawCharAt(CHAR_BOMB,8,10+menuIndex,15);
                }
         }
         else
          keyDown=false;
          
         
       if(GetAsyncKeyState(VK_LEFT)<0) //klavesa vlevo
         {
                if(!keyLeft)
                {
                    switch(menuIndex)
                    {
                        case 0:
                                                                              
                            break;
                        case 1:
                            patternIndex--;
                            if(patternIndex<0) patternIndex=patternCount-1;  
                            prekresliOpt();
                            break;
                    }
                    keyLeft=true;
                }
         }
         else
          keyLeft=false;
       FILE *f;
       char soubor[30]="SCHEMES\\";
       if(GetAsyncKeyState(VK_RIGHT)<0) //klavesa vpravo
         {
                if(!keyRight)
                {
                    switch(menuIndex)
                    {
                        case 0:  //zadani nazvu mapy:
                            drawStringAt("Scheme:               ",9,10,15);
                            setPos(0,20);
                            cout<<"Enter Scheme name:";
                            char scheme[30];
                            cin>>scheme;                                                                                    
                            
                             
                             strcat(soubor,scheme);
                             strcat(soubor,".SCH");
   
                            f=fopen(soubor,"rt");
                            if (f==NULL) { 
                                       cout<<"Scheme "<<scheme<<" does not exist.";
                                     } 
                                     else
                                     {
                                            schemeName=scheme;
                                            fclose(f); 
                                        }
                                
                            for(int y=20;y<24;y++)
                            {
                            setPos(0,y);   
                            cout<<"                                    ";                                        
                            }
                            prekresliOpt();
                            break;
                        case 1:
                            patternIndex++;
                            if(patternIndex>=patternCount) patternIndex=0; 
                            prekresliOpt();
                            break;
                    }
                    keyRight=true;
                }
         }
         else
          keyRight=false;
                       
        if(GetAsyncKeyState(VK_ESCAPE)<0)  //zpatky do hlavniho menu
        {            
            mainMenu();
            return;
        }
    }
}

//Menu nova hra
void newGameMenu()
{
    int menuIndex=0;
    int menuCount=10;
    clrscr();
    int colorsType[10]={0,0,0,0,0,0,0,0,0,0}; //typy hracu barev
    bool typesAssigned[4]={false,false,false,false}; //prirazene typy hracu
    colors[1]=8;
    
    setPos(9,9);
    setColors(15);
    printf("Available players:");
    
    setColors(14);
    for(int c=0;c<=8;c++)
     {
            setPos(10,10+c);
            setColors(colors[c]);
            printf("Player  %d: off",c+1);
     }
    setPos(10,10+9);
    setColors(colors[9]);
    printf("Player 10: off"); 
    drawCharAt(CHAR_BOMB,8,10+menuIndex,15); //bomba na pozici aktualni polozky
    bool keyUp=false;
    bool keyDown=false;
    bool keyRight=false;
    bool keyLeft=false;
    
    while(true)//nekonecna smycka menu
    {
        if(GetAsyncKeyState(VK_UP)<0)
         {
                if(!keyUp)
                {
                    drawCharAt(' ',8,10+menuIndex,15);
                    menuIndex--;
                    if(menuIndex<0) menuIndex=0;
                    keyUp=true;
                    drawCharAt(CHAR_BOMB,8,10+menuIndex,15);
                }
         }
         else
          keyUp=false;
        if(GetAsyncKeyState(VK_DOWN)<0)
         {
                if(!keyDown)
                {
                    drawCharAt(' ',8,10+menuIndex,15);
                    menuIndex++;
                    if(menuIndex==menuCount) menuIndex=menuCount-1;
                    keyDown=true;
                    drawCharAt(CHAR_BOMB,8,10+menuIndex,15);
                }
         }
         else
          keyDown=false;
       if(GetAsyncKeyState(VK_LEFT)<0)
         {
                if(!keyLeft)
                {
                    colorsType[menuIndex]--;
                    if(colorsType[menuIndex]<0) colorsType[menuIndex]=0;
                    setPos(10,10+menuIndex);
                    setColors(colors[menuIndex]);
                    if(menuIndex<=8)
                      printf("Player  %d: %s",menuIndex+1,getStateStr(colorsType[menuIndex]));
                    else
                      printf("Player %d: %s",menuIndex+1,getStateStr(colorsType[menuIndex]));
                    keyLeft=true;
                }
         }
         else
          keyLeft=false;
       if(GetAsyncKeyState(VK_RIGHT)<0)
         {
                if(!keyRight)
                {
                    colorsType[menuIndex]++;
                    if(colorsType[menuIndex]==3) colorsType[menuIndex]=2; //Zde zmenit pri AI
                    setPos(10,10+menuIndex);
                    setColors(colors[menuIndex]);
                    if(menuIndex<=8)
                      printf("Player  %d: %s",menuIndex+1,getStateStr(colorsType[menuIndex]));
                    else
                      printf("Player %d: %s",menuIndex+1,getStateStr(colorsType[menuIndex]));
                    keyRight=true;
                }
         }
         else
          keyRight=false;
       if(GetAsyncKeyState(VK_RETURN)<0) //spusti hru
        {
                    playerCount=0;
                    typesAssigned[1]=false;
                    typesAssigned[2]=false;
                    
                    for(int p=0;p<10;p++)
                     {
                            if(colorsType[p]==1)
                            {
                               if(typesAssigned[1]) break;
                               colors[1]=0;
                               players[0]=Player(0,colors[p],colors[p]); 
                               colors[1]=8;
                               playerCount++;
                               typesAssigned[1]=true;
                            }
                            if(colorsType[p]==2)
                            {
                               if(typesAssigned[2]) break;
                               playerCount++; 
                               typesAssigned[2]=true;
                               colors[1]=0;
                               players[1]=Player(0,colors[p],colors[p]); 
                               colors[1]=8;
                            }
                     }
                    if(playerCount>1) 
                      {
                            newGame();
                            mainMenu();
                            return;
                      }
                     else
                       colors[1]=8;
                    
        }
        
        if(GetAsyncKeyState(VK_ESCAPE)<0) //Zpatky do hlavniho menu
        {
            mainMenu();
            return;
        }
    }
}


//Hlavni menu:
void mainMenu()
{
    int menuIndex=0;
    int menuCount=3;
    showJSC("screen\\menu.jsc"); //zobrazeni grafiky s "bombermanem"
    
    setColors(14);
    setPos(50,15); printf("New Game"); 
    setPos(50,16); printf("Options");
    setPos(50,17); printf("Exit Game");
    setPos(55,45); cout<<strVersion;
    drawCharAt(CHAR_BOMB,49,15+menuIndex,15);
    bool keyUp=false;
    bool keyDown=false;
    while(true)
    {
        if(GetAsyncKeyState(VK_UP)<0)
         {
                if(!keyUp)
                {
                    drawCharAt(177,49,15+menuIndex,1);
                    menuIndex--;
                    if(menuIndex<0) menuIndex=0;
                    keyUp=true;
                    drawCharAt(CHAR_BOMB,49,15+menuIndex,15);
                }
         }
         else
          keyUp=false;
        if(GetAsyncKeyState(VK_DOWN)<0)
         {
                if(!keyDown)
                {
                    drawCharAt(177,49,15+menuIndex,1);
                    menuIndex++;
                    if(menuIndex==menuCount) menuIndex=menuCount-1;
                    keyDown=true;
                    drawCharAt(CHAR_BOMB,49,15+menuIndex,15);
                }
         }
         else
          keyDown=false;
       if(GetAsyncKeyState(VK_RETURN)<0)
        {
            switch(menuIndex)
            {
                case 0: //nova hra
                    newGameMenu();
                    return;
                    break;
                case 1: //nastaveni
                    optionsMenu();   
                    return;                 
                    break;
                case 2: //konec
                    return;
                    break;
            }
        }
    }
}

//hlavni metoda
int main (void)
{
  FullScreen(); //prechod na celou obrazovku
  hideCursor();; //skryti kurzoru
  showJSC("screen\\jpexs.jsc"); //Zobrazeni loga JPEXS
  setColors(9);
  setPos(32,32);
  printf("PRESENTS...");
  enterToContinue();
  showJSC("screen\\bomberman.jsc"); //Zobrazeni grafickeho loga ASCII Bomberman
  enterToContinue(); //Pokracuj po stisku ENTERu
  clrscr(); //vymazani obrazovky
  srand((unsigned)time(0));  //nastaveni seedu nahodneho generatoru
  
  //Graficka schemata:
  patterns[0]=Pattern(2*16+0,2,0,8,2,219,'#',' ',"green acres");
  patterns[1]=Pattern(3*16+0,3,8,8,3,219,'#',' ',"standard acres");
  patterns[2]=Pattern(6*16+0,6,4,4,6,219,176,' ',"beach");
  patterns[3]=Pattern(7*16+0,7,9,0,6,178,'o',' ',"hockey");
  patterns[4]=Pattern(6*16+0,6,4,2,4,219,177,176,"forest");
  patternIndex=0;//aktualni graficke schema
  bonusList[0]=Bonus(BNS_PLUSBOMB,"an extra bomb",224,9*16+0,10,1); // J
  bonusList[1]=Bonus(BNS_PLUSFLAME,"flame length",'@',9*16+14,10,2); // J
  bonusList[2]=Bonus(BNS_DISEASE,"a disease",207,12*16+15,3,0); //J
  bonusList[3]=Bonus(BNS_KICK,"kick bombs",'L',9*16+7,4,0);  // J
  bonusList[4]=Bonus(BNS_PLUSSPEED,"extra speed",175,9*16+7,8,0); // J
  bonusList[5]=Bonus(BNS_PUNCH,"punch bombs",'e',9*16+7,0,0);//not used //2,0);
  bonusList[6]=Bonus(BNS_GRAB,"grab bombs",'C',9*16+7,0,0); //not used //2,0);
  bonusList[7]=Bonus(BNS_SPOOGER,"the spooger",124,9*16+7,1,0); // J
  bonusList[8]=Bonus(BNS_GOLDFLAME,"goldflame",'@',14*16+6,-2,0); // J
  bonusList[9]=Bonus(BNS_TRIGGER,"trigger mechanism",147,9*16+7,-4,0); //J
  bonusList[10]=Bonus(BNS_JELLY,"jelly (bouncy) bombs",'o',9*16+5,1,0); //J
  bonusList[11]=Bonus(BNS_SUPERBADDISEASE,"super bad disease",207,14*16+12,-4,0); //J
  bonusList[12]=Bonus(BNS_RANDOM,"random",'?',9*16+7,-2,0); //not used
      
  mainMenu(); //vstup do hlavniho menu:
  
  showJSC("screen\\final.jsc"); //Vykresli kralicka
  enterToContinue(); //Pokracuj po stisku ENTERu
  
  return 0;
}

