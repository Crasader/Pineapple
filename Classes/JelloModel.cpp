//
//  PFJelloModel.cpp
//  PlatformerDemo
//
//  This encapsulates all of the information for the character avatar.  Note how this
//  class combines physics and animation.  This is a good template for models in
//  your game.
//
//  WARNING: There are a lot of shortcuts in this design that will do not adapt well
//  to data driven design.  This demo has a lot of simplifications to make it a bit
//  easier to see how everything fits together.  However, the model classes and how
//  they are initialized will need to be changed if you add dynamic level loading.
//
//  Pay close attention to how this class designed.  Subclasses of Cocos2d classes
//  (which are all subclasses of the class Ref) should never have normal public
//  constructors.  Instead, you should organize their constructors into three parts,
//  just like we have done in this class.
//
//  NORMAL CONSTRUCTOR:
//  The standard constructor should be protected (not private).  It should only
//  initialize pointers to nullptr and primitives to their defaults (pointers are
//  not always nullptr to begin with).  It should NOT take any arguments and should
//  not allocate any memory or call any methods.
//
//  STATIC CONSTRUCTOR
//  This is a static method that allocates the object and initializes it.  If
//  initialization fails, it immediately disposes of the object.  Otherwise, it
//  returns an autoreleased object, starting the garbage collection system.
//  These methods all look the same.  You can copy-and-paste them from sample code.
//  The only difference is the init method called.
//
//  INIT METHOD
//  This is a protected method that acts like what how would normally think a
//  constructor might work.  It allocates memory and initializes all values
//  according to provided arguments.  As memory allocation can fail, this method
//  needs to return a boolean indicating whether or not initialization was
//  successful.
//
//  This file is based on the CS 3152 PhysicsDemo Lab by Don Holden, 2007
//
//  Author: Walker White
//  Version: 1/15/15
//
#include "JelloModel.h"
#include <cornell/CUPolygonNode.h>
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>

#define SIGNUM(x)  ((x > 0) - (x < 0))

#pragma mark -
#pragma mark Physics Constants
/** The amount to shrink the body fixture (vertically) relative to the image */
#define JELLO_VSHRINK  0.8f
/** The amount to shrink the body fixture (horizontally) relative to the image */
#define JELLO_HSHRINK  0.72f

#pragma mark -
#pragma mark Static Constructors
/**
 * Creates a new jello at the origin.
 *
 * The jello is scaled so that 1 pixel = 1 Box2d unit
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @return  An autoreleased physics object
 */
JelloModel* JelloModel::create() {
    JelloModel* jello = new (std::nothrow) JelloModel();
    if (jello && jello->init()) {
        
        jello->autorelease();
        return jello;
    }
    CC_SAFE_DELETE(jello);
    return nullptr;
}

/**
 * Creates a new jello at the given position.
 *
 * The jello is scaled so that 1 pixel = 1 Box2d unit
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @param  pos      Initial position in world coordinates (of bottom left)
 *
 * @return  An autoreleased physics object
 */
JelloModel* JelloModel::create(const Vec2& pos) {
    JelloModel* jello = new (std::nothrow) JelloModel();
    if (jello && jello->init(pos)) {
        jello->setPosition(pos + Vec2(0, jello->getHeight()/2));
        jello->autorelease();
        return jello;
    }
    CC_SAFE_DELETE(jello);
    return nullptr;
}

/**
 * Creates a new jello at the given position.
 *
 * The jello is sized according to the given drawing scale.
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @param  pos      Initial position in world coordinates (of bottom left)
 * @param  scale    The drawing scale
 *
 * @return  An autoreleased physics object
 */
JelloModel* JelloModel::create(const Vec2& pos, const Vec2& scale) {
    JelloModel* jello = new (std::nothrow) JelloModel();
    if (jello && jello->init(pos,scale)) {
        jello->setPosition(pos + Vec2(0, jello->getHeight()/2));
        jello->autorelease();
        return jello;
    }
    CC_SAFE_DELETE(jello);
    return nullptr;
}


#pragma mark -
#pragma mark Initializers

/**
 * Initializes a new jello at the given position.
 *
 * The jello is sized according to the given drawing scale.
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
bool JelloModel::init(const Vec2& pos, const Vec2& scale) {
    SceneManager* scene = AssetManager::getInstance()->getCurrent();
    Texture2D* image = scene->get<Texture2D>(JELLO_TEXTURE);
    
    // Multiply by the scaling factor so we can be resolution independent
    float cscale = Director::getInstance()->getContentScaleFactor();
    Size nsize = image->getContentSize()*cscale;
    
    
    nsize.width  *= JELLO_HSHRINK/scale.x;
    nsize.height *= JELLO_VSHRINK/scale.y;
    if (BoxObstacle::init(pos,nsize)) {
        setFriction(0.0f);      // HE WILL STICK TO WALLS IF YOU FORGET
        setFixedRotation(true); // OTHERWISE, HE IS A WEEBLE WOBBLE
        
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
void JelloModel::createFixtures() {
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
void JelloModel::releaseFixtures() {
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
void JelloModel::update(float dt) {
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
void JelloModel::resetDebugNode() {
    BoxObstacle::resetDebugNode();
}



