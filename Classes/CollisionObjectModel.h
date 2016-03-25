#ifndef __COLLISION_OBJECT_H__
#define __COLLISION_OBJECT_H__

#include <cornell.h>

using namespace cocos2d;

class CollisionObjectModel : public SimpleObstacle {

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

	/**
	 *	Returns identifying key for the object class, for use by the collision controller
	 */
	virtual char* getCollisionClass() = 0;

#pragma mark Static Constructors
	/**
	* Creates a new spike at the origin.
	*
	* The spike is scaled so that 1 pixel = 1 Box2d unit
	*
	* The scene graph is completely decoupled from the physics system.
	* The node does not have to be the same size as the physics body. We
	* only guarantee that the scene graph node is positioned correctly
	* according to the drawing scale.
	*
	* @return  An autoreleased physics object
	*/
	static CollisionObjectModel* baseCreate();

	/**
	* Creates a new spike at the given position.
	*
	* The spike is scaled so that 1 pixel = 1 Box2d unit
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
	static CollisionObjectModel* baseCreate(const Vec2& pos);

	/**
	* Creates a new spike at the given position.
	*
	* The spike is sized according to the given drawing scale.
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
	static CollisionObjectModel* baseCreate(const Vec2& pos, const Vec2& scale);

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
	* Applies the force to the body of this spike
	*
	* This method should be called after the force attribute is set.
	*/
	void applyForce();
};

#endif