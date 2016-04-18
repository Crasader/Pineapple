//
//  LoseView.cpp
//  Pineapple
//
//  Created by Michael Patashnik on 4/18/16.
//
//

#include "ModalView.h"

#define BUTTON_SCALE                0.85f


const string ModalView::BUTTON_FILES[NUM_BUTTONS_MODAL*2] = {"textures/buttons/restart.png", "textures/buttons/restart_inverse.png", "textures/buttons/level_select.png","textures/buttons/level_select_inverse.png"};


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
    
    _resetButton = Button::create();
    _resetButton->setScale(BUTTON_SCALE);
    _resetButton->retain();
    _resetButton->loadTextureNormal(BUTTON_FILES[0]);
    _resetButton->loadTexturePressed(BUTTON_FILES[1]);
    _resetButton->setAnchorPoint(Vec2(0,0));
    
    _resetButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            _transferToReset = true;
        }
    });
    
    
    _toLevelSelectButton = Button::create();
    _toLevelSelectButton->setScale(BUTTON_SCALE);
    _toLevelSelectButton->retain();
    _toLevelSelectButton->loadTextureNormal(BUTTON_FILES[2]);
    _toLevelSelectButton->loadTexturePressed(BUTTON_FILES[3]);
    _toLevelSelectButton->setAnchorPoint(Vec2(0,0));
    
    _toLevelSelectButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            _transferToLevelSelect = true;
        }
    });
    
    _transferToReset = false;
    _transferToLevelSelect = false;

}

void ModalView::position() {
    Vec2 center = Vec2(_root->getContentSize().width/2.0f, _root->getContentSize().height/2.0f);
    _backgroundOverlay->setPosition(center);
    _splashImage->setPosition(center);
    
    _resetButton->setPosition(Vec2(center.x + _resetButton->getContentSize().width/4, center.y + VERTICAL_MARGIN));
    _toLevelSelectButton->setPosition(Vec2(center.x + _resetButton->getContentSize().width/4,
                                           center.y + VERTICAL_MARGIN + _resetButton->getContentSize().height));
    
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
    
    if (_toLevelSelectButton != nullptr) {
        _toLevelSelectButton->release();
        _toLevelSelectButton = nullptr;
    }
    
    if (_resetButton != nullptr) {
        _resetButton->release();
        _resetButton = nullptr;
    }
}

