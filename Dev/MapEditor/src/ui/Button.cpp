#include "Button.h"

namespace engine
{
namespace ui
{



Button::Button(int x, int y, int width, int height)
{
    m_state = ButtonState::NEUTRAL;
    m_onClick = []{};
    m_rect.m_rect.x = x;
    m_rect.m_rect.y = y;
    m_rect.m_rect.w = width;
    m_rect.m_rect.h = height;

    m_backgroundColor = GRAY;
}


Button::~Button(){

}


bool Button::handleEvents(SDL_Event * event)
{
    bool isEventCaptured = false;
    SDL_Point mousePoint;


    if(event->type == SDL_MOUSEBUTTONDOWN
	&& event->button.button == SDL_BUTTON_LEFT) {

        mousePoint.x = event->button.x;
        mousePoint.y = event->button.y;

        if (m_rect.ContainsPoint(mousePoint))
        {
            m_state = ButtonState::PRESSED;
            isEventCaptured = true;
        }
    }
    else if(event->type == SDL_MOUSEBUTTONUP
		 && event->button.button == SDL_BUTTON_LEFT) {

        mousePoint.x = event->button.x;
        mousePoint.y = event->button.y;

        if (ButtonState::PRESSED == m_state)
        {
            if (m_rect.ContainsPoint(mousePoint))
            {
                m_onClick();
                isEventCaptured = true;
                m_state = ButtonState::HOVERED;
			}
            else
            {
                m_state = ButtonState::NEUTRAL;
            }
		}
    }
    else if (event->type == SDL_MOUSEMOTION)
    {
        mousePoint.x = event->motion.x;
        mousePoint.y = event->motion.y;

        // moving mouse on the button
        if (m_rect.ContainsPoint(mousePoint))
        {
            if (ButtonState::NEUTRAL == m_state)
            {
                m_state = ButtonState::HOVERED;
            }
        }
        // moving mouse away from the button
        else if (!m_rect.ContainsPoint(mousePoint))
        {
            if (ButtonState::NEUTRAL != m_state)
            {
                m_state = ButtonState::NEUTRAL;
            }
        }
    }

    return isEventCaptured;
}

void Button::draw(void)
{
    drawBackground();
    drawForeground();
    drawBorder();
}

void Button::setBackgroundTexture(SDL_Texture * texture)
{
    m_backgroundTexture = texture;
}

void Button::setForegroundColor(SDL_Color color)
{
    isForegroundUpdated = false;
    m_foregroundColor = color;
}

void Button::setForegroundText(string text)
{
    isForegroundUpdated = false;
    m_foregroundText = text;
}

void Button::setOnClick(function<void ()> callback)
{
    m_onClick = callback;
}

void Button::drawBackground(void)
{
    if (NULL == m_backgroundTexture)
    {
        SDL_SetRenderDrawColor(RENDER_MAIN, m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, m_backgroundColor.a);
        SDL_RenderFillRect(RENDER_MAIN, &m_rect.m_rect);
    }
    else
    {
        SDL_RenderCopy(RENDER_MAIN, m_backgroundTexture, nullptr, &m_rect.m_rect);
    }

}

void Button::updateForeground(void)
{
    if (!isForegroundUpdated)
    {
        if (NULL != m_foregroundTextRaster)
        {
            SDL_DestroyTexture(m_foregroundTextRaster);
        }

        if ('\0' != m_foregroundText.c_str()[0])
        {
            auto surf = TTF_RenderText_Blended(Font24, m_foregroundText.c_str(), m_foregroundColor);
            m_foregroundTextRaster = SDL_CreateTextureFromSurface(RENDER_MAIN, surf);
        }
        else
        {
            m_foregroundTextRaster = NULL;
        }

        isForegroundUpdated = true;
    }
}

void Button::drawForeground(void)
{
    updateForeground();

    // center text
    int rasterW, rasterH;
    SDL_QueryTexture(m_foregroundTextRaster, NULL, NULL, &rasterW, &rasterH);

    SDL_Rect rasterRect;

    rasterRect.x = m_rect.m_rect.x + (m_rect.m_rect.w - rasterW) / 2;
    rasterRect.y = m_rect.m_rect.y + (m_rect.m_rect.h - rasterH) / 2;
    rasterRect.w = rasterW;
    rasterRect.h = rasterH;

    // render foreground text raster
    SDL_RenderCopy(RENDER_MAIN, m_foregroundTextRaster, NULL, &rasterRect);
}

void Button::drawBorder(void)
{
    SDL_Color borderColor;
    borderColor.a = 255;

    switch (m_state)
    {
    case ButtonState::NEUTRAL:
        borderColor = DARKGRAY;
    break;

    case ButtonState::HOVERED:
        borderColor = GRAY;
    break;

    case ButtonState::PRESSED:
        borderColor = LIGHTGRAY;
    break;
    }

    SDL_SetRenderDrawColor(
        RENDER_MAIN,
        borderColor.r,
        borderColor.g,
        borderColor.b,
        borderColor.a
    );

    SDL_Rect rect;

    rect.x = m_rect.m_rect.x;
    rect.y = m_rect.m_rect.y;
    rect.w = m_borderWidthPix;
    rect.h = m_rect.m_rect.h;
    SDL_RenderFillRect(RENDER_MAIN, &rect);

    rect.x = m_rect.m_rect.x + m_rect.m_rect.w - m_borderWidthPix;
    SDL_RenderFillRect(RENDER_MAIN, &rect);

    rect.x = m_rect.m_rect.x;
    rect.y = m_rect.m_rect.y;
    rect.w = m_rect.m_rect.w;
    rect.h = m_borderWidthPix;
    SDL_RenderFillRect(RENDER_MAIN, &rect);

    rect.x = m_rect.m_rect.x;
    rect.y = m_rect.m_rect.y + m_rect.m_rect.h - m_borderWidthPix;
    rect.w = m_rect.m_rect.w;
    rect.h = m_borderWidthPix;
    SDL_RenderFillRect(RENDER_MAIN, &rect);
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
