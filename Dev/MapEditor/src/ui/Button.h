#pragma once

#include <functional>
#include <string_view>
#include <cstdint>
#include "src/ui/rect.h"
#include "src/base.h"
#include "src/Interface.h"
#include "IItem.h"
#include "context_defaults.h"


namespace engine::ui
{

enum class ButtonState
{
    IDLE,
    HOVERED,
    PRESSED,
    DISABLED
};

/*
    User Interface Button with OnPressed event
*/
class Button : public IItem
{
public:
    Button(const ContextDefaults& defaults, int x, int y, int width, int height, std::string_view text);
    ~Button();
    bool handleEvents(SDL_Event& event);
    void draw(void);
    void setBackgroundTexture(SDL_Texture * texture);
    void setOnClick(function<void(void)> callback);
    void setForegroundColor(uint32_t color);
    void setForegroundText(std::string_view text);
    void setX(int x);
    void setY(int y);
    void setWidth(int width);
    void setHeight(int height);
    int getX();
    int getY();
    int getWidth();
    int getHeight();

private:

    // border
    void drawBorder(void);

    uint32_t m_border_width = 1;
    SDL_Color m_border_color;
    SDL_Color m_border_color_idle;
    SDL_Color m_border_color_hover;

    // background
    void drawBackground(void);

    SDL_Texture* m_bg_tex = NULL;
    SDL_Color m_bg_color;
    // button colors should be part of a style object
    SDL_Color m_bg_color_idle;
    SDL_Color m_bg_color_press;
    SDL_Color m_bg_color_disabled;

    // foreground
    void updateForeground(void);
    void drawForeground(void);

    bool m_is_fg_dirty;
    SDL_Color m_fg_color;
    string m_fg_text;
    SDL_Surface* m_fg_text_surf;
    SDL_Texture* m_fg_text_tex;

    Rect m_rect;

    ButtonState m_state;
    function<void(void)> m_on_press;
};

}
