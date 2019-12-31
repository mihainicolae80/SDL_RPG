#include "misc.h"


CLS_CONTROL::CLS_CONTROL(void){

ofFPScounter.open("DEBUG//FPS Count.txt");
second_counter = SDL_GetTicks();
}
CLS_CONTROL::~CLS_CONTROL(void){

ofFPScounter.close();
}
void CLS_CONTROL::updateFPS(){

if( SDL_GetTicks() - second_counter >= 990)
{
    second_counter = SDL_GetTicks();

    std::stringstream ssaux;
    ssaux << "RPG " << frames;
    SDL_SetWindowTitle( WINDOW_MAIN, ssaux.str().c_str() );

    frames = 0;
}

}
void CLS_CONTROL::countFrame(void){
frames++;
}
void CLS_CONTROL::limitFPS(){

if( (SDL_GetTicks()-time_frame) < 1000/FPS_CAP )
{
    SDL_Delay( (1000/FPS_CAP)-(SDL_GetTicks()-time_frame) );
}

}
void CLS_CONTROL::resetTimeFrame(){
time_frame = SDL_GetTicks();
}
