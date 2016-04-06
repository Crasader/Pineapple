#ifndef __COLLISION_OBJECT_H__
#define __COLLISION_OBJECT_H__

#include <cornell.h>
#include "Const.h"

using namespace cocos2d;

class CollisionObjectModel {

public:

	/**
	 *	Returns identifying key for the object class, for use by the collision controller
	 */
	virtual int getCollisionClass() { return UNASSIGNED_C; };

};
#endif