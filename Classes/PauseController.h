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

// global z order
#define PAUSE_MENU_Z_ORDER     6

#define NUM_BUTTONS            4
#define BUTTON_Y_OFFSET        60.0f
#define BUTTON_X_OFFSET        -20.0f
#define BUTTON_HEIGHT          200.0f
#define BUTTON_COL_OFFSET      200.0f
#define BUTTONS_PER_COL        2

// local z orders
#define PAUSE_OVERLAY_Z_ORDER  1
#define CHUNKY_Z_ORDER     2
#define BUTTON_Z_ORDER         3

#define ANIMATION_FRAMES       3

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
    
    static void animate() {
        PAUSE_CONTROLLER->_chunkyQuiver->setFrame((int)PAUSE_CONTROLLER->_animationCycle % ANIMATION_FRAMES);
        PAUSE_CONTROLLER->_animationCycle += .2;
    }
    
private:
    PauseController() { }
    ~PauseController() {
        _backgroundOverlay->release();
        for (int i = 0; i < NUM_BUTTONS; i++) {
            _buttons[i]->release();
        }
        _pauseNode->release();
        _chunkyQuiver->release();
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
    PolygonNode* _backgroundOverlay = nullptr;
    // animation in background
    AnimationNode* _chunkyQuiver = nullptr;
    float _animationCycle = 0.0;
    
    /** Reference to the game controller this is pausing for */
    AbsScreenController* _gameController;
    
    const static string BUTTON_FILES[NUM_BUTTONS*2];
};

#endif /* PauseController_h */
