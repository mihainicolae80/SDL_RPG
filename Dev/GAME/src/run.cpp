#include "run.h"
#include "console.h"
#include "misc.h"




void RUN_GAME(){

	bool run = true;


    //TEST ONLY
	PLAYER.TeleportXY(201, 201);
	PLAYER.TurnBody(RIGHT);
	PLAYER.teleportZone(1);

    INVENTORY.init_bags();
    INVENTORY.addTestItems();

    /// --- MAIN GAME LOOP ---
	while(run) {
		CONTROL.resetTimeFrame();

		/// EVENTS
		while(SDL_PollEvent(&event)){

		if(CONS.is_shown()) {
			CONS.sendEvent(&event);
		}


		if(WINDOW_MAIN_ID == event.window.windowID) {
			ENGINE_HandleEvents(&event);

			// SEND EVENTS
			if(PLAYER.SPEAK_NPC){
				DIALOG.handle_events(event);
			}
			if(QUEST.SHOW_QUEST) {
				QUEST.handle_events(event);
			}
			if(LOOT.SHOW_LOOT) {
				LOOT.handle_events(event);
			}

			PLAYER.handleEvents(&event);

			// -- X Quit event --
			if(event.window.event == SDL_WINDOWEVENT_CLOSE || event.type == SDL_QUIT) {
				run = false;
			}
			// -- KEYDOWN --
			else if(SDL_KEYDOWN == event.type) {

				if(SDLK_ESCAPE == event.key.keysym.sym) {
					run = false;
				}
				//INVENTORY
				else if(SDLK_i == event.key.keysym.sym) {
					///pause_timers();
					INVENTORY.open();
					///resume_timers();
				}
				//QUEST PANNEL
				else if(SDLK_p == event.key.keysym.sym) {
					QUEST.SHOW_QUEST = !QUEST.SHOW_QUEST;
				}
				else if(SDLK_BACKQUOTE == event.key.keysym.sym) {
					CONS.switch_if_display();
				}
				else if(SDLK_F11 == event.key.keysym.sym) {
					if( ENGINE_displaymode == DISPLAYMODE_FREE )  {
						ENGINE_displaymode = DISPLAYMODE_LETTERBOX;
					}
					else {
						ENGINE_displaymode = DISPLAYMODE_FREE;
					}
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

		if(PLAYER.SPEAK_NPC) {
			DIALOG.handle_logics();
		}

		// ===== RENDER =====
		// clear screen
		SDL_SetRenderDrawColor(RENDER_MAIN, 80, 80, 80, 255);
		SDL_RenderClear(RENDER_MAIN);

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

		if(LOOT.SHOW_LOOT) {
			LOOT.show();
		}

		if(QUEST.SHOW_QUEST) {
			QUEST.show();
		}

		CONS.draw();

		ENGINE_RenderPresent();

		///FPS Cap
		//CONTROL.limitFPS();

		CONTROL.countFrame();

    }
}
