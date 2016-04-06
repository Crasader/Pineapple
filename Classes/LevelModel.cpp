#include "LevelModel.h"
#include <base/CCValue.h>
#include "MoveablePlatformModel.h"

#define UNSET_LENGTH -2

#define WALL_VERTS      8
#define POS_COORDS      2

/** Layer groups used in Tiled Maps */
#define WALL_OBJECT_GROUP               "Walls"
#define GOAL_OBJECT_GROUP               "Goal"
#define JELLO_OBJECT_GROUP              "Jello"
#define SPIKES_OBJECT_GROUP             "Spikes"
#define KIDS_OBJECT_GROUP               "Kids"
#define WILL_OBJECT_GROUP               "Will"
#define CUP_OBJECT_GROUP                "Cups"
#define BUTTON_SWITCH_OBJECT_GROUP      "Switches"
#define MOVEABLE_PLATFORMS_GROUP        "Bridges"

/** Properties that Tiled Objects and maps posess */
/** Level Properties */
#define BLENDER_START_X_PROPERTY    "BlenderStartX"
#define BLENDER_Y_PROPERTY          "BlenderStartY"

/** All Object Properties */
#define WIDTH_PROPERTY      "width"
#define HEIGHT_PROPERTY     "height"
#define X_PROPERTY          "x"
#define Y_PROPERTY          "y"

/** ButtonSwitch + MoveablePlatform Properties */
#define COLOR_PROPERTY      "linkID"

/** ButtonSwitch Properties */
#define IS_SWITCH_PROPERTY  "isSwitch"

/** MoveablePlatform Properties */
#define LENGTH_PROPERTY         "length"
#define IS_OPEN_PROPERTY        "isOpen"
#define IS_VERTICAL_PROPERTY    "isVertical"

/**
 *	Will replace this constructor with some kind of populate/build level via levelController
 */
LevelModel::LevelModel() :
_goalDoor(nullptr),
_kids(),
_pineapple(nullptr),
_blender(nullptr),
_failed(false),
_length(UNSET_LENGTH),
_kidsRemaining(0),
_world(nullptr),
_rootnode(nullptr),
_debugnode(nullptr),
_worldnode(nullptr){}

/**
 * Creates a new game level with no source file.
 *
 * The source file can be set at any time via the setFile() method. This method
 * does NOT load the asset.  You must call the load() method to do that.
 *
 * @return  an autoreleased level file
 */
LevelModel* LevelModel::create() {
    LevelModel* level = new (std::nothrow) LevelModel();
    if (level && level->init()) {
        level->autorelease();
        return level;
    }
    CC_SAFE_DELETE(level);
    return nullptr;
    
}

/**
 * Creates a new game level with the given source file.
 *
 * This method does NOT load the level. You must call the load() method to do that.
 * This method returns false if file does not exist.
 *
 * @return  an autoreleased level file
 */
LevelModel* LevelModel::create(std::string file) {
    LevelModel* level = new (std::nothrow) LevelModel();
    if (level && level->init(file)) {
        level->autorelease();
        return level;
    }
    CC_SAFE_DELETE(level);
    return nullptr;
}

void initFootSensorProperties(Obstacle* obstacle) {
    
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
    obstacle->setFixedRotation(true);
    obstacle->setRestitution(0.0f);
    obstacle->setGravityScale(0);
    obstacle->setSensor(true);
}

/** Helper method for creating physical obstacles */
void initPhysicalObstacle(Obstacle* obstacle) {
    obstacle->setBodyType(b2_staticBody);
    obstacle->setDensity(BASIC_DENSITY);
    obstacle->setFriction(BASIC_FRICTION);
    obstacle->setFixedRotation(true);
    obstacle->setRestitution(BASIC_RESTITUTION);
    obstacle->setGravityScale(1);
    obstacle->setSensor(false);
}


bool LevelModel::load() {
    _world = WorldController::create(Rect(0,0,DEFAULT_WIDTH,DEFAULT_HEIGHT),Vec2(0,DEFAULT_GRAVITY)); //Replace if this ever changes
    _world->retain();
    
    float* position = new float[WALL_VERTS];
    
    _bounds.size.setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    
    TMXTiledMap *map = TMXTiledMap::create(_file);
    if(map == nullptr) {
        CCASSERT(false, "Failed to load level file");
        return false;
    }
    
    addLength(map->getMapSize().width);
    
    float tileX = map->getTileSize().width;
    float tileY = map->getTileSize().height;
    
    //For all imaged layers, need to add 1 to y coordinate because tiled is weird
    
    for(auto it = map->getObjectGroups().begin(); it != map->getObjectGroups().end(); ++it) {
        TMXObjectGroup* objectGroup = *it;
        ValueVector objects = objectGroup->getObjects();
        for(auto it2 = objects.begin(); it2 != objects.end(); ++it2) {
            
            //Casting bug occurs here
            Value obj = (*it2);
            if (obj.getType() == Value::Type::MAP) {
                ValueMap object = obj.asValueMap();
                
                float x = (float) object.at(X_PROPERTY).asFloat() / tileX;
                float y = (float) object.at(Y_PROPERTY).asFloat() / tileY;
                float w = (float) object.at(WIDTH_PROPERTY).asFloat() / tileX;
                float h = (float) object.at(HEIGHT_PROPERTY).asFloat() / tileY;
                
                position[0] = x;
                position[1] = y+1;
                
                if (objectGroup->getGroupName() == WALL_OBJECT_GROUP) {
                    position[0] = x;
                    position[1] = y;
                    position[2] = x;
                    position[3] = y+h;
                    position[4] = x+w;
                    position[5] = y+h;
                    position[6] = x+w;
                    position[7] = y;
                    addWall(position);
                } else if (objectGroup->getGroupName() == GOAL_OBJECT_GROUP) {
                    addGoal(position);
                } else if (objectGroup->getGroupName() == JELLO_OBJECT_GROUP) {
                    addJello(position);
                } else if (objectGroup->getGroupName() == SPIKES_OBJECT_GROUP) {
                    addSpikes(position);
                } else if (objectGroup->getGroupName() == WILL_OBJECT_GROUP) {
                    addPineapple(position);
                } else if (objectGroup->getGroupName() == KIDS_OBJECT_GROUP) {
                    addKid(position);
                } else if (objectGroup->getGroupName() == CUP_OBJECT_GROUP) {
                    addCup(position);
                } else if (objectGroup->getGroupName() == BUTTON_SWITCH_OBJECT_GROUP) {
                    bool isSwitch = object.at(IS_SWITCH_PROPERTY).asBool();
                    Color color = MoveablePlatformModel::getColor(object.at(COLOR_PROPERTY).asInt());
                    addButtonSwitch(position, isSwitch, color);
                } else if (objectGroup->getGroupName() == MOVEABLE_PLATFORMS_GROUP) {
                    int length = object.at(LENGTH_PROPERTY).asInt();
                    bool isOpen = object.at(IS_OPEN_PROPERTY).asBool();
                    bool isVertical = object.at(IS_VERTICAL_PROPERTY).asBool();
                    Color color = MoveablePlatformModel::getColor(object.at(COLOR_PROPERTY).asInt());
                    addMoveablePlatform(position, length, isOpen, isVertical, color);
                }
            }
        }
        
    }
    
    position[0] = map->getProperties().at(BLENDER_START_X_PROPERTY).asFloat();
    position[1] = map->getProperties().at(BLENDER_Y_PROPERTY).asFloat();
    addBlender(position);
    
    //Add walls that are offscreen and prevent you from going past end of level
    position[0] = 0;
    position[1] = 0;
    position[2] = 0;
    position[3] = DEFAULT_HEIGHT;
    position[4] = -2;
    position[5] = DEFAULT_HEIGHT;
    position[6] = -2;
    position[7] = 0;
    addWall(position);
    for(int i = 0; i < WALL_VERTS; i += 2) {
        position[i] += 2 + _length;
    }
    addWall(position);
    
    return true;
}

void LevelModel::unload() {
    //TODO - pre-nulling cleanup
    
    if (_pineapple != nullptr) {
        if (_world != nullptr) {
            _world->removeObstacle(_pineapple);
            _worldnode->removeChild(_pineapple->getSceneNode());
            _debugnode->removeChild(_pineapple->getDebugNode());
        }
        _pineapple->release();
        _pineapple = nullptr;
    }
    if (_goalDoor != nullptr) {
        if (_world != nullptr) {
            _world->removeObstacle(_goalDoor);
            _worldnode->removeChild(_goalDoor->getSceneNode());
            _debugnode->removeChild(_goalDoor->getDebugNode());
        }
        _goalDoor->release();
        _goalDoor = nullptr;
    }
    if(_blender != nullptr) {
        if (_world != nullptr) {
            _world->removeObstacle(_blender);
            _worldnode->removeChild(_blender->getSceneNode());
            _debugnode->removeChild(_blender->getDebugNode());
        }
        _blender->release();
        _blender = nullptr;
    }
    
    for(int i = 0; i < KID_COUNT; i++) {
        if (_kids[i] != nullptr) {
            if (_world != nullptr) {
                _world->removeObstacle(_kids[i]);
                _worldnode->removeChild(_kids[i]->getSceneNode());
                _debugnode->removeChild(_kids[i]->getDebugNode());
            }
            _kids[i]->release();
            _kids[i] = nullptr;
        }
    }
    
    for(auto it = _spikes.begin(); it != _spikes.end(); ++it) {
        if (_world != nullptr) {
            _world->removeObstacle(*it);
            _worldnode->removeChild((*it)->getSceneNode());
            _debugnode->removeChild((*it)->getDebugNode());
        }
        (*it)->release();
    }
    _spikes.clear();
    
    for(auto it = _walls.begin(); it != _walls.end(); ++it) {
        if (_world != nullptr) {
            _world->removeObstacle(*it);
            _worldnode->removeChild((*it)->getSceneNode());
            _debugnode->removeChild((*it)->getDebugNode());
        }
        (*it)->release();
    }
    _walls.clear();
    
    for(auto it = _jellos.begin(); it != _jellos.end(); ++it) {
        if (_world != nullptr && ! (*it)->isRemoved()) {
            _world->removeObstacle(*it);
            _worldnode->removeChild((*it)->getSceneNode());
            _debugnode->removeChild((*it)->getDebugNode());
        }
        (*it)->release();
    }
    _jellos.clear();
    
    for(auto it = _crushables.begin(); it != _crushables.end(); ++it) {
        if (_world != nullptr && ! (*it)->isRemoved()) {
            _world->removeObstacle(*it);
            _worldnode->removeChild((*it)->getSceneNode());
            _debugnode->removeChild((*it)->getDebugNode());
        }
        (*it)->release();
    }
    _crushables.clear();
    
    for(auto it = _buttonSwitches.begin(); it != _buttonSwitches.end(); ++it) {
        if (_world != nullptr && ! (*it)->isRemoved()) {
            _world->removeObstacle(*it);
            _worldnode->removeChild((*it)->getSceneNode());
            _debugnode->removeChild((*it)->getDebugNode());
        }
        (*it)->release();
    }
    _buttonSwitches.clear();
    
    for(auto it = _moveablePlatforms.begin(); it != _moveablePlatforms.end(); ++it) {
        if (_world != nullptr && ! (*it)->isRemoved()) {
            _world->removeObstacle(*it);
            _worldnode->removeChild((*it)->getSceneNode());
            _debugnode->removeChild((*it)->getDebugNode());
        }
        (*it)->release();
    }
    _moveablePlatforms.clear();
    
    if (_world != nullptr) {
        _world->clear();
        _world->release();
        _world = nullptr;
    }
    
    _length = UNSET_LENGTH;
    _kidsRemaining=0;
    _worldnode=nullptr;
    _debugnode=nullptr;
    _rootnode=nullptr;
}

LevelModel::~LevelModel(){
    unload();
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

void LevelModel::addLength(float length) {
    if (_length == UNSET_LENGTH) {
        _length = length;
    } else {
        CC_ASSERT(false);
    }
}

void LevelModel::addGoal(float goalPos[POS_COORDS]) {
    if (_goalDoor == nullptr) {
        _goalDoor = GoalModel::create(goalPos);
        
        initSensor(_goalDoor);
        initDebugProperties(_goalDoor);
        _goalDoor->setName(GOAL_NAME);
    } else {
        CC_ASSERT(false);
    }
}

void LevelModel::addWall(float wallPos[WALL_VERTS]) {
    Poly2 wallPoly(wallPos,WALL_VERTS);
    wallPoly.triangulate();
    WallModel* wallobj = WallModel::create(wallPoly);
    
    initDebugProperties(wallobj);
    initPhysicalObstacle(wallobj);
    wallobj->setName(WALL_NAME);
    _walls.push_back(wallobj);
}


void LevelModel::addPineapple(float pineapplePos[POS_COORDS]) {
    if (_pineapple == nullptr) {
        PineappleModel* will = PineappleModel::create(pineapplePos);
        
        b2Filter b = b2Filter();
        b.categoryBits = PINEAPPLE_MASK;
        b.maskBits = PINEAPPLE_COLLIDES_WITH;
        will->setFilterData(b);
        will->setName(PINEAPPLE_NAME);
        
        initDebugProperties(will);
        _pineapple = will;
    } else {
        
        CC_ASSERT(false);
    }
}

void LevelModel::addKid(float kidPos[POS_COORDS]) {
    if (_kids[_kidsRemaining] == nullptr && _kidsRemaining < KID_COUNT) {
        KidModel *kid = KidModel::create(kidPos,_kidsRemaining);
        kid->setMovement(KID_WALKSPEED);
        
        b2Filter b = b2Filter();
        b.categoryBits = KID_MASK;
        b.maskBits = KID_COLLIDES_WITH;
        kid->setFilterData(b);
        kid->setName(KID_NAME);
        
        initDebugProperties(kid);
        _kids[_kidsRemaining] = kid;
        _kidsRemaining++;
    } else {
        CC_ASSERT(false);
    }
}

void LevelModel::addJello(float jelloPos[POS_COORDS]) {
    JelloModel* jello = JelloModel::create(jelloPos);
    initDebugProperties(jello);
    initSensor(jello);
    jello->setName(JELLO_NAME);
    _jellos.push_back(jello);
}

void LevelModel::addCup(float cupPos[POS_COORDS]) {
    CrushableModel* cup = CrushableModel::create(RED_CUP_TEXTURE, cupPos);
    initDebugProperties(cup);
    initPhysicalObstacle(cup);
    cup->setName(CUP_NAME);
    _crushables.push_back(cup);
}

void LevelModel::addSpikes(float spikesPos[POS_COORDS]) {
    SpikeModel* spike = SpikeModel::create(spikesPos);
    
    initDebugProperties(spike);
    initSensor(spike);
    spike->setName(SPIKE_NAME);
    _spikes.push_back(spike);
}

void LevelModel::addBlender(float blenderPos[POS_COORDS]) {
    if (_blender == nullptr) {
        BlenderModel* blender = BlenderModel::create(blenderPos);
        
        b2Filter b = b2Filter();
        b.categoryBits = BLENDER_MASK;
        b.maskBits = BLENDER_COLLIDES_WITH;
        blender->setFilterData(b);
        
        blender->setName(BLENDER_NAME);
        
        initDebugProperties(blender);
        initSensor(blender);
        _blender = blender;
    } else {
        CC_ASSERT(false);
    }
}

void LevelModel::addButtonSwitch(float buttonSwitchPos[POS_COORDS], bool isSwitch, Color color) {
    ButtonSwitchModel* button = ButtonSwitchModel::create(buttonSwitchPos, Vec2(1,1), isSwitch, color);
    
    initDebugProperties(button);
    initSensor(button);
    button->setName(BUTTON_SWITCH_NAME);
    _buttonSwitches.push_back(button);
}

void LevelModel::addMoveablePlatform(float platformPos[POS_COORDS], float length, bool isOpen, bool vertical, Color color) {
    MoveablePlatformModel* platform = MoveablePlatformModel::create(platformPos, length, isOpen, vertical, color);
    
    initDebugProperties(platform);
    
    for(int ii = 0; ii < platform->getBodies().size(); ii++) {
        initPhysicalObstacle(platform->getBodies()[ii]);
    }
    
    platform->setName(MOVEABLE_PLATFORM_NAME);
    _moveablePlatforms.push_back(platform);
}

void LevelModel::setDrawScale(const Vec2& value) {
    if (value.x <= 0 || value.y <= 0) {
        CC_ASSERT(false);
    }
    _scale = value;
    
    //TODO - iterate over all objects, set the scale using setDrawScale
    
}

void LevelModel::addAnonymousObstacle(cocos2d::Obstacle *obj, int zOrder) {
    addObstacle(obj, zOrder);
}

/**
 * Sets the scene graph node for drawing purposes.
 *
 * The scene graph is completely decoupled from the physics system.  The node
 * does not have to be the same size as the physics body. We only guarantee
 * that the node is positioned correctly according to the drawing scale.
 *
 * @param value  the scene graph node for drawing purposes.
 *
 * @retain  a reference to this scene graph node
 * @release the previous scene graph node used by this object
 */
void LevelModel::setRootNode(Node* node) {
    if (_rootnode != nullptr) {
        clearRootNode();
    }
    
    SceneManager* assets =  AssetManager::getInstance()->getCurrent();
    
    _rootnode = node;
    _rootnode->retain();
    _scale.set(_rootnode->getContentSize().width/_bounds.size.width,
               _rootnode->getContentSize().height/_bounds.size.height);
    
    // Create, but transfer ownership to root
    _worldnode = Node::create();
    _debugnode = Node::create();
    _rootnode->addChild(_worldnode,2);
    _rootnode->addChild(_debugnode,3);
    
    // Add the individual elements
    PolygonNode* poly;
    
    for(auto it = _walls.begin(); it != _walls.end(); ++it) {
        WallModel* wall = *it;
        
        Texture2D* image = assets->get<Texture2D>(TILE_TEXTURE);
        
        wall->setDrawScale(_scale.x , _scale.y);
        poly = PolygonNode::createWithTexture(image);
        wall->setSceneNode(poly);
        
        addObstacle(wall, WALL_Z_INDEX);
    }
    
    if (_goalDoor != nullptr) {
        _goalDoor->setDrawScale(_scale.x, _scale.y);
        poly = PolygonNode::create();
        _goalDoor->setSceneNode(poly);
        
        addObstacle(_goalDoor, WALL_Z_INDEX);
    }
    
    if (_pineapple != nullptr) {
        Texture2D* image = assets->get<Texture2D>(PINEAPPLE_TEXTURE);
        _pineapple->setDrawScale(_scale.x , _scale.y);
        poly = AnimationNode::create(image, 1, PINEAPPLE_FRAME_COUNT, PINEAPPLE_FRAME_COUNT);
        _pineapple->setSceneNode(poly);
        
        addObstacle(_pineapple, PINEAPPLE_Z_INDEX);
    }
    
    if (_blender != nullptr) {
        Texture2D* image = assets->get<Texture2D>(BLENDER_TEXTURE);
        _blender->setDrawScale(_scale.x, _scale.y);
        poly = PolygonNode::createWithTexture(image);
        _blender->setSceneNode(poly);
        
        addObstacle(_blender, BLENDER_Z_INDEX);
    }
    
    for(int i = 0; i < KID_COUNT; i++) {
        if(_kids[i] != nullptr) {
            Texture2D* image = assets->get<Texture2D>(KidModel::getTexture(i));
            _kids[i]->setDrawScale(_scale.x, _scale.y);
            poly = AnimationNode::create(image, 1, KID_ANIMATION_FRAMES, KID_ANIMATION_FRAMES);
            _kids[i]->setSceneNode(poly);
            
            addObstacle(_kids[i], KID_Z_INDEX+i);
        }
    }
    
    
    for(auto it = _jellos.begin(); it != _jellos.end(); ++it) {
        JelloModel* jello = *it;
        
        Texture2D* image = assets->get<Texture2D>(JELLO_TEXTURE);
        jello->setDrawScale(_scale.x, _scale.y);
        poly = PolygonNode::createWithTexture(image);
        jello->setSceneNode(poly);
        
        addAnonymousObstacle(jello, JELLO_Z_INDEX);
    }
    
    for(auto it = _spikes.begin(); it != _spikes.end(); ++it) {
        SpikeModel* spike = *it;
        
        
        Texture2D* image = assets->get<Texture2D>(SPIKE_TEXTURE);
        spike->setDrawScale(_scale.x, _scale.y);
        poly = PolygonNode::createWithTexture(image);
        spike->setSceneNode(poly);
        
        addAnonymousObstacle(spike, SPIKES_Z_INDEX);
    }
    
    for(auto it = _crushables.begin(); it != _crushables.end(); ++it) {
        CrushableModel* cup = *it;
        
        
        Texture2D* image = assets->get<Texture2D>(cup->getTextureName());
        cup->setDrawScale(_scale.x, _scale.y);
        poly = PolygonNode::createWithTexture(image);
        cup->setSceneNode(poly);
        
        addAnonymousObstacle(cup, CUP_Z_INDEX);
    }
    
    for(auto it = _buttonSwitches.begin(); it != _buttonSwitches.end(); ++it) {
        ButtonSwitchModel* button = *it;
        
        Texture2D* image = assets->get<Texture2D>(button->isSwitch() ? SWITCH_TEXTURE_RED : BUTTON_TEXTURE_RED);
        button->setDrawScale(_scale.x, _scale.y);
        poly = PolygonNode::createWithTexture(image);
        button->setSceneNode(poly);
        
        addAnonymousObstacle(button, BUTTON_SWITCH_Z_INDEX);
    }
    
    for(auto it = _moveablePlatforms.begin(); it != _moveablePlatforms.end(); ++it) {
        MoveablePlatformModel* platform = *it;
        
        platform->storeDrawScale(_scale);
        platform->setDrawScale(_scale);
        poly = PolygonNode::create();
        platform->setSceneNode(poly);
        
        addAnonymousObstacle(platform, MOVEABLE_PLATFORM_Z_INDEX);        
    }
    
    //Hook up switches to platforms
    for(auto it = _buttonSwitches.begin(); it != _buttonSwitches.end(); ++it) {
        ButtonSwitchModel* button = *it;
        
        Color c = button->getColor();
        for(auto it2 = _moveablePlatforms.begin(); it2 != _moveablePlatforms.end(); ++it2) {
            MoveablePlatformModel* platform = *it2;
            
            Color c2 = platform->getColor();
            if(c == c2) {
                button->linkToPlatform(platform);
                break;
            }
        }
    }
    
    _pineapple->setGrounded(true);
}

/**
 * Clears the root scene graph node for this level
 */
void LevelModel::clearRootNode() {
    if (_rootnode == nullptr) {
        return;
    }
    
    if (_worldnode != nullptr) {
        _rootnode->removeChild(_worldnode);
        _worldnode = nullptr; // We do not own it
    }
    if (_debugnode != nullptr) {
        _rootnode->removeChild(_debugnode);
        _debugnode = nullptr; // We do not own it
    }
    _rootnode->release();
    _rootnode = nullptr;
}

void LevelModel::showDebug(bool flag) {
    _debugnode->setVisible(flag);
}

void LevelModel::kill(PineappleModel* will) {
    removeObstacle(will);
    clearPineapple();
    setFailure(true); //TODO: Move failure to main game loop?
}

void LevelModel::kill(KidModel* kid) {
    if (_kids[kid->getIndex()] != nullptr) {
        removeObstacle(kid);
        clearKid(kid->getIndex());
        subtractKidFromCount();
        // TODO: Move failure check to main game loop?
        if (_kidsRemaining == 0) {
            setFailure(true);
        }
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
