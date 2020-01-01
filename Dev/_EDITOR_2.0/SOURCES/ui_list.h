#ifndef UI_LIST_H_INCLUDED
#define UI_LIST_H_INCLUDED

#include "base.h"
#include <string>
#include <list>
#include "ui_item.h"


class UI_List : public UI_Item{

public:
    UI_List(int w,int h,SDL_Surface *surf);
    void setX(int x);
    void setY(int y);
    int  getX();
    int  getY();
    int  getWidth();
    int  getHeight();
    void draw();
    void handleEvents(SDL_Event *);
    void updateItems();

    int addTab();
    void addInTab(SDL_Surface *surf);
    ///TODO 1-2 fct de callback
    //void addCallbackLeftClick(int tab,obiect_callback);
    //void addCallbackRightClick(int tab,obiect_callback);

private:
    int x,y,w,h;
    SDL_Surface *surf;

    int tabsNr;
    std::list<std::list<SDL_Surface*>*> tabsList;
};

#endif // UI_LIST_H_INCLUDED
