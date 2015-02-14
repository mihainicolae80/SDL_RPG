#ifndef BASICS_H_INCLUDED
#define BASICS_H_INCLUDED

#include <iostream>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "structs.h"
#include <sstream>
#include "main_headers.h"
#include "debug.h"
#include "base.h"


using namespace std;

///ERROR TYPES
/** ERROR IMPACT **/
#define ERROR_CRIT 1
#define ERROR_HIGH 2
#define ERROR_LOW  3
#define ERROR_ADD(x) ERROR = x < ERROR ? x : ERROR

///COLORS
extern SDL_Color WHITE;



//Debug
extern ofstream DEBUG_outf;
extern ofstream DEBUG_FileLoadFails;
extern SDL_Surface *DEBUG_ExcMarkSurf;
extern SDL_Texture *DEBUG_ExcMarkTex;

//Basic value and string
void   BAS_ctos  ( char carray[], string &strnew );
double BAS_abs   ( double x );
void   BAS_itos  ( int nr,string &newstr );
void   BAS_strcpy( char *dest,const char *sourc );
void   BAS_strcat( char *dest,const char *sourc );

//Graphics
InterSpot *AddInterSpot( int x,int y ,int w,int h,int type,int i1,int i2,int i3,int i4,int id);
bool detect_colision(NotPLayerCreature *npc = NULL );


//TTF TEXT RENDER
int          BAS_LoadFonts(void);
SDL_Texture* BAS_RenderText( TTF_Font *font,const char *text,SDL_Color color );
//ENGINE
void         BAS_DestroyTex( SDL_Texture **tex );


void LoadingTitle( int percent );

#endif // BASICS_H_INCLUDED
