#include "label.h"
#include <SDL2/SDL_ttf.h>
#include "base.h"

namespace engine::ui
{

Label::Label(int x, int y, int width, int height, std::string_view text)
: m_x(x), m_y(y), m_width(width), m_height(height), 
  m_bg_color(TRANSPARENT), m_border_color(TRANSPARENT),
  m_fg_text(text), 
  m_is_fg_dirty(true), m_fg_color(BLACK),
  m_fg_raster_surf(nullptr), m_fg_raster_tex(nullptr)
 {}

// Label::Label(int x, int y)
// : Label(x, y, {}) {}

void Label::setBackgroundColor(SDL_Color color)
{
    m_bg_color = color;
}

void Label::setBorderColor(SDL_Color color)
{
    m_border_color = color;
}

void Label::setForegroundColor(SDL_Color color)
{
    m_fg_color = color;
    m_is_fg_dirty = true;
}

void Label::setForegroundText(std::string_view text)
{
    m_fg_text = text;
    m_is_fg_dirty = true;
}

void Label::setX(int x)
{
    m_x = x;
}

void Label::setY(int y)
{
    m_y = y;
}

int Label::getX()
{
    return m_x;
}

int Label::getY()
{
    return m_y;
}

int Label::getWidth()
{
    return m_width;
}

int Label::getHeight()
{
    return m_height;
}

void Label::updateForeground()
{
    if (m_is_fg_dirty)
    {
        if (m_fg_raster_surf)
        {
            SDL_FreeSurface(m_fg_raster_surf);
            m_fg_raster_surf = nullptr;
        }

        if (m_fg_raster_tex)
        {
            SDL_DestroyTexture(m_fg_raster_tex);
            m_fg_raster_tex = nullptr;

        }

        m_fg_raster_surf = TTF_RenderText_Blended(Font24, m_fg_text.c_str(), m_fg_color);
        m_fg_raster_tex = SDL_CreateTextureFromSurface(RENDER_MAIN, m_fg_raster_surf);

        m_is_fg_dirty = false;
    }
}

void Label::draw()
{
    updateForeground();
    drawBackground();
    drawForeground();
    drawBorder();
}

void Label::drawBackground()
{
    SDL_SetRenderDrawColor(
        RENDER_MAIN,
        m_bg_color.r,
        m_bg_color.g,
        m_bg_color.b,
        m_bg_color.a
    );

    SDL_Rect rect;
    rect.x = m_x;
    rect.y = m_y;
    rect.w = m_width;
    rect.h = m_height;
    SDL_RenderFillRect(RENDER_MAIN, &rect);
}

void Label::drawForeground()
{
    int tex_w, tex_h;
    SDL_QueryTexture(m_fg_raster_tex, NULL, NULL, &tex_w, &tex_h);

    SDL_SetRenderDrawBlendMode(RENDER_MAIN, SDL_BLENDMODE_BLEND);

    // render foreground text raster
    SDL_Rect rect;
    rect.x = m_x;
    rect.y = m_y;
    rect.w = tex_w;
    rect.h = tex_h;
    SDL_RenderCopy(RENDER_MAIN, m_fg_raster_tex, NULL, &rect);
}

void Label::drawBorder()
{
    SDL_SetRenderDrawColor(
        RENDER_MAIN,
        m_border_color.r,
        m_border_color.g,
        m_border_color.b,
        m_border_color.a
    );

    SDL_SetRenderDrawBlendMode(RENDER_MAIN, SDL_BLENDMODE_BLEND);

    SDL_Rect rect;
    rect.x = m_x;
    rect.y = m_y;
    rect.w = m_width;
    rect.h = m_height;
    SDL_RenderDrawRect(RENDER_MAIN, &rect);
}

bool Label::handleEvents(SDL_Event&)
{
    return false;
}

} // namespace engine::ui