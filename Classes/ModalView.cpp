//
//  LoseView.cpp
//  Pineapple
//
//  Created by Michael Patashnik on 4/18/16.
//
//

#include "ModalView.h"

void ModalView::init(Node *root, SceneManager *assets, string splashTexture) {
    _root = root;
    _assets = assets;
    _splashTextureID = splashTexture;
    
    Texture2D* image = assets->get<Texture2D>(PAUSE_SCREEN_OVERLAY);
    _backgroundOverlay = PolygonNode::createWithTexture(image);
    _backgroundOverlay->setAnchorPoint(Vec2(0.5f, 0.5f));
    _backgroundOverlay->setScale(_root->getContentSize().width/image->getContentSize().width,
                                 _root->getContentSize().height/image->getContentSize().height);
    _backgroundOverlay->retain();
    
    image = assets->get<Texture2D>(_splashTextureID);
    _splashImage = PolygonNode::createWithTexture(image);
    _splashImage->setAnchorPoint(Vec2(0.5f, 0.5f));
    _splashImage->retain();
}

void ModalView::position() {
    Vec2 center = Vec2(_root->getContentSize().width/2.0f, _root->getContentSize().height/2.0f);
    _backgroundOverlay->setPosition(center);
    _splashImage->setPosition(center);
}

void ModalView::dispose() {
    if (_backgroundOverlay != nullptr) {
        _backgroundOverlay->release();
        _backgroundOverlay = nullptr;
    }
    
    if (_splashImage != nullptr) {
        _splashImage->release();
        _splashImage = nullptr;
    }
}

