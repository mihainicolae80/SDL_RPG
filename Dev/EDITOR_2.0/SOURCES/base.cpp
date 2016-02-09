#include "base.h"
#include <SDL2/SDL_image.h>

// -- VAR --
int ENGINE_displaymode = DISPLAYMODE_FREE;


/// -- CLASSES --
CLS_WINDOWING   WIN_MAIN;


/// --- FUNCTIONS: CLS_WINDOWING ---
    CLS_WINDOWING::CLS_WINDOWING(){
width  = 100;
height = 100;

viewport_x = 100;
viewport_y = 100;

window = NULL;
render = NULL;
}
void CLS_WINDOWING::setTitle( std::string x ){

}
void CLS_WINDOWING::setWindowWidth(int x){
width = x;
}
void CLS_WINDOWING::setWindowHeight(int x){
height = x;
}
int  CLS_WINDOWING::getWindowWidth ( void ){
return width;
}
int  CLS_WINDOWING::getWindowHeight( void ){
return height;
}
Uint32 CLS_WINDOWING::getID(){
return SDL_GetWindowID( window );
}
void CLS_WINDOWING::setWindowFlags( int x ){
window_flags = x;
}
void CLS_WINDOWING::setRenderFlags( int x ){
render_flags = x;
}
int  CLS_WINDOWING::createWindow( void ){

window = SDL_CreateWindow( title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                           width,height,window_flags);

if( window == NULL )
    return true;

return false;
}
int  CLS_WINDOWING::createRender( void ){

render = SDL_CreateRenderer( window, 0 , render_flags );


if( render == NULL )
    return true;

return false;
}


/// --- FUNCTIONS: MISC ---
int  ENGINE_Init(){

    int ERROR = 0;
    int flags = 0;

    /// --- INIT Debug TEXT ---
    DEBUG.init_low();

    /// --- INIT SDL ---
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        DEBUG_OUT_GEN( "CRITICAL ERROR: Failed to load SDL!" );

        return ERROR_CRIT;
    }

    /// --- INIT TTF ---
    if( TTF_Init() == -1 )
    {
        DEBUG_OUT_GEN(  "CRITICAL ERROR: Failed to load TTF!" );
        return ERROR_CRIT;
    }

    ///--- INIT WINDOW + RENDERER ---

    WIN_MAIN.setTitle( "RPG Map Editor" );
    WIN_MAIN.setWindowWidth ( 1024 );
    WIN_MAIN.setWindowHeight( 768 );

    //Flags
    flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

    WIN_MAIN.setWindowFlags( flags );

    WIN_MAIN.setRenderFlags( SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

    if( WIN_MAIN.createWindow() )
    {
        DEBUG_OUT_GEN( "Failed to create WINDOW" );
        return ERROR_CRIT;
    }


    if( WIN_MAIN.createRender() )
    {
        DEBUG_OUT_GEN( "Failed to create RENDERER" );
        return ERROR_CRIT;
    }

    /// --- Debug  INIT_PICTURES ---
    if( DEBUG.init_high() )
    {
        ERROR_ADD( ERROR_LOW );
        //CONSOUT      ( "Faild to load DEBUG graphical elements" );
    }


    /// --- Load FONTS ----
    if( !BAS_LoadFonts() )
    {
        //CONSOUT( "Failed to load some/all FONTS" );
        ERROR_ADD( ERROR_HIGH );
    }



chunk_NULL.nr = -1;
chunk_NULL.orient = 0;
chunk_NULL.rotate = false;
chunk_NULL.skin = NULL;

NULL_FreeChunk = new FreeChunk;
NULL_FreeChunk ->next = NULL;
NULL_FreeChunk ->skin = NULL;

FirstFreeChunk_fore = new FreeChunk;
LastFreeChunk_fore = new FreeChunk;

FirstFreeChunk_fore -> next = LastFreeChunk_fore;
LastFreeChunk_fore -> next = NULL;

FirstFreeChunk_back = new FreeChunk;
LastFreeChunk_back = new FreeChunk;

FirstFreeChunk_back -> next = LastFreeChunk_back;
LastFreeChunk_back -> next = NULL;

FirstFreeChunk_dynam = new FreeChunk;
LastFreeChunk_dynam = new FreeChunk;

FirstFreeChunk_dynam -> next = LastFreeChunk_dynam;
LastFreeChunk_dynam -> next = NULL;

LastChunkBack = FirstFreeChunk_back;
LastChunkFore = FirstFreeChunk_fore;
LastChunkDynam = FirstFreeChunk_dynam;

FirstDNChunk = new DayNightChunk;
LastDNChunk = new DayNightChunk;

FirstDNChunk ->next = LastDNChunk;
LastDNChunk -> next = NULL;

FirstFCLib = new FreeChunk;
C_FreeChunk = new FreeChunk;
C_FreeChunk -> skin = DEBUG.sfExMark;

FirstCOLChunk = new ColideChunk;
LastCOLChunk = new ColideChunk;
FirstCOLChunk -> next = LastCOLChunk;
LastCOLChunk -> next = NULL;
CCOLChunk = NULL;

CDNChunk = NULL;

FirstItr = new InterSpot;
LastItr = new InterSpot;
FirstItr -> next = LastItr;


    /// --- INIT POINTERS ---
    #define init_pointer(first,last,type) first = new type;last = new type; first->next = last;last->next = NULL;

    //init_pointer( FirstFreeChunk_back,LastFreeChunk_back,FreeChunk );
    //init_pointer( FirstFreeChunk_fore,LastFreeChunk_fore,FreeChunk );
    //init_pointer( FirstFreeChunk_dynam,LastFreeChunk_dynam,FreeChunk );

    //init_pointer( FirstLight,LastLight,LIGHT );
    //init_pointer( FirstCol,LastCol,COLLIDER );
    //init_pointer( FirstItr,LastItr,InterSpot );
    //init_pointer( FirstNPC,LastNPC,NotPLayerCreature );
    //init_pointer( FirstEnemy,LastEnemy,NPCList );

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    return ERROR;
}
void ENGINE_handleEvent_resizeWindow( SDL_Event *EVENT ){
int w,h;
    if( EVENT->type         == SDL_WINDOWEVENT &&
        EVENT->window.event == SDL_WINDOWEVENT_RESIZED )
    {
        SDL_GetWindowSize( WIN_MAIN.window, &w, &h );
        WIN_MAIN.setWindowWidth ( w );
        WIN_MAIN.setWindowHeight( h );
    }
}
void LoadingTitle( int percent ){
static SDL_Texture *txLoadTitle,*txLoadBar_empty,*txLoadBar_fill;
static SDL_Rect rLoadBar;
if( percent == 0 )
{
    txLoadTitle = loadTex( "textures//LoadScreen_Title.png" );
    txLoadBar_empty = loadTex( "textures//LoadScreen_bar_empty.png" );
    txLoadBar_fill = loadTex( "textures//LoadScreen_bar_fill.png" );
    rLoadBar.x = 0;
    rLoadBar.y = 0;
    rLoadBar.w = 1;
    rLoadBar.h = 31;
}
else
rLoadBar.w = 547*percent/100;

SDL_SetRenderDrawColor( RENDER_MAIN, 0,0,0,255 );
SDL_RenderClear( RENDER_MAIN );

applyTex( (SCREEN_WIDTH - 600)/2, 300,txLoadTitle );
applyTex( (SCREEN_WIDTH - 537)/2, SCREEN_HEIGHT - 300,txLoadBar_empty );
applyTex( (SCREEN_WIDTH - 537)/2, SCREEN_HEIGHT - 300,txLoadBar_fill,&rLoadBar );
SDL_RenderPresent( RENDER_MAIN );
//SDL_Delay( 300 );
}
void ENGINE_HandleEvent_resizeWindow( SDL_Event *EVENT ){
int w,h;
    if( EVENT->type         == SDL_WINDOWEVENT &&
        EVENT->window.event == SDL_WINDOWEVENT_RESIZED )
    {
        SDL_GetWindowSize( WIN_MAIN.window, &w, &h );
        WIN_MAIN.setWindowWidth ( w );
        WIN_MAIN.setWindowHeight( h );
    }
}
void ENGINE_HandleEvents( SDL_Event *EVENT ){

    //Handle window resize events
    ENGINE_HandleEvent_resizeWindow( EVENT );
}
void ENGINE_Quit(void){


SDL_DestroyRenderer( RENDER_MAIN );
SDL_DestroyWindow( WINDOW_MAIN );

TTF_Quit();
SDL_Quit();
}



SDL_Surface *loadSurf(string file){
SDL_Surface *loadedimg = NULL;

loadedimg = IMG_Load( file.c_str() );

if( loadedimg == NULL )
{
    //DEBUG.fout( FILE_FLF, (string)"load fail: " + file );
    //DEBUG.fout( FILE_FLF, (string)"reason: "    + SDL_GetError() + (string)"\n" );

    DEBUG_OUT_FLF((string)"load fail: " + file);
    DEBUG_OUT_FLF((string)"reason: "    + SDL_GetError() + (string)"\n");

}

return loadedimg;
}
SDL_Texture *loadTex(string file){
SDL_Texture *loadedtex = NULL;

loadedtex = IMG_LoadTexture(WIN_MAIN.render,file.c_str() );

if( loadedtex == NULL )
{
    //DEBUG.fout( FILE_FLF, (string)"load fail: " + file );
    //DEBUG.fout( FILE_FLF, (string)"reason: "    + SDL_GetError() + (string)"\n" );
}

return loadedtex;
}
//Apply Tex
void applyTexFree(int x,int y,SDL_Texture *tex,SDL_Rect *clip ,int w,int h ){

    SDL_Rect pos;

    pos.x = x;
    pos.y = y;

    if( clip != NULL )
    {
        pos.w = clip->w;
        pos.h = clip->h;
    }
    else
    SDL_QueryTexture( tex,NULL,NULL,&pos.w,&pos.h );

    if( w != NO_CHANGE ) pos.w = w;
    if( h != NO_CHANGE ) pos.h = h;

    SDL_RenderCopy( RENDER_MAIN,tex,clip,&pos );
}
void applyTexLetterbox(int x,int y,SDL_Texture *tex,SDL_Rect *clip,int w,int h ){

    SDL_Rect pos;

    pos.y = Y_ABS;

    pos.x = ceil( (double)x * SCALE );
    pos.y += ceil( (double)y * SCALE  );


    if( clip != NULL )
    {
        pos.w = clip->w;
        pos.h = clip->h;
    }
    else
    SDL_QueryTexture( tex,NULL,NULL,&pos.w,&pos.h );

    if( w != NO_CHANGE ) pos.w = w;
    if( h != NO_CHANGE ) pos.h = h;

    //ADJUST
    pos.w = ceil((double)pos.w * SCALE);
    pos.h = ceil((double)pos.h * SCALE);


    SDL_RenderCopy( RENDER_MAIN,tex,clip,&pos );
}
void applyTex(int x,int y,SDL_Texture *tex,SDL_Rect *clip ,int w ,int h){

    //Free Mode
    if( ENGINE_displaymode == DISPLAYMODE_FREE )
    {
        applyTexFree( x, y, tex, clip, w, h );
    }
    //Letterbox Mode
    else
    if( ENGINE_displaymode == DISPLAYMODE_LETTERBOX )
    {
        applyTexLetterbox( x, y, tex, clip , w, h );
    }
}
/**
    Adaugata temporar, va fi eliminata si SDL_surf-urile vor fi inlocite cu tex
*/
void applySurf(int x,int y,SDL_Surface *tex,SDL_Rect *clip,int w,int h){

    if(tex == NULL){

        return;
    }

    SDL_Texture *temp = SDL_CreateTextureFromSurface(RENDER_MAIN, tex);

    applyTex(x,y,temp,clip,w,h);

    SDL_DestroyTexture(temp);
}



//Draw Rect
void DrawRectFree(int x,int y,int x2,int y2){

    SDL_Rect target;

    target.x = x<x2 ? x : x2 ;
    target.y = y<y2 ? y : y2 ;
    target.w = BAS_abs( x - x2 );
    target.h = BAS_abs( y - y2 );

    SDL_RenderDrawRect( RENDER_MAIN, &target );
}
void DrawRectLetterbox(int x,int y,int x2,int y2){

    SDL_Rect target;

    target.y = Y_ABS;

    target.x = (x<x2 ? x : x2)  * SCALE;
    target.y += (y<y2 ? y : y2) * SCALE;
    target.w = BAS_abs( x - x2 ) * SCALE;
    target.h = BAS_abs( y - y2 ) * SCALE;


    SDL_RenderDrawRect( RENDER_MAIN, &target );
}
void DrawRect(int x,int y,int x2,int y2,SDL_Color *color){

    SDL_SetRenderDrawColor( RENDER_MAIN,color->r,color->g,color->b,color->a );

    //If in FREE MODE
    if( ENGINE_displaymode == DISPLAYMODE_FREE )
    DrawRectFree(x,y,x2,y2);

    //If in Letterbox MODE
    else
    if( ENGINE_displaymode == DISPLAYMODE_LETTERBOX )
    DrawRectLetterbox(x,y,x2,y2);
}
//Fill Rect
void FillRectFree(int x,int y,int x2,int y2){

    SDL_Rect target;

    target.x = x<x2 ? x : x2 ;
    target.y = y<y2 ? y : y2 ;
    target.w = BAS_abs( x - x2 );
    target.h = BAS_abs( y - y2 );

    SDL_RenderFillRect( RENDER_MAIN, &target );
}
void FillRectLetterbox(int x,int y,int x2,int y2){

    SDL_Rect target;

    target.y = Y_ABS;

    target.x = (x<x2 ? x : x2)  * SCALE ;
    target.y += (y<y2 ? y : y2) * SCALE ;
    target.w = BAS_abs( x - x2 ) * SCALE ;
    target.h = BAS_abs( y - y2 ) * SCALE;


    SDL_RenderFillRect( RENDER_MAIN, &target );
}
void FillRect(int x,int y,int x2,int y2,SDL_Color *color){

    SDL_SetRenderDrawColor( RENDER_MAIN,color->r,color->g,color->b,color->a );

    //If in FREE MODE
    if( ENGINE_displaymode == DISPLAYMODE_FREE )
    FillRectFree(x,y,x2,y2);

    //If in Letterbox MODE
    else
    if( ENGINE_displaymode == DISPLAYMODE_LETTERBOX )
    FillRectLetterbox(x,y,x2,y2);
}


void ENGINE_RenderPresent( void){

    SDL_Rect blackline1,blackline2;

    /// -- BLACKLINES --
    if( ENGINE_displaymode == DISPLAYMODE_LETTERBOX )
    {
        //--- INIT BLACKLINES ---
        blackline1.x = 0;
        blackline1.y = 0;
        blackline2.x = 0;
        blackline2.y = Y_ABS + REAL_SCREEN_HEIGHT;
        blackline1.w = WIN_MAIN.getWindowWidth();
        blackline1.h = (WIN_MAIN.getWindowHeight()-REAL_SCREEN_HEIGHT)/2;
        blackline2.w = WIN_MAIN.getWindowWidth();
        blackline2.h = (WIN_MAIN.getWindowHeight()-REAL_SCREEN_HEIGHT)/2;

        //Draw Black Lines
        SDL_SetRenderDrawColor( RENDER_MAIN, 0,0,0,255 );
        SDL_RenderFillRect( RENDER_MAIN, &blackline1 );
        SDL_RenderFillRect( RENDER_MAIN, &blackline2 );
    }


    SDL_RenderPresent( RENDER_MAIN );
}



void AddChunk( FreeChunk *modelchunk,int x, int y,int list ){
FreeChunk *newchunk = new FreeChunk;
newchunk -> nr = modelchunk ->nr;
newchunk -> orient = modelchunk -> orient;

newchunk -> x = x;
newchunk -> y = y;
newchunk->frames = modelchunk->frames;
newchunk->animated = modelchunk->animated;
newchunk->cframe = 0;

switch( modelchunk -> orient ) //chose / render skin
{
    case 0:
    newchunk -> skin = modelchunk -> skin;
    break;
    case 1:
    newchunk -> skin = PIXELS.rotate_img( modelchunk -> skin ,ROTATE_RIGHT,newchunk->animated,newchunk->frames ) ;
    break;
    case 2:
    newchunk -> skin = PIXELS.flip_img( modelchunk -> skin , FLIP_HOR ) ;
    break;
    case 3:
    newchunk -> skin = PIXELS.rotate_img( modelchunk -> skin, ROTATE_LEFT,newchunk->animated,newchunk->frames ) ;
    break;
}


if( list == FreeBG )
{
    newchunk->next = LastChunkBack->next;
    LastChunkBack->next = newchunk;
    LastChunkBack = newchunk;
}
else if( list == FreeFG )
{
    newchunk->next = LastChunkFore->next;
    LastChunkFore->next = newchunk;
    LastChunkFore = newchunk;
}
else if( list == FreeDynam )
{
    newchunk->next = LastChunkDynam->next;
    LastChunkDynam->next = newchunk;
    LastChunkDynam = newchunk;
}


if( newchunk -> nr == DOOR_NR )
{
    newchunk ->info[0] = InterInfo[0];
    newchunk ->info[1] = InterInfo[1];
    newchunk ->info[2] = InterInfo[2];
}

}
DayNightChunk *AddLight( int x, int y,int nr, int range ){
DayNightChunk *newchunk = new DayNightChunk;
newchunk ->x = x;
newchunk ->y = y;
newchunk ->range = range;
newchunk ->nr = nr;

newchunk -> next = FirstDNChunk ->next;
FirstDNChunk -> next = newchunk;
return newchunk;
}
ColideChunk *AddColider( int x,int y, int w,int h,int id ){
ColideChunk *newchunk = new ColideChunk;
newchunk ->x = x;
newchunk ->y = y;
newchunk ->w = w;
newchunk ->h = h;
newchunk ->id = id;
newchunk ->g1x = x - w/2;
newchunk ->g1y = y - h/2;
newchunk ->g2x = x + w/2;
newchunk ->g2y = y + h/2;

newchunk ->skin = PIXELS.RenderRect(w,h,0);

newchunk -> next = FirstCOLChunk ->next;
FirstCOLChunk -> next = newchunk;
return newchunk;
}
InterSpot *AddInterSpot( int x,int y ,int w,int h,int type,int i1,int i2,int i3,int i4,int id){
InterSpot *nou = new InterSpot;
nou -> next = FirstItr -> next;
FirstItr -> next = nou;
nou -> x = x;
nou -> y = y;
nou -> w = w;
nou -> h = h;
nou -> type = type;
nou -> id      = id;
nou -> info[0] = i1;
nou -> info[1] = i2;
nou -> info[2] = i3;
nou -> info[3] = i4;
nou -> skin = PIXELS.RenderRect( nou -> w,nou -> h ,0 );
return nou;
}
InterSpot *AddInterSpot( int x,int y ){
InterSpot *nou = new InterSpot;
nou -> next = FirstItr -> next;
FirstItr -> next = nou;
nou -> x = x;
nou -> y = y;
nou -> w = nou -> h = 60;
nou -> type = 0;
nou -> id = 0;
nou -> info[0] = 0;
nou -> info[1] = 0;
nou -> info[2] = 0;
nou -> info[3] = 0;
nou -> skin = PIXELS.RenderRect( nou -> w,nou -> h ,0 );
return nou;
}
NPCchunk *AddNPCchunk( NPCchunk *modelchunk ){
NPCchunk *newchunk = new NPCchunk;

newchunk->skin = modelchunk->skin;
newchunk->x = modelchunk->x;
newchunk->y = modelchunk->y;
newchunk->w = modelchunk->w;
newchunk->h = modelchunk->h;
newchunk->Name = modelchunk->Name;
newchunk->Faction = "NONE";
newchunk->Behavior = "NONE";
newchunk->Relationship = 1;
newchunk->CanTalk = true;
newchunk->filenr = modelchunk->filenr;
newchunk->skinnr = modelchunk->skinnr;
newchunk->next = FirstNPCMap->next;
FirstNPCMap->next = newchunk;

return newchunk;
}


void ctos(char carray[],string &str ){

    str.erase(0, str.length());
    for( int i = 0; carray[i] != 0; i++ )
    str += carray[i];
}

//Fonts
TTF_Font *Font14,*Font18,*Font20,*Font24,*Font2_14,*Font2_18,*Font2_20,*Font2_24;

//COLORS
SDL_Color WHITE        = {255,255,255,255};
SDL_Color BLACK        = {0,0,0};
SDL_Color RED          = {255,21,5};
SDL_Color BLUE         = {0,0,255};
SDL_Color GREEN        = {93,255,61};
SDL_Color YELLOW_LIGHT = {255, 240, 168 };

//Global Svariables

int NrBlockX, NrBlockY;

int cDayTime,iMoveMap_x,iMoveMap_y = 50,iInitial_x,iInitial_y,FCMove_x,FCMove_y,maxx,maxy,cInterItm,InterInfo[3];
bool bCtrlDown,bMouseDrag, bCSkinOrigin = false,FCmoveing = false,LightMap_Changed[6],LibLock[3];

