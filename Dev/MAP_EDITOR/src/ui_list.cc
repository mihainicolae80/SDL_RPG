#include "ui_list.h"


UI_List::UI_List(int w,int h,SDL_Surface *surf){

    this->x = 0;
    this->y = 0;
    this->w = w;
    this->h = h;
    this->surf = surf;

    this->tabsNr = 0;

}

void UI_List::setX(int x){ this->x = x; }
void UI_List::setY(int y){ this->y = y; }
int  UI_List::getX(){ return x; }
int  UI_List::getY(){ return y; }
int  UI_List::getWidth(){ return w; }
int  UI_List::getHeight(){ return h; }

void UI_List::draw()
{
    //Draw the background
	//applyTex(x , y, tex, NULL, w, h);
}

int UI_List::addTab(){

    ///TODO delete the added lists
    tabsList.push_back(new std::list<SDL_Surface*>);
}

void UI_List::handleEvents(SDL_Event *){


}

void UI_List::updateItems(){ }
