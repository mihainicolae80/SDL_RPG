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

/*
    User Interface Button with OnPressed event
*/
class Button : public IItem
{
public:

     /*The texture will NOT be
    Destroyed Automatically*/
    Button(int x, int y, int width, int height, SDL_Texture * texture, function<void(void)> callback = []{});
    ~Button();

    bool handleEvents(SDL_Event *event);
    void draw();

    // TODO: stubs; can be simplified by removing?
    void setX(int x);
    void setY(int y);
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    void updateItems();


private:
    Rect m_rect;
    bool m_pressed;

    //Callback function to be called on button press
    function<void(void)> m_callback;

    SDL_Texture * m_texture;
};

}
}
