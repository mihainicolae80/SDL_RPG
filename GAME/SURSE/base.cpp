#include "base.h"
#include "debug.h"
#include "settings.h"
#include "main_headers.h"
#include "basics.h"
#include "gevents.h"
#include "pixel_manip.h"
#include "data.h"
#include "anim.h"
#include "combat.h"
#include "npc.h"
#include "map.h"
#include "interface.h"
#include "quests.h"
#include "dialog.h"
#include "inventory.h"
#include "loot.h"
#include "scripts.h"
#include "player.h"
#include "misc.h"
#include "console_extern.h"

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


///--- INIT CONSOLE ---
if( CONSOLE.init() )
{
    DEBUG_OUT_GEN( "Failed to init CONSOLE" );
    return ERROR_CRIT;
}


///--- INIT WINDOW + RENDERER ---
WIN_MAIN.setTitle( "RPG CONSOLE" );
WIN_MAIN.setWindowWidth ( SCREEN_WIDTH_CONS );
WIN_MAIN.setWindowHeight( SCREEN_HEIGHT_CONS );
WIN_MAIN.setWindowFlags( 0 );


WIN_MAIN.setTitle( "RPG GAME" );
WIN_MAIN.setWindowWidth ( SETTINGS.getScreenWidth() );
WIN_MAIN.setWindowHeight( SETTINGS.getScreenHeight() );
WIN_MAIN.setWindowFlags( SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );

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
    CONSOUT      ( "Faild to load DEBUG graphical elements" );
}


LoadingTitle( 0 );

/// --- Load FONTS ----
if( !BAS_LoadFonts() )
{
    CONSOUT( "Failed to load some/all FONTS" );
    ERROR_ADD( ERROR_HIGH );
}


LoadingTitle( 5 );


/// --- INIT POINTERS ---
#define init_pointer(first,last,type) first = new type;last = new type; first->next = last;last->next = NULL;

init_pointer( FirstFreeChunk_back,LastFreeChunk_back,FreeChunk );
init_pointer( FirstFreeChunk_fore,LastFreeChunk_fore,FreeChunk );
init_pointer( FirstFreeChunk_dynam,LastFreeChunk_dynam,FreeChunk );

init_pointer( FirstLight,LastLight,LIGHT );
init_pointer( FirstCol,LastCol,COLLIDER );
init_pointer( FirstItr,LastItr,InterSpot );
init_pointer( FirstNPC,LastNPC,NotPLayerCreature );
init_pointer( FirstEnemy,LastEnemy,NPCList );

init_pointer( FirstConsText,LastConsText,CONSOLE_TEXT );


SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");


LoadingTitle( 6 );

CONSOLE.out( "Init Pointers-OK" );

GAME_EVENTS .init();
CONSOLE.out( "Init Game_Event-OK" );    LoadingTitle( 15 );
Pixels      .init();
CONSOLE.out( "Init PixelManip-OK" );    LoadingTitle( 20 );
DATABASE    .init();
CONSOLE.out( "Init Database-OK" );      LoadingTitle( 25 );
ALLANIMES   .init();
CONSOLE.out( "Init Anim-OK" );          LoadingTitle( 30 );
COMBAT      .init();
CONSOLE.out( "Init Combat-OK" );        LoadingTitle( 35 );
GAME_NPCS   .init();
CONSOLE.out( "Init NPC-OK" );           LoadingTitle( 40 );
GAME_MAP    .init();
CONSOLE.out( "Init Map-OK" );           LoadingTitle( 55 );
cInterface  .init();
CONSOLE.out( "Init Interface-OK" );     LoadingTitle( 60 );
QUEST       .init();
CONSOLE.out( "Init Quests-OK" );        LoadingTitle( 70 );
DIALOG      .init();
CONSOLE.out( "Init Dialog-OK" );        LoadingTitle( 75 );
INVENTORY   .init();
CONSOLE.out( "Init Inventory-OK" );     LoadingTitle( 85 );
PLAYER      .init();
CONSOLE.out( "Init PLAYER-OK" );        LoadingTitle( 95 );

LOOT.init();

CONSOLE.out( "INITIALIZATION FINISED:" );

stringstream sserror;
sserror << "ERROR LEVEL: " << ERROR;

CONSOLE.out( sserror.str() );


LoadingTitle( 100 );

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
void ENGINE_Cleanup(){
INVENTORY.cleanup();
cInterface.cleanup();
ALLANIMES.cleanup();

SDL_DestroyRenderer( RENDER_MAIN );
SDL_DestroyRenderer( RENDER_CONSOLE );
SDL_DestroyWindow( WINDOW_MAIN );
SDL_DestroyWindow( WINDOW_CONSOLE );
TTF_Quit();
SDL_Quit();
}
void LoadingTitle( int percent ){
static SDL_Texture *txLoadTitle,*txLoadBar_empty,*txLoadBar_fill;
static SDL_Rect rLoadBar;
if( percent == 0 )
{
    txLoadTitle = LoadTex( "textures//LoadScreen_Title.png" );
    txLoadBar_empty = LoadTex( "textures//LoadScreen_bar_empty.png" );
    txLoadBar_fill = LoadTex( "textures//LoadScreen_bar_fill.png" );
    rLoadBar.x = 0;
    rLoadBar.y = 0;
    rLoadBar.w = 1;
    rLoadBar.h = 31;
}
else
rLoadBar.w = 547*percent/100;

SDL_SetRenderDrawColor( RENDER_MAIN, 0,0,0,255 );
SDL_RenderClear( RENDER_MAIN );

ApplyTex( (BASE_SCREEN_WIDTH - 600)/2, 300,txLoadTitle );
ApplyTex( (BASE_SCREEN_WIDTH - 537)/2, BASE_SCREEN_HEIGHT - 300,txLoadBar_empty );
ApplyTex( (BASE_SCREEN_WIDTH - 537)/2, BASE_SCREEN_HEIGHT - 300,txLoadBar_fill,&rLoadBar );
SDL_RenderPresent( RENDER_MAIN );
//SDL_Delay( 300 );
}


/// --- SHALL DISSAPEAR! ---

///CLASSES
Map GAME_MAP;
Quest QUEST;
DataBase DATABASE;
PixelManipulation Pixels;
GEvents GAME_EVENTS;
inventory INVENTORY;
NPCs GAME_NPCS;
Anime ALLANIMES;
Combat COMBAT;
Interface cInterface;
Dialog DIALOG;
Loot LOOT;
Script SCRIPT;
CLS_PLAYER PLAYER;
CLS_CONSOLE CONSOLE;
CLS_CONTROL CONTROL;

///STRUCTS

NPCList *FirstEnemy, *LastEnemy;
InterSpot *FirstItr,*LastItr;
COLLIDER *FirstCol,*LastCol;
CONSOLE_TEXT *FirstConsText,*LastConsText;
FreeChunk *FirstFreeChunk_back,*FirstFreeChunk_fore,*FirstFreeChunk_dynam,
          *LastFreeChunk_back ,*LastFreeChunk_fore ,*LastFreeChunk_dynam;
LIGHT *FirstLight, *LastLight;
NotPLayerCreature *FirstNPC,*LastNPC;
newQuest ACTIVE_QUEST[10],quest_NULL;

///BASIC
SDL_Event event;
GAME_event newGEvent,Gevent_NULL;

//Fonts
TTF_Font *Font14,*Font18,*Font20,*Font24,*Font2_14,*Font2_18,*Font2_20,*Font2_24;

//COLORS
SDL_Color WHITE        = {255,255,255};
SDL_Color BLACK        = {0,0,0};
SDL_Color RED          = {255,21,5};
SDL_Color BLUE         = {0,0,255};
SDL_Color GREEN        = {93,255,61};
SDL_Color YELLOW_LIGHT = {255, 240, 168 };

//Global Svariables

int NrBlockX, NrBlockY;


