//
//  PauseController.h
//  Pineapple
//

#ifndef PauseController_h
#define PauseController_h

#include <stdio.h>
#include <cornell.h>
#include "ui/CocosGUI.h"
#include "Const.h"
#include "Texture.h"
#include "InputController.h"
#include "AbsScreenController.h"

#define PAUSE_OVERLAY_Z_ORDER  1
#define BUTTON_Z_ORDER         2
#define NUM_BUTTONS            4
#define BUTTON_HEIGHT          400.0f
#define PAUSE_MENU_Z_ORDER     6

using namespace cocos2d;
using namespace cocos2d::ui;

/** Both controller and view for the pause screen, this is simple enough that they do not need
 *  to be separated.
 *  This is a singleton class, the window controller for the entire game
 */
class PauseController {
public:
    // adds the pause screen overlay and buttons to the world
    static void pause() {
        if (PAUSE_CONTROLLER) {
            if (!PAUSE_CONTROLLER->_isPaused) {
                // is pause while holding down movement don't want it to be down when unpause
                PAUSE_CONTROLLER->_inputController->clear();
                PAUSE_CONTROLLER->_isPaused = true;
                for (int i = 0; i < NUM_BUTTONS; i++) {
                    PAUSE_CONTROLLER->_buttons[i]->setPositionX(PAUSE_CONTROLLER->_center.x);
                }
                PAUSE_CONTROLLER->_rootNode->addChild(PAUSE_CONTROLLER->_pauseNode, PAUSE_MENU_Z_ORDER);
            }
        }
    }
    
    static void unPause() {
        if (PAUSE_CONTROLLER) {
            PAUSE_CONTROLLER->_isPaused = false;
            PAUSE_CONTROLLER->_rootNode->removeChild(PAUSE_CONTROLLER->_pauseNode);
        }
    }
    
    static bool isPaused() {
        return PAUSE_CONTROLLER->_isPaused;
    }
    
    static void init(AbsScreenController* gameController, Node* worldNode, SceneManager* assets, Node* root, InputController* input);
    
    static bool isInitialized() {
        return PAUSE_CONTROLLER != nullptr;
    }
    
    static void create() {
        if (!PAUSE_CONTROLLER) {
            PAUSE_CONTROLLER = new PauseController();
        }
    }
    
    static void release() {
        PAUSE_CONTROLLER->~PauseController();
    }
    
private:
    PauseController() { }
    ~PauseController() {
        _backgroundOverlay->release();
        for (int i = 0; i < NUM_BUTTONS; i++) {
            _buttons[i]->release();
        }
        _pauseNode->release();
    }
    
    // static reference to singleton
    static PauseController* PAUSE_CONTROLLER;
    InputController* _inputController = nullptr;
    
    bool _isPaused = false;
    // reference to worldNode
    Node* _rootNode = nullptr;
    Node* _pauseNode = nullptr;
    Vec2 _center;
    // array of buttons
    Button* _buttons[NUM_BUTTONS];
    // background overlay
    PolygonNode* _backgroundOverlay;
    
    /** Reference to the game controller this is pausing for */
    AbsScreenController* _gameController;
    
    const static string BUTTON_FILES[NUM_BUTTONS*2];
    
};

#endif /* PauseController_h */
