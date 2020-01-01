#include "ui_manager.h"

void UI_Manager::addGroup(UI_Group *target){

    groups.push_front(target);
}

void UI_Manager::draw(){

    std::list<UI_Group*>::iterator it;


    for(it = groups.begin(); it != groups.end(); it++){

        (*it)->draw();
    }
}

void UI_Manager::handleEvents(SDL_Event *event){

    std::list<UI_Group*>::iterator it;


    for(it = groups.begin(); it != groups.end(); it++){

        (*it)->handleEvents(event);
    }
}
