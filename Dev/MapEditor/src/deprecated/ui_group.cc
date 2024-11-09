#include "ui_group.h"

UI_Group::UI_Group(UI_GroupPosition positionX
        ,UI_GroupPosition positionY,UI_Bar *bar){


    this->positionX = positionX;
    this->positionY = positionY;
    this->bar = bar;
    this->offsetX = 0;
    this->offsetY = 0;
    this->updatePosition();
}


void UI_Group::setOffsetX(int offsetX){
    this->offsetX = offsetX;
}
void UI_Group::setOffsetY(int offsetY){
    this->offsetY = offsetY;
}
void UI_Group::updatePosition(){

    if(bar == NULL)
        return;

    switch(positionX){

    case UI_GROUP_CENTER:
        x = (WIN_MAIN.getWindowWidth() - bar->getWidth())/2;
        break;

    case UI_GROUP_WEST:
        x = 0;
        break;

    case UI_GROUP_EAST:
        x = WIN_MAIN.getWindowWidth() - bar->getWidth();
        break;
    default:
        x = 0;
    }

    switch(positionY){

    case UI_GROUP_CENTER:
        y = (WIN_MAIN.getWindowHeight() - bar->getHeight())/2;
        break;

    case UI_GROUP_NORTH:
        y = 0;
        break;

    case UI_GROUP_SOUTH:
        y = WIN_MAIN.getWindowHeight() - bar->getHeight();
        break;
    default:
        y = 0;
    }

    x += offsetX;
    y += offsetY;

    bar->setX(x);
    bar->setY(y);
    bar->updateItems();

}
void UI_Group::draw(){

    if(bar != NULL)
        bar->draw();
}

void UI_Group::handleEvents(SDL_Event *event){

    if(bar != NULL){

        if( event->type == SDL_WINDOWEVENT &&
        event->window.event == SDL_WINDOWEVENT_RESIZED ){

            this->updatePosition();
        }

        bar->handleEvents(event);
    }

}
