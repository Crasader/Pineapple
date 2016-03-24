#ifndef __LOADING_SCREEN_CONTROLLER_H__
#define __LOADING_SCREEN_CONTROLLER_H__

#include <cornell.h>
#include "BackgroundView.h"

#pragma mark -
#pragma mark Asset Constants

//TODO: move to object models where appropriate

/** The key for the tile tile texture in the asset manager */
#define TILE_TEXTURE   "tile"
/** The key for the platform texture in the asset manager */
#define PLATFORM_TEXTURE "platform"
/** The key for the win door texture in the asset manager */
#define GOAL_TEXTURE    "goal"
/** The key for the font for victory/failure messages in the asset manager */
#define MESSAGE_FONT    "retro"

class LoadingScreenController {
public:
	/**
	* Constructor
	*/
	LoadingScreenController() {};

	/**

	* Preloads all of the assets necessary for this game world
	*/
	void preload();
};

#endif