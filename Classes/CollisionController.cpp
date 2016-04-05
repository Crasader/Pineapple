#include "CollisionController.h"
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/b2Collision.h>
#include "PineappleModel.h"
#include "BlenderModel.h"
#include "KidModel.h"
#include "JelloModel.h"
#include "SpikeModel.h"
#include "CrushableModel.h"
#include "LevelModel.h"
#include "ButtonSwitchModel.h"

#pragma mark -
#pragma mark Initialization

CollisionController::CollisionController() :
	_level(nullptr) {}
	//_levelCtrlr(nullptr){}

CollisionController* CollisionController::create() {
	return new (std::nothrow) CollisionController();
}

void CollisionController::setLevel(LevelModel* level) {
	_level = level;
}

#pragma mark -
#pragma mark Collision Handling
/**
* Applies the jello force to the given pinepple.
* This method is called when the given pineapple collides with a jello
* to trigger upward momentum, and a jello quiver animation
*/
void CollisionController::handleJelloCollision(PineappleModel* will) {
	will->setCollidingWithJello(true);
	if (!will->isLarge()) {
		//Jump!
		b2Body* body = will->getBody();
		will->setVY(0);
		body->ApplyLinearImpulse(b2Vec2(0, JELLO_BOUNCE_FORCE), body->GetPosition(), true);
		will->setJumping(true);
		will->setGrounded(false);
	}
	else {
		//Squish
	}
}

/**
* Applies the jello force to the given kid.
* This method is called when a kid collides with a jello
* to trigger upward momentum, and a jello quiver animation
*/
void CollisionController::handleJelloCollision(KidModel* kid) {
	//Jump!
	kid->setVY(10);
	kid->setVX(KID_WALKSPEED + 2);
	kid->setGrounded(false);
	kid->setCollidingWithJello(true);
}

void CollisionController::handleSpikeCollision(SimpleObstacle* PineappleOrKid) {
	_level->removeObstacle(PineappleOrKid);

	//TODO: animation and sounds
}

/**
* helper to determine if a given Obstacle is below a given character
*/
bool isBelowChar(BoxObstacle* obj, CapsuleObstacle* character) {
	float e = 0.01f;
	float objTop = obj->getY() + (obj->getHeight() / 2);
	float charBot = character->getY() - (character->getHeight() / 2);
	return charBot + e >= objTop;
}

/**
* Processes the start of a collision
*
* This method is called when we first get a collision between two objects.  We use
* this method to test if it is the "right" kind of collision.  In particular, we
* use it to test if we make it to the win door.
*
* @param  contact  The two bodies that collided
*/
void CollisionController::beginContact(b2Contact* contact) {
	b2Fixture* fix1 = contact->GetFixtureA();
	b2Fixture* fix2 = contact->GetFixtureB();

	b2Body* body1 = fix1->GetBody();
	b2Body* body2 = fix2->GetBody();

	void* fd1 = fix1->GetUserData();
	void* fd2 = fix2->GetUserData();

	Obstacle* bd1 = (Obstacle*)body1->GetUserData();
	Obstacle* bd2 = (Obstacle*)body2->GetUserData();
    
    // BUTTON and SWITCH
    if ((bd1 == _level->getPineapple() && bd2->getName() == BUTTON_SWITCH_NAME) ||
        (bd2 == _level->getPineapple() && bd1->getName() == BUTTON_SWITCH_NAME)) {
        ButtonSwitchModel* button_switch;
        if (bd1 == _level->getPineapple()) {
            button_switch = (ButtonSwitchModel*) bd2;
        } else {
            button_switch = (ButtonSwitchModel*) bd1;
        }
        button_switch->handleContact();
    }

	// See if we have landed on the ground.
	// TODO this is super shitty.  we should make sure bd1/bd2 is a platform rather than not a kid
	if (_level->getPineapple() != nullptr && !_level->getPineapple()->isCollidingWithJello() &&
		((_level->getPineapple()->getSensorName() == fd2 && _level->getPineapple() != bd1 && bd1->getName() != KID_NAME) ||
			(_level->getPineapple()->getSensorName() == fd1 && _level->getPineapple() != bd2 && bd2->getName() != KID_NAME))) {
		_level->getPineapple()->setGrounded(true);
		// Could have more than one ground
		_sensorFixtures.emplace(_level->getPineapple() == bd1 ? fix2 : fix1);
	}

	if (bd1->getName() == JELLO_NAME || bd2->getName() == JELLO_NAME) {
		if (_level->getPineapple() != nullptr && !_level->getPineapple()->isCollidingWithJello() && (_level->getPineapple() == bd1 || _level->getPineapple() == bd2)) {
			handleJelloCollision(_level->getPineapple());
		}
		else {
			for (int i = 0; i < KID_COUNT; i++) {
				if (_level->getKid(i) != nullptr && !_level->getKid(i)->isCollidingWithJello() &&
					(_level->getKid(i) == bd1 || _level->getKid(i) == bd2)) {
					handleJelloCollision(_level->getKid(i));
				}
			}
		}
	}

	// See if a kid has landed on the ground.
	for (int i = 0; i < KID_COUNT; i++) {
		if (_level->getKid(i) != nullptr && !_level->getKid(i)->isCollidingWithJello() &&
			((_level->getKid(i)->getSensorName() == fd2 && _level->getKid(i) != bd1) ||
				(_level->getKid(i)->getSensorName() == fd1 && _level->getKid(i) != bd2))) {
			_level->getKid(i)->setGrounded(true);
			// Could have more than one ground
			_sensorFixtures.emplace(_level->getKid(i) == bd1 ? fix2 : fix1);
		}
	}

	// If a kid hits the "win" door, update the kid's status
	for (int i = 0; i < KID_COUNT; i++) {
		if ((bd1 == _level->getKid(i) && bd2 == _level->getGoal()) ||
			(bd1 == _level->getGoal() && bd2 == _level->getKid(i))) {
			_level->getKid(i)->setReachedGoal(true);
		}
	}

	// If Will hits the "win" door, update the Will's status
	if ((_level->getPineapple() == bd1 && bd2 == _level->getGoal()) || (_level->getPineapple() == bd2 && bd1 == _level->getGoal())) {
		_level->getPineapple()->setReachedGoal(true);
	}

	//See if we have collided with the blender
	if ((bd1 == _level->getPineapple() && bd2 == (Obstacle*)_level->getBlender()) || (bd2 == _level->getPineapple() && bd1 == (Obstacle*)_level->getBlender())) {
		_level->blendAndKill(_level->getPineapple());
		_level->clearPineapple();
		_level->setFailure(true);
	}

	//See if a kid has collided with the blender
	for (int i = 0; i < KID_COUNT; i++) {
		if ((bd1 == _level->getKid(i) && bd2 == (Obstacle*)_level->getBlender()) || (bd2 == _level->getKid(i) && bd1 == (Obstacle*)_level->getBlender())) {
			_level->blendAndKill(_level->getKid(i));
			_level->clearKid(i);
			_level->subtractKidFromCount();
			if (_level->numKidsRemaining() == 0) {
				_level->setFailure(true);
			}
		}
	}

	//See if we have collided with a spike!
	if ((bd1 == _level->getPineapple() && bd2->getName() == SPIKE_NAME) ||
		(bd2 == _level->getPineapple() && bd1->getName() == SPIKE_NAME)) {
		handleSpikeCollision(_level->getPineapple());
		_level->clearPineapple();
		_level->setFailure(true);
	}

	//See if a kid has collided with a spike
	for (int i = 0; i < KID_COUNT; i++) {
		if ((bd1 == _level->getKid(i) && bd2->getName() == SPIKE_NAME) ||
			(bd2 == _level->getKid(i) && bd1->getName() == SPIKE_NAME)) {
			handleSpikeCollision(_level->getKid(i));
			_level->clearKid(i);
			_level->subtractKidFromCount();
			if (_level->numKidsRemaining() == 0) {
				_level->setFailure(true);
			}
		}
	}

	//CRUSHHHHHHHHHHHHHHHHHHH TODO: SO BAD
	if (_level->getPineapple() != nullptr && _level->getPineapple()->isLarge()) {
		Obstacle* cup = nullptr;
		if (bd1 == _level->getPineapple() && bd2->getName() == CUP_NAME) {
			cup = bd2;
		}
		else if (bd1->getName() == CUP_NAME && bd2 == _level->getPineapple()) {
			cup = bd1;
		}
		if (cup != nullptr) {
			if (isBelowChar((BoxObstacle*)cup, _level->getPineapple())) {
				_level->removeObstacle(cup);
			}
		}
	}
}

/**
* Callback method for the start of a collision
*
* This method is called when two objects cease to touch.  The main use of this method
* is to determine when the characer is NOT on the ground.  This is how we prevent
* double jumping.
*/
void CollisionController::endContact(b2Contact* contact) {
	b2Fixture* fix1 = contact->GetFixtureA();
	b2Fixture* fix2 = contact->GetFixtureB();

	b2Body* body1 = fix1->GetBody();
	b2Body* body2 = fix2->GetBody();

	void* fd1 = fix1->GetUserData();
	void* fd2 = fix2->GetUserData();

	Obstacle* bd1 = (Obstacle*)body1->GetUserData();
	Obstacle* bd2 = (Obstacle*)body2->GetUserData();

    // BUTTON / SWITCH
    
    if ((bd1 == _level->getPineapple() && bd2->getName() == BUTTON_SWITCH_NAME) ||
        (bd2 == _level->getPineapple() && bd1->getName() == BUTTON_SWITCH_NAME)) {
        ButtonSwitchModel* button_switch;
        if (bd1 == _level->getPineapple()) {
            button_switch = (ButtonSwitchModel*) bd2;
        } else {
            button_switch = (ButtonSwitchModel*) bd1;
        }
        
        button_switch->handleEndContact();
    }

	if ((_level->getPineapple()->getSensorName() == fd2 && _level->getPineapple() != bd1) ||
		(_level->getPineapple()->getSensorName() == fd1 && _level->getPineapple() != bd2)) {
		_sensorFixtures.erase(_level->getPineapple() == bd1 ? fix2 : fix1);
		if (_sensorFixtures.empty()) {
			_level->getPineapple()->setGrounded(false);
		}
	}

	//See if you or a kid has stopped colliding with jello
	if (bd1->getName() == JELLO_NAME || bd2->getName() == JELLO_NAME) {
		if (_level->getPineapple() == bd1 || _level->getPineapple() == bd2) {
			_level->getPineapple()->setCollidingWithJello(false);
		}
		else {
			for (int i = 0; i < KID_COUNT; i++) {
				if (_level->getKid(i) == bd1 || _level->getKid(i) == bd2) {
					_level->getKid(i)->setCollidingWithJello(false);
				}
			}
		}
	}

	// See if a kid has left the ground.
	for (int i = 0; i < KID_COUNT; i++) {
		if ((_level->getKid(i)->getSensorName() == fd2 && _level->getKid(i) != bd1) ||
			(_level->getKid(i)->getSensorName() == fd1 && _level->getKid(i) != bd2)) {
			_sensorFixtures.erase(_level->getKid(i) == bd1 ? fix2 : fix1);
			if (_sensorFixtures.empty()) {
				_level->getKid(i)->setGrounded(false);
			}
		}
	}

	// If Will leaves the "win" door, update the Will's status
	if ((_level->getPineapple() == bd1 && bd2 == _level->getGoal()) || (_level->getPineapple() == bd2 && bd1 == _level->getGoal())) {
		_level->getPineapple()->setReachedGoal(false);
	}
}
