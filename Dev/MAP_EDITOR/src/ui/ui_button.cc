#include "ui_button.h"

UI_Button::UI_Button(const UI_Rect &rect, SDL_Texture *tex, ui_on_button_click_t callback_click)
{

	this->_rect = rect;
    this->tex = tex;
	this->_callback_click = callback_click;

	this->pressed = false;
}


/*
UI_Button::~UI_Button(){

}
*/


bool UI_Button::handle_events(SDL_Event *event)
{
	// flag if event should be captured by item
	bool capture = false;
	SDL_Point click_point;


    //If the left mouse button was pressed
    if(event->type == SDL_MOUSEBUTTONDOWN
	&& event->button.button == SDL_BUTTON_LEFT) {

		click_point.x = event->button.x;
		click_point.y = event->button.y;

		// check button pressed
		this->pressed = _rect.contains(click_point);

		// capture event
		capture = capture || this->pressed;
    }
    //If the left mouse button was released
    else if(event->type == SDL_MOUSEBUTTONUP
		 && event->button.button == SDL_BUTTON_LEFT) {

		click_point.x = event->button.x;
		click_point.y = event->button.y;

		// button was pressed
		if (this->pressed) {
			// mouse released over pressed button
			if (_rect.contains(click_point)) {
				// callback if available
				if (_callback_click) {
					_callback_click();
				}

				// capture event
				capture = true;
			}

			this->pressed = false;
		}
    }

	return capture;
}

void UI_Button::draw()
{

	SDL_Rect sdl_rect, sdl_rect_pressed;

	_rect.copy_to_sdl(sdl_rect);

	if(!pressed){
		// TODO: add engine UI scale
		SDL_RenderCopy(RENDER_MAIN, tex, nullptr, &sdl_rect);
    }
	else {
		//Fill the button frame with black
		SDL_SetRenderDrawColor(RENDER_MAIN, BLACK.r, BLACK.g, BLACK.b, BLACK.a);
		SDL_RenderFillRect( RENDER_MAIN, &sdl_rect);

		// draw pressed button
		sdl_rect_pressed.x = _rect.x + 5;
		sdl_rect_pressed.y = _rect.y + 5;
		sdl_rect_pressed.w = _rect.w - 10;
		sdl_rect_pressed.h = _rect.h - 10;

		SDL_RenderCopy(RENDER_MAIN, tex, nullptr, &sdl_rect_pressed);
    }
}
