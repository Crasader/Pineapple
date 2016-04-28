#ifndef __CRUSHABLE_MODEL_H__
#define __CRUSHABLE_MODEL_H__
#include <cornell/CUBoxObstacle.h>
#include <cornell/CUWireNode.h>
#include <cornell/CUAnimationNode.h>
#include "Const.h"

using namespace cocos2d;

#pragma mark -
#pragma mark Physics Constants
/** The Crushable specific scaling */
#define CRUSHABLE_SCALE     0.75f
#define CUP_SMASH_FRAMES	6

#pragma mark -
#pragma mark Drawing Constants

#define CUP_NAME						"cup"

#define RED_CUP_ID				0
#define BLUE_CUP_ID				1
#define GREEN_CUP_ID			2
#define CUPSTACK_ID				3


#pragma mark -
#pragma mark CRUSHABLE Model
/**
* Player avatar for the plaform game.
*
* Note that this class uses a capsule shape, not a rectangular shape.  In our
* experience, using a rectangular shape for a character will regularly snag
* on a platform.  The round shapes on the end caps lead to smoother movement.
*/
class CrushableModel : public BoxObstacle/*, public CollisionObjectModel*/ {
private:
	/** This macro disables the copy constructor (not allowed on physics objects) */
	CC_DISALLOW_COPY_AND_ASSIGN(CrushableModel);

protected:
	/**
	* Redraws the outline of the physics fixtures to the debug node
	*
	* The debug node is use to outline the fixtures attached to this object.
	* This is very useful when the fixtures have a very different shape than
	* the texture (e.g. a circular shape attached to a square texture).
	*/
	virtual void resetDebugNode() override;
    
    /** The name of this texture that will be attached to the crushableObject */
    const char* _textureName;
	/** Whether the cup smashing animation is happening */
	bool _smashing;
	/** Whether the cup has been smashed */
	bool _smashed;
	/** Filmstrip for smashing animation */
	AnimationNode* _smashCycle;
	/** Frame counter for smashing animation */
	float _smashCycleFrame;
	
    /** The x coordinate for this crushable in the tiled level */
    int _tiledXCoord;
    /** The y coordinate for this crushable in the tiled level */
    int _tiledYCoord;    
    /** True iff this has had its position moved down because it is in a stack that should be moved down */
    bool _movedDown;


public:
	/**
	*	returns collision class
	*/
	int getCollisionClass() { return CUP_C; };
    
    /** Returns the x coordinate for this crushable in the tiled level */
    int getTiledXCoord() { return _tiledXCoord; }
    
    /** Returns the y coordinate for this crushable in the tiled level */
    int getTiledYCoord() { return _tiledYCoord; }
    
    /** True iff this has had its position moved down because it is in a stack that should be moved down */
    bool isMovedDown() { return _movedDown; }
    
    /** Sets moved down to m,
     * True iff this has had its position moved down because it is in a stack that should be moved down */
    void setMovedDown(bool m) {
        if (_movedDown == m) return;
        
        _movedDown = m;
        
        if (_movedDown) {
            setPosition(getPosition() - Vec2(0, getHeight()/2));
        } else {
            setPosition(getPosition() + Vec2(0, getHeight()/2));
        }
    }
    
    /** Return the name of the texture that should be used for this */
    const char* getTextureName() { return _textureName; }
    
#pragma mark Static Constructors
	/**
	* Creates a new crushable at the origin.
	*
	* The crushable is scaled so that 1 pixel = 1 Box2d unit
	*
	* The scene graph is completely decoupled from the physics system.
	* The node does not have to be the same size as the physics body. We
	* only guarantee that the scene graph node is positioned correctly
	* according to the drawing scale.
	*
	* @return  An autoreleased physics object
	*/
	static CrushableModel* create(const char* texture);

	/**
	* Creates a new crushable at the given position.
	*
	* The crushable is scaled so that 1 pixel = 1 Box2d unit
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
	static CrushableModel* create(const char* texture, int x, int y, const Vec2& pos);

	/**
	* Creates a new crushable at the given position.
	*
	* The crushable is sized according to the given drawing scale.
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
	static CrushableModel* create(const char* texture, int x, int y, const Vec2& pos, const Vec2& scale);

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
	* Applies the force to the body of this crushable
	*
	* This method should be called after the force attribute is set.
	*/
	void applyForce();

	/** 
	* Returns true if the cup has been smashed
	*/
	bool getSmashed() { return _smashed; }

	/**
	* Sets whether the cup smashing animation is happening
	*/
	void setSmashing(bool smash) { _smashing = smash; }


#pragma mark Drawing Methods
    /**
     * Performs any necessary additions to the scene graph node.
     *
     * This method is necessary for custom physics objects that are composed
     * of multiple scene graph nodes.  In this case, it is because we
     * manage our own afterburner animations.
     */
    virtual void resetSceneNode() override;

	/**
	 * Animate the cup smashing and set _smashed to true if animation is done
	 */
	void animate();
    

CC_CONSTRUCTOR_ACCESS:
#pragma mark Hidden Constructors
	/**
	* Creates a degenerate Crushable object.
	*
	* This constructor does not initialize any of the crushable values beyond
	* the defaults.  To use a CrushableModel, you must call init().
	*/
	CrushableModel() : BoxObstacle() { }

	/**
	* Initializes a new crushable at the origin.
	*
	* The crushable is scaled so that 1 pixel = 1 Box2d unit
	*
	* The scene graph is completely decoupled from the physics system.
	* The node does not have to be the same size as the physics body. We
	* only guarantee that the scene graph node is positioned correctly
	* according to the drawing scale.
	*
	* @return  true if the obstacle is initialized properly, false otherwise.
	*/
	virtual bool init(const char* texture) { return init(texture, 0, 0, Vec2::ZERO, Vec2::ONE); }

	/**
	* Initializes a new crushable at the given position.
	*
	* The crushable is scaled so that 1 pixel = 1 Box2d unit
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
	virtual bool init(const char* texture, int x, int y, const Vec2& pos) { return init(texture, x, y, pos, Vec2::ONE); }

	/**
	* Initializes a new crushable at the given position.
	*
	* The crushable is sized according to the given drawing scale.
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
	virtual bool init(const char* texture, int x, int y, const Vec2& pos, const Vec2& scale);
};

#endif /* __PF_JELLO_MODEL_H__ */