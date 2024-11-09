#pragma once

#include "SDL2/SDL.h"

namespace engine::ui
{

class IItem
{
public:
    virtual ~IItem() {};
    virtual void setX(int x) = 0;
    virtual void setY(int y) = 0;
    virtual int getX() = 0;
    virtual int getY() = 0;
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
    virtual void draw() = 0;
    virtual bool handleEvents(SDL_Event&) = 0;
};

} // namespace engine::ui