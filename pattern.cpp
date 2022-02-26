//Graficke schema
class Pattern
{
    public:
        char charStone; //znak kamene
        char charWall; //znak zdi
        char charNothing; //znak prazdne
        int backColor; //barva pozadi
        int colorStone; //barva kamene
        int colorWall; //barva zdi
        int colorNothing; //barva pro prazdne pole
        char *name; //nazev schemata
        int frameColors; //barva ramecku mapy
        Pattern(){}
        Pattern(int _frameColors,int _backColor,
                int _colorStone,int _colorWall,int _colorNothing,
                char _charStone,char _charWall,char _charNothing,char *_name)
        {
            backColor=_backColor;
            charStone=_charStone;
            charWall=_charWall;
            frameColors=_frameColors;
            name=_name;
            charNothing=_charNothing;
            colorStone=_colorStone;
            colorWall=_colorWall;
            colorNothing=_colorNothing;
        }
};

const int patternCount=5; //pocet schemat
Pattern patterns[patternCount];
int patternIndex=0;


