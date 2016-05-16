//
//  GoalModel.cpp
//

#include "GoalModel.h"
#include <cornell/CUPolygonNode.h>
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>

#pragma mark -
#pragma mark Physics Constants

#define GOAL_SCALE		1.5f
#define GOAL_H_SHRINK   0.6f

#pragma mark -
#pragma mark Static Constructors
/**
 * Creates a new goal at the origin.
 *
 * The goal is scaled so that 1 pixel = 1 Box2d unit
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @return  An retained physics object
 */
GoalModel* GoalModel::create() {
    GoalModel* goal = new (std::nothrow) GoalModel();
    if (goal && goal->init()) {
        goal->retain();
        return goal;
    }
    CC_SAFE_DELETE(goal);
    return nullptr;
}

/**
 * Creates a new goal at the given position.
 *
 * The goal is scaled so that 1 pixel = 1 Box2d unit
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
GoalModel* GoalModel::create(const Vec2& pos) {
    GoalModel* goal = new (std::nothrow) GoalModel();
    if (goal && goal->init(pos)) {
        goal->setFriction(0.0f);      // HE WILL STICK TO WALLS IF YOU FORGET
        goal->setFixedRotation(true); // OTHERWISE, HE IS A WEEBLE WOBBLE
        goal->retain();
        return goal;
    }
    CC_SAFE_DELETE(goal);
    return nullptr;
}

/**
 * Creates a new goal at the given position.
 *
 * The goal is sized according to the given drawing scale.
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
GoalModel* GoalModel::create(const Vec2& pos, const Vec2& scale) {
    GoalModel* goal = new (std::nothrow) GoalModel();
    if (goal && goal->init(pos,scale)) {
        goal->retain();
        return goal;
    }
    CC_SAFE_DELETE(goal);
    return nullptr;
}


#pragma mark -
#pragma mark Initializers

/**
 * Initializes a new goal at the given position.
 *
 * The goal is sized according to the given drawing scale.
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
bool GoalModel::init(const Vec2& pos, const Vec2& scale) {
    if (BoxObstacle::init(pos, Size(scale))) {
		_fridgeCycleFrame = 0.0f;
		_closed = false;
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
void GoalModel::createFixtures() {
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
void GoalModel::releaseFixtures() {
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
void GoalModel::update(float dt) {
    BoxObstacle::update(dt);
}

/**
 * Animate the open fridge door
 */
void GoalModel::animate() {
	if (!_closed) {
		_fridgeCycleFrame += 0.1f;
		int tmp = (int)rint(_fridgeCycleFrame);
		_fridgeCycle->setFrame(tmp % (GOAL_FRAME_COUNT - 1));
	}
	else {
		_fridgeCycle->setFrame(GOAL_FRAME_COUNT - 1);
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
void GoalModel::resetSceneNode() {
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
        pnode->setScale(cscale * GOAL_SCALE);
		pnode->setFrame(0);
        pnode->setAnchorPoint(Vec2(0.65,0.5));

		setDimension(pnode->getContentSize().width * GOAL_SCALE * GOAL_H_SHRINK / _drawScale.x,
                     pnode->getContentSize().height * GOAL_SCALE / _drawScale.y);	

		_fridgeCycleFrame = 0.0f;
		_fridgeCycle = pnode;
    }
}

/**
 * Redraws the outline of the physics fixtures to the debug node
 *
 * The debug node is use to outline the fixtures attached to this object.
 * This is very useful when the fixtures have a very different shape than
 * the texture (e.g. a circular shape attached to a square texture).
 */
void GoalModel::resetDebugNode() {
    BoxObstacle::resetDebugNode();
}
