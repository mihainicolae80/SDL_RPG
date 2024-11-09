#include "ui/context.h"
#include "utils.h"

namespace engine::ui
{

Context::Context()
{
    m_items.reserve(100);
}

Context::~Context()
{
    for(auto* item : m_items)
    {
        delete item;
    }
}

bool Context::handleEvents(SDL_Event& event)
{
    for(auto* item : m_items)
    {
        if (item->handleEvents(event))
        {
            return true;
        }
    }

    return false;
}

void Context::draw()
{
    for(auto* item : m_items)
    {
        item->draw();
    }
}

Button* Context::MakeButton(int x, int y, std::string_view text)
{
    auto* btn = new Button(defaults, x, y, 100, 50, text);
    m_items.push_back(btn);
    return btn;
}

TextureList* Context::MakeTextureList(int x, int y, int width, int height)
{
    auto* list = new TextureList(defaults, x, y, 100, 50);
    m_items.push_back(list);
    return list;
}

} // namespace engine::ui