//
//  JSWallModel.h
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
#ifndef __WALL_MODEL_H__
#define __WALL_MODEL_H__

#include "Texture.h"
#include "Const.h"
#include <cornell/CUPolygonObstacle.h>
#include <cornell/CUPolygonNode.h>

using namespace cocos2d;
using namespace std;


#pragma mark -
#pragma mark Wall Model

/**
 * A wall door in the rocket lander game.
 *
 * Note that this class does keeps track of its own assets via keys, so that
 * they can be attached later as part of the scene graph.
 */
class WallModel : public PolygonObstacle {
private:
    /** This macro disables the copy constructor (not allowed on scene graphs) */
    CC_DISALLOW_COPY_AND_ASSIGN(WallModel);
    
protected:
    
    /**
     * Performs any necessary additions to the scene graph node.
     *
     * This method is necessary for custom physics objects that are composed
     * of multiple scene graph nodes.  In this case, it is because we
     * manage our own afterburner animations.
     */
    virtual void resetSceneNode() override;
    
    /** A reference to the node that defines the top texture (drawing only)
      * only used in floors */
    PolygonNode* _topNode;
    
    /** A reference to the node that defines the right texture (drawing only) 
      * only used in platforms */
    PolygonNode* _rightNode;
    
    /** A reference to the node that defines the left texture (drawing only)
     *  only used in platforms*/
    PolygonNode* _leftNode;
    
    /** The two points that make up the top left and top right corners of the wall */
    Vec2 _topPoints[2];
    
#pragma mark -
#pragma mark Static Constructors
public:
    
    /** Returns the central texture used for this wall. Depends if floor or platform */ 
    string getTextureID() {
        return isFloor() ? FLOOR_TEXTURE :
               isVerticalPlatform() ? PLATFORM_VERTICAL_CENTER_TEXTURE : PLATFORM_CENTER_TEXTURE ;
    }
    
    /** 
     * Returns true if this wall represents a floor (is touching the bottom of the screen),
     * false if this represents a platform (is not a floor)
     */
    bool isFloor() {
        return getY() - getHeight()/2 == 0;
    }
    
    /**
     * Returns true if this platform is vertical (taller than it is wide) */
    bool isVerticalPlatform() {
        return !isFloor() && (getHeight() > getWidth() || (getHeight() == getWidth() && getDrawScale().x < getDrawScale().y));
    }
    
    /** Returns the node that draws the top border of the wall */
    PolygonNode* getTopNode() { return _topNode; }
    
    /** Sets the node that draws the top border of the wall */
    void setTopNode(PolygonNode* topNode) { _topNode = topNode; }
    
    /** Returns the node that draws the top border of the wall */
    PolygonNode* getRightNode() { return _rightNode; }
    
    /** Sets the node that draws the top border of the wall */
    void setRightNode(PolygonNode* topNode) { _rightNode = topNode; }
    
    /** Returns the node that draws the top border of the wall */
    PolygonNode* getLeftNode() { return _leftNode; }
    
    /** Sets the node that draws the top border of the wall */
    void setLeftNode(PolygonNode* topNode) { _leftNode = topNode; }
    
    int getCollisionClass() { return WALL_C; };
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
    static WallModel* create(const Poly2& poly);
    
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
    static WallModel* create(const Poly2& poly, const Vec2& anchor);
    
    
#pragma mark -
#pragma mark Initializers
CC_CONSTRUCTOR_ACCESS:
    /*
     * Creates a new wall at the origin.
     */
    WallModel(void) : PolygonObstacle(), _topNode(nullptr) { }
    
    /**
     * Destroys this wall, releasing all resources.
     */
    virtual ~WallModel(void) {}
    
    /**
     * Initializes a wall from (not necessarily convex) polygon
     *
     * The anchor point (the rotational center) of the polygon is at the
     * center of the polygons bounding box.
     *
     * @param poly   The polygon vertices
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init(const Poly2& poly) override { return init(poly,Vec2(0.5,0.5)); }
    
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
    virtual bool init(const Poly2& poly, const Vec2& anchor) override;
    
};

#endif /* defined(__WALL_MODEL_H__) */