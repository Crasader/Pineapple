//
//  AbsScreenController.h
//  Pineapple
//
//  Created by Michael Patashnik on 4/14/16.
//
//

#ifndef __AbsScreenController_h__
#define __AbsScreenController_h__

#define TRANSITION_NONE                     0

#define TRANSITION_TO_HOME                  10
#define TRANSITION_TO_LEVEL_SELECT          11
#define TRANSITION_TO_GAME                  12
#define TRANSITION_TO_NEXT_LEVEL            13


#define TRANSITION_TO_EXIT                  99


#include "ui/CocosGUI.h"
#include "Texture.h"

namespace cocos2d {
    class RootLayer;
};

using namespace cocos2d;
using namespace cocos2d::ui;

class AbsScreenController {
    
protected:
    /** Reference to the root node of the scene graph */
    Node* _rootnode;
    
    /** The current transitional status of this AbsScreenController */
    int _transitionStatus;
    
    /** True once this has been initted */
    bool _isInitted = false;
    
public:
    void initButtonTextOps(Button* button) {
        button->setVisible(true);
        button->setTitleFontName(ELECTRIC_CIRCUS_FONT_LOCATION);
        button->setTitleColor(Color3B::WHITE);
        button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
            if (type == ui::Widget::TouchEventType::BEGAN) {
                ((Button*)sender)->setTitleColor(Color3B::BLACK);
            } else if (type == ui::Widget::TouchEventType::CANCELED) {
                ((Button*)sender)->setTitleColor(Color3B::WHITE);
            } else if (type == ui::Widget::TouchEventType::ENDED) {
                ((Button*)sender)->setTitleColor(Color3B::WHITE);
            }
        });
    }
    
    int getTransitionStatus() { return _transitionStatus; }
    
    void setTransitionStatus(int status) {_transitionStatus = status; }
    
    bool isInitted() { return _isInitted; }
    
    virtual void update(float dt) {};
    
};

#endif /* AbsScreenController_h */
