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

class TutorialView : public ModalView {
protected:
    /** The animations drawn on this tutorial view */
    vector<TutorialAnimationTuple> _animations;
    
    /** The frame count for the animations */
    float _frameCount;
    
    /** Button that dismisses the tutorial view */
    Button* _dismissButton;
    
    /** The x coordinate to pass for this tutorial view to become visible */
    float _triggerX;
    
public:
    float getTriggerX() { return _triggerX; }
    
    void update(float dt);
    
    void position();
    
    void dispose();
    
    vector<TutorialAnimationTuple> getAnimations() { return _animations; }
    
    static TutorialView* create(int tutorialID, float triggerX);
    
    void init(Node* root, SceneManager* assets, Vec2 scale);
    
};

#endif /* TutorialView_hpp */
