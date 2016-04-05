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

void CollisionController::ground(PineappleModel* will, b2Fixture* fix) {
	will->setGrounded(true);
	_sensorFixtures.emplace(fix);
}

void CollisionController::ground(KidModel* kid, b2Fixture* fix) {
	kid->setGrounded(true);
	_sensorFixtures.emplace(fix);
}

/**
* Applies the jello force to the given pinepple.
* This method is called when the given pineapple collides with a jello
* to trigger upward momentum, and a jello quiver animation
*/
void CollisionController::handleJelloCollision(JelloModel* jello) {
	PineappleModel* will = _level->getPineapple();
	if (!will->isCollidingWithJello()) {
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
			if (isBelowChar(jello, _level->getPineapple())) {
				_level->removeObstacle(jello);
			}
		}
	}
}

/**
* Applies the jello force to the given kid.
* This method is called when a kid collides with a jello
* to trigger upward momentum, and a jello quiver animation
*/
void CollisionController::handleJelloCollision(KidModel* kid) {
	//Jump!
	if (!kid->isCollidingWithJello()) {
		kid->setVY(10);
		kid->setVX(KID_WALKSPEED + 2);
		kid->setGrounded(false);
		kid->setCollidingWithJello(true);
	}
}

void CollisionController::handleBlenderCollision(PineappleModel* will) {
	_level->blendAndKill(will);
}

void CollisionController::handleBlenderCollision(KidModel* kid) {
	_level->blendAndKill(kid);
}

void CollisionController::handleSpikeCollision(PineappleModel* will) {
	_level->spikeAndKill(will);
}

void CollisionController::handleSpikeCollision(KidModel* kid) {
	_level->spikeAndKill(kid);
}

void CollisionController::handleCupCollision(CrushableModel* cup) {
	if (isBelowChar(cup, _level->getPineapple())) {
		_level->removeObstacle(cup);
	}
}

/**
* helper to determine if a given Obstacle is below a given character
*/
bool CollisionController::isBelowChar(BoxObstacle* obj, CapsuleObstacle* character) {
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

	// WILL COLLISIONS
	if (bd1->getCollisionClass() == PINEAPPLE_C || bd2->getCollisionClass() == PINEAPPLE_C) {
		PineappleModel* will = _level->getPineapple();
		// Will  x Ground
		if (bd1->getCollisionClass() == UNASSIGNED_C || bd2->getCollisionClass() == UNASSIGNED_C) {
			ground(will, bd1->getCollisionClass() == PINEAPPLE_C ? fix2 : fix1);
		}
		// Will x Jello
		if (bd1->getCollisionClass() == JELLO_C || bd2->getCollisionClass() == JELLO_C) {
			handleJelloCollision(bd1->getCollisionClass() == JELLO_C ? (JelloModel*)bd1 : (JelloModel*)bd2);
		}
		// Will x Cup
		if (will->isLarge() && (bd1->getCollisionClass() == CUP_C || bd2->getCollisionClass() == CUP_C)) {
			CrushableModel* cup = bd1->getCollisionClass() == CUP_C ? (CrushableModel*)bd1 : (CrushableModel*)bd2;
			handleCupCollision(cup);
		}
		// Will x Goal
		if (bd1 == _level->getGoal() || bd2 == _level->getGoal()) {
			will->setReachedGoal(true);
		}
		// Will x Blender
		if (bd1->getCollisionClass() == BLENDER_C || bd2->getCollisionClass() == BLENDER_C) {
			handleBlenderCollision(will);
		}
	} // END WILL COLLISIONS

	// KID COLLISIONS
	else if (bd1->getCollisionClass() == KID_C || bd2->getCollisionClass() == KID_C) {
		KidModel* kid = bd1->getCollisionClass() == KID_C ? (KidModel*)bd1 : (KidModel*)bd2;
		// Will  x Ground
		if (bd1->getCollisionClass() == UNASSIGNED_C || bd2->getCollisionClass() == UNASSIGNED_C) {
			ground(kid, bd1->getCollisionClass() == KID_C ? fix2 : fix1);
		}
		// Kid x Jello
		if (bd1->getCollisionClass() == JELLO_C || bd2->getCollisionClass() == JELLO_C) {
			handleJelloCollision(kid);
		}
		// Kid x Goal
		if (bd1 == _level->getGoal() || bd2 == _level->getGoal()) {
			kid->setReachedGoal(true);
		}
		// Kid x Blender
		if (bd1->getCollisionClass() == BLENDER_C || bd2->getCollisionClass() == BLENDER_C) {
			handleBlenderCollision(kid);
		}
	} // END KID COLLISIONS
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

	// WILL COLLISIONS
	if (bd1->getCollisionClass() == PINEAPPLE_C || bd2->getCollisionClass() == PINEAPPLE_C) {
		PineappleModel* will = _level->getPineapple();
		// Will x Ground
		if (bd1->getCollisionClass() == UNASSIGNED_C || bd2->getCollisionClass() == UNASSIGNED_C) {
			_sensorFixtures.erase(bd1 == will ? fix2 : fix1);
			if (_sensorFixtures.empty()) {
				will->setGrounded(false);
			}
		}
		// Will x Jello
		if (bd1->getCollisionClass() == JELLO_C || bd2->getCollisionClass() == JELLO_C) {
			will->setCollidingWithJello(false);
		}
		// Will x Goal
		if (bd1 == _level->getGoal() || bd2 == _level->getGoal()) {
			will->setReachedGoal(false);
		}
	} // END WILL COLLISIONS

	// KID COLLISIONS
	else if (bd1->getCollisionClass() == KID_C || bd2->getCollisionClass() == KID_C) {
		KidModel* kid = bd1->getCollisionClass() == KID_C ? (KidModel*)bd1 : (KidModel*)bd2;
		// Kid x Jello
		if (bd1->getCollisionClass() == JELLO_C || bd2->getCollisionClass() == JELLO_C) {
			kid->setCollidingWithJello(false);
		}
	} // END KID COLLISIONS


	//if ((_level->getPineapple()->getSensorName() == fd2 && _level->getPineapple() != bd1) ||
	//	(_level->getPineapple()->getSensorName() == fd1 && _level->getPineapple() != bd2)) {
	//	_sensorFixtures.erase(_level->getPineapple() == bd1 ? fix2 : fix1);
	//	if (_sensorFixtures.empty()) {
	//		_level->getPineapple()->setGrounded(false);
	//	}
	//}

	//// See if a kid has left the ground.
	//for (int i = 0; i < KID_COUNT; i++) {
	//	if ((_level->getKid(i)->getSensorName() == fd2 && _level->getKid(i) != bd1) ||
	//		(_level->getKid(i)->getSensorName() == fd1 && _level->getKid(i) != bd2)) {
	//		_sensorFixtures.erase(_level->getKid(i) == bd1 ? fix2 : fix1);
	//		if (_sensorFixtures.empty()) {
	//			_level->getKid(i)->setGrounded(false);
	//		}
	//	}
	//}
}
