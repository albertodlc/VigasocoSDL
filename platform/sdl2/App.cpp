// App.cpp
// SDL2 platform application layer — no runtime plugin system
/////////////////////////////////////////////////////////////////////////////

#include "App.h"
#include "CriticalSection.h"
#include "FileLoader.h"
#include "InputHandler.h"
#include "Palette.h"

#ifdef RDTSC
#include "RDTSCTimer.h"
#else
#include "Timer.h"
#endif

// SDL2 platform implementations
#include "audio/SDLAudioPlugin.h"
#include "input/SDLInputKeyboardPlugin.h"
#include "video/SDLVideoPlugins.h"

#include <SDL2/SDL.h>

/////////////////////////////////////////////////////////////////////////////
// initialization and cleanup
/////////////////////////////////////////////////////////////////////////////

VigasocoSDL::VigasocoSDL(std::string game, Strings paths) {
  _game = game;
  _sPaths = paths;
}

VigasocoSDL::~VigasocoSDL() {}

/////////////////////////////////////////////////////////////////////////////
// platform services
/////////////////////////////////////////////////////////////////////////////

ICriticalSection *VigasocoSDL::createCriticalSection() {
  return new SDLCriticalSection();
}

/////////////////////////////////////////////////////////////////////////////
// construction template methods
/////////////////////////////////////////////////////////////////////////////

bool VigasocoSDL::platformSpecificInit() { return true; }

void VigasocoSDL::createPalette() { _palette = new SDLPalette(); }

void VigasocoSDL::addCustomLoaders(FileLoader *fl) {
  // add optional paths to the file loader
  for (Strings::size_type i = 0; i < _sPaths.size(); i++) {
    fl->addPath(_sPaths[i]);
  }
}

void VigasocoSDL::createDrawPlugin() { _drawPlugin = new SDLDrawPlugin8bpp(); }

void VigasocoSDL::createAudioPlugin() { _audioPlugin = new SDLAudioPlugin(); }

void VigasocoSDL::addCustomInputPlugins() {
  SDLInputKeyboardPlugin *ip = new SDLInputKeyboardPlugin();
  _inputHandler->addInputPlugin(ip);
}

void VigasocoSDL::createTimer() {
#ifdef RDTSC
  _timer = new RDTSCTimer();
#else
  _timer = new SDLTimer();
#endif
}

void VigasocoSDL::initCompleted() {
  // Load font for text rendering
  SDLDrawPlugin8bpp *plugin = dynamic_cast<SDLDrawPlugin8bpp *>(_drawPlugin);
  if (plugin) {
    if (!plugin->loadFont("fonts/arial.ttf", 16)) {
      fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
    }
  }

  // TODO: set window title once we have access to the window handle
  // SDL_SetWindowTitle(window, ("VigasocoSDL: " +
  // _driver->getFullName()).c_str());
  SDL_ShowCursor(SDL_DISABLE);
}

/////////////////////////////////////////////////////////////////////////////
// destruction template methods
/////////////////////////////////////////////////////////////////////////////

void VigasocoSDL::destroyTimer() {
  delete _timer;
  _timer = 0;
}

void VigasocoSDL::removeCustomInputPlugins() {
  // input handler owns and deletes its plugins
}

void VigasocoSDL::destroyDrawPlugin() {
  delete _drawPlugin;
  _drawPlugin = 0;
}

void VigasocoSDL::destroyAudioPlugin() {
  delete _audioPlugin;
  _audioPlugin = 0;
}

void VigasocoSDL::removeCustomLoaders(FileLoader *fl) {
  // nothing to remove — no custom loaders added
}

void VigasocoSDL::destroyPalette() {
  delete _palette;
  _palette = 0;
}

void VigasocoSDL::platformSpecificEnd() {
  // Close the font system
  TTF_Quit();

  // Close SDL
  SDL_Quit();
}

/**
 * @brief Process the 'Main loop' exit events
 *
 * @return true if is an exit event, false otherwise
 */
bool VigasocoSDL::processExitEvents() {
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    bool isExitEvent =
        (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE ||
         event.type == SDL_QUIT);

    return isExitEvent;
  }

  return false;
}