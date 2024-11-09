#pragma once

#include <string_view>
#include <string>
#include "SDL2/SDL.h"
#include "IItem.h"

namespace engine::ui
{

class Label: public IItem
{
public:
    Label(int x, int y, int width, int height, std::string_view text);
    void setBackgroundColor(SDL_Color color);
    void setBorderColor(SDL_Color color);
    void setForegroundColor(SDL_Color color);
    void setForegroundText(std::string_view text);
    // NOTE: many of there are not terribly useful right now
    void setX(int x) override;
    void setY(int y) override;
    int getX() override;
    int getY() override;
    int getWidth() override;
    int getHeight() override;
    void draw() override;
    bool handleEvents(SDL_Event&) override;

private:
    void updateForeground();
    void drawBackground();
    void drawForeground();
    void drawBorder();

    int m_x, m_y;
    int m_width, m_height;
    SDL_Color m_bg_color;
    SDL_Color m_border_color;
    std::string m_fg_text;
    bool m_is_fg_dirty; // 'dirty' = foreground must be updated
    SDL_Color m_fg_color;
    SDL_Surface *m_fg_raster_surf;
    SDL_Texture *m_fg_raster_tex;
};

} // namespace engine::ui
