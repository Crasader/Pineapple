//
//  LoseView.cpp
//  Pineapple
//
//  Created by Michael Patashnik on 4/18/16.
//
//

#include "LoseView.h"

#define GRADIENT_Z      0
#define SPLASH_Z        1
#define PINEAPPLE_Z     2
#define BUTTON_Z        3


#define JUICE_SCALE                 0.7f
#define WILL_SCALE                  0.7f
#define CRYING_WILL_FRAME_COUNT     20

LoseView* LoseView::create(Node* root, SceneManager *assets, Vec2 scale) {
    LoseView* view = new (std::nothrow) LoseView();
    
    view->init(root, assets, scale);
    
    root->addChild(view->_backgroundOverlay, GRADIENT_Z);
    root->addChild(view->_splashImage, SPLASH_Z);
    root->addChild(view->_juiceCup, PINEAPPLE_Z);
    root->addChild(view->_cryingWill,PINEAPPLE_Z);
    root->addChild(view->_resetButton,BUTTON_Z);
    root->addChild(view->_toLevelSelectButton,BUTTON_Z);
    
    return view;
}

void LoseView::init(Node *root, SceneManager *assets, Vec2 scale){
    ModalView::init(root, assets, scale, LOSE_SPLASH);
    float cscale = Director::getInstance()->getContentScaleFactor();
    
    Texture2D* image = assets->get<Texture2D>(LOSE_JUICE_CUP);
    _juiceCup = PolygonNode::createWithTexture(image);
    _juiceCup->setAnchorPoint(Vec2(0,0));
    _juiceCup->setScale(JUICE_SCALE * MODAL_MAIN_SCALE * cscale);
    _juiceCup->retain();
    
    image = assets->get<Texture2D>(LOSE_CRYING_WILL);
    _cryingWill = AnimationNode::create(image, 1, CRYING_WILL_FRAME_COUNT, CRYING_WILL_FRAME_COUNT);
    _cryingWill->setAnchorPoint(Vec2(0,0));
    _cryingWill->setScale(WILL_SCALE * MODAL_MAIN_SCALE * cscale);
    _cryingWill->retain();
    
    _willFrame = 0;
}

void LoseView::position() {
    ModalView::position();
    float cscale = Director::getInstance()->getContentScaleFactor();
    
    Vec2 center = Vec2(_root->getContentSize().width/2.0f, _root->getContentSize().height/2.0f);
    
    _juiceCup->setPosition(center.x - HORIZONTAL_MARGIN - _juiceCup->getContentSize().width*MODAL_MAIN_SCALE*cscale/2,
                           center.y + VERTICAL_MARGIN);
    _cryingWill->setPosition(center.x - _cryingWill->getContentSize().width*MODAL_MAIN_SCALE*cscale/2,
                             center.y + VERTICAL_MARGIN);
    
}

void LoseView::update(float dt) {
    _willFrame+= 0.2;
    _cryingWill->setFrame((int)_willFrame % CRYING_WILL_FRAME_COUNT);
}

void LoseView::dispose() {
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


