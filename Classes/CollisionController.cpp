#include "CollisionController.h"

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

	// See if we have landed on the ground.
	// TODO this is super shitty.  we should make sure bd1/bd2 is a platform rather than not a kid
	if (_avatar != nullptr && !_avatar->isCollidingWithJello() &&
		((_avatar->getSensorName() == fd2 && _avatar != bd1 && bd1->getName() != KID_NAME) ||
			(_avatar->getSensorName() == fd1 && _avatar != bd2 && bd2->getName() != KID_NAME))) {
		_avatar->setGrounded(true);
		// Could have more than one ground
		_sensorFixtures.emplace(_avatar == bd1 ? fix2 : fix1);
	}

	if (bd1->getName() == JELLO_NAME || bd2->getName() == JELLO_NAME) {
		if (_avatar != nullptr && !_avatar->isCollidingWithJello() && (_avatar == bd1 || _avatar == bd2)) {
			handleJelloCollision(_avatar);
		}
		else {
			for (int i = 0; i < KID_COUNT; i++) {
				if (_kids[i] != nullptr && !_kids[i]->isCollidingWithJello() &&
					(_kids[i] == bd1 || _kids[i] == bd2)) {
					handleJelloCollision(_kids[i]);
				}
			}
		}
	}

	// See if a kid has landed on the ground.
	for (int i = 0; i < KID_COUNT; i++) {
		if (_kids[i] != nullptr && !_kids[i]->isCollidingWithJello() &&
			((_kids[i]->getSensorName() == fd2 && _kids[i] != bd1) ||
				(_kids[i]->getSensorName() == fd1 && _kids[i] != bd2))) {
			_kids[i]->setGrounded(true);
			// Could have more than one ground
			_sensorFixtures.emplace(_kids[i] == bd1 ? fix2 : fix1);
		}
	}

	// If a kid hits the "win" door, update the kid's status
	for (int i = 0; i < KID_COUNT; i++) {
		if ((bd1 == _kids[i] && bd2 == _goalDoor) ||
			(bd1 == _goalDoor && bd2 == _kids[i])) {
			_kids[i]->setReachedGoal(true);
		}
	}

	// If Will hits the "win" door, update the Will's status
	if ((_avatar == bd1 && bd2 == _goalDoor) || (_avatar == bd2 && bd1 == _goalDoor)) {
		_avatar->setReachedGoal(true);
	}

	//See if we have collided with the blender
	if ((bd1 == _avatar && bd2 == _blender) || (bd2 == _avatar && bd1 == _blender)) {
		blendAndKill(_avatar);
		_avatar = nullptr;
		setFailure(true);
	}

	//See if a kid has collided with the blender
	for (int i = 0; i < KID_COUNT; i++) {
		if ((bd1 == _kids[i] && bd2 == _blender) || (bd2 == _kids[i] && bd1 == _blender)) {
			blendAndKill(_kids[i]);
			_kids[i] = nullptr;
			_kidsRemaining--;
			if (_kidsRemaining == 0) {
				setFailure(true);
			}
		}
	}

	//See if we have collided with a spike!
	if ((bd1 == _avatar && bd2->getName() == SPIKE_NAME) ||
		(bd2 == _avatar && bd1->getName() == SPIKE_NAME)) {
		handleSpikeCollision(_avatar);
		_avatar = nullptr;
		setFailure(true);
	}

	//See if a kid has collided with a spike
	for (int i = 0; i < KID_COUNT; i++) {
		if ((bd1 == _kids[i] && bd2->getName() == SPIKE_NAME) ||
			(bd2 == _kids[i] && bd1->getName() == SPIKE_NAME)) {
			handleSpikeCollision(_kids[i]);
			_kids[i] = nullptr;
			_kidsRemaining--;
			if (_kidsRemaining == 0) {
				setFailure(true);
			}
		}
	}

	//CRUSHHHHHHHHHHHHHHHHHHH TODO: SO BAD
	if (_avatar != nullptr && _avatar->isLarge()) {
		Obstacle* cup = nullptr;
		if (bd1 == _avatar && bd2->getName() == CUP_NAME) {
			cup = bd2;
		}
		else if (bd1->getName() == CUP_NAME && bd2 == _avatar) {
			cup = bd1;
		}
		if (cup != nullptr) {
			if (isBelowChar((BoxObstacle*)cup, _avatar)) {
				removeObstacle(cup);
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


	if ((_avatar->getSensorName() == fd2 && _avatar != bd1) ||
		(_avatar->getSensorName() == fd1 && _avatar != bd2)) {
		_sensorFixtures.erase(_avatar == bd1 ? fix2 : fix1);
		if (_sensorFixtures.empty()) {
			_avatar->setGrounded(false);
		}
	}

	//See if you or a kid has stopped colliding with jello
	if (bd1->getName() == JELLO_NAME || bd2->getName() == JELLO_NAME) {
		if (_avatar == bd1 || _avatar == bd2) {
			_avatar->setCollidingWithJello(false);
		}
		else {
			for (int i = 0; i < KID_COUNT; i++) {
				if (_kids[i] == bd1 || _kids[i] == bd2) {
					_kids[i]->setCollidingWithJello(false);
				}
			}
		}
	}

	// See if a kid has left the ground.
	for (int i = 0; i < KID_COUNT; i++) {
		if ((_kids[i]->getSensorName() == fd2 && _kids[i] != bd1) ||
			(_kids[i]->getSensorName() == fd1 && _kids[i] != bd2)) {
			_sensorFixtures.erase(_kids[i] == bd1 ? fix2 : fix1);
			if (_sensorFixtures.empty()) {
				_kids[i]->setGrounded(false);
			}
		}
	}

	// If Will leaves the "win" door, update the Will's status
	if ((_avatar == bd1 && bd2 == _goalDoor) || (_avatar == bd2 && bd1 == _goalDoor)) {
		_avatar->setReachedGoal(false);
	}
}