#include "Level.h"

/**
*	Will replace this constructor with some kind of populate/build level via levelController
*/
Level::Level() :
_goalDoor(nullptr),
_kids(),
_pineapple(nullptr),
_blender(nullptr),
_failed(false),
_kidsRemaining(KID_COUNT) {}

Level* Level::create() {
	return new (std::nothrow) Level();
}

Level::~Level(){
    
    //TODO - pre-nulling cleanup
    
    clearPineapple();
    for(int i = 0; i < KID_COUNT; i++) {
        clearKid(i);
    }
    _goalDoor = nullptr;
    _blender = nullptr;
    _rootnode = nullptr;
    _worldnode = nullptr;
    _world = nullptr;
    _debugnode = nullptr;
    _kidsReachedGoal = nullptr;
    for(int i = 0; i < _platformCount; i++) {
        _platforms[i] = nullptr;
    }
    _platforms = nullptr;
    for(int i = 0; i < _wallCount; i++) {
        _walls[i] = nullptr;
    }
    _walls = nullptr;
}

void Level::addPineapple(Pineapple* will) {
	if (_pineapple == nullptr) {
		_pineapple = will;
    } else {
        CC_ASSERT(false);
    }
}

void Level::addBlender(BlenderModel* blender) {
	if (_blender == nullptr) {
		_blender = blender;
    } else {
        CC_ASSERT(false);
    }
}

void Level::addKids(KidModel* kids[KID_COUNT]) {
	if (_kids[0] == nullptr) {
		for (int i = 0; i < KID_COUNT; i++) {
			_kids[i] = kids[i];
		}
    } else {
        CC_ASSERT(false);
    }
}