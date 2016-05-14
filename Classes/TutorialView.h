//
//  TutorialView.hpp
//  Pineapple
//
//  Created by Michael Patashnik on 5/1/16.
//
//

#ifndef TutorialView_h
#define TutorialView_h

#include <cornell.h>
#include "ModalView.h"
#include "TutorialAnimationTuple.h"

using namespace std;

#define TUTORIAL_IMAGE_Z            1

class TutorialView : public ModalView {
protected:
    /** The animations drawn on this tutorial view */
    vector<TutorialAnimationTuple*> _animations;
    
    /** The coordinates to pass for this tutorial view to become visible */
    Vec2 _loc;
    
public:
    void addAnimation(TutorialAnimationTuple* t);
    
    Vec2 getPosition() {
        return _loc;
    }
    
    void update(float dt);
    
    void position();
    
    void dispose();
    
    vector<TutorialAnimationTuple*> getAnimations() { return _animations; }
    
    static TutorialView* create(int tutorialID, Vec2 position);
    
    void init(Node* root, SceneManager* assets, Vec2 scale);
    
    void addToRoot() {
        for(auto it = _animations.begin(); it != _animations.end(); ++it) {
            TutorialAnimationTuple* t = *it;
            _root->addChild(t->createNode(_assets), TUTORIAL_IMAGE_Z);
        }
    }
    
    void clearFromRoot() {
        for(auto it = _animations.begin(); it != _animations.end(); ++it) {
            TutorialAnimationTuple* t = *it;
            _root->removeChild(t->getNode());
        }
    }
    
};

#endif /* TutorialView_hpp */
