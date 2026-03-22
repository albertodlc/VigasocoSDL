#include "GameStateMachine.h"
#include "Juego.h"
#include "Paleta.h"

using namespace Abadia;

GameStateMachine::GameStateMachine(Juego *juego, MainMenu *mainMenu)
    : _juego(juego), _mainMenu(mainMenu) {}

void GameStateMachine::transitionTo(GameState newState) {
  _state = newState;
  _stateTimer = 0;
}

void GameStateMachine::tick() {
  switch (_state) {
  case GameState::PRESENTING:
    tickPresenting();
    break;
  case GameState::MAIN_MENU:
    tickMainMenu();
    break;
  case GameState::LANGUAGE_MENU:
    tickLanguageMenu();
    break;
  case GameState::LOAD_MENU:
    tickLoadMenu();
    break;
  case GameState::SAVE_MENU:
    tickSaveMenu();
    break;
  case GameState::HELP_MENU:
    tickHelpMenu();
    break;
  case GameState::KEYBOARD_MENU:
    tickKeyboardMenu();
    break;
  case GameState::OPTIONS_MENU:
    tickOptionsMenu();
    break;
  case GameState::CAMERA_MENU:
    tickCameraMenu();
    break;
  case GameState::TUTORIAL_MENU:
    tickTutorialMenu();
    break;
  case GameState::INTRO:
    tickIntro();
    break;
  case GameState::INIT_GAME:
    tickInitGame();
    break;
  case GameState::PLAYING:
    tickPlaying();
    break;
  case GameState::GAME_OVER:
    tickGameOver();
    break;
  }
}

void GameStateMachine::tickPresenting() {
  if (_stateTimer == 0) {
    Paleta *paleta = _juego->paleta;
    paleta->setIntroPalette();

    UINT8 *romsVGA = &_juego->roms[0x24000 - 1 - 0x4000];
    _juego->cpc6128->showVGAScreen(romsVGA + 0x1ADF0);
    _juego->cpc6128->markAllPixelsDirty();
  }
  _stateTimer++;
  if (_stateTimer > 180) {
    _juego->marcador->limpiaAreaMarcador();
    transitionTo(GameState::MAIN_MENU);
  }
}

void GameStateMachine::tickMainMenu() {
  if (_mainMenu->tickMainMenu()) {
    int pulsado = _mainMenu->getLastPulsado();
    _mainMenu->resetState();

    switch (pulsado) {
    case 0:
      transitionTo(GameState::LANGUAGE_MENU);
      break;
    case 1:
      transitionTo(GameState::LOAD_MENU);
      break;
    case 2:
      transitionTo(GameState::SAVE_MENU);
      break;
    case 4:
      transitionTo(GameState::HELP_MENU);
      break;
    case 5:
      transitionTo(GameState::TUTORIAL_MENU);
      break;
    case 6:
      transitionTo(GameState::INIT_GAME);
      break;
    case 8:
      transitionTo(GameState::INTRO);
      break;
    default:
      break;
    }
  }
}

void GameStateMachine::tickLanguageMenu() {
  if (_mainMenu->tickLanguageMenu()) {
    _mainMenu->resetState();
    transitionTo(GameState::MAIN_MENU);
  }
}

void GameStateMachine::tickLoadMenu() {
  if (_mainMenu->tickLoadMenu()) {
    _mainMenu->resetState();
    transitionTo(GameState::INIT_GAME);
  }
}

void GameStateMachine::tickSaveMenu() {
  if (_mainMenu->tickSaveMenu()) {
    _mainMenu->resetState();
    transitionTo(GameState::MAIN_MENU);
  }
}

void GameStateMachine::tickHelpMenu() {
  if (_mainMenu->tickHelpMenu()) {
    _mainMenu->resetState();
    transitionTo(GameState::MAIN_MENU);
  }
}

void GameStateMachine::tickKeyboardMenu() {
  if (_mainMenu->tickKeyboardMenu()) {
    _mainMenu->resetState();
    transitionTo(GameState::HELP_MENU);
  }
}

void GameStateMachine::tickOptionsMenu() {
  if (_mainMenu->tickOptionsMenu()) {
    _mainMenu->resetState();
    transitionTo(GameState::HELP_MENU);
  }
}

void GameStateMachine::tickCameraMenu() {
  if (_mainMenu->tickCameraMenu()) {
    _mainMenu->resetState();
    transitionTo(GameState::HELP_MENU);
  }
}

void GameStateMachine::tickTutorialMenu() {
  if (_mainMenu->tickTutorialMenu()) {
    _mainMenu->resetState();
    transitionTo(GameState::MAIN_MENU);
  }
}

void GameStateMachine::tickIntro() {
  if (_mainMenu->tickIntro()) {
    _mainMenu->resetState();
    _juego->marcador->limpiaAreaMarcador();
    transitionTo(GameState::INIT_GAME);
  }
}

void GameStateMachine::tickInitGame() {
  _juego->logica->inicia();
  _juego->ReiniciaPantalla();
  transitionTo(GameState::PLAYING);
}

void GameStateMachine::tickPlaying() {
  _juego->controles->actualizaEstado();

  // TODO: Por pasarselo
  // _juego->elBuscadorDeRutas->contadorAnimGuillermo =
  //     _juego->logica->guillermo->contadorAnimacion;

  _juego->logica->compruebaAbreEspejo();
  _juego->compruebaPausa();
  _juego->compruebaSave();

  if (_juego->compruebaLoad()) {
    _juego->ReiniciaPantalla();
    return;
  }

  _juego->compruebaCambioCPC_VGA();

  if (_juego->compruebaMenu()) {
    _juego->ReiniciaPantalla();
    return;
  }

  _juego->logica->actualizaVariablesDeTiempo();

  if (_juego->muestraPantallaFinInvestigacion()) {
    transitionTo(GameState::GAME_OVER);
    return;
  }

  _juego->logica->compruebaLecturaLibro();
  _juego->marcador->realizaScrollMomentoDia();
  _juego->logica->ejecutaAccionesMomentoDia();
  _juego->logica->compruebaBonusYCambiosDeCamara();
  _juego->motor->compruebaCambioPantalla();
  _juego->logica->compruebaCogerDejarObjetos();
  _juego->logica->compruebaAbrirCerrarPuertas();

  // for (int i = 0; i < Juego::numPersonajes; i++) {
  //   _juego->personajes[i]->run();
  // }

  // _juego->logica->buscRutas->generadoCamino = false;
  // _juego->actualizaLuz();
  // _juego->logica->realizaReflejoEspejo();

  if (_juego->cambioModoInformacion && _juego->modoInformacion) {
    _juego->limpiaAreaJuego(12);
    _juego->cambioModoInformacion = false;
  }
  if (_juego->cambioModoInformacion && !_juego->modoInformacion) {
    _juego->limpiaAreaJuego(12);
    _juego->motor->compruebaCambioPantalla(true);
    _juego->cambioModoInformacion = false;
  }

  // if (_juego->modoInformacion) {
  //   _juego->infoJuego->muestraInfo();
  // } else {
  //   _juego->motor->dibujaPantalla();
  //   _juego->motor->dibujaSprites();
  // }

  // if (_juego->logica->guillermo->contadorAnimacion == 1) {
  //   _juego->audio_plugin->Play(SONIDOS::Pasos);
  // }

  _juego->contadorInterrupcion = 0;
}

void GameStateMachine::tickGameOver() {
  _juego->muestraPantallaFinInvestigacion();
  _juego->logica->inicia();
  _juego->ReiniciaPantalla();
  transitionTo(GameState::PLAYING);
}