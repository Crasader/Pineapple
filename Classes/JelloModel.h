//
//  PFJelloModel.h
//  PlatformerDemo
//
//  This encapsulates all of the information for the character avatar.  Note how this
//  class combines physics and animation.  This is a good template for models in
//  your game.
//
//  WARNING: There are a lot of shortcuts in this design that will do not adapt well
//  to data driven design.  This demo has a lot of simplifications to make it a bit
//  easier to see how everything fits together.  However, the model classes and how
//  they are initialized will need to be changed if you add dynamic level loading.
//
//  Pay close attention to how this class designed.  Subclasses of Cocos2d classes
//  (which are all subclasses of the class Ref) should never have normal public
//  constructors.  Instead, you should organize their constructors into three parts,
//  just like we have done in this class.
//
//  NORMAL CONSTRUCTOR:
//  The standard constructor should be protected (not private).  It should only
//  initialize pointers to nullptr and primitives to their defaults (pointers are
//  not always nullptr to begin with).  It should NOT take any arguments and should
//  not allocate any memory or call any methods.
//
//  STATIC CONSTRUCTOR
//  This is a static method that allocates the object and initializes it.  If
//  initialization fails, it immediately disposes of the object.  Otherwise, it
//  returns an autoreleased object, starting the garbage collection system.
//  These methods all look the same.  You can copy-and-paste them from sample code.
//  The only difference is the init method called.
//
//  INIT METHOD
//  This is a protected method that acts like what how would normally think a
//  constructor might work.  It allocates memory and initializes all values
//  according to provided arguments.  As memory allocation can fail, this method
//  needs to return a boolean indicating whether or not initialization was
//  successful.
//
//  This file is based on the CS 3152 PhysicsDemo Lab by Don Holden, 2007
//
//  Author: Walker White
//  Version: 1/15/15
//
#ifndef __PF_JELLO_MODEL_H__
#define __PF_JELLO_MODEL_H__

#include <cornell/CUBoxObstacle.h>
#include <cornell/CUWireNode.h>
#include <cornell/CUAnimationNode.h>
#include "Const.h"
#include "Texture.h"
//#include "CollisionObjectModel.h"

using namespace cocos2d;
 
#pragma mark -
#pragma mark Drawing Constants
/** The name for a jello, for identification purposes */
#define JELLO_NAME       "jello"


#pragma mark -
#pragma mark Physics Constants
/** Amount of upward force given to will or a kid when hitting a jello */
#define JELLO_BOUNCE_FORCE  10.0f
/** Amount of horizontal force given to a kid when hitting a jello */
#define JELLO_HORIZONTAL_FORCE 5.0f

/** Number of frames in the rest animation */
#define JELLO_FRAME_COUNT	31


#pragma mark -
#pragma mark JELLO Model
/**
 * Player avatar for the plaform game.
 *
 * Note that this class uses a capsule shape, not a rectangular shape.  In our
 * experience, using a rectangular shape for a character will regularly snag
 * on a platform.  The round shapes on the end caps lead to smoother movement.
 */
class JelloModel : public BoxObstacle/*, public CollisionObjectModel*/ {
private:
    /** This macro disables the copy constructor (not allowed on physics objects) */
    CC_DISALLOW_COPY_AND_ASSIGN(JelloModel);

protected:
    /**
     * Redraws the outline of the physics fixtures to the debug node
     *
     * The debug node is use to outline the fixtures attached to this object.
     * This is very useful when the fixtures have a very different shape than
     * the texture (e.g. a circular shape attached to a square texture).
     */
    virtual void resetDebugNode() override;

	/** Filmstrip for restcycle animation */
	AnimationNode* _jelloRestcycle;
	/** Frame counter for restcycle animation */
	float _jelloRestcycleFrame;

	/** Whether something is bouncing on the jello */
	bool _bouncing;
	bool _tmp;
    
    /** Jello x coordinate in tiled level */
    int _tiledXCoord;
    /** Jello y coordinate in tiled level */
    int _tiledYCoord;


public:
	/**
	*	returns collision class
	*/
	int getCollisionClass() { return JELLO_C; };
    
    /** Returns the x coordinate for this crushable in the tiled level */
    int getTiledXCoord() { return _tiledXCoord; }
    
    /** Returns the y coordinate for this crushable in the tiled level */
    int getTiledYCoord() { return _tiledYCoord; }
    
#pragma mark Static Constructors
    /**
     * Creates a new jello at the origin.
     *
     * The jello is scaled so that 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @return  An autoreleased physics object
     */
    static JelloModel* create();
    
    /**
     * Creates a new jello at the given position.
     *
     * The jello is scaled so that 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos      Initial position in world coordinates
     *
     * @return  An autoreleased physics object
     */
    static JelloModel* create(int xCoord, int yCoord, const Vec2& pos);
    
    /**
     * Creates a new jello at the given position.
     *
     * The jello is sized according to the given drawing scale.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos      Initial position in world coordinates
     * @param  scale    The drawing scale
     *
     * @return  An autoreleased physics object
     */
    static JelloModel* create(int xCoord, int yCoord, const Vec2& pos, const Vec2& scale);
    
#pragma mark Drawing Methods
    /**
     * Performs any necessary additions to the scene graph node.
     *
     * This method is necessary for custom physics objects that are composed
     * of multiple scene graph nodes.  In this case, it is because we
     * manage our own afterburner animations.
     */
    virtual void resetSceneNode() override;
    
#pragma mark Physics Methods
    /**
     * Creates the physics Body(s) for this object, adding them to the world.
     *
     * This method overrides the base method to keep your ship from spinning.
     *
     * @param world Box2D world to store body
     *
     * @return true if object allocation succeeded
     */
    void createFixtures() override;
    
    /**
     * Release the fixtures for this body, reseting the shape
     *
     * This is the primary method to override for custom physics objects.
     */
    void releaseFixtures() override;
    
    /**
     * Updates the object's physics state (NOT GAME LOGIC).
     *
     * We use this method to reset cooldowns.
     *
     * @param delta Number of seconds since last animation frame
     */
    void update(float dt) override;
    
    /**
     * Applies the force to the body of this jello
     *
     * This method should be called after the force attribute is set.
     */
    void applyForce();

	/**
	* Animate the resting Jello
	*/
	void animate();

	/** 
	* Returns true if something is bouncing on the jello
	*/
	bool getBouncing() { return _bouncing; }

	/**
	* Sets whether something is bouncing on the jello
	*/
	void setBouncing(bool bounce) { _bouncing = bounce; }

    
CC_CONSTRUCTOR_ACCESS:
#pragma mark Hidden Constructors
    /**
     * Creates a degenerate Jello object.
     *
     * This constructor does not initialize any of the jello values beyond
     * the defaults.  To use a JelloModel, you must call init().
     */
    JelloModel() : BoxObstacle(),
	_jelloRestcycle(nullptr){ }
	~JelloModel() { }

    /**
     * Initializes a new jello at the origin.
     *
     * The jello is scaled so that 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init() override { return init(0, 0, Vec2::ZERO, Vec2::ONE); }
    
    /**
     * Initializes a new jello at the given position.
     *
     * The jello is scaled so that 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos      Initial position in world coordinates
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init(int x, int y, const Vec2& pos) { return init(x, y, pos, Vec2::ONE); }
    
    /**
     * Initializes a new jello at the given position.
     *
     * The jello is sized according to the given drawing scale.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos      Initial position in world coordinates
     * @param  scale    The drawing scale
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init(int x, int y, const Vec2& pos, const Vec2& scale);
};

#endif /* __PF_JELLO_MODEL_H__ */
