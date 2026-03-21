// SDLVideoPlugins.h 
/////////////////////////////////////////////////////////////////////////////

#ifndef _SDL_VIDEO_PLUGINS_H_
#define _SDL_VIDEO_PLUGINS_H_

#include "SDLBasicDrawPlugin.h"
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> // Required for TTF_Font


// NEW — palette entries must hold full 32bpp values
// Forward declaration needed before the class
struct PrintRequest {
    int x, y;
    std::string text;
};

class SDLDrawPlugin8bpp : public SDLBasicDrawPlugin<UINT32>
{
private:
    SDL_mutex *cs;
    TTF_Font *_font = nullptr;
    std::vector<PrintRequest> _printQueue;  // ← semicolon added

public:
    SDLDrawPlugin8bpp() {
        _bpp = 8;
        cs = SDL_CreateMutex();
    }

    virtual ~SDLDrawPlugin8bpp() {
        if (_font) TTF_CloseFont(_font);  // ← clean up font on destroy
        SDL_DestroyMutex(cs);
    }

    bool loadFont(const char* path, int size) {
        if (TTF_WasInit() == 0) TTF_Init();
        _font = TTF_OpenFont(path, size);
        return (_font != nullptr);
    }

    TTF_Font* getFont() const { return _font; }

    virtual void render(bool throttle);
    virtual void renderOverlays();          // ← add this
    virtual void setPixel(int x, int y, int color);
    void print(int x, int y, const char* text);

protected:
    virtual void update(IPalette *palette, int data);
    virtual void updateFullPalette(IPalette *palette);
};

class SDLDrawPlugin16bpp : public SDLBasicDrawPlugin<UINT16>
{
	public:
		SDLDrawPlugin16bpp() { _bpp = 16; }
};

class SDLDrawPlugin24bpp : public SDLBasicDrawPlugin<UINT32>
{
	public:
		SDLDrawPlugin24bpp() { _bpp = 24; }
		virtual void setPixel(int x, int y, int color);
};

class SDLDrawPlugin32bpp : public SDLBasicDrawPlugin<UINT32>
{
	public:
		SDLDrawPlugin32bpp() { _bpp = 32; }
};

class SDLDrawPluginPaletaGrises8bpp : public SDLBasicDrawPlugin<UINT8>
{
	public:
		SDLDrawPluginPaletaGrises8bpp() { _bpp = 8; }
		virtual bool init(const VideoInfo *vi, IPalette *pal);
};

typedef SDLDrawPlugin8bpp SDLDrawPluginWindow8bpp;
typedef SDLDrawPlugin16bpp SDLDrawPluginWindow16bpp;
typedef SDLDrawPlugin24bpp SDLDrawPluginWindow24bpp;
typedef SDLDrawPlugin32bpp SDLDrawPluginWindow32bpp;
typedef SDLDrawPluginPaletaGrises8bpp SDLDrawPluginWindowPaletaGrises8bpp;

template <class T>
class SDLDrawPluginFullScreen: public T
{
	public:
		SDLDrawPluginFullScreen() { T::_flags|=SDL_WINDOW_FULLSCREEN_DESKTOP; }
};

typedef SDLDrawPluginFullScreen<SDLDrawPlugin8bpp> SDLDrawPluginFullScreen8bpp;
typedef SDLDrawPluginFullScreen<SDLDrawPlugin16bpp> SDLDrawPluginFullScreen16bpp;
typedef SDLDrawPluginFullScreen<SDLDrawPlugin24bpp> SDLDrawPluginFullScreen24bpp;
typedef SDLDrawPluginFullScreen<SDLDrawPlugin32bpp> SDLDrawPluginFullScreen32bpp;
typedef SDLDrawPluginFullScreen<SDLDrawPluginPaletaGrises8bpp> SDLDrawPluginFullScreenPaletaGrises8bpp;

#endif // _SDL_VIDEO_PLUGINS_H_
