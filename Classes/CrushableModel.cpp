#include "CrushableModel.h"
#include <cornell/CUPolygonNode.h>
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>


#pragma mark -
#pragma mark Physics Constants
	/** The amount to shrink the body fixture (vertically) relative to the image */
#define CRUSHABLE_VSHRINK  0.3f
	/** The amount to shrink the body fixture (horizontally) relative to the image */
#define CRUSHABLE_HSHRINK  0.3f

#pragma mark -
#pragma mark Static Constructors
	/**
	* Creates a new crushable at the origin.
	*
	* The crushable is scaled so that 1 pixel = 1 Box2d unit
	*
	* The scene graph is completely decoupled from the physics system.
	* The node does not have to be the same size as the physics body. We
	* only guarantee that the scene graph node is positioned correctly
	* according to the drawing scale.
	*
	* @return  An retained physics object
	*/
	CrushableModel* CrushableModel::create(const char* texture) {
	CrushableModel* crushable = new (std::nothrow) CrushableModel();
	if (crushable && crushable->init(texture)) {
		crushable->retain();
		return crushable;
	}
	CC_SAFE_DELETE(crushable);
	return nullptr;
}

/**
* Creates a new crushable at the given position.
*
* The crushable is scaled so that 1 pixel = 1 Box2d unit
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
CrushableModel* CrushableModel::create(const char* texture, int x, int y, const Vec2& pos) {
	CrushableModel* crushable = new (std::nothrow) CrushableModel();
	if (crushable && crushable->init(texture, x, y, pos)) {
		crushable->retain();
		return crushable;
	}
	CC_SAFE_DELETE(crushable);
	return nullptr;
}

/**
* Creates a new crushable at the given position.
*
* The crushable is sized according to the given drawing scale.
*
* The scene graph is completely decoupled from the physics system.
* The node does not have to be the same size as the physics body. We
* only guarantee that the scene graph node is positioned correctly
* according to the drawing scale.
*
* @param  pos      Initial position in world coordinates
* @param  scale    The drawing scale
*
* @return  An retain physics object
*/
CrushableModel* CrushableModel::create(const char* texture, int x, int y, const Vec2& pos, const Vec2& scale) {
	CrushableModel* crushable = new (std::nothrow) CrushableModel();
	if (crushable && crushable->init(texture, x, y, pos, scale)) {
		crushable->retain();
		return crushable;
	}
	CC_SAFE_DELETE(crushable);
	return nullptr;
}

#pragma mark -
#pragma mark Initializers

/**
* Initializes a new crushable at the given position.
*
* The crushable is sized according to the given drawing scale.
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
bool CrushableModel::init(const char* texture, int x, int y, const Vec2& pos, const Vec2& scale) {
	if (BoxObstacle::init(pos, Size(scale))) {
        _movedDown = false;
        _textureName = texture;
        _tiledYCoord = y;
        _tiledXCoord = x;
		_smashCycleFrame = 0.0f;
		_smashing = false;
		_smashed = false;
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
void CrushableModel::createFixtures() {
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
void CrushableModel::releaseFixtures() {
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
void CrushableModel::update(float dt) {
	BoxObstacle::update(dt);
}

/**
* Animate the cup  if it's being smashed
*/
void CrushableModel::animate() {
	if (_smashing) {
		_smashCycleFrame += 0.5f;
		int tmp = (int)rint(_smashCycleFrame);
		if (tmp < CUP_SMASH_FRAMES) {
			_smashCycle->setFrame(tmp);
		}
		else {
			_smashed = true;
		}
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
void CrushableModel::resetSceneNode() {
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
        pnode->setScale(cscale * CRUSHABLE_SCALE);
		pnode->setFrame(0);
        
        setDimension(pnode->getContentSize().width * CRUSHABLE_HSHRINK / _drawScale.x,
                     pnode->getContentSize().height * CRUSHABLE_VSHRINK / _drawScale.y);
        
        pnode->setFrame(0);

		_smashCycleFrame = 0.0f;
		_smashCycle = pnode;
    }
}

/**
* Redraws the outline of the physics fixtures to the debug node
*
* The debug node is use to outline the fixtures attached to this object.
* This is very useful when the fixtures have a very different shape than
* the texture (e.g. a circular shape attached to a square texture).
*/
void CrushableModel::resetDebugNode() {
	BoxObstacle::resetDebugNode();
}