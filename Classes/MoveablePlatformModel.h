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

#define CLOSING_OPENING_SPEED         1.0f
#define OPEN_DISTANCE                 2.5f

#define MOVEABLE_PLATFORM_WIDTH       .25f
#define NUBBIN_LENGTH                 .125f

enum Color {purple = 1, red = 2, green = 3};

class MoveablePlatformModel {
private:
    bool _isClosed;
    bool _isOpening;
    bool _isClosing;
    bool _isOpen;
    
    bool _isVertical;
    
    // left if horizontal, top if vertical
    BoxObstacle* _nubbin1;
    // right if horizontal, bottom if vertical
    BoxObstacle* _nubbin2;
    // left if horizontal, top if vertical
    BoxObstacle* _box1;
    // right if horizontal, bottom if vertical
    BoxObstacle* _box2;
    
    // center of movable platform (if open it is the center between the two).
    Vec2 _pos;
    
    Color _color;
    
    void update();
    
public:
    Color getColor() { return _color; }
    
    void open() {
        if (!_isOpen) {
            _isClosing = false;
            _isClosed = false;
            _isOpening = true;
            // TODO(ekotlikoff) - this won't be great, update coordinates in update instead
            if (_isVertical) {
                _box1->setVY(CLOSING_OPENING_SPEED);
                _box2->setVY(-CLOSING_OPENING_SPEED);
            } else {
                _box1->setVX(CLOSING_OPENING_SPEED);
                _box2->setVX(-CLOSING_OPENING_SPEED);
            }
        }
    }
    
    void close() {
        if (!_isClosed) {
            _isOpening = false;
            _isOpen = false;
            _isClosing = true;
            // TODO(ekotlikoff) - this won't be great, update coordinates in update instead
            if (_isVertical) {
                _box1->setVY(-CLOSING_OPENING_SPEED);
                _box2->setVY(CLOSING_OPENING_SPEED);
            } else {
                _box1->setVX(-CLOSING_OPENING_SPEED);
                _box2->setVX(CLOSING_OPENING_SPEED);
            }
        }
    }
    
    float getDistance() {
        if (_isVertical) {
            return _box2->getY() + _box2->getHeight()/2.0f - _box1->getY() - _box1->getHeight()/2.0f;
        } else {
            return _box2->getX() + _box2->getWidth()/2.0f - _box1->getX() - _box1->getWidth()/2.0f;
        }
    }
    
    void setOpen() {
        _isOpen = true;
        _isOpening = false;
        if (_isVertical) {
            _box1->setY(_pos.y + OPEN_DISTANCE/2.0f + _box1->getHeight()/2.0f);
            _box2->setY(_pos.y - OPEN_DISTANCE/2.0f - _box2->getHeight()/2.0f);
        } else {
            _box1->setX(_pos.x + OPEN_DISTANCE/2.0f - _box1->getWidth()/2.0f);
            _box2->setX(_pos.x - OPEN_DISTANCE/2.0f + _box2->getWidth()/2.0f);
        }
    }
    
    void setClosed() {
        _isClosed = true;
        _isClosing = false;
        if (_isVertical) {
            _box1->setY(_pos.y + _box1->getHeight()/2.0f);
            _box2->setY(_pos.y - _box2->getHeight()/2.0f);
        } else {
            _box1->setX(_pos.x - _box1->getWidth()/2.0f);
            _box2->setX(_pos.x + _box2->getWidth()/2.0f);
        }
    }

    // pos is center of obstacle
    MoveablePlatformModel(Vec2 pos, float length, bool closed, bool vertical) : _isClosed(closed), _isOpening(false), _isClosing(false), _isOpen(!closed), _isVertical(vertical) {
        if (_isVertical) {
            _pos = pos;
            _nubbin1 = BoxObstacle::create(Vec2(_pos.x - MOVEABLE_PLATFORM_WIDTH / 2.0f, _pos.y + length/2.0f), Size(MOVEABLE_PLATFORM_WIDTH, NUBBIN_LENGTH));
            _nubbin1->setActive(false);
            _nubbin2 = BoxObstacle::create(Vec2(_pos.x - MOVEABLE_PLATFORM_WIDTH / 2.0f, _pos.y - length/2.0f + _nubbin1->getHeight()), Size(MOVEABLE_PLATFORM_WIDTH, NUBBIN_LENGTH));
            _nubbin2->setActive(false);
            _box1 = BoxObstacle::create(Vec2(_pos.x - MOVEABLE_PLATFORM_WIDTH / 2.0f, _pos.y), Size(MOVEABLE_PLATFORM_WIDTH, length/2.0f));
            _box2 = BoxObstacle::create(Vec2(_pos.x - MOVEABLE_PLATFORM_WIDTH / 2.0f, _pos.y - length / 2.0f), Size(MOVEABLE_PLATFORM_WIDTH, length/2.0f));
        }
    }
};

#endif /* MoveablePlatformModel_hpp */
