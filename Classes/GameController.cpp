//
//  GameController.cpp
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
#include "GameController.h"
#include "LevelController.h"
#include "CollisionController.h"


using namespace cocos2d;
//using namespace std;

#pragma mark -
#pragma mark LevelModel Geography

/** Width of the game world in Box2d units */
#define DEFAULT_WIDTH   32.0f
/** Height of the game world in Box2d units */
#define DEFAULT_HEIGHT  12.0f
/** Half-width of scrolling window in Box2d units */
#define WINDOW_SIZE     5.0f

#pragma mark -
#pragma mark Collision Constants

#pragma mark -
#pragma mark Physics Constants
/** The new heavier gravity for this world (so it is not so floaty) */
#define DEFAULT_GRAVITY -14.7f

/** Offset for bullet when firing */
#define BULLET_OFFSET   0.5f
/** The speed of the bullet after firing */
#define BULLET_SPEED   20.0f
/** The number of frame to wait before reinitializing the game */
#define EXIT_COUNT      240


#pragma mark -
#pragma mark Asset Constants
/** The name of a wall (for object identification) */
#define WALL_NAME       "wall"
/** The name of a platform (for object identification) */
#define PLATFORM_NAME   "platform"
/** The message for winning the game */
#define WIN_MESSAGE     "VICTORY"
/** The color of the win message */
#define WIN_COLOR       Color3B::YELLOW
/** The message for losing the game */
#define LOSE_MESSAGE    "BLENDED"
/** The color of the lose message */
#define LOSE_COLOR      Color3B::RED
/** The key the basic game music */
#define GAME_MUSIC      "game"
/** The key the victory game music */
#define WIN_MUSIC       "win"
/** The key the failure game music */
#define LOSE_MUSIC      "lose"
/** The sound effect for firing a bullet */
#define PEW_EFFECT      "pew"
/** The sound effect for a bullet collision */
#define POP_EFFECT      "pop"
/** The sound effect for jumping */
#define JUMP_EFFECT     "jump"
/** The volume for the music */
#define MUSIC_VOLUME    0.7f
/** The volume for sound effects */
#define EFFECT_VOLUME   0.8f


#pragma mark -
#pragma mark Initialization

/**
 * Creates a new game world with the default values.
 *
 * This constructor does not allocate any objects or start the controller.
 * This allows us to use a controller without a heap pointer.
 */
GameController::GameController() :
    _rootnode(nullptr),
    _worldnode(nullptr),
    _debugnode(nullptr),
    _world(nullptr),
    _active(false),
	_collision(nullptr),
    _background(nullptr),
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
bool GameController::init(RootLayer* root) {
    return init(root,Rect(0,0,DEFAULT_WIDTH,DEFAULT_HEIGHT),Vec2(0,DEFAULT_GRAVITY));
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
bool GameController::init(RootLayer* root, const Rect& rect) {
    return init(root,rect,Vec2(0,DEFAULT_GRAVITY));
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
bool GameController::init(RootLayer* root, const Rect& rect, const Vec2& gravity) {
		// TODO: PRELOAD: Check if this is right way to do things
		_assets = AssetManager::getInstance()->getCurrent();
    // Determine the center of the screen
    Size dimen  = root->getContentSize();
    Vec2 center(dimen.width/2.0f,dimen.height/2.0f);
    
    // Create the scale and notify the input handler
    _scale.set(root->getContentSize().width/rect.size.width,
               root->getContentSize().height/rect.size.height);
    Rect screen = rect;
    screen.origin.x *= _scale.x;    screen.origin.y *= _scale.y;
    screen.size.width *= _scale.x;  screen.size.height *= _scale.y;

    _input.init(screen);
    _input.start();
    
    // Create the world; there are no listeners this time.
    _collision = CollisionController::create();
    _world = WorldController::create(rect,gravity);
    _world->retain();
    _world->activateCollisionCallbacks(true);
    _world->onBeginContact = [this](b2Contact* contact) {
        _collision->beginContact(contact);
    };
    _world->onEndContact = [this](b2Contact* contact) {
        _collision->endContact(contact);
    };

    // Create the scene graph
    _worldnode = Node::create();
    _debugnode = Node::create();
    _winnode = Label::create();
    
    _winnode->setTTFConfig(_assets->get<TTFont>(MESSAGE_FONT)->getTTF());
    _winnode->setString(WIN_MESSAGE);
    
    _winnode->setPosition(root->getContentSize().width/2.0f,
                          root->getContentSize().height/2.0f);
    _winnode->setColor(WIN_COLOR);
    setComplete(false);

    _losenode = Label::create();
    
    _losenode->setTTFConfig(_assets->get<TTFont>(MESSAGE_FONT)->getTTF());
    _losenode->setString(LOSE_MESSAGE);
    
    _losenode->setPosition(root->getContentSize().width/2.0f,
                           root->getContentSize().height/2.0f);
    _losenode->setColor(LOSE_COLOR);
    
    // Add everything to the root and retain
    root->addChild(_worldnode,2);
    root->addChild(_debugnode,3);
    root->addChild(_winnode,4);
    root->addChild(_losenode,5);
    _rootnode = root;
    _rootnode->retain();
    
    // Now populate the physics objects
    _levelController = LevelController::create(_assets, _rootnode, _worldnode, _debugnode, _world, _scale);
    _levelController->read("");
    _collision->setLevel(_levelController->getLevel());
    _level = _levelController->getLevel();
    populate();
    _active = true;
    setDebug(false);
    setFailure(false);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 *
 * This method is different from dispose() in that it ALSO deletes the
 * input controller.
 */
GameController::~GameController() {
    dispose();
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void GameController::dispose() {
    if (_world != nullptr) {
        _world->clear();
        _world->release();
    }
    _worldnode = nullptr;
    _background = nullptr;
    _debugnode = nullptr;
    _winnode = nullptr;
    _rootnode->removeAllChildren();
    _rootnode->release();
    _rootnode = nullptr;
}


#pragma mark -
#pragma mark LevelModel Creation
/**
 * Lays out the game geography.
 *
 * This method is really, really long.  In practice, you would replace this
 * with your serialization loader, which would process a level file.
 */
void GameController::populate() {
    _levelOffset = 0.0f;
    _worldnode->setPositionX(0.0f);
    _debugnode->setPositionX(0.0f);
    _background = BackgroundView::createAndAddTo(_rootnode, _worldnode, _debugnode, _assets);
}




#pragma mark -
#pragma mark Gameplay Handling

/**
 * Resets the status of the game so that we can play again.
 *
 * This method disposes of the world and creates a new one.
 */
void GameController::reset() {
    _world->clear();
    _worldnode->removeAllChildren();
    _background->removeAllChildren();
    _debugnode->removeAllChildren();
    
    setFailure(false);
    setComplete(false);
    _levelController->read("");
    _collision->setLevel(_levelController->getLevel());
    _level = _levelController->getLevel();
    populate();
}

/**
 * Sets whether the level is completed.
 *
 * If true, the level will advance after a countdown
 *
 * @param value whether the level is completed.
 */
void GameController::setComplete(bool value) {
    _complete = value;
    if (value) {
        Sound* source = _assets->get<Sound>(WIN_MUSIC);
        ////SoundEngine::getInstance()->playMusic(source,false,MUSIC_VOLUME);
        _winnode->setVisible(true);
        _countdown = EXIT_COUNT;
    } else {
        _winnode->setVisible(false);
        _countdown = -1;
    }
}

/**
 * Sets whether the level is failed.
 *
 * If true, the level will reset after a countdown
 *
 * @param value whether the level is failed.
 */
void GameController::setFailure(bool value){
    _level->setFailure(value && !_complete);
    if (value) {
        Sound* source = _assets->get<Sound>(LOSE_MUSIC);
        //SoundEngine::getInstance()->playMusic(source,false,MUSIC_VOLUME);
        _losenode->setVisible(true);
        _countdown = EXIT_COUNT;
    } else {
        _losenode->setVisible(false);
        _countdown = -1;
    }
    
}

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
void GameController::update(float dt) {
    _input.update(dt);
    
		if (_level->haveFailed()) {
			setFailure(true);
		}

	// Check for Victory
	if (checkForVictory()) {
		setComplete(true);
	}

    // Process kids
    for(int i = 0; i < KID_COUNT; i++) {
        if(_level->getKid(i) != nullptr) {
            _level->getKid(i)->dampTowardsWalkspeed();
        }
    }
        
    // Process the toggled key commands
    if (_input.didDebug()) { setDebug(!isDebug()); }
    if (_input.didReset()) { reset(); }
    if (_input.didExit())  {
        _rootnode->shutdown();
    }
    
    // Process the movement
    if(_level->getPineapple() != nullptr) {
        _level->getPineapple()->setMovement(_input.getHorizontal()*_level->getPineapple()->getForce());
        _level->getPineapple()->setJumping( _input.didJump());
        float cscale = Director::getInstance()->getContentScaleFactor();
        if (_input.didGrow()) {
            if (_level->getPineapple()->grow()) {
                _level->getPineapple()->getSceneNode()->setScale(cscale * PINEAPPLE_SCALE * PINEAPPLE_GROW_SCALE);
            }
        }
        if (_input.didShrink()) {
            if (_level->getPineapple()->shrink()) {
                _level->getPineapple()->getSceneNode()->setScale(cscale * PINEAPPLE_SCALE * PINEAPPLE_SHRINK_SCALE);
            }
        }
        
        if ( _level->getPineapple()->updateSize(dt)) {
            _level->getPineapple()->getSceneNode()->setScale(cscale * PINEAPPLE_SCALE);
        }
        
        _level->getPineapple()->applyForce();
        
        if (_level->getPineapple()->isJumping()) {
            Sound* source = _assets->get<Sound>(JUMP_EFFECT);
            //SoundEngine::getInstance()->playEffect(JUMP_EFFECT,source,false,EFFECT_VOLUME);
        }
    }


    //Update the background
    _background->update(dt);

	// Scroll the screen if necessary
	handleScrolling();

    // Turn the physics engine crank
    _world->update(dt);
    
    // Since items may be deleted, garbage collect
    _world->garbageCollect();
    
    // Reset the game if we win or lose.
    if (_countdown > 0) {
        _countdown--;
    } else if (_countdown == 0) {
        reset();
    }
}

/**
* Compute offsets for horizontal scrolling.
*/
void GameController::handleScrolling() {
	// Parameters
	float L = 0.5f*DEFAULT_WIDTH - WINDOW_SIZE +_levelOffset;
	float R = 0.5f*DEFAULT_WIDTH + WINDOW_SIZE +_levelOffset;
	float maxLevelOffset = _level->getLength() - DEFAULT_WIDTH;
	float offset = 0.0f;
    float oldLevelOffset = _levelOffset;
    
	// Compute the offset
	if (_level->getPineapple() != nullptr && (_levelOffset > 0) && (_level->getPineapple()->getPosition().x < L)) {
		float tempOffset = L - _level->getPineapple()->getPosition().x;
		float tempLevelOffset = _levelOffset - tempOffset;
		_levelOffset = max(0.0f, tempLevelOffset);
		offset = (tempLevelOffset > 0) ? tempOffset : _levelOffset;
		offset = -offset;
	}
	else if (_level->getPineapple() != nullptr && (_levelOffset < maxLevelOffset) && (_level->getPineapple()->getPosition().x > R)) {
		float tempOffset = _level->getPineapple()->getPosition().x - R;
		float tempLevelOffset = _levelOffset + tempOffset;
		_levelOffset = min(maxLevelOffset, tempLevelOffset);
		offset = (tempLevelOffset < maxLevelOffset) ? tempOffset : (maxLevelOffset - _levelOffset);
	}

	// Move all the objects in _worldnode
	_worldnode->setPositionX(_worldnode->getPositionX() - (_scale.x*offset));
	_debugnode->setPositionX(_debugnode->getPositionX() - (_scale.x*offset));

    _background->handleScrolling(_levelOffset, oldLevelOffset, _scale);
}

#pragma mark -
#pragma mark Collision Handling

/** 
 * Checks for victory, triggering it if it occurs
 * Specifically, sees if every living child has reached the goal
 *
 * @return true if victory has occurred
 */
bool GameController::checkForVictory() {
    int count = 0;

	// count the number of kids who have reached the goal
    for(int i = 0; i < KID_COUNT; i++) {
		if (_level->getKid(i) != nullptr) {
			if (_level->getKid(i)->hasReachedGoal()) {
				count++;
			}
		}
    }
	
	// return true if avatar and remaining kids reached goal
	if (_level->getPineapple() != nullptr) {
		return _level->getPineapple()->hasReachedGoal() && count >= _level->numKidsRemaining();
	}

	return false;
}
