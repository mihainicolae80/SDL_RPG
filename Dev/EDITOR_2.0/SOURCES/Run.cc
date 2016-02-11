#include "Run.h"


#include "ui_manager.h"
#include "ui_button.h"
#include "ui_list.h"
#include "textures.h"


int runEditor(void){

    bool end = false;
    int error = 0;
    SDL_Event event;

    ///Initlialize
    error = ENGINE_Init();
    if(error) return error;

    INTERFACE.init();

    PIXELS.init();

    //UI_Button button(DEBUG.txExMark
    //    ,(new InterfaceCallback_showbg()));

    UI_Bar *bar1 = new UI_Bar(&GRAY,UI_BAR_HORIZONTAL);
    bar1->addItem(new UI_Button(50,50,DEBUG.txExMark,(new InterfaceCallback_showbg())));
    bar1->addItem(new UI_Button(100,50,DEBUG.txExMark,(new InterfaceCallback_showbg())));
    bar1->addItem(new UI_Button(50,50,DEBUG.txExMark,(new InterfaceCallback_showbg())));

    UI_Bar *bar2 = new UI_Bar(&GRAY,UI_BAR_HORIZONTAL);
    bar2->addItem(new UI_Button(50,50,DEBUG.txExMark,(new InterfaceCallback_showbg())));
    bar2->addItem(new UI_Button(100,30,DEBUG.txExMark,(new InterfaceCallback_showbg())));
    bar2->addItem(new UI_Button(50,50,DEBUG.txExMark,(new InterfaceCallback_showbg())));


    UI_List List(300,500,DEBUG.txExMark);

    UI_Bar* bar = new UI_Bar(&GRAY,UI_BAR_VERTICAL);
    bar->addItem(bar1);
    bar->addItem(bar2);
    bar->addItem(&List);

    UI_Group group(UI_GROUP_CENTER,UI_GROUP_CENTER, bar);

    UI_Manager um;

    um.addGroup(&group);

    ///Run the editor
    while( !end )
    {
        //Events
        while( SDL_PollEvent( &event ) )
        {
            if(  event.type == SDL_QUIT || ( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE ) )
            end = true;
            else if( event.type == SDL_KEYDOWN )
            {
                if( event.key.keysym.sym == SDLK_LCTRL )
                bCtrlDown = true;
            }
            else if( event.type == SDL_KEYUP )
            {
                bCtrlDown = false;
            }
            else if( event.type == SDL_MOUSEBUTTONDOWN )
            {
                if( bCtrlDown )
                {
                    bMouseDrag = true;
                    iInitial_x = event.button.x;
                    iInitial_y = event.button.y;
                }

            }
            else if( event.type == SDL_MOUSEBUTTONUP )
            {
                bMouseDrag = false;
            }

            ENGINE_HandleEvent_resizeWindow(&event);
            //External events
            INTERFACE.handle_events(event);

            um.handleEvents(&event);
        }
        ///Logics
         INTERFACE.handle_logics();

        ///RENDER
        //Clean the screen
        SDL_SetRenderDrawColor(RENDER_MAIN,0,0,0,255);
        SDL_RenderClear(RENDER_MAIN);



        INTERFACE.showNormaMap();
        //INTERFACE.showbg();

        //INTERFACE.showMouse();

        um.draw();


        //Update the screen
        SDL_RenderPresent(RENDER_MAIN);

    }


    ///Cleanup

    //PIXELS.cleanup();
    INTERFACE.cleanup();
    ENGINE_Quit();

    return 0;
}
