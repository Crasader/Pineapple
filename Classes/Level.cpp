#include "Level.h"

/**
*	Will replace this constructor with some kind of populate/build level via levelController
*/
Level::Level() :
_goalDoor(nullptr),
_kids(nullptr),
_pineapple(nullptr),
_blender(nullptr),
_failed(false),
_kidsRemaining(KID_COUNT) {}

Level* Level::create() {
	return new (std::nothrow) Level();
}