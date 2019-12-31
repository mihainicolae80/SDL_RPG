#include "main_headers.h"
#include "run.h"
#include "base.h"
#include "settings.h"



int main( int argc,char **argv ){
    int error;

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
