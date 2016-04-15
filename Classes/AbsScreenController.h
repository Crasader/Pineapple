//
//  AbsScreenController.h
//  Pineapple
//
//  Created by Michael Patashnik on 4/14/16.
//
//

#ifndef __AbsScreenController_h__
#define __AbsScreenController_h__

#define NONE                                0
#define TRANSITION_TO_LEVEL_SELECT          1
#define TRANSITION_TO_GAME                  2

namespace cocos2d {
    class RootLayer;
};

using namespace cocos2d;

class AbsScreenController {
    
protected:
    /** Reference to the root node of the scene graph */
    RootLayer* _rootnode;
    
    /** The current transitional status of this AbsScreenController */
    int _transitionStatus;
    
    
public:
    
    int getTransitionStatus() { return _transitionStatus; }
    
    void setTransitionStatus(int status) {_transitionStatus = status; }
    
};

#endif /* AbsScreenController_h */
