#pragma once

#include <functional>

#include "src/ui/Rect.h"
#include "CallbackClass.h"
#include "src/base.h"
#include "src/Interface.h"
#include "IItem.h"


namespace engine
{
namespace ui
{

using namespace std;

enum class ButtonState
{
    NEUTRAL,
    HOVERED,
    PRESSED
};

/*
    User Interface Button with OnPressed event
*/
class Button : public IItem
{
public:

     /*The texture will NOT be
    Destroyed Automatically*/
    Button(int x, int y, int width, int height);
    ~Button();

    bool handleEvents(SDL_Event *event);
    void draw(void);

    void setForegroundColor(SDL_Color color);
    void setForegroundText(string text);

    void setBackgroundTexture(SDL_Texture * texture);
    void setOnClick(function<void(void)> callback);

    // TODO: stubs; can be simplified by removing?
    void setX(int x);
    void setY(int y);
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    void updateItems();


private:

    // border
    void drawBorder(void);

    uint32_t m_borderWidthPix = 2;


    // background
    void drawBackground(void);

    SDL_Color m_backgroundColor = BLACK;
    SDL_Texture * m_backgroundTexture = NULL;

    // foreground
    void updateForeground(void);
    void drawForeground(void);

    bool isForegroundUpdated = false;
    SDL_Color m_foregroundColor = BLACK;
    string m_foregroundText = string();
    SDL_Texture * m_foregroundTextRaster = NULL;

    Rect m_rect;

    ButtonState m_state;
    function<void(void)> m_onClick;
};

}
}
