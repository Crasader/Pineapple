#include "LevelModel.h"

#define UNSET_LENGTH_PLATFORM_COUNT_WALL_COUNT -2

/**
*	Will replace this constructor with some kind of populate/build level via levelController
*/
LevelModel::LevelModel() :
_goalDoor(nullptr),
_kids(),
_pineapple(nullptr),
_blender(nullptr),
_failed(false),
_walls(nullptr),
_platforms(nullptr),
_length(UNSET_LENGTH_PLATFORM_COUNT_WALL_COUNT),
_platformCount(UNSET_LENGTH_PLATFORM_COUNT_WALL_COUNT),
_wallCount(UNSET_LENGTH_PLATFORM_COUNT_WALL_COUNT),
_kidsRemaining(KID_COUNT) {}

LevelModel* LevelModel::create(RootLayer* rootnode, Node* worldnode, Node* debugnode, WorldController* world) {
	LevelModel* l = new (std::nothrow) LevelModel();
    l->_rootnode = rootnode;
    l->_worldnode = worldnode;
    l->_debugnode = debugnode;
    l->_world = world;
    return l;
}

void LevelModel::dispose() {
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

LevelModel::~LevelModel(){
    dispose();
}

/**
 * Immediately adds the object to the physics world.
 * Should not be called directly, shuold only be called as part of another add function
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
void LevelModel::addObstacle(Obstacle* obj, int zOrder) {
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

void LevelModel::addGoal(BoxObstacle* goal) {
    if (_goalDoor == nullptr) {
        _goalDoor = goal;
        addObstacle(_goalDoor, GOAL_Z_INDEX);
    } else {
        CC_ASSERT(false);
    }
}

void LevelModel::addPineapple(PineappleModel* will) {
	if (_pineapple == nullptr) {
		_pineapple = will;
        addObstacle(_pineapple, PINEAPPLE_Z_INDEX);
    } else {
        CC_ASSERT(false);
    }
}

void LevelModel::addBlender(BlenderModel* blender) {
	if (_blender == nullptr) {
		_blender = blender;
        addObstacle(_blender, BLENDER_Z_INDEX);
    } else {
        CC_ASSERT(false);
    }
}

void LevelModel::addKids(KidModel* kids[KID_COUNT]) {
	if (_kids[0] == nullptr) {
		for (int i = 0; i < KID_COUNT; i++) {
			_kids[i] = kids[i];
            addObstacle(_kids[i], KID_Z_INDEX+i);
		}
    } else {
        CC_ASSERT(false);
    }
}

void LevelModel::addLength(float length) {
    if (_length == UNSET_LENGTH_PLATFORM_COUNT_WALL_COUNT) {
        _length = length;
    } else {
        CC_ASSERT(false);
    }
}

void LevelModel::addPlatformCount(int platformCount) {
    if (_platformCount == UNSET_LENGTH_PLATFORM_COUNT_WALL_COUNT) {
        _platformCount = platformCount;
    } else {
        CC_ASSERT(false);
    }
}

void LevelModel::addWallCount(int wallCount) {
    if (_wallCount == UNSET_LENGTH_PLATFORM_COUNT_WALL_COUNT) {
        _wallCount = wallCount;
    } else {
        CC_ASSERT(false);
    }
}

void LevelModel::addWalls(PolygonObstacle **walls) {
    if (_walls == nullptr) {
        _walls = walls;
        for(int i = 0; i < _wallCount; i++) {
            addObstacle(_walls[i], WALL_Z_INDEX);
        }
    } else {
        CC_ASSERT(false);
    }
}

void LevelModel::addPlatforms(PolygonObstacle **platforms) {
    if (_platforms == nullptr) {
        _platforms = platforms;
        for(int i = 0; i < _platformCount; i++) {
            addObstacle(_platforms[i], PLATFORM_Z_INDEX);
        }
    } else {
        CC_ASSERT(false);
    }
}

void LevelModel::addScale(Vec2 scale) {
    if (scale.x <= 0 || scale.y <= 0) {
        CC_ASSERT(false);
    }
    if (_scale == nullptr) {
        _scale = scale;
    } else {
        CC_ASSERT(false);
    }
}

void LevelModel::addAnonymousObstacle(cocos2d::Obstacle *obj, int zOrder) {
    addObstacle(obj, zOrder);
}

void LevelModel::kill(PineappleModel* will) {
	removeObstacle(will);
	clearPineapple();
	setFailure(true); //TODO: Move failure to main game loop?
}

void LevelModel::kill(KidModel* kid) {
	removeObstacle(kid);
	clearKid(kid->getIndex());
	subtractKidFromCount();
	// TODO: Move failure check to main game loop?
	if (_kidsRemaining == 0) {
		setFailure(true);
	}
}

void LevelModel::blendAndKill(PineappleModel* will) {
	kill(will);

	//TODO: Animation and sounds
}

void LevelModel::blendAndKill(KidModel* kid) {
	kill(kid);

	//TODO: Animation and sounds
}

void LevelModel::spikeAndKill(PineappleModel* will) {
	kill(will);

	//TODO: Animation and sounds
}

void LevelModel::spikeAndKill(KidModel* kid) {
	kill(kid);

	//TODO: Animation and sounds
}
