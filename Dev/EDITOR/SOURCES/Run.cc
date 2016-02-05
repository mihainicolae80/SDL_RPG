#include "Run.h"

int runEditor(void){

    bool end = false;

    ///Initlialize
    if( Init() == false) return 5;

    Pixels.init();
    INTERFACE.init();

    SDL_EnableUNICODE( SDL_ENABLE );

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
        //Logics
        INTERFACE.handle_logics();
        ///RENDER

        //Clean the screen
        SDL_FillRect(screen,&screen->clip_rect,SDL_MapRGB(screen->format,0,0,0));//0xFF

        INTERFACE.showNormaMap();
        INTERFACE.showbg();

        INTERFACE.showMouse();



        SDL_Flip( screen );
    }


    ///Cleanup
    cleanup();
    SDL_EnableUNICODE( SDL_DISABLE );

    return 0;
}
