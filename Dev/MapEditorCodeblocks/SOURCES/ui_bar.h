#ifndef UI_BAR_H_INCLUDED
#define UI_BAR_H_INCLUDED

#include <list>
#include "base.h"
#include "ui_item.h"

enum UI_BAR_TYPE{

    UI_BAR_HORIZONTAL,
    UI_BAR_VERTICAL
};


class UI_Bar : public UI_Item{
public:

    UI_Bar(SDL_Color *color, UI_BAR_TYPE barType);
    ~UI_Bar();
    void addItem(UI_Item *item);
    void handleEvents(SDL_Event *event);
    void setX(int x);
    int  getX();
    void setY(int x);
    int  getY();
    int getWidth();
    int getHeight();
    void draw();
    void updateItems();

private:
    int x,y,w,h;
    UI_BAR_TYPE barType;
    SDL_Color *color;

    std::list<UI_Item*> *items;

};

#endif // UI_BAR_H_INCLUDED
