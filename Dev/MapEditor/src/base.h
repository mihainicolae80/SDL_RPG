#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "main_headers.h"
#include "Placeable.h"
#include "PixelManipulation.h"
#include "debug.h"


#define RENDER_MAIN      WIN_MAIN.render
#define WINDOW_MAIN      WIN_MAIN.window
#define WINDOW_MAIN_ID   WIN_MAIN.getID()

//Virtual size
#define SCREEN_WIDTH  1920
#define SCREEN_HEIGHT 1080

//Real size
#define WIN_WIDTH WIN_MAIN.getWindowWidth()
#define WIN_HEIGHT WIN_MAIN.getWindowHeight()
#define REAL_SCREEN_HEIGHT (double)WIN_WIDTH/16*9
#define REAL_SCREEN_WIDTH  WIN_WIDTH
#define Y_ABS (WIN_HEIGHT - REAL_SCREEN_HEIGHT)/2



#define SCALE ( (long double)REAL_SCREEN_WIDTH/SCREEN_WIDTH  )

#define constraint(x,low,high) x >= low ? ( x <= high ? x : high ) : low
#define map(x,low,high,tolow,tohigh) (  ( (long double)x / (high-low) ) * (tohigh-tolow) )
#define mousetovirtual(x) map( x,0,WIN_WIDTH,0,SCREEN_WIDTH ) //Convert mouse coordonates relative to window
//image (without letterboxed part)


#define FLIP_VERT 0x01
#define FLIP_HOR 0x02
#define ROTATE_LEFT 1
#define ROTATE_RIGHT 2
#define FreeBG 0
#define FreeFG 1
#define FreeDynam 2
#define DOOR_NR 1
#define CHANGE_INT 1
#define CHANGE_STR 2




using namespace std;

///CONSTANTS
const int NO_CHANGE = -30000;
const int UPDATED   = true;
//Display Mode
const int DISPLAYMODE_FREE = 0;
const int DISPLAYMODE_LETTERBOX = 1;

//VAR
extern int ENGINE_displaymode;

///TODO Trebuie facute private
extern int cDayTime,iMoveMap_x,iMoveMap_y,mouse_drag_start_x,mouse_drag_start_y,FCMove_x,FCMove_y,maxx,maxy,cInterItm,InterInfo[3];
extern bool key_ctrl_down,btn_mouse_down, bCSkinOrigin,FCmoveing,LightMap_Changed[6],LibLock[3];



extern class CLS_WINDOWING
{
private:
	std::string title;
	uint32_t window_flags, render_flags;
	// --Screen --
	int width;
	int height;

	//ViewPorts
	int viewport_x;
	int viewport_y;

public:

	//---WINDOWS AND RENDERES---
	SDL_Window   *window;
	SDL_Renderer *render;

	CLS_WINDOWING();
	void setTitle(std::string x);
	void setWindowWidth (int x);
	void setWindowHeight(int x);
	int  getWindowWidth (void);
	int  getWindowHeight(void);
	Uint32 getID();
	void setWindowFlags(uint32_t x);
	void setRenderFlags(uint32_t x);
	int createWindow(void);
	int createRender(void);

} WIN_MAIN;


// -- GENERAL --
int  ENGINE_Init(void);
void ENGINE_HandleEvent_resizeWindow( SDL_Event *EVENT );
void ENGINE_HandleEvents( SDL_Event *EVENT );
void ENGINE_Quit(void);

// -- RENDERING --
SDL_Surface *loadSurf(string file);
SDL_Texture *loadTex(string file);
void applyTexFree(int x,int y,SDL_Texture *tex,SDL_Rect *clip = nullptr,int w = NO_CHANGE,int h = NO_CHANGE );
void applyTexLetterbox(int x,int y,SDL_Texture *tex,SDL_Rect *clip = nullptr,int w = NO_CHANGE,int h = NO_CHANGE );
void applyTex(int x,int y,SDL_Texture *tex,SDL_Rect *clip = nullptr,int w = NO_CHANGE,int h = NO_CHANGE );
void applySurf(int x,int y,SDL_Surface *tex,SDL_Rect *clip = nullptr,int w = NO_CHANGE,int h = NO_CHANGE );

//Draw Rectangle
void DrawRectFree(int x,int y,int x2,int y2);
void DrawRectAdjusted(int x,int y,int x2,int y2);
void DrawRect(int x,int y,int x2,int y2,SDL_Color *color);
//FillRect
void FillRectFree(int x,int y,int x2,int y2);
void FillRectAdjusted(int x,int y,int x2,int y2);
void FillRect(int x,int y,int x2,int y2,SDL_Color *color);



void ENGINE_RenderPresent(void);


//Misc
void AddChunk( FreeChunk *modelchunk,int x, int y,int list );
DayNightChunk *AddLight( int x, int y,int nr, int range );
ColideChunk *AddColider( int x,int y, int w,int h,int id );
InterSpot *AddInterSpot( int x,int y ,int w,int h,int type,int i1,int i2,int i3,int i4,int id);
InterSpot *AddInterSpot( int x,int y );
NPCchunk *AddNPCchunk( NPCchunk *modelchunk );
void ctos(char carray[],string &str );


//Fonts
extern TTF_Font *Font14,*Font18,*Font20,*Font24,*Font2_14,*Font2_18,*Font2_20,*Font2_24;

//Colors
extern SDL_Color WHITE, BLACK, GRAY,RED, BLUE, GREEN,YELLOW_LIGHT;

