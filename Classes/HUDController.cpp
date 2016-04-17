//
//  HUDController.cpp
//  Pineapple
//
//

#include "HUDController.h"

HUDController* HUDController::HUD_CONTROLLER = nullptr;

void HUDController::init(AbsScreenController* gameController, Node* worldNode, SceneManager* assets, Node* root, InputController* input) {
    CCASSERT(HUD_CONTROLLER != nullptr, "NULL HUD CONTROLLER INITIALIZED");
    HUD_CONTROLLER->_gameController = gameController;
    HUD_CONTROLLER->_rootNode = root;
    HUD_CONTROLLER->_hudNode = Node::create();
    HUD_CONTROLLER->_hudNode->retain();
    HUD_CONTROLLER->_inputController = input;
    HUD_CONTROLLER->_screenSize = Vec2(root->getContentSize().width, root->getContentSize().height);
    
    // init children status
    // init progress bar
    // init sound button
    // init pause button
    
}