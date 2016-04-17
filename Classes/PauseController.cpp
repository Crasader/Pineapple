//
//  PauseController.cpp
//  Pineapple
//

#include "PauseController.h"

const string PauseController::BUTTON_FILES[NUM_BUTTONS*2] = {"textures/buttons/resume.png", "textures/buttons/resume_inverse.png", "textures/buttons/restart.png", "textures/buttons/restart_inverse.png", "textures/buttons/level_select.png","textures/buttons/level_select_inverse.png", "textures/buttons/quit.png", "textures/buttons/quit_inverse.png"};

PauseController* PauseController::PAUSE_CONTROLLER = nullptr;

void PauseController::init(AbsScreenController* gameController, Node* worldNode, SceneManager* assets, Node* root, InputController* input) {
    CCASSERT(PAUSE_CONTROLLER != nullptr, "NULL PAUSE CONTROLLER INITIALIZED");
    PAUSE_CONTROLLER->_gameController = gameController;
    PAUSE_CONTROLLER->_rootNode = root;
    PAUSE_CONTROLLER->_pauseNode = Node::create();
    PAUSE_CONTROLLER->_pauseNode->retain();
    PAUSE_CONTROLLER->_inputController = input;
    PAUSE_CONTROLLER->_center = Vec2(root->getContentSize().width/2.0f, root->getContentSize().height/2.0f);
    
    Texture2D* image = assets->get<Texture2D>(PAUSE_SCREEN_OVERLAY);
    PAUSE_CONTROLLER->_backgroundOverlay = PolygonNode::createWithTexture(image);
    PAUSE_CONTROLLER->_backgroundOverlay->setScale(5.0f);
    PAUSE_CONTROLLER->_backgroundOverlay->setLocalZOrder(PAUSE_OVERLAY_Z_ORDER);
    PAUSE_CONTROLLER->_backgroundOverlay->retain();
    PAUSE_CONTROLLER->_pauseNode->addChild(PAUSE_CONTROLLER->_backgroundOverlay);
    for (int i = 0; i < NUM_BUTTONS; i++) {
        Button* button = Button::create();
        button->loadTextureNormal(BUTTON_FILES[i*2]);
        button->loadTexturePressed(BUTTON_FILES[i*2 + 1]);
        float bheight = button->getContentSize().height;
        button->setPositionY(PAUSE_CONTROLLER->_center.y - bheight/2.0f + BUTTON_HEIGHT/2.0f - (i * BUTTON_HEIGHT)/(float)NUM_BUTTONS);
        button->setLocalZOrder(BUTTON_Z_ORDER);
        button->setVisible(true);
        button->retain();
        PAUSE_CONTROLLER->_buttons[i] = button;
        PAUSE_CONTROLLER->_pauseNode->addChild(button);
        switch (i) {
            case 0: // resume
                button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
                    if (type == ui::Widget::TouchEventType::ENDED) {
                        PAUSE_CONTROLLER->unPause();
                    }
                });
                break;
            case 1: // restart
                button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
                    if (type == ui::Widget::TouchEventType::ENDED) {
                        PAUSE_CONTROLLER->_inputController->setReset();
                        PAUSE_CONTROLLER->unPause();
                    }
                });
                break;
            case 2: // level select
                button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
                    if (type == ui::Widget::TouchEventType::ENDED) {
                        PAUSE_CONTROLLER->unPause();
                        PAUSE_CONTROLLER->_gameController->setTransitionStatus(TRANSITION_TO_LEVEL_SELECT);
                    }
                });
                break;
            case 3: // quit
                button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
                    if (type == ui::Widget::TouchEventType::ENDED) {
                        PAUSE_CONTROLLER->unPause();
                        PAUSE_CONTROLLER->_gameController->setTransitionStatus(TRANSITION_TO_EXIT);
                    }
                });
                break;
        }
    }
}
