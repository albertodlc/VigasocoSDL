// SDLDrawPlugin8bpp.cpp

#include "SDLVideoPlugins.h"
#include "IPalette.h"

void SDLDrawPlugin8bpp::updateFullPalette(IPalette *palette)
{
    for (int i = 0; i < palette->getTotalColors(); i++){
        UINT8 r, g, b;
        palette->getColor(i, r, g, b);
        _palette[i] = SDL_MapRGB(pixelFormat, r, g, b);
    }
}

void SDLDrawPlugin8bpp::update(IPalette *palette, int data)
{
    if (data != -1){
        UINT8 r, g, b;
        palette->getColor(data, r, g, b);
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
    SDLBasicDrawPlugin<UINT32>::render(throttle);
    SDL_UnlockMutex(cs);
}

void SDLDrawPlugin8bpp::setPixel(int x, int y, int color)
{
    Uint32 *p = (Uint32 *)((Uint8 *)myPixels + y * _pitch + x * sizeof(Uint32));
    *p = _palette[color];
}

void SDLDrawPlugin8bpp::print(int x, int y, const char* text)
{
    if (!_font || !text || text[0] == '\0') return;
    _printQueue.push_back({x, y, std::string(text)});
}

void SDLDrawPlugin8bpp::renderOverlays()
{
    if (!_font || _printQueue.empty()) return;

    for (const auto& req : _printQueue)
    {
        SDL_Color white = {255, 255, 255, 255};
        SDL_Surface* surf = TTF_RenderText_Solid(_font, req.text.c_str(), white);
        if (!surf){
            fprintf(stderr, "TTF_RenderText_Solid failed: %s\n", TTF_GetError());
            continue;
        }

        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        if (tex){
            SDL_Rect dst = {req.x, req.y, surf->w, surf->h};
            SDL_RenderCopy(renderer, tex, NULL, &dst);
            SDL_DestroyTexture(tex);
        }

        SDL_FreeSurface(surf);
    }

    _printQueue.clear();
}