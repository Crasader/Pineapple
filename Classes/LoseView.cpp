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

LoseView* LoseView::create(Node* root, SceneManager *assets) {
    LoseView* view = new (std::nothrow) LoseView();
    
    view->init(root, assets);
    
    root->addChild(view->_backgroundOverlay, GRADIENT_Z);
    root->addChild(view->_splashImage, SPLASH_Z);
    
    return view;
}

void LoseView::init(Node *root, SceneManager *assets) {
    _root = root;
    
    Texture2D* image = assets->get<Texture2D>(PAUSE_SCREEN_OVERLAY);
    _backgroundOverlay = PolygonNode::createWithTexture(image);
    _backgroundOverlay->setAnchorPoint(Vec2(0.5f, 0.5f));
    _backgroundOverlay->retain();
    
    image = assets->get<Texture2D>(LOSE_SPLASH);
    _splashImage = PolygonNode::createWithTexture(image);
    _splashImage->setAnchorPoint(Vec2(0.5f, 0.5f));
    _splashImage->retain();
}

void LoseView::position() {
    Vec2 center = Vec2(_root->getContentSize().width/2.0f, _root->getContentSize().height/2.0f);
    _backgroundOverlay->setScale(_root->getContentSize().width/_backgroundOverlay->getContentSize().width,
                                 _root->getContentSize().height/_backgroundOverlay->getContentSize().height);
    _splashImage->setPosition(center);
}

void LoseView::dispose() {
    if (_backgroundOverlay != nullptr) {
        _backgroundOverlay->release();
        _backgroundOverlay = nullptr;
    }
    
    if (_splashImage != nullptr) {
        _splashImage->release();
        _splashImage = nullptr;
    }
}

