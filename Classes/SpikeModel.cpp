//
//  SpikeModel.cpp
//

#include "SpikeModel.h"
#include <cornell/CUPolygonNode.h>
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>

#pragma mark -
#pragma mark Physics Constants

#define SPIKE_SCALE 0.5

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
 * @return  An retained physics object
 */
SpikeModel* SpikeModel::create() {
    SpikeModel* spike = new (std::nothrow) SpikeModel();
    if (spike && spike->init()) {
        spike->retain();
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
 * @return  An retained physics object
 */
SpikeModel* SpikeModel::create(const Vec2& pos) {
    SpikeModel* spike = new (std::nothrow) SpikeModel();
    if (spike && spike->init(pos)) {
        spike->setPosition(pos + Vec2(spike->getWidth()/2, 0));
        spike->retain();
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
 * @return  An retained physics object
 */
SpikeModel* SpikeModel::create(const Vec2& pos, const Vec2& scale) {
    SpikeModel* spike = new (std::nothrow) SpikeModel();
    if (spike && spike->init(pos,scale)) {
        spike->retain();
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
    if (BoxObstacle::init(pos,Size(scale))) {
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
 * Performs any necessary additions to the scene graph node.
 *
 * This method is necessary for custom physics objects that are composed
 * of multiple scene graph nodes.  In this case, it is because we
 * manage our own afterburner animations.
 */
void SpikeModel::resetSceneNode() {
    PolygonNode* pnode = dynamic_cast<PolygonNode*>(_node);
    if (pnode != nullptr) {
        // We need to know the content scale for resolution independence
        // If the device is higher resolution than 1024x576, Cocos2d will scale it
        // THIS DOES NOT FIX ASPECT RATIO PROBLEMS
        // If you are using a device with a 3:2 aspect ratio, you will need to
        // completely redo the level layout.  We can help if this is an issue.
        float cscale = Director::getInstance()->getContentScaleFactor();
        
        Rect bounds;
        bounds.size = pnode->getContentSize();
        
        pnode->setPolygon(bounds);
        pnode->setScale(cscale * SPIKE_SCALE);
        
        setDimension(pnode->getContentSize().width * SPIKE_SCALE / _drawScale.x,
                     pnode->getContentSize().height * SPIKE_SCALE / _drawScale.y);
    }
}

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
