//
//  ButtonSwitchModel.cpp
//  Pineapple
//
//

#include "ButtonSwitchModel.h"
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>
#include <cornell/CUPolygonNode.h>

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
        button_switch->retain();
        return button_switch;
    }
    CC_SAFE_DELETE(button_switch);
    return nullptr;
}

bool ButtonSwitchModel::init(const Vec2& pos, const Vec2& scale, const bool isSwitch, const Color color) {
    if (isSwitch) {
        setName(BUTTON_SWITCH_NAME);
    }
    
    if (BoxObstacle::init(pos,Size(scale))) {
        setFriction(0.0f);
        setFixedRotation(true);
        
        // Gameplay attributes
        _color = color;
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
/** Updates the texture to the correct color and status texture */
void ButtonSwitchModel::updateTexture() {
    std::string texName;
    
    if(_isSwitch) {
        if (_color == Color::blue) {
            if (_isPressed) {
                texName = SWITCH_TEXTURE_BLUE;
            } else {
                texName = SWITCH_REVERSE_TEXTURE_BLUE;
            }
        } else if (_color == Color::green) {
            if (_isPressed) {
                texName = SWITCH_TEXTURE_GREEN;
            } else {
                texName = SWITCH_REVERSE_TEXTURE_GREEN;
            }
        } else {
            if (_isPressed) {
                texName = SWITCH_TEXTURE_RED;
            } else {
                texName = SWITCH_REVERSE_TEXTURE_RED;
            }
        }
    } else {
        if (_color == Color::blue) {
            texName = BUTTON_TEXTURE_BLUE;
        } else if (_color == Color::green) {
            texName = BUTTON_TEXTURE_GREEN;
        } else {
            texName = BUTTON_TEXTURE_RED;
        }
    }
    
    PolygonNode* pnode = dynamic_cast<PolygonNode*>(_node);
    SceneManager* assets =  AssetManager::getInstance()->getCurrent();
    Texture2D* image = assets->get<Texture2D>(texName);
    pnode->setTexture(image);
}


/**
 * Performs any necessary additions to the scene graph node.
 *
 * This method is necessary for custom physics objects that are composed
 * of multiple scene graph nodes.  In this case, it is because we
 * manage our own afterburner animations.
 */
void ButtonSwitchModel::resetSceneNode() {
    PolygonNode* pnode = dynamic_cast<PolygonNode*>(_node);
    if (pnode != nullptr) {
        // We need to know the content scale for resolution independence
        // If the device is higher resolution than 1024x576, Cocos2d will scale it
        // THIS DOES NOT FIX ASPECT RATIO PROBLEMS
        // If you are using a device with a 3:2 aspect ratio, you will need to
        // completely redo the level layout.  We can help if this is an issue.
        float cscale = Director::getInstance()->getContentScaleFactor();
        float scale = _isSwitch ? SWITCH_SCALE : BUTTON_SCALE;
        
        Rect bounds;
        bounds.size = pnode->getContentSize();
        
        pnode->setPolygon(bounds);
        pnode->setScale(cscale * scale);
        
        setDimension(pnode->getContentSize().width * scale / _drawScale.x,
                     pnode->getContentSize().height * scale / _drawScale.y);
        
        updateTexture();
    }
}

#pragma mark -
#pragma mark Collision Handling

void ButtonSwitchModel::handleContact()  {
    if (!_isPressed) {
        _isPressed = true;
        updateTexture();
        for(auto it = _linkedPlatforms.begin(); it != _linkedPlatforms.end(); ++it) {
            (*it)->toggle();
        }
    } else {
        _isPressed = false;
        updateTexture();
        for(auto it = _linkedPlatforms.begin(); it != _linkedPlatforms.end(); ++it) {
            (*it)->toggle();
        }
    }
}

void ButtonSwitchModel::handleEndContact() {
    if (!_isSwitch) {
        _isPressed = false;
        updateTexture();
        for(auto it = _linkedPlatforms.begin(); it != _linkedPlatforms.end(); ++it) {
            (*it)->toggle();
        }
    }
}

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