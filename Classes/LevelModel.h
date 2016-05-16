#ifndef __LEVEL_H__
#define __LEVEL_H__

#ifdef _MSC_VER
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

#include "PineappleModel.h"
#include "BlenderModel.h"
#include "KidModel.h"
#include "GoalModel.h"
#include "WallModel.h"
#include "SpikeModel.h"
#include "JelloModel.h"
#include "CrushableModel.h"
#include "ButtonSwitchModel.h"
#include "MoveablePlatformModel.h"
#include "Const.h"
#include "Texture.h"
#include "TutorialView.h"
#include <cornell.h>
#include <vector>


/** Z indices in root */
#define WORLD_Z_INDEX           4
#define DEBUG_Z_INDEX           5

/** 
 *          Z index table:
 * /------------------------------------\
 * | 0-9    |   Behind walls            |
 * | 10-19  |   Walls and equivalent    |
 * | 20-29  |   Obstacles               |
 * | 30-40  |   Characters              |
 * \------------------------------------/
 */

#define GOAL_Z_INDEX                29
 
#define WALL_Z_INDEX                10
#define MOVEABLE_PLATFORM_Z_INDEX   12

#define JELLO_Z_INDEX               20
#define CUP_Z_INDEX                 21
#define BUTTON_SWITCH_Z_INDEX       27
#define SPIKES_Z_INDEX              28

#define KID_Z_INDEX                 30 //30...30+NUM_CHILDREN-1 used by Kids

#define PINEAPPLE_Z_INDEX           35
#define BLENDER_Z_INDEX             40


using namespace cocos2d;

class LevelModel : public Asset {
protected:
    /** Size of a tile as read from the map */
    Size _tileSize;
    
	/** Reference to the goalDoor (for collision detection) */
	GoalModel*    _goalDoor;
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
    /** Reference to all the crushables */
    std::vector<CrushableModel*> _crushables;
    /** Reference to all of the buttonSwitches */
    std::vector<ButtonSwitchModel*> _buttonSwitches;
    /** Reference to all of the moveable platforms */
    std::vector<MoveablePlatformModel*> _moveablePlatforms;

    /** The tutorial images for this level. May be empty if none */
    std::vector<TutorialView*> _tutorialViews;
    
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
    
    /** True once this level is loaded */
    bool _isLoaded;
    /** True once this level is unloaded */
    bool _isUnloaded;
    
    /** 
     * True when this is the active level in use by the GameController.
     * Set to true when the root node is set, and false when it is unloaded 
     * Note that after loading isActive will still be false
     */
    bool _isActive;

public:
#pragma mark -
#pragma mark State Access
	/**
	 *
	 */
	GoalModel* getGoal() { return _goalDoor; }

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

    vector<KidModel*> getKids() {
        vector<KidModel*> vec;
        for (int i = 0; i < KID_COUNT; i++) {
            vec.push_back(_kids[i]);
        }
        return vec;
    }
    
    int getKidsRemaining() { return _kidsRemaining; }
    
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
	std::vector<JelloModel*> getJellos() { return _jellos; }
	
	/**
	*
	*/
	std::vector<CrushableModel*> getCups() { return _crushables; }

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
    
    /**
     *
     */
    bool isActive() { return _isActive; }
    
    WorldController* getWorld() { return _world; }

    float getBlenderPineappleDistance() {
        if (_pineapple == nullptr) { return -1.0f; }
        return _pineapple->getPosition().distance(_blender->getPosition());
    }
    
    vector<TutorialView*> getTutorialViews() { return _tutorialViews; }
    
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
    
    void addKid(float kidPos[]);
    
    void addJello(float jelloPos[], int x, int y);
    
    void addCup(float cupPos[], int x, int y);
    
    void addSpikes(float spikePos[]);
    
    void addBlender(float blenderPos[]);
    
    void addButtonSwitch(float buttonSwitchPos[], bool isSwitch, Color color);
    
    void addMoveablePlatform(float platformPos[], float length, bool isOpen, bool vertical, bool nubbinsVisible, Color color);
    
    void addTutorialImage(int ID, float x, float y);
    
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

	/**
	 * Kills will, triggers blending animations and sounds, and removes him from the game world.
	 */
	void kill(PineappleModel* will);

	/**
	 * Kills kid, triggers blending animations and sounds, and removes it from the game world.
	 */
	void kill(KidModel* kid);

	/**
	* Kills will and triggers death-by-spikes animations and sounds
	*/
	void spikeAndKill(PineappleModel* will);

	/**
	* Kills the given kid and triggers death-by-spikes animations and sounds
	*/
	void spikeAndKill(KidModel* kid);

	/**
	* Removes obstacle from level world
	*
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
    
    bool string2bool (std::string v){
        return !v.empty () &&
        (strcasecmp (v.c_str (), "true") == 0 ||
         atoi (v.c_str ()) != 0);
    }
    
    /** Actually handles the obstacle addition. Should not be called from the outside
     * Will be called as a part of the following add functions
     */
    void addObstacle(Obstacle* obstacle, int zOrder);

};


#endif