#include "LevelController.h"

#define WALL_NAME     "wall"
#define PLATFORM_NAME "platform"
#define JELLO_NAME    "jello"
#define SPIKE_NAME    "spike"
#define CUP_NAME      "cup"

#define WALL_VERTS      8
#define PLATFORM_VERTS  8
#define POS_COORDS      2

#define MAIN_PLATFORM_Y         2.0f        //DELETE ONCE LEVEL READING IN
#define SECOND_PLATFORM_Y       4.5f        //DELETE ONCE LEVEL READING IN
#define SECOND_PLATFORM_HEIGHT  0.5f        //DELETE ONCE LEVEL READING IN

#define JELLO_COUNT 1                       //DELETE ONCE LEVEL READING IN
#define SPIKE_COUNT 1                       //DELETE ONCE LEVEL READING IN
#define CUP_COUNT   1                       //DELETE ONCE LEVEL READING IN

#define LEVEL_LENGTH            40.0f       //DELETE ONCE LEVEL READING IN
#define WALL_COUNT              3           //DELETE ONCE LEVEL READING IN
#define PLATFORM_COUNT          1           //DELETE ONCE LEVEL READING IN
#define FLOOR_EXTRA_LENGTH      5.0f        //DELETE ONCE LEVEL READING IN
#define OFFSCREEN_BARRIER_WIDTH 3.0f        //DELETE ONCE LEVEL READING IN


LevelController::LevelController() :
_assets(nullptr),
_level(nullptr){}

LevelController* LevelController::create(SceneManager* assets, RootLayer* rootnode,
                                 Node* worldnode, Node* debugnode,
                                 WorldController* world, Vec2 scale) {
    LevelController* loader = new (std::nothrow) LevelController();

    loader->_assets = assets;
    loader->_rootnode = rootnode;
    loader->_worldnode = worldnode;
    loader->_debugnode = debugnode;
    loader->_world = world;
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

LevelModel* LevelController::read(string filename) {
    if(_level != nullptr) {
        _level->dispose();
    }
    
    //------------ BEGIN DEFAULT LEVEL SECTION ------------------------------

    float** walls = new float* [WALL_COUNT];
    walls[0] = new float[WALL_VERTS]{
                    -FLOOR_EXTRA_LENGTH, 0.0f,
                    LEVEL_LENGTH + FLOOR_EXTRA_LENGTH, 0.0f,
                    LEVEL_LENGTH + FLOOR_EXTRA_LENGTH, MAIN_PLATFORM_Y,
                    -FLOOR_EXTRA_LENGTH, MAIN_PLATFORM_Y
                };
    walls[1] = new float[WALL_VERTS]{
                    0.0f, 0.0f,
                    0.0f, DEFAULT_HEIGHT,
                    -OFFSCREEN_BARRIER_WIDTH, DEFAULT_HEIGHT,
                    -OFFSCREEN_BARRIER_WIDTH, 0.0
                };
    walls[2] = new float[WALL_VERTS]{
                    LEVEL_LENGTH, 0.0f,
                    LEVEL_LENGTH, DEFAULT_HEIGHT,
                    LEVEL_LENGTH + OFFSCREEN_BARRIER_WIDTH, DEFAULT_HEIGHT,
                    LEVEL_LENGTH + OFFSCREEN_BARRIER_WIDTH, 0.0
    };
    
    float** PLATFORM = new float*[PLATFORM_COUNT];
    PLATFORM[0] = new float[PLATFORM_VERTS]
        {17.0f, SECOND_PLATFORM_Y, 17.0f, SECOND_PLATFORM_Y + SECOND_PLATFORM_HEIGHT,
            20.0f, SECOND_PLATFORM_Y + SECOND_PLATFORM_HEIGHT, 20.0f, SECOND_PLATFORM_Y};
    
    float GOAL_POS[] = {38.0f, 3.0f};

    float PINEAPPLE_POS[] = {10.0f, 8.0f};

    float** KID_POS = new float* [KID_COUNT];
    KID_POS[0] = new float[POS_COORDS]{MAIN_PLATFORM_Y, 5.1f};
    KID_POS[1] = new float[POS_COORDS]{4.0f, 5.1f};
    KID_POS[2] = new float[POS_COORDS]{6.0f, 5.1f};
    KID_POS[3] = new float[POS_COORDS]{8.0f, 5.1f};
    
    float BLENDER_POS[] = {-14.0f, 7.0f};

    float** CUP_POS = new float* [CUP_COUNT];
    CUP_POS[0] = new float[POS_COORDS]{28.0f, 2.5f};

    float** JELLO_POS = new float* [JELLO_COUNT];
    JELLO_POS[0] = new float[POS_COORDS]{15.0f, MAIN_PLATFORM_Y};

    float** SPIKE_POS = new float* [SPIKE_COUNT];
    SPIKE_POS[0] = new float[POS_COORDS]{17.5f, MAIN_PLATFORM_Y};
    
    //------------ END SECTION --------------------------------------------
    
    _level = LevelModel::create(_rootnode, _worldnode, _debugnode, _world);
	_level->addLength(LEVEL_LENGTH);
    
    addGoal(GOAL_POS);
    addWalls(WALL_COUNT, walls);
    addPlatforms(PLATFORM_COUNT, PLATFORM);
    addPineapple(PINEAPPLE_POS);
    addKids(KID_POS);
    addCups(CUP_COUNT, CUP_POS);
    addJello(JELLO_COUNT, JELLO_POS);
    addSpikes(SPIKE_COUNT, SPIKE_POS);
    //addBlender(BLENDER_POS);
    
    addButtonSwitch(Vec2(5,5), true);
    
    return _level;
}

void LevelController::addGoal(float goalPos[POS_COORDS]) {
    float cscale = Director::getInstance()->getContentScaleFactor();
    Texture2D* image = _assets->get<Texture2D>(GOAL_TEXTURE);
    
    
    Size goalSize(image->getContentSize().width*cscale/_scale.x,
                  image->getContentSize().height*cscale/_scale.y);
    BoxObstacle* goalDoor = BoxObstacle::create(goalPos,goalSize);
    goalDoor->setDrawScale(_scale.x, _scale.y);
    
    PolygonNode* sprite = PolygonNode::createWithTexture(image);
    sprite->setScale(cscale);
    goalDoor->setSceneNode(sprite);
    
    initDebugProperties(goalDoor);
    initSensor(goalDoor);
    _level->addGoal(goalDoor);
}

void LevelController::addWalls(int wallCount, float** wallPos) {
    float cscale = Director::getInstance()->getContentScaleFactor();
    _level->addWallCount(wallCount);
    Texture2D* image  = _assets->get<Texture2D>(TILE_TEXTURE);
    PolygonObstacle** walls = new PolygonObstacle* [wallCount];
    for (int ii = 0; ii < wallCount; ii++) {
        Poly2 wallPoly(wallPos[ii],WALL_VERTS);
        wallPoly.triangulate();
        PolygonObstacle* wallobj = PolygonObstacle::create(wallPoly);
        wallobj->setDrawScale(_scale.x, _scale.y);
        // Add the scene graph nodes to this object
        wallPoly *= _scale;
        PolygonNode* sprite = PolygonNode::createWithTexture(image,wallPoly);
        wallobj->setSceneNode(sprite);
        
        initDebugProperties(wallobj);
        initPhysicalObstacle(wallobj);
        wallobj->setName(WALL_NAME);
        walls[ii] = wallobj;
    }
    _level->addWalls(walls);
}

void LevelController::addPlatforms(int platformCount, float* platformPos[PLATFORM_VERTS]) {
    float cscale = Director::getInstance()->getContentScaleFactor();
    _level->addPlatformCount(platformCount);
    Texture2D* image  = _assets->get<Texture2D>(PLATFORM_TEXTURE);
    PolygonObstacle** platforms = new PolygonObstacle* [platformCount];
    for (int ii = 0; ii < platformCount; ii++) {
        Poly2 platformPoly(platformPos[ii],PLATFORM_VERTS);
        platformPoly.triangulate();
        PolygonObstacle* platformObj = PolygonObstacle::create(platformPoly);
        platformObj->setDrawScale(_scale.x , _scale.y);
        
        // Add the scene graph nodes to this object
        platformPoly *= _scale / 1.95f; //This is janky AF.....
        PolygonNode*  sprite = PolygonNode::createWithTexture(image,platformPoly);
        sprite->setScale(1.95f);
        platformObj->setSceneNode(sprite);
        
        initDebugProperties(platformObj);
        initPhysicalObstacle(platformObj);
        platformObj->setName(PLATFORM_NAME);
        platforms[ii] = platformObj;
    }
    _level->addPlatforms(platforms);
}


void LevelController::addPineapple(float pineapplePos[POS_COORDS]) {
    float cscale = Director::getInstance()->getContentScaleFactor();
    Texture2D* image  = _assets->get<Texture2D>(PINEAPPLE_TEXTURE);
    PineappleModel* will = PineappleModel::create(pineapplePos,_scale / PINEAPPLE_SCALE);
    will->setDrawScale(_scale);
    
    /*PolygonNode* sprite = PolygonNode::createWithTexture(image);
    sprite->setScale(cscale * PINEAPPLE_SCALE);
    will->setSceneNode(sprite);*/
    
    b2Filter b = b2Filter();
    b.categoryBits = PINEAPPLE_MASK;
    b.maskBits = PINEAPPLE_COLLIDES_WITH;
    will->setFilterData(b);
    
    initDebugProperties(will);
	will->initAnimation(image, cscale * PINEAPPLE_SCALE);
    _level->addPineapple(will);
}

void LevelController::addKids(float* kidPos[POS_COORDS]) {
    float cscale = Director::getInstance()->getContentScaleFactor();
    KidModel** kids = new KidModel* [KID_COUNT];
    for (int i = 0; i < KID_COUNT; i++) {
        Vec2 pos = kidPos[i];
        Texture2D* image = _assets->get<Texture2D>(KidModel::getTexture(i));
        kids[i] = KidModel::create(pos,_scale / KID_SCALE, i);
        kids[i]->setDrawScale(_scale);
        
        /*PolygonNode* sprite = PolygonNode::createWithTexture(image);
        sprite->setScale(cscale * KID_SCALE);
        kids[i]->setSceneNode(sprite);*/
        
        kids[i]->setMovement(KID_WALKSPEED);
        
        b2Filter b = b2Filter();
        b.categoryBits = KID_MASK;
        b.maskBits = KID_COLLIDES_WITH;
        kids[i]->setFilterData(b);
        kids[i]->setName(KID_NAME);
        
        initDebugProperties(kids[i]);

		kids[i]->initAnimation(image, cscale * KID_SCALE);
    }
    _level->addKids(kids);
}

void LevelController::addJello(int jelloCount, float* jelloPos[POS_COORDS]) {
    float cscale = Director::getInstance()->getContentScaleFactor();
    for(int i = 0; i < jelloCount; i++) {
        Vec2 pos = jelloPos[i];
        Texture2D* image  = _assets->get<Texture2D>(JELLO_TEXTURE);
        JelloModel* jello = JelloModel::create(pos,_scale / JELLO_SCALE);
        jello->setDrawScale(_scale.x, _scale.y);
        
        PolygonNode* sprite = PolygonNode::createWithTexture(image);
        sprite->setScale(cscale * JELLO_SCALE);
        jello->setSceneNode(sprite);
        
        initDebugProperties(jello);
        initSensor(jello);
        jello->setName(JELLO_NAME);
        _level->addAnonymousObstacle(jello, JELLO_Z_INDEX);
    }
}

void LevelController::addCups(int cupCount, float* cupPos[POS_COORDS]) {
    float cscale = Director::getInstance()->getContentScaleFactor();
    for (int i = 0; i < cupCount; i++) {
        Vec2 pos = cupPos[i];
        Texture2D* image = _assets->get<Texture2D>(BLUE_CUP_TEXTURE);
        CrushableModel* cup = CrushableModel::create(BLUE_CUP_TEXTURE, pos, _scale / CRUSHABLE_SCALE);
        cup->setDrawScale(_scale);
        
        PolygonNode* sprite = PolygonNode::createWithTexture(image);
        sprite->setScale(cscale * CRUSHABLE_SCALE);
        cup->setSceneNode(sprite);
        
        cup->setName(CUP_NAME);
        initPhysicalObstacle(cup);
        initDebugProperties(cup);
        _level->addAnonymousObstacle(cup, CUP_Z_INDEX);
    }
}

void LevelController::addSpikes(int spikeCount, float* spikesPos[POS_COORDS]) {
    float cscale = Director::getInstance()->getContentScaleFactor();
    for(int i = 0; i < spikeCount; i++) {
        Vec2 spikePos = spikesPos[i];
        Texture2D* image  = _assets->get<Texture2D>(SPIKE_TEXTURE);
        SpikeModel* spike = SpikeModel::create(spikePos,_scale / SPIKE_SCALE);
        spike->setDrawScale(_scale.x, _scale.y);
        
        PolygonNode* sprite = PolygonNode::createWithTexture(image);
        sprite->setScale(cscale * SPIKE_SCALE);
        spike->setSceneNode(sprite);
        
        initDebugProperties(spike);
        initSensor(spike);
        spike->setName(SPIKE_NAME);
        _level->addAnonymousObstacle(spike, SPIKES_Z_INDEX);
    }
}

void LevelController::addBlender(float blenderPos[POS_COORDS]) {
    float cscale = Director::getInstance()->getContentScaleFactor();
    Texture2D* image  = _assets->get<Texture2D>(BLENDER_TEXTURE);
    BlenderModel* blender = BlenderModel::create(blenderPos,_scale / BLENDER_SCALE);
    blender->setDrawScale(_scale.x, _scale.y);
    
    PolygonNode* sprite = PolygonNode::createWithTexture(image);
    sprite->setScale(cscale * BLENDER_SCALE);
    blender->setSceneNode(sprite);
    
    b2Filter b = b2Filter();
    b.categoryBits = BLENDER_MASK;
    b.maskBits = BLENDER_COLLIDES_WITH;
    blender->setFilterData(b);
    
    initDebugProperties(blender);
    initSensor(blender);
    _level->addBlender(blender);
}

void LevelController::addButtonSwitch(const Vec2 pos, const bool isSwitch) {
    float cscale = Director::getInstance()->getContentScaleFactor();
    Texture2D* image  = _assets->get<Texture2D>(SWITCH_TEXTURE_RED);
    float scale;
    if (isSwitch) {
        scale = SWITCH_SCALE;
    } else {
        scale = BUTTON_SCALE;
    }
    ButtonSwitchModel* button_switch = ButtonSwitchModel::create(pos,_scale / scale, isSwitch, Color::red);
    button_switch->setDrawScale(_scale.x, _scale.y);
    
    PolygonNode* sprite = PolygonNode::createWithTexture(image);
    sprite->setScale(cscale * scale);
    button_switch->setSceneNode(sprite);
    
    b2Filter b = b2Filter();
    b.categoryBits = BLENDER_MASK;
    b.maskBits = BLENDER_COLLIDES_WITH;
    button_switch->setFilterData(b);
    
    initDebugProperties(button_switch);
    initSensor(button_switch);
    _level->addAnonymousObstacle(button_switch, BUTTON_SWITCH_Z_INDEX);
    
    MoveablePlatformModel* platform = MoveablePlatformModel::create(Vec2(15.0f, 5.0f), _scale, 12.0f, false, false, Color::red);
    initPhysicalObstacle(platform);
    initDebugProperties(platform);
    
    platform->setFilterData(b);
    
    Node* node = Node::create();
    platform->setSceneNode(node);
    
    button_switch->linkToPlatform(platform);
    _level->addAnonymousObstacle(platform, BUTTON_SWITCH_Z_INDEX);
}
