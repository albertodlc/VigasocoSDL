#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "Logica.h"
#include "Marcador.h"
#include "TimingHandler.h"
#include "Controles.h"
#include "cpc6128.h"
#include "Pergamino.h"
#include "MotorGrafico.h"

class MainMenu {
public:
    MainMenu(int language, Abadia::Pergamino *pergamino, Abadia::MotorGrafico* gameEngine, Abadia::Logica* gameLogic, Abadia::Controles *inputController, Abadia::Marcador* marcador, CPC6128 *cpc6128, TimingHandler *timer );
    ~MainMenu() = default;

    void setTimer(TimingHandler *timer){ _timer = timer; }
    void setInputController(Abadia::Controles *inputController){ _inputController = inputController; }

    bool process(int seleccionado = 8);
    bool processLanguageMenu(int seleccionado);
    bool processLoadMenu(int seleccionado = 0);
    bool processRecordingMenu();
    bool processIntro();
    bool processKeyboardMenu();
    bool processOptionsMenu();
    bool processCameraMenu();
    bool processTutorialMenu();
    bool processHelpMenu();

private:
    int _language;

    // This is the "storage slot" that stays alive as long as the class exists
    Abadia::Pergamino* _pergamino;
    Abadia::MotorGrafico* _gameEngine;
    Abadia::Logica* _gameLogic;
    Abadia::Controles* _inputController;
    Abadia::Marcador* _marcador;

    CPC6128* _cpc6128;
    TimingHandler* _timer;

    // Helper for clearing the menu area
    void clearMenuArea(int color);
    void ReiniciaPantalla();

    void renderMenu(int selected, bool enableEffect = false);
    void renderLanguageMenu(int selected, bool enableEffect = false);
    void renderLoadMenu(int selected, bool enableEffect = false);
    void renderRecordingMenu(int seleccionado, bool enableEffect = false);
    void renderIntro(int seleccionado);
    void renderKeyboardMenu(int seleccionado);
    void renderOptionsMenu(int seleccionado);
    void renderCameraMenu(int seleccionado);
    void renderTutorialMenu(int seleccionado, bool enableEffect = false);
    void renderHelpMenu(int seleccionado, bool enableEffect = false);

    // TODO Mover a otra clase
    void save(int slot);
    bool cargar(int slot);

};

#endif