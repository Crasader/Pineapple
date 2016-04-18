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

class WinView : public ModalView {
protected:
    /** The animation for will. Null if the will was the one who died */
    Node* _willAnimation;
    
    /** Button that causes the level to reset */
    Button* _resetButton;
    
    /** Button that goes to level select */
    Button* _toLevelSelectButton;
    
    /** The image of the juice cup */
    Node* _juiceCup;
    
    /** The animation of will crying */
    AnimationNode* _cryingWill;
    
    /** The current frame of will */
    int _willFrame;
    
public:
    static WinView* create(Node* root, SceneManager* assets);
    
    void position();
    
    void dispose();
    
    void update(float dt);
    
private:
    void init(Node* root, SceneManager* assets);

};

#endif /* winView_h */
