#include "LoadingScreenController.h"

#pragma mark -
#pragma mark Asset Management

void loadCharacters(TextureLoader* tloader) {
    tloader->loadAsync(PINEAPPLE_TEXTURE, "textures/will_walkcycle_reduced.png");
    
    tloader->loadAsync(KID_TEXTURE_1, "textures/child_bow_walkcycle_reduced.png");
    tloader->loadAsync(KID_TEXTURE_2, "textures/child_glasses_walkcycle_reduced.png");
    tloader->loadAsync(KID_TEXTURE_3, "textures/child_hat_walkcycle_reduced.png");
    tloader->loadAsync(KID_TEXTURE_4, "textures/child_pirate_walkcycle_reduced.png");
    
    tloader->loadAsync(BLENDER_TEXTURE, "textures/blender.png");
}

void loadObstacles(TextureLoader* tloader) {
    tloader->loadAsync(FLOOR_TEXTURE,      "textures/new_tile.png");
    tloader->loadAsync(FLOOR_TOP_TEXTURE, "textures/floor_top.png");
    tloader->loadAsync(PLATFORM_TEXTURE,  "textures/platform.png");
    
    tloader->loadAsync(JELLO_TEXTURE, "textures/jello.png");
    tloader->loadAsync(SPIKE_TEXTURE, "textures/bowl_o_knives.png");
    
    tloader->loadAsync(SPINNER_TEXTURE,   "textures/barrier.png");
    tloader->loadAsync(GOAL_TEXTURE,      "textures/goal.png");
    tloader->loadAsync(RED_CUP_TEXTURE,   "textures/redcup.png");
    tloader->loadAsync(BLUE_CUP_TEXTURE,  "textures/bluecup.png");
    tloader->loadAsync(GREEN_CUP_TEXTURE, "textures/greencup.png");
    tloader->loadAsync(CUPSTACK_TEXTURE,  "textures/stackedcups.png");
    
    tloader->loadAsync(LEFT_NUBBIN_TEXTURE_RED,  "textures/left-door-red.png");
    tloader->loadAsync(RIGHT_NUBBIN_TEXTURE_RED, "textures/right-door-red.png");
    tloader->loadAsync(MIDDLE_TEXTURE_RED,   "textures/center-door-red.png");
    
    tloader->loadAsync(LEFT_NUBBIN_TEXTURE_BLUE,  "textures/left-door-blue.png");
    tloader->loadAsync(RIGHT_NUBBIN_TEXTURE_BLUE, "textures/right-door-blue.png");
    tloader->loadAsync(MIDDLE_TEXTURE_BLUE,   "textures/center-door-blue.png");
    
    tloader->loadAsync(LEFT_NUBBIN_TEXTURE_GREEN,  "textures/left-door.png");
    tloader->loadAsync(RIGHT_NUBBIN_TEXTURE_GREEN, "textures/right-door.png");
    tloader->loadAsync(MIDDLE_TEXTURE_GREEN,   "textures/center-door.png");
    
    tloader->loadAsync(SWITCH_TEXTURE_RED,  "textures/lever-red.png");
    tloader->loadAsync(SWITCH_TEXTURE_GREEN, "textures/lever-green.png");
    tloader->loadAsync(SWITCH_TEXTURE_BLUE,   "textures/lever-blue.png");
    
    tloader->loadAsync(SWITCH_REVERSE_TEXTURE_RED,  "textures/lever-red-reverse.png");
    tloader->loadAsync(SWITCH_REVERSE_TEXTURE_GREEN, "textures/lever-green-reverse.png");
    tloader->loadAsync(SWITCH_REVERSE_TEXTURE_BLUE,   "textures/lever-blue-reverse.png");
}

void loadBackground(TextureLoader* tloader) {
    tloader->loadAsync(FRONT_BACKGROUND,  "textures/background_full.png");
    tloader->loadAsync(MIDDLE_BACKGROUND, "textures/hills.png");
    tloader->loadAsync(BACK_BACKGROUND,   "textures/clouds.png");
    
}

void loadPauseScreen(TextureLoader* tloader) {
    tloader->loadAsync(PAUSE_SCREEN_OVERLAY, "textures/pause-overlay.png");

}

void loadLevelSelectScreen(TextureLoader *tloader) {
    tloader->loadAsync(LEVEL_SELECT_BACKGROUND, "textures/level_select_bg.png");
}

void loadFonts(SceneManager* assets) {
    assets->loadAsync<TTFont>(MESSAGE_FONT, "fonts/RetroGame.ttf");

}

void loadLevels(SceneManager* assets) {
    assets->loadAsync<LevelModel>(LEVEL_ONE_KEY, LEVEL_ONE_FILE);
    assets->loadAsync<LevelModel>(LEVEL_TWO_KEY, LEVEL_TWO_FILE);
}


/**
* Preloads the assets needed for the game.
*/
void LoadingScreenController::preload() {
	SceneManager* assets = AssetManager::getInstance()->getCurrent();
	TextureLoader* tloader = (TextureLoader*)assets->access<Texture2D>();

    loadCharacters(tloader);
    loadObstacles(tloader);
    loadBackground(tloader);
    loadPauseScreen(tloader);
    loadLevelSelectScreen(tloader);
    
    loadFonts(assets);
    loadLevels(assets);
}

void LoadingScreenController::init(Node* root) {
    // Load the font NOW
    AssetManager::getInstance()->getCurrent()->load<TTFont>(LOADING_FONT_NAME, "fonts/MarkerFelt.ttf");
    _loadingLabel = nullptr;
    _rootnode = root;
    _isInitted = true;
}

void LoadingScreenController::update(float dt) {
    if (_loadingLabel == nullptr) {
        Size size = _rootnode->getContentSize();
        Vec2 center(size.width/2.0f,size.height/2.0f);
        
        // Create the message label.
        _loadingLabel = Label::create();
        _loadingLabel->setTTFConfig(AssetManager::getInstance()->getCurrent()->get<TTFont>(LOADING_FONT_NAME)->getTTF());
        _loadingLabel->setAnchorPoint(Vec2(0.5f,0.5f));
        _loadingLabel->setPosition(center);
        _loadingLabel->setString(LOADING_MESSAGE);
        
        // Add the label as a child to loading screen
        _rootnode->addChild(_loadingLabel, 5);
    }
}