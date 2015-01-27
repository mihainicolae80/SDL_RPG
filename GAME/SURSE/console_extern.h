#ifndef CONSOLE_EXTERN_H_INCLUDED
#define CONSOLE_EXTERN_H_INCLUDED

#include "main_headers.h"

#define WINDOW_CONSOLE CONSOLE.window
#define RENDER_CONSOLE CONSOLE.render
#define WINDOW_CONSOLE_ID CONSOLE.getID()


extern class CLS_CONSOLE{
private:
char ConsText[100];
bool ConsFocus;



public:

//WINDOING
SDL_Window   *window;
SDL_Renderer *render;

//font
TTF_Font *font;

//Functions

int  init();
void sendEvents( SDL_Event *EVENT );
void out(string text );
void outTex( int x,int y,SDL_Texture *tex,SDL_Rect *clip = NULL );
Uint32 getID();

}CONSOLE;


#endif // CONSOLE_EXTERN_H_INCLUDED
