#ifndef __BOMBS_H__
#define __BOMBS_H__

class Bomb
{
      public:
             int x,y;
             bool exploded; //Zda jiz bomba explodovala
             int fuse; //Delka zapalne snury
             int owner; //Cislo hrace, ktery bombu polozil
             int flameLength; //Delka plamene
             int moveDirection; //Smer pohybu (po nakopnuti)
             int moveDelay; //Doba mezi pohybem
             int moveTime; //Zbyvajici pocet tahu do dalsiho pohybu
             bool trigerred; //Zda je bomba dalkove odpalovatelna
             bool jelly; //Zele bomba (viz bonusy)
             Bomb()
             {
             }
             Bomb(int nx,int ny,int _owner,int _flameLength,bool _trigerred,bool _jelly);
             
             
             void explode();
             
             
             void finishMove();
             
             //Operace pro tah bomby
             void doTurn()
             {
                  if(exploded) return;
                  if(moveDirection!=MOV_NONE)
                  {
                        if(moveTime>0)
                        {
                            moveTime--;
                            if(moveTime==0)
                            {
                                finishMove();
                            }
                        }
                  }
                  if(trigerred) return;
                  
                  if(fuse>0) 
                   fuse--; 
                  else
                   return;                  
                  if(fuse==0)
                  {                                                
                     explode();
                  }
             }
};


//Polozka spojoveho seznamu bomb
class BombItem
{
    public:        
        Bomb bomb;
        BombItem *next;    
        BombItem *prev; 
        BombItem():next(NULL),prev(NULL){}    
        BombItem(const Bomb& nbomb):
            bomb(nbomb),next(NULL),prev(NULL) {}
};

//Spojovy seznam bomb
class BombList
{
    public:
        BombItem *head;
        BombItem *tail;
        BombItem *current;
        BombList():head(NULL),tail(NULL),current(NULL) {}

        
        void next()
        {
            if(!head) return;
            if(current==NULL) return;
             current=current->next;

        }
        
        void reset()
        {
            current=head;
        }
        
        void clear()
        {            
            head=NULL;
            tail=NULL;
            current=NULL;
        }
        
        void add(const Bomb& bomb)
        {

                BombItem *newItem=new BombItem();
                newItem->bomb=bomb;
                if(head==NULL)
                {
                    head=newItem;
                    current=head;
                }
                else
                  tail->next=newItem;
                newItem->prev=tail;
                tail=newItem;                      
        }
        
        
        
        void remove(BombItem *bi)
        {
             
            if(bi->next)
             {
              if(bi->prev) 
               {
                 bi->prev->next=bi->next;
                 bi->next->prev=bi->prev;
                 if(current==bi) current=bi->prev;
               }
               else
               {                  
                    head=bi->next;
                    head->prev=NULL;
                    if(current==bi) current=head;
                    delete(bi);
                    bi=NULL;
               }
             }
             else
             {
                   if(bi->prev) 
                   {
                        if(current==bi) current=bi->prev;
                        tail=bi->prev;
                        bi->prev->next=NULL;            
                        delete(bi);
                        bi=NULL;            
                   }
                   else
                   {
                        delete(head);
                        current=NULL;                        
                        tail=NULL;                        
                        head=NULL;
                   }
             }
        }
        
};

BombList blist;
Bomb* findBombAt(int bx,int by);
#endif
