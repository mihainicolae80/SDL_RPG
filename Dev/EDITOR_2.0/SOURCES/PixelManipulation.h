#ifndef PIXELMANIPULATION_H_INCLUDED
#define PIXELMANIPULATION_H_INCLUDED

#include "SDL2/SDL.h"
#include "base.h"

 class PixelManipulation{

private:
    SDL_Surface *Palette;
public:
    void init();
    Uint32 getpix( SDL_Surface *surface,int x,int y );
    void putpix( SDL_Surface *surface,int x,int y,Uint32 pixel );
    SDL_Surface *flip_img( SDL_Surface *surface,int direction );
    SDL_Surface *rotate_img( SDL_Surface *surface,int direction ,bool animated,int frames );
    SDL_Surface *RenderCorner( SDL_Surface *surface );
    SDL_Surface *RenderRect(int w,int h, int color);
    SDL_Surface *RenderLightMap( int nr );
    SDL_Surface *RenderGrid();
    void RenderMouseLines(SDL_Surface *&l1 ,SDL_Surface *&l2 );
    void cleanup();
};

extern PixelManipulation Pixels;

#endif // PIXELMANIPULATION_H_INCLUDED
