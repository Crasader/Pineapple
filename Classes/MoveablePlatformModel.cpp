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

#define MOVEABLE_PLATFORM_SCALE 1

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

MoveablePlatformModel* MoveablePlatformModel::create(const Vec2& pos, const Vec2& scale, float length, bool isOpen, bool vertical, Color color) {
    MoveablePlatformModel* platform = new (std::nothrow) MoveablePlatformModel();
    if (platform && platform->init(pos, scale, length, isOpen, vertical, color)) {
        platform->autorelease();
        return platform;
    }
    CC_SAFE_DELETE(platform);
    return nullptr;
}

bool MoveablePlatformModel::init(const Vec2& pos, const cocos2d::Vec2& scale, float length, bool isOpen, bool vertical, Color color) {
    Obstacle::init(pos);
    setDrawScale(scale);
    
    setName(MOVEABLE_PLATFORM_NAME);
    
    _isClosed = !isOpen;
    _isOpening = false;
    _isClosing = false;
    _isOpen = isOpen;
    _isVertical = vertical;
    _color = color;
    _pos = Vec2(pos.x * scale.x, pos.y * scale.y);
    _scale = scale;
    
    length = length * _scale.x;
    
    float cscale = Director::getInstance()->getContentScaleFactor();
    SceneManager* scene = AssetManager::getInstance()->getCurrent();
    Texture2D* nubImage = scene->get<Texture2D>(LEFT_NUBBIN_TEXTURE_RED);
    Texture2D* centerImage = scene->get<Texture2D>(MIDDLE_TEXTURE_RED);
    Size nubbinSize = nubImage->getContentSize();
    nubbinSize.width  *= cscale/scale.x;
    nubbinSize.height *= cscale/scale.y;
    Size centerSize = centerImage->getContentSize();
    centerSize.width  *= cscale/scale.x;
    centerSize.height *= cscale/scale.y;
    
    float nintyDegrees = 1.5708; //in radians
    
    if (_isVertical) {
        _nubbin1 = BoxObstacle::create(Vec2(_pos.x - MOVEABLE_PLATFORM_WIDTH / 2.0f, _pos.y + length/2.0f), nubbinSize);
        _nubbin1->setName(NUBBIN_NAME);
        _nubbin1->retain();
        _nubbin1->setActive(false);
        _nubbin1->setAngle(nintyDegrees);
        _bodies.push_back(_nubbin1);
        
        _nubbin2 = BoxObstacle::create(Vec2(_pos.x - MOVEABLE_PLATFORM_WIDTH / 2.0f, _pos.y - length/2.0f + _nubbin1->getHeight()), nubbinSize);
        _nubbin2->setName(NUBBIN_NAME);
        _nubbin2->retain();
        _nubbin2->setActive(false);
        _nubbin2->setAngle(nintyDegrees);
        _bodies.push_back(_nubbin2);
        
        _box1 = BoxObstacle::create(Vec2(_pos.x - MOVEABLE_PLATFORM_WIDTH / 2.0f, _pos.y), centerSize);
        _box1->setName(BOX_NAME);
        _box1->retain();
        _box1->setFixedRotation(true);
        _box1->setBodyType(b2_staticBody);
        _box1->setAngle(nintyDegrees);
        _bodies.push_back(_box1);
        
        _box2 = BoxObstacle::create(Vec2(_pos.x - MOVEABLE_PLATFORM_WIDTH / 2.0f, _pos.y - length / 2.0f), centerSize);
        _box2->setName(BOX_NAME);
        _box2->retain();
        _box2->setFixedRotation(true);
        _box2->setBodyType(b2_staticBody);
        _box2->setAngle(nintyDegrees);
        _bodies.push_back(_box2);
    } else {
        _nubbin1 = BoxObstacle::create(Vec2(_pos.x - NUBBIN_LENGTH/2.0f - length/2.0f, _pos.y - MOVEABLE_PLATFORM_WIDTH/2.0f), nubbinSize);
        _nubbin1->setName(NUBBIN_NAME);
        _nubbin1->retain();
        _nubbin1->setActive(false);
        _bodies.push_back(_nubbin1);
        
        _nubbin2 = BoxObstacle::create(Vec2(_pos.x + NUBBIN_LENGTH/2.0f + length / 2.0f, _pos.y - MOVEABLE_PLATFORM_WIDTH/2.0f), nubbinSize);
        _nubbin2->setName(NUBBIN_NAME);
        _nubbin2->retain();
        _nubbin2->setActive(false);
        _bodies.push_back(_nubbin2);
        
        _box1 = BoxObstacle::create(Vec2(_pos.x - length/4.0f, _pos.y - MOVEABLE_PLATFORM_WIDTH/2.0f), centerSize);
        _box1->setName(BOX_NAME);
        _box1->retain();
        _box1->setFixedRotation(true);
        _box1->setBodyType(b2_staticBody);
        _bodies.push_back(_box1);
        
        _box2 = BoxObstacle::create(Vec2(_pos.x + length/4.0f, _pos.y - MOVEABLE_PLATFORM_WIDTH/2.0f), centerSize);
        _box2->setName(BOX_NAME);
        _box2->retain();
        _box2->setFixedRotation(true);
        _box2->setBodyType(b2_staticBody);
        _bodies.push_back(_box2);
    }
    
    return true;
}

void MoveablePlatformModel::resetSceneNode() {
    SceneManager* scene = AssetManager::getInstance()->getCurrent();
    
    Texture2D* leftNubbin;
    Texture2D* rightNubbin;
    Texture2D* box;
    
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
    
    float cscale = Director::getInstance()->getContentScaleFactor();
    Texture2D* nubImage = scene->get<Texture2D>(LEFT_NUBBIN_TEXTURE_RED);
    Texture2D* centerImage = scene->get<Texture2D>(MIDDLE_TEXTURE_RED);
    Size nubbinSize = nubImage->getContentSize();
    nubbinSize.width  *= cscale/_scale.x;
    nubbinSize.height *= cscale/_scale.y;
    Size centerSize = centerImage->getContentSize();
    centerSize.width  *= cscale/_scale.x;
    centerSize.height *= cscale/_scale.y;
    
    PolygonNode* sprite = PolygonNode::createWithTexture(leftNubbin);
    //sprite->setScale(_scale);
    _bodies[0]->setSceneNode(sprite);
    _node->addChild(sprite);
    
    sprite = PolygonNode::createWithTexture(rightNubbin);
    //sprite->setScale(_scale);
    _bodies[1]->setSceneNode(sprite);
    _node->addChild(sprite);
    
    sprite = PolygonNode::createWithTexture(box);
    //sprite->setScale(_scale);
    _bodies[2]->setSceneNode(sprite);
    _node->addChild(sprite);
    
    sprite = PolygonNode::createWithTexture(box);
    //sprite->setScale(_scale);
    _bodies[3]->setSceneNode(sprite);
    _node->addChild(sprite);
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