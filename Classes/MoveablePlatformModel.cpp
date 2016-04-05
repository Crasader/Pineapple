//
//  MoveablePlatformModel.cpp
//  Pineapple
//
//  Created by Emmett Kotlikoff on 4/4/16.
//
//

#include "MoveablePlatformModel.h"

// I suspect the design here will be two boxObjects that when opened will move away from eachother

void MoveablePlatformModel::update() {
    if (_isOpening) {
        // if two pieces are at or beyond open distance
        if (getDistance() >= OPEN_DISTANCE) {
            setOpen();
        }
    } else if (_isClosing) {
        // if two pieces are touching or overlapping
        if (getDistance() <= 0.0f) {
            setClosed();
        }
    }
}