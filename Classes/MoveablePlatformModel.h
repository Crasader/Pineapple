//
//  MoveablePlatformModel.hpp
//  Pineapple
//
//  These are doors, controlled by buttons/levers that can open and close
//

#ifndef MoveablePlatformModel_hpp
#define MoveablePlatformModel_hpp

#include <cornell/CUComplexObstacle.h>
#include <cornell/CUBoxObstacle.h>
#include <cornell/CUWireNode.h>
#include "Const.h"
#include "Texture.h"
#include <stdio.h>

using namespace cocos2d;

#define MOVEABLE_PLATFORM_NAME      "moveable"
// outer marking nubbins that arent active
#define NUBBIN_NAME                 "nubbin"
// Inner moveable boxes
#define BOX_NAME                    "inner box"


#define CLOSING_OPENING_SPEED         1.0f
#define OPEN_DISTANCE                 2.5f

#define MOVEABLE_PLATFORM_HEIGHT      0.8f
#define MOVEABLE_PLATFORM_WIDTH       5.0f
#define NUBBIN_LENGTH                 1.0f

enum Color {blue = 1, red = 2, green = 3};

class MoveablePlatformModel : public ComplexObstacle {
protected:
    virtual void resetDebugNode() override;

    virtual void resetSceneNode() override;

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
    
    // pos is at center of movable platform (if open it is the center between the two).
    //   <nubbin> [_box1] _pos [_box2] <nubbin>
    Vec2 _pos;
    
    Vec2 _scale;
    
    Color _color;
    
    void update();
    
public:
    Color getColor() { return _color; }
    
    BoxObstacle* getNubbin1() {
        return _nubbin1;
    }
    
    BoxObstacle* getNubbin2() {
        return _nubbin2;
    }
    
    BoxObstacle* getBox1() {
        return _box1;
    }
    
    BoxObstacle* getBox2() {
        return _box2;
    }
    
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

    // length is length of two middle boxes
    static MoveablePlatformModel* create(const Vec2& pos, const Vec2& scale, float length, bool isOpen, bool vertical, Color color);
    
CC_CONSTRUCTOR_ACCESS:
    virtual bool init(const Vec2& pos, const Vec2& scale, float length, bool isOpen, bool vertical, Color color);
    
    // pos is center of obstacle
    MoveablePlatformModel() : _isClosed(false), _isOpening(false), _isClosing(false), _isOpen(false), _isVertical(false) { }
};

#endif /* MoveablePlatformModel_hpp */
