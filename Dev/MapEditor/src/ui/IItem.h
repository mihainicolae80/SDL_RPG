#ifndef IITEM_H_
#define IITEM_H_

#include "SDL2/SDL.h"

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
    virtual bool handleEvents(SDL_Event *) = 0;
    virtual void updateItems() = 0;
};

#endif // IITEM_H_
