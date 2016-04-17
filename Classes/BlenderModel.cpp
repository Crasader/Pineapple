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
/** The Blender specific scaling */
#define BLENDER_SCALE    2.68f
/** The amount to shrink the sensor fixture (horizontally) relative to the image */
#define BLENDER_SSHRINK  0.9f
/** Height of the sensor attached to the player's feet */
#define SENSOR_WIDTH     0.1f

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
 * @return  An retained physics object
 */
BlenderModel* BlenderModel::create() {
    BlenderModel* blender = new (std::nothrow) BlenderModel();
    if (blender && blender->init()) {
        blender->retain();
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
 * @return  An retained physics object
 */
BlenderModel* BlenderModel::create(const Vec2& pos) {
    BlenderModel* blender = new (std::nothrow) BlenderModel();
    if (blender && blender->init(pos)) {
        blender->retain();
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
 * @return  An retained physics object
 */
BlenderModel* BlenderModel::create(const Vec2& pos, const Vec2& scale) {
    BlenderModel* blender = new (std::nothrow) BlenderModel();
    if (blender && blender->init(pos,scale)) {
        blender->retain();
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
    if (BoxObstacle::init(pos,Size(scale))) {
        setVX(BLENDER_SPEED);
        
        // Gameplay attributes
		_isBlending = false;
		_blendcycleFrame = 0;
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
	b2FixtureDef sensorDef;
	sensorDef.density = 0.5f;
	sensorDef.isSensor = true;

	// Sensor dimensions
	b2Vec2 corners[4];
	corners[0].x = -SENSOR_WIDTH / 2.0f;
	corners[0].y =  BLENDER_SSHRINK*getHeight() / 2.0f;
	corners[1].x = -SENSOR_WIDTH / 2.0f;
	corners[1].y = -BLENDER_SSHRINK*getHeight() / 2.0f;
	corners[2].x =  SENSOR_WIDTH / 2.0f;
	corners[2].y = -BLENDER_SSHRINK*getHeight() / 2.0f;
	corners[3].x =  SENSOR_WIDTH / 2.0f;
	corners[3].y =  BLENDER_SSHRINK*getHeight() / 2.0f;

	b2PolygonShape sensorShape;
	sensorShape.Set(corners, 4);

	sensorDef.shape = &sensorShape;
	_sensorFixture = _body->CreateFixture(&sensorDef);
	_sensorFixture->SetUserData(getSensorName());
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
 * Performs any necessary additions to the scene graph node.
 *
 * This method is necessary for custom physics objects that are composed
 * of multiple scene graph nodes.  In this case, it is because we
 * manage our own afterburner animations.
 */
void BlenderModel::resetSceneNode() {
    AnimationNode* pnode = dynamic_cast<AnimationNode*>(_node);
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
		pnode->setScale(cscale * BLENDER_SCALE);
		pnode->setFrame(0);
        
        setDimension(pnode->getContentSize().width * BLENDER_SCALE / _drawScale.x,
                     pnode->getContentSize().height * BLENDER_SCALE / _drawScale.y);

		_blendcycleFrame = 0;
		_blendcycle = pnode;
    }
}

/**
 * Redraws the outline of the physics fixtures to the debug node
 *
 * The debug node is use to outline the fixtures attached to this object.
 * This is very useful when the fixtures have a very different shape than
 * the texture (e.g. a circular shape attached to a square texture).
 */
void BlenderModel::resetDebugNode() {
    BoxObstacle::resetDebugNode();

	float w = SENSOR_WIDTH*_drawScale.x;
	float h = BLENDER_SSHRINK*_dimension.height*_drawScale.y;
	Poly2 poly(Rect(-w / 2.0f, -h / 2.0f, w, h));
	poly.traverse(Poly2::Traversal::INTERIOR);

	if (_sensorNode != nullptr) {
		_debug->removeChild(_sensorNode);
	}

	_sensorNode = WireNode::createWithPoly(poly);
	_sensorNode->setPosition(Vec2(0.35f*_debug->getContentSize().width, 0.5f*_debug->getContentSize().height));
	_sensorNode->setColor(DEBUG_COLOR);
	_debug->addChild(_sensorNode);
}

/**
* Animate blender
*/
void BlenderModel::animate() {
	_blendcycleFrame++;
	if (_isBlending) {
		_blendcycle->setFrame((_blendcycleFrame % 8) + 2);
	}
	else {
		_blendcycle->setFrame(_blendcycleFrame % 2);
	}
}

