//
//  PineappleRoot.cpp
//  PlatformerDemo
//
//  This is the root class for a single game scene.  It shows how to subclass RootLayer to
//  make a new game.  This is a standard design pattern that we have used in all our samples.
//  You may find that you want more than one scene in your game, but that is not always
//  necessary.  You may find it easier to work with a single scene.
//
//  This file is based on the CS 3152 PhysicsDemo Lab by Don Holden, 2007
//
//  Author: Walker White
//  Version: 1/15/15
//
#include "PineappleRoot.h"
#include "LoadingScreenController.h"
#include <cornell/CUGenericLoader.h>

using namespace cocos2d;

#define HOME_ROOT_Z             1
#define GAME_ROOT_Z             2
#define LEVEL_SELECT_ROOT_Z     3
#define LOADING_ROOT_Z          4

#pragma mark -
#pragma mark Gameplay Control

/**
 * Starts the layer, allocating initial resources
 *
 * This method is used to bootstrap the game.  It should start up an asset manager
 * and load initial assets.
 */
void PineappleRoot::start() {
    int scene = AssetManager::getInstance()->createScene();
    
    _homeScreen = new HomeScreenController();
    _homeRoot = Node::create();
    _homeRoot->setContentSize(getContentSize());
    _homeRoot->retain();
    
    _gameplay = new GameController();
    _gameRoot = Node::create();
    _gameRoot->setContentSize(getContentSize());
    _gameRoot->retain();
    
    _levelSelect = new LevelSelectController();
    _levelSelectRoot = Node::create();
    _levelSelectRoot->setContentSize(getContentSize());
    _levelSelectRoot->retain();
    
    _loadingScreen = new LoadingScreenController();
    _loadingScreenRoot = Node::create();
    _loadingScreenRoot->setContentSize(getContentSize());
    _loadingScreenRoot->retain();
    
    FontLoader* fonts = FontLoader::create();
    fonts->setDefaultSize(DEFAULT_FONT_SIZE);
    AssetManager::getInstance()->at(scene)->attach<TTFont>(fonts);
    AssetManager::getInstance()->at(scene)->attach<Texture2D>(TextureLoader::create());
    AssetManager::getInstance()->at(scene)->attach<Sound>(SoundLoader::create());
    GenericLoader<LevelModel>* levels = GenericLoader<LevelModel>::create();
    AssetManager::getInstance()->at(scene)->attach<LevelModel>(levels);
    AssetManager::getInstance()->startScene(scene);
    
    _backgroundSound = nullptr;
    _loadStarted = false;
    _loadFinished = false;
    
    RootLayer::start(); // YOU MUST END with call to parent
}

/**
 * Stops the layer, releasing all resources
 *
 * This method is used to clean-up any allocation that occurred in either start or
 * update.  While Cocos2d does have rudimentary garbage collection, you still have
 * to release any objects that you have retained.
 */
void PineappleRoot::stop() {
    RootLayer::stop();  // YOU MUST BEGIN with call to parent
    int scene = AssetManager::getInstance()->getCurrentIndex();
    
    if (_homeScreen->isInitted()) {
        _homeScreen->dispose();
    }
    if (_gameplay->isInitted()) {
        _gameplay->dispose();
    }
    if (_levelSelect->isInitted()) {
        _levelSelect->dispose();
    }
    
    removeAllChildren();

    _homeRoot->release();
    _gameRoot->release();
    _levelSelectRoot->release();
    _loadingScreenRoot->release();
    
    SoundEngine::getInstance()->stopAll();
    AssetManager::getInstance()->stopScene(scene);
}

/** Set the game to the default properties - loading controller is active,
 * create input controller */
void PineappleRoot::onFirstUpdate() {
    Vec2 scale = Vec2(getContentSize().width/SCREEN.size.width,
                      getContentSize().height/SCREEN.size.height);
    Rect screen = SCREEN;
    screen.origin.x *= scale.x;    screen.origin.y *= scale.y;
    screen.size.width *= scale.x;  screen.size.height *= scale.y;
    
    _assets = AssetManager::getInstance()->getCurrent();
    
    _inputController.init(screen);
    _inputController.start();
    
    _gameplay->setTransitionStatus(TRANSITION_NONE);
    _loadingScreen->setTransitionStatus(TRANSITION_NONE);
    _levelSelect->setTransitionStatus(TRANSITION_NONE);
    
    _loadingScreen->init(_loadingScreenRoot);
    addChild(_loadingScreenRoot, LOADING_ROOT_Z);
    _activeController = _loadingScreen;
}

// Helper to update score after completing a level, didn't want to give gameplay controller access
// to LevelSelectController so decided to put this up here.  Because of this we need to think about where
// to call this.  Currently after beating a level you can transition to the next level, replay the current
// level, or go to level select.  After each we call updateScore here.
void updateScore(GameController* gameplay) {
    const char *lvlKey = LevelSelectController::LEVEL_KEYS[gameplay->getLevelIndex()].c_str();
    int prevScore = UserDefault::getInstance()->getIntegerForKey(lvlKey);
    int thisScore = gameplay->getScore();
    if (gameplay->isComplete() && thisScore > prevScore) {
        UserDefault::getInstance()->setIntegerForKey(lvlKey, thisScore);
    }
}

/** Helper method that transitions to home */
void PineappleRoot::transitionToHomeScreen() {
    if (_activeController == _gameplay) {
        removeChild(_gameRoot);
        addChild(_homeRoot, HOME_ROOT_Z);
    }
    
    if (_activeController == _loadingScreen) {
        removeChild(_loadingScreenRoot);
        addChild(_homeRoot, HOME_ROOT_Z);
    }
    
    if (_activeController == _levelSelect) {
        removeChild(_levelSelectRoot);
        addChild(_homeRoot, HOME_ROOT_Z);
    }
    
    _activeController = _homeScreen;
    
    if (!_homeScreen->isInitted()) {
        _homeScreen->init(_homeRoot, &_inputController);
    }
    
    if (_backgroundSoundKey != LEVEL_SELECT_HOME_SCREEN_BACKGROUND_SOUND) {
        if (_backgroundSound != nullptr) {
            if (SoundEngine::getInstance()->getMusicState() == SoundEngine::SoundState::PLAYING) {
                SoundEngine::getInstance()->stopMusic();
            }
        }
        
        _backgroundSoundKey = LEVEL_SELECT_HOME_SCREEN_BACKGROUND_SOUND;
        _backgroundSound = _assets->get<Sound>(_backgroundSoundKey);
        SoundEngine::getInstance()->playMusic(_backgroundSound, true, MUSIC_VOLUME);
        SoundEngine::getInstance()->setMusicVolume(MUSIC_VOLUME);
    }
    
    _homeScreen->setTransitionStatus(TRANSITION_NONE);
    _gameplay->setTransitionStatus(TRANSITION_NONE);
    _loadingScreen->setTransitionStatus(TRANSITION_NONE);
    _levelSelect->setTransitionStatus(TRANSITION_NONE);
}

/** Helper method that transitions to level select */
void PineappleRoot::transitionToLevelSelect() {
    if (_activeController == _homeScreen) {
        removeChild(_homeRoot);
        addChild(_levelSelectRoot, LEVEL_SELECT_ROOT_Z);
    }
    
    if (_activeController == _gameplay) {
        removeChild(_gameRoot);
        addChild(_levelSelectRoot, LEVEL_SELECT_ROOT_Z);
    }
    
    if (_activeController == _loadingScreen) {
        removeChild(_loadingScreenRoot);
        addChild(_levelSelectRoot, LEVEL_SELECT_ROOT_Z);
    }
    
    _activeController = _levelSelect;
    
    if (! _levelSelect->isInitted()) {
        _levelSelect->init(_levelSelectRoot, &_inputController);
    }
    
    // Updates to add a new button if new level was unlocked or update new high score
    _levelSelect->update();
    
    if (_backgroundSoundKey != LEVEL_SELECT_HOME_SCREEN_BACKGROUND_SOUND) {
        if (_backgroundSound != nullptr) {
            if (SoundEngine::getInstance()->getMusicState() == SoundEngine::SoundState::PLAYING) {
                SoundEngine::getInstance()->stopMusic();
            }
        }
        
        _backgroundSoundKey = LEVEL_SELECT_HOME_SCREEN_BACKGROUND_SOUND;
        _backgroundSound = _assets->get<Sound>(_backgroundSoundKey);
        SoundEngine::getInstance()->playMusic(_backgroundSound, true, MUSIC_VOLUME);
        SoundEngine::getInstance()->setMusicVolume(MUSIC_VOLUME);
    }
    
    _homeScreen->setTransitionStatus(TRANSITION_NONE);
    _gameplay->setTransitionStatus(TRANSITION_NONE);
    _loadingScreen->setTransitionStatus(TRANSITION_NONE);
    _levelSelect->setTransitionStatus(TRANSITION_NONE);

}

/** Helper method that transitions to gameplay on the given level */
void PineappleRoot::transitionToGame(int levelIndex) {
    string levelKey = LevelSelectController::LEVEL_KEYS[levelIndex];
    string levelFile = LevelSelectController::LEVEL_FILES[levelIndex];
    
    if (! _gameplay->isInitted()) {
        _gameplay->init(_gameRoot, &_inputController, levelIndex, levelKey, levelFile);
    } else {
        _gameplay->reset(levelIndex, levelKey, levelFile);
    }
    
    if (_backgroundSoundKey != GAME_BACKGROUND_SOUND) {
        if (_backgroundSound != nullptr) {
            SoundEngine::getInstance()->stopMusic();
        }
        
        _backgroundSoundKey = GAME_BACKGROUND_SOUND;
        _backgroundSound = _assets->get<Sound>(_backgroundSoundKey);
        SoundEngine::getInstance()->playMusic(_backgroundSound, true, MUSIC_VOLUME);
        SoundEngine::getInstance()->setMusicVolume(MUSIC_VOLUME);
    }
    
    if (_activeController == _homeScreen) {
        removeChild(_homeRoot);
        addChild(_gameRoot);
    }
    
    if (_activeController == _levelSelect) {
        removeChild(_levelSelectRoot);
        addChild(_gameRoot);
    }
    
    if (_activeController == _loadingScreen) {
        removeChild(_loadingScreenRoot);
        addChild(_gameRoot);
    }
    
    _activeController = _gameplay;
    
    _homeScreen->setTransitionStatus(TRANSITION_NONE);
    _gameplay->setTransitionStatus(TRANSITION_NONE);
    _loadingScreen->setTransitionStatus(TRANSITION_NONE);
    _levelSelect->setTransitionStatus(TRANSITION_NONE);
}

/**
 * Updates the game for a single animation frame
 *
 * This method is called every animation frame.  There is no draw() or render()
 * counterpoint to this method; drawing is done automatically in the scene graph.
 * However, this method is responsible for updating any transforms in the scene graph.
 *
 * @param  dt   the time in seconds since last update
 */
void PineappleRoot::update(float deltaTime) {
    RootLayer::update(deltaTime);  // YOU MUST BEGIN with call to parent
    
    if (_activeController == nullptr) {
        onFirstUpdate();
    }
    
    //Check for exit
    if (_activeController->getTransitionStatus() == TRANSITION_TO_EXIT) {
        shutdown();
        return;
    }
    
    //Check for transitions
    if ((_gameplay->getTransitionStatus() == TRANSITION_TO_HOME && _activeController == _gameplay) ||
        (_levelSelect->getTransitionStatus() == TRANSITION_TO_HOME && _activeController == _levelSelect) ||
        (_loadingScreen->getTransitionStatus() == TRANSITION_TO_HOME && _activeController == _loadingScreen) ) {
        transitionToHomeScreen();
    }

    
    if ((_homeScreen->getTransitionStatus() == TRANSITION_TO_LEVEL_SELECT && _activeController == _homeScreen) ||
        (_gameplay->getTransitionStatus() == TRANSITION_TO_LEVEL_SELECT && _activeController == _gameplay) ||
        (_loadingScreen->getTransitionStatus() == TRANSITION_TO_LEVEL_SELECT && _activeController == _loadingScreen) ) {
        transitionToLevelSelect();
    }
    
    if ((_levelSelect->getTransitionStatus() == TRANSITION_TO_GAME && _activeController == _levelSelect)) {
        transitionToGame(_levelSelect->getSelectedLevel());
        _levelSelect->clearSelectedLevel();
    }
    
    if (_gameplay->getTransitionStatus() == TRANSITION_TO_RESET && _activeController == _gameplay) {
        int levelIndex = _gameplay->getLevelIndex();
        transitionToGame(levelIndex);
    }
    if((_gameplay->getTransitionStatus() == TRANSITION_TO_NEXT_LEVEL && _activeController == _gameplay)) {
        int levelIndex = _gameplay->getLevelIndex() + 1;
        
        if (levelIndex < LEVELS_CREATED) {
            transitionToGame(levelIndex);
        } else {
            transitionToLevelSelect();
        }
    }
    
    //Do the updating
    _activeController->update(deltaTime);
    
    if (_levelSelect && _gameplay && _gameplay->isInitted() && _levelSelect->isInitted()) {
        // Check if we beat a level and update the score if needed
        updateScore(_gameplay);
    }
    
    if (! _loadFinished) {
        if (!_loadStarted) {
            _loadingScreen->preload();
            _loadStarted = true;
        } else if (AssetManager::getInstance()->getCurrent()->isComplete()) {
            _loadFinished = true;
            _loadingScreen->setTransitionStatus(TRANSITION_TO_HOME);
        }
    }
}



