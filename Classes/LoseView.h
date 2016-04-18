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
#include "ModalView.h"

class LoseView : public ModalView {
protected:
    /** The animation for will. Null if the will was the one who died */
    Node* _willAnimation;
    
    /** Button that causes the level to reset */
    Button* _resetButton;
    
    /** Button that goes to level select */
    Button* _toLevelSelectButton;
    
public:
    static LoseView* create(Node* root, SceneManager* assets);
    
private:
    void init(Node* root, SceneManager* assets);

};

#endif /* LoseView_hpp */
