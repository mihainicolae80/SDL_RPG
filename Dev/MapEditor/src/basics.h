#ifndef BASICS_H_INCLUDED
#define BASICS_H_INCLUDED

#include <iostream>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <sstream>
#include "debug.h"
#include "base.h"


using namespace std;

///ERROR TYPES
/** ERROR IMPACT **/
#define ERROR_CRIT 1
#define ERROR_HIGH 2
#define ERROR_LOW  3
#define ERROR_ADD(x) ERROR = x < ERROR ? x : ERROR




//Debug
extern ofstream DEBUG_outf;
extern ofstream DEBUG_FileLoadFails;

//Basic value and string
void   BAS_ctos  ( char carray[], string &strnew );
double BAS_abs   ( double x );
void   BAS_itos  ( int nr,string &newstr );
void   BAS_strcpy( char *dest,const char *sourc );
void   BAS_strcat( char *dest,const char *sourc );

//TTF TEXT RENDER
int          BAS_LoadFonts(void);
TTF_Font     *BAS_LoadFont(string file,int fontsize);
SDL_Texture* BAS_RenderText( TTF_Font *Font20,const char *text,SDL_Color color );
//ENGINE
void         BAS_DestroyTex( SDL_Texture **tex );



#endif // BASICS_H_INCLUDED
