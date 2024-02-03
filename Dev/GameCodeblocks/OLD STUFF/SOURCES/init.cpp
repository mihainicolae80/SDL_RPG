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



bool init(){

//Debug  INIT_TEXT
DEBUG.init_low();

//SDL
if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) return false;
//TTF
if( TTF_Init() == -1 ) return false;



WINDOW_CONSOLE = SDL_CreateWindow( "RPG CONSOLE",1300,SCREEN_HEIGHT-SCREEN_HEIGHT_CONS,
                                SCREEN_WIDTH_CONS,SCREEN_HEIGHT_CONS,0 );
if( WINDOW_CONSOLE == NULL ) return false;

RENDERER_CONSOLE = SDL_CreateRenderer( WINDOW_CONSOLE,-1, SDL_RENDERER_PRESENTVSYNC );
if( RENDERER_CONSOLE == NULL ) return false;

WINDOW_MAIN = SDL_CreateWindow( "RPG GAME",100,SDL_WINDOWPOS_CENTERED,
                                SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS );// | SDL_WINDOW_FULLSCREEN
if( WINDOW_MAIN == NULL ) return false;

RENDER_MAIN = SDL_CreateRenderer( WINDOW_MAIN,-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );//
if( RENDER_MAIN == NULL ) return false;

//Debug  INIT_PICTURES
DEBUG.init_high();

WINDOW_CONSOLE_ID = SDL_GetWindowID( WINDOW_CONSOLE );


LoadingTitle( 0 );


Font14 = TTF_OpenFont("textures//fonts//font_knight.ttf" ,14 );
Font18 = TTF_OpenFont("textures//fonts//font_knight.ttf" ,18 );
Font20 = TTF_OpenFont("textures//fonts//font_knight.ttf" ,20 );
Font24 = TTF_OpenFont("textures//fonts//font_knight.ttf" ,24 );

Font2_14 = TTF_OpenFont("textures//fonts//Amble-Regular.ttf" ,14 );
Font2_18 = TTF_OpenFont("textures//fonts//Amble-Regular.ttf" ,18 );
Font2_20 = TTF_OpenFont("textures//fonts//Amble-Regular.ttf" ,20 );
Font2_24 = TTF_OpenFont("textures//fonts//Amble-Regular.ttf" ,24 );

LoadingTitle( 5 );

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


CONSOLE.out( "Init Successfull" );

LoadingTitle( 100 );

return true;
}
void cleanup(){
INVENTORY.cleanup();
cInterface.cleanup();
ALLANIMES.cleanup();

SDL_DestroyRenderer( RENDER_MAIN );
SDL_DestroyRenderer( RENDERER_CONSOLE );
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

ApplyTex( (SCREEN_WIDTH - 600)/2, 300,txLoadTitle );
ApplyTex( (SCREEN_WIDTH - 537)/2, SCREEN_HEIGHT - 300,txLoadBar_empty );
ApplyTex( (SCREEN_WIDTH - 537)/2, SCREEN_HEIGHT - 300,txLoadBar_fill,&rLoadBar );
SDL_RenderPresent( RENDER_MAIN );
//SDL_Delay( 300 );
}

///WINDOWING
SDL_Window   *WINDOW_CONSOLE;
SDL_Renderer *RENDERER_CONSOLE;
unsigned int WINDOW_CONSOLE_ID;

///CLASSES
CLS_DEBUG DEBUG;
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



