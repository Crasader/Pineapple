//
//  LevelSelectController.cpp
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
// This is the root, so there are a lot of includes
#include <string>
#include <cornell.h>
#include "LevelSelectController.h"
#include "CollisionController.h"
#include "Const.h"
#include "Texture.h"
#include "Levels.h"


using namespace cocos2d;
//using namespace std;


#pragma mark -
#pragma mark Initialization

/**
 * Creates a new game world with the default values.
 *
 * This constructor does not allocate any objects or start the controller.
 * This allows us to use a controller without a heap pointer.
 */
LevelSelectController::LevelSelectController() :
_worldnode(nullptr),
_debugnode(nullptr),
_world(nullptr),
_debug(false){}

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
bool LevelSelectController::init(Node* root) {
    return init(root,SCREEN);
}

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
 * @param gravity The gravitational force on this Box2d world
 *
 * @retain a reference to the root layer
 * @return  true if the controller is initialized properly, false otherwise.
 */
bool LevelSelectController::init(Node* root, const Rect& rect) {
    _assets = AssetManager::getInstance()->getCurrent();
    
    // Determine the center of the screen
    Size dimen  = root->getContentSize();
    Vec2 center(dimen.width/2.0f,dimen.height/2.0f);
    
    // Create the scale and notify the input handler
    Vec2 scale = Vec2(root->getContentSize().width/rect.size.width,
                      root->getContentSize().height/rect.size.height);
    Rect screen = rect;
    screen.origin.x *= scale.x;    screen.origin.y *= scale.y;
    screen.size.width *= scale.x;  screen.size.height *= scale.y;
    
    _input.init(screen);
    _input.start();
    
    // Create the scene graph
    _debugnode = _debugnode = Node::create();
    
    _rootnode = root;
    _rootnode->retain();
    
    _isInitted = true;
    
    setDebug(false);
    
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 *
 * This method is different from dispose() in that it ALSO deletes the
 * input controller.
 */
LevelSelectController::~LevelSelectController() {
    dispose();
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void LevelSelectController::dispose() {
    _worldnode = nullptr;
    _debugnode = nullptr;
    if(_rootnode != nullptr) {
        _rootnode->removeAllChildren();
        _rootnode->release();
    }
    _rootnode = nullptr;
}

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
void LevelSelectController::update(float dt) {
    
    _input.update(dt);
    
    // Turn the physics engine crank
    _world->update(dt);
    
    // Since items may be deleted, garbage collect
    _world->garbageCollect();
}
