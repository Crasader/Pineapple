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
#include <cornell/CUBoxObstacle.h>
#include <cornell/CUCapsuleObstacle.h>
#include <cornell/CUWireNode.h>

using namespace cocos2d;

/** The texture for the character avatar */
#define KID_TEXTURE    "pineapple"
/** Identifier to allow us to track the sensor in ContactListener */
#define SENSOR_NAME     "kidsensor"

#pragma mark -
#pragma mark Physics Constants
/** The factor to multiply by the input */
#define KID_FORCE      20.0f
/** The amount to slow the character down */
#define KID_DAMPING    10.0f
/** The maximum character speed */
#define KID_MAXSPEED   5.0f

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
    /** The current horizontal movement of the character */
    float _movement;
    /** Whether our feet are on the ground */
    bool _isGrounded;
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
     *
     * @return  An autoreleased physics object
     */
    static KidModel* create(const Vec2& pos, const Vec2& scale);
    
    
#pragma mark Attribute Properties
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
     * Returns how much force to apply to get the dude moving
     *
     * Multiply this by the input to get the movement value.
     *
     * @return how much force to apply to get the dude moving
     */
    float getForce() const { return KID_FORCE; }
    
    /**
     * Returns ow hard the brakes are applied to get a dude to stop moving
     *
     * @return ow hard the brakes are applied to get a dude to stop moving
     */
    float getDamping() const { return KID_DAMPING; }
    
    /**
     * Returns the upper limit on dude left-right movement.
     *
     * This does NOT apply to vertical movement.
     *
     * @return the upper limit on dude left-right movement.
     */
    float getMaxSpeed() const { return KID_MAXSPEED; }
    
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
    void applyForce();
    
    
CC_CONSTRUCTOR_ACCESS:
#pragma mark Hidden Constructors
    /**
     * Creates a degenerate Dude object.
     *
     * This constructor does not initialize any of the dude values beyond
     * the defaults.  To use a DudeModel, you must call init().
     */
    KidModel() : CapsuleObstacle(), _sensorName(SENSOR_NAME) { }
    
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
    virtual bool init() override { return init(Vec2::ZERO, Vec2::ONE); }
    
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
    virtual bool init(const Vec2& pos) override { return init(pos, Vec2::ONE); }
    
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
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init(const Vec2& pos, const Vec2& scale);
};


#endif /* KidModel_h */
