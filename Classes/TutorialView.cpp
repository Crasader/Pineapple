//
//  TutorialView.cpp
//  Pineapple
//
//  Created by Michael Patashnik on 5/1/16.
//
//

#include "TutorialView.h"

#define TUTORIAL_BUTTON_SCALE           0.4f
#define TUTORIAL_BUTTON_FONT            28
#define TUTORIAL_BUTTON_OFFSET          Vec2(0.85, -0.6)

#define TUTORIAL_MOVE_ID        0
#define TUTORIAL_JUMP_ID        1
#define TUTORIAL_SHRINK_ID      2
#define TUTORIAL_GROW_ID        3
#define TUTORIAL_SWITCH_ID      4
#define TUTORIAL_JUMP_SIZE_ID   5
#define TUTORIAL_SMASH_ID       6
#define TUTORIAL_BLENDER_ID     7
#define TUTORIAL_FRIDGE_ID      8
#define TUTORIAL_KNIVES_ID      9

#define TUTORIAL_MESSAGE_SCALE      0.2f
#define TUTORIAL_MESSAGE_OFFSET   Vec2(0,0.18f)

#define TUTORIAL_POINTER_FRAMES 8
#define TUTORIAL_POINTER_SCALE  0.75f
#define TUTORIAL_POINTER_FRAMERATE  0.2f

#define TUTORIAL_GROW_SHRINK_FRAMES     3
#define TUTORIAL_GROW_SHRINK_SCALE      0.77f
#define TUTORIAL_GROW_SHRINK_FRAMERATE  0.075f

#define TUTORIAL_SWITCH_DOOR_SCALE      1.0f
#define TUTORIAL_SWITCH_DOOR_FRAMES     14
#define TUTORIAL_SWITCH_DOOR_FRAMERATE  0.15f

#define TUTORIAL_PINEAPPLE_SCALE        0.7f
#define TUTORIAL_PINEAPPLE_TOTAL_FRAMES 26
#define TUTORIAL_KID_SCALE              0.4f
#define TUTORIAL_KID_PINEAPPLE_FRAMES   12
#define TUTORIAL_KID_PINEAPPLE_SPEED    0.45f

void createBlenderTutorial(TutorialView* t) {
    t->addAnimation(TutorialAnimationTuple::create(TUTORIAL_BLENDER_MESSAGE, TUTORIAL_MESSAGE_OFFSET, TUTORIAL_MESSAGE_SCALE));
    
    t->addAnimation(TutorialAnimationTuple::create(BLENDER_TEXTURE, 2, 10, Vec2(-1,-0.2), 1.0f, 0.5));
    
    
    t->addAnimation(TutorialAnimationTuple::create(PINEAPPLE_TEXTURE, TUTORIAL_KID_PINEAPPLE_FRAMES, TUTORIAL_PINEAPPLE_TOTAL_FRAMES,
                                                   Vec2(1.7f,-0.1), TUTORIAL_PINEAPPLE_SCALE, TUTORIAL_KID_PINEAPPLE_SPEED));
    
    t->addAnimation(TutorialAnimationTuple::create(KID_TEXTURE_1, TUTORIAL_KID_PINEAPPLE_FRAMES, Vec2(-0.2f,-0.2),
                                                   TUTORIAL_KID_SCALE, TUTORIAL_KID_PINEAPPLE_SPEED));
    t->addAnimation(TutorialAnimationTuple::create(KID_TEXTURE_2, TUTORIAL_KID_PINEAPPLE_FRAMES, Vec2(0.15f,-0.2),
                                                   TUTORIAL_KID_SCALE, TUTORIAL_KID_PINEAPPLE_SPEED));
    t->addAnimation(TutorialAnimationTuple::create(KID_TEXTURE_3, TUTORIAL_KID_PINEAPPLE_FRAMES, Vec2(0.5f,-0.15),
                                                   TUTORIAL_KID_SCALE, TUTORIAL_KID_PINEAPPLE_SPEED));
    t->addAnimation(TutorialAnimationTuple::create(KID_TEXTURE_4, TUTORIAL_KID_PINEAPPLE_FRAMES, Vec2(1.0f,-0.22),
                                                   TUTORIAL_KID_SCALE, TUTORIAL_KID_PINEAPPLE_SPEED));
}

void createFridgeTutorial(TutorialView *t) {
    t->addAnimation(TutorialAnimationTuple::create(TUTORIAL_FRIDGE_MESSAGE, TUTORIAL_MESSAGE_OFFSET, TUTORIAL_MESSAGE_SCALE));

    t->addAnimation(TutorialAnimationTuple::create(PINEAPPLE_TEXTURE, TUTORIAL_KID_PINEAPPLE_FRAMES, TUTORIAL_PINEAPPLE_TOTAL_FRAMES,
                                                   Vec2(1.7f,-0.1), TUTORIAL_PINEAPPLE_SCALE, TUTORIAL_KID_PINEAPPLE_SPEED));
    
    t->addAnimation(TutorialAnimationTuple::create(KID_TEXTURE_1, TUTORIAL_KID_PINEAPPLE_FRAMES, Vec2(-0.2f,-0.2),
                                                   TUTORIAL_KID_SCALE, TUTORIAL_KID_PINEAPPLE_SPEED));
    t->addAnimation(TutorialAnimationTuple::create(KID_TEXTURE_2, TUTORIAL_KID_PINEAPPLE_FRAMES, Vec2(0.15f,-0.2),
                                                   TUTORIAL_KID_SCALE, TUTORIAL_KID_PINEAPPLE_SPEED));
    t->addAnimation(TutorialAnimationTuple::create(KID_TEXTURE_3, TUTORIAL_KID_PINEAPPLE_FRAMES, Vec2(0.5f,-0.15),
                                                   TUTORIAL_KID_SCALE, TUTORIAL_KID_PINEAPPLE_SPEED));
    t->addAnimation(TutorialAnimationTuple::create(KID_TEXTURE_4, TUTORIAL_KID_PINEAPPLE_FRAMES, Vec2(1.0f,-0.22),
                                                   TUTORIAL_KID_SCALE, TUTORIAL_KID_PINEAPPLE_SPEED));
    
    t->addAnimation(TutorialAnimationTuple::create(GOAL_TEXTURE, 8, 9, Vec2(2.1f,0.0f),0.8f, 0.3));
}

void createKnivesTutorial(TutorialView *t) {
    t->addAnimation(TutorialAnimationTuple::create(TUTORIAL_KNIVES_MESSAGE, TUTORIAL_MESSAGE_OFFSET, TUTORIAL_MESSAGE_SCALE));
    t->addAnimation(TutorialAnimationTuple::create(TUTORIAL_KNIVES_IMAGE, 4, Vec2(0,0), 1.0f, 0.12f));
}

void createMoveTutorial(TutorialView* t) {
    float pointerXDiff = 1.7f;
    
    t->addAnimation(TutorialAnimationTuple::create(TUTORIAL_MOVE_MESSAGE, TUTORIAL_MESSAGE_OFFSET, TUTORIAL_MESSAGE_SCALE));
    t->addAnimation(TutorialAnimationTuple::create(TUTORIAL_POINTER_IMAGE,
                                                   TUTORIAL_POINTER_FRAMES, Vec2(-pointerXDiff,0), TUTORIAL_POINTER_SCALE,
                                                   TUTORIAL_POINTER_FRAMERATE));
    t->addAnimation(TutorialAnimationTuple::create(TUTORIAL_POINTER_IMAGE,
                                                   TUTORIAL_POINTER_FRAMES, Vec2(pointerXDiff,0), TUTORIAL_POINTER_SCALE,
                                                   TUTORIAL_POINTER_FRAMERATE));
}

void createJumpTutorial(TutorialView* t) {
    float pointerYDiff = -0.15f;
    
    t->addAnimation(TutorialAnimationTuple::create(TUTORIAL_JUMP_MESSAGE, TUTORIAL_MESSAGE_OFFSET, TUTORIAL_MESSAGE_SCALE));
    t->addAnimation(TutorialAnimationTuple::create(TUTORIAL_POINTER_IMAGE,
                                                   TUTORIAL_POINTER_FRAMES, Vec2(0,pointerYDiff), TUTORIAL_POINTER_SCALE,
                                                   TUTORIAL_POINTER_FRAMERATE));
}

void createGrowTutorial(TutorialView* t) {
    t->addAnimation(TutorialAnimationTuple::create(TUTORIAL_GROW_MESSAGE, TUTORIAL_MESSAGE_OFFSET, TUTORIAL_MESSAGE_SCALE));
    t->addAnimation(TutorialAnimationTuple::create(TUTORIAL_GROW_IMAGE, TUTORIAL_GROW_SHRINK_FRAMES, Vec2(0,0),
                                                   TUTORIAL_GROW_SHRINK_SCALE,TUTORIAL_GROW_SHRINK_FRAMERATE));
}

void createShrinkTutorial(TutorialView* t) {
    t->addAnimation(TutorialAnimationTuple::create(TUTORIAL_SHRINK_MESSAGE, TUTORIAL_MESSAGE_OFFSET, TUTORIAL_MESSAGE_SCALE));
    t->addAnimation(TutorialAnimationTuple::create(TUTORIAL_SHRINK_IMAGE, TUTORIAL_GROW_SHRINK_FRAMES, Vec2(0,0),
                                                   TUTORIAL_GROW_SHRINK_SCALE,TUTORIAL_GROW_SHRINK_FRAMERATE));
}

void createSmashTutorial(TutorialView* t) {
    float elmYDiff = -0.15f;

    t->addAnimation(TutorialAnimationTuple::create(TUTORIAL_SMASH_MESSAGE, TUTORIAL_MESSAGE_OFFSET, TUTORIAL_MESSAGE_SCALE));
    t->addAnimation(TutorialAnimationTuple::create(TUTORIAL_SMASH_IMAGE, 12, Vec2(0,elmYDiff), 0.6f, 0.15f));

}

void createJumpSizeTutorial(TutorialView* t) {
    float elmYDiff = -0.15f;

    t->addAnimation(TutorialAnimationTuple::create(TUTORIAL_JUMP_SIZE_MESSAGE, TUTORIAL_MESSAGE_OFFSET, TUTORIAL_MESSAGE_SCALE));
    t->addAnimation(TutorialAnimationTuple::create(TUTORIAL_JUMP_IMAGE, 14, Vec2(0,elmYDiff), 0.8f, 0.35f));

}

void createSwitchTutorial(TutorialView* t) {
    float elmXDiff = 0.6f;
    float elmYDiff = -0.2f;
    
    t->addAnimation(TutorialAnimationTuple::create(TUTORIAL_SWITCH_MESSAGE, TUTORIAL_MESSAGE_OFFSET, TUTORIAL_MESSAGE_SCALE));

    t->addAnimation(TutorialAnimationTuple::create(TUTORIAL_SWITCH_IMAGE,
                                                   TUTORIAL_SWITCH_DOOR_FRAMES, Vec2(-elmXDiff,elmYDiff), TUTORIAL_SWITCH_DOOR_SCALE,
                                                   TUTORIAL_SWITCH_DOOR_FRAMERATE));
    t->addAnimation(TutorialAnimationTuple::create(TUTORIAL_DOOR_IMAGE,
                                                   TUTORIAL_SWITCH_DOOR_FRAMES, Vec2(elmXDiff,elmYDiff), TUTORIAL_SWITCH_DOOR_SCALE,
                                                   TUTORIAL_SWITCH_DOOR_FRAMERATE));
    
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
        case TUTORIAL_JUMP_SIZE_ID:
            createJumpSizeTutorial(t);
            break;
        case TUTORIAL_SMASH_ID:
            createSmashTutorial(t);
            break;
        case TUTORIAL_BLENDER_ID:
            createBlenderTutorial(t);
            break;
        case TUTORIAL_FRIDGE_ID:
            createFridgeTutorial(t);
            break;
        case TUTORIAL_KNIVES_ID:
            createKnivesTutorial(t);
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
    ModalView::init(root, assets, scale, "");
    
    float cscale = Director::getInstance()->getContentScaleFactor();
    
    Texture2D* image = assets->get<Texture2D>(PAUSE_SCREEN_OVERLAY);
    _backgroundOverlayTwo = PolygonNode::createWithTexture(image);
    _backgroundOverlayTwo->setAnchorPoint(Vec2(0.5f, 0.5f));
    _backgroundOverlayTwo->setScale(_root->getContentSize().width/image->getContentSize().width,
                                 _root->getContentSize().height/image->getContentSize().height);
    _backgroundOverlayTwo->retain();

    _dismissButton = Button::create();
    _dismissButton->retain();
    _dismissButton->setScale(MODAL_MAIN_SCALE * TUTORIAL_BUTTON_SCALE * cscale);
    initButton(_dismissButton, TUTORIAL_BUTTON_FONT, "");
    _dismissButton->setAnchorPoint(Vec2(0.5, 0.5));
    _dismissButton->loadTextureNormal(TUTORIAL_BUTTON_FILEPATH);
    _dismissButton->loadTexturePressed(TUTORIAL_BUTTON_ON_FILEPATH);
    
    _dismissButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            _dismissed = true;
        }
    });
}

void TutorialView::position() {
    ModalView::position();
    
    Vec2 center = Vec2(_root->getContentSize().width/2.0f, _root->getContentSize().height/2.0f);
    float cscale = Director::getInstance()->getContentScaleFactor();
    
    for(auto it = _animations.begin(); it != _animations.end(); ++it) {
        TutorialAnimationTuple* tuple = *it;
        
        AnimationNode* node = tuple->getNode();
        
        float dx = tuple->getPosition().x * cscale;
        float dy = tuple->getPosition().y * cscale;
        
        float w = node->getContentSize().width;
        float h = node->getContentSize().height;
        
        node->setAnchorPoint(Vec2(0.5, 0.5));
        node->setScale(tuple->getScale() * cscale);
        node->setPosition(center + Vec2(w*dx, h*dy));
    }
    
    if (_backgroundOverlayTwo != nullptr) {
        _backgroundOverlayTwo->setPosition(center);
    }
    
    if (_dismissButton != nullptr) {
        float w = _dismissButton->getContentSize().width;
        float h = _dismissButton->getContentSize().height;
        
        _dismissButton->setPosition(center + Vec2(w * TUTORIAL_BUTTON_OFFSET.x * cscale, h*TUTORIAL_BUTTON_OFFSET.y*cscale));
    }
}

void TutorialView::dispose() {
    ModalView::dispose();
    for(auto it = _animations.begin(); it != _animations.end(); ++it) {
        TutorialAnimationTuple* tuple = *it;
        tuple->dispose();
    }
    _animations.clear();
    
    if (_backgroundOverlayTwo != nullptr) {
        _backgroundOverlayTwo->release();
        _backgroundOverlayTwo = nullptr;
    }
    
    if (_dismissButton != nullptr) {
        _dismissButton->release();
        _dismissButton = nullptr;
    }
}

void TutorialView::update(float dt) {
    for(auto it = _animations.begin(); it != _animations.end(); ++it) {
        TutorialAnimationTuple *t = (*it);
        AnimationNode* node = t->getNode();
        node->setFrame((int)t->incAndGetFrame() % t->getFrameCount());
    }
}