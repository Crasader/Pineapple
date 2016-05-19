//
//  PineappleModel.cpp
//  G.M.P.
//
//  Author: Flameingos
//  Version: TODO
//
#include "PineappleModel.h"
#include <cornell/CUAssetManager.h>
//#include <cornell/CUSceneManager.h>

#pragma mark -
#pragma mark Physics Constants
/** Cooldown (in animation frames) for jumping */
#define JUMP_COOLDOWN				5
/** The amount to shrink the body fixture (vertically) relative to the image */
#define PINEAPPLE_VSHRINK			0.6f
/** The amount to shrink the body fixture (horizontally) relative to the image */
#define PINEAPPLE_HSHRINK			0.7f
/** The amount to shrink the sensor fixture (horizontally) relative to the image */
#define PINEAPPLE_SSHRINK			0.9f
/** Height of the sensor attached to the player's feet */
#define SENSOR_HEIGHT				0.07f
/** offset for the foot sensor to enable some logic in collisions before the pineapples physics kick in */
#define SENSOR_V_OFFSET             -0.085f
/** The density of the character */
#define PINEAPPLE_DENSITY			0.5f
/** The impulse for the character jump */
#define PINEAPPLE_JUMP              10.0f
/** Anchor point that is in the center of the pineapple's mass */
#define PINEAPPLE_ANCHOR_POINT   Vec2(0.5f, 0.25f)

#pragma mark -
#pragma mark Static Constructors
/**
 * Creates a new pineapple at the origin.
 *
 * The pineapple is scaled so that 1 pixel = 1 Box2d unit
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @return  An retained physics object
 */
PineappleModel* PineappleModel::create() {
    PineappleModel* pineapple = new (std::nothrow) PineappleModel();
    if (pineapple && pineapple->init()) {
        pineapple->retain();
        return pineapple;
    }
    CC_SAFE_DELETE(pineapple);
    return nullptr;
}

/**
 * Creates a new pineapple at the given position.
 *
 * The pineapple is scaled so that 1 pixel = 1 Box2d unit
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
PineappleModel* PineappleModel::create(const Vec2& pos) {
    PineappleModel* pineapple = new (std::nothrow) PineappleModel();
    if (pineapple && pineapple->init(pos)) {
        pineapple->retain();
        return pineapple;
    }
    CC_SAFE_DELETE(pineapple);
    return nullptr;
}

/**
 * Creates a new pineapple at the given position.
 *
 * The pineapple is sized according to the given drawing scale.
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
PineappleModel* PineappleModel::create(const Vec2& pos, const Vec2& scale) {
    PineappleModel* pineapple = new (std::nothrow) PineappleModel();
    if (pineapple && pineapple->init(pos, scale)) {
        pineapple->setDensity(PINEAPPLE_DENSITY);
        pineapple->cocos2d::Obstacle::setMass(PINEAPPLE_NORMAL_MASS);
        pineapple->resetMass();
        pineapple->retain();
        return pineapple;
    }
    CC_SAFE_DELETE(pineapple);
    return nullptr;
}


#pragma mark -
#pragma mark Initializers

/**
 * Initializes a new pineapple at the given position.
 *
 * The pineapple is sized according to the given drawing scale.
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
bool PineappleModel::init(const Vec2& pos, const Vec2& scale) {
    if (CapsuleObstacle::init(pos, Size(100, 101))) {
        setDensity(PINEAPPLE_DENSITY);
        
        // Gameplay attributes
        _isGrounded = false;
        _isJumping = false;
        _isCollidingWithJello = false;
        _isCollidingWithButtonSwitch = false;
        _faceRight = true;
        _reachedGoal = false;
		_isBlended = false;
		_isDead = false;
		_willWalkcycleFrame = 0;
        _jumpCooldown = 0;
        return true;
    }
    return false;
}

/**
 * Sets left/right movement of this character.
 *
 * This is the result of input times pineapple force.
 *
 * @param value left/right movement of this character.
 */
void PineappleModel::setMovement(float value) {
    _movement = value;
    bool face = _movement > 0;
    if (_movement == 0 || _faceRight == face) {
        return;
    }
    
    // Change facing
    /*TexturedNode* image = dynamic_cast<TexturedNode*>(_node);
     if (image != nullptr) {
     image->flipHorizontal(!image->isFlipHorizontal());
     }*/
    _faceRight = (_movement > 0);
}


#pragma mark -
#pragma mark Physics Methods
/**
 * Create new fixtures for this body, defining the shape
 *
 * This is the primary method to override for custom physics objects
 */
void PineappleModel::createFixtures() {
    if (_body == nullptr) {
        return;
    }
    
    CapsuleObstacle::createFixtures();
    b2FixtureDef sensorDef;
    sensorDef.density = PINEAPPLE_DENSITY;
    sensorDef.isSensor = true;
    
    // Foot sensor dimensions
    b2Vec2 corners[4];
    corners[0].x = -PINEAPPLE_SSHRINK*getWidth() / 2.0f;
    corners[0].y = SENSOR_V_OFFSET + (-getHeight() + SENSOR_HEIGHT) / 2.0f;
    corners[1].x = -PINEAPPLE_SSHRINK*getWidth() / 2.0f;
    corners[1].y = SENSOR_V_OFFSET + (-getHeight() - SENSOR_HEIGHT) / 2.0f;
    corners[2].x = PINEAPPLE_SSHRINK*getWidth() / 2.0f;
    corners[2].y = SENSOR_V_OFFSET + (-getHeight() - SENSOR_HEIGHT) / 2.0f;
    corners[3].x = PINEAPPLE_SSHRINK*getWidth() / 2.0f;
    corners[3].y = SENSOR_V_OFFSET + (-getHeight() + SENSOR_HEIGHT) / 2.0f;
    
    b2PolygonShape sensorShape;
    sensorShape.Set(corners, 4);
    
    sensorDef.shape = &sensorShape;
    _sensorFixture = _body->CreateFixture(&sensorDef);
    _sensorFixture->SetUserData(getSensorName());
    
    // Body sensor dimensions
    corners[0].y = getHeight() / 2.0f;
    corners[1].y = -getHeight() / 2.0f;
    corners[2].y = -getHeight() / 2.0f;
    corners[3].y = getHeight() / 2.0f;
    
    b2PolygonShape sensorBodyShape;
    sensorBodyShape.Set(corners, 4);
    sensorDef.shape = &sensorBodyShape;
    _sensorBodyFixture = _body->CreateFixture(&sensorDef);
    _sensorBodyFixture->SetUserData(&_sensorBodyName);
    
    // Override mass based on shape sizes to custom values
    b2MassData massData = b2MassData();
    massData.mass = this->_isSmall ? PINEAPPLE_SHRUNK_MASS : PINEAPPLE_NORMAL_MASS;
    setDensity(PINEAPPLE_DENSITY);
    _body->SetMassData(&massData);
}

/**
 * Release the fixtures for this body, reseting the shape
 *
 * This is the primary method to override for custom physics objects.
 */
void PineappleModel::releaseFixtures() {
    if (_body == nullptr) {
        return;
    }
    
    CapsuleObstacle::releaseFixtures();
    if (_sensorFixture != nullptr) {
        _body->DestroyFixture(_sensorFixture);
        _sensorFixture = nullptr;
    }
    if (_sensorBodyFixture != nullptr) {
        _body->DestroyFixture(_sensorBodyFixture);
        _sensorBodyFixture = nullptr;
    }
}

int PineappleModel::grow() {
    float currentHeight = getDimension().height;
    if (_isSmall) {
        setDimension(_normalSize);
        setY(getY() + (getDimension().height - currentHeight)/2);
        setIsSmall(false);
        _justGrewShrank = true;
        return 1;
    }
    return 0;
}

int PineappleModel::shrink() {
    float currentHeight = getDimension().height;
    if (!_isSmall) {
        setDimension(_normalSize * PINEAPPLE_SHRINK_SCALE);
        setY(getY() + (getDimension().height - currentHeight)/2);
        setIsSmall(true);
        _justGrewShrank = true;
        return 1;
    }
    return 0;
}

/**
 * Applies the force to the body of this pineapple
 *
 * This method should be called after the force attribute is set.
 */
void PineappleModel::applyForce() {
    if (!isActive()) {
        return;
    }
        
    // Don't want to be moving. Damp out player motion
    if (getMovement() == 0.0f || SIGNUM(getMovement()) != SIGNUM(getVX()) ) {
        b2Vec2 force(-getDamping()*getVX(), 0);
        _body->ApplyForce(force, _body->GetPosition(), true);
    }
    
    // Velocity too high, clamp it
    if (fabs(getVX()) >= getMaxSpeed()) {
        setVX(SIGNUM(getVX())*getMaxSpeed());
    }
    else {
        b2Vec2 force(getMovement(), 0);
        _body->ApplyForce(force, _body->GetPosition(), true);
    }
    
    // Jump!
    if (isJumping() && isGrounded()) {
        b2Vec2 force(0, PINEAPPLE_JUMP);
        _body->ApplyLinearImpulse(force, _body->GetPosition(), true);
    }
}

/**
 * Updates the object's physics state (NOT GAME LOGIC).
 *
 * We use this method to reset cooldowns.
 *
 * @param delta Number of seconds since last animation frame
 */
void PineappleModel::update(float dt) {
    // Apply cooldowns
    if (isJumping()) {
        _jumpCooldown = JUMP_COOLDOWN;
    }
    // Only cooldown while grounded
    else if (isGrounded()){
        _jumpCooldown = (_jumpCooldown > 0 ? _jumpCooldown - 1 : 0);
    }
    if (_justGrewShrank) {
        _framesJustGrewShrank++;
        if (_framesJustGrewShrank >= 2) {
            _justGrewShrank = false;
            _framesJustGrewShrank = 0;
        }
    }
	CapsuleObstacle::update(dt);
}

/**
 * Animate Will if he's moving
 */
void PineappleModel::animate() {
	// in the air
	if (!_isGrounded || _isJumping || getVY() < -0.2f) {
		if (_faceRight) {
			_willWalkcycleFrame = 0;
		}
		else {
			_willWalkcycleFrame = PINEAPPLE_FRAME_COUNT / 2;
		}
		_willWalkcycle->setFrame(_willWalkcycleFrame);
	}
	// moving
	else if (abs(getVX()) > 0.5f) {
		_willWalkcycleFrame++;
		if (_faceRight) {
			_willWalkcycle->setFrame(_willWalkcycleFrame % (PINEAPPLE_FRAME_COUNT / 2));
		}
		else {
			_willWalkcycle->setFrame((_willWalkcycleFrame % (PINEAPPLE_FRAME_COUNT / 2)) + (PINEAPPLE_FRAME_COUNT / 2));
		}
	}
	// at rest
	else {
		if (_faceRight) {
			_willWalkcycleFrame = 0;
		}
		else {
			_willWalkcycleFrame = PINEAPPLE_FRAME_COUNT / 2;
		}
		_willWalkcycle->setFrame(_willWalkcycleFrame);
	}
}

/**
* Make Will spiral towards blender blades
*
* @param x x-coordinate of the blender blades
* @param y y-coordinate of the blender
*/
void PineappleModel::spiral(float x, float y) {
	float val = 0.3f;
	
	// near blades
	if (abs(getY() - y) < val) {
		setY(y);
	}
	// above blades
	else if (getY() > y) {
		setY(getY() - val);
	}
	// below blades
	else {
		setY(getY() + val);
	}

	// move towards left side of screen, die if hit blender blades
	float newX = getX() - val;
	if (newX < x) {
		setIsDead(true);
        Device::vibrate(VIBRATION_DURATION);
	}
	else {
		setX(newX);
	}
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
void PineappleModel::resetSceneNode() {
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
        pnode->setScale(cscale * PINEAPPLE_SCALE);
        pnode->setFrame(0);
        
        setDimension(pnode->getContentSize().width * PINEAPPLE_SCALE * PINEAPPLE_HSHRINK / _drawScale.x,
                     pnode->getContentSize().height * PINEAPPLE_SCALE * (1 - PINEAPPLE_ANCHOR_POINT.y) * PINEAPPLE_VSHRINK / _drawScale.y);
        
        pnode->setAnchorPoint(PINEAPPLE_ANCHOR_POINT);
        
        _willWalkcycleFrame = 0;
        _willWalkcycle = pnode;
        
        _normalSize = getDimension();
    }
}

/**
 * Redraws the outline of the physics fixtures to the debug node
 *
 * The debug node is use to outline the fixtures attached to this object.
 * This is very useful when the fixtures have a very different shape than
 * the texture (e.g. a circular shape attached to a square texture).
 */
void PineappleModel::resetDebugNode() {
    CapsuleObstacle::resetDebugNode();
    
    float w = PINEAPPLE_SSHRINK*_dimension.width*_drawScale.x;
    float h = SENSOR_HEIGHT*_drawScale.y;
    Poly2 poly(Rect(-w / 2.0f, -h / 2.0f, w, h));
    poly.traverse(Poly2::Traversal::INTERIOR);
    
    if(_sensorNode != nullptr) {
        _debug->removeChild(_sensorNode);
    }
    
    _sensorNode = WireNode::createWithPoly(poly);
    _sensorNode->setColor(DEBUG_COLOR);
    _sensorNode->setPosition(Vec2(_debug->getContentSize().width / 2.0f, 0.0f));
    
    _debug->addChild(_sensorNode);
}



