#ifndef __LOADING_SCREEN_CONTROLLER_H__
#define __LOADING_SCREEN_CONTROLLER_H__

#include <cornell.h>
#include "BackgroundView.h"
#include "Texture.h"
#include "Sounds.h"
#include "LevelModel.h"
#include "Levels.h"
#include "AbsScreenController.h"

/** Font size for loading message */
#define LOADING_FONT_SIZE   64.0f

class LoadingScreenController : public AbsScreenController {
protected:
    Label* _loadingLabel;
    
    /** The background image displayed for the loading view */
    Node* _loadingImage;
    
public:
	/**
	* Constructor
	*/
	LoadingScreenController() {};

    void init(Node* root);
    
	/**

	* Preloads all of the assets necessary for this game world
	*/
	void preload();
    
    void update(float dt) override;
};

#endif