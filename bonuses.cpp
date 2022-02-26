class Bonus
{
    public:
        int index; //index v poli bonusu
        char * name;  //nazev
        int color; //barvy pro zobrazeni
        char write; //vypisovany znak
        int appearCount; //pocet na mape
        int bornWith; //pocet s kolika je hrac "narozen"
        int override; //nahrazeni poctu
        bool hasOverride; //zda ma nahrazeni
        bool forbidden; //zda je pouzito nahrazeni
        Bonus()
        {
        }
        Bonus(int _index,char * _name,char _write,int _color,int _appearCount,int _bornWith,bool _forbidden=false,int _override=255)
        {
            index=_index;
            name=_name;
            hasOverride=_override!=255;
            override=_override;
            write=_write;
            color=_color;
            forbidden=_forbidden;
            appearCount=_appearCount;
            bornWith=_bornWith;
        }
        int getAppearCount()
        {
            if(hasOverride) 
             return override;
            else
             return appearCount;
        }
};

const int bonusCount=13; //pocet bonusu v nasledujicim poli
Bonus bonusList[bonusCount]; //pole existujicich typu bonusu


