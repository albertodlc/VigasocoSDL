#include "MainMenu.h"

MainMenu::MainMenu(int language, Abadia::Pergamino *pergamino,
                   Abadia::MotorGrafico *gameEngine, Abadia::Logica *gameLogic,
                   Abadia::Controles *inputController,
                   Abadia::Marcador *marcador, CPC6128 *cpc6128,
                   TimingHandler *timer)
    : _language(language), _pergamino(pergamino), _gameEngine(gameEngine),
      _gameLogic(gameLogic), _inputController(inputController),
      _marcador(marcador), _cpc6128(cpc6128), _timer(timer) {}

void MainMenu::resetState() {
  _initialized = false;
  _lastResult = false;
  _selected = 0;
  _pulsado = -1;
  _salir = false;
}

/**
 * @brief Initial menu logic
 *
 * @return true
 * @return false
 */
bool MainMenu::tickMainMenu() {
  // first tick — render menu
  if (!_initialized) {
    renderMenu(_selected, true);
    _initialized = true;
    return false;
  }

  _pulsado = -1;
  _inputController->actualizaEstado();

  if (_inputController->seHaPulsado(P1_DOWN)) {
    _selected++;
    if (_selected == 9)
      _selected = 0;
    renderMenu(_selected);
  }

  if (_inputController->seHaPulsado(P1_UP)) {
    _selected--;
    if (_selected == -1)
      _selected = 8;
    renderMenu(_selected);
  }

  if (_inputController->seHaPulsado(P1_BUTTON1) ||
      _inputController->seHaPulsado(KEYBOARD_INTRO)) {
    _pulsado = _selected;
  }

  if (_inputController->seHaPulsado(KEYBOARD_0) || _pulsado == 0) {
    // → transition to LANGUAGE_MENU
    _lastResult = false;
    _salir = true;
    _pulsado = 0;
  } else if (_inputController->seHaPulsado(KEYBOARD_1) || _pulsado == 1) {
    // → transition to LOAD_MENU
    _lastResult = false;
    _salir = true;
    _pulsado = 1;
  } else if (_inputController->seHaPulsado(KEYBOARD_2) || _pulsado == 2) {
    // → transition to SAVE_MENU
    _lastResult = false;
    _salir = true;
    _pulsado = 2;
  } else if (_inputController->seHaPulsado(KEYBOARD_4) || _pulsado == 4) {
    // → transition to HELP_MENU
    _lastResult = false;
    _salir = true;
    _pulsado = 4;
  } else if (_inputController->seHaPulsado(KEYBOARD_5) || _pulsado == 5) {
    // → transition to TUTORIAL_MENU
    _lastResult = false;
    _salir = true;
    _pulsado = 5;
  } else if (_inputController->seHaPulsado(KEYBOARD_6) || _pulsado == 6) {
    // → PLAY (restart)
    _lastResult = true;
    _salir = true;
    _pulsado = 6;
  } else if (_inputController->seHaPulsado(KEYBOARD_8) || _pulsado == 8) {
    // → PLAY
    _lastResult = false;
    _salir = true;
    _pulsado = 8;
  }

  return _salir;
}

bool MainMenu::tickLanguageMenu() {
  if (!_initialized) {
    renderLanguageMenu(_selected, true);
    _initialized = true;
    return false;
  }

  _inputController->actualizaEstado();

  if (_inputController->seHaPulsado(P1_DOWN)) {
    _selected++;
    if (_selected == 8)
      _selected = 0;
    renderLanguageMenu(_selected);
  }
  if (_inputController->seHaPulsado(P1_UP)) {
    _selected--;
    if (_selected == -1)
      _selected = 7;
    renderLanguageMenu(_selected);
  }

  // With explicit checks — safer and avoids the cast
  if (_inputController->seHaPulsado(KEYBOARD_0) || _pulsado == 0) {
    _language = 0;
    _salir = true;
  } else if (_inputController->seHaPulsado(KEYBOARD_1) || _pulsado == 1) {
    _language = 1;
    _salir = true;
  } else if (_inputController->seHaPulsado(KEYBOARD_2) || _pulsado == 2) {
    _language = 2;
    _salir = true;
  } else if (_inputController->seHaPulsado(KEYBOARD_3) || _pulsado == 3) {
    _language = 3;
    _salir = true;
  } else if (_inputController->seHaPulsado(KEYBOARD_4) || _pulsado == 4) {
    _language = 4;
    _salir = true;
  } else if (_inputController->seHaPulsado(KEYBOARD_5) || _pulsado == 5) {
    _language = 5;
    _salir = true;
  } else if (_inputController->seHaPulsado(KEYBOARD_6) || _pulsado == 6) {
    _language = 6;
    _salir = true;
  } else if (_inputController->seHaPulsado(KEYBOARD_7) || _pulsado == 7) {
    _language = 7;
    _salir = true;
  }

  return _salir;
}

bool MainMenu::tickLoadMenu() {
  if (!_initialized) {
    clearMenuArea(0);
    renderLoadMenu(_selected, true);
    _initialized = true;
    return false;
  }

  _inputController->actualizaEstado();

  if (_inputController->seHaPulsado(P1_DOWN)) {
    _selected++;
    if (_selected == 8)
      _selected = 0;
    renderLoadMenu(_selected);
  }
  if (_inputController->seHaPulsado(P1_UP)) {
    _selected--;
    if (_selected == -1)
      _selected = 7;
    renderLoadMenu(_selected);
  }

  if (_inputController->seHaPulsado(P1_BUTTON1) ||
      _inputController->seHaPulsado(KEYBOARD_INTRO)) {
    _pulsado = _selected;
    _salir = true;
  }
  if (_inputController->seHaPulsado(KEYBOARD_7) || _pulsado == 7) {
    _salir = true;
  }

  return _salir;
}

bool MainMenu::tickSaveMenu() {
  if (!_initialized) {
    clearMenuArea(0);
    renderRecordingMenu(_selected, true);
    _initialized = true;
    return false;
  }

  _inputController->actualizaEstado();

  if (_inputController->seHaPulsado(P1_DOWN)) {
    _selected++;
    if (_selected == 8)
      _selected = 0;
    renderRecordingMenu(_selected);
  }
  if (_inputController->seHaPulsado(P1_UP)) {
    _selected--;
    if (_selected == -1)
      _selected = 7;
    renderRecordingMenu(_selected);
  }

  if (_inputController->seHaPulsado(P1_BUTTON1) ||
      _inputController->seHaPulsado(KEYBOARD_INTRO)) {
    _pulsado = _selected;
    _salir = true;
  }
  if (_inputController->seHaPulsado(KEYBOARD_7) || _pulsado == 7) {
    _salir = true;
  }

  return _salir;
}

bool MainMenu::tickHelpMenu() {
  if (!_initialized) {
    clearMenuArea(0);
    renderHelpMenu(_selected, true);
    _initialized = true;
    return false;
  }

  _inputController->actualizaEstado();

  if (_inputController->seHaPulsado(P1_DOWN)) {
    _selected++;
    if (_selected == 6)
      _selected = 0;
    renderHelpMenu(_selected);
  }
  if (_inputController->seHaPulsado(P1_UP)) {
    _selected--;
    if (_selected == -1)
      _selected = 5;
    renderHelpMenu(_selected);
  }

  if (_inputController->seHaPulsado(P1_BUTTON1) ||
      _inputController->seHaPulsado(KEYBOARD_INTRO)) {
    _pulsado = _selected;
  }

  if (_inputController->seHaPulsado(KEYBOARD_5) || _pulsado == 5) {
    _salir = true;
  }

  return _salir;
}

bool MainMenu::tickIntro() {
  if (!_initialized) {
    clearMenuArea(0);
    _pergamino->muestraTexto(
        Abadia::Pergamino::pergaminoIntroduccion[_language]);
    _initialized = true;
    _salir = false;
    return false;
  }

  _inputController->actualizaEstado();

  // wait for player to PRESS a button to exit
  if (_inputController->seHaPulsado(P1_BUTTON1) ||
      _inputController->seHaPulsado(KEYBOARD_INTRO) ||
      _inputController->seHaPulsado(KEYBOARD_SPACE)) {
    _salir = true;
  }

  return _salir;
}

bool MainMenu::tickKeyboardMenu() {
  if (!_initialized) {
    clearMenuArea(0);
    _pergamino->muestraTexto(Abadia::Pergamino::pergaminoManejo[_language]);
    _initialized = true;
    return false;
  }

  _inputController->actualizaEstado();

  if (_inputController->seHaPulsado(P1_BUTTON1) ||
      _inputController->seHaPulsado(KEYBOARD_INTRO) ||
      _inputController->seHaPulsado(KEYBOARD_SPACE)) {
    _salir = true;
  }

  return _salir;
}

bool MainMenu::tickOptionsMenu() {
  if (!_initialized) {
    clearMenuArea(0);
    renderOptionsMenu(0);
    _initialized = true;
    return false;
  }

  _inputController->actualizaEstado();

  if (_inputController->seHaPulsado(P1_BUTTON1) ||
      _inputController->seHaPulsado(KEYBOARD_INTRO) ||
      _inputController->seHaPulsado(KEYBOARD_SPACE)) {
    _salir = true;
  }

  return _salir;
}

bool MainMenu::tickCameraMenu() {
  if (!_initialized) {
    clearMenuArea(0);
    renderCameraMenu(0);
    _initialized = true;
    return false;
  }

  _inputController->actualizaEstado();

  if (_inputController->seHaPulsado(P1_BUTTON1) ||
      _inputController->seHaPulsado(KEYBOARD_INTRO) ||
      _inputController->seHaPulsado(KEYBOARD_SPACE)) {
    _salir = true;
  }

  return _salir;
}

bool MainMenu::tickTutorialMenu() {
  if (!_initialized) {
    clearMenuArea(0);
    renderTutorialMenu(0, true);
    _initialized = true;
    return false;
  }

  _inputController->actualizaEstado();

  if (_inputController->seHaPulsado(P1_BUTTON1) ||
      _inputController->seHaPulsado(KEYBOARD_INTRO) ||
      _inputController->seHaPulsado(KEYBOARD_SPACE)) {
    _salir = true;
  }

  return _salir;
}

bool MainMenu::process(int seleccionado) {
  int pulsado = -1;
  bool salir = false;

  // Muestra 1 vez el menu principal
  renderMenu(seleccionado, true);

  // Gestionamos las actualizaciones posteriores
  while (!salir) {
    pulsado = -1;
    _inputController->actualizaEstado();

    // ! DOWN ARROW
    if (_inputController->estaSiendoPulsado(P1_DOWN)) {
      seleccionado++;
      if (seleccionado == 9)
        seleccionado = 0;

      renderMenu(seleccionado);
    }

    // ! UP ARROW
    if (_inputController->estaSiendoPulsado(P1_UP)) {
      seleccionado--;
      if (seleccionado == -1)
        seleccionado = 8;
      renderMenu(seleccionado);
    }

    // ! ENTER
    if (_inputController->estaSiendoPulsado(P1_BUTTON1) ||
        _inputController->estaSiendoPulsado(KEYBOARD_INTRO)) {
      pulsado = seleccionado;
    }

    if (_inputController->estaSiendoPulsado(KEYBOARD_0) || pulsado == 0) {
      processLanguageMenu(_language);
      renderMenu(seleccionado, true);
    } else if (_inputController->estaSiendoPulsado(KEYBOARD_1) ||
               pulsado == 1) {
      // return menuCargar();
    } else if (_inputController->estaSiendoPulsado(KEYBOARD_2) ||
               pulsado == 2) {
      // menuGrabar();
      renderMenu(seleccionado, true);
    } else if (_inputController->estaSiendoPulsado(KEYBOARD_3) ||
               pulsado == 3) {
      // cambioCPC_VGA();
      renderMenu(seleccionado, true);
    } else if (_inputController->estaSiendoPulsado(KEYBOARD_4) ||
               pulsado == 4) {
      // menuAyuda();
      renderMenu(seleccionado, true);
    } else if (_inputController->estaSiendoPulsado(KEYBOARD_5) ||
               pulsado == 5) {
      // menuTutorial();
      renderMenu(seleccionado, true);
    } else if (_inputController->estaSiendoPulsado(KEYBOARD_6) ||
               pulsado == 6) {
      // TODO habria que pedir confirmacion S/N

      // Frase vacia para parar la frase actual
      // elGestorFrases->muestraFraseYa(0x38);
      // Esperamos a que se limpie el marcador
      // while (elGestorFrases->mostrandoFrase)
      // {
      //     elGestorFrases->actualizaEstado();
      // }
      // logica->inicia();
      return true;
    } else if (_inputController->estaSiendoPulsado(KEYBOARD_7) ||
               pulsado == 7) {
      // mute=!mute;
      // audio_plugin->setProperty("mute",mute);
    }

    if (_inputController->estaSiendoPulsado(KEYBOARD_8) || pulsado == 8) {
      salir = true;
    }
  }

  return false;
}

/**
 * @brief Render the language selection menu
 *
 * @param seleccionado
 * @return true
 * @return false
 */
bool MainMenu::processLanguageMenu(int seleccionado) {
  bool salir = false;
  int pulsado = -1;

  // Muestra 1 vez el menu principal
  renderLanguageMenu(seleccionado, true);

  while (!salir) {
    pulsado = -1;
    _inputController->actualizaEstado();

    if (_inputController->estaSiendoPulsado(P1_DOWN)) {
      seleccionado++;
      if (seleccionado == 8)
        seleccionado = 0;
      renderLanguageMenu(seleccionado);
    }
    if (_inputController->estaSiendoPulsado(P1_UP)) {
      seleccionado--;
      if (seleccionado == -1)
        seleccionado = 7;
      renderLanguageMenu(seleccionado);
    }
    if (_inputController->estaSiendoPulsado(P1_BUTTON1) ||
        _inputController->estaSiendoPulsado(KEYBOARD_INTRO)) {
      pulsado = seleccionado;
    }

    if (_inputController->estaSiendoPulsado(KEYBOARD_0) || pulsado == 0) {
      _language = 0;
      salir = true;
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_1) || pulsado == 1) {
      _language = 1;
      salir = true;
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_2) || pulsado == 2) {
      _language = 2;
      salir = true;
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_3) || pulsado == 3) {
      _language = 3;
      salir = true;
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_4) || pulsado == 4) {
      _language = 4;
      salir = true;
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_5) || pulsado == 5) {
      _language = 5;
      salir = true;
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_6) || pulsado == 6) {
      _language = 6;
      salir = true;
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_7) || pulsado == 7) {
      _language = 7;
      salir = true;
    }
  }

  return false;
}

/**
 * @brief Render the main menu
 *
 * @param selected
 * @param enableEffect
 */
void MainMenu::renderMenu(int selected, bool enableEffect) {
  static const char *textos[8][9] = {
      {// 0 Castellano
       "0 IDIOMA", "1 CARGAR PARTIDA", "2 GRABAR PARTIDA", "3 GRÁFICOS VGA-CPC",
       "4 AYUDA", "5 TUTORIAL", "6 REINICIAR", "7 SONIDO", "8 JUGAR"},
      {// 1 INGLES
       "0 IDIOMA", "1 LOAD GAME", "2 SAVE GAME", "3 GRAPHICS VGA-CPC", "4 HELP",
       "5 TUTORIAL", "6 RESTART", "7 SOUND", "8 PLAY"},
      {// 2 PORTUGUES BRASIL
       "0 IDIOMA", "1 CARGAR PARTIDA", "2 GRABAR PARTIDA", "3 GRÁFICOS VGA-CPC",
       "4 AYUDA", "5 TUTORIAL", "6 REINICIAR", "7 SONIDO", "8 JUGAR"},
      {// 3 CATALAN
       "0 IDIOMA", "1 CARGAR PARTIDA", "2 GRABAR PARTIDA", "3 GRÁFICOS VGA-CPC",
       "4 AYUDA", "5 TUTORIAL", "6 REINICIAR", "7 SONIDO", "8 JUGAR"},
      {// 4 GALLEGO
       "0 IDIOMA", "1 CARGAR PARTIDA", "2 GRABAR PARTIDA", "3 GRÁFICOS VGA-CPC",
       "4 AYUDA", "5 TUTORIAL", "6 REINICIAR", "7 SONIDO", "8 JUGAR"},
      {// 5 ITALIANO
       "0 IDIOMA", "1 CARGAR PARTIDA", "2 GRABAR PARTIDA", "3 GRÁFICOS VGA-CPC",
       "4 AYUDA", "5 TUTORIAL", "6 REINICIAR", "7 SONIDO", "8 JUGAR"},
      {// 6 FINES
       "0 IDIOMA", "1 CARGAR PARTIDA", "2 GRABAR PARTIDA", "3 GRÁFICOS VGA-CPC",
       "4 AYUDA", "5 TUTORIAL", "6 REINICIAR", "7 SONIDO", "8 JUGAR"},
      {// 7 PORTUGUES
       "0 IDIOMA", "1 CARGAR PARTIDA", "2 GRABAR PARTIDA", "3 GRÁFICOS VGA-CPC",
       "4 AYUDA", "5 TUTORIAL", "6 REINICIAR", "7 SONIDO", "8 JUGAR"}};

  // Limpia el área que ocupa el marcador
  clearMenuArea(0);

  // Repinta todo el menu
  for (int x = enableEffect ? 8 : 88; x < 88; x += 10) {
    _cpc6128->fillMode1Rect(8, 0, x - 1, 160, 0);
    for (int i = 0; i < 9; i++) {
      _marcador->imprimeFrase(textos[_language][i], x, 16 + (i * 16), 4, 0);
    }
  }

  _cpc6128->fillMode1Rect(8, 0, 88, 160, 0);
  for (int i = 0; i < 9; i++) {
    _marcador->imprimeFrase(textos[_language][i], 88, 16 + (i * 16), 4, 0);
  }

  // pinta la opción seleccionado con el color de fondo y el color
  // de letra cambiado
  _marcador->imprimeFrase(textos[_language][selected], 88, 16 + (selected * 16),
                          0, 4);
}

void MainMenu::renderLanguageMenu(int selected, bool enableEffect) {
  static const char *textos[8][8] = {
      {// 0 Castellano
       "0 CASTELLANO", "1 ENGLISH", "2 PORTUGUÉS BRASIL", "3 CATALÁN",
       "4 GALLEGO", "5 ITALIANO", "6 FINES", "7 PORTUGUÉS"},
      {// 1 INGLES
       "0 CASTELLANO", "1 ENGLISH", "2 PORTUGUÉS BRASIL", "3 CATALÁN",
       "4 GALLEGO", "5 ITALIANO", "6 FINES", "7 PORTUGUÉS"},
      {// 2 PORTUGUES BRASIL
       "0 CASTELLANO", "1 ENGLISH", "2 PORTUGUÉS BRASIL", "3 CATALÁN",
       "4 GALLEGO", "5 ITALIANO", "6 FINES", "7 PORTUGUÉS"},
      {// 3 CATALAN
       "0 CASTELLANO", "1 ENGLISH", "2 PORTUGUÉS BRASIL", "3 CATALÁN",
       "4 GALLEGO", "5 ITALIANO", "6 FINES", "7 PORTUGUÉS"},
      {// 4 GALLEGO
       "0 CASTELLANO", "1 ENGLISH", "2 PORTUGUÉS BRASIL", "3 CATALÁN",
       "4 GALLEGO", "5 ITALIANO", "6 FINES", "7 PORTUGUÉS"},
      {// 5 ITALIANO
       "0 CASTELLANO", "1 ENGLISH", "2 PORTUGUÉS BRASIL", "3 CATALÁN",
       "4 GALLEGO", "5 ITALIANO", "6 FINES", "7 PORTUGUÉS"},
      {// 6 FINES
       "0 CASTELLANO", "1 ENGLISH", "2 PORTUGUÉS BRASIL", "3 CATALÁN",
       "4 GALLEGO", "5 ITALIANO", "6 FINES", "7 PORTUGUÉS"},
      {// 7 PORTUGUES
       "0 CASTELLANO", "1 ENGLISH", "2 PORTUGUÉS BRASIL", "3 CATALÁN",
       "4 GALLEGO", "5 ITALIANO", "6 FINES", "7 PORTUGUÉS"}};

  // limpia el área que ocupa el marcador
  clearMenuArea(0);

  // repintar con un enableEffect para que vaya apareciendo el
  // menu de izquierda a derecha y asi dar tiempo a soltar las teclas
  // al usuario

  // repinta todo el menu
  for (int x = enableEffect ? 8 : 88; x < 88; x += 10) {
    _cpc6128->fillMode1Rect(8, 0, x - 1, 160, 0);
    for (int i = 0; i < 8; i++) {
      _marcador->imprimeFrase(textos[_language][i], x, 32 + (i * 16), 4, 0);
    }
  }

  _cpc6128->fillMode1Rect(8, 0, 88, 160, 0);

  for (int i = 0; i < 8; i++) {
    _marcador->imprimeFrase(textos[_language][i], 88, 32 + (i * 16), 4, 0);
  }

  // pinta la opción seleccionado con el color de fondo y el color
  // de letra cambiado
  _marcador->imprimeFrase(textos[_language][selected], 88, 32 + (selected * 16),
                          0, 4);
}

void MainMenu::renderLoadMenu(int selected, bool enableEffect) {
  renderRecordingMenu(selected, enableEffect);
}

bool MainMenu::processLoadMenu(int seleccionado) {
  int pulsado = -1;
  bool salir = false;

  clearMenuArea(0);
  renderLoadMenu(seleccionado, true);

  while (!salir) {
    pulsado = -1;
    _inputController->actualizaEstado();

    if (_inputController->estaSiendoPulsado(P1_DOWN)) {
      seleccionado++;
      if (seleccionado == 8)
        seleccionado = 0;
      renderLoadMenu(seleccionado);
    }
    if (_inputController->estaSiendoPulsado(P1_UP)) {
      seleccionado--;
      if (seleccionado == -1)
        seleccionado = 7;
      renderLoadMenu(seleccionado);
    }

    if (_inputController->estaSiendoPulsado(P1_BUTTON1) ||
        _inputController->estaSiendoPulsado(KEYBOARD_INTRO)) {
      pulsado = seleccionado;
      salir = true;
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_0) || pulsado == 0) {
      _gameLogic->inicia();
      return cargar(0);
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_1) || pulsado == 1) {
      _gameLogic->inicia();
      return cargar(1);
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_2) || pulsado == 2) {
      _gameLogic->inicia();
      return cargar(2);
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_3) || pulsado == 3) {
      _gameLogic->inicia();
      return cargar(3);
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_4) || pulsado == 4) {
      _gameLogic->inicia();
      return cargar(4);
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_5) || pulsado == 5) {
      _gameLogic->inicia();
      return cargar(5);
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_6) || pulsado == 6) {
      _gameLogic->inicia();
      cargar(6);
      return cargar(6);
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_7) || pulsado == 7) {
      salir = true;
    }
  }

  return false;
}

void MainMenu::renderRecordingMenu(int seleccionado, bool enableEffect) {
  static const char *textos[8][8] = {
      {// 0 Castellano
       "0 SLOT 0 AUTOGUARDADO", "1 SLOT 1", "2 SLOT 2", "3 SLOT 3", "4 SLOT 4",
       "5 SLOT 5", "6 SLOT 6", "7 VOLVER AL MENU ANTERIOR"},
      {// 1 INGLES
       "0 SLOT 0 AUTOSAVE", "1 SLOT 1", "2 SLOT 2", "3 SLOT 3", "4 SLOT 4",
       "5 SLOT 5", "6 SLOT 6", "7 RETURN TO PREVIOUS MENU"},
      {// 2 PORTUGUES BRASIL
       "0 SLOT 0 AUTOGUARDADO", "1 SLOT 1", "2 SLOT 2", "3 SLOT 3", "4 SLOT 4",
       "5 SLOT 5", "6 SLOT 6", "7 VOLVER AL MENU ANTERIOR"},
      {// 3 CATALAN
       "0 SLOT 0 AUTOGUARDADO", "1 SLOT 1", "2 SLOT 2", "3 SLOT 3", "4 SLOT 4",
       "5 SLOT 5", "6 SLOT 6", "7 VOLVER AL MENU ANTERIOR"},
      {// 4 GALLEGO
       "0 SLOT 0 AUTOGUARDADO", "1 SLOT 1", "2 SLOT 2", "3 SLOT 3", "4 SLOT 4",
       "5 SLOT 5", "6 SLOT 6", "7 VOLVER AL MENU ANTERIOR"},
      {// 5 ITALIANO
       "0 SLOT 0 AUTOGUARDADO", "1 SLOT 1", "2 SLOT 2", "3 SLOT 3", "4 SLOT 4",
       "5 SLOT 5", "6 SLOT 6", "7 VOLVER AL MENU ANTERIOR"},
      {// 6 FINES
       "0 SLOT 0 AUTOSAVE", "1 SLOT 1", "2 SLOT 2", "3 SLOT 3", "4 SLOT 4",
       "5 SLOT 5", "6 SLOT 6", "7 RETURN TO PREVIOUS MENU"},
      {// 7 PORTUGUES
       "0 SLOT 0 AUTOGUARDADO", "1 SLOT 1", "2 SLOT 2", "3 SLOT 3", "4 SLOT 4",
       "5 SLOT 5", "6 SLOT 6", "7 VOLVER AL MENU ANTERIOR"}};
  // limpia el área que ocupa el marcador
  clearMenuArea(0);

  // repintar con un enableEffect para que vaya apareciendo el
  //  menu de izquierda a derecha y asi dar tiempo a soltar las teclas
  //  al usuario

  // repinta todo el menu
  for (int x = enableEffect ? 8 : 88; x < 88; x += 10) {
    _cpc6128->fillMode1Rect(8, 0, x - 1, 160, 0);
    for (int i = 0; i < 8; i++) {
      _marcador->imprimeFrase(textos[_language][i], x, 32 + (i * 16), 4, 0);
    }
  }
  _cpc6128->fillMode1Rect(8, 0, 88, 160, 0);
  for (int i = 0; i < 8; i++) {
    _marcador->imprimeFrase(textos[_language][i], 88, 32 + (i * 16), 4, 0);
  }

  // pinta la opción seleccionado con el color de fondo y el color
  // de letra cambiado
  _marcador->imprimeFrase(textos[_language][seleccionado], 88,
                          32 + (seleccionado * 16), 0, 4);
}

bool MainMenu::processRecordingMenu() {
  int seleccionado = 0;
  int pulsado = -1;
  bool salir = false;

  clearMenuArea(0);
  renderRecordingMenu(seleccionado, true);

  while (salir == false) {
    pulsado = -1;
    _inputController->actualizaEstado();

    if (_inputController->estaSiendoPulsado(P1_DOWN)) {
      seleccionado++;
      if (seleccionado == 8)
        seleccionado = 0;
      renderRecordingMenu(seleccionado);
    }
    if (_inputController->estaSiendoPulsado(P1_UP)) {
      seleccionado--;
      if (seleccionado == -1)
        seleccionado = 7;
      renderRecordingMenu(seleccionado);
    }

    if (_inputController->estaSiendoPulsado(P1_BUTTON1) ||
        _inputController->estaSiendoPulsado(KEYBOARD_INTRO)) {
      pulsado = seleccionado;
      salir = true;
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_0) || pulsado == 0) {
      save(0);
      salir = true;
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_1) || pulsado == 1) {
      save(1);
      salir = true;
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_2) || pulsado == 2) {
      save(2);
      salir = true;
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_3) || pulsado == 3) {
      save(3);
      salir = true;
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_4) || pulsado == 4) {
      save(4);
      salir = true;
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_5) || pulsado == 5) {
      save(5);
      salir = true;
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_6) || pulsado == 6) {
      save(6);
      salir = true;
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_7) || pulsado == 7) {
      salir = true;
    }
  }

  return false;
}

bool MainMenu::processIntro() {
  clearMenuArea(0);
  _pergamino->muestraTexto(Abadia::Pergamino::pergaminoIntroduccion[_language]);

  // espera a que se suelte el botón
  bool espera = true;

  while (espera) {
    _inputController->actualizaEstado();
    espera = _inputController->estaSiendoPulsado(P1_BUTTON1);
  }

  // Es necesario ya que el pergamino cambia la paleta
  _marcador->limpiaAreaMarcador();
  ReiniciaPantalla();

  while (_inputController->estaSiendoPulsado(P1_BUTTON1) ||
         _inputController->estaSiendoPulsado(KEYBOARD_INTRO)) {
    _inputController->actualizaEstado();
  }

  return false;
}

void MainMenu::renderIntro(int seleccionado) {
  static const char *textos[8][9] = {
      {// 0 Castellano
       "           MANEJO DEL TECLADO          ",
       "CURSOR ARRIBA: MOVER A GUILLERMO", "CURSOR ABAJO: MOVER A ADSO",
       "CURSOR IZQUIERDA: GIRAR A LA IZQUIERDA",
       "CURSOR DERECHA: GIRAR A LA DERECHA", "ESPACIO: DEJAR OBJETOS",
       "SUPR: PAUSA", "", "    -PULSA ESPACIO PARA CONTINUAR-"},
      {// 1 INGLES
       "                 KEYBOARD              ", "UP ARROW: MOVE WILLIAM",
       "DOWN ARROW: MOVE ADSO", "LEFT ARROW: TURN LEFT",
       "RIGHT ARROW: TURN RIGHT", "SPACE: DROP OBJECTS", "SUPR: PAUSE", "",
       "           -PRESS SPACE-          "},
      {
          // 2 PORTUGUES BRASIL
          "           MANEJO DEL TECLADO          ",
          "CURSOR ARRIBA: MOVER A GUILLERMO",
          "CURSOR ABAJO: MOVER A ADSO",
          "CURSOR IZQUIERDA: GIRAR A LA IZQUIERDA",
          "CURSOR DERECHA: GIRAR A LA DERECHA",
          "ESPACIO: DEJAR OBJETOS",
          "SUPR: PAUSA",
          "",
      },
      {
          // 3 CATALAN
          "           MANEJO DEL TECLADO          ",
          "CURSOR ARRIBA: MOVER A GUILLERMO",
          "CURSOR ABAJO: MOVER A ADSO",
          "CURSOR IZQUIERDA: GIRAR A LA IZQUIERDA",
          "CURSOR DERECHA: GIRAR A LA DERECHA",
          "ESPACIO: DEJAR OBJETOS",
          "SUPR: PAUSA",
          "",
      },
      {
          // 4 GALLEGO
          "           MANEJO DEL TECLADO          ",
          "CURSOR ARRIBA: MOVER A GUILLERMO",
          "CURSOR ABAJO: MOVER A ADSO",
          "CURSOR IZQUIERDA: GIRAR A LA IZQUIERDA",
          "CURSOR DERECHA: GIRAR A LA DERECHA",
          "ESPACIO: DEJAR OBJETOS",
          "SUPR: PAUSA",
          "",
      },
      {
          // 5 ITALIANO
          "           MANEJO DEL TECLADO          ",
          "CURSOR ARRIBA: MOVER A GUILLERMO",
          "CURSOR ABAJO: MOVER A ADSO",
          "CURSOR IZQUIERDA: GIRAR A LA IZQUIERDA",
          "CURSOR DERECHA: GIRAR A LA DERECHA",
          "ESPACIO: DEJAR OBJETOS",
          "SUPR: PAUSA",
          "",
      },
      {
          // 6 FINES
          "                 KEYBOARD              ",
          "UP ARROW: MOVE WILLIAM",
          "DOWN ARROW: MOVE ADSO",
          "LEFT ARROW: TURN LEFT",
          "RIGHT ARROW: TURN RIGHT",
          "SPACE: DROP OBJECTS",
          "SUPR: PAUSE",
          "",
      },
      {
          // 7 PORTUGUES
          "           MANEJO DEL TECLADO          ",
          "CURSOR ARRIBA: MOVER A GUILLERMO",
          "CURSOR ABAJO: MOVER A ADSO",
          "CURSOR IZQUIERDA: GIRAR A LA IZQUIERDA",
          "CURSOR DERECHA: GIRAR A LA DERECHA",
          "ESPACIO: DEJAR OBJETOS",
          "SUPR: PAUSA",
          "",
      }};
  // limpia el área que ocupa el marcador
  clearMenuArea(0);

  for (int i = 0; i < 9; i++) {
    _marcador->imprimeFrase(textos[_language][i], 8, 16 + (i * 16), 4, 0);
  }

  // pinta la opción seleccionado con el color de fondo y el color
  // de letra cambiado
  _marcador->imprimeFrase(textos[_language][seleccionado], 8,
                          16 + (seleccionado * 16), 0, 4);
}

bool MainMenu::processKeyboardMenu() {
  clearMenuArea(0);

  _pergamino->muestraTexto(Abadia::Pergamino::pergaminoManejo[_language]);

  // espera a que se suelte el botón
  bool espera = true;

  while (espera) {
    _inputController->actualizaEstado();
    espera = _inputController->estaSiendoPulsado(P1_BUTTON1);
  }

  // Es necesario ya que el pergamino
  // cambia la paleta
  _marcador->limpiaAreaMarcador();
  ReiniciaPantalla();
  renderKeyboardMenu(0);

  bool salir = false;
  while (salir == false) {
    _inputController->actualizaEstado();

    if (_inputController->estaSiendoPulsado(P1_BUTTON1) ||
        _inputController->estaSiendoPulsado(KEYBOARD_INTRO)) {
      salir = true;
    }
  }

  return false;
}

void MainMenu::renderKeyboardMenu(int seleccionado) {
  static const char *textos[8][9] = {
      {// 0 Castellano
       "           MANEJO DEL TECLADO          ",
       "CURSOR ARRIBA: MOVER A GUILLERMO", "CURSOR ABAJO: MOVER A ADSO",
       "CURSOR IZQUIERDA: GIRAR A LA IZQUIERDA",
       "CURSOR DERECHA: GIRAR A LA DERECHA", "ESPACIO: DEJAR OBJETOS",
       "SUPR: PAUSA", "", "    -PULSA ESPACIO PARA CONTINUAR-"},
      {// 1 INGLES
       "                 KEYBOARD              ", "UP ARROW: MOVE WILLIAM",
       "DOWN ARROW: MOVE ADSO", "LEFT ARROW: TURN LEFT",
       "RIGHT ARROW: TURN RIGHT", "SPACE: DROP OBJECTS", "SUPR: PAUSE", "",
       "           -PRESS SPACE-          "},
      {
          // 2 PORTUGUES BRASIL
          "           MANEJO DEL TECLADO          ",
          "CURSOR ARRIBA: MOVER A GUILLERMO",
          "CURSOR ABAJO: MOVER A ADSO",
          "CURSOR IZQUIERDA: GIRAR A LA IZQUIERDA",
          "CURSOR DERECHA: GIRAR A LA DERECHA",
          "ESPACIO: DEJAR OBJETOS",
          "SUPR: PAUSA",
          "",
      },
      {
          // 3 CATALAN
          "           MANEJO DEL TECLADO          ",
          "CURSOR ARRIBA: MOVER A GUILLERMO",
          "CURSOR ABAJO: MOVER A ADSO",
          "CURSOR IZQUIERDA: GIRAR A LA IZQUIERDA",
          "CURSOR DERECHA: GIRAR A LA DERECHA",
          "ESPACIO: DEJAR OBJETOS",
          "SUPR: PAUSA",
          "",
      },
      {
          // 4 GALLEGO
          "           MANEJO DEL TECLADO          ",
          "CURSOR ARRIBA: MOVER A GUILLERMO",
          "CURSOR ABAJO: MOVER A ADSO",
          "CURSOR IZQUIERDA: GIRAR A LA IZQUIERDA",
          "CURSOR DERECHA: GIRAR A LA DERECHA",
          "ESPACIO: DEJAR OBJETOS",
          "SUPR: PAUSA",
          "",
      },
      {
          // 5 ITALIANO
          "           MANEJO DEL TECLADO          ",
          "CURSOR ARRIBA: MOVER A GUILLERMO",
          "CURSOR ABAJO: MOVER A ADSO",
          "CURSOR IZQUIERDA: GIRAR A LA IZQUIERDA",
          "CURSOR DERECHA: GIRAR A LA DERECHA",
          "ESPACIO: DEJAR OBJETOS",
          "SUPR: PAUSA",
          "",
      },
      {
          // 6 FINES
          "                 KEYBOARD              ",
          "UP ARROW: MOVE WILLIAM",
          "DOWN ARROW: MOVE ADSO",
          "LEFT ARROW: TURN LEFT",
          "RIGHT ARROW: TURN RIGHT",
          "SPACE: DROP OBJECTS",
          "SUPR: PAUSE",
          "",
      },
      {
          // 7 PORTUGUES
          "           MANEJO DEL TECLADO          ",
          "CURSOR ARRIBA: MOVER A GUILLERMO",
          "CURSOR ABAJO: MOVER A ADSO",
          "CURSOR IZQUIERDA: GIRAR A LA IZQUIERDA",
          "CURSOR DERECHA: GIRAR A LA DERECHA",
          "ESPACIO: DEJAR OBJETOS",
          "SUPR: PAUSA",
          "",
      }};
  // limpia el área que ocupa el marcador
  clearMenuArea(0);

  for (int i = 0; i < 9; i++) {
    _marcador->imprimeFrase(textos[_language][i], 8, 16 + (i * 16), 4, 0);
  }

  // pinta la opción seleccionado con el color de fondo y el color
  // de letra cambiado
  _marcador->imprimeFrase(textos[_language][seleccionado], 8,
                          16 + (seleccionado * 16), 0, 4);
}

bool MainMenu::processOptionsMenu() {
  clearMenuArea(0);
  renderOptionsMenu(0);

  bool salir = false;
  while (salir == false) {
    _inputController->actualizaEstado();

    if (_inputController->estaSiendoPulsado(P1_BUTTON1) ||
        _inputController->estaSiendoPulsado(KEYBOARD_INTRO)) {
      salir = true;
    }
  }

  return false;
}

void MainMenu::renderOptionsMenu(int seleccionado) {
  static const char *textos[8][9] = {
      {// 0 Castellano
       "   USA ESTAS TECLAS DURANTE EL JUEGO   ", "G GRABAR LA PARTIDA",
       "C CARGAR LA PARTIDA", "F2 CAMBIAR ENTRE GRÁFICOS VGA O CPC",
       "F3 PANTALLA COMPLETA", "F5 MOSTRAR MAPAS", "SUPR PAUSA", "",
       "    -PULSA ESPACIO PARA CONTINUAR-"},
      {// 1 INGLES
       "            KEYBOARD SHORTCUTS         ", "G SAVE GAME", "C LOAD GAME",
       "F2 SWITCH VGA CPC GRAPHICS", "F3 FULLSCREEN", "F5 SHOW MAPS",
       "SUPR PAUSE", "", "           -PRESS SPACE-          "},
      {// 2 PORTUGUES BRASIL
       "   USA ESTAS TECLAS DURANTE EL JUEGO   ", "G GRABAR LA PARTIDA",
       "C CARGAR LA PARTIDA", "F2 CAMBIAR ENTRE GRÁFICOS VGA O CPC",
       "F3 PANTALLA COMPLETA", "F5 MOSTRAR MAPAS", "SUPR PAUSA", "",
       "    -PULSA ESPACIO PARA CONTINUAR-"},
      {// 3 CATALAN
       "   USA ESTAS TECLAS DURANTE EL JUEGO   ", "G GRABAR LA PARTIDA",
       "C CARGAR LA PARTIDA", "F2 CAMBIAR ENTRE GRÁFICOS VGA O CPC",
       "F3 PANTALLA COMPLETA", "F5 MOSTRAR MAPAS", "SUPR PAUSA", "",
       "    -PULSA ESPACIO PARA CONTINUAR-"},
      {// 4 GALLEGO
       "   USA ESTAS TECLAS DURANTE EL JUEGO   ", "G GRABAR LA PARTIDA",
       "C CARGAR LA PARTIDA", "F2 CAMBIAR ENTRE GRÁFICOS VGA O CPC",
       "F3 PANTALLA COMPLETA", "F5 MOSTRAR MAPAS", "SUPR PAUSA", "",
       "    -PULSA ESPACIO PARA CONTINUAR-"},
      {// 5 ITALIANO
       "   USA ESTAS TECLAS DURANTE EL JUEGO   ", "G GRABAR LA PARTIDA",
       "C CARGAR LA PARTIDA", "F2 CAMBIAR ENTRE GRÁFICOS VGA O CPC",
       "F3 PANTALLA COMPLETA", "F5 MOSTRAR MAPAS", "SUPR PAUSA", "",
       "    -PULSA ESPACIO PARA CONTINUAR-"},
      {// 6 FINES
       "            KEYBOARD SHORTCUTS         ", "G SAVE GAME", "C LOAD GAME",
       "F2 SWITCH VGA CPC GRAPHICS", "F3 FULLSCREEN", "F5 SHOW MAPS",
       "SUPR PAUSE", "", "           -PRESS SPACE-          "},
      {// 7 PORTUGUES
       "   USA ESTAS TECLAS DURANTE EL JUEGO   ", "G GRABAR LA PARTIDA",
       "C CARGAR LA PARTIDA", "F2 CAMBIAR ENTRE GRÁFICOS VGA O CPC",
       "F3 PANTALLA COMPLETA", "F5 MOSTRAR MAPAS", "SUPR PAUSA", "",
       "    -PULSA ESPACIO PARA CONTINUAR-"}};
  // limpia el área que ocupa el marcador
  clearMenuArea(0);

  for (int i = 0; i < 9; i++) {
    _marcador->imprimeFrase(textos[_language][i], 8, 16 + (i * 16), 4, 0);
  }

  // pinta la opción seleccionado con el color de fondo y el color
  // de letra cambiado
  _marcador->imprimeFrase(textos[_language][seleccionado], 8,
                          16 + (seleccionado * 16), 0, 4);
}

bool MainMenu::processCameraMenu() {
  clearMenuArea(0);
  renderCameraMenu(0);

  bool salir = false;
  while (salir == false) {
    _inputController->actualizaEstado();

    if (_inputController->estaSiendoPulsado(P1_BUTTON1) ||
        _inputController->estaSiendoPulsado(KEYBOARD_INTRO)) {
      salir = true;
    }
  }

  while (_inputController->estaSiendoPulsado(P1_BUTTON1) ||
         _inputController->estaSiendoPulsado(KEYBOARD_INTRO)) {
    _inputController->actualizaEstado();
  }

  return false;
}

void MainMenu::renderCameraMenu(int seleccionado) {
  static const char *textos[8][9] = {
      {// 0 Castellano
       "   USA ESTAS TECLAS DURANTE EL JUEGO   ",
       "1 DEJAD PULSADO PARA SEGUIR AL ABAD",
       "2 DEJAD PULSADO PARA SEGUIR A SEVERINO",
       "3 DEJAD PULSADO PARA SEGUIR A MALAQUIAS",
       "4 DEJAD PULSADO PARA SEGUIR A BERENGARIO",
       "5 DEJAD PULSADO PARA SEGUIR A JORGE",
       "6 DEJAD PULSADO PARA SEGUIR A BERNARDO",
       "7 DEJAD PULSADO PARA SEGUIR A ADSO",
       "    -PULSA ESPACIO PARA CONTINUAR-"

      },
      {// 1 INGLES
       "ERR", "", "", "", "", "", "", "", "PRESS SPACE TO CONTINUE"},
      {// 2 PORTUGUES BRASIL
       "TODO", "", "", "", "", "", "", "", "PRESS SPACE TO CONTINUE"},
      {// 3 CATALAN
       "TODO", "", "", "", "", "", "", "", "PRESS SPACE TO CONTINUE"},
      {// 4 GALLEGO
       "TODO", "", "", "", "", "", "", "", "PRESS SPACE TO CONTINUE"},
      {// 5 ITALIANO
       "TODO", "", "", "", "", "", "", "", "PRESS SPACE TO CONTINUE"},
      {// 6 FINES
       "TODO", "", "", "", "", "", "", "", "PRESS SPACE TO CONTINUE"},
      {// 7 PORTUGUES
       "TODO", "", "", "", "", "", "", "", "PRESS SPACE TO CONTINUE"}};
  // limpia el área que ocupa el marcador
  clearMenuArea(0);

  for (int i = 0; i < 9; i++) {
    _marcador->imprimeFrase(textos[_language][i], 0, 16 + (i * 16), 4, 0);
  }

  // pinta la opción seleccionado con el color de fondo y el color
  // de letra cambiado
  _marcador->imprimeFrase(textos[_language][seleccionado], 1,
                          16 + (seleccionado * 16), 0, 4);
}

void MainMenu::renderTutorialMenu(int seleccionado, bool enableEffect) {
  static const char *textos[8][8] = {
      {// 0 Castellano
       "0 PENDIENTE ", "", "PULSA ESPACIO PARA CONTINUAR", "", "", "", "", ""},
      {// 1 INGLES
       "", "", "PULSA ESPACIO PARA CONTINUAR", "", "", "", "", ""},
      {// 2 PORTUGUES BRASIL
       "", "", "PULSA ESPACIO PARA CONTINUAR", "", "", "", "", ""},
      {// 3 CATALAN
       "", "", "PULSA ESPACIO PARA CONTINUAR", "", "", "", "", ""},
      {// 4 GALLEGO
       "", "", "-PULSA ESPACIO PARA CONTINUAR", "", "", "", "", ""},
      {// 5 ITALIANO
       "", "", "PULSA ESPACIO PARA CONTINUAR", "", "", "", "", ""},
      {// 6 FINES
       "", "PULSA ESPACIO PARA CONTINUAR", "", "", "", "", "", ""},
      {// 7 PORTUGUES
       "", "", "PULSA ESPACIO PARA CONTINUAR", "", "", "", "", ""}};
  // limpia el área que ocupa el marcador
  clearMenuArea(0);

  // repintar con un enableEffect para que vaya apareciendo el
  // menu de izquierda a derecha y asi dar tiempo a soltar las teclas
  // al usuario

  // repinta todo el menu
  for (int x = enableEffect ? 8 : 88; x < 88; x += 10) {
    _cpc6128->fillMode1Rect(8, 0, x - 1, 160, 0);
    for (int i = 0; i < 8; i++) {
      _marcador->imprimeFrase(textos[_language][i], x, 32 + (i * 16), 4, 0);
    }
  }
  _cpc6128->fillMode1Rect(8, 0, 88, 160, 0);

  for (int i = 0; i < 8; i++) {
    _marcador->imprimeFrase(textos[_language][i], 88, 32 + (i * 16), 4, 0);
  }

  // pinta la opción seleccionado con el color de fondo y el color
  // de letra cambiado
  _marcador->imprimeFrase(textos[_language][seleccionado], 88,
                          32 + (seleccionado * 16), 0, 4);
}

bool MainMenu::processTutorialMenu() {
  int seleccionado = 0;
  int pulsado = -1;

  clearMenuArea(0);
  renderTutorialMenu(seleccionado, true);

  bool salir = false;
  while (salir == false) {
    pulsado = -1;
    _inputController->actualizaEstado();

    if (_inputController->estaSiendoPulsado(P1_BUTTON1) ||
        _inputController->estaSiendoPulsado(KEYBOARD_INTRO)) {
      pulsado = seleccionado;
      salir = true;
    }
  }

  return false;
}

bool MainMenu::processHelpMenu() {
  int seleccionado = 0;
  int pulsado = -1;

  clearMenuArea(0);
  renderHelpMenu(seleccionado, true);

  bool salir = false;
  while (salir == false) {
    pulsado = -1;
    _inputController->actualizaEstado();

    if (_inputController->estaSiendoPulsado(P1_DOWN)) {
      seleccionado++;
      if (seleccionado == 6)
        seleccionado = 0;
      renderHelpMenu(seleccionado);
    }
    if (_inputController->estaSiendoPulsado(P1_UP)) {
      seleccionado--;
      if (seleccionado == -1)
        seleccionado = 5;
      renderHelpMenu(seleccionado);
    }
    if (_inputController->estaSiendoPulsado(P1_BUTTON1) ||
        _inputController->estaSiendoPulsado(KEYBOARD_INTRO)) {
      pulsado = seleccionado;
    }

    if (_inputController->estaSiendoPulsado(KEYBOARD_0) || pulsado == 0) {
      processIntro();
      renderHelpMenu(seleccionado);
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_1) || pulsado == 1) {
      processKeyboardMenu();
      renderHelpMenu(seleccionado);
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_2) || pulsado == 2) {
      processOptionsMenu();
      renderHelpMenu(seleccionado);
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_3) || pulsado == 3) {
      processCameraMenu();
      renderHelpMenu(seleccionado);
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_4) || pulsado == 4) {
      _pergamino->muestraTexto(
          Abadia::Pergamino::pergaminoReferencias[_language]);

      // espera a que se suelte el botón
      bool espera = true;

      while (espera) {
        _inputController->actualizaEstado();
        espera = _inputController->estaSiendoPulsado(P1_BUTTON1);
      }

      // Es necesario ya que el pergamino
      // cambia la paleta
      _marcador->limpiaAreaMarcador();
      ReiniciaPantalla();
      renderHelpMenu(seleccionado);
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_5) || pulsado == 5) {
      salir = true;
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_6) || pulsado == 6) {
      salir = true;
    }
    if (_inputController->estaSiendoPulsado(KEYBOARD_7) || pulsado == 7) {
      salir = true;
    }
  }

  return false;
}

void MainMenu::renderHelpMenu(int seleccionado, bool enableEffect) {
  static const char *textos[8][8] = {
      {// 0 Castellano
       "0 INTRODUCCION", "1 MANEJO DEL TECLADO", "2 AYUDAS Y MEJORAS",
       "3 CAMARAS", "4 REFERENCIAS", "5 VOLVER AL MENU ANTERIOR", "", ""},
      {// 1 INGLES
       "0 INTRODUCCION", "1 MANEJO DEL TECLADO", "2 AYUDAS Y MEJORAS",
       "3 CAMARAS", "4 REFERENCIAS", "5 VOLVER AL MENU ANTERIOR", "", ""},
      {// 2 PORTUGUES BRASIL
       "0 INTRODUCCION", "1 MANEJO DEL TECLADO", "2 AYUDAS Y MEJORAS",
       "3 CAMARAS", "4 REFERENCIAS", "5 VOLVER AL MENU ANTERIOR", "", ""},
      {// 3 CATALAN
       "0 INTRODUCCION", "1 MANEJO DEL TECLADO", "2 AYUDAS Y MEJORAS",
       "3 CAMARAS", "4 REFERENCIAS", "5 VOLVER AL MENU ANTERIOR", "", ""},
      {// 4 GALLEGO
       "0 INTRODUCCION", "1 MANEJO DEL TECLADO", "2 AYUDAS Y MEJORAS",
       "3 CAMARAS", "4 REFERENCIAS", "5 VOLVER AL MENU ANTERIOR", "", ""},
      {// 5 ITALIANO
       "0 INTRODUCCION", "1 MANEJO DEL TECLADO", "2 AYUDAS Y MEJORAS",
       "3 CAMARAS", "4 REFERENCIAS", "5 VOLVER AL MENU ANTERIOR", "", ""},
      {// 6 FINES
       "0 INTRODUCCION", "1 MANEJO DEL TECLADO", "2 AYUDAS Y MEJORAS",
       "3 CAMARAS", "4 REFERENCIAS", "5 VOLVER AL MENU ANTERIOR", "", ""},
      {// 7 PORTUGUES
       "0 INTRODUCCION", "1 MANEJO DEL TECLADO", "2 AYUDAS Y MEJORAS",
       "3 CAMARAS", "4 REFERENCIAS", "5 VOLVER AL MENU ANTERIOR", "", ""}};
  // limpia el área que ocupa el marcador
  clearMenuArea(0);

  // repintar con un enableEffect para que vaya apareciendo el
  // menu de izquierda a derecha y asi dar tiempo a soltar las teclas
  // al usuario

  // repinta todo el menu
  for (int x = enableEffect ? 8 : 88; x < 88; x += 10) {
    _cpc6128->fillMode1Rect(8, 0, x - 1, 160, 0);
    for (int i = 0; i < 8; i++) {
      _marcador->imprimeFrase(textos[_language][i], x, 32 + (i * 16), 4, 0);
    }
  }

  _cpc6128->fillMode1Rect(8, 0, 88, 160, 0);
  for (int i = 0; i < 8; i++) {
    _marcador->imprimeFrase(textos[_language][i], 88, 32 + (i * 16), 4, 0);
  }

  // pinta la opción seleccionado con el color de fondo y el color
  // de letra cambiado
  _marcador->imprimeFrase(textos[_language][seleccionado], 88,
                          32 + (seleccionado * 16), 0, 4);
}

// ! HELPER
void MainMenu::clearMenuArea(int color) {
  /* CPC
  _cpc6128->fillMode1Rect(0, 0, 32, 160, 3);
  _cpc6128->fillMode1Rect(32, 0, 256, 160, color);
  _cpc6128->fillMode1Rect(32 + 256, 0, 32, 160, 3);
  */
  // VGA
  _cpc6128->fillMode1Rect(0, 0, 32, 160, 0);
  _cpc6128->fillMode1Rect(32, 0, 256, 160, color);
  _cpc6128->fillMode1Rect(32 + 256, 0, 32, 160, 0);
}

void MainMenu::ReiniciaPantalla() {
  // limpia el área de juego y dibuja el marcador
  // CPC limpiaAreaJuego(0);
  clearMenuArea(12); // el 0 es el cyan en CPC, no se cual poner en VGA
  // pongo el 12 que es un amarillo cantoso, para comparar con Abadia32

  _marcador->dibujaMarcador();

  // inicia el contador de la interrupción
  // TODO: NO borrar, descomentar
  // contadorInterrupcion = 0;

  // pone una posición de pantalla inválida para que se redibuje la pantalla
  _gameEngine->posXPantalla = _gameEngine->posYPantalla = -1;

  // dibuja los objetos que tiene guillermo en el marcador
  // TODO: NO borrar, descomentar
  // _marcador->dibujaObjetos(personajes[0]->objetos, 0xff);

  // inicia el marcador (día y momento del día, obsequium y el espacio de las
  // frases)
  _marcador->muestraDiaYMomentoDia();
  _marcador->decrementaObsequium(0);
  _marcador->limpiaAreaFrases();
}

// TODO: Mover s Clase
bool MainMenu::cargar(int slot) {
  // TODO: Stub class

  return false;
}

void MainMenu::save(int slot) {
#ifdef __native_client__
  {
    // En Chrome bajo Linux se graba bien
    // pero en Chrome bajo windows da un error
    // al grabar indicando que no hay espacio libre
    // En Chrome "Versión 35.0.1904.0 canary"
    // al menos se puede si se crea inicialmente el
    // archivo para añadir, se cierra y luego se abre
    // ya para truncar
    std::ofstream out(savefile[slot], std::ofstream::out | std::ofstream::app);
    out.close();
  }
#endif

  // TODO: NO borrar, arreglar
  // std::ofstream out(savefile[slot], std::ofstream::out|std::ofstream::trunc);

  // out << _gameLogic;

  // if ( out.fail() )
  // {
  // 	/* CPC
  // 	   elMarcador->imprimeFrase("            ", 110, 164, 2, 3);
  // 	   elMarcador->imprimeFrase("¡¡¡ERROR!!!", 110, 164, 2, 3); */
  // 	// VGA
  // 	_marcador->imprimeFrase("                  ", 100, 164, 4, 0);
  // 	_marcador->imprimeFrase("ERROR: PRESS SPACE", 100, 164, 4, 0);

  // 	do
  // 	{
  // 		_inputController->actualizaEstado();
  // 	} while (_inputController->estaSiendoPulsado(P1_BUTTON1) == false);
  // }
}