//
//  PineappleModel.h
//  G.M.P.
//
//  Author: Flameingos
//  Version: TODO
//
#ifndef __PINEAPPLE_H__
#define __PINEAPPLE_H__
#include <cornell/CUCapsuleObstacle.h>
#include <cornell/CUWireNode.h>
#include <cornell/CUAnimationNode.h>
#include "Const.h"
#include "Texture.h"
//#include "CollisionObjectModel.h"


using namespace cocos2d;

/** Identifier to allow us to track the sensor in ContactListener */
#define PINEAPPLE_SENSOR     "pineapple sensor"
#define PINEAPPLE_BODY_SENSOR "pineapple body sensor"

#pragma mark -
#pragma mark Physics Constants
#define PINEAPPLE_DENSITY					   0.5f
#define PINEAPPLE_NORMAL_MASS				   2.0f
#define PINEAPPLE_SHRUNK_MASS				   1.0f
/** The amount to shrink the whole body, including image */
#define PINEAPPLE_SCALE						   0.6f
/** The factor to multiply by the input */
#define PINEAPPLE_FORCE						   50.0f
/** The amount to slow the character down */
#define PINEAPPLE_DAMPING					   10.0f
/** The maximum character speed */
#define PINEAPPLE_MAXSPEED					   5.0f
/** The relative size of smaller pineapple */
#define PINEAPPLE_SHRINK_SCALE			       0.50f

/** Number of frames in the walk animation */
#define PINEAPPLE_FRAME_COUNT	26

#define PINEAPPLE_MASK 0x0002
#define PINEAPPLE_COLLIDES_WITH 0xFFFB //All but 0x0004

#pragma mark -
#pragma mark PineappleModel Model
/**
* William the Papa PineappleModel.
*/
class PineappleModel : public CapsuleObstacle/*, public CollisionObjectModel*/ {
private:
	/** This macro disables the copy constructor (not allowed on physics objects) */
	CC_DISALLOW_COPY_AND_ASSIGN(PineappleModel);

protected:
	/** The current horizontal movement of the character */
	float _movement;
	/** Which direction is the character facing */
	bool _faceRight;
	/** Whether Will is large or not */
	bool _isSmall = false;
    bool _justGrewShrank = false;
    int _framesJustGrewShrank = 0;
	/** Size object to store William's current size */
	Size _normalSize = Size();
	/** How long until we can jump again */
	int  _jumpCooldown;
	/** Whether we are actively jumping */
	bool _isJumping;
	/** Whether we are currently colliding with a jello */
	bool _isCollidingWithJello;
    /** Whether we are currently colliding with a buttonswitch */
    bool _isCollidingWithButtonSwitch;
	/** Whether our feet are on the ground */
	bool _isGrounded;
	/** Whether or not we have reached the goal */
	bool _reachedGoal;
	/** Ground sensor to represent our feet */
	b2Fixture*  _sensorFixture;
    /** Sensor to represent our body, used for lever collisions */
    b2Fixture*  _sensorBodyFixture;
	/** Reference to the sensor name (since a constant cannot have a pointer) */
	std::string _sensorName;
    std::string _sensorBodyName;
	/** The node for debugging the sensor */
	WireNode* _sensorNode;
	/** Filmstrip for walkcycle animation */
	AnimationNode* _willWalkcycle;
	/** Frame counter for walkcycle animation */
	int _willWalkcycleFrame;
	/** Whether Will is spiraling towards blender blades */
	bool _isBlended;
	/** Whether Will has been blended */
	bool _isDead;


	/**
	* Redraws the outline of the physics fixtures to the debug node
	*
	* The debug node is use to outline the fixtures attached to this object.
	* This is very useful when the fixtures have a very different shape than
	* the texture (e.g. a circular shape attached to a square texture).
	*/
	virtual void resetDebugNode() override;

public:

	/**
	*	returns collision class
	*/
	int getCollisionClass() override { return PINEAPPLE_C; };
    const b2Fixture* const getCore() { return _core; }
    

#pragma mark Static Constructors
	/**
	* Creates a new pineapple at the origin.
	*
	* The pineapple is scaled so that 1 pixel = 1 Box2d unit
	*
	* The scene graph is completely decoupled from the physics system.
	* The node does not have to be the same size as the physics body. We
	* only guarantee that the scene graph node is positioned correctly
	* according to the drawing scale.
	*
	* @return  An autoreleased physics object
	*/
	static PineappleModel* create();

	/**
	* Creates a new pineapple at the given position.
	*
	* The pineapple is scaled so that 1 pixel = 1 Box2d unit
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
	static PineappleModel* create(const Vec2& pos);

	/**
	* Creates a new pineapple at the given position.
	*
	* The pineapple is sized according to the given drawing scale.
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
	static PineappleModel* create(const Vec2& pos, const Vec2& scale);

#pragma mark Attribute Properties

	/**
	* TODO: method spec
	*/
	int updateSize(float dt);

	///**
	//* TODO: method spec
	//*/
	//void toNormalSize() { setDimension(_normalSize); }

	/**
	* TODO: method spec
	*/
	bool isSmall() const { return _isSmall; }

	/**
	* Setter for _isLarge
	*/
	void setIsSmall(bool isSmall) {
		_isSmall = isSmall;
	}

    /**
     * Grows the pineapple, returns 1 if now normal size, 0 if already normal
     */
    int grow();
    
    /**
     * Shrinks the pineapple, returns 1 if now small size, 0 if already small
     */
    int shrink();
    
    bool justGrewShrank() { return _justGrewShrank; }

	/**
	* Returns left/right movement of this character.
	*
	* This is the result of input times pineapple force.
	*
	* @return left/right movement of this character.
	*/
	float getMovement() const { return _movement; }

	/**
	* Sets left/right movement of this character.
	*
	* This is the result of input times pineapple force.
	*
	* @param value left/right movement of this character.
	*/
	void setMovement(float value);

	/**
	* Returns true if the pineapple is actively jumping.
	*
	* @return true if the pineapple is actively jumping.
	*/
	bool isJumping() const { return _isJumping && _jumpCooldown <= 0; }

	/**
	* Sets whether the pineapple is actively jumping.
	*
	* @param value whether the pineapple is actively jumping.
	*/
	void setJumping(bool value) { _isJumping = value; }

	/**
	* Returns true if the pineapple is actively colliding with jello
	*
	* @return true if the pineapple is actively colliding with jello
	*/
	bool isCollidingWithJello() { return _isCollidingWithJello; }
    
    /**
     * Returns true if the pineapple is actively colliding with buttonSwitch
     *
     * @return true if the pineapple is actively colliding with buttonSwitch
     */
    bool isCollidingWithButtonSwitch() { return _isCollidingWithButtonSwitch; }

	/**
	* Sets whether the pineapple is currently colliding with a jello
	*
	* @param value whether the pineapple is currently colliding with a jello
	*/
	void setCollidingWithJello(bool value) { _isCollidingWithJello = value; }
    
    /**
     * Sets whether the pineapple is currently colliding with a jello
     *
     * @param value whether the pineapple is currently colliding with a jello
     */
    void setCollidingWithButtonSwitch(bool value) { _isCollidingWithButtonSwitch = value; }
    
	/**
	* Returns true if the pineapple is on the ground.
	*
	* @return true if the pineapple is on the ground.
	*/
	bool isGrounded() const { return _isGrounded; }

	/**
	* Sets whether the pineapple is on the ground.
	*
	* @param value whether the pineapple is on the ground.
	*/
	void setGrounded(bool value) {
        _isGrounded = value;
    }

	/**
	* Returns true if Will has reached the goal.
	*
	* @return true if Will has reached the goal.
	*/
	bool hasReachedGoal() const { return _reachedGoal; }

	/**
	* Sets whether Will has reached the goal.
	*
	* @param value whether Will has reached the goal.
	*/
	void setReachedGoal(bool value) { _reachedGoal = value; }

	/**
	* Returns how much force to apply to get the pineapple moving
	*
	* Multiply this by the input to get the movement value.
	*
	* @return how much force to apply to get the pineapple moving
	*/
	float getForce() const { return PINEAPPLE_FORCE; }

	/**
	* Returns ow hard the brakes are applied to get a pineapple to stop moving
	*
	* @return ow hard the brakes are applied to get a pineapple to stop moving
	*/
	float getDamping() const { return PINEAPPLE_DAMPING; }

	/**
	* Returns the upper limit on pineapple left-right movement.
	*
	* This does NOT apply to vertical movement.
	*
	* @return the upper limit on pineapple left-right movement.
	*/
	float getMaxSpeed() const { return PINEAPPLE_MAXSPEED; }

	/**
	* Returns the name of the ground sensor
	*
	* This is used by ContactListener
	*
	* @return the name of the ground sensor
	*/
	std::string* getSensorName() { return &_sensorName; }
    
    std::string* getSensorBodyName() { return &_sensorBodyName; }

	/**
	* Returns true if this character is facing right
	*
	* @return true if this character is facing right
	*/
	bool isFacingRight() const { return _faceRight; }

	/**
	* Sets whether will is spiralling towards the blender blades
	*/
	void setIsBlended(bool blending) { _isBlended = blending; }

	/**
	* Returns true if will is spiralling towards the blender blades
	*/
	bool getIsBlended() { return _isBlended; }

	/**
	* Sets whether will is dead
	*/
	void setIsDead(bool dead) { _isDead = dead; }

	/**
	* Returns true if will is dead
	*/
	bool getIsDead() { return _isDead; }

    
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
	* Applies the force to the body of this pineapple
	*
	* This method should be called after the force attribute is set.
	*/
	void applyForce();

	/**
	* Animate Will if he's moving
	*/
	void animate();

	/** 
	* Make Will spiral towards blender blades
	*
	* @param x x-coordinate of the blender blades
	* @param y y-coordinate of the blender
	*/
	void spiral(float x, float y);


CC_CONSTRUCTOR_ACCESS:
#pragma mark Hidden Constructors
	/**
	* Creates a degenerate pineapple object.
	*
	* This constructor does not initialize any of the pineapple values beyond
	* the defaults.  To use a PineappleModel, you must call init().
	*/
	PineappleModel() : CapsuleObstacle(),
    _isGrounded(true),
    _sensorFixture(nullptr),
    _sensorBodyFixture(nullptr),
    _sensorName(PINEAPPLE_SENSOR),
    _sensorBodyName(PINEAPPLE_BODY_SENSOR),
    _willWalkcycle(nullptr),
    _jumpCooldown(0){ }
	~PineappleModel() { }

	/**
	* Initializes a new pineapple at the origin.
	*
	* The pineapple is scaled so that 1 pixel = 1 Box2d unit
	*
	* The scene graph is completely decoupled from the physics system.
	* The node does not have to be the same size as the physics body. We
	* only guarantee that the scene graph node is positioned correctly
	* according to the drawing scale.
	*
	* @return  true if the obstacle is initialized properly, false otherwise.
	*/
	virtual bool init() override { return init(Vec2::ZERO, DEFAULT_DRAW_SCALE); }

	/**
	* Initializes a new pineapple at the given position.
	*
	* The pineapple is scaled so that 1 pixel = 1 Box2d unit
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
	virtual bool init(const Vec2& pos) override { return init(pos, DEFAULT_DRAW_SCALE); }

	/**
	* Initializes a new pineapple at the given position.
	*
	* The pineapple is sized according to the given drawing scale.
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
	virtual bool init(const Vec2& pos, const Vec2& scale);
};

#endif /* __PINEAPPLE_H__ */
