#ifndef UI_BUTTON_H_INCLUDED
#define UI_BUTTON_H_INCLUDED

#include "base.h"
#include "Interface.h"
#include "ui_item.h"



/**
        User Interface Button.
    When pressed it will call a
    void foo(void) calback function.
**/
class UI_Button : public UI_Item{
public:

     /*The texture will NOT be
    Destroyed Automatically*/
    UI_Button(int w,int h, SDL_Texture *tex
              ,CallbackClass* callback);
    ~UI_Button();

    void handleEvents(SDL_Event *event);
    void draw();
    void setX(int x);
    void setY(int y);
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    void updateItems();

private:
    //TEMP
    int x,y,w,h;
    bool pressed;

    //Callback function to be called on button press
    CallbackClass* callback;

    SDL_Texture *tex;
};

#endif // UI_BUTTON_H_INCLUDED
