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
    
    _gameRoot = Node::create();
    _gameRoot->setContentSize(getContentSize());
    _gameRoot->retain();
    _levelSelectRoot = Node::create();
    _levelSelectRoot->setContentSize(getContentSize());
    _levelSelectRoot->retain();
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
    
    // Create a "loading" screen
    _preloaded = false;
    
    _gameplay.init(_gameRoot);
    _levelSelect.init(_levelSelectRoot);
    _loadingScreen.init(_loadingScreenRoot);
    
    _loadingScreen.setTransitionStatus(TRANSITION_TO_VISIBLE);
    
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
    
    removeAllChildren();

    _gameRoot->release();
    _levelSelectRoot->release();
    _loadingScreenRoot->release();
    
    SoundEngine::getInstance()->stopAll();
    AssetManager::getInstance()->stopScene(scene);
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
        CCLOG("No active controller");
        return;
    }
    
    //Check for exit
    if (_activeController->getTransitionStatus() == TRANSITION_TO_EXIT) {
        shutdown();
        return;
    }
    
    bool complete = AssetManager::getInstance()->getCurrent()->isComplete();
    
    if (_preloaded && !_gameplay.isActive() && complete) {
        // Transfer control to the gameplay subcontroller
        
    } else if (_gameplay.isActive()) {
        _gameplay.update(deltaTime);
    } else if (!_preloaded) {
        _preloaded = true;
        _loadingScreen.preload();
    }
    
    //Check for transitions
    if ((_gameplay.getTransitionStatus() == TRANSITION_TO_LEVEL_SELECT && _activeController == &_gameplay) ||
        (_loadingScreen.getTransitionStatus() == TRANSITION_TO_LEVEL_SELECT && _activeController == &_loadingScreen) ) {
        
        _levelSelect.setTransitionStatus(TRANSITION_TO_VISIBLE);
    }
    
    if ((_levelSelect.getTransitionStatus() == TRANSITION_TO_GAME && _activeController == &_levelSelect) ||
        (_loadingScreen.getTransitionStatus() == TRANSITION_TO_GAME && _activeController == &_loadingScreen)) {
        
        _gameplay.setTransitionStatus(TRANSITION_TO_VISIBLE);
    }
    
    
    //Transitioning to gameplay
    if (_gameplay.getTransitionStatus() == TRANSITION_TO_VISIBLE) {
        
        if (_activeController == &_levelSelect) {
            removeChild(_levelSelectRoot);
        }
        
        if (_activeController == &_loadingScreen) {
            removeChild(_loadingScreenRoot);
        }
        
        _activeController = &_gameplay;
        addChild(_gameRoot, GAME_ROOT_Z);
    }
    
    //Transitioning to level select
    if (_levelSelect.getTransitionStatus() == TRANSITION_TO_VISIBLE) {
        
        if (_activeController == &_gameplay) {
            removeChild(_gameRoot);
        }
        
        if (_activeController == &_loadingScreen) {
            removeChild(_loadingScreenRoot);
        }
        
        _activeController = &_levelSelect;
        addChild(_levelSelectRoot, LEVEL_SELECT_ROOT_Z);
    }
    
    
    //Transitioning to loadingscreen
    if (_loadingScreen.getTransitionStatus() == TRANSITION_TO_VISIBLE) {
        
        if (_activeController == &_levelSelect) {
            removeChild(_levelSelectRoot);
        }
        
        if (_activeController == &_gameplay) {
            removeChild(_gameRoot);
        }
        
        _activeController = &_loadingScreen;
        addChild(_loadingScreenRoot, LOADING_ROOT_Z);
    }
    
    //Do the updating
    _activeController->update(deltaTime);
}



