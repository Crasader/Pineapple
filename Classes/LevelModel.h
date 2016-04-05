#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "PineappleModel.h"
#include "BlenderModel.h"
#include "KidModel.h"
#include "GoalModel.h"
#include "WallModel.h"
#include "SpikeModel.h"
#include "JelloModel.h"
#include "Const.h"
#include "Texture.h"
#include <cornell.h>
#include <vector>

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

#define CUP_Z_INDEX             20
#define JELLO_Z_INDEX           21
#define SPIKES_Z_INDEX          29

#define KID_Z_INDEX             30 //30...30+NUM_CHILDREN-1 used by Kids

#define PINEAPPLE_Z_INDEX       35
#define BLENDER_Z_INDEX         36

using namespace cocos2d;

class LevelModel : public Asset {
protected:
	/** Reference to the goalDoor (for collision detection) */
	BoxObstacle*    _goalDoor;
	/** Reference to the player avatar */
	PineappleModel*      _pineapple;
	/** References to the kid avatars */
	KidModel*     _kids[KID_COUNT];
	/** Reference to the blender avatar */
	BlenderModel* _blender;
	/** Reference to the root node of the scene graph */
	Node* _rootnode;
	/** Reference to the physics root of the scene graph */
	Node* _worldnode;
    /** The bounds of this level in physics coordinates */
    Rect _bounds;
	/** Reference to the debug root of the scene graph */
	Node* _debugnode;

	/** Flags for kids who have reached the goal */
	bool* _kidsReachedGoal;

	/** Length of the level in box2d units */
	float _length;

    
    /** Reference to all the walls */
    std::vector<WallModel*> _walls;
    
    /** Reference to all the jellos */
    std::vector<JelloModel*> _jellos;
    /** Reference to all the spikes */
    std::vector<SpikeModel*> _spikes;

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
	PineappleModel* getPineapple() { return _pineapple; }

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
     * Creates a new game level with no source file.
     *
     * The source file can be set at any time via the setFile() method. This method
     * does NOT load the asset.  You must call the load() method to do that.
     *
     * @return  an autoreleased level file
     */
    static LevelModel* create();
    
    /**
     * Creates a new game level with the given source file.
     *
     * This method does NOT load the level. You must call the load() method to do that.
     * This method returns false if file does not exist.
     *
     * @return  an autoreleased level file
     */
    static LevelModel* create(std::string file);
    
    void addLength(float length);
    
    void addGoal(float goalPos[]);
    
    void addWall(float wallPos[]);
        
    void addPineapple(float pineapplePos[]);
    
    void addKids(float* kidPos[]);
    
    void addJello(float jelloPos[]);
    
    void addCup(float cupPos[]);
    
    void addSpikes(float spikePos[]);
    
    void addBlender(float blenderPos[]);
    /** Adds the given obstacle to the level. Should only be called on
     * an obstacle not in the above list, i.e. a jello or a cup */
    void addAnonymousObstacle(Obstacle* obj, int zOrder);

#pragma mark -
#pragma mark Asset Loading
    /**
     * Loads this game level from the source file
     *
     * This load method should NEVER access the AssetManager.  Assets are loaded in
     * parallel, not in sequence.  If an asset (like a game level) has references to
     * other assets, then these should be connected later, during scene initialization.
     *
     * @return true if successfully loaded the asset from a file
     */
    virtual bool load() override;
    
    /**
     * Unloads this game level, releasing all sources
     *
     * This load method should NEVER access the AssetManager.  Assets are loaded and
     * unloaded in parallel, not in sequence.  If an asset (like a game level) has
     * references to other assets, then these should be disconnected earlier.
     */
    virtual void unload() override;
    
    /**
     * Resets this game level, restoring all objects to their original position.
     */
    void reset();
    
#pragma mark Physics Attributes
    /**
     * Returns the bounds of this level in physics coordinates
     *
     * @return the bounds of this level in physics coordinates
     */
    const Rect& getBounds() const   { return _bounds; }
    
    /**
     * Activates all of the physics objects in this level
     *
     * @param  controller  the world controller to manage physics
     */
    void activate(WorldController* controller);
    
#pragma mark Drawing Methods
    /**
     * Returns the drawing scale for this game level
     *
     * The drawing scale is the number of pixels to draw before Box2D unit. Because
     * mass is a function of area in Box2D, we typically want the physics objects
     * to be small.  So we decouple that scale from the physics object.  However,
     * we must track the scale difference to communicate with the scene graph.
     *
     * We allow for the scaling factor to be non-uniform.
     *
     * @return the drawing scale for this game level
     */
    const Vec2& getDrawScale() const { return _scale; }
    
    /**
     * Sets the drawing scale for this game level
     *
     * The drawing scale is the number of pixels to draw before Box2D unit. Because
     * mass is a function of area in Box2D, we typically want the physics objects
     * to be small.  So we decouple that scale from the physics object.  However,
     * we must track the scale difference to communicate with the scene graph.
     *
     * We allow for the scaling factor to be non-uniform.
     *
     * @param value  the drawing scale for this game level
     */
    void setDrawScale(const Vec2& value);
    
    /**
     * Returns the scene graph node for drawing purposes.
     *
     * The scene graph is completely decoupled from the physics system.  The node
     * does not have to be the same size as the physics body. We only guarantee
     * that the node is positioned correctly according to the drawing scale.
     *
     * @return the scene graph node for drawing purposes.
     */
    Node* getRootNode() const { return _rootnode; }
    
    /**
     * Sets the scene graph node for drawing purposes.
     *
     * The scene graph is completely decoupled from the physics system.  The node
     * does not have to be the same size as the physics body. We only guarantee
     * that the node is positioned correctly according to the drawing scale.
     *
     * @param value  the scene graph node for drawing purposes.
     *
     * @retain  a reference to this scene graph node
     * @release the previous scene graph node used by this object
     */
    void setRootNode(Node* node);
    
    /**
     * Clears the root scene graph node for this level
     */
    void clearRootNode();
    
    /**
     * Toggles whether to show the debug layer of this game world.
     *
     * The debug layer displays wireframe outlines of the physics fixtures.
     *
     * @param  flag whether to show the debug layer of this game world
     */
    void showDebug(bool flag);
    
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
	LevelModel();
    
    ~LevelModel();
    
    
    /** Actually handles the obstacle addition. Should not be called from the outside
     * Will be called as a part of the following add functions
     */
    void addObstacle(Obstacle* obstacle, int zOrder);

};


#endif