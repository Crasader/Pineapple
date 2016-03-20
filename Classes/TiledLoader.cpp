#include "TiledLoader.h"

#define WALL_NAME "wall"
#define PLATFORM_NAME "platform"
#define JELLO_NAME "jello"
#define SPIKE_NAME "spike"
#define CUP_NAME "cup"

#define WALL_VERTS 8
#define PLATFORM_VERTS 8

TiledLoader::TiledLoader() :
_assets(nullptr),
_scale(nullptr),
_level(nullptr){}

TiledLoader* TiledLoader::create(SceneManager* assets, Vec2 scale) {
    TiledLoader* loader = new (std::nothrow) TiledLoader();
    
    loader->_assets = assets;
    loader->_scale = scale;
    loader->_level = nullptr;
    return loader;
}


/** Helper method for creating all level elements that initializes the
 * debug properties of the object to be added */
void initDebugProperties(Obstacle* obstacle) {
    WireNode* draw = WireNode::create();
    draw->setColor(DEBUG_COLOR);
    draw->setOpacity(DEBUG_OPACITY);
    obstacle->setDebugNode(draw);
}

/** Helper method for creating sensors (and non-physical obstacles) */
void initSensor(Obstacle* obstacle) {
    obstacle->setBodyType(b2_staticBody);
    obstacle->setDensity(0.0f);
    obstacle->setFriction(0.0f);
    obstacle->setRestitution(0.0f);
    obstacle->setGravityScale(0);
    obstacle->setSensor(true);
}

/** Helper method for creating physical obstacles */
void initPhysicalObstacle(Obstacle* obstacle) {
    obstacle->setBodyType(b2_staticBody);
    obstacle->setDensity(BASIC_DENSITY);
    obstacle->setFriction(BASIC_FRICTION);
    obstacle->setRestitution(BASIC_RESTITUTION);
    obstacle->setGravityScale(0);
    obstacle->setSensor(false);
}

Level* TiledLoader::read(string filename) {
    if(_level != nullptr) {
        _level->dispose();
    }
    Level* level = Level::create();
    
    
    return level;
}

void TiledLoader::addGoalDoor(Vec2 goalPos) {
    Texture2D* image = _assets->get<Texture2D>(GOAL_TEXTURE);
    
    Size goalSize(image->getContentSize().width*cscale/_scale.x,
                  image->getContentSize().height*cscale/_scale.y);
    BoxObstable* goalDoor = BoxObstacle::create(goalPos,goalSize);
    goalDoor->setDrawScale(_scale.x, _scale.y);
    
    PolygonNode* sprite = PolygonNode::createWithTexture(image);
    sprite->setScale(cscale);
    goalDoor->setSceneNode(sprite);
    
    initDebugProperties(goalDoor);
    initSensor(goalDoor);
    _level->addGoal(goalDoor);
}

void TiledLoader::addWalls(int wallCount, float** wallPos) {
    _level->addWallCount(wallCount);
    Texture2D* image  = _assets->get<Texture2D>(TILE_TEXTURE);
    PolygonObstacle** walls = new PolygonObstacle[wallCount];
    for (int ii = 0; ii < wallCount; ii++) {
        Poly2 wallPoly(wallPos[ii],WALL_VERTS);
        wallPoly.triangulate();
        PolygonObstacle* wallobj = PolygonObstacle::create(wallPoly);
        wallobj->setDrawScale(_scale.x, _scale.y);
        // Add the scene graph nodes to this object
        wallPoly *= _scale;
        sprite = PolygonNode::createWithTexture(image,wallPoly);
        wallobj->setSceneNode(sprite);
        
        initDebugProperties(wallobj);
        initPhysicalObstacle(wallobj);
        wallobj->setName(WALL_NAME);
        walls[ii] = wallobj;
    }
    _level->addWalls(walls);
}

void TileLoader::addPlatforms(int platformCount, float** platformPos) {
    _level->addPlatformCount(platformCount);
    Texture2D* image  = _assets->get<Texture2D>(PLATFORM_TEXTURE);
    PolygonObstacle** platforms = new PolygonObstacle[platformCount];
    for (int ii = 0; ii < PLATFORM_COUNT; ii++) {
        Poly2 platformPoly(platformPos[ii],PLATFORM_VERTS);
        platformPoly.triangulate();
        PolygonObstacle* platformObj = PolygonObstacle::create(platformPoly);
        platformObj->setDrawScale(_scale.x , _scale.y);
                
        // Add the scene graph nodes to this object
        platformPoly *= _scale / 1.95f; //This is janky AF.....
        sprite = PolygonNode::createWithTexture(image,platformPoly);
        sprite->setScale(1.95f);
        platformObj->setSceneNode(sprite);
        
        initDebugProperties(platformObj);
        initPhysicalObstacle(platformObj);
        platformObj->setName(PLATFORM_NAME);
        platforms[ii] = platformObj;
    }
    _level->addPlatforms(platforms);
}


void TiledLoader::addPineapple(Vec2 pineapplePos) {
    Texture2D* image  = _assets->get<Texture2D>(PINEAPPLE_TEXTURE);
    Pineapple* will = Pineapple::create(pineapplePos,_scale / PINEAPPLE_SCALE);
    will->setDrawScale(_scale);
    
    PolygonNode* sprite = PolygonNode::createWithTexture(image);
    sprite->setScale(cscale * PINEAPPLE_SCALE);
    will->setSceneNode(sprite);

    
    b2Filter b = b2Filter();
    b.categoryBits = PINEAPPLE_MASK;
    b.maskBits = PINEAPPLE_COLLIDES_WITH;
    will->setFilterData(b);
    
    initDebugProperties(will);
    _level->addPineapple(will);
}

void TiledLoader::addKids(Vec2* kidPos) {
    KidModel** kids = new KidModel[KID_COUNT];
    for (int i = 0; i < KID_COUNT; i++) {
        Vec2 kidPos = kidPos[i];
        Texture2D* image = _assets->get<Texture2D>(KidModel::getTexture(i));
        kids[i] = KidModel::create(kidPos,_scale / KID_SCALE, i);
        kids[i]->setDrawScale(_scale);
        
        PolygonNode* sprite = PolygonNode::createWithTexture(image);
        sprite->setScale(cscale * KID_SCALE);
        kids[i]->setSceneNode(sprite);
        
        kids[i]->setMovement(KID_WALKSPEED);
        
        b2Filter b = b2Filter();
        b.categoryBits = KID_MASK;
        b.maskBits = KID_COLLIDES_WITH;
        kids[i]->setFilterData(b);
        kids[i]->setName(KID_NAME);
        
        initDebugProperties(kids[i]);
    }
    _level->addKids(kids);
}

void TiledLoader::addJello(int jelloCount, Vec2* jelloPos) {
    for(int i = 0; i < jelloCount; i++) {
        Vec2 jelloPos = jelloPos[i];
        Texture2D* image  = _assets->get<Texture2D>(JELLO_TEXTURE);
        JelloModel* jello = JelloModel::create(jelloPos,_scale / JELLO_SCALE);
        jello->setDrawScale(_scale.x, _scale.y);
        
        PolygonNode* sprite = PolygonNode::createWithTexture(image);
        sprite->setScale(cscale * JELLO_SCALE);
        jello->setSceneNode(sprite);
        
        initDebugProperties(jello);
        initSensor(jello);
        jello->setName(JELLO_NAME);
        _level->addObstacle(jello, JELLO_Z_INDEX);
    }
}

void TiledLoader::addCups(int cupCount, Vec2* cupPos) {
    for (int i = 0; i < cupCount; i++) {
        Vec2 cupPos = cupCount[i];
        Texture2D* image = _assets->get<Texture2D>(BLUE_CUP_TEXTURE);
        PolygonNode* sprite = PolygonNode::createWithTexture(image);
        CrushableModel* cup = CrushableModel::create(BLUE_CUP_TEXTURE, cupPos, _scale / CRUSHABLE_SCALE);
        cup->setDrawScale(_scale);
        
        PolygonNode* sprite = PolygonNode::createWithTexture(image);
        sprite->setScale(cscale * CRUSHABLE_SCALE);
        cup->setSceneNode(sprite);
        
        initPhysicalObstacle(cup, CUP_NAME);
        initDebugProperties(cup);
        _level->addObstacle(cup, CUP_Z_INDEX);
    }
}

void TiledLoader::addSpikes(int spikeCount, Vec2* spikesPos) {
    for(int i = 0; i < spikeCount; i++) {
        Vec2 spikePos = spikePos[i];
        Texture2D* image  = _assets->get<Texture2D>(SPIKE_TEXTURE);
        SpikeModel* spike = SpikeModel::create(spikePos,_scale / SPIKE_SCALE);
        spike->setDrawScale(_scale.x, _scale.y);
        
        PolygonNode* sprite = PolygonNode::createWithTexture(image);
        sprite->setScale(cscale * SPIKE_SCALE);
        spike->setSceneNode(sprite);
        
        initDebugProperties(spike);
        initSensor(spike);
        spike->setName(SPIKE_NAME);
        _level->addObstacle(spike, SPIKE_Z_INDEX);
    }
}

void TiledLoader::addBlender(Vec2 blenderPos) {
    Texture2D* image  = _assets->get<Texture2D>(BLENDER_TEXTURE);
    BlenderModel* blender = BlenderModel::create(blenderPos,_scale / BLENDER_SCALE);
    blender->setDrawScale(_scale.x, _scale.y);
    
    PolygonNode* sprite = PolygonNode::createWithTexture(image);
    sprite->setScale(cscale * BLENDER_SCALE);
    blender->setSceneNode(sprite);
    
    b = b2Filter();
    b.categoryBits = BLENDER_MASK;
    b.maskBits = BLENDER_COLLIDES_WITH;
    blender->setFilterData(b);
    
    initDebugProperties(blender);
    initSensor(blender);
    _level->addBlender(blender);
}

