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

#define PAUSE_OVERLAY_Z_ORDER  1
#define BUTTON_Z_ORDER         2
#define NUM_BUTTONS            4
#define BUTTON_HEIGHT          400.0f
#define PAUSE_MENU_Z_ORDER     10

using namespace cocos2d;
using namespace cocos2d::ui;

/** Both controller and view for the pause screen, this is simple enough that they do not need
 *  to be separated.
 *  This is a singleton class, the window controller for the entire game
 */
class PauseController {
public:
    // adds the pause screen overlay and buttons to the world
    void pause() {
        if (!_isPaused) {
            // is pause while holding down movement don't want it to be down when unpause
            _inputController->clear();
            _isPaused = true;
            for (int i = 0; i < NUM_BUTTONS; i++) {
                _buttons[i]->setPositionX(_center.x);
            }
            _rootNode->addChild(_pauseNode, PAUSE_MENU_Z_ORDER);
        }
    }
    
    void unPause() {
        _isPaused = false;
        _rootNode->removeChild(_pauseNode);
    }
    
    bool isPaused() {
        return _isPaused;
    }
    
    void init(Node* worldNode, SceneManager* assets, RootLayer* root, InputController* input);
    
    static PauseController* getController() {
        if (PAUSE_CONTROLLER) {
            return PAUSE_CONTROLLER;
        } else {
            PAUSE_CONTROLLER = new PauseController();
            return PAUSE_CONTROLLER;
        }
    }
    
    static void releaseController() {
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
    const static string BUTTON_FILES[NUM_BUTTONS*2];
    
};

#endif /* PauseController_h */
