#include "src/main_headers.h"
#include "src/run.h"
#include "src/base.h"
#include "src/settings.h"



int main(int argc, char **argv)
{
    int error;

    (void)argc;
    (void)argv;


    // load settings
    SETTINGS.load();

    // init
    error = ENGINE_Init();
    if(error != 0) {
        return error;
    }

    // run game
    RUN_GAME();

    // clean up and quit engine
    ENGINE_Quit();

    return 0;
}
