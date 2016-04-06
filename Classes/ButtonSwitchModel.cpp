//
//  ButtonSwitchModel.cpp
//  Pineapple
//
//

#include "ButtonSwitchModel.h"
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>

/** The amount to shrink the body fixture (vertically) relative to the image */
#define SWITCH_VSHRINK  1.0f
/** The amount to shrink the body fixture (horizontally) relative to the image */
#define SWITCH_HSHRINK  1.0f
/** The amount to shrink the body fixture (vertically) relative to the image */
#define BUTTON_VSHRINK  1.0f
/** The amount to shrink the body fixture (horizontally) relative to the image */
#define BUTTON_HSHRINK  1.0f

ButtonSwitchModel* ButtonSwitchModel::create(const Vec2& pos, const Vec2& scale, const bool isSwitch, const Color color) {
    ButtonSwitchModel* button_switch = new (std::nothrow) ButtonSwitchModel();
    if (button_switch && button_switch->init(pos,scale, isSwitch, color)) {
        button_switch->setPosition(pos + Vec2(button_switch->getWidth()/2, button_switch->getHeight()/2));
        button_switch->autorelease();
        return button_switch;
    }
    CC_SAFE_DELETE(button_switch);
    return nullptr;
}

bool ButtonSwitchModel::init(const Vec2& pos, const Vec2& scale, const bool isSwitch, const Color color) {
    if (isSwitch) {
        setName(BUTTON_SWITCH_NAME);
    }
    
    _color = color;
    
    SceneManager* scene = AssetManager::getInstance()->getCurrent();
    Texture2D* image;
    if (isSwitch) {
        // TODO(ekotlikoff)
        //image = scene->get<Texture2D>(SWITCH_TEXTURE);
    } else {
        //image = scene->get<Texture2D>(BUTTON_TEXTURE);
    }
    // TODO(ekotlikoff) - remove
    image = scene->get<Texture2D>(PLATFORM_TEXTURE);
    
    // Multiply by the scaling factor so we can be resolution independent
    float cscale = Director::getInstance()->getContentScaleFactor();
    Size nsize = image->getContentSize()*cscale;
    
    if (isSwitch) {
        nsize.width  *= SWITCH_HSHRINK/scale.x;
        nsize.height *= SWITCH_VSHRINK/scale.y;
    } else {
        nsize.width  *= BUTTON_HSHRINK/scale.x;
        nsize.height *= BUTTON_VSHRINK/scale.y;
    }
    if (BoxObstacle::init(pos,nsize)) {
        setFriction(0.0f);
        setFixedRotation(true);
        
        // Gameplay attributes
        _isSwitch = isSwitch;
        return true;
    }
    return false;
}

#pragma mark -
#pragma mark Physics Methods
/**
 * Create new fixtures for this body, defining the shape
 *
 * This is the primary method to override for custom physics objects
 */
void ButtonSwitchModel::createFixtures() {
    if (_body == nullptr) {
        return;
    }
    
    BoxObstacle::createFixtures();
}

/**
 * Release the fixtures for this body, reseting the shape
 *
 * This is the primary method to override for custom physics objects.
 */
void ButtonSwitchModel::releaseFixtures() {
    if (_body != nullptr) {
        return;
    }
    
    BoxObstacle::releaseFixtures();
}

/**
 * Updates the object's physics state (NOT GAME LOGIC).
 *
 * We use this method to reset cooldowns.
 *
 * @param delta Number of seconds since last animation frame
 */
void ButtonSwitchModel::update(float dt) {
    BoxObstacle::update(dt);
}


#pragma mark -
#pragma mark Scene Graph Methods

/**
 * Redraws the outline of the physics fixtures to the debug node
 *
 * The debug node is use to outline the fixtures attached to this object.
 * This is very useful when the fixtures have a very different shape than
 * the texture (e.g. a circular shape attached to a square texture).
 */
void ButtonSwitchModel::resetDebugNode() {
    BoxObstacle::resetDebugNode();
}