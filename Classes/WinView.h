//
//  LoseView.hpp
//  Pineapple
//
//  Created by Michael Patashnik on 4/18/16.
//
//

#ifndef __WIN_VIEW_H__
#define __WIN_VIEW_H__

#include <stdio.h>
#include <cornell.h>
#include "ModalView.h"
#include "Const.h"

class WinView : public ModalView {
protected:
    /** The animation for will.*/
    AnimationNode* _willAnimation;
    
    AnimationNode* _kidAnimations[KID_COUNT];
    
    /** The current frame of will */
    float _willFrame;
    
    float _kidFrames[KID_COUNT];
    
public:
    static WinView* create(Node* root, SceneManager* assets);
    
    void position();
    
    void dispose();
    
    void update(float dt);
    
private:
    void init(Node* root, SceneManager* assets);

};

#endif /* winView_h */
