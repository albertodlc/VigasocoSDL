// SDLDrawPlugin8bpp.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "SDLVideoPlugins.h"
#include "IPalette.h"

void SDLDrawPlugin8bpp::updateFullPalette(IPalette *palette)
{
    for (int i = 0; i < palette->getTotalColors(); i++){
        UINT8 r, g, b;
        palette->getColor(i, r, g, b);
        _palette[i] = SDL_MapRGB(pixelFormat, r, g, b);
        // TEMP DEBUG — print first 8 colors
        if (i < 8) fprintf(stderr, "color[%d] r=%d g=%d b=%d mapped=0x%08x\n", i, r, g, b, _palette[i]);
    }
}

void SDLDrawPlugin8bpp::update(IPalette *palette, int data)
{
    if (data != -1){
        UINT8 r, g, b;
        palette->getColor(data, r, g, b);
        // TEMP DEBUG
        fprintf(stderr, "update color[%d] r=%d g=%d b=%d\n", data, r, g, b);
        SDL_LockMutex(cs);
        _palette[data] = SDL_MapRGB(pixelFormat, r, g, b);
        SDL_UnlockMutex(cs);
    } else {
        updateFullPalette(palette);
    }
}

void SDLDrawPlugin8bpp::render(bool throttle)
{
    SDL_LockMutex(cs);
    SDLBasicDrawPlugin<UINT32>::render(throttle);  // ← UINT8 → UINT32
    SDL_UnlockMutex(cs);
}

void SDLDrawPlugin8bpp::setPixel(int x, int y, int color)
{
    Uint32 *p = (Uint32 *)((Uint8 *)myPixels + y * _pitch + x * sizeof(Uint32));
    *p = _palette[color];
}