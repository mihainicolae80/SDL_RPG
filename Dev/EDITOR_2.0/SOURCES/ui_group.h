#ifndef UI_GROUP_H_INCLUDED
#define UI_GROUP_H_INCLUDED

#include "ui_bar.h"
#include "base.h"

enum UI_GroupPosition{

UI_GROUP_CENTER,
UI_GROUP_NORTH,
UI_GROUP_SOUTH,
UI_GROUP_EAST,
UI_GROUP_WEST
};

class UI_Group{

private:
    int x,y,offsetX,offsetY;
    UI_GroupPosition positionX,positionY;
    UI_Bar *bar;



public:
    /*Set the position of the group*/
    UI_Group(UI_GroupPosition positionX
            ,UI_GroupPosition positionY,UI_Bar *bar);

    /*Set the offset relative to the chosen position*/
    void setOffsetX(int offsetx);
    void setOffsetY(int offsety);
    void updatePosition();
    void handleLogics(SDL_Event *event);

    void draw();
};


#endif // UI_GROUP_H_INCLUDED
