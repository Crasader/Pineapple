//
//  LevelSelectController.h
//  PlatformerDemo
//
//  This is the most important class in this demo.  This class manages the gameplay
//  for this demo.  It also handles collision detection. There is not much to do for
//  collisions; our WorldController class takes care of all of that for us.  This
//  controller mainly transforms input into gameplay.
//
//  WARNING: There are a lot of shortcuts in this design that will do not adapt well
//  to data driven design.  This demo has a lot of simplifications to make it a bit
//  easier to see how everything fits together.  However, the model classes and how
//  they are initialized will need to be changed if you add dynamic level loading.
//
//  This file is based on the CS 3152 PhysicsDemo Lab by Don Holden, 2007
//
//  Author: Walker White
//  Version: 1/15/15
//
#ifndef __PF_LEVEL_SELECT_CONTROLLER_H__
#define __PF_LEVEL_SELECT_CONTROLLER_H__

#include "InputController.h"
#include "AbsScreenController.h"
#include "Levels.h"
#include "ui/CocosGUI.h"

#define NUM_LEVELS                  13
#define NO_LEVEL_SELECTED           -1

// We need a lot of forward references to the classes used by this controller
// These forward declarations are in cocos2d namespace
namespace cocos2d {
    class RootLayer;
    class WorldController;
    class SceneManager;
}

// These forward declarations are in the project
class CollisionController;

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;

#pragma mark -
#pragma mark LevelSelectController
/**
 * Instance is a controls the gameplay for the demo.
 *
 * A world has its own objects, assets, and input controller.  Thus this is
 * really a mini-GameEngine in its own right.  The only thing that it does
 * not do is create the scene graph root; that is shared with it by the 
 * game root (which has scaled the scene graph to fix the device with the
 * desired aspect ratio).
 */
class LevelSelectController : public AbsScreenController {
protected:    
    /** Reference to the physics root of the scene graph */
    Node* _worldnode;
    /** Reference to the debug root of the scene graph */
    Node* _debugnode;
    /** The Box2D world */
    WorldController* _world;
    
    /** The scene manager for this game demo */
    SceneManager* _assets;
    
    /** Controller for abstracting out input away from layer */
    InputController* _input;
    
    /** True iff we should display the debug nodes of the physics bodies */
    bool _debug;
    
    /** The node representing the background image drawn for this screen */
    Node* _backgroundNode;
    
    // array of buttons
    Button* _buttons[NUM_LEVELS];
    
    /** The level button that is selected. -1 when none */
    int _levelSelected;
    
#pragma mark Internal Object Management
    /**
     * Lays out the game geography.
     */
    void populate();
    
    /**
     * Immediately adds the object to the physics world
     *
     * Objects have a z-order.  This is the order they are drawn in the scene
     * graph node.  Objects with the different textures should have different
     * z-orders whenever possible.  This will cut down on the amount of drawing done
     *
     * param  obj       The object to add
     * param  zOrder    The drawing order
     *
     * @retain a reference to the obstacle
     */
    void addObstacle(Obstacle* obj, int zOrder);
    
public:
    
    const static string LEVEL_KEYS[NUM_LEVELS];
    const static string LEVEL_FILES[NUM_LEVELS];
    
#pragma mark -
#pragma mark Initialization
    /** Creates and returns a button for the given index */
    Button* initButton(Size dimen, int i);
    
    /**
     * Initializes the controller contents, and starts the game
     *
     * The constructor does not allocate any objects or memory.  This allows
     * us to have a non-pointer reference to this controller, reducing our
     * memory allocation.  Instead, allocation happens in this method.
     *
     * The game world is scaled so that the screen coordinates do not agree
     * with the Box2d coordinates.  This initializer uses the default scale.
     *
     * @retain a reference to the root layer
     * @return  true if the controller is initialized properly, false otherwise.
     */
    bool init(Node* root, InputController* input);
    
    /**
     * Initializes the controller contents, and starts the game
     *
     * The constructor does not allocate any objects or memory.  This allows
     * us to have a non-pointer reference to this controller, reducing our
     * memory allocation.  Instead, allocation happens in this method.
     *
     * The game world is scaled so that the screen coordinates do not agree
     * with the Box2d coordinates.  The bounds are in terms of the Box2d
     * world, not the screen.
     *
     * @param bounds The game bounds in Box2d coordinates
     * @param scale  The difference between screen and Box2d coordinates
     *
     * @retain a reference to the root layer
     * @return  true if the controller is initialized properly, false otherwise.
     */
    bool init(Node* root, InputController* input, const Rect& rect);
    
    
#pragma mark -
#pragma mark State Access
    /** Returns the selected level, -1 if none */
    int getSelectedLevel() { return _levelSelected; }
    
    /** Resets the selected level to none */
    void clearSelectedLevel() { _levelSelected = NO_LEVEL_SELECTED; }
    
    /**
     * Returns true if debug mode is active.
     *
     * If true, all objects will display their physics bodies.
     *
     * @return true if debug mode is active.
     */
    bool isDebug( ) const { return _debug; }
    
    /**
     * Sets whether debug mode is active.
     *
     * If true, all objects will display their physics bodies.
     *
     * @param value whether debug mode is active.
     */
    void setDebug(bool value) { _debug = value; _debugnode->setVisible(value); }
    
#pragma mark -
#pragma mark Allocation
    /**
     * Creates a new game world with the default values.
     *
     * This constructor does not allocate any objects or start the controller.
     * This allows us to use a controller without a heap pointer.
     */
    LevelSelectController();
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    ~LevelSelectController();
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     */
    void dispose();
    
    ///**
    // * Preloads all of the assets necessary for this game world
    // */
    //void preload();
    
#pragma mark -
#pragma mark Gameplay Handling
    /**
     * Executes the core gameplay loop of this world.
     *
     * This method contains the specific update code for this mini-game. It does
     * not handle collisions, as those are managed by the parent class WorldController.
     * This method is called after input is read, but before collisions are resolved.
     * The very last thing that it should do is apply forces to the appropriate objects.
     *
     * @param  delta    Number of seconds since last animation frame
     */
    void update(float dt) override;
};

#endif /* defined(__PF_LEVEL_SELECT_CONTROLLER_H__) */
