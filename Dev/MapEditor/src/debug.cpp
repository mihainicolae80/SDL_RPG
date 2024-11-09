#include "debug.h"

using namespace std;

//Declaration
CLS_DEBUG DEBUG;

void CLS_DEBUG::init_low(){

//ClEAN UP OLD FILES
ofFlf.open( "DEBUG//FileLoadFails.txt" );
ofFlf.close();

ofGeneral.open( "DEBUG//general_errors.txt" );
ofGeneral.close();
ofConsOut.open( "DEBUG//CONSOLE OUTPUT.txt" );
ofConsOut.close();
drawcolrects = true;
}
int  CLS_DEBUG::init_high(){
int ERROR = 0;


//init picture
sfExMark = nullptr;
txExMark = nullptr;

//Surf
sfExMark = loadSurf( "textures//debug//ExcMark.png" );
if( sfExMark == nullptr )
{
    CLS_DEBUG::fout( FILE_FLF, "Couldn't load ExMark surface" );
    ERROR =  true;
}

//Texture
txExMark = loadTex( "textures//debug//ExcMark.png" );
if( txExMark == nullptr )
{
    CLS_DEBUG::fout( FILE_FLF, "Couldn't load ExMark surface" );
    ERROR = true;
}

return ERROR;
}


CLS_DEBUG::~CLS_DEBUG() {

    //Close FILES
    if( ofFlf.is_open() )
    ofFlf.close();

    if( ofGeneral.is_open() )
    ofGeneral.close();

    SDL_FreeSurface   ( sfExMark  );
    SDL_DestroyTexture( txExMark );
}
void        CLS_DEBUG::fout( int file_out, string text ){

if( file_out == FILE_FLF )
{
    ofFlf.open( "DEBUG//FileLoadFails.txt", ios_base::app );
    if( ofFlf.is_open() )
    {
        ofFlf << text << '\n';
        ofFlf.close();
    }
}
else
if( file_out == FILE_GENERAL )
{
    ofGeneral.open( "DEBUG//general_errors.txt", ios_base::app );
    if( ofGeneral.is_open() )
    {
        ofGeneral << text << '\n';
        ofGeneral.close();
    }
}
else
if( file_out == FILE_CONSOUT )
{
    ofConsOut.open( "DEBUG//CONSOLE OUTPUT.txt", ios_base::app );
    if( ofConsOut.is_open() )
    {
        ofConsOut << text << '\n';
        ofConsOut.close();
    }
}
}
SDL_Surface *CLS_DEBUG::getNewsfExc(void){
SDL_Surface *temp;
temp = loadSurf( "textures//debug//ExcMark.png" );
return temp;
}

void        CLS_DEBUG::setDrawCollRects(bool x){
drawcolrects = x;
}
