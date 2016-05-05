//
//  JSWallModel.cpp
//  JSONDemo
//
//  This encapsulates all of the information for representing a wall. Since wall never
//  move, we normally would not need a separate class (because we could just use an instance
//  of PolygonObstacle).  However, this changes when we do dynamic level loading.
//
//  The problem is that we need to initialize an physics object when a level is loaded,
//  but NOT attach any assets yet.  That is because the physics object is itself an asset
//  and it is not safe to assume the order of asset loading.  So instead, we just store
//  the keys of the assets in this class, and they attach the assets later when we
//  initialize the scene graph.
//
//  This file is based on the CS 3152 PhysicsDemo Lab by Don Holden, 2007
//
//  Author: Walker White
//  Version: 1/20/16
//
#include "WallModel.h"
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>
#include <cornell/CUPolygonNode.h>

#define WALL_SCALE 0.5f

#pragma mark -
#pragma mark Static Constructors
/**
 * Creates a wall from (not necessarily convex) polygon
 *
 * The anchor point (the rotational center) of the polygon is at the
 * center of the polygons bounding box.
 *
 * Unlike most constructors, this one RETAINS the object and does not
 * autorelease.  That is because (1) this object is allocated in a
 * second thread (the asset loader thread) and (2) autoreleased objects
 * are freed in the main thread.  Hence autorelease is not thread safe,
 *
 * @param poly   The polygon vertices
 *
 * @retain a reference to the object (transferable)
 *
 * @return  An RETAINED physics object
 */
WallModel* WallModel::create(const Poly2& poly) {
    WallModel* obstacle = new (std::nothrow) WallModel();
    if (obstacle && obstacle->init(poly)) {
        // AUTORELEASE IS NOT THREAD SAFE FOR ASSET LOADING
        obstacle->retain();
        return obstacle;
    }
    CC_SAFE_DELETE(obstacle);
    return nullptr;
}

/**
 * Creates a wall from (not necessarily convex) polygon
 *
 * The anchor point (the rotational center) of the polygon is specified as a
 * ratio of the bounding box.  An anchor point of (0,0) is the bottom left of
 * the bounding box.  An anchor point of (1,1) is the top right of the bounding
 * box.  The anchor point does not need to be contained with the bounding box.
 *
 * Unlike most constructors, this one RETAINS the object and does not
 * autorelease.  That is because (1) this object is allocated in a
 * second thread (the asset loader thread) and (2) autoreleased objects
 * are freed in the main thread.  Hence autorelease is not thread safe,
 *
 * @param  poly     The polygon vertices
 * @param  anchor   The rotational center of the polygon
 *
 * @retain a reference to the object (transferable)
 *
 * @return  An RETAINED physics object
 */
WallModel* WallModel::create(const Poly2& poly, const Vec2& anchor) {
    WallModel* obstacle = new (std::nothrow) WallModel();
    if (obstacle && obstacle->init(poly,anchor)) {
        // AUTORELEASE IS NOT THREAD SAFE FOR ASSET LOADING
        obstacle->retain();
        return obstacle;
    }
    CC_SAFE_DELETE(obstacle);
    return nullptr;
}


#pragma mark -
#pragma mark Initializers

/**
 * Initializes a wall from (not necessarily convex) polygon
 *
 * The anchor point (the rotational center) of the polygon is specified as a
 * ratio of the bounding box.  An anchor point of (0,0) is the bottom left of
 * the bounding box.  An anchor point of (1,1) is the top right of the bounding
 * box.  The anchor point does not need to be contained with the bounding box.
 *
 * @param  poly     The polygon vertices
 * @param  anchor   The rotational center of the polygon
 *
 * @return  true if the obstacle is initialized properly, false otherwise.
 */
bool WallModel::init(const Poly2& poly, const Vec2& anchor) {
    if (PolygonObstacle::init(poly,anchor)) {
        
        //Find the two top points
        int i = 0;
        float yMin = INT_MAX;
        
        for(auto it = poly.getVertices().begin(); it != poly.getVertices().end(); ++it) {
            Vec2 v = (Vec2) *it;
            yMin = min(yMin, v.y);
        }
        
        for(auto it = poly.getVertices().begin(); it != poly.getVertices().end(); ++it) {
            Vec2 v = (Vec2) *it;

            if (v.y != yMin) {
                _topPoints[i] = v;
                i++;
            }
        }
        
        //Check if need right left flip
        if (_topPoints[0].x > _topPoints[1].x) {
            Vec2 v = _topPoints[0];
            _topPoints[0] = _topPoints[1];
            _topPoints[1] = v;
        }
                
        _topNode = nullptr;
        _leftNode = nullptr;
        _rightNode = nullptr;
        
        return true;
    }
    
    return false;
}


#pragma mark -
#pragma mark Animation
/**
 * Performs any necessary additions to the scene graph node.
 *
 * This method is necessary for custom physics objects that are composed
 * of multiple scene graph nodes.  In this case, it is because we
 * manage our own afterburner animations.
 */
void WallModel::resetSceneNode() {
    PolygonNode* pnode = dynamic_cast<PolygonNode*>(_node);
    if (pnode != nullptr) {
        // We need to know the content scale for resolution independence
        // If the device is higher resolution than 1024x576, Cocos2d will scale it
        // THIS DOES NOT FIX ASPECT RATIO PROBLEMS
        // If you are using a device with a 3:2 aspect ratio, you will need to
        // completely redo the level layout.  We can help if this is an issue.
        
        float radToDeg = 180.0f / M_PI;
        
        if( _topNode != nullptr) {
            float yMin = min(_topPoints[0].y, _topPoints[1].y);
            float y = fabs(_topPoints[0].y - _topPoints[1].y);
            float x =fabs(_topPoints[0].x - _topPoints[1].x);
            
            Vec2 p = Vec2(pnode->getPositionX(),
                          pnode->getPositionY() + (yMin * _drawScale.y -_topNode->getContentSize().height)/2);
            
            _topNode->setPosition(p);
            
            float angle = atan((y*_drawScale.y)/(x*_drawScale.x)) * radToDeg;
            
            if (_topPoints[0].y < _topPoints[1].y) {
                angle = -angle;
            }
            
            _topNode->setScaleX(sqrt(x*x + y*y) * _drawScale.x /_topNode->getContentSize().width);
            _topNode->setRotation(angle);
        }
        
        if (isVerticalPlatform()) {
            if (_leftNode != nullptr) {
                _leftNode->setRotation(-90);
                _leftNode->setScale(getSize().width * _drawScale.x /_leftNode->getContentSize().height);
                _leftNode->setPosition(Vec2(getX() * _drawScale.x, getY() * _drawScale.y) -
                                       Vec2(0, getSize().height * _drawScale.y/2 - _leftNode->getContentSize().width * _leftNode->getScale()/2));
                
            }
            if (_rightNode != nullptr) {
                _rightNode->setRotation(-90);
                _rightNode->setScale(getSize().width * _drawScale.x /_rightNode->getContentSize().height);
                _rightNode->setPosition(Vec2(getX() * _drawScale.x, getY() * _drawScale.y) +
                                        Vec2(0, getSize().height * _drawScale.y/2 - _rightNode->getContentSize().width * _rightNode->getScale()/2));
            }

        } else {
            if (_leftNode != nullptr) {
                _leftNode->setScale(getSize().height * _drawScale.y /_leftNode->getContentSize().height);
                _leftNode->setPosition(Vec2(getX() * _drawScale.x, getY() * _drawScale.y) -
                                       Vec2(getSize().width * _drawScale.x/2 - _leftNode->getContentSize().width * _leftNode->getScale()/2, 0));
                
            }
            if (_rightNode != nullptr) {
                _rightNode->setScale(getSize().height * _drawScale.y /_rightNode->getContentSize().height);
                _rightNode->setPosition(Vec2(getX() * _drawScale.x, getY() * _drawScale.y) +
                                        Vec2(getSize().width * _drawScale.x/2 - _rightNode->getContentSize().width * _rightNode->getScale()/2, 0));
            }
        }
    }
}
