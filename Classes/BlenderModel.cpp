//
//  BlenderModel.cpp
//  PlatformerDemo
//
//  Author: Flameingos
//  Version: TODO
//
#include "BlenderModel.h"
#include <cornell/CUPolygonNode.h>
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>

#pragma mark -
#pragma mark Physics Constants
/** The amount to shrink the body fixture (vertically) relative to the image */
#define BLENDER_VSHRINK  0.8f
/** The amount to shrink the body fixture (horizontally) relative to the image */
#define BLENDER_HSHRINK  0.72f


#pragma mark -
#pragma mark Static Constructors
/**
 * Creates a new blender at the origin.
 *
 * The blender is scaled so that 1 pixel = 1 Box2d unit
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @return  An autoreleased physics object
 */
BlenderModel* BlenderModel::create() {
    BlenderModel* blender = new (std::nothrow) BlenderModel();
    if (blender && blender->init()) {
        blender->autorelease();
        return blender;
    }
    CC_SAFE_DELETE(blender);
    return nullptr;
}

/**
 * Creates a new blender at the given position.
 *
 * The blender is scaled so that 1 pixel = 1 Box2d unit
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
BlenderModel* BlenderModel::create(const Vec2& pos) {
    BlenderModel* blender = new (std::nothrow) BlenderModel();
    if (blender && blender->init(pos)) {
        blender->autorelease();
        return blender;
    }
    CC_SAFE_DELETE(blender);
    return nullptr;
}

/**
 * Creates a new blender at the given position.
 *
 * The blender is sized according to the given drawing scale.
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
BlenderModel* BlenderModel::create(const Vec2& pos, const Vec2& scale) {
    BlenderModel* blender = new (std::nothrow) BlenderModel();
    if (blender && blender->init(pos,scale)) {
        blender->autorelease();
        return blender;
    }
    CC_SAFE_DELETE(blender);
    return nullptr;
}


#pragma mark -
#pragma mark Initializers

/**
 * Initializes a new blender at the given position.
 *
 * The blender is sized according to the given drawing scale.
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
bool BlenderModel::init(const Vec2& pos, const Vec2& scale) {
    SceneManager* scene = AssetManager::getInstance()->getCurrent();
    Texture2D* image = scene->get<Texture2D>(BLENDER_TEXTURE);
    
    // Multiply by the scaling factor so we can be resolution independent
    float cscale = Director::getInstance()->getContentScaleFactor();
    Size nsize = image->getContentSize()*cscale;
    
    nsize.width  *= BLENDER_HSHRINK/scale.x;
    nsize.height *= BLENDER_VSHRINK/scale.y;
    if (BoxObstacle::init(pos,nsize)) {
        setFriction(0.0f);      // HE WILL STICK TO WALLS IF YOU FORGET
        setFixedRotation(true); // OTHERWISE, HE IS A WEEBLE WOBBLE
        setVX(BLENDER_SPEED);
        
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
void BlenderModel::createFixtures() {
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
void BlenderModel::releaseFixtures() {
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
void BlenderModel::update(float dt) {
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
void BlenderModel::resetDebugNode() {
    BoxObstacle::resetDebugNode();
}



