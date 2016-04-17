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
#include "CollisionController.h"
#include "Const.h"
#include "Texture.h"
#include "Levels.h"


using namespace cocos2d;
//using namespace std;

/** The number of frame to wait before reinitializing the game */
#define EXIT_COUNT      180


#pragma mark -
#pragma mark Initialization

/**
 * Creates a new game world with the default values.
 *
 * This constructor does not allocate any objects or start the controller.
 * This allows us to use a controller without a heap pointer.
 */
GameController::GameController() :
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
bool GameController::init(Node* root, InputController* input) {
    return init(root,input,SCREEN);
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
bool GameController::init(Node* root, InputController* input, const Rect& rect) {
    _rootnode = root;
    _rootnode->retain();
    
    _input = input;
    
    _levelKey = LEVEL_ONE_KEY;
    _levelFile = LEVEL_ONE_FILE;
    
    _assets = AssetManager::getInstance()->getCurrent();
    _level = _assets->get<LevelModel>(_levelKey);
    
    PauseController::create();
    
    // Determine the center of the screen
    Size dimen  = root->getContentSize();
    Vec2 center(dimen.width/2.0f,dimen.height/2.0f);
    
    _level->setRootNode(root);
    
    // Create the world; there are no listeners this time.
    _collision = CollisionController::create();
    _world = _level->getWorld();
    
    // Create the scene graph
    _worldnode = _level->getWorldNode();
    _debugnode = _level->getDebugNode();
    
    PauseController::init(this, _worldnode, _assets, root, _input);

    _winnode = Label::create();
    _winnode->setTTFConfig(_assets->get<TTFont>(MESSAGE_FONT)->getTTF());
    _winnode->setString(WIN_MESSAGE);
    _winnode->setPosition(root->getContentSize().width/2.0f,
                          root->getContentSize().height/2.0f);
    _winnode->setColor(WIN_COLOR);
    
    _losenode = Label::create();
    _losenode->setTTFConfig(_assets->get<TTFont>(MESSAGE_FONT)->getTTF());
    _losenode->setString(LOSE_MESSAGE);
    _losenode->setPosition(root->getContentSize().width/2.0f,
                           root->getContentSize().height/2.0f);
    _losenode->setColor(LOSE_COLOR);
    
    _loadnode = Label::create();
    _loadnode->retain();
    _loadnode->setTTFConfig(_assets->get<TTFont>(MESSAGE_FONT)->getTTF());
    _loadnode->setString(LOAD_MESSAGE);
    _loadnode->setPosition(root->getContentSize().width/2.0f,
                           root->getContentSize().height/2.0f);
    _loadnode->setColor(LOAD_COLOR);
    
    root->addChild(_winnode,4);
    root->addChild(_losenode,5);
    root->addChild(_loadnode,6);
    
    _loadnode->setVisible(false);
    
    _collision->setLevel(_level);
    
    _levelOffset = 0.0f;
    _worldnode->setPositionX(0.0f);
    _debugnode->setPositionX(0.0f);
    _background = BackgroundView::createAndAddTo(_rootnode, _worldnode, _assets);
    
    _world->activateCollisionCallbacks(true);
    _world->onBeginContact = [this](b2Contact* contact) {
        _collision->beginContact(contact);
    };
    _world->onEndContact = [this](b2Contact* contact) {
        _collision->endContact(contact);
    };
    
    _active = true;
    setComplete(false);
    setDebug(false);
    setFailure(false);
    _isInitted = true;
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
    if (_level != nullptr) {
        _level->unload();
    }
    _worldnode = nullptr;
    _background = nullptr;
    PauseController::release();
    _debugnode = nullptr;
    _winnode = nullptr;
    _loadnode->release();
    _loadnode = nullptr;
    if (_rootnode != nullptr) {
        _rootnode->removeAllChildren();
        _rootnode->release();
    }
    _rootnode = nullptr;
}

#pragma mark -
#pragma mark Gameplay Handling

/**
 * Resets the status of the game so that we can play again.
 *
 * This method disposes of the world and creates a new one.
 */
void GameController::reset() {
    setFailure(false);
    setComplete(false);
    
    // Unload the level but keep in memory temporarily
    _level->retain();
    _assets->unload<LevelModel>(_levelKey);
    
    // Load a new level and quit update
    _assets->loadAsync<LevelModel>(_levelKey,_levelFile);
    _loadnode->setVisible(true);
}

/** Called after the loadAsync for the level finishes */
void GameController::onReset() {
    
    // Release the old level permanently
    _level->release();
    
    // Access and initialize level
    _level = _assets->get<LevelModel>(_levelKey);
    _level->setRootNode(_rootnode);
    _level->showDebug(_debug);
    _world = _level->getWorld();
    _worldnode = _level->getWorldNode();
    _debugnode = _level->getDebugNode();
    
    _collision->setLevel(_level);
    _world->activateCollisionCallbacks(true);
    _world->onBeginContact = [this](b2Contact* contact) {
        _collision->beginContact(contact);
    };
    _world->onEndContact = [this](b2Contact* contact) {
        _collision->endContact(contact);
    };
    _levelOffset = 0.0f;
    _worldnode->setPositionX(0.0f);
    _debugnode->setPositionX(0.0f);
    _background->reset();
    _loadnode->setVisible(false);
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

void handleAvatarGrowth(float cscale, InputController* _input, PineappleModel* _avatar) {
    int size = 0;
    float scale = 1.0f;
    if (_input->didGrow()) {
        size = _avatar->grow();
        if (size == 2)
            scale = PINEAPPLE_GROW_SCALE;
    } else if (_input->didShrink()) {
        size = _avatar->shrink();
        if (size == 2)
            scale = PINEAPPLE_SHRINK_SCALE;
    }
    if (size) {
        _avatar->getSceneNode()->setScale(cscale * PINEAPPLE_SCALE * scale);
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
    if (_level == nullptr) {
        return;
    }
    
    // Check to see if new level loaded yet
    if (_loadnode->isVisible()) {
        if (_assets->isComplete()) {
            onReset();
        } else {
            // Level is not loaded yet; refuse input
            return;
        }
    }
    
    _input->update(dt);
    
    // Process the toggled key commands
    if (_input->didPause()) {
        if (!PauseController::isPaused()) {
            PauseController::pause();
            return;
        } else {
            PauseController::unPause();
        }
    }
    if (_input->didDebug()) { setDebug(!isDebug()); }
    if (_input->didReset()) {
        reset();
        return;
    }
    if (_input->didExit())  {
        setTransitionStatus(TRANSITION_TO_EXIT);
        return;
    }
    if (PauseController::isPaused()) {
        return;
    }
    
    if (_level->haveFailed() && _countdown == -1) {
        setFailure(true);
    }
    
    // Check for Victory
    if (checkForVictory() && ! _level->haveFailed() && ! _winnode->isVisible()) {
        setComplete(true);
    }
    
    // Process kids
    for(int i = 0; i < KID_COUNT; i++) {
        if(_level->getKid(i) != nullptr) {
            _level->getKid(i)->dampTowardsWalkspeed();
            _level->getKid(i)->animate();
        }
    }
    
    // Process the movement
    if(_level->getPineapple() != nullptr) {
        _level->getPineapple()->setMovement(_input->getHorizontal()*_level->getPineapple()->getForce());
        _level->getPineapple()->setJumping( _input->didJump());
        float cscale = Director::getInstance()->getContentScaleFactor();

        handleAvatarGrowth(cscale, _input, _level->getPineapple());
        
        _level->getPineapple()->applyForce();
        
        if (_level->getPineapple()->isJumping()) {
            Sound* source = _assets->get<Sound>(JUMP_EFFECT);
            //SoundEngine::getInstance()->playEffect(JUMP_EFFECT,source,false,EFFECT_VOLUME);
        }
        
        _level->getPineapple()->animate();
    }
    
    // Update the background (move the clouds)
    _background->update(dt);
    
    // Scroll the screen (with parallax) if necessary
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
    _worldnode->setPositionX(_worldnode->getPositionX() - (_level->getDrawScale().x*offset));
    _debugnode->setPositionX(_debugnode->getPositionX() - (_level->getDrawScale().x*offset));
    
    // Do parallax scrolling of the background
    _background->handleScrolling(offset, _levelOffset, oldLevelOffset, _level->getDrawScale());
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
