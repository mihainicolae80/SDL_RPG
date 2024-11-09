#include "Rect.h"

namespace engine::ui
{

bool engine::ui::Rect::ContainsPoint(SDL_Point p)
{
    return (p.x >= m_rect.x)
        && (p.x < (m_rect.x + m_rect.w))
        && (p.y >= m_rect.y)
        && (p.y < (m_rect.y + m_rect.h));
}

}