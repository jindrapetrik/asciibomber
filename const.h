#ifndef __CONST_H__
#define __CONST_H__

bool endit=false; //Ukonceni cyklu
const int startX=32; //Pozice X mapy na obrazovce
const int startY=20; //Pozice Y mapy na obrazovce
const int screenWidth=15; //Sirka obrazovky
const int screenHeight=11; //Vyska obrazovky

const int FLD_NOTHING=1; //Dlazdice prazdna
const int FLD_STONE=2; //Dlazdice kamen
const int FLD_WALL=3; //Dlazdice zed
const int FLD_BOMB=4; //Dlazdice bomba
const int FLD_EXPLOSION=5; //Dlazdice exploze

const int turnTime=10;  //Cas kola (pro metodu Sleep)
const int fuseLength=300; //Delka zapalne snury bomb
const int smallFuseLength=100; //Delka kratke zapalne snury (pro nemoc)
const int explosionDelay=100; //Doba zobrazeni exploze
const int diseaseDelay=2500; //Doba jak dlouho je hrac nemocen

const char* strVersion="Version 1.0"; //Zobrazena verze

COORD coordArr[5]={{0,0},{-1,0},{0,-1},{1,0},{0,1}}; //Koordinaty pro pohyb

const int MOV_NONE=0; //Zadny pohyb
const int MOV_LEFT=1; //Pohyb vlevo
const int MOV_UP=2; //Pohyb nahoru
const int MOV_RIGHT=3; //Pohyb vpravo
const int MOV_DOWN=4; //Pohyb dolu

const int diseaseCount=8; //Pocet Nemoci

//Nemoce:
const int DIS_NOTHING=-1;
const int DIS_NOBOMBS=0;
const int DIS_SWAP=1;
const int DIS_BOMBMANIA=2;
const int DIS_SLOWDOWN=3;
const int DIS_INVERTKEYS=4;
const int DIS_MINFLAME=5;
const int DIS_SMALLFUSE=6;
const int DIS_SPEEDUP=7;

//Bonusy:
const int BNS_NOTHING=-1;
const int BNS_PLUSBOMB=0;
const int BNS_PLUSFLAME=1;     
const int BNS_DISEASE=2;
const int BNS_KICK=3;
const int BNS_PLUSSPEED=4;
const int BNS_PUNCH=5;
const int BNS_GRAB=6;
const int BNS_SPOOGER=7;
const int BNS_GOLDFLAME=8; 
const int BNS_TRIGGER=9;        
const int BNS_JELLY=10;
const int BNS_SUPERBADDISEASE=11;
const int BNS_RANDOM=12;


//Znaky:
const char CHAR_PLAYER=2;
const char CHAR_BOMB=224;
const char CHAR_TRIGGERBOMB=147;
const char CHAR_EXPLOSION=15;

//Barvy:
int colors[10]={15,0,12,9,10,14,11,13,4,5};

//Mapa:
int map[screenWidth][screenHeight]={
     {1,1,1,1,1,1,1,1,1,1,1},
     {1,1,1,1,1,1,1,1,1,1,1},
     {1,1,1,1,1,1,1,1,1,1,1},
     {1,1,1,1,1,1,1,1,1,1,1},
     {1,1,1,1,1,1,1,1,1,1,1},
     {1,1,1,1,1,1,1,1,1,1,1},
     {1,1,1,1,1,1,1,1,1,1,1},
     {1,1,1,1,1,1,1,1,1,1,1},
     {1,1,1,1,1,1,1,1,1,1,1},
     {1,1,1,1,1,1,1,1,1,1,1},
     {1,1,1,1,1,1,1,1,1,1,1},
     {1,1,1,1,1,1,1,1,1,1,1},
     {1,1,1,1,1,1,1,1,1,1,1},          
     {1,1,1,1,1,1,1,1,1,1,1},
     {1,1,1,1,1,1,1,1,1,1,1}
     };

//Doby zobrazeni objektu (exploze):
int hideMap[screenWidth][screenHeight];

//Mapa bonusu:
int bonusMap[screenWidth][screenHeight];

char *schemeName="BASIC"; //Mapa

void setExplosionAt(int ex,int ey,int owner); //Nastaveni eploze na pozici
void swapPlayers(); //Prohodit hrace (nemoc)
void testWin(); //Zkouska vyhry
void mainMenu(); //Hlavni menu
#endif
