#include "Button.h"

namespace engine
{
namespace ui
{



Button::Button(int x, int y, int width, int height, SDL_Texture * texture, function<void(void)> callback)
: m_pressed(false), m_callback(callback), m_texture(texture)
{
    m_rect.m_rect.x = x;
    m_rect.m_rect.y = y;
    m_rect.m_rect.w = width;
    m_rect.m_rect.h = height;
}


Button::~Button(){

}


bool Button::handleEvents(SDL_Event * event)
{
	// flag if event should be captured by item
    bool isEventCaptured = false;
    SDL_Point pointClick;


    if(event->type == SDL_MOUSEBUTTONDOWN
	&& event->button.button == SDL_BUTTON_LEFT) {

        pointClick.x = event->button.x;
        pointClick.y = event->button.y;

		// check button pressed
        this->m_pressed = m_rect.ContainsPoint(pointClick);

		// capture event
        isEventCaptured = isEventCaptured || this->m_pressed;
    }
    else if(event->type == SDL_MOUSEBUTTONUP
		 && event->button.button == SDL_BUTTON_LEFT) {

        pointClick.x = event->button.x;
        pointClick.y = event->button.y;

		// button was pressed
        if (this->m_pressed)
        {
			// mouse released over pressed button
            if (m_rect.ContainsPoint(pointClick))
            {
				// callback if available
                m_callback();

				// capture event
                isEventCaptured = true;
			}

            this->m_pressed = false;
		}
    }

    return isEventCaptured;
}

void Button::draw()
{
    SDL_Rect rectSDL = m_rect.m_rect;

    if(!m_pressed){
		// TODO: add engine UI scale
        SDL_RenderCopy(RENDER_MAIN, m_texture, nullptr, &rectSDL);
    }
	else {
		//Fill the button frame with black
		SDL_SetRenderDrawColor(RENDER_MAIN, BLACK.r, BLACK.g, BLACK.b, BLACK.a);
        SDL_RenderFillRect( RENDER_MAIN, &rectSDL);

		// draw pressed button
        SDL_Rect rectSDLPressed;
        rectSDLPressed.x = rectSDL.x + 5;
        rectSDLPressed.y = rectSDL.y + 5;
        rectSDLPressed.w = rectSDL.w - 10;
        rectSDLPressed.h = rectSDL.h - 10;

        SDL_RenderCopy(RENDER_MAIN, m_texture, nullptr, &rectSDLPressed);
    }
}

void Button::setX(int x)
{
    m_rect.m_rect.x = x;
}

void Button::setY(int y)
{
    m_rect.m_rect.y = y;
}

int Button::getX()
{
    return m_rect.m_rect.x;
}

int Button::getY()
{
    return m_rect.m_rect.y;
}

int Button::getWidth()
{
    return m_rect.m_rect.w;
}

int Button::getHeight()
{
    return m_rect.m_rect.h;
}

void Button::updateItems()
{
    // stub
}


}
}
