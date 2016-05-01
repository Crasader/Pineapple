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

#define TUTORIAL_GRADIENT_Z         0
#define TUTORIAL_IMAGE_Z            1
#define TUTORIAL_BUTTON_Z           2

class TutorialView : public ModalView {
protected:
    /** The animations drawn on this tutorial view */
    vector<TutorialAnimationTuple*> _animations;
    
    /** The frame count for the animations */
    float _frameCount;
    
    /** Button that dismisses the tutorial view */
    Button* _dismissButton;
    
    /** The x coordinate to pass for this tutorial view to become visible */
    float _triggerX;
    
    /** True once this is dismissed */
    bool _dismissed;
    
public:
    void addAnimation(TutorialAnimationTuple* t);
    
    float getTriggerX() { return _triggerX; }
    
    void update(float dt);
    
    void position();
    
    void dispose();
    
    bool isDismissed() { return _dismissed; }
    
    vector<TutorialAnimationTuple*> getAnimations() { return _animations; }
    
    static TutorialView* create(int tutorialID, float triggerX);
    
    void init(Node* root, SceneManager* assets, Vec2 scale);
    
    void addToRoot() {
        _root->addChild(_backgroundOverlay, TUTORIAL_GRADIENT_Z);
        
        for(auto it = _animations.begin(); it != _animations.end(); ++it) {
            TutorialAnimationTuple* t = *it;
            _root->addChild(t->createNode(_assets), TUTORIAL_IMAGE_Z);
        }
        
        _root->addChild(_dismissButton, TUTORIAL_BUTTON_Z);
    }
    
    void clearFromRoot() {
        _root->removeChild(_backgroundOverlay);
        _root->removeChild(_dismissButton);
        
        for(auto it = _animations.begin(); it != _animations.end(); ++it) {
            TutorialAnimationTuple* t = *it;
            _root->removeChild(t->getNode());
        }
    }
    
};

#endif /* TutorialView_hpp */
