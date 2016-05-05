//
//  PauseController.cpp
//  Pineapple
//

#include "PauseController.h"

const string PauseController::BUTTON_FILES[PAUSE_CONTROLLER_NUM_BUTTONS*2] = {"textures/buttons/resume.png", "textures/buttons/resume_inverse.png", "textures/buttons/restart.png", "textures/buttons/restart_inverse.png", "textures/buttons/level_select.png","textures/buttons/level_select_inverse.png", "textures/buttons/quit.png", "textures/buttons/quit_inverse.png"};

PauseController* PauseController::PAUSE_CONTROLLER = nullptr;

void PauseController::init(Node* worldNode, SceneManager* assets, Node* root, InputController* input) {
    CCASSERT(PAUSE_CONTROLLER != nullptr, "NULL PAUSE CONTROLLER INITIALIZED");
    float cscale = Director::getInstance()->getContentScaleFactor();

    PAUSE_CONTROLLER->_rootNode = root;
    PAUSE_CONTROLLER->_pauseNode = Node::create();
    PAUSE_CONTROLLER->_pauseNode->retain();
    PAUSE_CONTROLLER->_inputController = input;
    PAUSE_CONTROLLER->_center = Vec2(root->getContentSize().width/2.0f, root->getContentSize().height/2.0f);
    
    PAUSE_CONTROLLER->_chunkyQuiver = AnimationNode::create(assets->get<Texture2D>(CHUNKY_QUIVER), 1, ANIMATION_FRAMES, ANIMATION_FRAMES);
    PAUSE_CONTROLLER->_chunkyQuiver->retain();
    PAUSE_CONTROLLER->_chunkyQuiver->setScale(cscale);
    PAUSE_CONTROLLER->_chunkyQuiver->setLocalZOrder(CHUNKY_Z_ORDER);
    PAUSE_CONTROLLER->_chunkyQuiver->setPosition(PAUSE_CONTROLLER->_center.x, PAUSE_CONTROLLER->_center.y);
    PAUSE_CONTROLLER->_pauseNode->addChild(PAUSE_CONTROLLER->_chunkyQuiver);

    Texture2D* image = assets->get<Texture2D>(PAUSE_SCREEN_OVERLAY);
    PAUSE_CONTROLLER->_backgroundOverlay = PolygonNode::createWithTexture(image);
    PAUSE_CONTROLLER->_backgroundOverlay->setScale(5.0f);
    PAUSE_CONTROLLER->_backgroundOverlay->setLocalZOrder(PAUSE_OVERLAY_Z_ORDER);
    PAUSE_CONTROLLER->_backgroundOverlay->retain();
    PAUSE_CONTROLLER->_pauseNode->addChild(PAUSE_CONTROLLER->_backgroundOverlay);
    for (int i = 0; i < PAUSE_CONTROLLER_NUM_BUTTONS; i++) {
        Button* button = Button::create();
        button->loadTextureNormal(BUTTON_FILES[i*2]);
        button->loadTexturePressed(BUTTON_FILES[i*2 + 1]);
        float bheight = button->getContentSize().height;
        button->setPositionY(BUTTON_Y_OFFSET + PAUSE_CONTROLLER->_center.y - bheight/2.0f + BUTTON_HEIGHT/2.0f - (i % BUTTONS_PER_COL * BUTTON_HEIGHT)/(float)BUTTONS_PER_COL);
        if (i < BUTTONS_PER_COL) {
            button->setPositionX(PAUSE_CONTROLLER->_center.x - BUTTON_COL_OFFSET/2.0f + BUTTON_X_OFFSET);
        } else {
            button->setPositionX(PAUSE_CONTROLLER->_center.x + BUTTON_COL_OFFSET/2.0f + BUTTON_X_OFFSET);
        }
        button->setScale(cscale);
        button->setLocalZOrder(BUTTON_Z_ORDER);
        button->retain();
        PAUSE_CONTROLLER->_buttons[i] = button;
        PAUSE_CONTROLLER->_pauseNode->addChild(button);
    }
}
