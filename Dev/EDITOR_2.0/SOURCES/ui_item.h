#ifndef UI_ITEM_H_INCLUDED
#define UI_ITEM_H_INCLUDED

#include "SDL2/SDL.h"

class UI_Item{
public:
    virtual ~UI_Item() {};
    virtual void setX(int x) = 0;
    virtual void setY(int y) = 0;
    virtual int getX() = 0;
    virtual int getY() = 0;
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
    virtual void draw() = 0;
    virtual void handleEvents(SDL_Event *) = 0;
    virtual void updateItems() = 0;
};

#endif // UI_ITEM_H_INCLUDED
