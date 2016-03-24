#include "Level.h"

#define UNSET_LENGTH_PLATFORM_COUNT_WALL_COUNT -2

/**
*	Will replace this constructor with some kind of populate/build level via levelController
*/
Level::Level() :
_goalDoor(nullptr),
_kids(),
_pineapple(nullptr),
_blender(nullptr),
_failed(false),
_length(UNSET_LENGTH_PLATFORM_COUNT_WALL_COUNT),
_platformCount(UNSET_LENGTH_PLATFORM_COUNT_WALL_COUNT),
_wallCount(UNSET_LENGTH_PLATFORM_COUNT_WALL_COUNT),
_kidsRemaining(KID_COUNT) {}

Level* Level::create() {
	return new (std::nothrow) Level();
}

Level::~Level(){
    
    //TODO - pre-nulling cleanup
    
    _pineapple = nullptr;
    for(int i = 0; i < KID_COUNT; i++) {
        _kids[i] = nullptr;
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
    _wallCount = UNSET_LENGTH_PLATFORM_COUNT_WALL_COUNT;
    _platformCount = UNSET_LENGTH_PLATFORM_COUNT_WALL_COUNT;
    _length = UNSET_LENGTH_PLATFORM_COUNT_WALL_COUNT;
}

void Level::addGoal(BoxObstacle* goal) {
    if (_goalDoor == nullptr) {
        _goalDoor = goal;
    } else {
        CC_ASSERT(false);
    }
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

void Level::addRootNode(RootLayer* root) {
    if (_rootnode == nullptr) {
        _rootnode = root;
    } else {
        CC_ASSERT(false);
    }
}

void Level::addWorldNode(Node* worldnode) {
    if (_worldnode == nullptr) {
        _worldnode = worldnode;
    } else {
        CC_ASSERT(false);
    }
}

void Level::addWorld(WorldController* world) {
    if (_world == nullptr) {
        _world = world;
    } else {
        CC_ASSERT(false);
    }
}

void Level::addDebugNode(Node* debugnode) {
    if (_debugnode == nullptr) {
        _debugnode = debugnode;
    } else {
        CC_ASSERT(false);
    }
}

void Level::addLength(float length) {
    if (_length == UNSET_LENGTH_PLATFORM_COUNT_WALL_COUNT) {
        _length = length;
    } else {
        CC_ASSERT(false);
    }
}

void Level::addPlatformCount(int platformCount) {
    if (_platformCount == UNSET_LENGTH_PLATFORM_COUNT_WALL_COUNT) {
        _platformCount = platformCount;
    } else {
        CC_ASSERT(false);
    }
}

void Level::addWallCount(int wallCount) {
    if (_wallCount == UNSET_LENGTH_PLATFORM_COUNT_WALL_COUNT) {
        _wallCount = wallCount;
    } else {
        CC_ASSERT(false);
    }
}

void Level::addWalls(PolygonObstacle **walls) {
    if (_walls == nullptr) {
        _walls = walls;
    } else {
        CC_ASSERT(false);
    }
}

void Level::addPlatforms(PolygonObstacle **platforms) {
    if (_platforms == nullptr) {
        _platforms = platforms;
    } else {
        CC_ASSERT(false);
    }
}

void Level::addScale(Vec2 scale) {
    if (scale.x <= 0 || scale.y <= 0) {
        CC_ASSERT(false);
    }
    if (_scale == nullptr) {
        _scale = scale;
    } else {
        CC_ASSERT(false);
    }
}

/**
 * Immediately adds the object to the physics world
 *
 * Objects have a z-order.  This is the order they are drawn in the scene
 * graph node.  Objects with the different textures should have different
 * z-orders whenever possible.  This will cut down on the amount of drawing done
 *
 * param obj The object to add
 * param zOrder The drawing order
 *
 * @retain a reference to the obstacle
 */
void Level::addObstacle(Obstacle* obj, int zOrder) {
    if (_world == nullptr || _worldnode == nullptr || _debugnode == nullptr) {
        CC_ASSERT(false);
    }
    
    _world->addObstacle(obj);  // Implicit retain
    if (obj->getSceneNode() != nullptr) {
        _worldnode->addChild(obj->getSceneNode(),zOrder);
    }
    if (obj->getDebugNode() != nullptr) {
        _debugnode->addChild(obj->getDebugNode(),zOrder);
    }
}
