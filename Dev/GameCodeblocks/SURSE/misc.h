#ifndef MISC_H_INCLUDED
#define MISC_H_INCLUDED

#include <fstream>
#include "SDL2/SDL.h"
#include "main_headers.h"
#include "base.h"


extern class CLS_CONTROL{
private:
std::ofstream ofFPScounter;
int frames;
Uint32 second_counter,time_frame;

public:
CLS_CONTROL(void);
~CLS_CONTROL(void);
void updateFPS(void);
void countFrame(void);
void limitFPS(void);
void resetTimeFrame(void);
}CONTROL;

#endif // MISC_H_INCLUDED
