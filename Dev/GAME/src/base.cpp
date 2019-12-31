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
#include "console.h"


#define init_pointer(first,last,type) first = new type;last = new type; first->next = last;last->next = NULL;

// -- VAR --
int ENGINE_displaymode = DISPLAYMODE_LETTERBOX;


/// -- CLASSES --
CLS_WINDOWING   WIN_MAIN;


/// --- FUNCTIONS: CLS_WINDOWING ---
CLS_WINDOWING::CLS_WINDOWING()
{
	width  = 100;
	height = 100;

	viewport_x = 100;
	viewport_y = 100;

	window = nullptr;
	render = nullptr;
}


void CLS_WINDOWING::setTitle(std::string x)
{

}


void CLS_WINDOWING::setWindowWidth(int x)
{
	width = x;
}


void CLS_WINDOWING::setWindowHeight(int x)
{
	height = x;
}


int  CLS_WINDOWING::getWindowWidth ( void )
{
	return width;
}


int  CLS_WINDOWING::getWindowHeight( void )
{
	return height;
}


Uint32 CLS_WINDOWING::getID()
{
	return SDL_GetWindowID( window );
}


void CLS_WINDOWING::setWindowFlags(int x)
{
	window_flags = x;
}


void CLS_WINDOWING::setRenderFlags(int x)
{
	render_flags = x;
}

int  CLS_WINDOWING::createWindow( void )
{
	window = SDL_CreateWindow(
				title.c_str(),
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				width,height,window_flags
	);

	if( window == nullptr) {
		return true;
	}

	return false;
}


bool  CLS_WINDOWING::createRender(void)
{
	render = SDL_CreateRenderer(window, 0, render_flags);

	return (render == nullptr);
}


int  ENGINE_Init()
{
    int ERROR = 0;
    int flags = 0;


	// INIT Debug TEXT
    DEBUG.init_low();

	// INIT SDL
	if(-1 == SDL_Init(SDL_INIT_EVERYTHING)) {
		DEBUG_OUT_GEN("CRITICAL ERROR: Failed to load SDL!");
        return ERROR_CRIT;
    }

	// INIT TTF
	if(-1 == TTF_Init()) {
		DEBUG_OUT_GEN("CRITICAL ERROR: Failed to load TTF!");
        return ERROR_CRIT;
    }

    /*
    ///--- INIT //CONSOLE ---
    if( //CONSOLE.init() )
    {
        DEBUG_OUT_GEN( "Failed to init //CONSOLE" );
        return ERROR_CRIT;
    }
    */

	// Init Window + Renderer
	WIN_MAIN.setTitle("RPG GAME");
	WIN_MAIN.setWindowWidth(SETTINGS.getScreenWidth());
	WIN_MAIN.setWindowHeight(SETTINGS.getScreenHeight());

    //Flags
    flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
	if(SETTINGS.getFullScreen()) {
		flags |= SDL_WINDOW_FULLSCREEN;
	}

	WIN_MAIN.setWindowFlags(flags);
	WIN_MAIN.setRenderFlags(SDL_RENDERER_ACCELERATED); // | SDL_RENDERER_PRESENTVSYNC

	if(WIN_MAIN.createWindow()) {
		DEBUG_OUT_GEN("Failed to create WINDOW");
        return ERROR_CRIT;
    }

	if(WIN_MAIN.createRender()) {
        DEBUG_OUT_GEN( "Failed to create RENDERER" );
        return ERROR_CRIT;
    }

    // -- INIT CONSOLE --
    CONS.init();

	// Debug  INIT_PICTURES
	if(DEBUG.init_high()) {
        ERROR_ADD( ERROR_LOW );
        //CONSOUT      ( "Faild to load DEBUG graphical elements" );
    }

	LoadingTitle(0);
	// load fonts
	if(!BAS_LoadFonts()) {
        //CONSOUT( "Failed to load some/all FONTS" );
        ERROR_ADD( ERROR_HIGH );
    }

    LoadingTitle( 5 );

	// init pointers
	init_pointer(FirstFreeChunk_back, LastFreeChunk_back, FreeChunk)
	init_pointer(FirstFreeChunk_fore, LastFreeChunk_fore, FreeChunk)
	init_pointer(FirstFreeChunk_dynam, LastFreeChunk_dynam, FreeChunk)

	init_pointer(FirstLight, LastLight, LIGHT)
	init_pointer(FirstCol, LastCol, COLLIDER)
	init_pointer(FirstItr, LastItr, InterSpot)
	init_pointer(FirstNPC, LastNPC, NotPLayerCreature)
	init_pointer(FirstEnemy, LastEnemy, NPCList)

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

	LoadingTitle(6);

    CONS.out( "Init Pointers-OK" );

	GAME_EVENTS.init();
	// CONSOLE.out( "Init Game_Event-OK" );    LoadingTitle( 15 );
	Pixels.init();
	//CONS.out( "Init PixelManip-OK" );    LoadingTitle( 20 );
	DATABASE.init();
    ////CONS.out( "Init Database-OK" );      LoadingTitle( 25 );
	ALLANIMES.init();
    ////CONS.out( "Init Anim-OK" );          LoadingTitle( 30 );
	COMBAT.init();
    ////CONS.out( "Init Combat-OK" );        LoadingTitle( 35 );
	GAME_NPCS.init();
    //CONS.out( "Init NPC-OK" );           LoadingTitle( 40 );
	GAME_MAP.init();
    //CONS.out( "Init Map-OK" );           LoadingTitle( 55 );
	cInterface.init();
    //CONS.out( "Init Interface-OK" );     LoadingTitle( 60 );
	QUEST.init();
    //CONS.out( "Init Quests-OK" );        LoadingTitle( 70 );
	DIALOG.init();
    //CONS.out( "Init Dialog-OK" );        LoadingTitle( 75 );
	INVENTORY.init();
    //CONS.out( "Init Inventory-OK" );     LoadingTitle( 85 );
	PLAYER.init();
    //CONS.out( "Init PLAYER-OK" );        LoadingTitle( 95 );

    LOOT.init();

    //CONS.out( "INITIALIZATION FINISED:" );
    stringstream sserror;
    sserror << "ERROR LEVEL: " << ERROR;

    //CONS.out( sserror.str() );

    LoadingTitle( 100 );

    return ERROR;
}
void ENGINE_handleEvent_resizeWindow( SDL_Event *EVENT )
{
	int w, h;


	if(EVENT->type == SDL_WINDOWEVENT
		&& EVENT->window.event == SDL_WINDOWEVENT_RESIZED) {
		SDL_GetWindowSize(WIN_MAIN.window, &w, &h);
		WIN_MAIN.setWindowWidth(w);
		WIN_MAIN.setWindowHeight(h);
    }
}


void LoadingTitle( int percent )
{
	static SDL_Texture *txLoadTitle, *txLoadBar_empty, *txLoadBar_fill;
	static SDL_Rect rLoadBar;


	if(0 == percent) {
		txLoadTitle = LoadTex( "textures//LoadScreen_Title2.png" );
		txLoadBar_empty = LoadTex( "textures//LoadScreen_bar_empty.png" );
		txLoadBar_fill = LoadTex( "textures//LoadScreen_bar_fill.png" );
		rLoadBar.x = 0;
		rLoadBar.y = 0;
		rLoadBar.w = 1;
		rLoadBar.h = 31;
	} else {
		rLoadBar.w = 547 * percent / 100;
	}

	SDL_SetRenderDrawColor(RENDER_MAIN, 0, 0, 0, 255);
	SDL_RenderClear(RENDER_MAIN);

	ApplyTex((SCREEN_WIDTH - 330)/2, 300, txLoadTitle);
	ApplyTex((SCREEN_WIDTH - 537)/2, SCREEN_HEIGHT - 300, txLoadBar_empty);
	ApplyTex((SCREEN_WIDTH - 537)/2, SCREEN_HEIGHT - 300, txLoadBar_fill, &rLoadBar);
	SDL_RenderPresent(RENDER_MAIN);
	//SDL_Delay( 300 );
}


void ENGINE_HandleEvent_resizeWindow( SDL_Event *EVENT )
{
	int w, h;


	if(EVENT->type == SDL_WINDOWEVENT
		&& EVENT->window.event == SDL_WINDOWEVENT_RESIZED) {
        SDL_GetWindowSize( WIN_MAIN.window, &w, &h );
        WIN_MAIN.setWindowWidth ( w );
        WIN_MAIN.setWindowHeight( h );
    }
}


void ENGINE_HandleEvents(SDL_Event *event)
{
    //Handle window resize events
	ENGINE_HandleEvent_resizeWindow(event);
}


void ENGINE_Quit(void)
{
	INVENTORY.cleanup();
	cInterface.cleanup();
	ALLANIMES.cleanup();

	SDL_DestroyRenderer( RENDER_MAIN );
	SDL_DestroyWindow( WINDOW_MAIN );

	TTF_Quit();
	SDL_Quit();
}



SDL_Surface *LoadSurf(string file)
{
	SDL_Surface *loadedimg = nullptr;


	loadedimg = IMG_Load( file.c_str() );

	if(nullptr == loadedimg) {
		//DEBUG.fout( FILE_FLF, (string)"load fail: " + file );
		//DEBUG.fout( FILE_FLF, (string)"reason: "    + SDL_GetError() + (string)"\n" );
	}

	return loadedimg;
}


SDL_Texture *LoadTex(string file)
{
	SDL_Texture *loadedtex = nullptr;


	loadedtex = IMG_LoadTexture(WIN_MAIN.render,file.c_str() );

	if(loadedtex == nullptr) {
		//DEBUG.fout( FILE_FLF, (string)"load fail: " + file );
		//DEBUG.fout( FILE_FLF, (string)"reason: "    + SDL_GetError() + (string)"\n" );
	}

	return loadedtex;
}

void ApplyTexFree(int x,int y,SDL_Texture *tex,SDL_Rect *clip ,int w,int h ){

    SDL_Rect pos;

    pos.x = x;
    pos.y = y;

	if(clip != nullptr) {
        pos.w = clip->w;
        pos.h = clip->h;
	} else {
		SDL_QueryTexture(tex, nullptr, nullptr, &pos.w, &pos.h);
	}

	if( w != NO_CHANGE )
		pos.w = w;
	if( h != NO_CHANGE )
		pos.h = h;

	SDL_RenderCopy(RENDER_MAIN, tex, clip, &pos);
}


void ApplyTexLetterbox(int x,int y,SDL_Texture *tex,SDL_Rect *clip,int w,int h )
{
    SDL_Rect pos;

	pos.y = static_cast<int>(Y_ABS);

    pos.x = ceil( (double)x * SCALE );
    pos.y += ceil( (double)y * SCALE  );

	if(nullptr != clip)
    {
        pos.w = clip->w;
        pos.h = clip->h;
	} else {
		SDL_QueryTexture( tex,NULL,NULL,&pos.w,&pos.h );
	}

    if( w != NO_CHANGE ) pos.w = w;
    if( h != NO_CHANGE ) pos.h = h;

    //ADJUST
    pos.w = ceil((double)pos.w * SCALE);
    pos.h = ceil((double)pos.h * SCALE);


    SDL_RenderCopy( RENDER_MAIN,tex,clip,&pos );
}


void ApplyTex(int x,int y,SDL_Texture *tex,SDL_Rect *clip ,int w ,int h){

    //Free Mode
    if( ENGINE_displaymode == DISPLAYMODE_FREE )
    {
        ApplyTexFree( x, y, tex, clip, w, h );
    }
    //Letterbox Mode
    else
    if( ENGINE_displaymode == DISPLAYMODE_LETTERBOX )
    {
        ApplyTexLetterbox( x, y, tex, clip , w, h );
    }
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
		SDL_SetRenderDrawColor(RENDER_MAIN, 0, 0, 0, 255);
		SDL_RenderFillRect(RENDER_MAIN, &blackline1 );
		SDL_RenderFillRect(RENDER_MAIN, &blackline2 );
    }


    SDL_RenderPresent( RENDER_MAIN );
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
SDL_Color WHITE        = {255, 255, 255, 255};
SDL_Color BLACK        = {0, 0, 0, 255};
SDL_Color RED          = {255, 21, 5, 255};
SDL_Color BLUE         = {0, 0, 255, 255};
SDL_Color GREEN        = {93, 255, 61, 255};
SDL_Color YELLOW_LIGHT = {255, 240, 168, 255};

//Global Svariables

int NrBlockX, NrBlockY;


