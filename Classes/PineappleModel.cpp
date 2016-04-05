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
#define PINEAPPLE_VSHRINK		0.95f
/** The amount to shrink the body fixture (horizontally) relative to the image */
#define PINEAPPLE_HSHRINK		0.7f
/** The amount to shrink the sensor fixture (horizontally) relative to the image */
#define PINEAPPLE_SSHRINK		0.9f
/** Height of the sensor attached to the player's feet */
#define SENSOR_HEIGHT				0.07f
/** The density of the character */
#define PINEAPPLE_DENSITY   0.5f
/** The impulse for the character jump */
#define PINEAPPLE_JUMP      10.0f


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
    if (CapsuleObstacle::init(pos, Size(scale))) {
		setDensity(PINEAPPLE_DENSITY);

														// Gameplay attributes
		_isGrounded = false;
		_isJumping = false;
		_isCollidingWithJello = false;
		_faceRight = true;
		_reachedGoal = false;

		_jumpCooldown = 0;
		return true;
	}
	return false;
}

#pragma mark -
#pragma mark Attribute Properties
/**
* TODO: method spec
*/
int PineappleModel::updateSize(float dt) {
	if (_isLarge || _isSmall) {
		_durationSinceGrowOrShrink += dt;
	}
	if (_durationSinceGrowOrShrink > PINEAPPLE_MAX_SIZE_DURATION) {
		_isLarge = false;
		_isSmall = false;
		_durationSinceGrowOrShrink = 0.0f;
		toNormalSize();
		return 1;
	}
	return 0;
}

/**
* Grows the pineapple, returns 1 if grown
*/
int PineappleModel::grow() {
	if (!_isLarge  && !_isSmall) {
		setDimension(_normalSize * PINEAPPLE_GROW_SCALE);
		setIsLarge(true);
		return 1;
	}
	return 0;
}

/**
* Shrinks the pineapple, returns 1 if shrunk
*/
int PineappleModel::shrink() {
	if (!_isLarge && !_isSmall) {
		setDimension(_normalSize * PINEAPPLE_SHRINK_SCALE);
		setIsSmall(true);
		return 1;
	}
	return 0;
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

/**
* Initialize the filmstrip for walking animation
*/
void PineappleModel::initAnimation(Texture2D* image, float scale) {
	_willWalkcycleFrame = 0;
	_willWalkcycle = AnimationNode::create(image, 1, 12, 12);
	_willWalkcycle->setScale(scale);
	_willWalkcycle->setFrame(0);
	setSceneNode(_willWalkcycle);
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

	// Sensor dimensions
	b2Vec2 corners[4];
	corners[0].x = -PINEAPPLE_SSHRINK*getWidth() / 2.0f;
	corners[0].y = (-getHeight() + SENSOR_HEIGHT) / 2.0f;
	corners[1].x = -PINEAPPLE_SSHRINK*getWidth() / 2.0f;
	corners[1].y = (-getHeight() - SENSOR_HEIGHT) / 2.0f;
	corners[2].x = PINEAPPLE_SSHRINK*getWidth() / 2.0f;
	corners[2].y = (-getHeight() - SENSOR_HEIGHT) / 2.0f;
	corners[3].x = PINEAPPLE_SSHRINK*getWidth() / 2.0f;
	corners[3].y = (-getHeight() + SENSOR_HEIGHT) / 2.0f;

	b2PolygonShape sensorShape;
	sensorShape.Set(corners, 4);

	sensorDef.shape = &sensorShape;
	_sensorFixture = _body->CreateFixture(&sensorDef);
	_sensorFixture->SetUserData(getSensorName());

	// Override mass based on shape sizes to custom values
	b2MassData massData = b2MassData();
	float mass = this->_isLarge ? PINEAPPLE_GROWN_MASS : PINEAPPLE_NORMAL_MASS;
	mass = this->_isSmall ? PINEAPPLE_SHRUNK_MASS : mass;
	massData.mass = mass;
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
	if (getMovement() == 0.0f) {
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
	else {
		// Only cooldown while grounded
		_jumpCooldown = (_jumpCooldown > 0 ? _jumpCooldown - 1 : 0);
	}

	CapsuleObstacle::update(dt);
}

/**
* Animate Will if he's moving
*/
void PineappleModel::animate() {
	if (getVX() > 0.5f) {
		_willWalkcycleFrame++;
		_willWalkcycle->setFrame(_willWalkcycleFrame % 12);
	}
	else if (getVX() < -0.5f) {
		_willWalkcycleFrame += 11;
		_willWalkcycle->setFrame(_willWalkcycleFrame % 12);
	}
	else {
		_willWalkcycleFrame = 0;
		_willWalkcycle->setFrame(_willWalkcycleFrame);
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
        pnode->setScale(cscale * PINEAPPLE_SCALE);
        
        setDimension(pnode->getContentSize().width * PINEAPPLE_SCALE / _drawScale.x,
                     pnode->getContentSize().height * PINEAPPLE_SCALE / _drawScale.y);
        
        _normalSize = getDimension();
        
        SceneManager* assets =  AssetManager::getInstance()->getCurrent();
        Texture2D* image = assets->get<Texture2D>(PINEAPPLE_TEXTURE);
        initAnimation(image, cscale * PINEAPPLE_SCALE);
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

	_sensorNode = WireNode::createWithPoly(poly);
	_sensorNode->setColor(DEBUG_COLOR);
	_sensorNode->setPosition(Vec2(_debug->getContentSize().width / 2.0f, 0.0f));
	_debug->addChild(_sensorNode);
}



