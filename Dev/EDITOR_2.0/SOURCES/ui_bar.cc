#include "ui_bar.h"


UI_Bar::UI_Bar(SDL_Color *color,UI_BAR_TYPE barType){

    this->x = 0;
    this->y = 0;
    this->w = 0;
    this->h = 0;
    this->barType = barType;
    this->color = color;

    //Allocate space for the Item list
    this->items = new list<UI_Item*>();
}
UI_Bar::~UI_Bar(){

    UI_Item *temp;

    //Empty the list and free the buttons
    while(items->empty() == false){

        temp = items->front();
        items->pop_front();
        delete temp;
    }

    //Free the item list
    delete items;
}
void UI_Bar::addItem(UI_Item *newitem){

    items->push_back(newitem);

    updateItems();

}
void UI_Bar::handleEvents(SDL_Event *event){

    std::list<UI_Item*>::iterator it;

    for(it = items->begin();it != items->end();it++){

        (*it)->handleEvents(event);
    }
}
void UI_Bar::draw(){

    int cx,cy;
    std::list<UI_Item*>::iterator it;

    cx = x;
    cy = y;


    //Background
    FillRect(x,y,x+w,y+h,color);


    for(it = items->begin();it != items->end();it++){

        (*it)->draw();

        if(barType == UI_BAR_HORIZONTAL){

            cx += (*it)->getWidth();
        }
        else{

            cy += (*it)->getHeight();
        }
    }

}


void UI_Bar::setX(int x){ this->x = x; }
int  UI_Bar::getX(){ return x; }
void UI_Bar::setY(int y){ this->y = y; }
int  UI_Bar::getY(){ return y; }
int UI_Bar::getWidth(){ return w; }
int UI_Bar::getHeight(){ return h; }

/*Update the position of all items*/
void UI_Bar::updateItems(){

    std::list<UI_Item*>::iterator it;

    w = 0;
    h = 0;

    for(it = items->begin(); it != items->end(); it++){

        if(barType == UI_BAR_HORIZONTAL){

            (*it)->setX(x+w);
            (*it)->setY(y);
            w += (*it)->getWidth();

            if((*it)->getHeight() > h){

                h = (*it)->getHeight();
            }
        }
        else{

            (*it)->setX(x);
            (*it)->setY(y+h);

            h += (*it)->getHeight();

            if((*it)->getWidth() > w){

                w = (*it)->getWidth();
            }
        }

        (*it)->updateItems();
    }
}
