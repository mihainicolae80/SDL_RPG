#pragma once
#include <cstdint>

namespace engine::ui
{

struct ContextDefaults
{
    // colors as ARGB, A channel inverted (0 - opaque, FF - transparent)
    uint32_t colorButtonFgIdle          = 0xF3E5A6;
    
    uint32_t colorButtonBgIdle          = 0x404040;
    uint32_t colorButtonBgPress         = 0x5B5B3C;
    uint32_t colorButtonBgDisabled      = 0xBEBEBE;

    uint32_t colorButtonBorderIdle      = 0x000000;
    uint32_t colorButtonBorderHover     = 0x646464;
};

} // namespace engine::ui