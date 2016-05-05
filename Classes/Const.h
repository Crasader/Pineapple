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
#define SIZE_ONE    Size(1,1)

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

/** The default screen size, in box2d units */
#define SCREEN Rect(0,0,DEFAULT_WIDTH,DEFAULT_HEIGHT)
/** The default drawing scale of the game */
#define DEFAULT_DRAW_SCALE Vec2(32,32)

/** The new heavier gravity for this world (so it is not so floaty) */
#define DEFAULT_GRAVITY -14.7f
/** The density for most physics objects */
#define BASIC_DENSITY   1.0f
/** The density for a bullet */
#define HEAVY_DENSITY   10.0f
/** Friction of most platforms */
#define BASIC_FRICTION  0.4f
/** The restitution for all physics objects */
#define BASIC_RESTITUTION   0.1f

/** Collision Class Names */
#define PINEAPPLE_C             1
#define KID_C					3
#define BLENDER_C               5
#define BUTTON_SWITCH_C         9
#define GOAL_C                  11

//Elements that count as ground
#define JELLO_C                 2
#define SPIKES_C                4
#define CUP_C					6
#define MOVEABLE_PLATFORM_C     10
#define WALL_C                  12

#define UNASSIGNED_C            100
/** End Collision Class Names */

/** The name given to the pineapple object */
#define PINEAPPLE_NAME "will"
/** The name given to the blender object */
#define BLENDER_NAME   "blender"
/** The name given to a kid object, plus its index */
#define KID_NAME       "kid" //i.e. kid_2
/** The name given to wall objects */
#define WALL_NAME     "wall"
/** The name given to jello objects */
#define JELLO_NAME    "jello"
/** The name given to spike objects */
#define SPIKE_NAME    "spike"
/** The name given to cup objects */
#define CUP_NAME      "cup"
/** The name given to the goal object */
#define GOAL_NAME     "goal"

/** The message for winning the game */
#define WIN_MESSAGE     "VICTORY"
/** The color of the win message */
#define WIN_COLOR       Color3B::YELLOW
/** The message for losing the game */
#define LOSE_MESSAGE    "BLENDED"
/** The color of the lose message */
#define LOSE_COLOR      Color3B::RED
/** The message for resetting the game */
#define LOAD_MESSAGE    "Loading"
/** The color of the lose message */
#define LOAD_COLOR      Color3B::WHITE
/** The size of default fonts */
#define DEFAULT_FONT_SIZE      64.0f

/** The key for no currently playing music */
#define NO_BACKGROUND_SOUND "no_background_music"
/** The key for the main background music */
#define GAME_BACKGROUND_SOUND "game_background_music"
/** The key for the level select background music */
#define LEVEL_SELECT_HOME_SCREEN_BACKGROUND_SOUND "level_select_background_music"

/** The key for saved levels completed int */
#define LEVELS_COMPLETED_KEY    "levels_completed"

/** The volume for the music */
#define MUSIC_VOLUME    0.7f
/** The volume for sound effects */
#define EFFECT_VOLUME   1.0f

/** The duration of the vibration when a kid / you are blended */
#define VIBRATION_DURATION  1.0f

// Max value for scaling blender sound based on distance
#define MAX_VOL_SCALE						1.3f
// Distance at which the blender should be silent
#define BLENDER_VOL_OFF_DISTANCE 32.0f

// The distance value for which the blending sound will play at EFFECT_VOLUME
#define NORMAL_BLENDER_DISTANCE 2.0f


#endif /* Const_h */
