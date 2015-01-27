#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

#include <fstream>

extern class CLS_SETTINGS{
private:

int _screen_width,_screen_height,_render_scalequality;
bool _fullscreen;


public:
     CLS_SETTINGS();
void load();
int  getScreenWidth();
int  getScreenHeight();
bool getFullScreen();
int  getRenderScaleQuality();

}SETTINGS;

#endif // SETTINGS_H_INCLUDED
