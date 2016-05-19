#include "CollisionController.h"
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/b2Collision.h>
#include "platform/CCDevice.h"
#include "PineappleModel.h"
#include "BlenderModel.h"
#include "KidModel.h"
#include "JelloModel.h"
#include "SpikeModel.h"
#include "CrushableModel.h"
#include "LevelModel.h"
#include "ButtonSwitchModel.h"
#include <cornell/CUSoundEngine.h>
#include "Sounds.h"

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
#pragma mark Collision Sound Playing

void playSoundEffect(char* key, float volume, bool force) {
	Sound* source = AssetManager::getInstance()->getCurrent()->get<Sound>(key);
	SoundEngine::getInstance()->playEffect(key, source, false, volume, force);
}

void playSoundEffect(char* key, float volume) {
	playSoundEffect(key, volume, false);
}

void playSoundEffect(char* key, bool force) {
	playSoundEffect(key, EFFECT_VOLUME, force);
}

void playSoundEffect(char* key) {
	playSoundEffect(key, false);
}

#pragma mark -
#pragma mark Collision Handling

void CollisionController::ground(PineappleModel* will, b2Fixture* fix, BoxObstacle *ground) {
    will->setGrounded(true);
    _pSensorFixtures.emplace(fix);
}

void playKidScream(KidModel* kid) {
	char* key;
	int i = kid->getIndex();
	switch (i) {
	case 0: key = PINEAPPLET1_DEATH_SOUND;
		break;
	case 1: key = PINEAPPLET2_DEATH_SOUND;
		break;
	case 2: key = PINEAPPLET3_DEATH_SOUND;
		break;
	case 3: key = PINEAPPLET4_DEATH_SOUND;
		break;
	default: key = "we gon crash if this happens, but it won't so it's chill.";
	}
	playSoundEffect(key);
}

/**
* Applies the jello force to the given pinepple.
* This method is called when the given pineapple collides with a jello
* to trigger upward momentum, and a jello quiver animation
*/
void CollisionController::handleJelloCollision(PineappleModel* will, JelloModel* jello) {
	if (!will->isCollidingWithJello()) {
		jello->setBouncing(true);
		will->setCollidingWithJello(true);
		if (will->isSmall()) {
			//Jump!
			b2Body* body = will->getBody();
			will->setVY(0);
			body->ApplyLinearImpulse(b2Vec2(0, JELLO_BOUNCE_FORCE), body->GetPosition(), true);
			will->setJumping(true);
			will->setGrounded(false);
			playSoundEffect(JELLO_BOING);
		}
	}
}

/**
* Applies the jello force to the given kid.
* This method is called when a kid collides with a jello
* to trigger upward momentum, and a jello quiver animation
*/
void CollisionController::handleJelloCollision(KidModel* kid, JelloModel* jello) {
	//Jump!
	if (!kid->isCollidingWithJello()) {
		jello->setBouncing(true);
		kid->setVY(10);
		kid->setVX(KID_WALKSPEED + 4);
		kid->setCollidingWithJello(true);
		playSoundEffect(JELLO_BOING, true);
	}
}

void CollisionController::handleBlenderCollision(PineappleModel* will) {
	Sound* source = AssetManager::getInstance()->getCurrent()->get<Sound>(WILL_DEATH_SOUND);
	SoundEngine::getInstance()->playEffect(WILL_DEATH_SOUND, source, false, EFFECT_VOLUME);
	will->setIsBlended(true);
    b2Filter b = will->getFilterData();
    b.maskBits = BLENDER_MASK;
    will->setFilterData(b);
}

void CollisionController::handleBlenderCollision(KidModel* kid) {
	playKidScream(kid);
	kid->setIsBlended(true);
    b2Filter b = kid->getFilterData();
    b.maskBits = BLENDER_MASK;
    kid->setFilterData(b);
}

void CollisionController::handleBlenderBladeCollision(PineappleModel* will) {
	_level->kill(will);
	playSoundEffect(SPLAT_SOUND);
	//_pSensorFixtures.clear();
}

void CollisionController::handleBlenderBladeCollision(KidModel* kid) {
	_level->kill(kid);
	playSoundEffect(SPLAT_SOUND);
}

void CollisionController::handleSpikeCollision(PineappleModel* will) {
	_level->spikeAndKill(will);
	playSoundEffect(WILL_DEATH_SOUND);
	//_pSensorFixtures.clear();
}

void CollisionController::handleSpikeCollision(KidModel* kid) {
	playKidScream(kid);
	//_level->spikeAndKill(kid);
	kid->setIsImpaled(true);
}

void CollisionController::handleCupCollision(PineappleModel* will, CrushableModel* cup) {
	playSoundEffect(CUP_CRUSH_SOUND, true);
	will->setVY(4);
	cup->setSmashing(true);
	/*if (isBelowChar(cup, will)) {
		_level->removeObstacle(cup);
	}*/
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
	float charBot = character->getY() - (character->getHeight() / 2);
	return charBot + e >= objTop;
}

void ground(KidModel* kid, b2Fixture* fix) {
    kid->setGrounded(true);
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

    // WILL FEET COLLISIONS
    PineappleModel* will = bd1->getCollisionClass() == PINEAPPLE_C ? (PineappleModel*)bd1 : (PineappleModel*)bd2;
    string* feetSensorName = _level->getPineapple()->getSensorName();
    if (fix2->GetUserData() == feetSensorName || fix1->GetUserData() == feetSensorName) {
        // with jello
        if (bd1->getCollisionClass() == JELLO_C || bd2->getCollisionClass() == JELLO_C) {
            // if will is large and traveling downwards, squish
            if (!will->isSmall() && will->getVY() < MAX_V_TO_CRUSH) {
                // set v to 0 because sometimes the sensor touches too early for the physics
                // engine to do it
                will->setVY(0);
				JelloModel* jello = bd1->getCollisionClass() == JELLO_C ? (JelloModel*)bd1 : (JelloModel*)bd2;
				jello->setSmushing(true);
				playSoundEffect(JELLO_SQUISH_SOUND, true);
            }
        }
        // with cup
        if (bd1->getCollisionClass() == CUP_C || bd2->getCollisionClass() == CUP_C) {
            // if will is large and traveling downwards, crush
            if (!will->isSmall() && will->getVY() < MAX_V_TO_CRUSH) {
								handleCupCollision(will, bd1->getCollisionClass() == CUP_C ? (CrushableModel*)bd1 : (CrushableModel*)bd2);
            }
        }
        // with ground
        if (bd1->getCollisionClass() % 2 == 0 || bd2->getCollisionClass() % 2 == 0) {
            ground(will, bd1->getCollisionClass() == PINEAPPLE_C ? fix2 : fix1, will == bd1 ? (BoxObstacle*)bd2 : (BoxObstacle*)bd1);
        }
    }
    // WILL BODY SENSOR COLLISIONS
    // (needed because collisions are on fixture level, will is made of 3 fixtures)
    else if (fix1->GetUserData() == _level->getPineapple()->getSensorBodyName() ||
             fix2->GetUserData() == _level->getPineapple()->getSensorBodyName()) {
        // Will x ButtonSwitch
        if (bd1->getCollisionClass() == BUTTON_SWITCH_C ||
            bd2->getCollisionClass() == BUTTON_SWITCH_C) {
            ButtonSwitchModel* buttonSwitch = bd1->getCollisionClass() == BUTTON_SWITCH_C ? (ButtonSwitchModel*)bd1 : (ButtonSwitchModel*)bd2;
            // only collided if will didn't JUST grow/shrink
            if (!will->justGrewShrank()) {
                handleButtonSwitchStartCollision(will, buttonSwitch);
            }
        }
    }
	// WILL COLLISIONS
	else if (bd1->getCollisionClass() == PINEAPPLE_C || bd2->getCollisionClass() == PINEAPPLE_C) {
		// Will x Jello
		if (bd1->getCollisionClass() == JELLO_C || bd2->getCollisionClass() == JELLO_C) {
			handleJelloCollision(will, bd1->getCollisionClass() == JELLO_C ? (JelloModel*)bd1 : (JelloModel*)bd2);
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
	} // END WILL COLLISIONS

	// KID COLLISIONS
	else if (bd1->getCollisionClass() == KID_C || bd2->getCollisionClass() == KID_C) {
		KidModel* kid = bd1->getCollisionClass() == KID_C ? (KidModel*)bd1 : (KidModel*)bd2;
        // with ground
        if (bd1->getCollisionClass() % 2 == 0 || bd2->getCollisionClass() % 2 == 0) {
            kid->setGrounded(true);
        }
		// Kid x Jello
		if (bd1->getCollisionClass() == JELLO_C || bd2->getCollisionClass() == JELLO_C) {
			handleJelloCollision(kid, bd1->getCollisionClass() == JELLO_C ? (JelloModel*)bd1 : (JelloModel*)bd2);
            kid->setGrounded(false);
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
	
    // WILL FEET COLLISIONS
    PineappleModel* will = bd1->getCollisionClass() == PINEAPPLE_C ? (PineappleModel*)bd1 : (PineappleModel*)bd2;
    string* feetSensorName = _level->getPineapple()->getSensorName();
    if ((bd1->getCollisionClass() % 2 == 0 && fix2->GetUserData() == feetSensorName) ||
        (bd2->getCollisionClass() % 2 == 0 && fix1->GetUserData() == feetSensorName)) {        _pSensorFixtures.erase(bd1 == will ? fix2 : fix1);
        if (_pSensorFixtures.empty()) {
            will->setGrounded(false);
        }
    }
    
	// WILL COLLISIONS
	if (bd1->getCollisionClass() == PINEAPPLE_C || bd2->getCollisionClass() == PINEAPPLE_C) {
		PineappleModel* will = bd1->getCollisionClass() == PINEAPPLE_C ? (PineappleModel*)bd1 : (PineappleModel*)bd2;
		// Will x Jello
		if (bd1->getCollisionClass() == JELLO_C || bd2->getCollisionClass() == JELLO_C) {
			will->setCollidingWithJello(false);
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
