#pragma once

#include "ui/MainMenu.h"

namespace Abadia {

class Juego; // forward declaration

enum class GameState {
  PRESENTING,
  MAIN_MENU,
  LANGUAGE_MENU,
  LOAD_MENU,
  SAVE_MENU,
  HELP_MENU,
  KEYBOARD_MENU,
  OPTIONS_MENU,
  CAMERA_MENU,
  TUTORIAL_MENU,
  INTRO,
  INIT_GAME,
  PLAYING,
  GAME_OVER
};

class GameStateMachine {
public:
  GameStateMachine(Juego *juego, MainMenu *mainMenu);

  void tick();
  GameState getState() const { return _state; }

private:
  Juego *_juego;
  MainMenu *_mainMenu;
  GameState _state = GameState::PRESENTING;
  int _stateTimer = 0;

  void transitionTo(GameState newState);

  void tickPresenting();
  void tickMainMenu();
  void tickLanguageMenu();
  void tickLoadMenu();
  void tickSaveMenu();
  void tickHelpMenu();
  void tickKeyboardMenu();
  void tickOptionsMenu();
  void tickCameraMenu();
  void tickTutorialMenu();
  void tickIntro();
  void tickInitGame();
  void tickPlaying();
  void tickGameOver();
};

} // namespace Abadia