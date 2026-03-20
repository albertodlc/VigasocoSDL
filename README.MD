## What VigasocoSDL is

This is a **full reverse-engineered port** of La Abadía del Crimen, already done in C++. The name "VIGASOCO" stands for **VI**deo **GA**mes **SO**urce **CO**de.

---

## Compilation
Install this dependencies
```bash
sudo apt install libsdl2-dev libsdl1.2-dev g++ make
```

Execute fdrom the root
```bash
make clean
make
```

## Key discoveries
**1. The game reads from a `.DSK` file** — not loose files like `abadia1.ovl`. The `DskReader.cpp` reads Amstrad CPC disk images. Your game files are the PC version — you'll need to check compatibility.

**2. SDL1 → SDL2 migration is the main upgrade needed** — `sdl-config` needs to become `sdl2-config` and several SDL1 APIs changed in SDL2.

**3. The plugin system is over-engineered for Linux** — on Linux you can simplify by compiling everything into one binary instead of using `.so` plugins.

## What you should do with your fork
There are three levels of improvement possible:

```
Level 1 — Just get it building on modern Linux
  • SDL1 → SDL2 migration
  • Fix deprecated APIs
  • Replace Makefile with CMake

Level 2 — Modernize the codebase
  • C++17 features
  • Replace raw pointers with smart pointers
  • Add proper error handling

Level 3 — Feature improvements
  • Higher resolution support
  • Better audio
  • Save states
  • Controller support
```

---

## Architecture — Plugin-based system
The project uses a **plugin architecture** — the core game is separated from platform-specific code via interfaces:

```
VigasocoSDL/
│
├── core/                    ← Platform-independent game logic
│   ├── abadia/              ← The actual game code
│   │   ├── Juego.cpp        ← Main game (= game_main we analyzed)
│   │   ├── Guillermo.cpp    ← Guillermo de Occam character
│   │   ├── Adso.cpp         ← Adso character (player)
│   │   ├── Monje.cpp        ← Generic monk base class
│   │   ├── Jorge.cpp        ← Jorge character
│   │   ├── Malaquias.cpp    ← Malaquias character
│   │   ├── Berengario.cpp   ← Berengario character
│   │   ├── Severino.cpp     ← Severino character
│   │   ├── Bernardo.cpp     ← Bernardo Gui character
│   │   ├── Logica.cpp       ← Game logic / rules
│   │   ├── MotorGrafico.cpp ← Graphics engine
│   │   ├── GeneradorPantallas.cpp ← Screen/room generator
│   │   ├── Sprite.cpp       ← Sprite system
│   │   ├── Paleta.cpp       ← Palette management
│   │   ├── Puerta.cpp       ← Door system
│   │   ├── Objeto.cpp       ← Game objects/items
│   │   ├── Pergamino.cpp    ← Scroll/text system
│   │   ├── Comandos.cpp     ← Command system
│   │   └── BuscadorRutas.cpp ← Pathfinding (NPC AI!)
│   │
│   ├── Vigasoco.cpp         ← Framework core
│   ├── GameDriver.cpp       ← Game driver base
│   ├── AbadiaDriver.cpp     ← Abadia-specific driver
│   ├── TimingHandler.cpp    ← Frame timing (= our frame limiter)
│   ├── InputHandler.cpp     ← Input management
│   └── systems/
│       ├── cpc6128.cpp      ← Amstrad CPC emulation layer
│       └── DskReader.cpp    ← .DSK floppy image reader
│
├── VigasocoSDL/             ← SDL2 platform layer
│   ├── SDLMain.cpp          ← Entry point
│   ├── VigasocoSDL.cpp      ← SDL application wrapper
│   ├── SDLTimer.cpp         ← SDL timer (= our frame limiter)
│   ├── SDLPalette.cpp       ← SDL palette
│   ├── SDLThread.cpp        ← SDL threading
│   └── PluginHandler.cpp    ← Loads plugin .so files at runtime
│
├── SDLVideoPlugins/         ← Video plugin (.so shared library)
│   ├── SDLDrawPlugin.cpp    ← SDL drawing
│   ├── SDLDrawPlugin8bpp.cpp  ← 8-bit color mode
│   └── SDLDrawPlugin24bpp.cpp ← 24-bit color mode
│
├── SDLInputKeyboardPlugin/  ← Keyboard plugin (.so)
├── SDLAudioPlugin/          ← Audio plugin (.so)
├── NULLAudioPlugin/         ← Null audio plugin (.so)
└── DirectDrawPlugins/       ← Windows-only DirectDraw plugins
```

---

## Build system — GNU Make + SDL1

```makefile
CXXFLAGS = -g -I../core `sdl-config --cflags`
$(CXX) $(OBJECTS) `sdl-config --libs` -o VigasocoSDL
```

**Important:** It uses **`sdl-config`** not `sdl2-config` — this is **SDL1**, not SDL2. This is the first thing to update.

The build system uses **GNU Make**, not CMake. Each component has its own `Makefile`. The top-level `Makefile` builds everything in order:
1. `SDLInputKeyboardPlugin`
2. `SDLVideoPlugins`
3. `SDLAudioPlugin`
4. `NULLAudioPlugin`
5. `VigasocoSDL` (main executable)

The video/input/audio plugins are built as **shared libraries (`.so`)** and loaded at runtime — that's why the main executable calls `PluginHandler`.
