#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include "base.h"
#include "basics.h"



extern class CLS_CONSOLE{

    private:

    bool _DISPLAY;
    char INPUT_LINE_TEXT[100];
    int firstline,item_counter;
    TTF_Font *CONS_FONT = NULL;
    SDL_Texture *INPUT_LINE_TEX  = NULL;
    SDL_Surface *INPUT_LINE_SURF = NULL;

    struct node
    {
        SDL_Texture *tx_text;
        node *next;
    }*_First_node,*_Last_node;


    public:
    void init();
    void draw();
    void switch_if_display();
    void out( std::string );
    void sendEvent( SDL_Event *event );
    bool is_shown(void);
}CONS;

#endif // CONSOLE_H_INCLUDED
