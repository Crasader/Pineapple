//
//  HUDController.cpp
//  Pineapple
//
//

#include "HUDController.h"

HUDController* HUDController::HUD_CONTROLLER = nullptr;

void HUDController::init(AbsScreenController* gameController, Node* worldNode, SceneManager* assets, Node* root, InputController* input, float blenderPos) {
    CCASSERT(HUD_CONTROLLER != nullptr, "NULL HUD CONTROLLER INITIALIZED");
    HUD_CONTROLLER->_gameController = gameController;
    HUD_CONTROLLER->_rootNode = root;
    HUD_CONTROLLER->_hudNode = Node::create();
    HUD_CONTROLLER->_hudNode->retain();
    HUD_CONTROLLER->_inputController = input;
    HUD_CONTROLLER->_screenSize = Vec2(root->getContentSize().width, root->getContentSize().height);
    HUD_CONTROLLER->_assets = assets;
    // init children status
    initChildrenStatus(assets);
    // init progress bar
    initProgressBar(assets);
    // init sound button
    initSoundButton();
    // init pause button
    initPauseButton();
    addHUD();
}

void HUDController::initChildrenStatus(SceneManager* assets) {
    float cscale = Director::getInstance()->getContentScaleFactor();
    for (int i = 0; i < NUM_STATUS_NODES; i++) {
        string key;
        if (i == 0) { key = STATUS_ONE; }
        else if (i == 1) { key = STATUS_TWO; }
        else if (i == 2) { key = STATUS_THREE; }
        else if (i == 3) { key = STATUS_FOUR; }
        else if (i == 4) { key = SLASH_FOUR; }
        PolygonNode* status = PolygonNode::createWithTexture(assets->get<Texture2D>(key));
        status->retain();
        HUD_CONTROLLER->_numbers[i] = status;
        status->setScale(cscale * STATUS_SCALE);
        // progress bar is centered
        status->setPosition(HUDController::HUD_CONTROLLER->_screenSize.x * CHILDREN_STATUS_HORIZ_POS_RATIO + status->getContentSize().width * STATUS_SCALE / 2.0f,
                            HUDController::HUD_CONTROLLER->_screenSize.y * HUD_MIDDLE_Y_POS_RATIO);
        status->setLocalZOrder(TOP_BAR_BACKGROUND_Z_ORDER);
        if (i == 3 || i == 4) {
            HUD_CONTROLLER->_hudNode->addChild(status);
        }
    }
}

void HUDController::initProgressBar(SceneManager* assets) {
    float cscale = Director::getInstance()->getContentScaleFactor();
    // top bar
    PolygonNode* topbar = PolygonNode::createWithTexture(assets->get<Texture2D>(TOP_BAR));
    topbar->retain();
    topbar->setScale(cscale * PROGRESS_BAR_SCALE_HORIZ, cscale * PROGRESS_BAR_SCALE_VERT);
    // progress bar is centered
    topbar->setPosition(HUDController::HUD_CONTROLLER->_screenSize.x /2.0f,
                        HUDController::HUD_CONTROLLER->_screenSize.y * HUD_MIDDLE_Y_POS_RATIO);
    HUD_CONTROLLER->_progressBarWidth = topbar->getContentSize().width * cscale * PROGRESS_BAR_SCALE_HORIZ;
    HUD_CONTROLLER->_progressBarLeftXPos = topbar->getPosition().x - HUD_CONTROLLER->_progressBarWidth / 2.0f;
    topbar->setLocalZOrder(TOP_BAR_BACKGROUND_Z_ORDER);
    HUD_CONTROLLER->_hudNode->addChild(topbar, HUD_Z_ORDER);
    HUD_CONTROLLER->_topbar = topbar;
    
    // blender
    PolygonNode* blender = PolygonNode::createWithTexture(assets->get<Texture2D>(TOP_BLENDER));
    blender->retain();
    blender->setScale(cscale * BLENDER_ICON_SCALE);
    blender->setPositionY(HUD_CONTROLLER->_screenSize.y * HUD_MIDDLE_Y_POS_RATIO + blender->getContentSize().height * BLENDER_ICON_SCALE / 2.0f);
    blender->setLocalZOrder(TOP_BAR_FOREGROUND_Z_ORDER);
    HUD_CONTROLLER->_blender = blender;
    HUD_CONTROLLER->_hudNode->addChild(HUD_CONTROLLER->_blender, HUD_Z_ORDER);
    
    // children bow glasses pinkhat pirate
    for (int i = 0; i < KID_COUNT; i++) {
        string key;
        if (i == 0) { key = TOP_CHILD_BOW; }
        else if (i == 1) { key = TOP_CHILD_GLASSES; }
        else if (i == 2) { key = TOP_CHILD_HAT; }
        else if (i == 3) {key = TOP_CHILD_PIRATE; }
        PolygonNode* child = PolygonNode::createWithTexture(assets->get<Texture2D>(key));
        child->retain();
        child->setScale(cscale * KID_ICON_SCALE);
        child->setPositionY(HUD_CONTROLLER->_screenSize.y * HUD_MIDDLE_Y_POS_RATIO + child->getContentSize().height * KID_ICON_SCALE * cscale / 2.0f);
        HUD_CONTROLLER->_children[i] = child;
        HUD_CONTROLLER->_hudNode->addChild(HUD_CONTROLLER->_children[i], HUD_Z_ORDER);
    }
    
    // will
    PolygonNode* will = PolygonNode::createWithTexture(assets->get<Texture2D>(TOP_WILL));
    will->retain();
    will->setScale(cscale * WILL_SCALE);
    will->setPositionY(HUD_CONTROLLER->_screenSize.y * HUD_MIDDLE_Y_POS_RATIO + WILL_OFFSET_Y + will->getContentSize().height * WILL_SCALE * cscale / 2.0f);
    HUD_CONTROLLER->_will = will;
    HUD_CONTROLLER->_hudNode->addChild(HUD_CONTROLLER->_will, HUD_Z_ORDER);
}

void HUDController::initSoundButton() {
    float cscale = Director::getInstance()->getContentScaleFactor();

    HUD_CONTROLLER->_soundButton = CheckBox::create("textures/buttons/music_toggle_on.png", "textures/buttons/music_toggle_off.png");
    HUD_CONTROLLER->_soundButton->cocos2d::Node::setScale(cscale * BUTTON_SCALE);
    float bheight = HUD_CONTROLLER->_soundButton->getContentSize().height;
    float bwidth  = HUD_CONTROLLER->_soundButton->getContentSize().width;
    HUD_CONTROLLER->_soundButton->setPositionY(HUD_CONTROLLER->_screenSize.y * HUD_MIDDLE_Y_POS_RATIO);
    HUD_CONTROLLER->_soundButton->setPositionX(HUD_CONTROLLER->_screenSize.x * SOUND_BUTTON_HORIZ_POS_RATIO);
    HUD_CONTROLLER->_soundButton->retain();
    HUD_CONTROLLER->_hudNode->addChild(HUD_CONTROLLER->_soundButton);
    HUD_CONTROLLER->_soundButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            if (HUD_CONTROLLER->_soundButton->isSelected()) {
                SoundEngine::getInstance()->stopMusic();
                SoundEngine::getInstance()->stopAllEffects();
            } else {
                Sound* sound = AssetManager::getInstance()->getCurrent()->get<Sound>(GAME_BACKGROUND_SOUND);
                SoundEngine::getInstance()->playMusic(sound, true, MUSIC_VOLUME);
                SoundEngine::getInstance()->setMusicVolume(MUSIC_VOLUME);
            }
        }
    });
}

void HUDController::initPauseButton() {
    float cscale = Director::getInstance()->getContentScaleFactor();

    HUD_CONTROLLER->_pauseButton = Button::create();
    HUD_CONTROLLER->_pauseButton->loadTextureNormal("textures/buttons/pauseButton.png");
    HUD_CONTROLLER->_pauseButton->loadTexturePressed("textures/buttons/pause_inverse.png");
    HUD_CONTROLLER->_pauseButton->cocos2d::Node::setScale(cscale * BUTTON_SCALE);
    float bheight = HUD_CONTROLLER->_pauseButton->getContentSize().height;
    float bwidth  = HUD_CONTROLLER->_pauseButton->getContentSize().width;
    HUD_CONTROLLER->_pauseButton->setPositionY(HUD_CONTROLLER->_screenSize.y * HUD_MIDDLE_Y_POS_RATIO);
    HUD_CONTROLLER->_pauseButton->setPositionX(HUD_CONTROLLER->_screenSize.x * PAUSE_BUTTON_HORIZ_POS_RATIO);
    HUD_CONTROLLER->_pauseButton->retain();
    HUD_CONTROLLER->_hudNode->addChild(HUD_CONTROLLER->_pauseButton);
    HUD_CONTROLLER->_pauseButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            PauseController::pause();
        }
    });
}

void HUDController::update(int childrenAlive, float blenderLoc, vector<KidModel*> children, PineappleModel* will, float goalLoc) {
    float cscale = Director::getInstance()->getContentScaleFactor();
    
    // update children status numbers
    if (!HUD_CONTROLLER->_hudNode->getChildren().contains(HUD_CONTROLLER->_numbers[childrenAlive-1])) {
        if (childrenAlive == 0) {
            HUD_CONTROLLER->_hudNode->removeChild(HUD_CONTROLLER->_numbers[childrenAlive]);
            HUD_CONTROLLER->_hudNode->removeChild(HUD_CONTROLLER->_numbers[NUM_STATUS_NODES-1]);
        } else {
            HUD_CONTROLLER->_hudNode->removeChild(HUD_CONTROLLER->_numbers[childrenAlive]);
            HUD_CONTROLLER->_hudNode->addChild(HUD_CONTROLLER->_numbers[childrenAlive-1]);
        }
    }
    
    // update blender
    if (blenderLoc < 0) {
        blenderLoc = 0;
    }
    float blenderPos = unitsToTopBarX(blenderLoc, goalLoc);
    // now make it intuitive when you're gonna die
    HUD_CONTROLLER->_blender->setPositionX(blenderPos);
    
    // update children icons
    for (int i = 0; i < KID_COUNT; i++) {
        // get child's current place
        KidModel* kid = children[i];
        int place = KID_COUNT;
        for (int j = 0; j < KID_COUNT; j++) {
            // if this child is dead then I'm beating him
            if (children[j] == nullptr) {
                place -= 1;
            } else if (kid != nullptr && j != i && kid->getX() > children[j]->getX()) {
                place -= 1;
            }
        }
        if (kid != nullptr) {
            float pos = unitsToTopBarX(kid->getPosition().x, goalLoc);
            float width = HUD_CONTROLLER->_children[i]->getContentSize().width * KID_ICON_SCALE * cscale;
            pos += width / 2.0f;
            if (pos <= blenderPos) {
                if (HUD_CONTROLLER->_hudNode->getChildren().contains(HUD_CONTROLLER->_children[i])) {
                    HUD_CONTROLLER->_hudNode->removeChild(HUD_CONTROLLER->_children[i]);
                }
            } else if (pos >= HUD_CONTROLLER->_progressBarLeftXPos + HUD_CONTROLLER->_progressBarWidth -
                    (place * width/2.0f)) {
                pos = HUD_CONTROLLER->_progressBarLeftXPos + HUD_CONTROLLER->_progressBarWidth -
                (place * width/2.0f);
            }
            HUD_CONTROLLER->_children[i]->setPositionX(pos);
        } else {
            HUD_CONTROLLER->_hudNode->removeChild(HUD_CONTROLLER->_children[i]);
        }
    }
    float willPos = 0.0f;
    // update will
    if (will) {
        willPos = unitsToTopBarX(will->getPosition().x, goalLoc);
        float width = HUD_CONTROLLER->_will->getContentSize().width * WILL_SCALE * cscale;
        // now center it
        willPos += width / 2.0f;
        if (willPos <= blenderPos) {
            if (HUD_CONTROLLER->_hudNode->getChildren().contains(HUD_CONTROLLER->_will)) {
                HUD_CONTROLLER->_hudNode->removeChild(HUD_CONTROLLER->_will);
            }
        } else if (willPos >= HUD_CONTROLLER->_progressBarLeftXPos + HUD_CONTROLLER->_progressBarWidth) {
            willPos = HUD_CONTROLLER->_progressBarLeftXPos + HUD_CONTROLLER->_progressBarWidth;
        }
        HUD_CONTROLLER->_will->setPositionX(willPos);
    } else {
        HUD_CONTROLLER->_hudNode->removeChild(HUD_CONTROLLER->_will);
    }
}

void HUDController::release() {
    if (HUD_CONTROLLER) {
        HUD_CONTROLLER->_hudNode->release();
        // release children status nodes
        for (int i = 0; i < KID_COUNT; i++) {
            HUD_CONTROLLER->_hudNode->removeChild(HUD_CONTROLLER->_children[i]);
            HUD_CONTROLLER->_children[i]->release();
        }
        // release the status nodes
        for (int i = 0; i < NUM_STATUS_NODES; i++) {
            HUD_CONTROLLER->_hudNode->removeChild(HUD_CONTROLLER->_numbers[i]);
            HUD_CONTROLLER->_numbers[i]->release();
            HUD_CONTROLLER->_numbers[i] = nullptr;
        }
        // release will
        HUD_CONTROLLER->_hudNode->removeChild(HUD_CONTROLLER->_will);
        HUD_CONTROLLER->_will->release();
        HUD_CONTROLLER->_will = nullptr;
        // release sound node
        HUD_CONTROLLER->_hudNode->removeChild(HUD_CONTROLLER->_soundButton);
        HUD_CONTROLLER->_soundButton->release();
        HUD_CONTROLLER->_soundButton = nullptr;
        // release pause node
        HUD_CONTROLLER->_hudNode->removeChild(HUD_CONTROLLER->_pauseButton);
        HUD_CONTROLLER->_pauseButton->release();
        HUD_CONTROLLER->_pauseButton = nullptr;
        // release blender
        HUD_CONTROLLER->_hudNode->removeChild(HUD_CONTROLLER->_blender);
        HUD_CONTROLLER->_blender->release();
        HUD_CONTROLLER->_blender = nullptr;
        // release bar
        HUD_CONTROLLER->_hudNode->removeChild(HUD_CONTROLLER->_topbar);
        HUD_CONTROLLER->_topbar->release();
        HUD_CONTROLLER->_topbar = nullptr;
        HUD_CONTROLLER->_rootNode->removeChild(HUD_CONTROLLER->_hudNode);
        HUD_CONTROLLER->~HUDController();
    }
}