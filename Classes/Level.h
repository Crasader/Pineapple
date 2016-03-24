#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <cornell/CUBoxObstacle.h>
#include <cornell/CUSimpleObstacle.h>
#include <cornell/CUObstacle.h>
#include <cornell/CUPolygonObstacle.h>
#include <cornell/CURootLayer.h>
#include <cornell/CUWorldController.h>

#include "Pineapple.h"
#include "PFBlenderModel.h"
#include "KidModel.h"
#include "SpikeModel.h"
#include <cornell.h>

#define KID_COUNT 4
/** Color to outline the physics nodes */
#define DEBUG_COLOR     Color3B::YELLOW
/** Opacity of the physics outlines */
#define DEBUG_OPACITY   192

/** The density for most physics objects */
#define BASIC_DENSITY   0.0f
/** The density for a bullet */
#define HEAVY_DENSITY   10.0f
/** Friction of most platforms */
#define BASIC_FRICTION  0.4f
/** The restitution for all physics objects */
#define BASIC_RESTITUTION   0.1f

/** 
 *          Z index table:
 * /------------------------------------\
 * | 0-9    |   Behind walls            |
 * | 10-19  |   Walls and equivalent    |
 * | 20-29  |   Obstacles               |
 * | 30-40  |   Characters              |
 * \------------------------------------/
 */

#define GOAL_Z_INDEX            0
 
#define WALL_Z_INDEX            10
#define PLATFORM_Z_INDEX        11

#define CUP_Z_INDEX             20
#define JELLO_Z_INDEX           21
#define SPIKES_Z_INDEX          29

#define KID_Z_INDEX             30 //30...30+NUM_CHILDREN-1 used by Kids

#define PINEAPPLE_Z_INDEX       35
#define BLENDER_Z_INDEX         36

using namespace cocos2d;

class Level {
protected:
	/** Reference to the goalDoor (for collision detection) */
	BoxObstacle*    _goalDoor;
	/** Reference to the player avatar */
	Pineapple*      _pineapple;
	/** References to the kid avatars */
	KidModel*     _kids[KID_COUNT];
	/** Reference to the blender avatar */
	BlenderModel* _blender;
	/** Reference to the root node of the scene graph */
	RootLayer* _rootnode;
	/** Reference to the physics root of the scene graph */
	Node* _worldnode;
	/** Reference to the debug root of the scene graph */
	Node* _debugnode;

	/** Flags for kids who have reached the goal */
	bool* _kidsReachedGoal;

	/** Length of the level in box2d units */
	float _length;
	int _platformCount;
	int _wallCount;

	PolygonObstacle** _walls;
	PolygonObstacle** _platforms;


	/** The Box2D world */
	WorldController* _world;
	/** The world scale (computed from root node) */
	Vec2 _scale;

	/** Whether we have completed this "game" */
	bool _complete;

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
	void clearPineapple() { _pineapple = nullptr; }

	/**
	*
	*/
	KidModel* getKid(int i) { return _kids[i]; }

	/**
	*
	*/
	void clearKid(int i) { _kids[i] = nullptr; }


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
    
    /**
     *
     */
    float getLength() { return _length; }

#pragma mark -
#pragma mark Allocation
	/**
	*
	*/
	static Level* create(RootLayer* rootnode, Node* worldnode, Node* debugnode, WorldController* world);

    /** Actually handles the obstacle addition. Should not be called from the outside
     * Will be called as a part of the following add functions
     */
    void addObstacle(Obstacle* obstacle, int zOrder);
    
	/** Reference to the goalDoor (for collision detection) */
	void addGoal(BoxObstacle* goal);
	/** Reference to the player avatar */
	void addPineapple(Pineapple* pineapple);
	/** References to the kid avatars */
	void addKids(KidModel* kids[KID_COUNT]);
	/** Reference to the blender avatar */
	void addBlender(BlenderModel* blender);

	/** Length of the level in box2d units */
	void addLength (float length);
	void addPlatformCount(int platformCount);
	void addWallCount(int wallCount);

	void addWalls(PolygonObstacle* walls[]);
	void addPlatforms(PolygonObstacle* platforms[]);
	/** The world scale (computed from root node) */
	void addScale(Vec2 _scale);
    
    /** Adds the given obstacle to the level. Should only be called on
     * an obstacle not in the above list, i.e. a jello or a cup */
    void addAnonymousObstacle(Obstacle* obj, int zOrder);

#pragma mark -
#pragma mark Deallocation
    
    void dispose();
    
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

    
    
private:
	/**
	*
	*/
	Level();
    
    ~Level();

};


#endif