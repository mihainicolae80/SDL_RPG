#include "button.h"
#include "utils.h"

namespace engine::ui
{

Button::Button(const ContextDefaults& defaults, int x, int y, int width, int height, std::string_view text)
{
    m_border_color_idle     = HexToSDLColor(defaults.colorButtonBorderIdle);
    m_border_color_hover    = HexToSDLColor(defaults.colorButtonBorderHover);
    m_border_color          = m_border_color_idle;
    m_bg_color_idle         = HexToSDLColor(defaults.colorButtonBgIdle);
    m_bg_color_press        = HexToSDLColor(defaults.colorButtonBgPress);
    m_bg_color_disabled     = HexToSDLColor(defaults.colorButtonBgDisabled);
    m_bg_color              = m_bg_color_idle;
    m_fg_color              = HexToSDLColor(defaults.colorButtonFgIdle);

    m_is_fg_dirty = true;
    m_fg_text = text;
    
    m_state = ButtonState::IDLE;
    m_on_press = []{};
    m_rect.m_rect.x = x;
    m_rect.m_rect.y = y;
    m_rect.m_rect.w = width;
    m_rect.m_rect.h = height;
}

Button::~Button(){

}

bool Button::handleEvents(SDL_Event& event)
{
    if (ButtonState::DISABLED == m_state)
    {
        return false;
    }

    // from event to action
    bool is_captured = false;
    enum
    {
        ACTION_ENTER,
        ACTION_EXIT,
        ACTION_PRESS,
        ACTION_RELEASE,
        ACTION_NONE
    } action = ACTION_NONE;

    if(event.type == SDL_MOUSEBUTTONDOWN
	&& event.button.button == SDL_BUTTON_LEFT
    && m_rect.ContainsPoint(SDL_Point{event.button.x, event.button.y}))
    {
        is_captured = true;
        action = ACTION_PRESS;
    }
    else if(event.type == SDL_MOUSEBUTTONUP
         && event.button.button == SDL_BUTTON_LEFT
         && m_rect.ContainsPoint(SDL_Point{event.button.x, event.button.y}))
    {
        is_captured = true;
        action = ACTION_RELEASE;
    }
    else if (event.type == SDL_MOUSEMOTION)
    {
        if (m_rect.ContainsPoint(SDL_Point{event.motion.x, event.motion.y}))
        {
            // moving mouse on the button
            if (ButtonState::IDLE == m_state)
            {
                action = ACTION_ENTER;
            }
        }
        else
        {
            if ((ButtonState::HOVERED == m_state)
            ||  (ButtonState::PRESSED == m_state))
            {
                action = ACTION_EXIT;
            }
        }
    }

    // from action to state
    switch (action)
    {
    case ACTION_ENTER:
        m_state         = ButtonState::HOVERED;
        m_border_color  = m_border_color_hover;
        m_bg_color      = m_bg_color_idle;
        break;
    
    case ACTION_PRESS:
        m_state         = ButtonState::PRESSED;
        m_border_color  = m_border_color_hover;
        m_bg_color      = m_bg_color_press;
        if(m_on_press)
        {
            m_on_press();
        }
        break;

    case ACTION_EXIT:
    case ACTION_RELEASE:
        m_state         = ButtonState::IDLE;
        m_border_color  = m_border_color_idle;
        m_bg_color      = m_bg_color_idle;
        break;

    case ACTION_NONE:
        break;
    }

    return is_captured;
}

void Button::draw(void)
{
    drawBackground();
    drawForeground();
    drawBorder();
}

void Button::setBackgroundTexture(SDL_Texture * texture)
{
    m_bg_tex = texture;
}

void Button::setForegroundColor(uint32_t color)
{
    m_is_fg_dirty = true;
    m_fg_color = HexToSDLColor(color);
}

void Button::setForegroundText(std::string_view text)
{
    m_is_fg_dirty = true;
    m_fg_text = text;
}

void Button::setOnClick(function<void ()> callback)
{
    m_on_press = callback;
}

void Button::drawBackground(void)
{
    if (m_bg_tex)
    {
        SDL_RenderCopy(RENDER_MAIN, m_bg_tex, nullptr, &m_rect.m_rect);
    }
    else
    {
        SDL_SetRenderDrawColor(RENDER_MAIN, m_bg_color.r, m_bg_color.g, m_bg_color.b, m_bg_color.a);
        SDL_RenderFillRect(RENDER_MAIN, &m_rect.m_rect);
    }
}

void Button::updateForeground(void)
{
    if (m_is_fg_dirty)
    {
        if (m_fg_text_tex)
        {
            SDL_DestroyTexture(m_fg_text_tex);
        }

        if (m_fg_text_surf)
        {
            SDL_FreeSurface(m_fg_text_surf);
        }

        // text not empty
        if (m_fg_text.c_str()[0])
        {
            m_fg_text_surf = TTF_RenderText_Blended(Font24, m_fg_text.c_str(), m_fg_color);
            m_fg_text_tex  = SDL_CreateTextureFromSurface(RENDER_MAIN, m_fg_text_surf);
        }
        else
        {
            m_fg_text_tex = nullptr;
            m_fg_text_surf = nullptr;
        }

        m_is_fg_dirty = false;
    }
}

void Button::drawForeground(void)
{
    updateForeground();

    // center text
    int rasterW, rasterH;
    SDL_QueryTexture(m_fg_text_tex, nullptr, nullptr, &rasterW, &rasterH);

    SDL_Rect rect;
    rect.x = m_rect.m_rect.x + (m_rect.m_rect.w - rasterW) / 2;
    rect.y = m_rect.m_rect.y + (m_rect.m_rect.h - rasterH) / 2;
    rect.w = rasterW;
    rect.h = rasterH;

    // render foreground text raster
    SDL_RenderCopy(RENDER_MAIN, m_fg_text_tex, nullptr, &rect);
}

void Button::drawBorder(void)
{
    SDL_SetRenderDrawColor(
        RENDER_MAIN,
        m_border_color.r,
        m_border_color.g,
        m_border_color.b,
        m_border_color.a
    );

    SDL_Rect rect;
    rect.x = m_rect.m_rect.x;
    rect.y = m_rect.m_rect.y;
    rect.w = m_border_width;
    rect.h = m_rect.m_rect.h;
    SDL_RenderFillRect(RENDER_MAIN, &rect);

    rect.x = m_rect.m_rect.x + m_rect.m_rect.w - m_border_width;
    SDL_RenderFillRect(RENDER_MAIN, &rect);

    rect.x = m_rect.m_rect.x;
    rect.y = m_rect.m_rect.y;
    rect.w = m_rect.m_rect.w;
    rect.h = m_border_width;
    SDL_RenderFillRect(RENDER_MAIN, &rect);

    rect.x = m_rect.m_rect.x;
    rect.y = m_rect.m_rect.y + m_rect.m_rect.h - m_border_width;
    rect.w = m_rect.m_rect.w;
    rect.h = m_border_width;
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

void Button::setWidth(int width)
{
    m_rect.m_rect.w = width;
}

void Button::setHeight(int height)
{
    m_rect.m_rect.h = height;
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

}
