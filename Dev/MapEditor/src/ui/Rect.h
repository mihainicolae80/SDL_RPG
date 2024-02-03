#pragma once

#include "SDL2/SDL.h"

namespace engine
{
namespace ui
{

class Rect
{
public:

    bool ContainsPoint(SDL_Point p);

    SDL_Rect m_rect;
};

}
}
