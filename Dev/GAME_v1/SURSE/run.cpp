#include "run.h"
#include "console.h"
#include "misc.h"




void RUN_GAME(){

    bool RUN = true;


    //TEST ONLY
    PLAYER.TeleportXY( 201,201 );
    PLAYER.TurnBody( RIGHT );
    PLAYER.teleportZone( 1 );

    INVENTORY.init_bags();
    INVENTORY.addTestItems();


    /// --- MAIN GAME LOOP ---
    while( RUN ){

    CONTROL.resetTimeFrame();

    ///EVENTS
    while( SDL_PollEvent( &event ) ){

    if( CONS.is_shown() )
        CONS.sendEvent( &event );


    if( event.window.windowID == WINDOW_MAIN_ID )
    {
        ENGINE_HandleEvents( &event );

        // SEND EVENTS
        if( PLAYER.SPEAK_NPC )          DIALOG.handle_events(event);

        if( QUEST.SHOW_QUEST )          QUEST .handle_events(event);

        if( LOOT.SHOW_LOOT )            LOOT  .handle_events( event );

        PLAYER.handleEvents( &event );

        // -- X Quit event --
        if( event.window.event == SDL_WINDOWEVENT_CLOSE || event.type == SDL_QUIT )
            RUN = false;
        else
        // -- KEYDOWN --
        if( event.type == SDL_KEYDOWN )
        {

            if( event.key.keysym.sym == SDLK_ESCAPE ) {
                RUN = false;
            }
            else if( event.key.keysym.sym == SDLK_i )            //INVENTORY
            {
                ///pause_timers();
                INVENTORY.open();
                ///resume_timers();
            }
            else
            if( event.key.keysym.sym == SDLK_p )            //QUEST PANNEL
            {
                QUEST.SHOW_QUEST = !QUEST.SHOW_QUEST;
            }
            else
            if( event.key.keysym.sym == SDLK_BACKQUOTE )
            {
                CONS.switch_if_display();
            }
            else
            if( event.key.keysym.sym == SDLK_F11 )
            {
                if( ENGINE_displaymode == DISPLAYMODE_FREE ) ENGINE_displaymode = DISPLAYMODE_LETTERBOX;
                else
                ENGINE_displaymode = DISPLAYMODE_FREE;
            }
        }
    }
}

    ///LOGICS
    CONTROL.updateFPS();

    handle_GAMEevents();

    SCRIPT.handle();

    PLAYER.handleMovement();
    PLAYER.handleMisc();

    GAME_MAP.handle_logics();

    GAME_NPCS.handle_interaction();
    ALLANIMES.handle_anime();

    if( PLAYER.SPEAK_NPC ) DIALOG.handle_logics();

    // ===== RENDER =====
    // clear screen
    SDL_SetRenderDrawColor( RENDER_MAIN,80,80,80,255 );
    SDL_RenderClear( RENDER_MAIN );

    GAME_MAP.display_back();

    GAME_NPCS.show( BACK );

    ///DISPLAY HERO

    PLAYER.draw();

    ALLANIMES.showanimes();

    GAME_NPCS.show(FORE);

    //GAME_MAP.display_fore();


    COMBAT.showtexts();


    if( PLAYER.SPEAK_NPC ) {
        DIALOG.show();
    } else {
        cInterface.display();
    }


    if( LOOT.SHOW_LOOT ) {
        LOOT.show();
    }

    if(QUEST.SHOW_QUEST) {
        QUEST.show();
    }


    CONS.draw();

    ENGINE_RenderPresent();


    ///FPS Cap
    CONTROL.limitFPS();

    CONTROL.countFrame();

    }
}
