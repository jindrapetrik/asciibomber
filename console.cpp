#include <windows.h>
#include <iostream>
using namespace std;

//Vymaze obrazovku
void clrscr()
{
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);  
  SetConsoleTextAttribute(hStdOut, 15);
  system("cls");
  return;
}

//Nastavi pozici kurzoru
void setPos(int nx,int ny)
{
   HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);  
   COORD coord={nx,ny};
   SetConsoleCursorPosition(hStdOut, coord); 
}

//Nastavi barvy konzole
void setColors(int color)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, color);
}

//Vykresli retezec na urcite pozici
void drawStringAt(char c[],int nx,int ny,int colors)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, colors);
    COORD coord={nx,ny};
    SetConsoleCursorPosition(hStdOut, coord);                          
    cout<<c;
}

//Vybresli znak na pozici
void drawCharAt(char c,int nx,int ny,int colors)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, colors);
    COORD coord={nx,ny};
    SetConsoleCursorPosition(hStdOut, coord);                          
    cout<<c;
}


//Zobrazi grafiku ve JSC formatu
void showJSC(char *filename,int colorize=5)
{
    clrscr();
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    
    FILE *f=fopen(filename,"rb");
    char scrColors[79][50];
    char scrChars[79][50];
    fread(scrColors,sizeof(scrColors),1,f);
    fread(scrChars,sizeof(scrChars),1,f);
    fclose(f);
    for(int y=0;y<50;y++)
    {
     for(int x=0;x<79;x++)
      {
            int color=(int)scrColors[x][y];
            
            if(color<0) color+=256;
            int backColor=color / 16;
            int textColor=color%16;
            if(backColor==5) backColor=colorize;
            if(textColor==5) textColor=colorize;
            color=backColor*16+textColor;
            SetConsoleTextAttribute(hStdOut, color);
            cout<<scrChars[x][y];
      }
      if(y!=49) printf("\n");
    }
    
}

//Program pokracuje po stisknuti enteru:
void enterToContinue()
{
  while(GetAsyncKeyState(VK_RETURN)>=0);
  while(GetAsyncKeyState(VK_RETURN)<0);
}

//Vykresli ramecek
void drawFrame()
{
     COORD coord = {startX-1, startY-1};
     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     SetConsoleTextAttribute(hStdOut, patterns[patternIndex].frameColors);
     SetConsoleCursorPosition(hStdOut, coord);
     cout<<char(218);
     for(int p=0;p<screenWidth;p++)
      cout<<char(196);
     cout<<char(191);
    
     for(int p=0;p<screenHeight;p++)
      {
        coord.Y=startY+1+p-1;
        coord.X=startX-1;
        SetConsoleCursorPosition(hStdOut, coord);
        cout<<char(179);
        coord.X=startX+screenWidth+1-1;
        SetConsoleCursorPosition(hStdOut, coord);     
        cout<<char(179);
      }
     
     coord.Y=startY+screenHeight;
     coord.X=startX-1;
     SetConsoleCursorPosition(hStdOut, coord);    
     cout<<char(192);
     for(int p=0;p<screenWidth;p++)
      cout<<char(196);     
     cout<<char(217);
     
}


//Vykresli dvojity ramecek
void drawDoubleFrame()
{
     COORD coord = {startX-1, startY-1};
     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     SetConsoleTextAttribute(hStdOut, 32);
     SetConsoleCursorPosition(hStdOut, coord);
     cout<<char(201);
     for(int p=0;p<screenWidth;p++)
      cout<<char(205);
     cout<<char(187);
    
     for(int p=0;p<screenHeight;p++)
      {
        coord.Y=startY+1+p-1;
        coord.X=startX-1;
        SetConsoleCursorPosition(hStdOut, coord);
        cout<<char(186);
        coord.X=startX+screenWidth+1-1;
        SetConsoleCursorPosition(hStdOut, coord);     
        cout<<char(186);
      }
     
     coord.Y=startY+screenHeight;
     coord.X=startX-1;
     SetConsoleCursorPosition(hStdOut, coord);    
     cout<<char(200);
     for(int p=0;p<screenWidth;p++)
      cout<<char(205);     
     cout<<char(188);
     
}


//Prepne do Fullscreen modu
void FullScreen()
{
    keybd_event(VK_MENU,
                0x38,
                0,
                0);
    keybd_event(VK_RETURN,
                0x1c,
                0,
                0);
    keybd_event(VK_RETURN,
                0x1c,
                KEYEVENTF_KEYUP,
                0);
    keybd_event(VK_MENU,
                0x38,
                KEYEVENTF_KEYUP,
                0);
    return;
}

//Vykresli znak na mape na pozici
void drawCharAtMapAt(char c,int nx,int ny,int colors)
{
    drawCharAt(c,startX+nx,startY+ny,colors);
}

//Skryti kurzozu:
CONSOLE_CURSOR_INFO lastCCInfo; 
void hideCursor()
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hStdOut,&lastCCInfo);
    CONSOLE_CURSOR_INFO newCCInfo;
    newCCInfo.dwSize=lastCCInfo.dwSize;
    newCCInfo.bVisible=false;
    SetConsoleCursorInfo(hStdOut,&newCCInfo);
}

//Zpetne zobrazeni kurzoru:
void showCursor()
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorInfo(hStdOut,&lastCCInfo);
}
