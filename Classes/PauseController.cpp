//
//  PauseController.cpp
//  Pineapple
//

#include "PauseController.h"

const string PauseController::BUTTON_FILES[NUM_BUTTONS*2] = {"textures/buttons/resume.png", "textures/buttons/resume_inverse.png", "textures/buttons/restart.png", "textures/buttons/restart_inverse.png", "textures/buttons/level_select","textures/buttons/level_select_inverse", "textures/buttons/quit", "textures/buttons/quit_inverse"};

void PauseController::init(Node* worldNode, SceneManager* assets, RootLayer* root) {
    _worldNode = worldNode;
    _center = Vec2(root->getContentSize().width/2.0f, root->getContentSize().height/2.0f);
    
    Texture2D* image = assets->get<Texture2D>(PAUSE_SCREEN_OVERLAY);
    _backgroundOverlay = PolygonNode::createWithTexture(image);
    _backgroundOverlay->setPosition(0,0);
    _backgroundOverlay->setScale(5.0f);
    _backgroundOverlay->setLocalZOrder(PAUSE_OVERLAY_Z_ORDER);
    _backgroundOverlay->retain();
    for (int i = 0; i < NUM_BUTTONS; i++) {
        Button* button = Button::create();
        button->loadTextureNormal(BUTTON_FILES[i*2]);
        button->loadTexturePressed(BUTTON_FILES[i*2 + 1]);
        float bheight = button->getContentSize().height;
        button->setPositionY(_center.y - bheight/2.0f + BUTTON_HEIGHT/2.0f - (i * BUTTON_HEIGHT)/(float)NUM_BUTTONS);
        button->setPositionX(_center.x);
        button->setLocalZOrder(BUTTON_Z_ORDER);
        button->setTitleText("hi");
        button->setVisible(true);
        button->retain();
        _buttons[i] = button;
        switch (i) {
            case 0:
                button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
                    switch (type)
                    {
                        case ui::Widget::TouchEventType::BEGAN:
                            break;
                        case ui::Widget::TouchEventType::ENDED:
                            std::cout << "Button 0 clicked" << std::endl;
                            break;
                        default:
                            break;
                    }
                });
                break;
            case 1:
                button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
                    switch (type)
                    {
                        case ui::Widget::TouchEventType::BEGAN:
                            break;
                        case ui::Widget::TouchEventType::ENDED:
                            std::cout << "Button 1 clicked" << std::endl;
                            break;
                        default:
                            break;
                    }
                });
                break;
            case 2:
                button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
                    switch (type)
                    {
                        case ui::Widget::TouchEventType::BEGAN:
                            break;
                        case ui::Widget::TouchEventType::ENDED:
                            std::cout << "Button 2 clicked" << std::endl;
                            break;
                        default:
                            break;
                    }
                });
                break;
            case 3:
                button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
                    switch (type)
                    {
                        case ui::Widget::TouchEventType::BEGAN:
                            break;
                        case ui::Widget::TouchEventType::ENDED:
                            std::cout << "Button 3 clicked" << std::endl;
                            break;
                        default:
                            break;
                    }
                });
                break;
        }
    }
}
