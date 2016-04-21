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

#define SPLAT_Z         5
#define WIN_SPLASH_Z    7
#define LOSE_SPLASH_Z   8

/** The number of frames to wait before removing the splat */
#define SPLAT_COUNT     20


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
_splat(nullptr), 
_world(nullptr),
_active(false),
_collision(nullptr),
_background(nullptr),
_debug(false),
_loseViewVisible(false),
_winViewVisible(false){}

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
bool GameController::init(Node* root, InputController* input, int levelIndex, string levelKey, string levelFile) {
    return init(root, input, levelIndex, levelKey, levelFile, SCREEN);
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
bool GameController::init(Node* root, InputController* input, int levelIndex, string levelKey, string levelFile, const Rect& rect) {
    _rootnode = root;
    _rootnode->retain();
    
    _input = input;
    
    _levelIndex = levelIndex;
    _levelKey = levelKey;
    _levelFile = levelFile;
    
    _assets = AssetManager::getInstance()->getCurrent();
    _level = _assets->get<LevelModel>(_levelKey);
    
    PauseController::create();
    HUDController::create();
    
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

	_splat = PolygonNode::createWithTexture(_assets->get<Texture2D>(SPLAT_TEXTURE_1));
	_splat->retain();
	_splat->setVisible(false);
	root->addChild(_splat, SPLAT_Z);
	_splatCount = -1;
	_rootSize = root->getContentSize();
    
    PauseController::init(this, _worldnode, _assets, root, _input);
    HUDController::init(this, _worldnode, _assets, root, _input, _level->getBlender()->getPosition().x);

    _winnode = Label::create();
    _winnode->setTTFConfig(_assets->get<TTFont>(MESSAGE_FONT)->getTTF());
    _winnode->setString(WIN_MESSAGE);
    _winnode->setPosition(root->getContentSize().width/2.0f,
                          root->getContentSize().height/2.0f);
    _winnode->setColor(WIN_COLOR);
    
    _loseroot = Node::create();
    _loseroot->setContentSize(_rootnode->getContentSize());
    _loseroot->retain();
    _loseview = LoseView::create(_loseroot, _assets, _level->getDrawScale());
    
    _winroot = Node::create();
    _winroot->setContentSize(_rootnode->getContentSize());
    _winroot->retain();
    _winview = WinView::create(_winroot, _assets, _level->getDrawScale());
    
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
    _isReloading = false;
    _loseViewVisible = false;
    _winViewVisible = false;
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
    if (_loseview != nullptr) {
        _loseview->dispose();
    }
    if (_loseroot != nullptr) {
        _loseroot->release();
        _loseroot = nullptr;
    }
    if (_winview != nullptr) {
        _winview->dispose();
    }
    if (_winroot != nullptr) {
        _winroot->release();
        _winroot = nullptr;
    }
    _worldnode = nullptr;
    _background = nullptr;
    PauseController::release();
    _debugnode = nullptr;
	_splat->release();
	_splat = nullptr; 
    _winnode = nullptr;
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
void GameController::reset(int levelIndex, string levelKey, string levelFile) {
    setFailure(false);
    setComplete(false);
	
    // clear state
    _collision->reset();
    
    // Unload the level but keep in memory temporarily
    _level->retain();
    _assets->unload<LevelModel>(_levelKey);
    
    // Load a new level and quit update
    _levelIndex = levelIndex;
    _levelKey = levelKey;
    _levelFile = levelFile;

    if (_assets->get<LevelModel>(_levelKey) == nullptr) {
        _assets->loadAsync<LevelModel>(_levelKey,_levelFile);
        _isReloading = true;
    } else {
        onReset();
    }
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
    
    _splat->setVisible(false);
    _splatCount = -1;
    
    //reset the hud
    HUDController::reset(this, _worldnode, _assets, _rootnode, _input, _level->getBlender()->getPosition().x);
    
    _levelOffset = 0.0f;
    _worldnode->setPositionX(0.0f);
    _debugnode->setPositionX(0.0f);
    _background->reset(_worldnode);
    _isReloading = false;
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
        //SoundEngine::getInstance()->playMusic(source,false,MUSIC_VOLUME);
        _rootnode->addChild(_winroot, WIN_SPLASH_Z);
        _winview->position();
        _winViewVisible = true;
        HUDController::setEnabled(false);
    } else {
        if (_winViewVisible) {
            _rootnode->removeChild(_winroot);
        }
        _winViewVisible = false;
        HUDController::setEnabled(true);
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
        //SoundEngine::getInstance()->playMusic(source,false,MUSIC_VOLUME);
        _rootnode->addChild(_loseroot, LOSE_SPLASH_Z);
        _loseview->position();
        _loseViewVisible = true;
        HUDController::setEnabled(false);
    } else {
        if (_loseViewVisible) {
            _rootnode->removeChild(_loseroot);
        }
        _loseViewVisible = false;
        HUDController::setEnabled(true);
    }
    
}

void handleAvatarGrowth(float cscale, InputController* _input, PineappleModel* _avatar) {
    int size = 0;
    float scale = 1.0f;
    if (_input->didGrow()) {
        size = _avatar->grow();
    } else if (_input->didShrink()) {
        size = _avatar->shrink();
        if (size == 1)
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
    if (_isReloading) {
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
        PauseController::animate();
        return;
    }
    
    if (_level->haveFailed() && ! _loseViewVisible) {
        setFailure(true);
    }
    
    // Check for Victory
    if (checkForVictory() && ! _level->haveFailed() && ! _winViewVisible) {
        setComplete(true);
    }
    
    if (_loseViewVisible)  {
        _loseview->update(dt);
        
        if (_loseview->shouldReset()) {
            _loseview->resetButtons();
            reset();
            return;
        } else if (_loseview->shouldTransferToLevelSelect()) {
            _loseview->resetButtons();
            Sound* sound = AssetManager::getInstance()->getCurrent()->get<Sound>(GAME_BACKGROUND_SOUND);
            SoundEngine::getInstance()->playMusic(sound, true, MUSIC_VOLUME);
            setTransitionStatus(TRANSITION_TO_LEVEL_SELECT);
            return;
        }
    } else if (_winViewVisible) {
        _winview->update(dt);
        
        if (_winview->shouldReset()) {
            _winview->resetButtons();
            reset();
            return;
        } else if (_winview->shouldTransferToLevelSelect()) {
            _winview->resetButtons();
            Sound* sound = AssetManager::getInstance()->getCurrent()->get<Sound>(GAME_BACKGROUND_SOUND);
            SoundEngine::getInstance()->playMusic(sound, true, MUSIC_VOLUME);
            setTransitionStatus(TRANSITION_TO_LEVEL_SELECT);
            return;
        } else if (_winview->shouldTransferToNextLevel()) {
            _winview->resetButtons();
            Sound* sound = AssetManager::getInstance()->getCurrent()->get<Sound>(GAME_BACKGROUND_SOUND);
            SoundEngine::getInstance()->playMusic(sound, true, MUSIC_VOLUME);
            setTransitionStatus(TRANSITION_TO_NEXT_LEVEL);
            return;

        }
    } else {
        // Process kids
        for(int i = 0; i < KID_COUNT; i++) {
            if(_level->getKid(i) != nullptr) {
                if (!_level->getKid(i)->getIsBlended()) {
                    _level->getKid(i)->dampTowardsWalkspeed();
                    _level->getKid(i)->animate();
                }
                else {
                    _level->getKid(i)->spiral(_level->getBlender()->getPosition().x - 4.0f, _level->getBlender()->getPosition().y);
                    _level->getKid(i)->setFixedRotation(false);
                    _level->getKid(i)->setAngularVelocity(6.0f);
                    if (_level->getKid(i)->getIsDead()) {
                        bool offScreen = (_level->getBlender()->getX() + (_level->getBlender()->getWidth() / 2.0f)) < _levelOffset;
                        activateSplat(_assets->get<Texture2D>(_level->getKid(i)->getSplatTexture(i)), offScreen);
                        _level->kill(_level->getKid(i));
                        _level->getBlender()->setIsBlending(true);
                        _splatCount = SPLAT_COUNT;
                    }
                }
                // check if off bounds death
                if (_level->getKid(i) != nullptr && _level->getKid(i)->getPosition().y < 0) {
                    _level->kill(_level->getKid(i));
                }
            }
        }
        
        // Process the movement
        if (_level->getPineapple() != nullptr) {
            if (!_level->getPineapple()->getIsBlended()) {
                _level->getPineapple()->setMovement(_input->getHorizontal()*_level->getPineapple()->getForce());
                _level->getPineapple()->setJumping(_input->didJump());
                float cscale = Director::getInstance()->getContentScaleFactor();
                
                handleAvatarGrowth(cscale, _input, _level->getPineapple());
                
                _level->getPineapple()->applyForce();
                
                _level->getPineapple()->animate();
                
                if (_level->getPineapple()->isJumping()) {
                    //Sound* source = _assets->get<Sound>(JUMP_EFFECT);
                    //SoundEngine::getInstance()->playEffect(JUMP_EFFECT,source,false,EFFECT_VOLUME);
                }
                
                if (_level->getPineapple()->getPosition().y < 0) {
                    _level->kill(_level->getPineapple());
                }
                
                // Scroll the screen (with parallax) if necessary
                handleScrolling();
            } else {
                _level->getPineapple()->spiral(_level->getBlender()->getPosition().x - 4.0f, _level->getBlender()->getPosition().y);
                _level->getPineapple()->setFixedRotation(false);
                _level->getPineapple()->setAngularVelocity(6.0f);
                if (_level->getPineapple()->getIsDead()) {
                    _level->kill(_level->getPineapple());
                    _level->getBlender()->setIsBlending(true);
                }
            }
        }
        
        // Animate the jello
        std::vector<JelloModel*> jellos = _level->getJellos();
        for (auto it = jellos.begin(); it != jellos.end(); ++it) {
            JelloModel* jello = *it;
            jello->animate();
        }
        
        // Animate the blender
        _level->getBlender()->animate();
        
        HUDController::update(_level->numKidsRemaining(), _level->getBlender()->getPosition().x + _level->getBlender()->getWidth()/2.0f, _level->getKids(), _level->getPineapple(), _level->getGoal()->getPosition().x);
        
        // Update the background (move the clouds)
        _background->update(dt);
        
        // Turn the physics engine crank
        _world->update(dt);
    }
    
    // Since items may be deleted, garbage collect
    _world->garbageCollect();
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

/**
* Activate the splat effect when shit hits the fan
*/
void GameController::activateSplat(Texture2D* image, bool offScreen) {
	_splat->setTexture(image);
	_splat->setScale(2.0f, 2.0f);
	_splat->setVisible(true);

	if (offScreen) {
		_splat->setPosition(0.0f, _rootSize.height/2.0f);
	}
	else {
		_splat->setPosition(_rootSize.width/2.0f, _rootSize.height / 2.0f);
	}
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
