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
    /** Button that causes a move to the next level */
    Button* _toNextLevelButton;
    
    /** Button that causes the level to reset */
    Button* _resetButton;
    
    /** Button that goes to level select */
    Button* _toLevelSelectButton;
    
    /** The animation for will.*/
    AnimationNode* _willAnimation;
    
    /** The animations for the kids. 
      * May contain null entries if that kid is not present */
    AnimationNode* _kidAnimations[KID_COUNT];
    
    /** The current frame of will */
    float _willFrame;
    
    /** The current frame for each kid. */
    float _kidFrames[KID_COUNT];
    
public:
    static WinView* create(Node* root, SceneManager* assets, Vec2 scale);
    
    void position();
    
    void dispose();
    
    void update(float dt);
    
private:
    void init(Node* root, SceneManager* assets, Vec2 scale);

};

#endif /* winView_h */
