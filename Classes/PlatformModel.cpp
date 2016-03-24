#include "PlatformModel.h"
#include <cornell/CUPolygonNode.h>
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>


#pragma mark -
#pragma mark Physics Constants
/** The amount to shrink the body fixture (vertically) relative to the image */
#define platform_VSHRINK  0.8f
/** The amount to shrink the body fixture (horizontally) relative to the image */
#define platform_HSHRINK  0.72f
/** Friction of most platforms */
#define BASIC_FRICTION  0.4f
/** The density for most physics objects */
#define BASIC_DENSITY   0.0f
/** The restitution for all physics objects */
#define BASIC_RESTITUTION   0.1f

#pragma mark -
#pragma mark Static Constructors

/**
 * Creates a new platform at the given position.
 *
 * The platform is sized according to the given drawing scale.
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @param  pos      Initial position in world coordinates
 * @param  scale    The drawing scale
 *
 * @return  An autoreleased physics object
 */
PlatformModel* PlatformModel::create(const Vec2& pos, const Vec2& scale, const bool isMainFloor) {
    PlatformModel* platform = new (std::nothrow) PlatformModel(isMainFloor);
    if (platform && platform->init(pos, scale)) {
        platform->autorelease();
        return platform;
    }
    CC_SAFE_DELETE(platform);
    return nullptr;
}

PlatformModel* PlatformModel::create(const Vec2& pos, const Vec2& scale, const Size& size, const bool isMainFloor) {
    PlatformModel* platform = new (std::nothrow) PlatformModel(isMainFloor);
    if (platform && platform->init(pos, scale)) {
        platform->resize(size);
        platform->autorelease();
        return platform;
    }
    CC_SAFE_DELETE(platform);
    return nullptr;
}

#pragma mark -
#pragma mark Initializers

/**
 * Initializes a new platform at the given position.
 *
 * The platform is sized according to the given drawing scale.
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @param  pos      Initial position in world coordinates
 * @param  scale    The drawing scale
 *
 * @return  true if the obstacle is initialized properly, false otherwise.
 */
bool PlatformModel::init(const Vec2& pos, const Vec2& scale) {
    SceneManager* scene = AssetManager::getInstance()->getCurrent();
    Texture2D* image;
    if (_isMainFloor) {
        image = scene->get<Texture2D>(MAIN_FLOOR_TEXTURE);
    } else {
        image = scene->get<Texture2D>(PLATFORM_TEXTURE);
    }
    
    // Multiply by the scaling factor so we can be resolution independent
    float cscale = Director::getInstance()->getContentScaleFactor();
    Size nsize = image->getContentSize()*cscale;
    
    
    nsize.width *= platform_HSHRINK / scale.x;
    nsize.height *= platform_VSHRINK / scale.y;
    if (BoxObstacle::init(pos, nsize)) {
        setName(PLATFORM_NAME);
        
        setBodyType(b2_staticBody);
        setFriction(BASIC_FRICTION);
        setDensity(BASIC_DENSITY);
        setRestitution(BASIC_RESTITUTION);
        setFixedRotation(true);
        
        return true;
    }
    return false;
}


#pragma mark -
#pragma mark Physics Methods
/**
 * Create new fixtures for this body, defining the shape
 *
 * This is the primary method to override for custom physics objects
 */
void PlatformModel::createFixtures() {
    if (_body == nullptr) {
        return;
    }
    
    BoxObstacle::createFixtures();
}

/**
 * Release the fixtures for this body, reseting the shape
 *
 * This is the primary method to override for custom physics objects.
 */
void PlatformModel::releaseFixtures() {
    if (_body == nullptr) {
        return;
    }
    
    BoxObstacle::releaseFixtures();
}

/**
 * Updates the object's physics state (NOT GAME LOGIC).
 *
 * We use this method to reset cooldowns.
 *
 * @param delta Number of seconds since last animation frame
 */
void PlatformModel::update(float dt) {
    BoxObstacle::update(dt);
}


#pragma mark -
#pragma mark Scene Graph Methods

/**
 * Redraws the outline of the physics fixtures to the debug node
 *
 * The debug node is use to outline the fixtures attached to this object.
 * This is very useful when the fixtures have a very different shape than
 * the texture (e.g. a circular shape attached to a square texture).
 */
void PlatformModel::resetDebugNode() {
    BoxObstacle::resetDebugNode();
}