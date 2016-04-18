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


#define JUICE_SCALE     0.5f
#define WILL_SCALE      0.5f
#define CRYING_WILL_FRAME_COUNT 20

WinView* WinView::create(Node* root, SceneManager *assets) {
    WinView* view = new (std::nothrow) WinView();
    
    view->init(root, assets);
    
    root->addChild(view->_backgroundOverlay, GRADIENT_Z);
    root->addChild(view->_splashImage, SPLASH_Z);
    root->addChild(view->_juiceCup, PINEAPPLE_Z);
    root->addChild(view->_cryingWill,PINEAPPLE_Z);
    
    return view;
}

void WinView::init(Node *root, SceneManager *assets){
    ModalView::init(root, assets, LOSE_SPLASH);
    
    
    Texture2D* image = assets->get<Texture2D>(LOSE_JUICE_CUP);
    _juiceCup = PolygonNode::createWithTexture(image);
    _juiceCup->setAnchorPoint(Vec2(0.5f, 0.5f));
    _juiceCup->setScale(JUICE_SCALE);
    _juiceCup->retain();
    
    image = assets->get<Texture2D>(LOSE_CRYING_WILL);
    _cryingWill = AnimationNode::create(image, 1, CRYING_WILL_FRAME_COUNT, CRYING_WILL_FRAME_COUNT);
    _cryingWill->setAnchorPoint(Vec2(0.5f, 0.5f));
    _cryingWill->setScale(WILL_SCALE);
    _cryingWill->retain();
    
    _willFrame = 0;
}

void WinView::position() {
    ModalView::position();
    
    Vec2 center = Vec2(_root->getContentSize().width/2.0f, _root->getContentSize().height/2.0f);
    
    _juiceCup->setPosition(center.x - _juiceCup->getContentSize().width, center.y);
    _cryingWill->setPosition(center.x + _cryingWill->getContentSize().width, center.y);
    
}

void WinView::update(float dt) {
    _willFrame++;
    _cryingWill->setFrame(_willFrame % CRYING_WILL_FRAME_COUNT);
}

void WinView::dispose() {
    ModalView::dispose();
    
    if (_juiceCup != nullptr) {
        _juiceCup->release();
        _juiceCup = nullptr;
    }
    
    if (_cryingWill != nullptr) {
        _cryingWill->release();
        _cryingWill = nullptr;
    }
}


