#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

extern class CLS_SETTINGS {
public:
	CLS_SETTINGS();

	void load();
	int  getScreenWidth();
	int  getScreenHeight();
	bool getFullScreen();
	int  getRenderScaleQuality();

private:
	int _screen_width, _screen_height, _render_scalequality;
	bool _fullscreen;

} SETTINGS;

#endif // SETTINGS_H_INCLUDED
