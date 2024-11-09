#include "ui/texture_list.h"

namespace engine::ui
{

TextureList::TextureList(const ContextDefaults& defaults, int x, int y, int width, int height)
: m_lb_bg(x, y, width, height, ""),
  m_texture_buttons(),
  m_btn_navigate_prev(defaults, x, y + height - 25, 30, 25, "<"),
  m_btn_navigate_next(defaults, x + 30, y + height - 25, 30, 25, ">"),
  m_lb_crt_page(x + 60, y + height - 25, 30, 25, "0/0")
{
    m_lb_bg.setBackgroundColor(WHITE);
}

void TextureList::setX(int x)
{

}

void TextureList::setY(int y)
{

}


int TextureList::getX()
{

}

int TextureList::getY()
{

}

int TextureList::getWidth()
{

}

int TextureList::getHeight()
{

}

void TextureList::draw()
{
    //m_lb_bg.draw();
    // for(auto* btn : m_texture_buttons)
    // {
    //     btn->draw();
    // }
    // m_btn_navigate_prev.draw();
    // m_btn_navigate_next.draw();
    // m_lb_crt_page.draw();
}

bool TextureList::handleEvents(SDL_Event& event)
{
    for(auto* btn : m_texture_buttons)
    {
        if (btn->handleEvents(event))
        {
            return true;
        }
    }

    if (m_btn_navigate_prev.handleEvents(event))
    {
        return true;
    }

    if (m_btn_navigate_next.handleEvents(event))
    {
        return true;
    }

    return false;
}

} // namespace engine::ui