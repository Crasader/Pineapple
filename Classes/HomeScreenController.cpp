//
//  HomeScreenController.cpp
//  Pineapple
//
//  Created by Michael Patashnik on 4/27/16.
//
//

#include "HomeScreenController.h"
#include "Const.h"

#define BUTTON_FONT_SIZE    30


bool HomeScreenController::init(Node *root, InputController* input) {
    return init(root, input, SCREEN);
}

bool HomeScreenController::init(Node *root, InputController *input, const Rect &rect) {
    _rootnode = root;
    
    _playButton = Button::create();
    initButtonTextOps(_playButton);
    _playButton->setTitleText("Play");
    _playButton->setTitleFontSize(BUTTON_FONT_SIZE);
    
    _playButton->setPosition(Vec2(100,100));
    
    _playButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            setTransitionStatus(TRANSITION_TO_LEVEL_SELECT);
        }
    });
    
    _playButton->retain();
    _rootnode->addChild(_playButton);
    
    
    _isInitted = true;
    return true;
}

void HomeScreenController::update(float dt) {
    
}

void HomeScreenController::dispose() {
    if (_playButton != nullptr)  {
        _playButton->release();
        _playButton = nullptr;
    }
}