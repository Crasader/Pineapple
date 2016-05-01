//
//  TutorialView.cpp
//  Pineapple
//
//  Created by Michael Patashnik on 5/1/16.
//
//

#include "TutorialView.h"

#define TUTORIAL_BUTTON_SCALE           1.2f
#define TUTORIAL_BUTTON_FONT            28
#define TUTORIAL_BUTTON_DISMISS_TEXT    "GOT IT!"
#define TUTORIAL_BUTTON_Y_DIFF          -180

#define TUTORIAL_MOVE_ID    0
#define TUTORIAL_JUMP_ID    1
#define TUTORIAL_SHRINK_ID  2
#define TUTORIAL_GROW_ID    3
#define TUTORIAL_SWITCH_ID  4

#define TUTORIAL_MESSAGE_SCALE      0.2f
#define TUTORIAL_MESSAGE_MOVEMENT   Vec2(0,0.16f)

void createMoveTutorial(TutorialView* t) {
    float pointerFrames = 8;
    
    t->addAnimation(TutorialAnimationTuple::create(TUTORIAL_MOVE_MESSAGE,   1, TUTORIAL_MESSAGE_MOVEMENT, TUTORIAL_MESSAGE_SCALE));
    t->addAnimation(TutorialAnimationTuple::create(TUTORIAL_POINTER_IMAGE,  pointerFrames, Vec2(-1.6f,0), 0.75f));
    t->addAnimation(TutorialAnimationTuple::create(TUTORIAL_POINTER_IMAGE,  pointerFrames, Vec2(1.6f,0), 0.75f));
}

void createJumpTutorial(TutorialView* t) {
    
}

void createGrowTutorial(TutorialView* t) {
    
}

void createShrinkTutorial(TutorialView* t) {
    
}

void createSwitchTutorial(TutorialView* t) {
    
}

void TutorialView::addAnimation(TutorialAnimationTuple *t) {
    _animations.push_back(t);
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
    t->_dismissed = false;
    
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
    _dismissButton->setAnchorPoint(Vec2(0.5, 0.5));
    
    _dismissButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            _dismissed = true;
        }
    });
}



void TutorialView::position() {
    ModalView::position();
    
    Vec2 center = Vec2(_root->getContentSize().width/2.0f, _root->getContentSize().height/2.0f);
    
    for(auto it = _animations.begin(); it != _animations.end(); ++it) {
        TutorialAnimationTuple* tuple = *it;
        
        AnimationNode* node = tuple->getNode();
        
        float dx = tuple->getPosition().x;
        float dy = tuple->getPosition().y;
        
        float w = node->getContentSize().width;
        float h = node->getContentSize().height;
        
        node->setAnchorPoint(Vec2(0.5, 0.5));
        node->setScale(tuple->getScale());
        node->setPosition(center + Vec2(w*dx, h*dy));
    }
    
    if (_dismissButton != nullptr) {
        _dismissButton->setPosition(center + Vec2(0, TUTORIAL_BUTTON_Y_DIFF));
    }
}

void TutorialView::dispose() {
    ModalView::dispose();
    for(auto it = _animations.begin(); it != _animations.end(); ++it) {
        TutorialAnimationTuple* tuple = *it;
        tuple->dispose();
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
        AnimationNode* node = (*it)->getNode();
        node->setFrame((int)_frameCount % node->getSize());
    }
}