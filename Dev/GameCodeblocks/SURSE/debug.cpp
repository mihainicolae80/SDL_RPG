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
sfExMark = NULL;
txExMark = NULL;

//Surf
sfExMark = LoadSurf( "textures//debug//ExcMark.png" );
if( sfExMark == NULL )
{
    CLS_DEBUG::fout( FILE_FLF, "Couldn't load ExMark surface" );
    ERROR =  true;
}

//Texture
txExMark = LoadTex( "textures//debug//ExcMark.png" );
if( txExMark == NULL )
{
    CLS_DEBUG::fout( FILE_FLF, "Couldn't load ExMark surface" );
    ERROR = true;
}

return ERROR;
}


            CLS_DEBUG::~CLS_DEBUG(){

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
temp = LoadSurf( "textures//debug//ExcMark.png" );
return temp;
}
void        CLS_DEBUG::DrawCollRects(){
SDL_Rect rAux;
COLLIDER *icol;

SDL_SetRenderDrawBlendMode( RENDER_MAIN, SDL_BLENDMODE_BLEND );

SDL_SetRenderDrawColor( RENDER_MAIN,80,80,80,100 );

rAux.x = PLAYER._PhysicalBody.x + GAME_MAP.DisplayLocation_x;
rAux.y = PLAYER._PhysicalBody.y + GAME_MAP.DisplayLocation_y;
rAux.w = PLAYER._PhysicalBody.w;
rAux.h = PLAYER._PhysicalBody.h;
SDL_RenderFillRect( RENDER_MAIN, &rAux );
for( icol = FirstCol->next ;icol != LastCol;icol = icol->next ){
    rAux.x = icol->x + GAME_MAP.DisplayLocation_x;
    rAux.y = icol->y + GAME_MAP.DisplayLocation_y;
    rAux.w = icol->w;
    rAux.h = icol->h;

    if( icol->ON )
    SDL_SetRenderDrawColor( RENDER_MAIN,150,230,255,100 );
    else
    SDL_SetRenderDrawColor( RENDER_MAIN,230,180,20,100 );

    SDL_RenderFillRect( RENDER_MAIN, &rAux );
}

}
void        CLS_DEBUG::setDrawCollRects(bool x){
drawcolrects = x;
}
