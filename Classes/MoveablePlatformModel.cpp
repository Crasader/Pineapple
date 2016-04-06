//
//  MoveablePlatformModel.cpp
//  Pineapple
//
//  Created by Emmett Kotlikoff on 4/4/16.
//
//

#include "MoveablePlatformModel.h"
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>
#include <cornell/CUPolygonNode.h>

#define MOVEABLE_PLATFORM_SCALE 0.18

MoveablePlatformModel* MoveablePlatformModel::create(const Vec2& pos, float length, bool isOpen, bool vertical, Color color) {
    MoveablePlatformModel* platform = new (std::nothrow) MoveablePlatformModel();
    if (platform && platform->init(pos, length, isOpen, vertical, color)) {
        platform->retain();
        return platform;
    }
    CC_SAFE_DELETE(platform);
    return nullptr;
}

bool MoveablePlatformModel::init(const Vec2& pos, float length, bool isOpen, bool vertical, Color color) {
    Obstacle::init(pos);
    
    _isClosed = !isOpen;
    _isOpening = false;
    _isClosing = false;
    _isOpen = isOpen;
    _isVertical = vertical;
    _color = color;
    _pos = pos;
    
    _length = length;
    
    _nubbin1 = BoxObstacle::create(Vec2::ZERO, SIZE_ONE);
    _nubbin1->setName(NUBBIN_NAME);
    _nubbin1->retain();
    _bodies.push_back(_nubbin1);
    
    _nubbin2 = BoxObstacle::create(Vec2::ZERO, SIZE_ONE);
    _nubbin2->setName(NUBBIN_NAME);
    _nubbin2->retain();
    _bodies.push_back(_nubbin2);
    
    _box1 = BoxObstacle::create(Vec2::ZERO, SIZE_ONE);
    _box1->setName(BOX_NAME);
    _box1->retain();
    _box1->setFixedRotation(true);
    _bodies.push_back(_box1);
    
    _box2 = BoxObstacle::create(Vec2::ZERO, SIZE_ONE);
    _box2->setName(BOX_NAME);
    _box2->retain();
    _box2->setFixedRotation(true);
    _bodies.push_back(_box2);
    
    return true;
}

void MoveablePlatformModel::resetSceneNode() {
    SceneManager* scene = AssetManager::getInstance()->getCurrent();
    
    Texture2D* leftNubbin;
    Texture2D* rightNubbin;
    Texture2D* box;
    
    //Get the textures
    if (_color == Color::red) {
        leftNubbin = scene->get<Texture2D>(LEFT_NUBBIN_TEXTURE_RED);
        rightNubbin = scene->get<Texture2D>(RIGHT_NUBBIN_TEXTURE_RED);
        box = scene->get<Texture2D>(MIDDLE_TEXTURE_RED);
    } else if (_color == Color::green) {
        leftNubbin = scene->get<Texture2D>(LEFT_NUBBIN_TEXTURE_GREEN);
        rightNubbin = scene->get<Texture2D>(RIGHT_NUBBIN_TEXTURE_GREEN);
        box = scene->get<Texture2D>(MIDDLE_TEXTURE_GREEN);
    } else if (_color == Color::blue) {
        leftNubbin = scene->get<Texture2D>(LEFT_NUBBIN_TEXTURE_BLUE);
        rightNubbin = scene->get<Texture2D>(RIGHT_NUBBIN_TEXTURE_BLUE);
        box = scene->get<Texture2D>(MIDDLE_TEXTURE_BLUE);
    }
    
    
    //Calculate sizes
    float cscale = Director::getInstance()->getContentScaleFactor();
    Texture2D* nubImage = scene->get<Texture2D>(LEFT_NUBBIN_TEXTURE_RED);
    Texture2D* centerImage = scene->get<Texture2D>(MIDDLE_TEXTURE_RED);
    Size nubbinSize = nubImage->getContentSize();
    nubbinSize.width  *= cscale;
    nubbinSize.height *= cscale;
    Size centerSize = centerImage->getContentSize();
    centerSize.width  *= cscale;
    centerSize.height *= cscale;
    
    if(_isVertical) {
        _maxXStretch = _length/2 * _drawScale.y / centerSize.width;
    } else {
        _maxXStretch = _length/2 * _drawScale.x / centerSize.width;
    }
    if (_isOpen) {
        setOpen();
    } else {
        setClosed();
    }
    
    Rect bounds;
    BoxObstacle *ob;
    
    //Left/Top Nubbin
    PolygonNode* pnode = PolygonNode::createWithTexture(leftNubbin);
    bounds.size = pnode->getContentSize();
    pnode->setPolygon(bounds);
    pnode->setScale(MOVEABLE_PLATFORM_SCALE);
    _bodies[0]->setSceneNode(pnode);
    
    ob = (BoxObstacle*) _bodies[0];
    if (_isVertical) {
        ob->setPosition(Vec2(_pos.x - MOVEABLE_PLATFORM_WIDTH / 2.0f, _pos.y + nubbinSize.width/2.0f * MOVEABLE_PLATFORM_SCALE/ _drawScale.y  + _length / 2.0f));
    } else {
        ob->setPosition(Vec2(_pos.x - nubbinSize.width/2.0f * MOVEABLE_PLATFORM_SCALE/ _drawScale.x - _length/2.0f, _pos.y - MOVEABLE_PLATFORM_WIDTH/2.0f));
    }
    ob->setDimension(pnode->getContentSize().width * MOVEABLE_PLATFORM_SCALE / _drawScale.x,
                     pnode->getContentSize().height * MOVEABLE_PLATFORM_SCALE/ _drawScale.y);
    
    _node->addChild(pnode);
    
    
    //Right/Bottom Nubbin
    pnode = PolygonNode::createWithTexture(rightNubbin);
    bounds.size = pnode->getContentSize();
    
    pnode->setPolygon(bounds);
    pnode->setScale(MOVEABLE_PLATFORM_SCALE);
    _bodies[1]->setSceneNode(pnode);
    
    ob = (BoxObstacle*) _bodies[1];
    if(_isVertical) {
        ob->setPosition(Vec2(_pos.x - MOVEABLE_PLATFORM_WIDTH / 2.0f, _pos.y - nubbinSize.width/2.0f * MOVEABLE_PLATFORM_SCALE/ _drawScale.y - _length/2.0f));
    } else {
        ob->setPosition(Vec2(_pos.x + nubbinSize.width/2.0f * MOVEABLE_PLATFORM_SCALE/ _drawScale.x  + _length / 2.0f, _pos.y - MOVEABLE_PLATFORM_WIDTH/2.0f));
    }
    pnode->setScale(MOVEABLE_PLATFORM_SCALE);
    ob->setDimension(pnode->getContentSize().width * MOVEABLE_PLATFORM_SCALE / _drawScale.x,
                     pnode->getContentSize().height * MOVEABLE_PLATFORM_SCALE/ _drawScale.y);
    
    
    _node->addChild(pnode);
    
    //Left Center
    ob = (BoxObstacle*) _bodies[2];
    if(_isVertical) {
        ob->setPosition(Vec2(_pos.x - MOVEABLE_PLATFORM_WIDTH / 2.0f, _pos.y - _length/4.0f));
        ob->setDimension(_length/2 * _drawScale.y/_drawScale.x,
                         pnode->getContentSize().height * MOVEABLE_PLATFORM_SCALE / _drawScale.y);
    } else {
        ob->setPosition(Vec2(_pos.x - _length/4.0f, _pos.y - MOVEABLE_PLATFORM_WIDTH/2.0f));
        ob->setDimension(_length/2,
                         pnode->getContentSize().height * MOVEABLE_PLATFORM_SCALE / _drawScale.y);
    }

    pnode = PolygonNode::createWithTexture(box);
    bounds.size = pnode->getContentSize();
    
    pnode->setPolygon(bounds);
    pnode->setScaleX(_xStretch);
    pnode->setScaleY(MOVEABLE_PLATFORM_SCALE);
    _bodies[2]->setSceneNode(pnode);
    
    _node->addChild(pnode);
    
    //Right Center
    ob = (BoxObstacle*) _bodies[3];
    if(_isVertical) {
        ob->setPosition(Vec2(_pos.x - MOVEABLE_PLATFORM_WIDTH / 2.0f, _pos.y + _length/4.0f));
        ob->setDimension(_length/2 * _drawScale.y/_drawScale.x,
                         pnode->getContentSize().height * MOVEABLE_PLATFORM_SCALE / _drawScale.y);
    } else {
        ob->setPosition(Vec2(_pos.x + _length/4.0f, _pos.y - MOVEABLE_PLATFORM_WIDTH/2.0f));
        ob->setDimension(_length/2,
                         pnode->getContentSize().height * MOVEABLE_PLATFORM_SCALE / _drawScale.y);
    }
    
    pnode = PolygonNode::createWithTexture(box);
    bounds.size = pnode->getContentSize();
    
    pnode->setPolygon(bounds);
    pnode->setScaleX(_xStretch);
    pnode->setScaleY(MOVEABLE_PLATFORM_SCALE);
    _bodies[3]->setSceneNode(pnode);
    
    _node->addChild(pnode);
    
    float nintyDegrees = -1.5708;

    if (_isVertical) {
        _nubbin1->setAngle(nintyDegrees);
        _nubbin2->setAngle(nintyDegrees);
        _box1->setAngle(nintyDegrees);
        _box2->setAngle(nintyDegrees);
    }
    
}

void MoveablePlatformModel::open() {
    if (!_isOpen) {
        _isClosing = false;
        _isClosed = false;
        _isOpening = true;
    }
}

void MoveablePlatformModel::setOpen() {
    _isOpen = true;
    _isOpening = false;
    _isClosed = false;
    _isClosing = false;
    _xStretch = MIN_X_STRETCH;
}

void MoveablePlatformModel::close() {
    if (!_isClosed) {
        _isOpening = false;
        _isOpen = false;
        _isClosing = true;
    }
}

void MoveablePlatformModel::setClosed() {
    _isClosed = true;
    _isClosing = false;
    _isOpen = false;
    _isOpening = false;
    _xStretch = _maxXStretch;
}

void MoveablePlatformModel::update(float dt) {
    ComplexObstacle::update(dt);
    
    //Change the strech value
    float oldStretch = _xStretch;
    if (_isOpening) {
        _xStretch = fmax(MIN_X_STRETCH, _xStretch - X_STRETCH_INC_PER_SEC * dt);
        if (_xStretch == MIN_X_STRETCH) {
            setOpen();
        }
    } else if (_isClosing) {
        _xStretch = fmin(_maxXStretch, _xStretch + X_STRETCH_INC_PER_SEC * dt);
        if (_xStretch == _maxXStretch) {
            setClosed();
        }
    }
    
    if (oldStretch != _xStretch) {
        _box1->getSceneNode()->setScaleX(_xStretch);
        _box2->getSceneNode()->setScaleX(_xStretch);
        
        if (_isVertical) {
            _box1->setWidth(_length/2.0f * _xStretch/_maxXStretch* _drawScale.y / _drawScale.x);
            _box2->setWidth(_length/2.0f * _xStretch/_maxXStretch* _drawScale.y / _drawScale.x);
            _box1->setY(_nubbin1->getY() - (_nubbin1->getWidth()/2.0f + _box1->getWidth()/2.0f)* _drawScale.x / _drawScale.y);
            _box2->setY(_nubbin2->getY() + (_nubbin2->getWidth()/2.0f + _box2->getWidth()/2.0f)* _drawScale.x / _drawScale.y);
        } else {
            _box1->setWidth(_length/2.0f * _xStretch/_maxXStretch);
            _box2->setWidth(_length/2.0f * _xStretch/_maxXStretch);
            _box1->setX(_nubbin1->getX() + _nubbin1->getWidth()/2.0f + _box1->getWidth()/2.0f);
            _box2->setX(_nubbin2->getX() - _nubbin2->getWidth()/2.0f - _box2->getWidth()/2.0f);
            
        }
        
    }
}

void MoveablePlatformModel::resetDebugNode() {
    for(int ii = 0; ii < _bodies.size(); ii++) {
        WireNode* wire = WireNode::create();
        wire->setColor(_debug->getColor());
        
        if (_bodies[ii]->getDebugNode() != nullptr) {
            _debug->removeChild(_bodies[ii]->getDebugNode());
        }
        
        _bodies[ii]->setDebugNode(wire);
        _debug->addChild(wire);
    }
}