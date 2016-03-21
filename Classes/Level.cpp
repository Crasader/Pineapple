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

void Level::addPineapple(Pineapple* will) {
	if (_pineapple == nullptr) {
		_pineapple = will;
	}
}

void Level::addBlender(BlenderModel* blender) {
	if (_blender == nullptr) {
		_blender = blender;
	}
}

void Level::addKids(KidModel* kids[KID_COUNT]) {
	if (_kids == nullptr) {
		for (int i = 0; i < KID_COUNT; i++) {
			_kids[i] = kids[i];
		}
	}
}