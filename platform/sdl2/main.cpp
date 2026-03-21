// main.cpp
// SDL2 entry point

#include "App.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <vector>

typedef std::vector<std::string> Strings;

void showError(std::string error)
{
    if (error.empty()){
        std::cerr << "Unexpected error loading Vigasoco." << std::endl;
    } else {
        std::cerr << error << std::endl;
    }
}

int main(int argc, char **argv)
{
    // default game
    std::string game = "abadia";
    Strings paths;

    // optional: first argument overrides the game name
    if (argc > 1){
        game = argv[1];
    }

    // optional: -path:/some/path argument
    for (int i = 2; i < argc; i++){
        std::string arg = argv[i];
        if (arg.substr(0, 6) == "-path:"){
            paths.push_back(arg.substr(6));
        }
    }

    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) < 0){
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    VigasocoSDL app(game, paths);

    if (!app.init(game)){
        showError(app.getError());
        app.end();
        SDL_Quit();
		
        return -1;
    }

    app.mainLoop();
    app.end();
    SDL_Quit();

    return 0;
}