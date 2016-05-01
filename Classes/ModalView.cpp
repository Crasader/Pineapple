//
//  LoseView.cpp
//  Pineapple
//
//  Created by Michael Patashnik on 4/18/16.
//
//

#include "ModalView.h"

void ModalView::init(Node *root, SceneManager *assets, Vec2 scale, string splashTexture) {
    _root = root;
    _assets = assets;
    _splashTextureID = splashTexture;
    _scale = scale;
    
    _dismiss = false;
    _transferToReset = false;
    _transferToNextLevel = false;
    _transferToLevelSelect = false;
    
    float cscale = Director::getInstance()->getContentScaleFactor();
    
    Texture2D* image = assets->get<Texture2D>(PAUSE_SCREEN_OVERLAY);
    _backgroundOverlay = PolygonNode::createWithTexture(image);
    _backgroundOverlay->setAnchorPoint(Vec2(0.5f, 0.5f));
    _backgroundOverlay->setScale(_root->getContentSize().width/image->getContentSize().width,
                                 _root->getContentSize().height/image->getContentSize().height);
    _backgroundOverlay->retain();
    
    if (splashTexture != "") {
        image = assets->get<Texture2D>(_splashTextureID);
        _splashImage = PolygonNode::createWithTexture(image);
        _splashImage->setAnchorPoint(Vec2(0.5f, 0.5f));
        _splashImage->retain();
        _splashImage->setScale(MODAL_MAIN_SCALE * cscale);
    }

}

void ModalView::initButton(Button *button, int fontSize, string text) {
    button->loadTextureNormal(BUTTON_UNPRESSED_TEXTURE_LOCATION);
    button->loadTexturePressed(BUTTON_PRESSED_TEXTURE_LOCATION);
    
    button->setTitleFontName(ELECTRIC_CIRCUS_FONT_LOCATION);
    button->setTitleColor(Color3B::WHITE);
    button->setTitleAlignment(TextHAlignment::CENTER);
    button->setTitleFontSize(fontSize);
    button->setTitleText(text);
    
    button->setAnchorPoint(Vec2(0,0));
    
    button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::BEGAN) {
            ((Button*)sender)->setTitleColor(Color3B::BLACK);
        } else if (type == ui::Widget::TouchEventType::CANCELED) {
            ((Button*)sender)->setTitleColor(Color3B::WHITE);
        } else if (type == ui::Widget::TouchEventType::ENDED) {
            ((Button*)sender)->setTitleColor(Color3B::WHITE);
        }
    });
}

void ModalView::position() {
    Vec2 center = Vec2(_root->getContentSize().width/2.0f, _root->getContentSize().height/2.0f);
    
    if (_backgroundOverlay != nullptr) {
        _backgroundOverlay->setPosition(center);
    }
    if (_splashImage != nullptr) {
        _splashImage->setPosition(center);
    }
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

