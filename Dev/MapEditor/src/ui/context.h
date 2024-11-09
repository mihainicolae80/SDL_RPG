#pragma once

#include <string_view>
#include <vector>
#include "context_defaults.h"
#include "iitem.h"
#include "button.h"
#include "texture_list.h"

namespace engine::ui
{

class Context
{
public:
    Context();
    ~Context();

    bool handleEvents(SDL_Event& event);
    void draw(void);

    Button* MakeButton(int x, int y, std::string_view text);
    TextureList* MakeTextureList(int x, int y, int width, int height);

    ContextDefaults defaults;

private:
    std::vector<IItem*> m_items;
};

} // namespace engine::ui
