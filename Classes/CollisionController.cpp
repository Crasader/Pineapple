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

void CollisionController::ground(PineappleModel* will, b2Fixture* fix, BoxObstacle *ground) {
    if (isBelowChar(ground, will)) {
        cout << "GROUNDED\n";
        will->setGrounded(true);
        _pSensorFixtures.emplace(fix);
    }
}

/**
* Applies the jello force to the given pinepple.
* This method is called when the given pineapple collides with a jello
* to trigger upward momentum, and a jello quiver animation
*/
void CollisionController::handleJelloCollision(PineappleModel* will, JelloModel* jello) {
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
			if (isBelowChar(jello, will)) {
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
		kid->setVX(KID_WALKSPEED + 4);
		kid->setCollidingWithJello(true);
	}
}

void CollisionController::handleBlenderCollision(PineappleModel* will) {
	_level->blendAndKill(will);
	//_pSensorFixtures.clear();
}

void CollisionController::handleBlenderCollision(KidModel* kid) {
	_level->blendAndKill(kid);
}

void CollisionController::handleSpikeCollision(PineappleModel* will) {
	_level->spikeAndKill(will);
	//_pSensorFixtures.clear();
}

void CollisionController::handleSpikeCollision(KidModel* kid) {
	_level->spikeAndKill(kid);
}

void CollisionController::handleCupCollision(PineappleModel* will, CrushableModel* cup) {
	if (isBelowChar(cup, will)) {
		_level->removeObstacle(cup);
	}
}

void CollisionController::handleButtonSwitchStartCollision(PineappleModel* will, ButtonSwitchModel* buttonSwitch) {
    if (! will->isCollidingWithButtonSwitch()) {
        buttonSwitch->handleContact();
        will->setCollidingWithButtonSwitch(true);
    }
}

void CollisionController::handleButtonSwitchEndCollision(PineappleModel* will, ButtonSwitchModel* buttonSwitch) {
    if (will->isCollidingWithButtonSwitch()) {
        buttonSwitch->handleEndContact();
        will->setCollidingWithButtonSwitch(false);
    }
}

/**
* helper to determine if a given Obstacle is below a given character
*/
bool CollisionController::isBelowChar(BoxObstacle* obj, CapsuleObstacle* character) {
	float e = 0.01f;
	float objTop = obj->getY() + (obj->getHeight() / 2);
    cout << "OBJECT TOP: " << objTop << "\n";
    cout << "OBJECT getY: " << obj->getY() << "\n";
	float charBot = character->getY() - (character->getHeight() / 2);
    cout << "charBot: " << charBot << "\n";
    cout << "char getY: " << character->getY() << "\n";
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

	Obstacle* bd1 = (Obstacle*)body1->GetUserData();
	Obstacle* bd2 = (Obstacle*)body2->GetUserData();

	// WILL COLLISIONS
	if (bd1->getCollisionClass() == PINEAPPLE_C || bd2->getCollisionClass() == PINEAPPLE_C) {
		PineappleModel* will = bd1->getCollisionClass() == PINEAPPLE_C ? (PineappleModel*)bd1 : (PineappleModel*)bd2;
		// Will  x Ground
		if (bd1->getCollisionClass() % 2 == 0 || bd2->getCollisionClass() % 2 == 0) {
            ground(will, bd1->getCollisionClass() == PINEAPPLE_C ? fix2 : fix1, will == bd1 ? (BoxObstacle*)bd2 : (BoxObstacle*)bd1);
		}
		// Will x Jello
		if (bd1->getCollisionClass() == JELLO_C || bd2->getCollisionClass() == JELLO_C) {
			handleJelloCollision(will, bd1->getCollisionClass() == JELLO_C ? (JelloModel*)bd1 : (JelloModel*)bd2);
		}
		// Will x Cup
		if (will->isLarge() && (bd1->getCollisionClass() == CUP_C || bd2->getCollisionClass() == CUP_C)) {
			CrushableModel* cup = bd1->getCollisionClass() == CUP_C ? (CrushableModel*)bd1 : (CrushableModel*)bd2;
			handleCupCollision(will, cup);
		}
		// Will x Goal
		if (_level->getGoal() != nullptr && (bd1 == _level->getGoal() || bd2 == _level->getGoal())) {
			will->setReachedGoal(true);
		}
		// Will x Blender
		if (bd1->getCollisionClass() == BLENDER_C || bd2->getCollisionClass() == BLENDER_C) {
			handleBlenderCollision(will);
		}
		// Will x Spikes
		if (bd1->getCollisionClass() == SPIKES_C || bd2->getCollisionClass() == SPIKES_C) {
			handleSpikeCollision(will);
		}
        // Will x ButtonSwitch
        if (bd1->getCollisionClass() == BUTTON_SWITCH_C || bd2->getCollisionClass() == BUTTON_SWITCH_C) {
            ButtonSwitchModel* buttonSwitch = bd1->getCollisionClass() == BUTTON_SWITCH_C ? (ButtonSwitchModel*)bd1 : (ButtonSwitchModel*)bd2;
            handleButtonSwitchStartCollision(will, buttonSwitch);
        }
        
	} // END WILL COLLISIONS

	// KID COLLISIONS
	else if (bd1->getCollisionClass() == KID_C || bd2->getCollisionClass() == KID_C) {
		KidModel* kid = bd1->getCollisionClass() == KID_C ? (KidModel*)bd1 : (KidModel*)bd2;
		// Kid x Jello
		if (bd1->getCollisionClass() == JELLO_C || bd2->getCollisionClass() == JELLO_C) {
			handleJelloCollision(kid);
		}
		// Kid x Goal
		if (_level->getGoal() != nullptr && (bd1 == _level->getGoal() || bd2 == _level->getGoal())) {
			kid->setReachedGoal(true);
		}
		// Kid x Blender
		if (bd1->getCollisionClass() == BLENDER_C || bd2->getCollisionClass() == BLENDER_C) {
			handleBlenderCollision(kid);
		}
		// Kid x Spikes
		if (bd1->getCollisionClass() == SPIKES_C || bd2->getCollisionClass() == SPIKES_C) {
			handleSpikeCollision(kid);
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

	Obstacle* bd1 = (Obstacle*)body1->GetUserData();
	Obstacle* bd2 = (Obstacle*)body2->GetUserData();
	
	// WILL COLLISIONS
	if (bd1->getCollisionClass() == PINEAPPLE_C || bd2->getCollisionClass() == PINEAPPLE_C) {
		PineappleModel* will = bd1->getCollisionClass() == PINEAPPLE_C ? (PineappleModel*)bd1 : (PineappleModel*)bd2;
		// Will x Ground
		if (bd1->getCollisionClass() % 2 == 0 || bd2->getCollisionClass() % 2 == 0) {
			_pSensorFixtures.erase(bd1 == will ? fix2 : fix1);
			if (_pSensorFixtures.empty()) {
				will->setGrounded(false);
			}
		}
		// Will x Jello
		if (bd1->getCollisionClass() == JELLO_C || bd2->getCollisionClass() == JELLO_C) {
			will->setCollidingWithJello(false);
		}
		// Will x Goal
		if (_level->getGoal() != nullptr && (bd1 == _level->getGoal() || bd2 == _level->getGoal())) {
			will->setReachedGoal(false);
		}
        // Will x ButtonSwitch
        if (bd1->getCollisionClass() == BUTTON_SWITCH_C || bd2->getCollisionClass() == BUTTON_SWITCH_C) {
            ButtonSwitchModel* buttonSwitch = bd1->getCollisionClass() == BUTTON_SWITCH_C ? (ButtonSwitchModel*)bd1 : (ButtonSwitchModel*)bd2;
            handleButtonSwitchEndCollision(will, buttonSwitch);
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
}
