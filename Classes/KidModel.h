//
//  KidModel.hpp
//  PineappleModel
//
//  Created by Michael Patashnik on 2/24/16.
//
//

#ifndef KidModel_h
#define KidModel_h

#include <stdio.h>
#include <cornell/CUCapsuleObstacle.h>
#include <cornell/CUWireNode.h>
#include <cornell/CUAnimationNode.h>
#include "Const.h"
#include "Texture.h"
//#include "CollisionObjectModel.h"

using namespace cocos2d;

/** Identifier to allow us to track the sensor in ContactListener */
#define KID_SENSOR     "kidsensor"

#pragma mark -
#pragma mark Physics Constants
/** The amount to shrink the whole body, including image */
#define KID_SCALE				0.3f
/** The kid walking speed */
#define KID_WALKSPEED			1.5f
/** The number of frames in the kids animation strip */
#define KID_ANIMATION_FRAMES	16

#define KID_MASK				0x0004
#define KID_COLLIDES_WITH		0xFFFD //All but 0x0002

#pragma mark -
#pragma mark Dude Model
/**
 * Player avatar for the plaform game.
 *
 * Note that this class uses a capsule shape, not a rectangular shape.  In our
 * experience, using a rectangular shape for a character will regularly snag
 * on a platform.  The round shapes on the end caps lead to smoother movement.
 */
class KidModel : public CapsuleObstacle/*, public CollisionObjectModel*/ {
private:
    /** This macro disables the copy constructor (not allowed on physics objects) */
    CC_DISALLOW_COPY_AND_ASSIGN(KidModel);

protected:
    /** The index of this kid in the array of kids */
    int _index;
    /** The current horizontal movement of the character */
    float _movement;
    /** Whether this kid is currently colliding with jello */
    bool _isCollidingWithJello;
	/** Whether or not this kid has reached the goal */
	bool _reachedGoal;
    /** Ground sensor to represent our feet */
    b2Fixture*  _sensorFixture;
    /** Reference to the sensor name (since a constant cannot have a pointer) */
    std::string _sensorName;
    /** The node for debugging the sensor */
    WireNode* _sensorNode;
	/** Filmstrip for walkcycle animation */
	AnimationNode* _kidWalkcycle;
	/** Frame counter for walkcycle animation */
	float _kidWalkcycleFrame;
	/** Whether kid is spiraling towards blender blades */
	bool _isBlended;
	/** Whether kid has been blended */
	bool _isDead;
    /** Whether kid is falling into knives */
	bool _isImpaled;
	/** Whether kid has been impaled */
	bool _isDead2;
	bool _tmp;
	bool _isGrounded = false;
    
    bool _gotPush = true;
    
    /**
     * Redraws the outline of the physics fixtures to the debug node
     *
     * The debug node is use to outline the8 fixtures attached to this object.
     * This is very useful when the fixtures have a very different shape than
     * the texture (e.g. a circular shape attached to a square texture).
     */
    virtual void resetDebugNode() override;
    
public:
	/**
	*	returns collision class
	*/
	int getCollisionClass() { return KID_C; };
#pragma mark Static Constructors
    /**
     * Creates a new dude at the origin.
     *
     * The dude is scaled so that 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @return  An autoreleased physics object
     */
    static KidModel* create();
    
    /**
     * Creates a new dude at the given position.
     *
     * The dude is scaled so that 1 pixel = 1 Box2d unit
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
    static KidModel* create(const Vec2& pos);
    
    /**
     * Creates a new dude at the given position.
     *
     * The dude is sized according to the given drawing scale.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos      Initial position in world coordinates
     * @param  idx      The index of this kid, for selecting texture, in range [0..NUM_KIDS)
     *
     * @return  An autoreleased physics object
     */
    static KidModel* create(const Vec2& pos, int idx);
    
    /**
     * Creates a new dude at the given position.
     *
     * The dude is sized according to the given drawing scale.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos      Initial position in world coordinates
     * @param  scale    The drawing scale
     * @param  idx      The index of this kid, for selecting texture, in range [0..NUM_KIDS)
     *
     * @return  An autoreleased physics object
     */
    static KidModel* create(const Vec2& pos, const Vec2& scale, int idx);
    
    /**
     * Returns the kid texture name for the given kid index
     */
    static std::string getTexture(int idx);

	/**
	* Returns the splat texture name for the given kid index
	*/
	static std::string getSplatTexture(int idx);
    
#pragma mark Attribute Properties
    /**
     * Returns the index of this kid in the kid array
     */
    int getIndex() const { return _index; }
    
    void setGrounded(bool value) { _isGrounded = value; }
    
    /**
     * Returns left/right movement of this character.
     *
     * This is the result of input times dude force.
     *
     * @return left/right movement of this character.
     */
    float getMovement() const { return _movement; }
    
    /**
     * Sets left/right movement of this character.
     *
     * This is the result of input times dude force.
     *
     * @param value left/right movement of this character.
     */
    void setMovement(float value);
    
    /**
     * Returns true if the dude is actively colliding with jello
     *
     * @return true if the dude is actively colliding with jello
     */
    bool isCollidingWithJello() {return _isCollidingWithJello; }
    
    /**
     * Sets whether the dude is currently colliding with a jello
     *
     * @param value whether the dude is currently colliding with a jello
     */
    void setCollidingWithJello(bool value) { _isCollidingWithJello = value; }
    
    /**
     * Returns how much force to apply to get the dude moving
     *
     * Multiply this by the input to get the movement value.
     *
     * @return how much force to apply to get the dude moving
     */

	 /**
	 * Returns true if the kid has reached the ground.
	 *
	 * @return true if the kid has reached the ground.
	 */
	bool hasReachedGoal() const { return _reachedGoal; }

	/**
	* Sets whether the kid has reached the goal.
	*
	* @param value whether the kid has reached the goal.
	*/
	void setReachedGoal(bool value) { _reachedGoal = value; }
    
    /**
     * Returns the upper limit on kid left-right movement while on the ground.
     *
     * This does NOT apply to vertical movement, or when in the air.
     *
     * @return the upper limit on dude left-right movement.
     */
    float getWalkingSpeed() const { return KID_WALKSPEED; }
    
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
    bool isFacingRight() const { return true; }

	/**
	 * Sets whether the kid is spiralling towards the blender blades 
	 */
	void setIsBlended(bool blending) { _isBlended = blending; }

	/**
	* Returns true if the kid is spiralling towards the blender blades
	*/
	bool getIsBlended() { return _isBlended; }

	/**
	* Sets whether the kid is dead
	*/
	void setIsDead(bool dead) { _isDead = dead; }

	/**
	* Returns true if the kid is dead (from blender)
	*/
	bool getIsDead() { return _isDead; }

	/**
	* Sets whether the kid is being impaled
	*/
	void setIsImpaled(bool impale) { _isImpaled = impale; }

	/**
	* Returns true if the kid is being impaled
	*/
	bool getIsImpaled() { return _isImpaled; }

	/**
	* Returns true if the kid is dead (from knives)
	*/
	bool getIsDead2() { return _isDead2; }
    
    
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
     * Applies the force to the body of this dude
     *
     * This method should be called after the force attribute is set.
     */
    void dampTowardsWalkspeed();
    
	/**
	* Animate the kid if they're moving
	*/
	void animate();

	/**
	* Animate death by knives
	*/
	void animate2();

	/**
	* Make the kid spiral towards blender blades
	*
	* @param x x-coordinate of the blender blades
	* @param y y-coordinate of the blender
	*/
	void spiral(float x, float y);

#pragma mark Drawing Methods
    /**
     * Performs any necessary additions to the scene graph node.
     *
     * This method is necessary for custom physics objects that are composed
     * of multiple scene graph nodes.  In this case, it is because we
     * manage our own afterburner animations.
     */
    virtual void resetSceneNode() override;
    
    
CC_CONSTRUCTOR_ACCESS:
#pragma mark Hidden Constructors
    /**
     * Creates a degenerate Dude object.
     *
     * This constructor does not initialize any of the dude values beyond
     * the defaults.  To use a DudeModel, you must call init().
     */
    KidModel() : CapsuleObstacle(), _sensorFixture(nullptr), _sensorName(KID_SENSOR) { }
    
    /**
     * Initializes a new dude at the origin.
     *
     * The dude is scaled so that 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init() override { return init(Vec2::ZERO, Vec2::ONE, 0); }
    
    /**
     * Initializes a new dude at the given position.
     *
     * The dude is scaled so that 1 pixel = 1 Box2d unit
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
    virtual bool init(const Vec2& pos) override { return init(pos, Vec2::ONE, 0); }
    
    /**
     * Initializes a new dude at the given position.
     *
     * The dude is scaled so that 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos      Initial position in world coordinates
     * @param  idx      The index of this kid, for selecting texture, in range [0..NUM_KIDS)
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init(const Vec2& pos, int idx) { return init(pos, Vec2::ONE, idx); }
    
    /**
     * Initializes a new dude at the given position.
     *
     * The dude is sized according to the given drawing scale.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos      Initial position in world coordinates
     * @param  scale    The drawing scale
     * @param  idx      The index of this kid, for selecting texture, in range [0..NUM_KIDS)
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init(const Vec2& pos, const Vec2& scale, int idx);
};


#endif /* KidModel_h */
