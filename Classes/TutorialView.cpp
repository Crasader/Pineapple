//
//  TutorialView.cpp
//  Pineapple
//
//  Created by Michael Patashnik on 5/1/16.
//
//

#include "TutorialView.h"

#define TUTORIAL_BUTTON_SCALE           1.0f
#define TUTORIAL_BUTTON_FONT            30
#define TUTORIAL_BUTTON_DISMISS_TEXT    "OK"

#define TUTORIAL_GRADIENT_Z      0
#define TUTORIAL_BUTTON_Z        2

#define TUTORIAL_MOVE_ID    0
#define TUTORIAL_JUMP_ID    1
#define TUTORIAL_SHRINK_ID  2
#define TUTORIAL_GROW_ID    3
#define TUTORIAL_SWITCH_ID  4

void createMoveTutorial(TutorialView* t) {
    
}

void createJumpTutorial(TutorialView* t) {
    
}

void createGrowTutorial(TutorialView* t) {
    
}

void createShrinkTutorial(TutorialView* t) {
    
}

void createSwitchTutorial(TutorialView* t) {
    
}

TutorialView* TutorialView::create(int id, float triggerX) {
    TutorialView* t = new (std::nothrow) TutorialView();
    
    switch (id) {
        case TUTORIAL_MOVE_ID:
            createMoveTutorial(t);
            break;
        case TUTORIAL_JUMP_ID:
            createJumpTutorial(t);
            break;
        case TUTORIAL_GROW_ID:
            createGrowTutorial(t);
            break;
        case TUTORIAL_SHRINK_ID:
            createShrinkTutorial(t);
            break;
        case TUTORIAL_SWITCH_ID:
            createSwitchTutorial(t);
            break;
        default:
            CC_ASSERT(false);
            break;
    }
    
    t->_triggerX = triggerX;
    
    return t;
}

void TutorialView::init(Node *root, SceneManager *assets, Vec2 scale) {
    ModalView::init(root, assets, scale, ""); //TODO - add texture id here
    
    float cscale = Director::getInstance()->getContentScaleFactor();
    
    _frameCount = 0;
    
    _dismissButton = Button::create();
    _dismissButton->retain();
    _dismissButton->setScale(MODAL_MAIN_SCALE * TUTORIAL_BUTTON_SCALE * cscale);
    initButton(_dismissButton, TUTORIAL_BUTTON_FONT, TUTORIAL_BUTTON_DISMISS_TEXT);
    
    _dismissButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            _dismiss = true;
        }
    });
    
    root->addChild(_backgroundOverlay, TUTORIAL_GRADIENT_Z);
    root->addChild(_dismissButton, TUTORIAL_BUTTON_Z);
}

void TutorialView::position() {
    ModalView::position();
    
    Vec2 center = Vec2(_root->getContentSize().width/2.0f, _root->getContentSize().height/2.0f);
    
    if (_dismissButton != nullptr) {
        _dismissButton->setPosition(center);
    }
}

void TutorialView::dispose() {
    ModalView::dispose();
    for(auto it = _animations.begin(); it != _animations.end(); ++it) {
        TutorialAnimationTuple tuple = *it;
        tuple.dispose();
    }
    _animations.clear();
    
    if (_dismissButton != nullptr) {
        _dismissButton->release();
        _dismissButton = nullptr;
    }
}

void TutorialView::update(float dt) {
    _frameCount += 0.2;
    for(auto it = _animations.begin(); it != _animations.end(); ++it) {
        AnimationNode* node = (*it).getNode();
        node->setFrame((int)_frameCount % node->getSize());
    }
}