//
//  KidModel.hpp
//  Pineapple
//
//  Created by Michael Patashnik on 2/24/16.
//
//

#ifndef KidModel_h
#define KidModel_h

#include <stdio.h>
//#include <cornell/CUBoxObstacle.h>
#include <cornell/CUCapsuleObstacle.h>
#include <cornell/CUWireNode.h>

using namespace cocos2d;

/** The texture for the first kid */
#define KID_TEXTURE_1    "pineapplet_bow"
/** The texture for the first kid */
#define KID_TEXTURE_2    "pineapplet_glasses"
/** The texture for the first kid */
#define KID_TEXTURE_3    "pineapplet_hat"
/** The texture for the first kid */
#define KID_TEXTURE_4    "pineapplet_pirate"

/** Identifier to allow us to track the sensor in ContactListener */
#define KID_SENSOR     "kidsensor"

#define KID_NAME       "kid"

#pragma mark -
#pragma mark Physics Constants
/** The amount to shrink the whole body, including image */
#define KID_SCALE 0.075f
/** The kid walking speed */
#define KID_WALKSPEED   1.5f

#pragma mark -
#pragma mark Dude Model
/**
 * Player avatar for the plaform game.
 *
 * Note that this class uses a capsule shape, not a rectangular shape.  In our
 * experience, using a rectangular shape for a character will regularly snag
 * on a platform.  The round shapes on the end caps lead to smoother movement.
 */
class KidModel : public CapsuleObstacle {
private:
    /** This macro disables the copy constructor (not allowed on physics objects) */
    CC_DISALLOW_COPY_AND_ASSIGN(KidModel);

protected:
    /** The index of this kid in the array of kids */
    float _index;
    /** The current horizontal movement of the character */
    float _movement;
    /** Whether our feet are on the ground */
    bool _isGrounded;
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
    
    /**
     * Redraws the outline of the physics fixtures to the debug node
     *
     * The debug node is use to outline the8 fixtures attached to this object.
     * This is very useful when the fixtures have a very different shape than
     * the texture (e.g. a circular shape attached to a square texture).
     */
    virtual void resetDebugNode() override;
    
public:
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
    
#pragma mark Attribute Properties
    /**
     * Returns the index of this kid in the kid array
     */
    int getIndex() const { return _index; }
    
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
     * Returns true if the dude is on the ground.
     *
     * @return true if the dude is on the ground.
     */
    bool isGrounded() const { return _isGrounded; }
    
    /**
     * Sets whether the dude is on the ground.
     *
     * @param value whether the dude is on the ground.
     */
    void setGrounded(bool value) { _isGrounded = value; }
    
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
    
    
CC_CONSTRUCTOR_ACCESS:
#pragma mark Hidden Constructors
    /**
     * Creates a degenerate Dude object.
     *
     * This constructor does not initialize any of the dude values beyond
     * the defaults.  To use a DudeModel, you must call init().
     */
    KidModel() : CapsuleObstacle(), _sensorName(KID_SENSOR) { }
    
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
