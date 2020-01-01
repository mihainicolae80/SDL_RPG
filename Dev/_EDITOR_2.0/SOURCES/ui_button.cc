#include "ui_button.h"

UI_Button::UI_Button(int w,int h,SDL_Texture *tex
         ,CallbackClass *callback){

    this->x = 0;
    this->y = 0;
    this->w = w;
    this->h = h;
    this->tex = tex;
    this->pressed = false;
    this->callback = callback;
}

UI_Button::~UI_Button(){

}

void UI_Button::handleEvents(SDL_Event *event){

    int mouseX,mouseY;

    //If the left mouse button was pressed
    if(event->type == SDL_MOUSEBUTTONDOWN
    && event->button.button == SDL_BUTTON_LEFT){

        mouseX = event->button.x;
        mouseY = event->button.y;

        //Check is the pointer is on the button
        if(mouseX > x && mouseX < x + w
        && mouseY > y && mouseY < y + h){

            this->pressed = true;
          }
    }
    //If the left mouse button was released
    else if(event->type == SDL_MOUSEBUTTONUP
         && event->button.button == SDL_BUTTON_LEFT){

        mouseX = event->button.x;
        mouseY = event->button.y;

        //Check is the pointer is on the button
        if(mouseX > x && mouseX < x + w
        && mouseY > y && mouseY < y + h){

            //If the button was pressed
            if(pressed){

                pressed = false;

                //Call the Callback function if possible
                if(callback != NULL){

                    callback->execute();
                }
            }
        }
    }
    else if(event->type == SDL_MOUSEMOTION){

        mouseX = event->button.x;
        mouseY = event->button.y;

        if(!(mouseX > x && mouseX < x + w
        && mouseY > y && mouseY < y + h)
        && pressed){

            pressed = false;
        }
    }
}

void UI_Button::draw(){

    if(pressed == false){

        //Draw the Button
        applyTex(x,y,tex,NULL,w,h);
    }
    else{

        //Fill the button frame with black
        FillRect(x,y,x+w,y+h,&BLACK);

        //Define a section of the button
        SDL_Rect clip;
        clip.x = 0;
        clip.y = 0;
        clip.w = w-10;
        clip.h = h-10;


        //Draw the button with a slight offset
        applyTex(x+10,y+10,tex,&clip);
    }
}
void UI_Button::updateItems(){}

int UI_Button::getWidth(){ return w; }
int UI_Button::getHeight(){ return h; }
int UI_Button::getX(){ return x; }
int UI_Button::getY(){ return y; }
void UI_Button::setX(int x){ this->x = x; }
void UI_Button::setY(int y){ this->y = y; }
