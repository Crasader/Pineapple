#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "Pineapple.h"
#include "PFBlenderModel.h"
#include "KidModel.h"
#include "SpikeModel.h"

#define KID_COUNT 4

class Level {
protected:
	/** Reference to the goalDoor (for collision detection) */
	BoxObstacle*    _goalDoor;
	/** Reference to the player avatar */
	Pineapple*      _pineapple;
	/** References to the kid avatars */
	KidModel**     _kids;
	/** Reference to the blender avatar */
	BlenderModel* _blender;
	/** Reference to the physics root of the scene graph TODO: is this shared across levels? */
	Node* _worldnode;
	/** Reference to the debug root of the scene graph TODO: is this shared across levels? */
	Node* _debugnode;

	/** Count of remaining kids */
	int _kidsRemaining;
	/** Whether we have failed at this level (and need a reset) */
	bool _failed;

public:
#pragma mark -
#pragma mark State Access
	/**
	 *
	 */
	BoxObstacle* getGoal() { return _goalDoor; }

	/**
	*
	*/
	Pineapple* getPineapple() { return _pineapple; }

	/**
	*
	*/
	Pineapple* clearPineapple() { _pineapple = nullptr; }

	/**
	*
	*/
	KidModel* getKid(int i) { return _kids[i]; }

	/**
	*
	*/
	Pineapple* clearKid(int i) { _kids[i] = nullptr; }


	/**
	*
	*/
	BlenderModel* getBlender() { return _blender; }

	/**
	*
	*/
	Node* getWorldNode() { return _worldnode; }

	/**
	*
	*/
	void setWorldNode(Node* node) { _worldnode = node; }

	/**
	*
	*/
	Node* getDebugNode() { return _debugnode; }

	/**
	*
	*/
	void setDebugNode(Node* node) { _debugnode = node; }

	/**
	*
	*/
	int numKidsRemaining() { return _kidsRemaining; }

	/**
	*
	*/
	void subtractKidFromCount() { _kidsRemaining--; }

	/**
	*
	*/
	bool haveFailed() { return _failed; }

	/**
	*
	*/
	void setFailure(bool value) { _failed = value; }

#pragma mark -
#pragma mark Allocation

	/**
	* shitty constructor to get collisioncontroller up and running
	*/
	Level();

	/**
	*
	*/
	bool init(BoxObstacle* goal, KidModel** kid, Pineapple* will, BlenderModel* blender, Node* debug, Node* world);

	/**
	*
	*/
	static Level* create(BoxObstacle* goal, KidModel** kid, Pineapple* will, BlenderModel* blender, Node* debug, Node* world);

	/**
	* Kills the given player or child.
	* This method is called when Will or one of his kids collides with the blender,
	* to trigger any blending animation and remove the given object from the world
	*
	* This method shouldn't do any checks for gameover, that should be handled elsewhere
	*
	* TODO: Putting this here for the sake of completing CollisionController, but should be moved to LevelController when it is made
	*				Since this is just here temporarily, the full implementation is in the header.
	*/
	void blendAndKill(SimpleObstacle* pineappleOrKid) {
		removeObstacle(pineappleOrKid);

		//TODO: Animation and sounds
	}

	/**
	* Removes obstacle from level world
	*
	* TODO: Putting this here for the sake of completing CollisionController, but should be moved to LevelController when it is made
	*				Since this is just here temporarily, the full implementation is in the header.
	*/
	void removeObstacle(Obstacle* obj) {
		_worldnode->removeChild(obj->getSceneNode());
		_debugnode->removeChild(obj->getDebugNode());
		obj->markRemoved(true);
	}


};

#endif