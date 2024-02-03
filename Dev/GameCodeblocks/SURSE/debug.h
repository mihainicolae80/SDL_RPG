#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#include <fstream>
#include "basics.h"
#include "player.h"



const int FILE_FLF     = 0;
const int FILE_GENERAL = 1;
const int FILE_CONSOUT = 1;

/**    --- DEBUG OUTPUT TO FILE ---

    To be used for GAME/CONSOLE CRASHING ERRORS
**/

#define DEBUG_OUT_GEN(x) DEBUG.fout( FILE_GENERAL, x )
#define DEBUG_OUT_FLF(x) DEBUG.fout( FILE_FLF    , x )



extern class CLS_DEBUG{
private:
ofstream ofGeneral;
ofstream ofFlf;
ofstream ofConsOut;
bool drawcolrects;

public:

SDL_Surface *sfExMark;
SDL_Texture *txExMark;

    void init_low();
    int  init_high();
    void fout( int file_out, string text );
    void DrawCollRects(void);
    void setDrawCollRects(bool x);
    SDL_Surface *getNewsfExc(void);
    ~CLS_DEBUG();
}DEBUG;

#endif // DEBUG_H_INCLUDED
