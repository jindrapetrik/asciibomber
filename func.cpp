int MyGetch (void)
{
  int Ret = getch();
  if ((!Ret || Ret == 0xE0) && kbhit()) {
    Ret = 256 + getch();
  };
  return Ret;
}
//Zjisti zda jsou souradnice na mape platne
bool isValidXY(int nx,int ny)
{
    if(nx<0) return false;
    if(ny<0) return false;
    if(nx>=screenWidth) return false;
    if(ny>=screenHeight) return false;
    return true;
}

//Obratit smer
int invertDirection(int direction)
{
   if(direction==0)  return 0;
   if(direction<=2) return direction+2;
    else return direction-2;
}
