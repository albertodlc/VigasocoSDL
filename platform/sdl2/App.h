// App.h
// SDL2 platform application layer — no runtime plugin system
/////////////////////////////////////////////////////////////////////////////

#ifndef _APP_H_
#define _APP_H_

#include "Vigasoco.h"
#include <string>
#include <vector>

// Forward declarations
class SDLDrawPlugin8bpp;
class SDLAudioPlugin;
class SDLInputKeyboardPlugin;

class VigasocoSDL : public Vigasoco {
public:
  typedef std::vector<std::string> Strings;

public:
  VigasocoSDL(std::string game, Strings paths);
  virtual ~VigasocoSDL();

  virtual ICriticalSection *createCriticalSection();

protected:
  Strings _sPaths;

  // template methods overrides — construction
  virtual bool platformSpecificInit();
  virtual void createPalette();
  virtual void addCustomLoaders(FileLoader *fl);
  virtual void createDrawPlugin();
  virtual void createAudioPlugin();
  virtual void addCustomInputPlugins();
  virtual void createTimer();
  virtual void createAsyncThread();
  virtual void initCompleted();

  // template methods overrides — destruction
  virtual void destroyAsyncThread();
  virtual void destroyTimer();
  virtual void removeCustomInputPlugins();
  virtual void destroyDrawPlugin();
  virtual void destroyAudioPlugin();
  virtual void removeCustomLoaders(FileLoader *fl);
  virtual void destroyPalette();
  virtual void platformSpecificEnd();
  virtual bool processExitEvents();
};

#endif // _APP_H_