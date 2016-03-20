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

Level* Level::create(BoxObstacle* goal, KidModel** kid, Pineapple* will, BlenderModel* blender, Node* debug, Node* world) {
	Level* level = new (std::nothrow) Level();
	if (level && level->init(goal, kid, will, blender, debug, world)) {
		return level;
	}
}

bool Level::init(BoxObstacle* goal, KidModel** kids, Pineapple* will, BlenderModel* blender, Node* debug, Node* world) {
	_goalDoor = goal;
	_kids = kids;
	_pineapple = will;
	_blender = blender;
	_debugnode = debug;
	_worldnode = world;
	return true;
}