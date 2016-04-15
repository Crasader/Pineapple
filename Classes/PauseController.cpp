//
//  PauseController.cpp
//  Pineapple
//

#include "PauseController.h"

const string PauseController::BUTTON_FILES[NUM_BUTTONS*2] = {"textures/buttons/resume.png", "textures/buttons/resume_inverse.png", "textures/buttons/restart.png", "textures/buttons/restart_inverse.png", "textures/buttons/level_select.png","textures/buttons/level_select_inverse.png", "textures/buttons/quit.png", "textures/buttons/quit_inverse.png"};

PauseController* PauseController::PAUSE_CONTROLLER = nullptr;

void PauseController::init(Node* worldNode, SceneManager* assets, RootLayer* root, InputController* input) {
    _rootNode = root;
    _pauseNode = Node::create();
    _pauseNode->retain();
    _inputController = input;
    _center = Vec2(root->getContentSize().width/2.0f, root->getContentSize().height/2.0f);
    
    Texture2D* image = assets->get<Texture2D>(PAUSE_SCREEN_OVERLAY);
    _backgroundOverlay = PolygonNode::createWithTexture(image);
    _backgroundOverlay->setScale(5.0f);
    _backgroundOverlay->setLocalZOrder(PAUSE_OVERLAY_Z_ORDER);
    _backgroundOverlay->retain();
    _pauseNode->addChild(_backgroundOverlay);
    for (int i = 0; i < NUM_BUTTONS; i++) {
        Button* button = Button::create();
        button->loadTextureNormal(BUTTON_FILES[i*2]);
        button->loadTexturePressed(BUTTON_FILES[i*2 + 1]);
        float bheight = button->getContentSize().height;
        button->setPositionY(_center.y - bheight/2.0f + BUTTON_HEIGHT/2.0f - (i * BUTTON_HEIGHT)/(float)NUM_BUTTONS);
        button->setLocalZOrder(BUTTON_Z_ORDER);
        button->setVisible(true);
        button->retain();
        _buttons[i] = button;
        _pauseNode->addChild(button);
        switch (i) {
            case 0: // resume
                button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
                    if (type == ui::Widget::TouchEventType::ENDED) {
                        this->unPause();
                    }
                });
                break;
            case 1: // restart
                button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
                    if (type == ui::Widget::TouchEventType::ENDED) {
                        _inputController->setReset();
                        this->unPause();
                    }
                });
                break;
            case 2: // level select
                button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
                    if (type == ui::Widget::TouchEventType::ENDED) {
                    }
                });
                break;
            case 3: // quit
                button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
                    if (type == ui::Widget::TouchEventType::ENDED) {
                    }
                });
                break;
        }
    }
}
