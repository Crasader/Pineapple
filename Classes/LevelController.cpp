#include "LevelController.h"

#include "cocos2d.h"
#include <cstdio>
#include <cstdlib>

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

LevelModel* LevelController::read(string file) {
    if(_level != nullptr) {
        _level->dispose();
    }
    
    
//    Tmx::Map *map = new Tmx::Map();
//    string fileName = LEVEL_FILE_LOCATION + levelName;
//    map->ParseFile(fileName);
//    
//    if (map->HasError())
//    {
//        printf("error code: %d\n", map->GetErrorCode());
//        printf("error text: %s\n", map->GetErrorText().c_str());
//        CC_ASSERT(false);
//    }
//    
//    printf("====================================\n");
//    printf("Map\n");
//    printf("====================================\n");
//    
//    printf("Version: %1.1f\n", map->GetVersion());
//    printf("Orientation: %d\n", map->GetOrientation());
//    if (!map->GetBackgroundColor().empty())
//        printf("Background Color (hex): %s\n",
//               map->GetBackgroundColor().c_str());
//    printf("Render Order: %d\n", map->GetRenderOrder());
//    if (map->GetStaggerAxis())
//        printf("Stagger Axis: %d\n", map->GetStaggerAxis());
//    if (map->GetStaggerIndex())
//        printf("Stagger Index: %d\n", map->GetStaggerIndex());
//    printf("Width: %d\n", map->GetWidth());
//    printf("Height: %d\n", map->GetHeight());
//    printf("Tile Width: %d\n", map->GetTileWidth());
//    printf("Tile Height: %d\n", map->GetTileHeight());
//    
//    // Iterate through the tilesets.
//    for (int i = 0; i < map->GetNumTilesets(); ++i)
//    {
//        printf("                                    \n");
//        printf("====================================\n");
//        printf("Tileset : %02d\n", i);
//        printf("====================================\n");
//        
//        // Get a tileset.
//        const Tmx::Tileset *tileset = map->GetTileset(i);
//        
//        // Print tileset information.
//        printf("Name: %s\n", tileset->GetName().c_str());
//        printf("Margin: %d\n", tileset->GetMargin());
//        printf("Spacing: %d\n", tileset->GetSpacing());
//        printf("First gid: %d\n", tileset->GetFirstGid());
//        printf("Image Width: %d\n", tileset->GetImage()->GetWidth());
//        printf("Image Height: %d\n", tileset->GetImage()->GetHeight());
//        printf("Image Source: %s\n", tileset->GetImage()->GetSource().c_str());
//        if (!tileset->GetImage()->GetTransparentColor().empty())
//            printf("Transparent Color (hex): %s\n",
//                   tileset->GetImage()->GetTransparentColor().c_str());
//        
//        if (tileset->GetTiles().size() > 0)
//        {
//            // Get a tile from the tileset.
//            const Tmx::Tile *tile = *(tileset->GetTiles().begin());
//            
//            // Print the properties of a tile.
//            std::map<std::string, std::string> list =
//            tile->GetProperties().GetList();
//            std::map<std::string, std::string>::iterator iter;
//            for (iter = list.begin(); iter != list.end(); ++iter)
//            {
//                printf("%s = %s\n", iter->first.c_str(), iter->second.c_str());
//            }
//            
//            if (tile->IsAnimated())
//            {
//                printf(
//                       "Tile is animated: %d frames with total duration of %dms.\n",
//                       tile->GetFrameCount(), tile->GetTotalDuration());
//                
//                const std::vector<Tmx::AnimationFrame> &frames =
//                tile->GetFrames();
//                
//                int i = 0;
//                for (std::vector<Tmx::AnimationFrame>::const_iterator it =
//                     frames.begin(); it != frames.end(); it++, i++)
//                {
//                    printf("\tFrame %d: Tile ID = %d, Duration = %dms\n", i,
//                           it->GetTileID(), it->GetDuration());
//                }
//            }
//            
//            if(tile->HasObjects())
//            {
//                printf(
//                       "Tile has objects.\n");
//                
//                
//                // Iterate through all Collision objects in the tile.
//                for (int j = 0; j < tile->GetNumObjects(); ++j)
//                {
//                    // Get an object.
//                    const Tmx::Object *object = tile->GetObject(j);
//                    
//                    // Print information about the object.
//                    printf("Object Name: %s\n", object->GetName().c_str());
//                    printf("Object Position: (%03d, %03d)\n", object->GetX(),
//                           object->GetY());
//                    printf("Object Size: (%03d, %03d)\n", object->GetWidth(),
//                           object->GetHeight());
//                    
//                    // Print Polygon points.
//                    const Tmx::Polygon *polygon = object->GetPolygon();
//                    if (polygon != 0)
//                    {
//                        for (int i = 0; i < polygon->GetNumPoints(); i++)
//                        {
//                            const Tmx::Point &point = polygon->GetPoint(i);
//                            printf("Object Polygon: Point %d: (%f, %f)\n", i, point.x,
//                                   point.y);
//                        }
//                    }
//                    
//                    // Print Polyline points.
//                    const Tmx::Polyline *polyline = object->GetPolyline();
//                    if (polyline != 0)
//                    {
//                        for (int i = 0; i < polyline->GetNumPoints(); i++)
//                        {
//                            const Tmx::Point &point = polyline->GetPoint(i);
//                            printf("Object Polyline: Point %d: (%f, %f)\n", i, point.x,
//                                   point.y);
//                        }
//                    }
//                }
//                
//            }
//        }
//    }
//    
//    // Iterate through the tile layers.
//    for (int i = 0; i < map->GetNumTileLayers(); ++i)
//    {
//        printf("                                    \n");
//        printf("====================================\n");
//        printf("Layer : %02d/%s \n", i, map->GetTileLayer(i)->GetName().c_str());
//        printf("====================================\n");
//        
//        // Get a layer.
//        const Tmx::TileLayer *tileLayer = map->GetTileLayer(i);
//        
//        for (int y = 0; y < tileLayer->GetHeight(); ++y)
//        {
//            for (int x = 0; x < tileLayer->GetWidth(); ++x)
//            {
//                if (tileLayer->GetTileTilesetIndex(x, y) == -1)
//                {
//                    printf("........    ");
//                }
//                else
//                {
//                    // Get the tile's id and gid.
//                    printf("%03d(%03d)", tileLayer->GetTileId(x, y), tileLayer->GetTileGid(x, y));
//                    
//                    // Find a tileset for that id.
//                    //const Tmx::Tileset *tileset = map->FindTileset(layer->GetTileId(x, y));
//                    if (tileLayer->IsTileFlippedHorizontally(x, y))
//                    {
//                        printf("h");
//                    }
//                    else
//                    {
//                        printf(" ");
//                    }
//                    if (tileLayer->IsTileFlippedVertically(x, y))
//                    {
//                        printf("v");
//                    }
//                    else
//                    {
//                        printf(" ");
//                    }
//                    if (tileLayer->IsTileFlippedDiagonally(x, y))
//                    {
//                        printf("d ");
//                    }
//                    else
//                    {
//                        printf("  ");
//                    }
//                }
//            }
//            
//            printf("\n");
//        }
//    }
//    
//    printf("\n\n");
//    
//    // Iterate through all of the object groups.
//    for (int i = 0; i < map->GetNumObjectGroups(); ++i)
//    {
//        printf("                                    \n");
//        printf("====================================\n");
//        printf("Object group : %02d\n", i);
//        printf("====================================\n");
//        
//        // Get an object group.
//        const Tmx::ObjectGroup *objectGroup = map->GetObjectGroup(i);
//        
//        // Iterate through all objects in the object group.
//        for (int j = 0; j < objectGroup->GetNumObjects(); ++j)
//        {
//            // Get an object.
//            const Tmx::Object *object = objectGroup->GetObject(j);
//            
//            // Print information about the object.
//            printf("Object Name: %s\n", object->GetName().c_str());
//            printf("Object Position: (%03d, %03d)\n", object->GetX(),
//                   object->GetY());
//            printf("Object Size: (%03d, %03d)\n", object->GetWidth(),
//                   object->GetHeight());
//            
//            // Print Polygon points.
//            const Tmx::Polygon *polygon = object->GetPolygon();
//            if (polygon != 0)
//            {
//                for (int i = 0; i < polygon->GetNumPoints(); i++)
//                {
//                    const Tmx::Point &point = polygon->GetPoint(i);
//                    printf("Object Polygon: Point %d: (%f, %f)\n", i, point.x,
//                           point.y);
//                }
//            }
//            
//            // Print Polyline points.
//            const Tmx::Polyline *polyline = object->GetPolyline();
//            if (polyline != 0)
//            {
//                for (int i = 0; i < polyline->GetNumPoints(); i++)
//                {
//                    const Tmx::Point &point = polyline->GetPoint(i);
//                    printf("Object Polyline: Point %d: (%f, %f)\n", i, point.x,
//                           point.y);
//                }
//            }
//        }
//    }
//    
//    delete map;
    
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
    
        float PINEAPPLE_POS[] = {10.0f, 7.0f};
    
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
    
    _level = LevelModel::create();
    _level->addLength(LEVEL_LENGTH);
    
    addGoal(GOAL_POS);
    addWalls(WALL_COUNT, walls);
    addPlatforms(PLATFORM_COUNT, PLATFORM);
    addPineapple(PINEAPPLE_POS);
    addKids(KID_POS);
    addCups(CUP_COUNT, CUP_POS);
    addJello(JELLO_COUNT, JELLO_POS);
    addSpikes(SPIKE_COUNT, SPIKE_POS);
    addBlender(BLENDER_POS);
    
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

