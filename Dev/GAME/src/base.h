#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>


#define RENDER_MAIN      WIN_MAIN.render
#define WINDOW_MAIN      WIN_MAIN.window
#define WINDOW_MAIN_ID   WIN_MAIN.getID()


//Virtual size
#define SCREEN_WIDTH  1920
#define SCREEN_HEIGHT 1080

//Real size
#define WIN_WIDTH WIN_MAIN.getWindowWidth()
#define WIN_HEIGHT WIN_MAIN.getWindowHeight()
#define REAL_SCREEN_HEIGHT static_cast<double>(WIN_WIDTH / 16 * 9)
#define REAL_SCREEN_WIDTH  WIN_WIDTH
#define Y_ABS ((WIN_HEIGHT - REAL_SCREEN_HEIGHT) / 2)

#define SCALE ( static_cast<double>(REAL_SCREEN_WIDTH) / SCREEN_WIDTH)

#define constraint(x,low,high) x >= low ? ( x <= high ? x : high ) : low
#define map(x, low, high, tolow, tohigh) ((static_cast<long double>(x) / (high-low)) * (tohigh-tolow))
#define mousetovirtual(x) map( x,0,WIN_WIDTH,0,SCREEN_WIDTH ) //Convert mouse coordonates relative to window
                                                                //to mouse coordonates relative to the rendered
                                                                //image (without letterboxed part)

using namespace std;

///CONSTANTS
const int NO_CHANGE = -30000;
const int UPDATED   = true;
//Display Mode
const int DISPLAYMODE_FREE = 0;
const int DISPLAYMODE_LETTERBOX = 1;

//VAR
extern int ENGINE_displaymode;


extern class CLS_WINDOWING{

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
    void setTitle(std::string x );
    void setWindowWidth ( int x );
    void setWindowHeight( int x );
    int  getWindowWidth ( void );
    int  getWindowHeight( void );
    Uint32 getID();
    void setWindowFlags( int x );
    void setRenderFlags( int x );
    int createWindow(void);
	bool createRender(void);

} WIN_MAIN;


// Engine
int  ENGINE_Init(void);
void ENGINE_HandleEvent_resizeWindow(SDL_Event *EVENT);
void ENGINE_HandleEvents(SDL_Event *event);
void ENGINE_RenderPresent(void);
void ENGINE_Quit(void);

// Surface / Textures
SDL_Surface *LoadSurf(string file);
SDL_Texture *LoadTex(string file);
void ApplyTexFree(int x,int y,SDL_Texture *tex,SDL_Rect *clip = nullptr,int w = NO_CHANGE,int h = NO_CHANGE );
void ApplyTexLetterbox(int x,int y,SDL_Texture *tex,SDL_Rect *clip = nullptr,int w = NO_CHANGE,int h = NO_CHANGE );
void ApplyTex(int x,int y,SDL_Texture *tex,SDL_Rect *clip = nullptr,int w = NO_CHANGE,int h = NO_CHANGE );

// Draw Rectangle
void DrawRectFree(int x, int y, int x2, int y2);
void DrawRectAdjusted(int x, int y, int x2, int y2);
void DrawRect(int x, int y, int x2, int y2, SDL_Color *color);

// FillRect
void FillRectFree(int x, int y, int x2, int y2);
void FillRectAdjusted(int x, int y, int x2, int y2);
void FillRect(int x, int y, int x2, int y2, SDL_Color *color);

#endif // BASE_H_INCLUDED
