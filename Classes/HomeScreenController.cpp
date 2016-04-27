//
//  HomeScreenController.cpp
//  Pineapple
//
//  Created by Michael Patashnik on 4/27/16.
//
//

#include "HomeScreenController.h"
#include <cornell.h>
#include "Texture.h"

#define PLAY_BUTTON_POSITION     Vec2(700,150)
#define PLAY_BUTTON_SCALE   2.0f

#define HOME_SCREEN_BACKGROUND_Z_INDEX  0
#define HOME_SCREEN_BUTTON_Z_INDEX      1

bool HomeScreenController::init(Node *root, InputController* input) {
    return init(root, input, SCREEN);
}

bool HomeScreenController::init(Node *root, InputController *input, const Rect &rect) {
    
    _assets = AssetManager::getInstance()->getCurrent();
    _input = input;
    
    // Determine the center of the screen
    Size dimen  = root->getContentSize();
    Vec2 center(dimen.width/2.0f,dimen.height/2.0f);
    
    // Create the scale and notify the input handler
    Vec2 scale = Vec2(root->getContentSize().width/rect.size.width,
                      root->getContentSize().height/rect.size.height);
    Rect screen = rect;
    screen.origin.x *= scale.x;    screen.origin.y *= scale.y;
    screen.size.width *= scale.x;  screen.size.height *= scale.y;
    
    _rootnode = root;
    _rootnode->retain();
    
    //Create the background image
    Texture2D* image = _assets->get<Texture2D>(HOME_SCREEN_BACKGROUND);
    _backgroundNode = PolygonNode::createWithTexture(image);
    _backgroundNode->setPosition(center);
    _backgroundNode->setAnchorPoint(Vec2(0.5f, 0.5f));
    _backgroundNode->setScale(dimen.width/image->getContentSize().width, dimen.height/image->getContentSize().height);
    
    _rootnode->addChild(_backgroundNode, HOME_SCREEN_BACKGROUND_Z_INDEX);

    //Create the play button
    _playButton = Button::create();
    initButtonTextOps(_playButton);
    _playButton->loadTextureNormal(HOME_SCREEN_PLAY_BUTTON_OFF_FILEPATH);
    _playButton->loadTexturePressed(HOME_SCREEN_PLAY_BUTTON_ON_FILEPATH);
    
    _playButton->setPosition(PLAY_BUTTON_POSITION);
    _playButton->setScale(PLAY_BUTTON_SCALE);
    
    _playButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            setTransitionStatus(TRANSITION_TO_LEVEL_SELECT);
        }
    });
    
    _playButton->retain();
    _rootnode->addChild(_playButton, HOME_SCREEN_BUTTON_Z_INDEX);
    
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