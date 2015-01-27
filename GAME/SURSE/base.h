#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

// -- FROM CLASSES --
//#include "assets.h"
//#include "console.h"
//#include "map.h"
//#include "player.h"
//#include "settings.h"

#define SCREEN_WIDTH  WIN_MAIN.getWindowWidth()
#define SCREEN_HEIGHT WIN_MAIN.getWindowHeight()

#define RENDER_MAIN      WIN_MAIN.render
#define WINDOW_MAIN      WIN_MAIN.window
#define WINDOW_MAIN_ID   WIN_MAIN.getID()

extern class CLS_WINDOWING{
private:
std::string title;
int window_flags,render_flags;
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

        CLS_WINDOWING(void);
void    setTitle(std::string x );
void    setWindowWidth ( int x );
void    setWindowHeight( int x );
int     getWindowWidth ( void );
int     getWindowHeight( void );
Uint32  getID(void);
void    setWindowFlags( int x );
void    setRenderFlags( int x );
int     createWindow(void);
int     createRender(void);

}WIN_MAIN;



int  ENGINE_Init(void);
void ENGINE_handleEvent_resizeWindow( SDL_Event *EVENT );

void ENGINE_Cleanup(void);

#endif // BASE_H_INCLUDED
