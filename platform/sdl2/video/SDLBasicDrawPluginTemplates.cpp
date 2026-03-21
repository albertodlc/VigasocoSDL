// SDLBasicDrawPluginTemplates.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "SDLBasicDrawPlugin.h"
#include "IPalette.h"

// 6 para cuadricula de 64*64 pixeles (2^6)
// 7 para cuadricula de 128*128 pixeles (2^7)
#define FACTOR_REJILLA 6

template<typename T>
bool SDLBasicDrawPlugin<T>::init(const VideoInfo *vi, IPalette *pal)
{
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr,
				"Couldn't initialize SDL: %s\n", SDL_GetError());
		return false;
	}

	myPixels = new Uint32[vi->width*vi->height];
	if (myPixels==NULL) {
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"Memoria VigasocoSDLDrawPlugin ERROR",
			"No se puede reservar memoria para los gráficos",
			NULL);
		return false;
	}

	SDL_CreateWindowAndRenderer(vi->width,vi->height,_flags,&window,&renderer);
	
	if ( window == NULL || renderer == NULL ) {
		fprintf(stderr, "Couldn't set %dx%dx%d video mode: %s\n",
				vi->width,vi->height,_bpp,SDL_GetError());
		return false;
	}

	Uint32 format = SDL_PIXELFORMAT_ARGB8888;
	// SDL2 does not support palettized textures.
	// Always use ARGB8888 — palette colors are pre-mapped to 32bpp in _palette.
	_pitch = vi->width * sizeof(Uint32);
	
	// TEMP DEBUG — remove after confirming
	fprintf(stderr, "init: width=%d height=%d pitch=%d\n", vi->width, vi->height, _pitch);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	texture=SDL_CreateTexture(renderer,
		format, 
		SDL_TEXTUREACCESS_STREAMING,
		vi->width, vi->height);

	if (texture==NULL) {
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"SDL_CreateTexture VigasocoSDLDrawPlugin ERROR",
			SDL_GetError(),
			NULL);
		return false;
	}

	if (SDL_RenderSetLogicalSize(renderer, vi->width, vi->height)<0) {
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"SDL_RenderSetLogicalSize VigasocoSDLDrawPlugin ERROR",
			SDL_GetError(),
			NULL);
		return false;
	}

//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//SDL_RenderClear(renderer);
//SDL_RenderPresent(renderer);

	//if (SDL_QueryTexture(texture,&format,NULL,NULL,NULL)<0) {
	if (SDL_QueryTexture(texture,&format,NULL,NULL,NULL)<0) {
		//	fprintf(stderr,"format %s ",SDL_GetPixelFormatName(format));
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"SDL_QueryTexture VigasocoSDLDrawPlugin ERROR",
			SDL_GetError(),
			NULL);
		return false;
	}
	pixelFormat=SDL_AllocFormat(format);
	if (pixelFormat==NULL) {
		//	fprintf(stderr,"BytesPerPixel %d ",pf->BytesPerPixel);
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"SDL_AllocFormat VigasocoSDLDrawPlugin ERROR",
			SDL_GetError(),
			NULL);
		return false;
	}

	_originalPalette=pal;

	_palette = new T[pal->getTotalColors()];
	pal->attach(this);
	updateFullPalette(pal);

#ifndef _EE
	// TODO SDL2
	// limpiar, no creo que haya SDL2 homebrew para Playstation2
	xrects=(vi->width>>FACTOR_REJILLA);
	yrects=(vi->height>>FACTOR_REJILLA);
	int nrects=xrects*yrects;

	updated_rect = new bool*[xrects];
	for (int i=0;i<xrects;i++)
	{
		updated_rect[i]=new bool[yrects];
	}  
	for(int i=0;i<xrects;i++)
	{
		for(int j=0;j<yrects;j++)
		{
			updated_rect[i][j]=false;
		}
	}

	SDLRects = new SDL_Rect[nrects];
#endif

	_isInitialized = true;
	
	return _isInitialized;
};


template<typename T>
void SDLBasicDrawPlugin<T>::end()
{
    if (_originalPalette)
        _originalPalette->detach(this);

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    if (myPixels != NULL) delete[] myPixels;

    _isInitialized = false;
}


// Palette changes
template<typename T>
void SDLBasicDrawPlugin<T>::updateFullPalette(IPalette *palette)
{
	for (int i = 0; i < palette->getTotalColors(); i++){
		UINT8 r, g, b;

		palette->getColor(i, r, g, b);
		_palette[i] = SDL_MapRGB(pixelFormat,r,g,b);
	}
}

template<typename T>
void SDLBasicDrawPlugin<T>::update(IPalette *palette, int data)
{
	if (data != -1){
		// single color update
		UINT8 r, g, b;

		palette->getColor(data, r, g, b);
		_palette[data] = SDL_MapRGB(pixelFormat,r,g,b);
	} else {
		// full palette update
		updateFullPalette(palette);	
	}
}

template<typename T>
inline void SDLBasicDrawPlugin<T>::updateRect(int x,int y)
{
//	fprintf(stderr,"%d %d -> rect %d,%d\n",x,y,x>>FACTOR_REJILLA,y>>FACTOR_REJILLA);

#ifndef _EE
	updated_rect[x>>FACTOR_REJILLA][y>>FACTOR_REJILLA]=true;
#endif
}

// drawing methods
template<typename T>
void SDLBasicDrawPlugin<T>::render(bool throttle)
{
	#ifdef _EE
		SDL_UpdateRects(screen,0,NULL);
	#else

	/* 
	TODO SDL2
	probar a optimizar con rectangulos como en el plugin SDL1

		int n=0;
		for(int i=0;i<xrects;i++)
		{
			for(int j=0;j<yrects;j++)
			{
				if (updated_rect[i][j])
				{
					SDLRects[n].x=i<<FACTOR_REJILLA;
					SDLRects[n].y=j<<FACTOR_REJILLA;
					SDLRects[n].w=1<<FACTOR_REJILLA;
					SDLRects[n++].h=1<<FACTOR_REJILLA;
				}
			}
		}
		if(n)
		{
			SDL_UpdateRects(screen,n,SDLRects);
					for(int i=0;i<xrects;i++)
					{
							for(int j=0;j<yrects;j++)
							{
									updated_rect[i][j]=false;
							}
					}
		}
	*/

	SDL_UpdateTexture(texture, NULL, myPixels, _pitch);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer,texture,NULL,NULL);
	// NOTE: text/overlays should be drawn here, after RenderCopy
    // but before RenderPresent — subclasses can override renderOverlays()
    renderOverlays();
	SDL_RenderPresent(renderer);
#endif
};

template<typename T>
void SDLBasicDrawPlugin<T>::setPixel(int x, int y, int color)
{
    // TEMP DEBUG
    static int maxY = 0;
    static int maxX = 0;
    if (y > maxY) { maxY = y; fprintf(stderr, "maxY=%d\n", maxY); }
    if (x > maxX) { maxX = x; fprintf(stderr, "maxX=%d\n", maxX); }

    Uint32 *p = (Uint32 *)((Uint8 *)myPixels + y * _pitch + x * sizeof(Uint32));
    *p = _palette[color];
}