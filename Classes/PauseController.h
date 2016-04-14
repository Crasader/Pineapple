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

#define PAUSE_OVERLAY_Z_ORDER  100
#define BUTTON_Z_ORDER         150
#define NUM_BUTTONS            3
#define BUTTON_HEIGHT          400.0f

using namespace cocos2d;
using namespace cocos2d::ui;

/** Both controller and view for the pause screen, this is simple enough that they do not need
 *  to be separated
 */
class PauseController {
public:
    PauseController() { }
    ~PauseController() {
        _backgroundOverlay->release();
        for (int i = 0; i < NUM_BUTTONS; i++) {
            _buttons[i]->release();
        }
    }
    
    // adds the pause screen overlay and buttons to the world
    void pause() {
        _worldNode->addChild(_backgroundOverlay);
        for (int i = 0; i < NUM_BUTTONS; i++) {
            _worldNode->addChild(_buttons[i]);
        }
    }
    
    void unPause() {
        _worldNode->removeChild(_backgroundOverlay);
        for (int i = 0; i < NUM_BUTTONS; i++) {
            _worldNode->removeChild(_buttons[i]);
        }
    }
    
    void init(Node* worldNode, SceneManager* assets, RootLayer* root);
    
private:
    // reference to worldNode
    Node* _worldNode = nullptr;
    Vec2 _center;
    // array of buttons
    Button* _buttons[NUM_BUTTONS];
    // background overlay
    PolygonNode* _backgroundOverlay;
    
};

#endif /* PauseController_h */
