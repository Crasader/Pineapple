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

#define MIN_X_STRETCH                 0.01f
#define X_STRETCH_INC_PER_SEC         1.0f

#define MOVEABLE_PLATFORM_WIDTH       1.0f

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
    
    bool _nubbinsVisible;
    float _length;
    
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
    
    Vec2 _drawScale;
    
    Color _color;
    
    float _xStretch;
    
    float _maxXStretch;
    
    void update(float dt) override;
    
    void open();
    
    void close();
    
public:
    bool createJoints(b2World& world) {  return true;  }
    
    virtual int getCollisionClass() { return MOVEABLE_PLATFORM_C; }
    
    static Color getColor(int index) {
        if (index == 1) {
            return Color::blue;
        } else if (index == 2) {
            return Color::red;
        } else if (index == 3) {
            return Color::green;
        } else {
            CC_ASSERT(false);
        }
    }
    
    void storeDrawScale(Vec2 scale) { _drawScale = scale; }
    
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
        
    void setOpen();
    
    void setClosed();
    
    void toggle();

    // length is length of two middle boxes
    static MoveablePlatformModel* create(const Vec2& pos, float length, bool isOpen, bool vertical, bool nubbinsVisible, Color color);
    
#pragma mark Drawing Methods
    
    
CC_CONSTRUCTOR_ACCESS:
    virtual bool init(const Vec2& pos, float length, bool isOpen, bool vertical, bool nubbinsVisible, Color color);
    
    // pos is center of obstacle
    MoveablePlatformModel() : _isClosed(false), _isOpening(false), _isClosing(false), _isOpen(false), _isVertical(false), _nubbinsVisible(false) { }
};

#endif /* MoveablePlatformModel_hpp */
