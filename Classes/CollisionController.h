#ifndef __COLLISION_CONTROLLER_H__
#define __COLLISION_CONTROLLER_H__

//#include "cocos2d.h"
//#include <vector>
//#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <unordered_set>
#include <cornell/CUSimpleObstacle.h>
#include <cornell/CUBoxObstacle.h>
#include <cornell/CUCapsuleObstacle.h>

#define MAX_V_TO_CRUSH -1.0f

// Forward declarations in the project
class LevelModel;
class LevelController;
class PineappleModel;
class KidModel;
class JelloModel;
class CrushableModel;
class ButtonSwitchModel;

using namespace cocos2d;
using namespace std;

class CollisionController {
protected:
	LevelModel* _level;
	LevelController* _levelCtrlr;
	/** Mark set to handle more sophisticated collision callbacks */
	unordered_set<b2Fixture*> _pSensorFixtures;
#pragma mark -
#pragma mark Collision Handling

	/**
	 * grounds will for jump enabling
	 */
	void ground(PineappleModel * will, b2Fixture* fix, BoxObstacle *ground);

	/**
	* Applies the jello force to william if he's small or crushes the jello if he's big.
	* This method is called when will collides with a jello
	* to trigger upward momentum, and a jello quiver animation
	*/
	void handleJelloCollision(PineappleModel* will, JelloModel* jello);

	/**
	* Applies the jello force to the given kid.
	* This method is called when a kid collides with a jello
	* to trigger upward momentum, and a jello quiver animation
	*/
	void handleJelloCollision(KidModel* kid, JelloModel* jello);

	/**
	 * Triggers blending of will
	 */
	void handleBlenderCollision(PineappleModel* will);

	/**
	 * Triggers blending of kid
	 */
	void handleBlenderCollision(KidModel* kid);
	
	/**
	* Triggers death of will
	*/
	void handleBlenderBladeCollision(PineappleModel* will);

	/**
	* Triggers death of kid
	*/
	void handleBlenderBladeCollision(KidModel* kid);

	/**
	* Kills will and triggers any associated death-by-spikes animations and sound
	*/
	void handleSpikeCollision(PineappleModel* will);
	/**
	* Kills kid and triggers any associated death-by-spikes animations and sound
	*/
	void handleSpikeCollision(KidModel* kid);
	/**
	 * Crushes cup if will is large and above it
	 */
	void handleCupCollision(PineappleModel* will, CrushableModel* cup);
    
    /** Turns button on, toggles switch */
    void handleButtonSwitchStartCollision(PineappleModel* will, ButtonSwitchModel* buttonSwitch);
    
    /** Turns button off */
    void handleButtonSwitchEndCollision(PineappleModel* will, ButtonSwitchModel* buttonSwitch);

	/**
	 * Helper function to determine if an object is in a position to be crushed by a large Will
	 */
	bool isBelowChar(BoxObstacle * obj, CapsuleObstacle * character);

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
    
    void reset() { _pSensorFixtures.clear(); }

#pragma mark -
#pragma mark Allocation
	/**
	* 
	*/
	static CollisionController* create();

	void setLevel(LevelModel * level);

private:
	/**
	* Creates a new game world with the default values.
	*
	* This constructor does not allocate any objects or start the controller.
	* This allows us to use a controller without a heap pointer.
	*/
	CollisionController();

};

#endif