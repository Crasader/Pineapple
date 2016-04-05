//
//  KidModel.cpp
//  PineappleModel
//
//  Created by Michael Patashnik on 2/24/16.
//
//

#include "KidModel.h"
//#include <cornell/CUPolygonNode.h>
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>

#pragma mark -
#pragma mark Physics Constants
/** The amount to shrink the body fixture (vertically) relative to the image */
#define KID_VSHRINK  0.95f
/** The amount to shrink the body fixture (horizontally) relative to the image */
#define KID_HSHRINK  0.7f
/** The amount to shrink the sensor fixture (horizontally) relative to the image */
#define KID_SSHRINK  0.9f
/** Height of the sensor attached to the player's feet */
#define SENSOR_HEIGHT   0.1f
/** The density of the character */
#define KID_DENSITY    .5f
/** The amount kids change force to get back to max speed */
#define KID_FORCE    5.0f
/** Epsilon on Kid Speed - if within this amount, just set to max speed */
#define KID_SPEED_EPSILON   .1f


#pragma mark -
#pragma mark Static Constructors
/**
 * Creates a new dude at the origin.
 *
 * The dude is scaled so that 1 pixel = 1 Box2d unit
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @return  An autoreleased physics object
 */
KidModel* KidModel::create() {
    KidModel* dude = new (std::nothrow) KidModel();
    if (dude && dude->init()) {
        dude->autorelease();
        return dude;
    }
    CC_SAFE_DELETE(dude);
    return nullptr;
}

/**
 * Creates a new dude at the given position.
 *
 * The dude is scaled so that 1 pixel = 1 Box2d unit
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
KidModel* KidModel::create(const Vec2& pos) {
    KidModel* dude = new (std::nothrow) KidModel();
    if (dude && dude->init(pos)) {
        dude->autorelease();
        return dude;
    }
    CC_SAFE_DELETE(dude);
    return nullptr;
}

/**
 * Creates a new dude at the given position.
 *
 * The dude is sized according to the given drawing scale.
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @param  pos      Initial position in world coordinates
 * @param  scale    The drawing scale
 * @param  idx      The index of this kid, for selecting texture, in range [0..NUM_KIDS)
 *
 * @return  An autoreleased physics object
 */
KidModel* KidModel::create(const Vec2& pos, const Vec2& scale, int idx) {
    KidModel* dude = new (std::nothrow) KidModel();
    if (dude && dude->init(pos,scale,idx)) {
        dude->autorelease();
        return dude;
    }
    CC_SAFE_DELETE(dude);
    return nullptr;
}

/**
 * Returns the kid texture name for the given kid index
 */
string KidModel::getTexture(int idx) {
    if (idx == 0) {
        return KID_TEXTURE_1;
    } else if (idx == 1) {
        return KID_TEXTURE_2;
    } else if (idx == 2) {
        return KID_TEXTURE_3;
    } else if (idx == 3) {
        return KID_TEXTURE_4;
    } else {
        CC_ASSERT(false);
        return nullptr;
    }
}


#pragma mark -
#pragma mark Initializers

/**
 * Initializes a new dude at the given position.
 *
 * The dude is sized according to the given drawing scale.
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @param  pos      Initial position in world coordinates
 * @param  scale    The drawing scale
 * @param  idx      The index of this kid, for selecting texture, in range [0..NUM_KIDS)
 *
 * @return  true if the obstacle is initialized properly, false otherwise.
 */
bool KidModel::init(const Vec2& pos, const Vec2& scale, int idx) {
    SceneManager* scene = AssetManager::getInstance()->getCurrent();
    Texture2D* image = scene->get<Texture2D>(getTexture(idx));
    
    // Multiply by the scaling factor so we can be resolution independent
    float cscale = Director::getInstance()->getContentScaleFactor();
    Size nsize = image->getContentSize()*cscale;
    
    _index = idx;
    
    nsize.width  *= KID_HSHRINK/(12.0f*scale.x);
    nsize.height *= KID_VSHRINK/scale.y;
    if (CapsuleObstacle::init(pos,nsize)) {
        setDensity(KID_DENSITY);
        setFriction(0.0f);      // HE WILL STICK TO WALLS IF YOU FORGET
        setFixedRotation(true); // OTHERWISE, HE IS A WEEBLE WOBBLE
        
        // Gameplay attributes
        _isCollidingWithJello = false;
        _isGrounded = false;
		_reachedGoal = false;
        return true;
    }
    return false;
}


#pragma mark -
#pragma mark Attribute Properties

/**
 * Sets left/right movement of this character.
 *
 * This is the result of input times dude force.
 *
 * @param value left/right movement of this character.
 */
void KidModel::setMovement(float value) {
    _movement = value;
    bool face = _movement > 0;
    if (_movement == 0) {
        return;
    }
    
    // Change facing
    /*TexturedNode* image = dynamic_cast<TexturedNode*>(_node);
    if (image != nullptr) {
        image->flipHorizontal(!image->isFlipHorizontal());
    }*/
}

/**
* Initialize the filmstrip for walking animation
*/
void KidModel::initAnimation(Texture2D* image, float scale) {
	_kidWalkcycleFrame = 0.0f;
	_kidWalkcycle = AnimationNode::create(image, 1, 12, 12);
	_kidWalkcycle->setScale(scale);
	_kidWalkcycle->setFrame(_kidWalkcycleFrame);
	this->setSceneNode(_kidWalkcycle);
}


#pragma mark -
#pragma mark Physics Methods
/**
 * Create new fixtures for this body, defining the shape
 *
 * This is the primary method to override for custom physics objects
 */
void KidModel::createFixtures() {
    if (_body == nullptr) {
        return;
    }
    
    CapsuleObstacle::createFixtures();
    b2FixtureDef sensorDef;
    sensorDef.density = KID_DENSITY;
    sensorDef.isSensor = true;
    
    // Sensor dimensions
    b2Vec2 corners[4];
    corners[0].x = -KID_SSHRINK*getWidth()/2.0f;
    corners[0].y = (-getHeight()+SENSOR_HEIGHT)/2.0f;
    corners[1].x = -KID_SSHRINK*getWidth()/2.0f;
    corners[1].y = (-getHeight()-SENSOR_HEIGHT)/2.0f;
    corners[2].x =  KID_SSHRINK*getWidth()/2.0f;
    corners[2].y = (-getHeight()-SENSOR_HEIGHT)/2.0f;
    corners[3].x =  KID_SSHRINK*getWidth()/2.0f;
    corners[3].y = (-getHeight()+SENSOR_HEIGHT)/2.0f;
    
    b2PolygonShape sensorShape;
    sensorShape.Set(corners,4);
    
    sensorDef.shape = &sensorShape;
    _sensorFixture = _body->CreateFixture(&sensorDef);
    _sensorFixture->SetUserData(getSensorName());
}

/**
 * Release the fixtures for this body, reseting the shape
 *
 * This is the primary method to override for custom physics objects.
 */
void KidModel::releaseFixtures() {
    if (_body == nullptr) {
        return;
    }
    
    CapsuleObstacle::releaseFixtures();
    if (_sensorFixture != nullptr) {
        _body->DestroyFixture(_sensorFixture);
        _sensorFixture = nullptr;
    }
}

/**
 * Applies the force to the body of this dude
 *
 * This method should be called after the force attribute is set.
 */
void KidModel::dampTowardsWalkspeed() {
    if (!isActive()) {
        return;
    }

    if (_isGrounded && getVX() != KID_WALKSPEED) {
        if(fabs(getVX() - KID_WALKSPEED) < KID_SPEED_EPSILON) {
            setVX(KID_WALKSPEED);
        } else if (getVX() > getWalkingSpeed()) {
            b2Vec2 force(-KID_FORCE,0);
            _body->ApplyForce(force,_body->GetPosition(),true);
        }
        else if (getVX() < getWalkingSpeed()) {
            b2Vec2 force(KID_FORCE,0);
            _body->ApplyForce(force,_body->GetPosition(),true);
        }
    }
    
//    // Velocity too high, clamp it
//    if (fabs(getVX()) >= getMaxWalkingSpeed() && _isGrounded) {
//        setVX(SIGNUM(getVX())*getMaxWalkingSpeed());
//    } else {
//        b2Vec2 force(getMovement(),0);
//        _body->ApplyForce(force,_body->GetPosition(),true);
//    }
}

/**
 * Updates the object's physics state (NOT GAME LOGIC).
 *
 * We use this method to reset cooldowns.
 *
 * @param delta Number of seconds since last animation frame
 */
void KidModel::update(float dt) {
    CapsuleObstacle::update(dt);
}

/**
* Animate the kid if they're moving
*/
void KidModel::animate() {
	// in the air
	if (!_isGrounded || getVY() < -0.2f) {
		_kidWalkcycleFrame = 0.0f;
		_kidWalkcycle->setFrame(_kidWalkcycleFrame);
	}
	// moving
	else if (getVX() > 0.2f) {
		_kidWalkcycleFrame += 0.5f;
		int tmp = (int)rint(_kidWalkcycleFrame);
		CCLOG("frame: %f", _kidWalkcycleFrame);
		CCLOG("frame no. is: %d", tmp);
		_kidWalkcycle->setFrame(tmp % 12);
	}
	// at rest
	else {
		_kidWalkcycleFrame = 0.0f;
		_kidWalkcycle->setFrame(_kidWalkcycleFrame);
	}
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
void KidModel::resetDebugNode() {
    CapsuleObstacle::resetDebugNode();
    float w = KID_SSHRINK*_dimension.width*_drawScale.x;
    float h = SENSOR_HEIGHT*_drawScale.y;
    Poly2 poly(Rect(-w/2.0f,-h/2.0f,w,h));
    poly.traverse(Poly2::Traversal::INTERIOR);
    
    _sensorNode = WireNode::createWithPoly(poly);
    _sensorNode->setPosition(Vec2(_debug->getContentSize().width/2.0f, 0.0f));
    _debug->addChild(_sensorNode);	
}



