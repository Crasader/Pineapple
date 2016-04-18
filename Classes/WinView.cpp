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


#define WILL_SCALE      1.0f
#define KID_SCALE       0.25f
#define HAPPY_KID_FRAME_COUNT   14
#define HAPPY_WILL_FRAME_COUNT  28

using namespace std;

WinView* WinView::create(Node* root, SceneManager *assets) {
    WinView* view = new (std::nothrow) WinView();
    
    view->init(root, assets);
    
    root->addChild(view->_backgroundOverlay, GRADIENT_Z);
    root->addChild(view->_splashImage, SPLASH_Z);
    root->addChild(view->_willAnimation, PINEAPPLE_Z);
    
    for(int i = 0; i < KID_COUNT; i++) {
        if (view->_kidAnimations[i] != nullptr) {
            root->addChild(view->_kidAnimations[i],PINEAPPLE_Z);
        }
    }
    
    root->addChild(view->_resetButton, BUTTON_Z);
    root->addChild(view->_toLevelSelectButton, BUTTON_Z);
    
    return view;
}

void WinView::init(Node *root, SceneManager *assets){
    ModalView::init(root, assets, WIN_SPLASH);
    
    Texture2D* image = assets->get<Texture2D>(WIN_SPLASH_WILL);
    _willAnimation = AnimationNode::create(image, 1, HAPPY_WILL_FRAME_COUNT, HAPPY_WILL_FRAME_COUNT);
    _willAnimation->setAnchorPoint(Vec2(0, 0));
    _willAnimation->setScale(WILL_SCALE);
    _willAnimation->retain();
    
    string texIDs[] = {WIN_SPLASH_CHILD_1, WIN_SPLASH_CHILD_2, WIN_SPLASH_CHILD_3, WIN_SPLASH_CHILD_4};
    
    for (int i = 0; i < KID_COUNT; i++) {
        image = assets->get<Texture2D>(texIDs[i]);
        _kidAnimations[i] = AnimationNode::create(image, 1, HAPPY_KID_FRAME_COUNT, HAPPY_KID_FRAME_COUNT);
        _kidAnimations[i]->setAnchorPoint(Vec2(0, 0));
        _kidAnimations[i]->setScale(KID_SCALE);
        _kidAnimations[i]->retain();
    }
    
    _willFrame = 0;
    for (int i = 0; i < KID_COUNT; i++) {
        _kidFrames[i] = 0;
    }
}

void WinView::position() {
    ModalView::position();
    
    Vec2 center = Vec2(_root->getContentSize().width/2.0f, _root->getContentSize().height/2.0f);
    
    _willAnimation->setPosition(center.x - _willAnimation->getContentSize().width*4/5, center.y + VERTICAL_MARGIN + 20);
    
    for (int i = 0; i < KID_COUNT; i++) {
        if (_kidAnimations[i] != nullptr) {
            _kidAnimations[i]->setPosition(center.x - HORIZONTAL_MARGIN*1.75f + _kidAnimations[i]->getContentSize().width*i/7, center.y+VERTICAL_MARGIN);
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
