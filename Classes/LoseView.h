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
#include "ui/CocosGUI.h"
#include "Const.h"
#include "Texture.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class LoseView {
protected:
    /** A reference to the root node for this splash */
    Node* _root;
    
    /** The translucent gray overlay */
    Node* _backgroundOverlay;
    
    /** The splash image */
    Node* _splashImage;
    
    /** The animation for will. Null if the will was the one who died */
    Node* _willAnimation;
    
    /** Button that causes the level to reset */
    Button* _resetButton;
    
    /** Button that goes to level select */
    Button* _toLevelSelectButton;
    
public:
    static LoseView* create(Node* rootNode, SceneManager* assets);
    
    void position();
    
    void dispose();
    
private:
    void init(Node* rootNode, SceneManager* assets);
    
};

#endif /* LoseView_hpp */
