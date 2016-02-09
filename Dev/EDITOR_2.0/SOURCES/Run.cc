#include "Run.h"

int runEditor(void){

    bool end = false;
    int error = 0;
    SDL_Event event;

    ///Initlialize
    error = ENGINE_Init();
    if(error) return error;

    INTERFACE.init();

    PIXELS.init();

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

            //External events
            INTERFACE.handle_events( event );
        }
        ///Logics
         INTERFACE.handle_logics();

        ///RENDER
        //Clean the screen
        SDL_SetRenderDrawColor(RENDER_MAIN,0,0,0,255);
        SDL_RenderClear(RENDER_MAIN);

        //INTERFACE.showNormaMap();
        //INTERFACE.showbg();

        //INTERFACE.showMouse();


        //Update the screen
        SDL_RenderPresent(RENDER_MAIN);

    }


    ///Cleanup

    //PIXELS.cleanup();
    INTERFACE.cleanup();
    ENGINE_Quit();

    return 0;
}
