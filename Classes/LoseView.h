//
//  LoseView.hpp
//  Pineapple
//
//  Created by Michael Patashnik on 4/18/16.
//
//

#ifndef __LOSE_VIEW_H__
#define __LOSE_VIEW_H__

#include <stdio.h>
#include <cornell.h>
#include "ModalView.h"

class LoseView : public ModalView {
protected:
    /** The animation for will. Null if the will was the one who died */
    Node* _willAnimation;
    
    /** The image of the juice cup */
    Node* _juiceCup;
    
    /** The animation of will crying */
    AnimationNode* _cryingWill;
    
    /** The current frame of will */
    float _willFrame;

    
public:
    static LoseView* create(Node* root, SceneManager* assets, Vec2 scale);
    
    void position();
    
    void dispose();
    
    void update(float dt);
    
private:
    void init(Node* root, SceneManager* assets, Vec2 scale);

};

#endif /* LoseView_hpp */
