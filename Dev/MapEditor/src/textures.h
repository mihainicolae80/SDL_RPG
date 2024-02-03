#ifndef TEXTURES_H_INCLUDED
#define TEXTURES_H_INCLUDED

#include "PixelManipulation.h"
#include <sstream>

#define MAX_ITEMS 300
#define MAX_STATES 8


class Textures{
public:
    Textures();
    SDL_Surface* getSurf(int nr,PixelManipulationRotation state);

private:
    SDL_Surface*  surf[MAX_ITEMS][MAX_STATES];
};

extern Textures TEXTURES;

#endif // TEXTURES_H_INCLUDED
