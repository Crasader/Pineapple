//
//  PFBlenderModel.h
//  G.M.P.
//
//  Author: Flameingos
//  Version: TODO
//
#ifndef __PF_BLENDER_MODEL_H__
#define __PF_BLENDER_MODEL_H__
#include <cornell/CUBoxObstacle.h>
#include <cornell/CUWireNode.h>


using namespace cocos2d;
 
#pragma mark -
#pragma mark Drawing Constants
/** The texture for the character avatar */
#define BLENDER_TEXTURE    "blender"

#pragma mark -
#pragma mark Physics Constants
/** The factor to multiply by the input */
#define BLENDER_FORCE      50.0f
/** The amount to slow the character down */
#define BLENDER_DAMPING    10.0f
/** The maximum character speed */
#define BLENDER_MAXSPEED   0.5f
/** The Blender specific scaling */
#define BLENDER_SCALE      0.75f


#pragma mark -
#pragma mark BLENDER Model
/**
 * Player avatar for the plaform game.
 *
 * Note that this class uses a capsule shape, not a rectangular shape.  In our
 * experience, using a rectangular shape for a character will regularly snag
 * on a platform.  The round shapes on the end caps lead to smoother movement.
 */
class BlenderModel : public BoxObstacle {
private:
    /** This macro disables the copy constructor (not allowed on physics objects) */
    CC_DISALLOW_COPY_AND_ASSIGN(BlenderModel);

protected:
    /** The current horizontal movement of the character */
    float _movement;
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
     * Creates a new blender at the origin.
     *
     * The blender is scaled so that 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @return  An autoreleased physics object
     */
    static BlenderModel* create();
    
    /**
     * Creates a new blender at the given position.
     *
     * The blender is scaled so that 1 pixel = 1 Box2d unit
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
    static BlenderModel* create(const Vec2& pos);
    
    /**
     * Creates a new blender at the given position.
     *
     * The blender is sized according to the given drawing scale.
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
    static BlenderModel* create(const Vec2& pos, const Vec2& scale);

    
#pragma mark Attribute Properties
    /**
     * Returns left/right movement of this character.
     *
     * This is the result of input times blender force.
     *
     * @return left/right movement of this character.
     */
    float getMovement() const { return _movement; }
    
    /**
     * Sets left/right movement of this character.
     *
     * This is the result of input times blender force.
     *
     * @param value left/right movement of this character.
     */
    void setMovement(float value);
    
    /**
     * Returns how much force to apply to get the blender moving
     *
     * Multiply this by the input to get the movement value.
     *
     * @return how much force to apply to get the blender moving
     */
    float getForce() const { return BLENDER_FORCE; }
    
    /**
     * Returns ow hard the brakes are applied to get a blender to stop moving
     *
     * @return ow hard the brakes are applied to get a blender to stop moving
     */
    float getDamping() const { return BLENDER_DAMPING; }
    
    /**
     * Returns the upper limit on blender left-right movement.
     *
     * This does NOT apply to vertical movement.
     *
     * @return the upper limit on blender left-right movement.
     */
    float getMaxSpeed() const { return BLENDER_MAXSPEED; }
    
    
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
     * Applies the force to the body of this blender
     *
     * This method should be called after the force attribute is set.
     */
    void applyForce();

    
CC_CONSTRUCTOR_ACCESS:
#pragma mark Hidden Constructors
    /**
     * Creates a degenerate Blender object.
     *
     * This constructor does not initialize any of the blender values beyond
     * the defaults.  To use a BlenderModel, you must call init().
     */
    BlenderModel() : BoxObstacle() { }

    /**
     * Initializes a new blender at the origin.
     *
     * The blender is scaled so that 1 pixel = 1 Box2d unit
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
     * Initializes a new blender at the given position.
     *
     * The blender is scaled so that 1 pixel = 1 Box2d unit
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
     * Initializes a new blender at the given position.
     *
     * The blender is sized according to the given drawing scale.
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

#endif /* __PF_BLENDER_MODEL_H__ */
