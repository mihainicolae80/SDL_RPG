#include <iostream>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "map_editor.h"
#include "base.h"
#include "Interface.h"
#include "textures.h"
#include "debug.h"
#include "ui/context.h" 
#include "ui/button.h"
#include "ui/label.h"

using namespace engine;
using namespace std;

static void setup_ui(ui::Context&);

void MapEditor::Run(void)
{
    int error = ENGINE_Init();
    if(error) {
        exit(error);
    }

    ui::Context ui_context;
    setup_ui(ui_context);

    INTERFACE.init();
    PIXELS.init();

    // Program Loop
    bool runLoop = true;
    while(runLoop) {

        // === Event Handling ===
        SDL_Event event;
        while(SDL_PollEvent(&event)) {

            if(event.type == SDL_QUIT)
            {
                runLoop = false;
            }
            else if(SDL_KEYDOWN == event.type)
            {
                if(SDLK_LCTRL == event.key.keysym.sym)
                {
                    key_ctrl_down = true;
                }
                else if ((SDLK_ESCAPE == event.key.keysym.sym)
                      || (SDLK_q == event.key.keysym.sym))
                {
                    runLoop = false;
                }
            }
            else if(SDL_KEYUP == event.type)
            {
                key_ctrl_down = false;
            }
            else if(SDL_MOUSEBUTTONDOWN == event.type)
            {
                if(key_ctrl_down)
                {
                    btn_mouse_down = true;
                    mouse_drag_start_x = event.button.x;
                    mouse_drag_start_y = event.button.y;
                }

            }
            else if(event.type == SDL_MOUSEBUTTONUP)
            {
                btn_mouse_down = false;
            }

            ENGINE_HandleEvent_resizeWindow(&event);
            ui_context.handleEvents(event);
            INTERFACE.handle_events(event);
        }

        // === LOGICS ===
        INTERFACE.handle_logics();

        // === RENDER ===
        //Clean the screen
        SDL_SetRenderDrawColor(RENDER_MAIN, 0, 0, 0, 255);
        SDL_RenderClear(RENDER_MAIN);

        ui_context.draw();

        INTERFACE.showNormaMap();
        INTERFACE.showbg();
        INTERFACE.showMouse();

        //Update the screen
        SDL_RenderPresent(RENDER_MAIN);
    }

    /// Cleanup
    //PIXELS.cleanup();
    INTERFACE.cleanup();
    ENGINE_Quit();
}

static void setup_ui(ui::Context& ui_context)
{
    const int origin_x = 700;
    const int origin_y = 200;
    const int btn_width = 150;
    const int btn_width_narrow = 60;
    const int btn_height = 30;

    auto* btn_building = ui_context.MakeButton(origin_x, origin_y, "Building");
    btn_building->setWidth(btn_width);
    btn_building->setHeight(btn_height);
    btn_building->setForegroundColor(0xAD6A4F);
    btn_building->setOnClick([] {
        cout << "Clicked Building" << endl;
    });

    auto* btn_objects = ui_context.MakeButton(origin_x, origin_y + btn_height, "Objects");
    btn_objects->setWidth(btn_width);
    btn_objects->setHeight(btn_height);
    btn_objects->setForegroundColor(0xADCFFF);
    btn_objects->setOnClick([] {
        cout << "Clicked Objects" << endl;
    });

    auto* btn_interactions = ui_context.MakeButton(origin_x, origin_y + btn_height * 2, "Interactions");
    btn_interactions->setWidth(btn_width);
    btn_interactions->setHeight(btn_height);
    btn_interactions->setForegroundColor(0xB1BBED);
    btn_interactions->setOnClick([] {
        cout << "Clicked Interactions" << endl;
    });

    auto* btn_NPCs = ui_context.MakeButton(origin_x, origin_y + btn_height * 3, "NPCs");
    btn_NPCs->setWidth(btn_width);
    btn_NPCs->setHeight(btn_height);
    btn_NPCs->setForegroundColor(0xFF9849);
    btn_NPCs->setOnClick([] {
        cout << "Clicked NPCs" << endl;
    });

    auto* btn_new = ui_context.MakeButton(origin_x + btn_width, origin_y, "New");
    btn_new->setWidth(btn_width_narrow);
    btn_new->setHeight(btn_height);
    btn_new->setOnClick([] {
        cout << "Clicked New" << endl;
    });

    auto* btn_save = ui_context.MakeButton(
        origin_x + btn_width + btn_width_narrow,
        origin_y,
        "Save"
    );
    btn_save->setWidth(btn_width_narrow);
    btn_save->setHeight(btn_height);
    btn_save->setOnClick([] {
        cout << "Clicked Save" << endl;
    });

    auto* btn_load = ui_context.MakeButton(
        origin_x + btn_width + btn_width_narrow * 2,
        origin_y,
        "Load"
    );
    btn_load->setWidth(btn_width_narrow);
    btn_load->setHeight(btn_height);
    btn_load->setOnClick([] {
        cout << "Clicked Load" << endl;
    });

    auto* btn_fullscreen = ui_context.MakeButton(
        origin_x + btn_width + btn_width_narrow * 3,
        origin_y,
        "Full Screen"
    );
    btn_fullscreen->setWidth(btn_width);
    btn_fullscreen->setHeight(btn_height);
    btn_fullscreen->setOnClick([] {
        cout << "Clicked Full Screen" << endl;
    });

    auto *texture_list = ui_context.MakeTextureList(
        origin_x,
        origin_y  + btn_height * 3,
        btn_width,
        300
    );
}