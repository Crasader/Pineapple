//
//  MoveablePlatformModel.hpp
//  Pineapple
//
//  These are doors, controlled by buttons/levers that can open and close
//

#ifndef MoveablePlatformModel_hpp
#define MoveablePlatformModel_hpp

#include <cornell/CUBoxObstacle.h>
#include <cornell/CUWireNode.h>
#include "Const.h"
#include "Texture.h"
#include <stdio.h>

using namespace cocos2d;

class MoveablePlatformModel {
private:
    bool isClosed;
    bool isOpening;
    bool isClosing;
    bool isOpen;
    
    BoxObstacle box1;
    BoxObstacle box2;
    
public:
    void open() {
        if (!isOpen) {
            isClosing = false;
            isClosed = false;
            isOpening = true;
            // TODO
        }
    }
    
    void close() {
        if (!isClosed) {
            isOpening = false;
            isOpen = false;
            isClosing = true;
            // TODO
        }
    }
};

#endif /* MoveablePlatformModel_hpp */
