//
//  ButtonSwitchModel.h
//  Pineapple
//

#ifndef ButtonSwitchModel_h
#define ButtonSwitchModel_h

#include <cornell/CUBoxObstacle.h>
#include <cornell/CUWireNode.h>
#include "Const.h"
#include "Texture.h"
#include "MoveablePlatformModel.h"
#include <stdio.h>

using namespace cocos2d;

#define BUTTON_SWITCH_NAME       "button_switch"

#define BUTTON_SCALE         1.0f
#define SWITCH_SCALE         1.0f

class ButtonSwitchModel : public BoxObstacle {
private:
    /** This macro disables the copy constructor (not allowed on physics objects) */
    CC_DISALLOW_COPY_AND_ASSIGN(ButtonSwitchModel);
    
protected:
    // true if switch, false if button
    bool isSwitch;
    // true if button is pressed or level is switched on
    bool isPressed;
    // never has ownership
    MoveablePlatformModel* linkedPlatform;
    
    /**
     * Redraws the outline of the physics fixtures to the debug node
     *
     * The debug node is use to outline the fixtures attached to this object.
     * This is very useful when the fixtures have a very different shape than
     * the texture (e.g. a circular shape attached to a square texture).
     */
    virtual void resetDebugNode() override;
    
public:
#pragma mark Static Constructors
    
    /**
     * Creates a new ButtonSwitchModel at the given position.
     *
     * The ButtonSwitchModel is sized according to the given drawing scale.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos      Initial position in world coordinates
     * @param  scale    The drawing scale
     *
     * @return  An autoreleased physics object
     */
    static ButtonSwitchModel* create(const Vec2& pos, const Vec2& scale, const bool isSwitch);
    
    
#pragma mark Physics Methods
    /**
     * Creates the physics Body(s) for this object, adding them to the world.
     *
     * This method overrides the base method to keep your ship from spinning.
     *
     * @param world Box2D world to store body
     *
     * @return true if object allocation succeeded
     */
    void createFixtures() override;
    
    /**
     * Release the fixtures for this body, reseting the shape
     *
     * This is the primary method to override for custom physics objects.
     */
    void releaseFixtures() override;
    
    /**
     * Updates the object's physics state (NOT GAME LOGIC).
     *
     * We use this method to reset cooldowns.
     *
     * @param delta Number of seconds since last animation frame
     */
    void update(float dt) override;
    
    void handleContact() {
        if (!isPressed) {
            isPressed = true;
            if (linkedPlatform != nullptr) {
                linkedPlatform->open();
            }
        } else {
            isPressed = false;
            if (linkedPlatform != nullptr) {
                linkedPlatform->close();
            }
        }
    }
    
    void handleEndContact() {
        if (!isSwitch) {
            isPressed = false;
            if (linkedPlatform != nullptr) {
                linkedPlatform->close();
            }
        }
    }
    
CC_CONSTRUCTOR_ACCESS:
#pragma mark Hidden Constructors
    /**
     * Creates a degenerate button/switch object.
     *
     * This constructor does not initialize any of the ButtonSwitchModel values beyond
     * the defaults.  To use a ButtonSwitchModel, you must call init().
     */
    ButtonSwitchModel() : BoxObstacle(), isSwitch(false), isPressed(false), linkedPlatform(nullptr) { }
    
    virtual bool init(const Vec2& pos, const Vec2& scale, const bool isSwitch);
};


#endif /* ButtonSwitchModel_h */
