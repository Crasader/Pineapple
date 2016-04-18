//
//  HUDController.h
//  Pineapple
//
//

#ifndef HUDController_h
#define HUDController_h

#include <stdio.h>
#include <cornell.h>
#include "ui/CocosGUI.h"
#include "Const.h"
#include "Texture.h"
#include "InputController.h"
#include "PauseController.h"
#include "AbsScreenController.h"
#include "Const.h"
#include "KidModel.h"
#include "PineappleModel.h"


#define         CHILDREN_STATUS_HORIZ_POS_RATIO  .04f
#define         PROGRESS_BAR_HORIZ_POS_RATIO     .15f
#define         SOUND_BUTTON_HORIZ_POS_RATIO     .85f
#define         PAUSE_BUTTON_HORIZ_POS_RATIO     .94f
#define         HUD_MIDDLE_Y_POS_RATIO           .9f
#define         HUD_Z_ORDER                      5
#define         BUTTON_SCALE                     .15f
#define         PROGRESS_BAR_SCALE_HORIZ         .6f
#define         PROGRESS_BAR_SCALE_VERT          .5f
// for mini blender etc
#define         BLENDER_ICON_SCALE               .4f
#define         KID_ICON_SCALE                   .8f
#define         WILL_SCALE                       .9f
#define         WILL_OFFSET_Y                    -10

// local z order
#define TOP_BAR_BACKGROUND_Z_ORDER  1
#define TOP_BAR_FOREGROUND_Z_ORDER  2

// 1 through 4 then slash
#define STATUS_SCALE            .24f
#define NUM_STATUS_NODES        5

// singleton class
using namespace cocos2d;
using namespace cocos2d::ui;

/** Both controller and view for the pause screen, this is simple enough that they do not need
 *  to be separated.
 *  This is a singleton class, the window controller for the entire game
 */
class HUDController {
public:
    // add nodes to rootNode for the hud
    static void addHUD() {
        if (HUD_CONTROLLER) {
            HUD_CONTROLLER->_rootNode->addChild(HUD_CONTROLLER->_hudNode, HUD_Z_ORDER);
        } else {
            cout << "HUD CONTROLLER IS NULL\n";
        }
    }
    
    // remove nodes to rootNode for the hud
    static void removeHUD() {
        HUD_CONTROLLER->_rootNode->removeChild(HUD_CONTROLLER->_hudNode);
    }
    
    static void init(AbsScreenController* gameController, Node* worldNode, SceneManager* assets, Node* root, InputController* input, float blenderPos);
    
    // update number of alive children icon and progress bar, blenderLoc should be right most x position
    static void update(int childrenAlive, float blenderLoc, vector<KidModel*> children, PineappleModel* will, float goalLoc);
    
    static void create() {
        if (!HUD_CONTROLLER) {
            HUD_CONTROLLER = new HUDController();
        }
    }
    
    static void release();
    
    static void reset(AbsScreenController* gameController, Node* worldNode, SceneManager* assets, Node* root, InputController* input, float blenderPos) {
        HUDController::release();
        HUDController::create();
        HUDController::init(gameController, worldNode, assets, root, input, blenderPos);
    }
    
    static void setEnabled(bool enabled) {
        HUD_CONTROLLER->_pauseButton->setEnabled(enabled);
        HUD_CONTROLLER->_soundButton->setEnabled(enabled);
    }
    
private:
    HUDController() { }
    ~HUDController() { }
    
    static void initChildrenStatus(SceneManager* assets);
    
    static void initProgressBar(SceneManager* assets);
    
    static void initPauseButton();
    
    static void initSoundButton();
    
    // if you have game units (game object location and goal location) this will return
    // relative units for the top progress bar, we're treating goalLoc as end of level.
    // you will have to add your getContentsize().width * your relative scale * scale / 2.0f to make it non
    // centered tho, srry
    static float unitsToTopBarX(float location, float goalLoc) {
        return HUD_CONTROLLER->_progressBarLeftXPos + (HUD_CONTROLLER->_progressBarWidth * location / goalLoc);
    }
    
    // static reference to singleton
    static HUDController* HUD_CONTROLLER;
    InputController* _inputController = nullptr;
    
    // reference to worldNode
    Node* _rootNode = nullptr;
    Node* _hudNode = nullptr;
    Vec2 _screenSize;
    Button* _pauseButton = nullptr;
    CheckBox* _soundButton = nullptr;
    SceneManager* _assets = nullptr;
    
    /** Reference to the game controller this is pausing for */
    AbsScreenController* _gameController;
    
    // to get widths you need to getcontentsize().width * their respective scale * cscale
    // 1 through 4 then the slash
    PolygonNode* _numbers[NUM_STATUS_NODES];
    PolygonNode* _blender = nullptr;
    PolygonNode* _will    = nullptr;
    PolygonNode* _children[KID_COUNT];
    PolygonNode* _topbar = nullptr;
    float _progressBarLeftXPos = 0.0f;
    float _progressBarWidth    = 0.0f;
};

#endif /* HUDController_h */
