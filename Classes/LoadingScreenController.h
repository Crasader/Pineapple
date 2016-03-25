#ifndef __LOADING_SCREEN_CONTROLLER_H__
#define __LOADING_SCREEN_CONTROLLER_H__

#include <cornell.h>
#include "BackgroundView.h"
#include "Texture.h"

#pragma mark -
#pragma mark Asset Constants

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