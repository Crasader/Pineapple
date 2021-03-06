#include "LevelModel.h"

#include <cornell/CUJSONReader.h>
#include <cornell/CUStrings.h>
#include "MoveablePlatformModel.h"
#include "Const.h"

#define UNSET_LENGTH -2

#define WALL_VERTS      8
#define POS_COORDS      2

#define GOAL_TILE_OFFSET    Vec2(-3,5)

/** Layer groups used in Tiled Maps */
#define WALL_OBJECT_GROUP               "Walls"
#define JELLO_OBJECT_GROUP              "Jello"
#define SPIKES_OBJECT_GROUP             "Spikes"
#define KIDS_OBJECT_GROUP               "Kids"
#define WILL_OBJECT_GROUP               "Will"
#define CUP_OBJECT_GROUP                "Cups"
#define BUTTON_SWITCH_OBJECT_GROUP      "Switches"
#define MOVEABLE_PLATFORMS_GROUP        "Bridges"
#define TUTORIAL_IMAGES_GROUP           "Tutorial"

/** Properties that Tiled Objects and maps posess */
/** Level Properties */
#define LAYERS_PROPERTY             "layers"
#define LAYER_NAME_PROPERTY         "name"
#define LAYER_OBJECTS_PROPERTY      "objects"

#define BLENDER_START_X_PROPERTY    "BlenderStartX"
#define BLENDER_Y_PROPERTY          "BlenderStartY"
#define TILE_WIDTH_PROPERTY         "tilewidth"
#define TILE_HEIGHT_PROPERTY        "tileheight"
#define OBJECT_PROPERTIES_PROPERTY  "properties"

/** Walls */
#define WALL_IS_POLY_PROPERTY       "polygon"

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
#define NUBBINS_VISIBLE         "nubbinsVisible"
#define IS_OPEN_PROPERTY        "isOpen"
#define IS_VERTICAL_PROPERTY    "isVertical"

/** Tutorial image properties */
#define TUTORIAL_ID             "id"

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
_worldnode(nullptr),
_isLoaded(false),
_isUnloaded(false),
_nextCrushableTexture(0){}

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
        level->retain();
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
        level->retain();
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
    obstacle->setFriction(0.0f);
    obstacle->setFixedRotation(true);
    obstacle->setRestitution(BASIC_RESTITUTION);
    obstacle->setGravityScale(1);
    obstacle->setSensor(false);
}


bool LevelModel::load() {
    if (! _isLoaded) {
        _world = WorldController::create(Rect(0,0,DEFAULT_WIDTH,DEFAULT_HEIGHT),Vec2(0,DEFAULT_GRAVITY)); //Replace if this ever changes
        _world->retain();
        
        float* position = new float[WALL_VERTS];
        
        _bounds.size.setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
        _isActive = false;
        
        JSONReader reader;
        reader.initWithFile(_file);
        if (!reader.startJSON()) {
            CCASSERT(false, "Failed to load level file");
            return false;
        }
        
        float stageWidth = reader.getNumber(WIDTH_PROPERTY);
        float stageHeight = reader.getNumber(HEIGHT_PROPERTY);
        
        addLength(stageWidth);
        
        float tileX = reader.getNumber(TILE_WIDTH_PROPERTY);
        float tileY = reader.getNumber(TILE_HEIGHT_PROPERTY);
        
        _tileSize = Size(tileX, tileY);
        
        reader.startObject(LAYERS_PROPERTY); //Start layer
        int wsize = reader.startArray(); //Start layer as array
        for(int ii = 0; ii < wsize; ii++) {
            
            //Start layer in this object
            reader.startObject();
            
            string layerName = reader.getString(LAYER_NAME_PROPERTY);
            
            //Start reading objects in this layer
            reader.startObject(LAYER_OBJECTS_PROPERTY);
            int wsize2 = reader.startArray();
            
            for (int jj = 0; jj < wsize2; jj++) {
                
                //Start object in array
                reader.startObject();
                
                
                float x = (float) reader.getNumber(X_PROPERTY) / tileX;
                float y = stageHeight - (float) reader.getNumber(Y_PROPERTY) / tileY;
                float w = (float) reader.getNumber(WIDTH_PROPERTY) / tileX;
                float h = (float) reader.getNumber(HEIGHT_PROPERTY) / tileY;
                
                position[0] = x;
                position[1] = y;
                
                if (layerName == WALL_OBJECT_GROUP) {
                    if (reader.startArray(WALL_IS_POLY_PROPERTY)) {
                        for (int kk = 0; kk < WALL_VERTS; kk += 2) {
                            reader.startObject();
                            float x2 = reader.getNumber(X_PROPERTY) / tileX + x;
                            float y2 = -reader.getNumber(Y_PROPERTY) / tileY + y;
                            position[kk] = x2;
                            position[kk+1] = y2;
                            reader.endObject();
                            reader.advance();
                        }
                        reader.endArray();
                    } else {
                        reader.endArray();
                        position[0] = x;
                        position[1] = y-h;
                        position[2] = x;
                        position[3] = y;
                        position[4] = x+w;
                        position[5] = y;
                        position[6] = x+w;
                        position[7] = y-h;
                    }
                    addWall(position);
                } else if (layerName == JELLO_OBJECT_GROUP) {
                    position[0] += 0.5;
                    addJello(position, x, y);
                } else if (layerName == SPIKES_OBJECT_GROUP) {
                    position[0] += 0.5;
                    position[1] += 0.5;
                    addSpikes(position);
                } else if (layerName == WILL_OBJECT_GROUP) {
                    addPineapple(position);
                } else if (layerName == KIDS_OBJECT_GROUP) {
                    addKid(position);
                } else if (layerName == CUP_OBJECT_GROUP) {
                    position[0] += 0.5;
                    position[1] += 0.5;
                    addCup(position, x, y);
                } else if (layerName == BUTTON_SWITCH_OBJECT_GROUP) {
                    reader.startObject(OBJECT_PROPERTIES_PROPERTY);
                    
                    bool isSwitch = string2bool(reader.getString(IS_SWITCH_PROPERTY));
                    double colorF = cocos2d::stod(reader.getString(COLOR_PROPERTY));
                    Color color = MoveablePlatformModel::getColor((int)colorF);
                    
                    reader.endObject();
                    addButtonSwitch(position, isSwitch, color);
                } else if (layerName == MOVEABLE_PLATFORMS_GROUP) {
                    reader.startObject(OBJECT_PROPERTIES_PROPERTY);
                    
                    bool isOpen = string2bool(reader.getString(IS_OPEN_PROPERTY));
                    bool isVertical = string2bool(reader.getString(IS_VERTICAL_PROPERTY));
                    bool nubbinsVisible = string2bool(reader.getString(NUBBINS_VISIBLE));
                    
                    float length;
                    if (isVertical) {
                        position[0] = x + w;
                        position[1] = y + h/2;
                        length = h;
                    } else {
                        position[0] = x + w/2;
                        position[1] = y + h + 0.175; // Hack to get horizontal platform to be flush with floor next to it
                        length = w;
                    }
                    double colorF = cocos2d::stod(reader.getString(COLOR_PROPERTY));
                    Color color = MoveablePlatformModel::getColor((int)colorF);
                    
                    reader.endObject();
                    addMoveablePlatform(position, length, isOpen, isVertical, nubbinsVisible, color);
                } else if (layerName == TUTORIAL_IMAGES_GROUP) {
                    reader.startObject(OBJECT_PROPERTIES_PROPERTY);

                    int ID = cocos2d::stod(reader.getString(TUTORIAL_ID));
                    addTutorialImage(ID, x, y);
                    
                    reader.endObject();
                } else {
                    cout << " Unknown layer name " << layerName;
                }
                
                //End object in array
                reader.endObject();
                
                //Advance to next object in group
                reader.advance();
            }
            
            //End object group array
            reader.endArray();
            
            reader.endObject();
            reader.endObject();
            
            //Advance to next object group
            reader.advance();
        }
        
        //End layer array
        reader.endArray();
        
        //End layer
        reader.endObject();
        
        //Start map properties
        reader.startObject(OBJECT_PROPERTIES_PROPERTY);
        
        //GOAL
        position[0] = _length + GOAL_TILE_OFFSET.x;
        position[1] = GOAL_TILE_OFFSET.y;
        addGoal(position);
        
        //BLENDER
        position[0] = cocos2d::stod(reader.getString(BLENDER_START_X_PROPERTY));
        position[1] = cocos2d::stod(reader.getString(BLENDER_Y_PROPERTY));
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
        
        //Add roof wall
        position[0] = 0;
        position[1] = DEFAULT_HEIGHT + 1;
        position[2] = 0;
        position[3] = DEFAULT_HEIGHT + 3;
        position[4] = _length;
        position[5] = DEFAULT_HEIGHT + 3;
        position[6] = _length;
        position[7] = DEFAULT_HEIGHT + 1;
        addWall(position);
        
        reader.endObject();
        
        reader.endJSON();
        
        _isLoaded = true;
    }
    
    return true;
}

void LevelModel::unload() {
    if(! _isUnloaded || ! _isLoaded) {
        //TODO - pre-nulling cleanup
        
        if (_pineapple != nullptr) {
            if (_isActive) {
                _world->removeObstacle(_pineapple);
                _worldnode->removeChild(_pineapple->getSceneNode());
                _debugnode->removeChild(_pineapple->getDebugNode());
            }
            _pineapple->release();
            _pineapple = nullptr;
        }
        if (_goalDoor != nullptr) {
            if (_isActive) {
                _world->removeObstacle(_goalDoor);
                _worldnode->removeChild(_goalDoor->getSceneNode());
                _debugnode->removeChild(_goalDoor->getDebugNode());
            }
            _goalDoor->release();
            _goalDoor = nullptr;
        }
        if(_blender != nullptr) {
            if (_isActive) {
                _world->removeObstacle(_blender);
                _worldnode->removeChild(_blender->getSceneNode());
                _debugnode->removeChild(_blender->getDebugNode());
                _worldnode->removeChild(_blender->getSensor());
                _debugnode->removeChild(_blender->getSensor());
            }
            _blender->release();
            _blender = nullptr;
        }
        
        for(int i = 0; i < KID_COUNT; i++) {
            if (_kids[i] != nullptr) {
                if (_isActive) {
                    _world->removeObstacle(_kids[i]);
                    _worldnode->removeChild(_kids[i]->getSceneNode());
                    _debugnode->removeChild(_kids[i]->getDebugNode());
                }
                _kids[i]->release();
                _kids[i] = nullptr;
            }
        }
        
        for(auto it = _spikes.begin(); it != _spikes.end(); ++it) {
            if (_isActive) {
                _world->removeObstacle(*it);
                _worldnode->removeChild((*it)->getSceneNode());
                _debugnode->removeChild((*it)->getDebugNode());
            }
            (*it)->release();
        }
        _spikes.clear();
        
        for(auto it = _walls.begin(); it != _walls.end(); ++it) {
            if (_isActive) {
                _world->removeObstacle(*it);
                _worldnode->removeChild((*it)->getSceneNode());
                _debugnode->removeChild((*it)->getDebugNode());
                
                if ((*it)->isFloor()) {
                    _worldnode->removeChild((*it)->getTopNode());
                } else {
                    _worldnode->removeChild((*it)->getLeftNode());
                    _worldnode->removeChild((*it)->getRightNode());
                }
            }
            (*it)->release();
        }
        _walls.clear();
        
        for(auto it = _jellos.begin(); it != _jellos.end(); ++it) {
            if (_isActive && !(*it)->isRemoved()) {
                _world->removeObstacle(*it);
                _worldnode->removeChild((*it)->getSceneNode());
                _debugnode->removeChild((*it)->getDebugNode());
            }
            (*it)->release();
        }
        _jellos.clear();
        
        for(auto it = _crushables.begin(); it != _crushables.end(); ++it) {
            if (_isActive && !(*it)->isRemoved()) {
                _world->removeObstacle(*it);
                _worldnode->removeChild((*it)->getSceneNode());
                _debugnode->removeChild((*it)->getDebugNode());
            }
            (*it)->release();
        }
        _crushables.clear();
        
        for(auto it = _buttonSwitches.begin(); it != _buttonSwitches.end(); ++it) {
            if (_isActive && !(*it)->isRemoved()) {
                _world->removeObstacle(*it);
                _worldnode->removeChild((*it)->getSceneNode());
                _debugnode->removeChild((*it)->getDebugNode());
            }
            (*it)->release();
        }
        _buttonSwitches.clear();
        
        for(auto it = _moveablePlatforms.begin(); it != _moveablePlatforms.end(); ++it) {
            if (_isActive && !(*it)->isRemoved()) {
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
        
        _isUnloaded = true;
        _isActive = false;
    }
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
        _goalDoor->setName(GOAL_NAME);
    } else {
        CC_ASSERT(false);
    }
}

void LevelModel::addWall(float wallPos[WALL_VERTS]) {
    Poly2 wallPoly(wallPos,WALL_VERTS);
    wallPoly.triangulate();
    WallModel* wallobj = WallModel::create(wallPoly);
    
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
        
        _kids[_kidsRemaining] = kid;
        _kidsRemaining++;
    } else {
        CC_ASSERT(false);
    }
}

void LevelModel::addJello(float jelloPos[POS_COORDS], int x, int y) {
    JelloModel* jello = JelloModel::create(x, y, jelloPos);
    initSensor(jello);
    jello->setName(JELLO_NAME);
    _jellos.push_back(jello);
}

void LevelModel::addCup(float cupPos[POS_COORDS], int x, int y) {
    CrushableModel* cup = CrushableModel::create(CrushableModel::getNextTexture(_nextCrushableTexture), x, y, cupPos);
    _nextCrushableTexture++;
    initPhysicalObstacle(cup);
    cup->setName(CUP_NAME);
    _crushables.push_back(cup);
}

void LevelModel::addSpikes(float spikesPos[POS_COORDS]) {
    SpikeModel* spike = SpikeModel::create(spikesPos);
    
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
        
        initSensor(blender);
        _blender = blender;
    } else {
        CC_ASSERT(false);
    }
}

void LevelModel::addButtonSwitch(float buttonSwitchPos[POS_COORDS], bool isSwitch, Color color) {
    ButtonSwitchModel* button = ButtonSwitchModel::create(buttonSwitchPos, Vec2(1,1), isSwitch, color);
    
    initSensor(button);
    button->setName(BUTTON_SWITCH_NAME);
    _buttonSwitches.push_back(button);
}

void LevelModel::addMoveablePlatform(float platformPos[POS_COORDS], float length, bool isOpen, bool vertical, bool nubbinsVisible, Color color) {
    MoveablePlatformModel* platform = MoveablePlatformModel::create(platformPos, length, isOpen, vertical, nubbinsVisible, color);
    
    platform->setName(MOVEABLE_PLATFORM_NAME);
    _moveablePlatforms.push_back(platform);
}

void LevelModel::addTutorialImage(int ID, float x, float y) {
    TutorialView* view = TutorialView::create(ID, Vec2(x, y));
    _tutorialViews.push_back(view);
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

/** Returns true if there is a jello at the given location */
JelloModel* hasJelloAt(std::vector<JelloModel*> jellos, int x, int y) {
    for(auto it = jellos.begin(); it != jellos.end(); ++it) {
        JelloModel* jello = *it;
        if (jello->getTiledXCoord() == x && jello->getTiledYCoord() == y) return jello;
    }
    return nullptr;
}

/** Returns true if there is a cup at the given location */
CrushableModel* hasCupAt(std::vector<CrushableModel*> cups, int x, int y) {
    for(auto it = cups.begin(); it != cups.end(); ++it) {
        CrushableModel* cup = *it;
        if (cup->getTiledXCoord() == x && cup->getTiledYCoord() == y) return cup;
    }
    return nullptr;
}

/** Compares crushablemodels by their tiled y coordinates. For use in sorting in setRootNode */
bool sortCupsByYCoord(CrushableModel* c1, CrushableModel* c2) {
    return c1->getTiledYCoord() < c2->getTiledYCoord();
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
        //Shouldn't set root node of a level twice
        CC_ASSERT(false);
    }
    
    SceneManager* assets =  AssetManager::getInstance()->getCurrent();
    
    _rootnode = node;
    _rootnode->retain();
    _scale.set(_rootnode->getContentSize().width/_bounds.size.width,
               _rootnode->getContentSize().height/_bounds.size.height);
    
    // Create, but transfer ownership to root
    _worldnode = Node::create();
    _debugnode = Node::create();
    _rootnode->addChild(_worldnode,WORLD_Z_INDEX);
    _rootnode->addChild(_debugnode,DEBUG_Z_INDEX);
    
    // Add the individual elements
    PolygonNode* poly;
    
    //Tiling params
    Texture2D::TexParams params;
    params.wrapS = GL_REPEAT;
    params.wrapT = GL_REPEAT;
    params.magFilter = GL_NEAREST;
    params.minFilter = GL_NEAREST;
    
    for(auto it = _walls.begin(); it != _walls.end(); ++it) {
        WallModel* wall = *it;
        wall->setDrawScale(_scale.x , _scale.y);
        
        Texture2D* image = assets->get<Texture2D>(wall->getTextureID());
        
        if (wall->isFloor()) {
            image->setTexParameters(params);
            poly = PolygonNode::createWithTexture(image, wall->getPolygon() * _scale);
            Texture2D* topImage = assets->get<Texture2D>(FLOOR_TOP_TEXTURE);
            wall->setTopNode(PolygonNode::createWithTexture(topImage));
        } else {
            poly = PolygonNode::createWithTexture(image);
            poly->setScale(wall->getSize().width*_scale.x/poly->getContentSize().width, wall->getSize().height*_scale.y/poly->getContentSize().height);
            PolygonNode* left = PolygonNode::createWithTexture(assets->get<Texture2D>(PLATFORM_EDGE_LEFT_TEXTURE));
            PolygonNode* right =PolygonNode::createWithTexture(assets->get<Texture2D>(PLATFORM_EDGE_RIGHT_TEXTURE));
            
            wall->setLeftNode(left);
            wall->setRightNode(right);
        }
        
        wall->setSceneNode(poly);
        
        initDebugProperties(wall);
        
        addObstacle(wall, WALL_Z_INDEX);
        
        if (wall->isFloor()) {
            _worldnode->addChild(wall->getTopNode(),WALL_Z_INDEX + 1);
        } else {
            _worldnode->addChild(wall->getRightNode(), WALL_Z_INDEX+1);
            _worldnode->addChild(wall->getLeftNode(), WALL_Z_INDEX+1);
        }
    }
    
    if (_goalDoor != nullptr) {
		Texture2D* image = assets->get<Texture2D>(GOAL_TEXTURE);
        _goalDoor->setDrawScale(_scale.x, _scale.y);
		poly = AnimationNode::create(image, 1, GOAL_FRAME_COUNT, GOAL_FRAME_COUNT);
        _goalDoor->setSceneNode(poly);
        initDebugProperties(_goalDoor);
		_goalDoor->setY(_goalDoor->getY() + 0.7);
        
        addObstacle(_goalDoor, GOAL_Z_INDEX);
    }
    
    if (_pineapple != nullptr) {
        Texture2D* image = assets->get<Texture2D>(PINEAPPLE_TEXTURE);
        _pineapple->setDrawScale(_scale.x, _scale.y);
        poly = AnimationNode::create(image, 1, PINEAPPLE_FRAME_COUNT, PINEAPPLE_FRAME_COUNT);
        _pineapple->setSceneNode(poly);
        initDebugProperties(_pineapple);
        
        addObstacle(_pineapple, PINEAPPLE_Z_INDEX);
    }
    
    if (_blender != nullptr) {
        Texture2D* image = assets->get<Texture2D>(BLENDER_TEXTURE);
        _blender->setDrawScale(_scale.x, _scale.y);
		poly = AnimationNode::create(image, 1, BLENDER_FRAME_COUNT, BLENDER_FRAME_COUNT);
        _blender->setSceneNode(poly);
        initDebugProperties(_blender);
        
        addObstacle(_blender, BLENDER_Z_INDEX);
    }
    
    for(int i = 0; i < KID_COUNT; i++) {
        if(_kids[i] != nullptr) {
            Texture2D* image = assets->get<Texture2D>(KidModel::getTexture(i));
            _kids[i]->setDrawScale(_scale.x, _scale.y);
            poly = AnimationNode::create(image, 1, KID_ANIMATION_FRAMES, KID_ANIMATION_FRAMES);
            
            _kids[i]->setSceneNode(poly);
            initDebugProperties(_kids[i]);
            
            
            addObstacle(_kids[i], KID_Z_INDEX+i);
        }
    }
        
    for(auto it = _jellos.begin(); it != _jellos.end(); ++it) {
        JelloModel* jello = *it;
        
        Texture2D* image = assets->get<Texture2D>(JELLO_TEXTURE);
        jello->setDrawScale(_scale.x, _scale.y);
        poly = AnimationNode::create(image, 1, JELLO_FRAME_COUNT, JELLO_FRAME_COUNT);
        jello->setSceneNode(poly);
        initDebugProperties(jello);
        
        addAnonymousObstacle(jello, JELLO_Z_INDEX);
    }
    
    for(auto it = _spikes.begin(); it != _spikes.end(); ++it) {
        SpikeModel* spike = *it;
        
        Texture2D* image = assets->get<Texture2D>(SPIKE_TEXTURE);
        spike->setDrawScale(_scale.x, _scale.y);
        poly = PolygonNode::createWithTexture(image);
        spike->setSceneNode(poly);
        initDebugProperties(spike);
        
        addAnonymousObstacle(spike, SPIKES_Z_INDEX);
    }
    
    //Sort cups so that lower cups come first
    std::sort(_crushables.begin(), _crushables.end(), sortCupsByYCoord);
    
    for(auto it = _crushables.begin(); it != _crushables.end(); ++it) {
        CrushableModel* cup = *it;
        
        Texture2D* image = assets->get<Texture2D>(cup->getTextureName());
        cup->setDrawScale(_scale.x, _scale.y);
        poly = AnimationNode::create(image, 1, CUP_SMASH_FRAMES, CUP_SMASH_FRAMES);
        cup->setSceneNode(poly);
        initDebugProperties(cup);
        addAnonymousObstacle(cup, CUP_Z_INDEX);
        
        //Check for moving down
        if (! cup->isMovedDown()) {
            int x = cup->getTiledXCoord();
            int y = cup->getTiledYCoord()-1;
            JelloModel* jello = hasJelloAt(_jellos, x, y);
            if (jello != nullptr) {
                cup->setMovedDown(true);
            } else {
                CrushableModel* cup2 = hasCupAt(_crushables, x, y);
                if (cup2 != nullptr && cup2->isMovedDown()) {
                    cup->setMovedDown(true);
                }
            }
        }
    }
    
    for(auto it = _buttonSwitches.begin(); it != _buttonSwitches.end(); ++it) {
        ButtonSwitchModel* button = *it;
		button->setY(button->getY() - 0.1f);
        Texture2D* image = assets->get<Texture2D>(button->isSwitch() ? SWITCH_TEXTURE_RED : SWITCH_TEXTURE_RED); //TODO - replace when button is ready
        button->setDrawScale(_scale.x, _scale.y);
        poly = PolygonNode::createWithTexture(image);
        button->setSceneNode(poly);
        initDebugProperties(button);
        
        addAnonymousObstacle(button, BUTTON_SWITCH_Z_INDEX);
    }
    
    for(auto it = _moveablePlatforms.begin(); it != _moveablePlatforms.end(); ++it) {
        MoveablePlatformModel* platform = *it;
        
        platform->storeDrawScale(_scale);
        platform->setDrawScale(_scale);
        poly = PolygonNode::create();
        platform->setSceneNode(poly);
        for(int ii = 0; ii < platform->getBodies().size(); ii++) {
            initPhysicalObstacle(platform->getBodies()[ii]);
        }
        initDebugProperties(platform);
        
        addAnonymousObstacle(platform, MOVEABLE_PLATFORM_Z_INDEX);
    }
    
    //Hook up switches to platforms
    //Note that multiple buttons can now link to multiple platforms
    //Hence the lack of breaks
    for(auto it = _buttonSwitches.begin(); it != _buttonSwitches.end(); ++it) {
        ButtonSwitchModel* button = *it;
        
        Color c = button->getColor();
        for(auto it2 = _moveablePlatforms.begin(); it2 != _moveablePlatforms.end(); ++it2) {
            MoveablePlatformModel* platform = *it2;
            
            Color c2 = platform->getColor();
            if(c == c2) {
                button->linkToPlatform(platform);
            }
        }
    }
    
    //Set that this is active
    _isActive = true;
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
        _debugnode = nullptr; // we do not own it
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
  setFailure(true); 
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

void LevelModel::spikeAndKill(PineappleModel* will) {
    kill(will);
    
    //TODO: Animation and sounds
}

void LevelModel::spikeAndKill(KidModel* kid) {
    kill(kid);
    
    //TODO: Animation and sounds
}