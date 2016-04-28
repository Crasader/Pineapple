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

#define PLAY_BUTTON_POSITION            Vec2(700,150)
#define PLAY_BUTTON_SCALE               2.0f

#define WILL_POPUP_FRAME_COUNT          12
#define WILL_POPUP_OFFSCREEN            Vec2(-100, -100)
#define WILL_POPUP_SCALE                0.75f

#define HAPPY_KID_FRAME_COUNT           14
#define KID_SCALE                       0.5f
#define KID_SPACING                     70
#define KID_BASE_POSITION               Vec2(80, 190)

#define MIN_DELAY                       15
#define MAX_DELAY                       30

#define HOME_SCREEN_BACKGROUND_Z_INDEX  0
#define HOME_SCREEN_BUTTON_Z_INDEX      1
#define HOME_SCREEN_KID_Z_INDEX         2
#define HOME_SCREEN_WILL_Z_INDEX        3

bool HomeScreenController::init(Node *root, InputController* input) {
    return init(root, input, SCREEN);
}

int getRandomDelay() {
    return (rand() % (MAX_DELAY - MIN_DELAY)) + MIN_DELAY;
}

bool HomeScreenController::init(Node *root, InputController *input, const Rect &rect) {
    
    _assets = AssetManager::getInstance()->getCurrent();
    _input = input;
    
    float cscale = Director::getInstance()->getContentScaleFactor();
    
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
    _playButton->setScale(PLAY_BUTTON_SCALE * cscale);
    
    _playButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            setTransitionStatus(TRANSITION_TO_LEVEL_SELECT);
        }
    });
    
    _playButton->retain();
    _rootnode->addChild(_playButton, HOME_SCREEN_BUTTON_Z_INDEX);
    
    //Create the will animation
    _willPopup = AnimationNode::create(_assets->get<Texture2D>(HOME_SCREEN_WILL_ANIMATION), 1, WILL_POPUP_FRAME_COUNT);
    _willPopup->setAnchorPoint(Vec2(0,0));
    _willPopup->setPosition(WILL_POPUP_OFFSCREEN);
    _willPopup->setScale(WILL_POPUP_SCALE * cscale);
    _willPopup->retain();
    _rootnode->addChild(_willPopup, HOME_SCREEN_WILL_Z_INDEX);
    
    _willFrame = - getRandomDelay();
    
    string texIDs[] = {WIN_SPLASH_CHILD_4, WIN_SPLASH_CHILD_1, WIN_SPLASH_CHILD_3, WIN_SPLASH_CHILD_2};
    for (int i = 0; i < KID_COUNT; i++) {
        image = _assets->get<Texture2D>(texIDs[i]);
        _kidAnimations[i] = AnimationNode::create(image, 1, HAPPY_KID_FRAME_COUNT, HAPPY_KID_FRAME_COUNT);
        _kidAnimations[i]->setAnchorPoint(Vec2(0, 0));
        _kidAnimations[i]->setScale(KID_SCALE * cscale);
        
        _kidAnimations[i]->setPosition(KID_BASE_POSITION + Vec2(i * KID_SPACING, 0));
        
        _kidAnimations[i]->retain();
        _rootnode->addChild(_kidAnimations[i], HOME_SCREEN_KID_Z_INDEX);
    }
    
    for (int i = 0; i < KID_COUNT; i++) {
        _kidFrames[i] = i*2;
    }
    
    _isInitted = true;
    return true;
}

void HomeScreenController::positionWill() {
    
    int side = rand() % 4;
    if (side == _mostRecentSide) {
        side = (side + 1) % 4;
    }
    
    _mostRecentSide = side;
    _willPopup->setRotation(90 * side);
    
    int x = (rand() % (int)(_rootnode->getContentSize().width - _willPopup->getContentSize().width)) + _willPopup->getContentSize().width / 2;
    int y = (rand() % (int)(_rootnode->getContentSize().height - _willPopup->getContentSize().width)) + _willPopup->getContentSize().width / 2;
    
    if (side == 0) {
        y = 0;
    } else if (side == 1) {
        x = 0;
    } else if (side == 2) {
        y = (int)_rootnode->getContentSize().height;
    } else {
        x = (int)_rootnode->getContentSize().width;
    }
    
    _willPopup->setPosition(x, y);
}

void HomeScreenController::update(float dt) {
    _input->update(dt);
    
    if (_willFrame >= 0 && _willFrame <= 0.2) {
        positionWill();
    } else if (_willFrame > 0 && (int)_willFrame < WILL_POPUP_FRAME_COUNT) {
        _willPopup->setFrame((int)_willFrame % WILL_POPUP_FRAME_COUNT);
    } else if ((int)_willFrame >= WILL_POPUP_FRAME_COUNT) {
        _willFrame = -getRandomDelay();
        _willPopup->setPosition(WILL_POPUP_OFFSCREEN);
    }
    
    _willFrame += 0.2;
    
    for (int i = 0; i < KID_COUNT; i++) {
        _kidFrames[i] += 0.4;
        _kidAnimations[i]->setFrame((int)(_kidFrames[i])%HAPPY_KID_FRAME_COUNT);
    }
}

void HomeScreenController::dispose() {
    if (_willPopup != nullptr) {
        _willPopup->release();
        _willPopup = nullptr;
    }
    
    if (_playButton != nullptr)  {
        _playButton->release();
        _playButton = nullptr;
    }
    
    for(int i = 0; i <KID_COUNT; i++) {
        if (_kidAnimations[i] != nullptr) {
            _kidAnimations[i]->release();
        }
        _kidAnimations[i] = nullptr;
    }
}