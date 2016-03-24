//
//  BlenderModel.h
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
/** The maximum character speed */
#define BLENDER_SPEED   1.0f
/** The Blender specific scaling */
#define BLENDER_SCALE      0.75f

#define BLENDER_MASK 0x0008
#define BLENDER_COLLIDES_WITH 0x006 //Only kid and pineapple

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
     * Returns the upper limit on blender left-right movement.
     *
     * This does NOT apply to vertical movement.
     *
     * @return the upper limit on blender left-right movement.
     */
    float getSpeed() const { return BLENDER_SPEED; }
    
    
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
