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

#define GAME_ROOT_Z             1
#define LEVEL_SELECT_ROOT_Z     2
#define LOADING_ROOT_Z          3

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
    _preloaded = false;
    
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
    
    if (_gameplay->isInitted()) {
        _gameplay->dispose();
    }
    if (_levelSelect->isInitted()) {
        _levelSelect->dispose();
    }
    
    removeAllChildren();

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
    
    _loadingScreen->init(_loadingScreenRoot);
    _activeController = _loadingScreen;
    addChild(_loadingScreenRoot);
    _loadingScreen->setTransitionStatus(TRANSITION_NONE);
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
    if ((_gameplay->getTransitionStatus() == TRANSITION_TO_LEVEL_SELECT && _activeController == _gameplay) ||
        (_loadingScreen->getTransitionStatus() == TRANSITION_TO_LEVEL_SELECT && _activeController == _loadingScreen) ) {
        
        if (_activeController == _gameplay) {
            removeChild(_gameRoot);
        }
        
        if (_activeController == _loadingScreen) {
            removeChild(_loadingScreenRoot);
        }
        
        _activeController = _levelSelect;
        
        if (! _levelSelect->isInitted()) {
            _levelSelect->init(_levelSelectRoot, &_inputController);
        }
        
        if (_backgroundSoundKey != LEVEL_SELECT_BACKGROUND_SOUND) {
            if (_backgroundSound != nullptr) {
                SoundEngine::getInstance()->stopMusic();
            }
            
            _backgroundSoundKey = LEVEL_SELECT_BACKGROUND_SOUND;
            _backgroundSound = _assets->get<Sound>(_backgroundSoundKey);
            SoundEngine::getInstance()->playMusic(_backgroundSound, true, MUSIC_VOLUME);
            SoundEngine::getInstance()->setMusicVolume(MUSIC_VOLUME);
        }
        
        addChild(_levelSelectRoot, LEVEL_SELECT_ROOT_Z);
        
        _gameplay->setTransitionStatus(TRANSITION_NONE);
        _loadingScreen->setTransitionStatus(TRANSITION_NONE);
        _levelSelect->setTransitionStatus(TRANSITION_NONE);

    }
    
    if ((_levelSelect->getTransitionStatus() == TRANSITION_TO_GAME && _activeController == _levelSelect)) {
        
        string levelKey = LevelSelectController::LEVEL_KEYS[_levelSelect->getSelectedLevel()];
        string levelFile = LevelSelectController::LEVEL_FILES[_levelSelect->getSelectedLevel()];
        
        _levelSelect->clearSelectedLevel();
        
        removeChild(_levelSelectRoot);
        
        if (! _gameplay->isInitted()) {
            _gameplay->init(_gameRoot, &_inputController, levelKey, levelFile);
        } else {
            _gameplay->reset();
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
        
        _activeController = _gameplay;
        addChild(_gameRoot, GAME_ROOT_Z);
        
        _gameplay->setTransitionStatus(TRANSITION_NONE);
        _loadingScreen->setTransitionStatus(TRANSITION_NONE);
        _levelSelect->setTransitionStatus(TRANSITION_NONE);
    }
    
    //Do the updating
    _activeController->update(deltaTime);
    
    if (_activeController == _loadingScreen) {
        if (!_preloaded) {
            _loadingScreen->preload();
            _preloaded = true;
        } else if (AssetManager::getInstance()->getCurrent()->isComplete()) {
            _loadingScreen->setTransitionStatus(TRANSITION_TO_LEVEL_SELECT);
        }
    }
}



