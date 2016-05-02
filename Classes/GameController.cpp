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
#include "Sounds.h"


using namespace cocos2d;
//using namespace std;

#define SPLAT_Z             6
#define TUTORIAL_SPLASH_Z   7
#define WIN_SPLASH_Z        8
#define LOSE_SPLASH_Z       9
#define GOAL_DOOR_Z         38

// global z order
#define MOVEMENT_VIEW_Z 5


//Min levels for functionality
#define MIN_JUMP_LEVEL      1
#define MIN_SHRINK_LEVEL    2
#define MIN_GROW_LEVEL      5


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
_splatCycle(nullptr),
_fridgeDoor(nullptr),
_world(nullptr),
_active(false),
_collision(nullptr),
_background(nullptr),
_debug(false),
_loseViewVisible(false),
_winViewVisible(false),
_tutorialViewVisible(false){}

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

void configureMoveView(Button* button) {
    button->setCascadeColorEnabled(true);
    button->setSwallowTouches(false);
    button->setTouchEnabled(true);
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

	_splatCycle = AnimationNode::create(_assets->get<Texture2D>(SPLAT_TEXTURE_1), 1, 14, 14);
	_splatCycle->setFrame(0);
	_splatCycle->setVisible(false);
	_splatCycle->retain();
	root->addChild(_splatCycle, SPLAT_Z);
	_splatFrame = 0.0f;
	_hasSplat = false;
	_rootSize = root->getContentSize();

	_fridgeDoor = PolygonNode::createWithTexture(_assets->get<Texture2D>(GOAL_DOOR_TEXTURE));
	_fridgeDoor->setScale(1.5f, 1.5f); // GOAL_SCALE
	_fridgeDoor->setPosition(_level->getDrawScale().x*_level->getGoal()->getPosition().x, 
							 _level->getDrawScale().y*_level->getGoal()->getPosition().y);
	_fridgeDoor->setVisible(true);
	_fridgeDoor->retain();
	_worldnode->addChild(_fridgeDoor, GOAL_DOOR_Z);
    
    PauseController::init(this, _worldnode, _assets, root, _input);
    HUDController::init(this, _worldnode, _assets, root, _input, _level->getBlender()->getPosition().x);
    
    _loseroot = Node::create();
    _loseroot->setContentSize(_rootnode->getContentSize());
    _loseroot->retain();
    _loseview = LoseView::create(_loseroot, _assets, _level->getDrawScale());
    
    _winroot = Node::create();
    _winroot->setContentSize(_rootnode->getContentSize());
    _winroot->retain();
    _winview = WinView::create(_winroot, _assets, _level->getDrawScale());
    
    _tutorialroot = Node::create();
    _tutorialroot->setContentSize(_rootnode->getContentSize());
    _tutorialroot->retain();
    
    _tutorialviews = _level->getTutorialViews();
    for(auto it = _tutorialviews.begin(); it != _tutorialviews.end(); ++it) {
        (*it)->init(_tutorialroot, _assets, _level->getDrawScale());
    }
    
    _collision->setLevel(_level);
    
    _levelOffset = 0.0f;
    _worldnode->setPositionX(0.0f);
    _debugnode->setPositionX(0.0f);
    _background = BackgroundView::createAndAddTo(_rootnode, _worldnode, _assets);
    
    // add left and right movement textures
    _moveLeftView = Button::create();
    _moveLeftView->init(MOVE_LEFT_UNPRESSED);
    _moveLeftView->loadTexturePressed(MOVE_LEFT_PRESSED);
    float cscale = Director::getInstance()->getContentScaleFactor();
    float scale = 1.0f / (_moveLeftView->getContentSize().width / root->getContentSize().width);
    _moveLeftView->setScaleX(LEFT_ZONE * scale);
    _moveLeftView->setScaleY(root->getContentSize().height / _moveLeftView->getContentSize().height);
    _moveLeftView->setPositionY(root->getContentSize().height /2.0f);
    _moveLeftView->setPositionX((_moveLeftView->getContentSize().width * _moveLeftView->getScaleX()/2.0f));
    // set normal and pressed image
    _moveRightView = Button::create();
    _moveRightView->init(MOVE_RIGHT_UNPRESSED);
    _moveRightView->loadTexturePressed(MOVE_RIGHT_PRESSED);
    scale = 1.0f / (_moveRightView->getContentSize().width / root->getContentSize().width);
    _moveRightView->setScaleX(RIGHT_ZONE * scale);
    _moveRightView->setScaleY(root->getContentSize().height / _moveRightView->getContentSize().height);
    _moveRightView->setPositionY(root->getContentSize().height /2.0f);
    _moveRightView->setPositionX(root->getContentSize().width * (1.0f - RIGHT_ZONE) + (_moveRightView->getContentSize().width * _moveRightView->getScaleX()/ 2.0f));
    _rootnode->addChild(_moveLeftView, MOVEMENT_VIEW_Z);
    _moveLeftView->retain();
    _rootnode->addChild(_moveRightView, MOVEMENT_VIEW_Z);
    _moveRightView->retain();
    
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
    setTutorialVisible(nullptr);
    _isInitted = true;
    _isReloading = false;
    _loseViewVisible = false;
    _winViewVisible = false;
    _tutorialViewVisible = false;
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
        _level = nullptr;
    }
    if (_loseview != nullptr) {
        _loseview->dispose();
        _loseview = nullptr;
    }
    if (_loseroot != nullptr) {
        _loseroot->removeAllChildren();
        _loseroot->release();
        _loseroot = nullptr;
    }
    if (_winview != nullptr) {
        _winview->dispose();
        _winview = nullptr;
    }
    if (_winroot != nullptr) {
        _winroot->removeAllChildren();
        _winroot->release();
        _winroot = nullptr;
    }
    for(auto it = _tutorialviews.begin(); it != _tutorialviews.end(); ++it) {
        (*it)->clearFromRoot();
        (*it)->dispose();
    }
    _tutorialviews.clear();
    
    if (_tutorialroot != nullptr) {
        _tutorialroot->removeAllChildren();
        _tutorialroot->release();
        _tutorialroot = nullptr;
    }
    
    _worldnode = nullptr;
    _background = nullptr;
    PauseController::release();
    _debugnode = nullptr;
    
    if (_splatCycle != nullptr) {
        _splatCycle->release();
        _splatCycle = nullptr;
    }
    
    if (_fridgeDoor != nullptr) {
        _fridgeDoor->release();
        _fridgeDoor = nullptr;
    }
    
    if (_moveRightView != nullptr) {
        _moveRightView->release();
        _moveRightView = nullptr;
    }
    
    if (_moveLeftView) {
        _moveLeftView->release();
        _moveLeftView = nullptr;
    }

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
    setTutorialVisible(nullptr);
	
    // clear state
    _collision->reset();
    
    if (_fridgeDoor != nullptr) {
        _fridgeDoor->release();
        _fridgeDoor = nullptr;
    }
    
    // Unload the level but keep in memory temporarily
    
    for(auto it = _tutorialviews.begin(); it != _tutorialviews.end(); ++it) {
        (*it)->clearFromRoot();
        (*it)->dispose();
    }
    _tutorialviews.clear();
    
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
    
    _tutorialroot->removeAllChildren();
    _tutorialviews = _level->getTutorialViews();
    for(auto it = _tutorialviews.begin(); it != _tutorialviews.end(); ++it) {
        (*it)->init(_tutorialroot, _assets, _level->getDrawScale());
    }
    
    _world->activateCollisionCallbacks(true);
    _world->onBeginContact = [this](b2Contact* contact) {
        _collision->beginContact(contact);
    };
    _world->onEndContact = [this](b2Contact* contact) {
        _collision->endContact(contact);
    };
    
	_splatCycle->setVisible(false);
	_splatCycle->setFrame(0);
	_splatFrame = 0.0f;
	_hasSplat = false;
    
    //reset the hud
    HUDController::reset(this, _worldnode, _assets, _rootnode, _input, _level->getBlender()->getPosition().x);
    
    
    
    _levelOffset = 0.0f;
    _worldnode->setPositionX(0.0f);
    _debugnode->setPositionX(0.0f);
    _background->reset(_worldnode);
    _isReloading = false;

    _fridgeDoor = PolygonNode::createWithTexture(_assets->get<Texture2D>(GOAL_DOOR_TEXTURE));
    _fridgeDoor->setScale(1.5f, 1.5f); // GOAL_SCALE
    _fridgeDoor->setPosition(_level->getDrawScale().x*_level->getGoal()->getPosition().x,
                             _level->getDrawScale().y*_level->getGoal()->getPosition().y);
    _fridgeDoor->setVisible(true);
    _fridgeDoor->retain();
    _worldnode->addChild(_fridgeDoor, GOAL_DOOR_Z);
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
		_level->getGoal()->setClosed(true);
        _rootnode->addChild(_winroot, WIN_SPLASH_Z);
        _winview->position();
        _winViewVisible = true;
        HUDController::setEnabled(false);
        // If we just beat the most recent level increment the saved state for level select controller
        // to see upon update
        int currentLevelsComplete = UserDefault::getInstance()->getIntegerForKey(LEVELS_COMPLETED_KEY);
        if (_levelIndex == currentLevelsComplete) {
            UserDefault::getInstance()->setIntegerForKey(LEVELS_COMPLETED_KEY, currentLevelsComplete + 1);
        }
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

void GameController::setTutorialVisible(TutorialView* view) {
    bool value = view != nullptr;
    if (value == _tutorialViewVisible) return;
    
    if (value) {
        _rootnode->addChild(_tutorialroot, TUTORIAL_SPLASH_Z);
        view->addToRoot();
        view->position();
        _activeTutorialView = view;
        HUDController::setEnabled(false);
    } else {
        _rootnode->removeChild(_tutorialroot);
        if (_activeTutorialView != nullptr) {
            _activeTutorialView->clearFromRoot();
        }
        _activeTutorialView = nullptr;
        HUDController::setEnabled(true);
    }
    
    _tutorialViewVisible = value;
}

float GameController::getBlenderVolScale() {
		float scale = NORMAL_BLENDER_DISTANCE / _level->getBlenderPineappleDistance();
		return scale > MAX_VOL_SCALE ? MAX_VOL_SCALE : scale;
}

void handleAvatarGrowth(int levelIndex, float cscale, InputController* _input, PineappleModel* _avatar) {
    int size = 0;
    float scale = 1.0f;
    if (_input->didGrow() && levelIndex >= MIN_GROW_LEVEL) {
        size = _avatar->grow();
    } else if (_input->didShrink() && levelIndex >= MIN_SHRINK_LEVEL) {
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
    //if (_input->didPause()) {
    //    if (!PauseController::isPaused()) {
    //        PauseController::pause();
    //        return;
    //    } else {
    //        PauseController::unPause();
    //    }
    //}
    //if (_input->didDebug()) {
    //    setDebug(!isDebug());
        //setTutorialVisible(_tutorialview != nullptr && !_tutorialViewVisible);
    //}
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
        _moveLeftView->setTouchEnabled(false);
        _moveRightView->setTouchEnabled(false);
        PauseController::animate();
        return;
    }
    
    configureMoveView(_moveLeftView);
    configureMoveView(_moveRightView);
    
    // Check for failure
    if (_level->haveFailed() && ! _loseViewVisible) {
        setFailure(true);
    }
    
    // Check for Victory
    if (checkForVictory() && ! _level->haveFailed() && ! _winViewVisible) {
        setComplete(true);
    }
    
    // Check for tutorials
    for(auto it = _tutorialviews.begin(); it != _tutorialviews.end(); ++it) {
        TutorialView* view = (*it);
        if (! view->isDismissed() && _level->getPineapple() != nullptr &&
            _level->getPineapple()->getPosition().x > view->getTriggerX()) {
            setTutorialVisible(view);
            break;
        }
    }
    
    if (_loseViewVisible)  {
        _moveLeftView->setTouchEnabled(false);
        _moveRightView->setTouchEnabled(false);
        _loseview->update(dt);
        
        if (_loseview->shouldReset()) {
            _loseview->resetButtons();
            reset();
            return;
        } else if (_loseview->shouldTransferToLevelSelect()) {
            _loseview->resetButtons();
            Sound* sound = AssetManager::getInstance()->getCurrent()->get<Sound>(GAME_BACKGROUND_SOUND);
            SoundEngine::getInstance()->playMusic(sound, true, MUSIC_VOLUME);//TODO CHECK THIS OUT FOR SOUNDS YO
            setTransitionStatus(TRANSITION_TO_LEVEL_SELECT);
            return;
        }
    } else if (_winViewVisible) {
        _moveLeftView->setTouchEnabled(false);
        _moveRightView->setTouchEnabled(false);
        _winview->update(dt);
        
        if (_winview->shouldReset()) {
            _winview->resetButtons();
            setTransitionStatus(TRANSITION_TO_RESET);
            return;
        } else if (_winview->shouldTransferToLevelSelect()) {
            _winview->resetButtons();
            Sound* sound = AssetManager::getInstance()->getCurrent()->get<Sound>(GAME_BACKGROUND_SOUND);
            SoundEngine::getInstance()->playMusic(sound, true, MUSIC_VOLUME);
            setTransitionStatus(TRANSITION_TO_LEVEL_SELECT);
            return;
        } else if (_winview->shouldTransferToNextLevel()) {
            _winview->resetButtons();
            setTransitionStatus(TRANSITION_TO_NEXT_LEVEL);
            return;

        }
    } else if (_tutorialViewVisible) {
        _activeTutorialView->update(dt);
        
        if (_activeTutorialView->isDismissed()) {
            _activeTutorialView->resetButtons();
            setTutorialVisible(nullptr);
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
                        activateSplat(_assets->get<Texture2D>(_level->getKid(i)->getSplatTexture(i)));
                        _level->kill(_level->getKid(i));
                        _level->getBlender()->setIsBlending(true);
                    }
                }
                // check if off bounds death
                if (_level->getKid(i) != nullptr && _level->getKid(i)->getPosition().y < 0) {
									char* key;
									switch (i) {
									case 0: key = PINEAPPLET1_DEATH_SOUND;
										break;
									case 1: key = PINEAPPLET2_DEATH_SOUND;
										break;
									case 2: key = PINEAPPLET3_DEATH_SOUND;
										break;
									case 3: key = PINEAPPLET4_DEATH_SOUND;
										break;
									default:
                                            key = "we gon crash if this happens, but it won't so it's chill.";
                                            CC_ASSERT(false);
									}
									Sound* source = AssetManager::getInstance()->getCurrent()->get<Sound>(key);
									SoundEngine::getInstance()->playEffect(key, source, false, EFFECT_VOLUME);
                  _level->kill(_level->getKid(i));
                }
            }
        }
        
        // Process the movement
        if (_level->getPineapple() != nullptr) {
            if (!_level->getPineapple()->getIsBlended()) {
                _level->getPineapple()->setMovement(_input->getHorizontal()*_level->getPineapple()->getForce());
                _level->getPineapple()->setJumping(_input->didJump() && _levelIndex >= MIN_JUMP_LEVEL);
                float cscale = Director::getInstance()->getContentScaleFactor();
                
                handleAvatarGrowth(_levelIndex, cscale, _input, _level->getPineapple());
                
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

		// Animate the fridge
		_level->getGoal()->animate();

		// Animate cups if they're being smashed and remove them when they're done
		std::vector<CrushableModel*> cups = _level->getCups();
		for (auto it = cups.begin(); it != cups.end(); ++it) {
			CrushableModel* cup = *it;
			
			cup->animate();
			if (cup->getSmashed()) {
				_level->removeObstacle(cup);
				//cups.erase(it);
			}
		}
        
        // Animate the blender
        _level->getBlender()->animate();

		// Animate the splats
		if (_hasSplat) {
			_splatFrame += 0.25f;
			int tmp = (int)rint(_splatFrame);
			if (tmp < _splatCycle->getSize()) {
				_splatCycle->setFrame(tmp);
			} 
			else {
				_hasSplat = false;
				_splatCycle->setVisible(false);
			}
		}
        
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
void GameController::activateSplat(Texture2D* image) {
	_splatCycle->setTexture(image);
	_splatCycle->setFrame(0);
	_splatCycle->setScale(4.0f, 4.0f);
	_splatCycle->setVisible(true);
	_splatFrame = 0.0f;
	_hasSplat = true;

	// Randomize location of splat on screen 
	int windowX = (int)rint(_rootSize.width / 2.0f);
	int windowY = (int)rint(_rootSize.height / 2.0f);
	float randX = (float)((rand() % windowX) + (_rootSize.width / 4.0f));
	float randY = (float)((rand() % windowY) + (_rootSize.height / 4.0f));
	_splatCycle->setPosition(randX, randY);
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
