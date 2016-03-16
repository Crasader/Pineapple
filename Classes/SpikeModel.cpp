//
//  SpikeModel.cpp
//

#include "SpikeModel.h"
//#include <cornell/CUPolygonNode.h>
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>

#define SIGNUM(x)  ((x > 0) - (x < 0))

#pragma mark -
#pragma mark Physics Constants
/** The amount to shrink the body fixture (vertically) relative to the image */
#define SPIKE_VSHRINK  0.8f
/** The amount to shrink the body fixture (horizontally) relative to the image */
#define SPIKE_HSHRINK  0.72f

#pragma mark -
#pragma mark Static Constructors
/**
 * Creates a new spike at the origin.
 *
 * The spike is scaled so that 1 pixel = 1 Box2d unit
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @return  An autoreleased physics object
 */
SpikeModel* SpikeModel::create() {
    SpikeModel* spike = new (std::nothrow) SpikeModel();
    if (spike && spike->init()) {
        spike->autorelease();
        return spike;
    }
    CC_SAFE_DELETE(spike);
    return nullptr;
}

/**
 * Creates a new spike at the given position.
 *
 * The spike is scaled so that 1 pixel = 1 Box2d unit
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @param  pos      Initial position in world coordinates
 *
 * @return  An autoreleased physics object
 */
SpikeModel* SpikeModel::create(const Vec2& pos) {
    SpikeModel* spike = new (std::nothrow) SpikeModel();
    if (spike && spike->init(pos)) {
        spike->setPosition(pos + Vec2(spike->getWidth()/2, spike->getHeight()/2));
        spike->autorelease();
        return spike;
    }
    CC_SAFE_DELETE(spike);
    return nullptr;
}

/**
 * Creates a new spike at the given position.
 *
 * The spike is sized according to the given drawing scale.
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @param  pos      Initial position in world coordinates(of bottom left point)
 * @param  scale    The drawing scale
 *
 * @return  An autoreleased physics object
 */
SpikeModel* SpikeModel::create(const Vec2& pos, const Vec2& scale) {
    SpikeModel* spike = new (std::nothrow) SpikeModel();
    if (spike && spike->init(pos,scale)) {
        spike->setPosition(pos + Vec2(spike->getWidth()/2, spike->getHeight()/2));
        spike->autorelease();
        return spike;
    }
    CC_SAFE_DELETE(spike);
    return nullptr;
}


#pragma mark -
#pragma mark Initializers

/**
 * Initializes a new spike at the given position.
 *
 * The spike is sized according to the given drawing scale.
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
bool SpikeModel::init(const Vec2& pos, const Vec2& scale) {
    SceneManager* scene = AssetManager::getInstance()->getCurrent();
    Texture2D* image = scene->get<Texture2D>(SPIKE_TEXTURE);
    
    // Multiply by the scaling factor so we can be resolution independent
    float cscale = Director::getInstance()->getContentScaleFactor();
    Size nsize = image->getContentSize()*cscale;
    
    
    nsize.width  *= SPIKE_HSHRINK/scale.x;
    nsize.height *= SPIKE_VSHRINK/scale.y;
    if (BoxObstacle::init(pos,nsize)) {
        setFriction(0.0f);
        setFixedRotation(true);
        
        // Gameplay attributes
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
void SpikeModel::createFixtures() {
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
void SpikeModel::releaseFixtures() {
    if (_body != nullptr) {
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
void SpikeModel::update(float dt) {
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
void SpikeModel::resetDebugNode() {
    BoxObstacle::resetDebugNode();
}
