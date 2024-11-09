#pragma once

#include <vector>
#include "ui/iitem.h"
#include "ui/button.h"
#include "ui/label.h"

namespace engine::ui
{

class TextureList: public IItem
{
public:
    TextureList(const ContextDefaults& defaults, int x, int y, int width, int height);
    void setX(int x) override;
    void setY(int y) override;
    int getX() override;
    int getY() override;
    int getWidth() override;
    int getHeight() override;
    void draw() override;
    bool handleEvents(SDL_Event&) override;

private:

    void navigatePrev();
    void navigateNext();

    Label m_lb_bg;
    std::vector<Button*> m_texture_buttons;
    Button m_btn_navigate_prev;
    Button m_btn_navigate_next;
    Label m_lb_crt_page;
};

} // namespace engine::ui
