#include "run.h"
#include "console_extern.h"
#include "misc.h"




void RUN_GAME(){

bool RUN = true;


//TEST ONLY
PLAYER.TeleportXY( 101,101 );
PLAYER.TurnBody( RIGHT );
PLAYER.teleportZone( 1 );

INVENTORY.init_bags();
INVENTORY.addTestItems();


///MAIN GAME LOOP
while( RUN ){

CONTROL.resetTimeFrame();

///EVENTS
while( SDL_PollEvent( &event ) ){

CONSOLE.sendEvents( &event );


if( event.window.windowID == WINDOW_MAIN_ID )
{
    ENGINE_handleEvent_resizeWindow( &event );

    // SEND EVENTS

    if( PLAYER.SPEAK_NPC )          DIALOG.handle_events(event);

    if( QUEST.SHOW_QUEST )          QUEST .handle_events(event);

    if( LOOT.SHOW_LOOT )            LOOT  .handle_events( event );

    PLAYER.handleEvents( &event );


    if( event.window.event == SDL_WINDOWEVENT_CLOSE || event.type == SDL_QUIT )
        RUN = false;
    else
    if( event.type == SDL_KEYDOWN )
    {

        if( event.key.keysym.sym == SDLK_ESCAPE )       RUN = false;
        else
        if( event.key.keysym.sym == SDLK_i )            //INVENTORY
        {
            ///pause_timers();
            INVENTORY.open();
            ///resume_timers();
        }
        else
        if( event.key.keysym.sym == SDLK_p )            //QUEST PANNEL
            QUEST.SHOW_QUEST = !QUEST.SHOW_QUEST;

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







///RENDER

SDL_SetRenderDrawColor( RENDER_MAIN,80,80,80,255 );
SDL_RenderClear( RENDER_MAIN );

//MAP
GAME_MAP.display_back();

GAME_NPCS.show( BACK );

///DISPLAY HERO

PLAYER.draw();

ALLANIMES.showanimes();

GAME_NPCS.show( FORE );

GAME_MAP.display_fore();


COMBAT.showtexts();


if( PLAYER.SPEAK_NPC )     DIALOG.show();
else                       cInterface.display();


if( LOOT.SHOW_LOOT ) LOOT.show();

if( QUEST.SHOW_QUEST ) QUEST.show();



SDL_RenderPresent( RENDER_MAIN );


///FPS Cap
CONTROL.limitFPS();

CONTROL.countFrame();

}


}
