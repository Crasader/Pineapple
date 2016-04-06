//
//  Const.h
//  Pineapple
//
//  Created by Michael Patashnik on 3/24/16.
//
//

#ifndef __CONST_H__
#define __CONST_H__

#define SIGNUM(x)  ((x > 0) - (x < 0))

/** Width of the game world in Box2d units */
#define DEFAULT_WIDTH   32.0f
/** Height of the game world in Box2d units */
#define DEFAULT_HEIGHT  12.0f
/** Half-width of scrolling window in Box2d units */
#define WINDOW_SIZE     5.0f

/** The number of kids */
#define KID_COUNT 4

/** Color to outline the physics nodes */
#define DEBUG_COLOR     Color3B::YELLOW
/** Opacity of the physics outlines */
#define DEBUG_OPACITY   192

/** The density for most physics objects */
#define BASIC_DENSITY   0.0f
/** The density for a bullet */
#define HEAVY_DENSITY   10.0f
/** Friction of most platforms */
#define BASIC_FRICTION  0.4f
/** The restitution for all physics objects */
#define BASIC_RESTITUTION   0.1f

/** Collision Class Names */
#define BLENDER_C			5
#define PINEAPPLE_C		1
#define KID_C					3
#define JELLO_C				2
#define SPIKES_C			4
#define CUP_C					6
#define PLATFORM_C		8
#define UNASSIGNED_C	100
/** End Collision Class Names */

#endif /* Const_h */
