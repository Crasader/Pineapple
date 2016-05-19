#include "LoadingScreenController.h"
#include "LevelSelectController.h"

#pragma mark -
#pragma mark Asset Management

void loadCharacters(TextureLoader* tloader) {
    tloader->loadAsync(PINEAPPLE_TEXTURE, "textures/will_walkcycle_reduced.png");
    
    tloader->loadAsync(KID_TEXTURE_1, "textures/death_bow.png");
	tloader->loadAsync(KID_TEXTURE_2, "textures/death_glasses.png");
    tloader->loadAsync(KID_TEXTURE_3, "textures/death_hat.png");
	tloader->loadAsync(KID_TEXTURE_4, "textures/death_pirate.png");
    
    tloader->loadAsync(BLENDER_TEXTURE, "textures/blender_blendCycle2.png");
}

void loadObstacles(TextureLoader* tloader) {
    tloader->loadAsync(FLOOR_TEXTURE,      "textures/new_tile.png");
    tloader->loadAsync(FLOOR_TOP_TEXTURE, "textures/floor_top.png");

    tloader->loadAsync(PLATFORM_CENTER_TEXTURE,  "textures/platform-center.png");
    tloader->loadAsync(PLATFORM_VERTICAL_CENTER_TEXTURE, "textures/platform-center-vertical.png");
    tloader->loadAsync(PLATFORM_EDGE_RIGHT_TEXTURE,  "textures/platform-right.png");
    tloader->loadAsync(PLATFORM_EDGE_LEFT_TEXTURE,  "textures/platform-left.png");
    
    tloader->loadAsync(JELLO_TEXTURE, "textures/jelloFull3.png");
    tloader->loadAsync(SPIKE_TEXTURE, "textures/bowl_o_knives2.png");
    
    tloader->loadAsync(SPINNER_TEXTURE,   "textures/barrier.png");
    tloader->loadAsync(GOAL_TEXTURE,      "textures/fridgeFull4.png");
	tloader->loadAsync(GOAL_DOOR_TEXTURE, "textures/fridgeDoor.png");
    tloader->loadAsync(RED_CUP_TEXTURE,   "textures/cup_smash.png");
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
    
    tloader->loadAsync(SWITCH_TEXTURE_RED,  "textures/lever-red2.png");
    tloader->loadAsync(SWITCH_TEXTURE_GREEN, "textures/lever-green2.png");
    tloader->loadAsync(SWITCH_TEXTURE_BLUE,   "textures/lever-blue2.png");
    
    tloader->loadAsync(SWITCH_REVERSE_TEXTURE_RED,  "textures/lever-red-reverse2.png");
    tloader->loadAsync(SWITCH_REVERSE_TEXTURE_GREEN, "textures/lever-green-reverse2.png");
    tloader->loadAsync(SWITCH_REVERSE_TEXTURE_BLUE,   "textures/lever-blue-reverse2.png");
	
	tloader->loadAsync(SPLAT_TEXTURE_1, "textures/splatcycle1.png");
	tloader->loadAsync(SPLAT_TEXTURE_2, "textures/splatcycle2.png");
	tloader->loadAsync(SPLAT_TEXTURE_3, "textures/splatcycle3.png");
	tloader->loadAsync(SPLAT_TEXTURE_4, "textures/splatcycle4.png");
}

void loadHomeScreen(TextureLoader* tloader) {
    tloader->loadAsync(HOME_SCREEN_BACKGROUND, "textures/home_screen.png");
    tloader->loadAsync(HOME_SCREEN_WILL_ANIMATION, "textures/will_pop_out.png");
}

void loadBackground(TextureLoader* tloader) {
    tloader->loadAsync(FRONT_BACKGROUND,  "textures/backgroundFull.png");
    tloader->loadAsync(MIDDLE_BACKGROUND, "textures/hills2.png");
    tloader->loadAsync(BACK_BACKGROUND,   "textures/clouds.png");
    
}

void loadPauseScreen(TextureLoader* tloader) {
    tloader->loadAsync(PAUSE_SCREEN_OVERLAY, "textures/pause-overlay.png");
    tloader->loadAsync(CHUNKY_QUIVER, "textures/chunky_quiver.png");
}

void loadHUD(TextureLoader* tloader) {
    tloader->loadAsync(TOP_BAR, "textures/HUD/topBar.png");
    tloader->loadAsync(TOP_BLENDER, "textures/HUD/blender.png");
    tloader->loadAsync(TOP_CHILD_BOW, "textures/HUD/top_bar_bow.png");
    tloader->loadAsync(TOP_CHILD_GLASSES, "textures/HUD/top_bar_glasses.png");
    tloader->loadAsync(TOP_CHILD_HAT, "textures/HUD/top_bar_hat.png");
    tloader->loadAsync(TOP_CHILD_PIRATE, "textures/HUD/top_bar_pirate.png");
    tloader->loadAsync(TOP_WILL, "textures/HUD/will.png");
    tloader->loadAsync(STATUS_ONE, "textures/HUD/1.png");
    tloader->loadAsync(STATUS_TWO, "textures/HUD/2.png");
    tloader->loadAsync(STATUS_THREE, "textures/HUD/3.png");
    tloader->loadAsync(STATUS_FOUR, "textures/HUD/4.png");
    tloader->loadAsync(SLASH_FOUR, "textures/HUD/slash 4.png");
}

void loadSplashScreens(TextureLoader *tloader) {
    tloader->loadAsync(LOSE_SPLASH, "textures/failure.png");
    tloader->loadAsync(LOSE_JUICE_CUP, "textures/cup_o'_chillens.png");
    tloader->loadAsync(LOSE_CRYING_WILL, "textures/crying_will.png");
    
    tloader->loadAsync(WIN_SPLASH, "textures/victory.png");
    tloader->loadAsync(WIN_SPLASH_CHILD_1, "textures/happy-bow-spritesheet.png");
    tloader->loadAsync(WIN_SPLASH_CHILD_2, "textures/happy-glasses-spritesheet.png");
    tloader->loadAsync(WIN_SPLASH_CHILD_3, "textures/happy-hat-spritesheet.png");
    tloader->loadAsync(WIN_SPLASH_CHILD_4, "textures/happy-pirapplet-spritesheet.png");
    tloader->loadAsync(WIN_SPLASH_WILL, "textures/happy-will-spritesheet.png");
}

void loadTutorial(TextureLoader *tloader) {
    tloader->loadAsync(TUTORIAL_MOVE_MESSAGE, "textures/tutorial/movement_message.png");
    tloader->loadAsync(TUTORIAL_GROW_MESSAGE, "textures/tutorial/grow_message.png");
    tloader->loadAsync(TUTORIAL_SHRINK_MESSAGE, "textures/tutorial/shrink_message.png");
    tloader->loadAsync(TUTORIAL_JUMP_MESSAGE, "textures/tutorial/jump_message.png");
    tloader->loadAsync(TUTORIAL_SWITCH_MESSAGE, "textures/tutorial/switch_message.png");
    tloader->loadAsync(TUTORIAL_SMASH_MESSAGE, "textures/tutorial/cup_crush_message.png");
    tloader->loadAsync(TUTORIAL_JUMP_SIZE_MESSAGE, "textures/tutorial/jump_size_message.png");
    tloader->loadAsync(TUTORIAL_BLENDER_MESSAGE, "textures/tutorial/blender_message.png");
    tloader->loadAsync(TUTORIAL_FRIDGE_MESSAGE, "textures/tutorial/fridge_message.png");
    tloader->loadAsync(TUTORIAL_KNIVES_MESSAGE, "textures/tutorial/knives_message.png");
    
    tloader->loadAsync(TUTORIAL_SHRINK_IMAGE, "textures/tutorial/shrink_animation.png");
    tloader->loadAsync(TUTORIAL_GROW_IMAGE, "textures/tutorial/grow_animation.png");
    tloader->loadAsync(TUTORIAL_JUMP_IMAGE, "textures/tutorial/jumping_animation.png");
    tloader->loadAsync(TUTORIAL_DOOR_IMAGE, "textures/tutorial/door_animation.png");
    tloader->loadAsync(TUTORIAL_SWITCH_IMAGE, "textures/tutorial/switch_animation.png");
    tloader->loadAsync(TUTORIAL_POINTER_IMAGE, "textures/tutorial/pointer.png");
    tloader->loadAsync(TUTORIAL_SMASH_IMAGE, "textures/tutorial/smash_animation.png");
    tloader->loadAsync(TUTORIAL_KNIVES_IMAGE, "textures/tutorial/knife_death_animation.png");
}

void loadLevelSelectScreen(TextureLoader *tloader) {
    tloader->loadAsync(LEVEL_SELECT_BACKGROUND, "textures/level_select_bg.png");
}

void loadFonts(SceneManager* assets) {
    assets->loadAsync<TTFont>(MESSAGE_FONT, "fonts/RetroGame.ttf");
}

void loadSounds(SceneManager* assets) {
    assets->loadAsync<Sound>(GAME_BACKGROUND_SOUND, "sounds/background.mp3");
    assets->loadAsync<Sound>(PINEAPPLET1_DEATH_SOUND, "sounds/kid1_death.mp3");
    assets->loadAsync<Sound>(PINEAPPLET2_DEATH_SOUND, "sounds/kid2_death.mp3");
    assets->loadAsync<Sound>(PINEAPPLET3_DEATH_SOUND, "sounds/kid3_death.mp3");
    assets->loadAsync<Sound>(PINEAPPLET4_DEATH_SOUND, "sounds/kid4_death.mp3");
    assets->loadAsync<Sound>(SPLAT_SOUND, "sounds/splat.mp3");
    assets->loadAsync<Sound>(JELLO_BOING, "sounds/boing.mp3");
    assets->loadAsync<Sound>(JELLO_SQUISH_SOUND, "sounds/squish.mp3");
    assets->loadAsync<Sound>(CUP_CRUSH_SOUND, "sounds/cup_crush.mp3");
    assets->loadAsync<Sound>(BLENDER_SOUND, "sounds/blender_sound.mp3");
    assets->loadAsync<Sound>(LEVEL_SELECT_HOME_SCREEN_BACKGROUND_SOUND, "sounds/levelSelectBackground.mp3");
    assets->loadAsync<Sound>(VICTORY_SOUND, "sounds/victory.mp3");
    assets->loadAsync<Sound>(LOSS_SOUND, "sounds/loss.mp3");
}

void loadLevels(SceneManager* assets) {
    for(int i = 0; i < LEVELS_CREATED; i++) {
        assets->loadAsync<LevelModel>(LevelSelectController::LEVEL_KEYS[i], LevelSelectController::LEVEL_FILES[i]);
    }
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
    loadHUD(tloader);
    loadHomeScreen(tloader);
    loadLevelSelectScreen(tloader);
    loadSplashScreens(tloader);
    loadTutorial(tloader);
    
    loadSounds(assets);
    loadFonts(assets);
    loadLevels(assets);
}

void LoadingScreenController::init(Node* root) {
    // Load the font NOW
    AssetManager::getInstance()->getCurrent()->load<TTFont>(ELECTRIC_CIRCUS_FONT, ELECTRIC_CIRCUS_FONT_LOCATION);
    AssetManager::getInstance()->getCurrent()->load<Texture2D>(LOADING_BACKGROUND, "textures/loadingBackground.png");
    _loadingLabel = nullptr;
    _rootnode = root;
    _isInitted = true;
    
    Size size = _rootnode->getContentSize();
    Vec2 center(size.width/2.0f,size.height/2.0f);
    
    //Create the background image
//    Texture2D* image = AssetManager::getInstance()->getCurrent()->get<Texture2D>(LOADING_BACKGROUND);
//    _loadingImage = PolygonNode::createWithTexture(image);
//    _loadingImage->setPosition(center);
//    _loadingImage->setAnchorPoint(Vec2(0.5f, 0.5f));
//    _loadingImage->setScale(size.width/image->getContentSize().width, size.height/image->getContentSize().height);
//    
//    _rootnode->addChild(_loadingImage, 0);
    
    // Create the message label.
    _loadingLabel = Label::create();
    _loadingLabel->setTTFConfig(AssetManager::getInstance()->getCurrent()->get<TTFont>(ELECTRIC_CIRCUS_FONT)->getTTF());
    _loadingLabel->setAnchorPoint(Vec2(0.5f,0.5f));
    _loadingLabel->setPosition(center);
    _loadingLabel->setString("Loading...");
    
    // Add the label as a child to loading screen
    _rootnode->addChild(_loadingLabel, 1);
}

void LoadingScreenController::update(float dt) {
    
}