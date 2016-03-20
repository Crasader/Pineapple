#ifndef __COLLISION_CONTROLLER_H__
#define __COLLISION_CONTROLLER_H__

#include "cocos2d.h"
#include <vector>
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <Box2D/Dynamics/Joints/b2MouseJoint.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <unordered_set>
#include <cornell/CUSimpleObstacle.h>

// Forward declarations in the project
class Level;
class LevelController;
class Pineapple;
class KidModel;

using namespace std;

class CollisionController {
protected:
	Level* _level;
	LevelController* _levelCtrlr;
	/** Mark set to handle more sophisticated collision callbacks */
	unordered_set<b2Fixture*> _sensorFixtures;
#pragma mark -
#pragma mark Collision Handling
	/**
	* Applies the jello force to the given pineapple.
	* This method is called when will collides with a jello
	* to trigger upward momentum, and a jello quiver animation
	*/
	void handleJelloCollision(Pineapple* will);

	/**
	* Applies the jello force to the given kid.
	* This method is called when a kid collides with a jello
	* to trigger upward momentum, and a jello quiver animation
	*/
	void handleJelloCollision(KidModel* kid);

	/**
	* Kills the given player or child.
	* This method is called when Will or one of his kids collides with a spike,
	* to trigger any blending animation and remove the given object from the world
	*
	* This method shouldn't do any checks for gameover, that should be handled elsewhere
	*
	* If necesarry to enable different animations this can be separated into separate funcs for
	* kid/pineapple
	*/
	void handleSpikeCollision(SimpleObstacle* pineappleOrKid);

public:
	/**
	* Processes the start of a collision
	*
	* This method is called when we first get a collision between two objects.  We use
	* this method to test if it is the "right" kind of collision.  In particular, we
	* use it to test if we make it to the win door.  We also us it to eliminate bullets.
	*
	* @param  contact  The two bodies that collided
	*/
	void beginContact(b2Contact* contact);

	/**
	* Processes the end of a collision
	*
	* This method is called when we no longer have a collision between two objects.
	* We use this method allow the character to jump again.
	*
	* @param  contact  The two bodies that collided
	*/
	void endContact(b2Contact* contact);

#pragma mark -
#pragma mark Allocation
	/**
	* Creates a new game world with the default values.
	*
	* This constructor does not allocate any objects or start the controller.
	* This allows us to use a controller without a heap pointer.
	*/
	CollisionController();

};

#endif