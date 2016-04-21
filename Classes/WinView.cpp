//
//  LoseView.cpp
//  Pineapple
//
//  Created by Michael Patashnik on 4/18/16.
//
//

#include "WinView.h"

#define GRADIENT_Z      0
#define SPLASH_Z        1
#define PINEAPPLE_Z     2
#define BUTTON_Z        3

#define HAPPY_KID_FRAME_COUNT   14
#define HAPPY_WILL_FRAME_COUNT  28

#define WILL_SCALE      1.0f
#define KID_SCALE       0.25f
#define KID_SPACING     1.0f/7.0f

#define BUTTON_SCALE            0.8f
#define BUTTON_FONT_SIZE        30
#define NEXT_LEVEL_TEXT         "Next\nLevel"
#define RESET_TEXT              "Try\nAgain"
#define LEVEL_SELECT_TEXT       "Level\nSelect"

using namespace std;

WinView* WinView::create(Node* root, SceneManager *assets, Vec2 scale) {
    WinView* view = new (std::nothrow) WinView();
    
    view->init(root, assets, scale);
    
    root->addChild(view->_backgroundOverlay, GRADIENT_Z);
    root->addChild(view->_splashImage, SPLASH_Z);
    root->addChild(view->_willAnimation, PINEAPPLE_Z);
    
    for(int i = 0; i < KID_COUNT; i++) {
        if (view->_kidAnimations[i] != nullptr) {
            root->addChild(view->_kidAnimations[i],PINEAPPLE_Z);
        }
    }
    
    root->addChild(view->_toNextLevelButton, BUTTON_Z);
    root->addChild(view->_resetButton, BUTTON_Z);
    root->addChild(view->_toLevelSelectButton, BUTTON_Z);
    
    return view;
}

void WinView::init(Node *root, SceneManager *assets, Vec2 scale){
    ModalView::init(root, assets, scale, WIN_SPLASH);
    float cscale = Director::getInstance()->getContentScaleFactor();
    
    _toNextLevelButton = Button::create();
    _toNextLevelButton->retain();
    
    _toNextLevelButton->setScale(MODAL_MAIN_SCALE * BUTTON_SCALE * cscale);
    initButton(_toNextLevelButton, BUTTON_FONT_SIZE, NEXT_LEVEL_TEXT);
    
    _toNextLevelButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            _transferToNextLevel = true;
        }
    });
    
    _resetButton = Button::create();
    _resetButton->retain();
    
    _resetButton->setScale(MODAL_MAIN_SCALE * BUTTON_SCALE * cscale);
    initButton(_resetButton, BUTTON_FONT_SIZE, RESET_TEXT);
    
    _resetButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            _transferToReset = true;
        }
    });
    
    _toLevelSelectButton = Button::create();
    _toLevelSelectButton->retain();
    
    _toLevelSelectButton->setScale(MODAL_MAIN_SCALE * BUTTON_SCALE * cscale);
    initButton(_toLevelSelectButton, BUTTON_FONT_SIZE, LEVEL_SELECT_TEXT);
    
    _toLevelSelectButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            _transferToLevelSelect = true;
        }
    });

    
    Texture2D* image = assets->get<Texture2D>(WIN_SPLASH_WILL);
    _willAnimation = AnimationNode::create(image, 1, HAPPY_WILL_FRAME_COUNT, HAPPY_WILL_FRAME_COUNT);
    _willAnimation->setAnchorPoint(Vec2(0, 0));
    _willAnimation->setScale(WILL_SCALE * MODAL_MAIN_SCALE * cscale);
    _willAnimation->retain();
    
    string texIDs[] = {WIN_SPLASH_CHILD_1, WIN_SPLASH_CHILD_2, WIN_SPLASH_CHILD_3, WIN_SPLASH_CHILD_4};
    
    for (int i = 0; i < KID_COUNT; i++) {
        image = assets->get<Texture2D>(texIDs[i]);
        _kidAnimations[i] = AnimationNode::create(image, 1, HAPPY_KID_FRAME_COUNT, HAPPY_KID_FRAME_COUNT);
        _kidAnimations[i]->setAnchorPoint(Vec2(0, 0));
        _kidAnimations[i]->setScale(KID_SCALE * MODAL_MAIN_SCALE * cscale);
        _kidAnimations[i]->retain();
    }
    
    _willFrame = 0;
    for (int i = 0; i < KID_COUNT; i++) {
        _kidFrames[i] = 0;
    }
}

void WinView::position() {
    ModalView::position();
    float cscale = Director::getInstance()->getContentScaleFactor();
    
    Vec2 center = Vec2(_root->getContentSize().width/2.0f, _root->getContentSize().height/2.0f);
    
    _toLevelSelectButton->setPosition(Vec2(center.x + _toLevelSelectButton->getContentSize().width*MODAL_MAIN_SCALE*cscale/4,
                                           center.y + VERTICAL_MARGIN - 20));
    
    _resetButton->setPosition(Vec2(center.x + _resetButton->getContentSize().width*MODAL_MAIN_SCALE*cscale/4,
                                   _toLevelSelectButton->getPositionY() + _resetButton->getContentSize().height*MODAL_MAIN_SCALE*cscale * 0.75f));
    
    _toNextLevelButton->setPosition(Vec2(center.x + _toNextLevelButton->getContentSize().width*MODAL_MAIN_SCALE*cscale/4,
                                   _resetButton->getPositionY() + _toNextLevelButton->getContentSize().height*MODAL_MAIN_SCALE*cscale * 0.75f));
    
    _willAnimation->setPosition(center.x - _willAnimation->getContentSize().width*MODAL_MAIN_SCALE*cscale/1.25f, center.y + VERTICAL_MARGIN + 20);
    
    for (int i = 0; i < KID_COUNT; i++) {
        if (_kidAnimations[i] != nullptr) {
            _kidAnimations[i]->setPosition(center.x - HORIZONTAL_MARGIN - _willAnimation->getContentSize().width*MODAL_MAIN_SCALE*cscale/2.5f + _kidAnimations[i]->getContentSize().width*cscale*i * KID_SPACING, center.y+VERTICAL_MARGIN);
        }
    }
    
}

void WinView::update(float dt) {
    _willFrame += 0.4f;
    _willAnimation->setFrame((int)_willFrame % HAPPY_WILL_FRAME_COUNT);
    
    for(int i = 0; i < KID_COUNT; i++) {
        if (_kidAnimations[i] != nullptr) {
            _kidFrames[i] += 0.4f;
            _kidAnimations[i]->setFrame( ((int)_kidFrames[i] + 2*i) % HAPPY_KID_FRAME_COUNT);
        }
    }
}

void WinView::dispose() {
    ModalView::dispose();
    
    if (_toLevelSelectButton != nullptr) {
        _toLevelSelectButton->release();
        _toLevelSelectButton = nullptr;
    }
    
    if (_resetButton != nullptr) {
        _resetButton->release();
        _resetButton = nullptr;
    }
    
    
    if (_toNextLevelButton != nullptr) {
        _toNextLevelButton->release();
        _toNextLevelButton = nullptr;
    }
    
    if (_willAnimation != nullptr) {
        _willAnimation->release();
        _willAnimation = nullptr;
    }
    
    for(int i = 0; i <KID_COUNT; i++) {
        if (_kidAnimations[i] != nullptr) {
            _kidAnimations[i]->release();
        }
        _kidAnimations[i] = nullptr;
    }
}
