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


using namespace cocos2d;

/** Identifier to allow us to track the sensor in ContactListener */
#define PINEAPPLE_SENSOR     "pineapple sensor"


#pragma mark -
#pragma mark Physics Constants
#define PINEAPPLE_DENSITY						0.5f
#define PINEAPPLE_GROWN_MASS				2.0f
#define PINEAPPLE_NORMAL_MASS				1.5f
#define PINEAPPLE_SHRUNK_MASS				1.0f
#define PINEAPPLE_DENSITY						0.5f
/** The amount to shrink the whole body, including image */
#define PINEAPPLE_SCALE							0.1f
/** The factor to multiply by the input */
#define PINEAPPLE_FORCE							50.0f
/** The amount to slow the character down */
#define PINEAPPLE_DAMPING						10.0f
/** The maximum character speed */
#define PINEAPPLE_MAXSPEED					5.0f
/** The maximum duration of pineapple size */
#define PINEAPPLE_MAX_SIZE_DURATION 3.0f
/** The relative size of enlarged pineapple */
#define PINEAPPLE_GROW_SCALE				1.5f
/** The relative size of smaller pineapple */
#define PINEAPPLE_SHRINK_SCALE			0.75f

#define PINEAPPLE_MASK 0x0002
#define PINEAPPLE_COLLIDES_WITH 0xFFFB //All but 0x0004

#pragma mark -
#pragma mark PineappleModel Model
/**
* William the Papa PineappleModel.
*/
class PineappleModel : public CapsuleObstacle {
private:
	/** This macro disables the copy constructor (not allowed on physics objects) */
	CC_DISALLOW_COPY_AND_ASSIGN(PineappleModel);

protected:
	/** The current horizontal movement of the character */
	float _movement;
	/** Which direction is the character facing */
	bool _faceRight;
	/** Whether Will is large or not */
	bool _isLarge = false;
	/** Whether Will is small or not */
	bool _isSmall = false;
	/** Size object to store William's current size */
	Size _normalSize = Size();
	/** Duration since last grow or shrink, 0 if currently normal size */
	float _durationSinceGrowOrShrink = 0.0f;
	/** How long until we can jump again */
	int  _jumpCooldown;
	/** Whether we are actively jumping */
	bool _isJumping;
	/** Whether we are currently colliding with a jello */
	bool _isCollidingWithJello;
	/** Whether our feet are on the ground */
	bool _isGrounded;
	/** Whether or not we have reached the goal */
	bool _reachedGoal;
	/** Ground sensor to represent our feet */
	b2Fixture*  _sensorFixture;
	/** Reference to the sensor name (since a constant cannot have a pointer) */
	std::string _sensorName;
	/** The node for debugging the sensor */
	WireNode* _sensorNode;
	/** Filmstrip for walkcycle animation */
	AnimationNode* _willWalkcycle;
	/** Frame counter for walkcycle animation */
	int _willWalkcycleFrame;

	/**
	* Redraws the outline of the physics fixtures to the debug node
	*
	* The debug node is use to outline the fixtures attached to this object.
	* This is very useful when the fixtures have a very different shape than
	* the texture (e.g. a circular shape attached to a square texture).
	*/
	virtual void resetDebugNode() override;

public:
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

	/**
	* TODO: method spec
	*/
	void toNormalSize() { setDimension(_normalSize); }

	/**
	* TODO: method spec
	*/
	bool isLarge() const { return _isLarge; }

	/**
	* Setter for _isLarge
	*/
	void setIsLarge(bool isLarge) {
		_durationSinceGrowOrShrink = 0.0f;
		_isLarge = isLarge;
	}

	/**
	* Grows the pineapple, returns 1 if grown
	*/
	int grow();

	/**
	* Shrinks the pineapple, returns 1 if shrunk
	*/
	int shrink();

	/**
	* TODO: method spec
	*/
	bool isSmall() const { return _isSmall; }

	/**
	* Setter for _isSmall
	*/
	void setIsSmall(bool isSmall) {
		this->_durationSinceGrowOrShrink = 0.0f;
		this->_isSmall = isSmall;
	}

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
	* Sets whether the pineapple is currently colliding with a jello
	*
	* @param value whether the pineapple is currently colliding with a jello
	*/
	void setCollidingWithJello(bool value) { _isCollidingWithJello = value; }

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
	void setGrounded(bool value) { _isGrounded = value; }

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

	/**
	* Returns true if this character is facing right
	*
	* @return true if this character is facing right
	*/
	bool isFacingRight() const { return _faceRight; }

	/**
	* Initialize the filmstrip for walking animation
	*/
    void initAnimation(Texture2D* image, float scale);

    
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


CC_CONSTRUCTOR_ACCESS:
#pragma mark Hidden Constructors
	/**
	* Creates a degenerate pineapple object.
	*
	* This constructor does not initialize any of the pineapple values beyond
	* the defaults.  To use a PineappleModel, you must call init().
	*/
	PineappleModel() : CapsuleObstacle(), _sensorFixture(nullptr), _sensorName(PINEAPPLE_SENSOR) { }
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
	virtual bool init() override { return init(Vec2::ZERO, Vec2::ONE); }

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
	virtual bool init(const Vec2& pos) override { return init(pos, Vec2::ONE); }

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
