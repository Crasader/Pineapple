//
//  TutorialView.cpp
//  Pineapple
//
//  Created by Michael Patashnik on 5/1/16.
//
//

#include "TutorialView.h"


void TutorialView::init(Node *root, SceneManager *assets, Vec2 scale) {
    ModalView::init(root, assets, scale, ""); //TODO - add texture id here
    
    _frameCount = 0;
}

void TutorialView::position() {
    ModalView::position();
    
}

void TutorialView::dispose() {
    ModalView::dispose();
    for(auto it = _animations.begin(); it != _animations.end(); ++it) {
        TutorialAnimationTuple tuple = *it;
        tuple.dispose();
    }
    _animations.clear();
}

void TutorialView::update(float dt) {
    _frameCount += 0.2;
    for(auto it = _animations.begin(); it != _animations.end(); ++it) {
        AnimationNode* node = (*it).getNode();
        node->setFrame((int)_frameCount % node->getSize());
    }
}