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
    initPauseButton();
    addHUD();
}

void HUDController::initPauseButton() {
    HUD_CONTROLLER->_pauseButton = Button::create();
    HUD_CONTROLLER->_pauseButton->loadTextureNormal("textures/buttons/pauseButton.png");
    HUD_CONTROLLER->_pauseButton->loadTexturePressed("textures/buttons/pause_inverse.png");
    HUD_CONTROLLER->_pauseButton->cocos2d::Node::setScale(HUD_SCALE);
    float bheight = HUD_CONTROLLER->_pauseButton->getContentSize().height;
    float bwidth  = HUD_CONTROLLER->_pauseButton->getContentSize().width;
    HUD_CONTROLLER->_pauseButton->setPositionY(HUD_CONTROLLER->_screenSize.y * HUD_MIDDLE_Y_POS_RATIO);
    HUD_CONTROLLER->_pauseButton->setPositionX(HUD_CONTROLLER->_screenSize.x * PAUSE_BUTTON_HORIZ_POS_RATIO);
    HUD_CONTROLLER->_pauseButton->retain();
    HUD_CONTROLLER->_hudNode->addChild(HUD_CONTROLLER->_pauseButton);
    HUD_CONTROLLER->_pauseButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            PauseController::pause();
        }
    });
}