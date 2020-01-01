#include "ui_list_item.h"


UI_List_Item::UI_List_Item(int w,int h,SDL_Surface *surf
        ,CallbackClass leftclick,CallbackClass rightclick){

    this->w = w;
    this->h = h;
    this->surf = surf;
}
void UI_List_Item::handleEvents(SDL_Event *event){


}
void UI_List_Item::draw(){

    applySurf(x,y,surf,NULL,w,h);
}
