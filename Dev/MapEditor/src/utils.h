#pragma once
#include <cstdint>
#include "SDL2/SDL_pixels.h"
// various utils; probably should be moved from here in the future

static inline SDL_Color HexToSDLColor(uint32_t hex)
{
    return SDL_Color {
        static_cast<uint8_t>((hex >> 16) & 0xFF), // r
        static_cast<uint8_t>((hex >>  8) & 0xFF), // g
        static_cast<uint8_t>((hex >>  0) & 0xFF), // b
        static_cast<uint8_t>((hex >> 24) & 0xFF)  // a
    };
}
