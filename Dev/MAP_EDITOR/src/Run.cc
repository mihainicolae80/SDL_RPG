#include "Run.h"

#include "textures.h"
#include "debug.h"
#include "ui/ui_window.h"
#include "ui/ui_button.h"



int run_editor(void)
{
	bool run = true;
    int error = 0;
    SDL_Event event;
	SDL_Texture *tex;


	// Initialize
    error = ENGINE_Init();
	if(error) {
		return error;
	}

	UI_Window win;


	tex = loadTex("textures/BUILDING/build/1.png");
	win.add_item(new UI_Button(UI_Rect(0, 0, 100, 100), tex, nullptr));
	win.add_item(new UI_Button(UI_Rect(50, 50, 100, 100), tex, nullptr));


	//INTERFACE.init();
	//PIXELS.init();

	// Program Loop
	while(run) {
        //Events
		while(SDL_PollEvent(&event)) {

			if(event.type == SDL_QUIT) {
				run = false;
			} else if(SDL_KEYDOWN == event.type) {
				if(SDLK_LCTRL == event.key.keysym.sym) {
					key_ctrl_down = true;
				} else if (SDLK_ESCAPE == event.key.keysym.sym) {
					run = false;
				}
			} else if(SDL_KEYUP == event.type) {
				key_ctrl_down = false;
			} else if(SDL_MOUSEBUTTONDOWN == event.type) {
				if(key_ctrl_down) {
					btn_mouse_down = true;
					mouse_drag_start_x = event.button.x;
					mouse_drag_start_y = event.button.y;
                }

			} else if(event.type == SDL_MOUSEBUTTONUP) {
				btn_mouse_down = false;
            }

            ENGINE_HandleEvent_resizeWindow(&event);

			win.handle_events(&event);

            //External events
			//INTERFACE.handle_events(event);
        }

		/// LOGICS

//      INTERFACE.handle_logics();

		/// RENDER
        //Clean the screen
		//SDL_SetRenderDrawColor(RENDER_MAIN, 0, 0, 0, 255);
		SDL_SetRenderDrawColor(RENDER_MAIN, 255, 255, 255, 255);
        SDL_RenderClear(RENDER_MAIN);

		win.draw();

//      INTERFACE.showNormaMap();
//		INTERFACE.showbg();
//		INTERFACE.showMouse();

        //Update the screen
        SDL_RenderPresent(RENDER_MAIN);
    }

	/// Cleanup
    //PIXELS.cleanup();
	//INTERFACE.cleanup();
    ENGINE_Quit();

    return 0;
}


/*
 ///// NEW UI TEST

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


	UI_List List(300, 500, DEBUG.sfExMark);

	UI_Bar* bar = new UI_Bar(&GRAY,UI_BAR_VERTICAL);
	bar->addItem(bar1);
	bar->addItem(bar2);
	bar->addItem(&List);

	UI_Group group(UI_GROUP_CENTER, UI_GROUP_CENTER, bar);

	UI_Manager um;

	um.addGroup(&group);


	// TODO: in run loop
	um.handleEvents(&event);
	um.draw();
*/
