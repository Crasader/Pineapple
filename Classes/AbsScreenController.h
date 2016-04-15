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
#define TRANSITION_TO_VISIBLE               1


#define TRANSITION_TO_LEVEL_SELECT          10
#define TRANSITION_TO_GAME                  11


#define TRANSITION_TO_EXIT                  99

namespace cocos2d {
    class RootLayer;
};

using namespace cocos2d;

class AbsScreenController {
    
protected:
    /** Reference to the root node of the scene graph */
    Node* _rootnode;
    
    /** The current transitional status of this AbsScreenController */
    int _transitionStatus;
    
    /** True once this has been initted */
    bool _isInitted;
    
public:
    
    int getTransitionStatus() { return _transitionStatus; }
    
    void setTransitionStatus(int status) {_transitionStatus = status; }
    
    bool isInitted() { return _isInitted; }
    
    virtual void update(float dt) {};
    
};

#endif /* AbsScreenController_h */
