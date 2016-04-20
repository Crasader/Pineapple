//
//  InputController.cpp
//  PlatformerDemo
//
//  This input controller is primarily designed for keyboard control.  On mobile
//  you will notice that we use gestures to emulate keyboard commands. They even
//  use the same variables (though we need other variables for internal keyboard
//  emulation).  This simplifies our design quite a bit.
//
//  This file is based on the CS 3152 PhysicsDemo Lab by Don Holden, 2007
//
//  Author: Walker White
//  Version: 1/15/15
//
#include "InputController.h"


#pragma mark -
#pragma mark Input Settings
/** The key to use for reseting the game */
#define RESET_KEY EventKeyboard::KeyCode::KEY_R
/** The key for toggling the debug display */
#define DEBUG_KEY EventKeyboard::KeyCode::KEY_D
/** The key for exitting the game */
#define EXIT_KEY  EventKeyboard::KeyCode::KEY_ESCAPE
/** The key for firing a bullet */
#define FIRE_KEY EventKeyboard::KeyCode::KEY_SPACE
/** The key for jumping up */
#define JUMP_KEY EventKeyboard::KeyCode::KEY_UP_ARROW
/** The key for grow is A */
#define GROW_KEY EventKeyboard::KeyCode::KEY_A
/** The key for shrink is S */
#define SHRINK_KEY EventKeyboard::KeyCode::KEY_S
/** The key for pause is <ESC> */
#define PAUSE_KEY EventKeyboard::KeyCode::KEY_P

/** How fast a double click must be in milliseconds */
#define EVENT_DOUBLE_CLICK  300
/** How fast we must swipe left or right for a gesture */
#define EVENT_SWIPE_TIME    1000
/** How far we must swipe left or right for a gesture (as ratio of screen) */
#define EVENT_SWIPE_LENGTH  0.05f

// The screen is divided into four zones: Left, Bottom, Right and Main/
// These are all shown in the diagram below.
//
//   |---------------|
//   |   |       |   |
//   | L |   M   | R |
//   |   |       |   |
//   -----------------
//   |       B       |
//   -----------------
//
// The meaning of any touch depends on the zone it begins in.

/** The portion of the screen used for the left zone */
#define LEFT_ZONE       0.15f
/** The portion of the screen used for the right zone */
#define RIGHT_ZONE      0.15f
/** The portion of the screen used for the bottom zone */
#define BOTTOM_ZONE     0.2f


#pragma mark -
#pragma mark Input Controller
/**
 * Creates a new input controller.
 *
 * This constructor does NOT do any initialzation.  It simply allocates the
 * object. This makes it safe to use this class without a pointer.
 */
InputController::InputController() :
_active(false),
_resetPressed(false),
_debugPressed(false),
_exitPressed(false),
_touchListener(nullptr),
_previousDelta(0),
_id1(-1), _touch1(-1, -1), _time1(current_time()),
_id2(-1), _touch2(-1, -1), _time2(current_time()),
_prevTap(-1,-1)
{
    _keyReset = false;
    _keyDebug = false;
    _keyExit  = false;
    
    _horizontal = 0.0f;
    _keyFire  = false;
    _keyJump  = false;
    _keyGrow = false;
    _keyShrink = false;
    _keyPause = false;
    
    // Initialize the touch values.
    _ltouch.touchid = -1;
    _rtouch.touchid = -1;
    _btouch.touchid = -1;
    _mtouch.touchid = -1;
    _ltouch.count = 0;
    _rtouch.count = 0;
    _btouch.count = 0;
    _mtouch.count = 0;
}

/**
 * Disposes of this input controller, releasing all listeners.
 */
InputController::~InputController() {
    if (_touchListener != nullptr) {
        _touchListener->release();
        _touchListener = nullptr;
    }
}

/**
 * Initializes the input control for the given drawing scale and bounds.
 *
 * This method works like a proper constructor, initializing the input
 * controller and allocating memory.  However, it still does not activate
 * the listeners.  You must call start() do that.
 *
 * The meaning of touch events depends on the screen size. The parameter
 * bounds allows the input controller to identify the bounds of the screen.
 *
 * @param  bounds   the bounds of the touch device
 *
 * @return true if the controller was initialized successfully
 */
bool InputController::init(const Rect& bounds) {
    _bounds = bounds;
    createZones();
    
    _swipetime = current_time();
    _dbtaptime = current_time();
    // Create the touch listener. This is an autorelease object.
    _touchListener = MultiTouchListener::create();
    if (_touchListener != nullptr) {
        _touchListener->retain();
        _touchListener->onTouchesBegan = [this](std::vector<Touch*> touches, timestamp_t time) {
            return touchesBeganCB(touches,time);
        };
        _touchListener->onTouchesMoved = [=](std::vector<Touch*> touches, timestamp_t time) {
            return this->touchesMovedCB(touches,time);
        };
        _touchListener->onTouchesEnded = [=](std::vector<Touch*> touches, timestamp_t time) {
            return this->touchesEndedCB(touches,time);
        };
        _touchListener->onTouchesCancelled = [=](std::vector<Touch*> touches, timestamp_t time) {
            return this->touchesCancelCB(touches,time);
        };
        return true;
    }
    return false;
}

/**
 * Starts the input processing for this input controller.
 *
 * This method must be called AFTER the input controller is initialized
 */
void InputController::start() {
    if (!_active) {
        _active = true;
        // Prioritize the input
        // BUG: Cocos2D always prioritizes touch
        KeyboardPoller::start(1);
        _touchListener->start(2);
    }
}

/**
 * Stops the input processing for this input controller.
 *
 * This method will not dispose of the input controller. It can be restarted.
 */
void InputController::stop() {
    if (_active) {
        _active = false;
        _touchListener->stop();
        KeyboardPoller::stop();
    }
}


#pragma mark -
#pragma mark Input Handling

/**
 * Processes the currently cached inputs.
 *
 * This method is used to to poll the current input state.  This will poll the
 * keyboad and accelerometer.
 *
 * This method also gathers the delta difference in the touches. Depending on
 * the OS, we may see multiple updates of the same touch in a single animation
 * frame, so we need to accumulate all of the data together.
 */
void InputController::update(float dt) {
    if (!_active) {
        return;
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    // DESKTOP CONTROLS
    KeyboardPoller* keys = KeyboardPoller::getInstance();
    keys->update();
    
    // Map "keyboard" events to the current frame boundary
    _keyReset  = keys->keyPressed(RESET_KEY);
    _keyDebug  = keys->keyPressed(DEBUG_KEY);
    _keyExit   = keys->keyPressed(EXIT_KEY);
    
    _keyFire   = keys->keyPressed(FIRE_KEY);
    _keyJump   = keys->keyPressed(JUMP_KEY);
    _keyGrow   = keys->keyPressed(GROW_KEY);
    _keyShrink = keys->keyPressed(SHRINK_KEY);
    _keyPause  = keys->keyPressed(PAUSE_KEY);
    
    _keyLeft  = keys->keyDown(EventKeyboard::KeyCode::KEY_LEFT_ARROW);
    _keyRight = keys->keyDown(EventKeyboard::KeyCode::KEY_RIGHT_ARROW);
#endif
    // Nothing to do for MOBILE CONTROLS

    // Capture the current state
    _resetPressed  = _keyReset;
    _debugPressed  = _keyDebug;
    _exitPressed   = _keyExit;
    _firePressed   = _keyFire;
    _jumpPressed   = _keyJump;
    _growPressed   = _keyGrow;
    _shrinkPressed = _keyShrink;
    _pausePressed  = _keyPause;
    
    // Directional controls
    _horizontal = 0.0f;
    if (_keyRight) {
        _horizontal += 1.0f;
    }
    if (_keyLeft) {
        _horizontal -= 1.0f;
    }
  
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // Need to clear keys in the mobile state
    _keyDebug  = false;
    _keyReset  = false;
    _keyExit   = false;
    _keyJump   = false;
    _keyFire   = false;
    _keyGrow   = false;
    _keyShrink = false;
    _keyPause  = false;
#endif
}


/**
 * Defines the zone boundaries, so we can quickly categorize touches.
 */
void InputController::createZones() {
    _lzone = _bounds;
    _lzone.size.width *= LEFT_ZONE;
    _rzone = _bounds;
    _rzone.size.width *= RIGHT_ZONE;
    _rzone.origin.x = _bounds.origin.x+_bounds.size.width-_rzone.size.width;
    _bzone = _bounds;
    _bzone.size.height *= BOTTOM_ZONE;
}

/**
 * Returns the correct zone for the given position.
 *
 * See the comments above for a description of how zones work.
 *
 * @param  pos  a position in screen coordinates
 *
 * @return the correct zone for the given position.
 */
InputController::Zone InputController::getZone(const Vec2& pos) {
    if (_lzone.containsPoint(pos)) {
        return Zone::LEFT;
    } else if (_rzone.containsPoint(pos)) {
        return Zone::RIGHT;
    } else if (_bzone.containsPoint(pos)) {
        return Zone::BOTTOM;
    } else if (_bounds.containsPoint(pos)) {
        return Zone::MAIN;
    }
    return Zone::UNDEFINED;
}

/** 
 * NOT CURRENTLY USED - SWITCHED TO TAPS
 *
 * Returns true if this is a jump swipe.
 *
 * A jump swipe is a quick swipe on left or right section.
 *
 * @param  start    the start position of the candidate swipe
 * @param  stop     the end position of the candidate swipe
 * @param  current  the current timestamp of the gesture
 *
 * @return true if this is a jump swipe.
 */
bool InputController::checkJump(const int id, const Vec2& loc) {
    // Look for swipes up that are "long enough"
    Vec2 prev;
    if (id == _id1) {
        prev = _touch1;
    } else if (id == _id2) {
        prev = _touch2;
    } else {
        std::cout << "SOMETHING WENT WRONG IN CHECK JUMP";
        return false;
    }
    return (prev.y < loc.y && loc.y - prev.y > MIN_SWIPE_SPEED);
}

/**
 * Returns a nonzero value if this is a quick left or right swipe
 *
 * The function returns -1 if it is left swipe and 1 if it is a right swipe.
 *
 * @param  start    the start position of the candidate swipe
 * @param  stop     the end position of the candidate swipe
 * @param  current  the current timestamp of the gesture
 *
 * @return a nonzero value if this is a quick left or right swipe
 */
int InputController::checkSwipe(const Vec2& start, const Vec2& stop, timestamp_t current) {
    // Look for swipes up that are "long enough"
    float xdiff = (stop.x-start.x);
    if (elapsed_millis(_swipetime,current) < EVENT_SWIPE_TIME) {
        float thresh = EVENT_SWIPE_LENGTH*_bounds.size.width;
        if (xdiff > thresh) {
            return 1;
        } else if (xdiff < thresh) {
            return -1;
        }
    }
    return 0;
}


#pragma mark -
#pragma mark Touch Callbacks
/**
 * Callback for the beginning of a touch event
 *
 * @param t     The touch information
 * @param event The associated event
 *
 * @return True if the touch was processed; false otherwise.
 */
bool InputController::touchesBeganCB(std::vector<Touch*> touches, timestamp_t current) {
    for (std::vector<Touch*>::iterator i = touches.begin(); i != touches.end(); i++) {
        Touch* t = *i;
        // double tap is jump
        if (_prevTap.distance(t->getLocation()) <= TAP_RADIUS && (elapsed_millis(_dbtaptime,current) <= EVENT_DOUBLE_CLICK)) {
            _keyJump = true;
        }
        if (_id1 != -1) {
            // we already have a finger down, potential gesture
            _touch2 = t->getLocation();
            if (_touch2 > _touch1) {
                _id2 = t->getID();
                _time2 = current_time();
            } else {
                Vec2 temp = _touch1;
                _touch1 = _touch2;
                _touch2 = temp;
                _id2 = _id1;
                _id1 = t->getID();
                _time1 = current_time();
            }
            _previousDelta = _touch1.distance(_touch2);
            break;
        }
        _touch1 = t->getLocation();
        _id1 = t->getID();
        _time1 = current_time();
        Vec2 pos = t->getLocation();
        Zone zone = getZone(pos);
        switch (zone) {
            case Zone::LEFT:
                CCLOG("Zone left");
                // Only process if no touch in zone
                if (_ltouch.touchid == -1) {
                    _ltouch.position = pos;
                    _ltouch.touchid = t->getID();
                    // Cannot do both.
                    // only move if one finger down
                    if (oneFingerDown()) {
                        _keyLeft = _rtouch.touchid == -1;
                    }
                }
                break;
            case Zone::RIGHT:
                CCLOG("Zone right");
                // Only process if no touch in zone
                if (_rtouch.touchid == -1) {
                    _rtouch.position = pos;
                    _rtouch.touchid = t->getID();
                    if (oneFingerDown()) {
                        _keyRight = _ltouch.touchid == -1;
                    }
                }
                break;
            case Zone::BOTTOM:
                CCLOG("Zone bottom");
                // Only process if no touch in zone
                if (_btouch.touchid == -1) {
                    _btouch.position = pos;
                    _btouch.touchid = t->getID();
                }
                _keyFire = true;
                break;
            case Zone::MAIN:
                // Only check for double tap in Main if nothing else down
                if (_ltouch.touchid == -1 && _rtouch.touchid == -1 && _btouch.touchid == -1 && _mtouch.touchid == -1) {
                    _keyDebug = (elapsed_millis(_dbtaptime,current) <= EVENT_DOUBLE_CLICK);
                }
                
                // Keep count of touches in Main zone.
                if (_mtouch.touchid == -1) {
                    _mtouch.position = pos;
                    _mtouch.touchid = t->getID();
                }
                _mtouch.count++;
                break;
            default:
                CCASSERT(false, "Touch is out of bounds");
                break;
        }
        _swipetime = current;
    }
    _prevTap = touches[0]->getLocation();
    _dbtaptime = current;
    return true;
}

/**
 * Callback for the end of a touch event
 *
 * @param t     The touch information
 * @param event The associated event
 */
void InputController::touchesEndedCB(std::vector<Touch*> touches, timestamp_t current) {
    for (std::vector<Touch*>::iterator i = touches.begin(); i != touches.end(); i++) {
        Touch* t = *i;
        if (t->getID() == _id1) {
            _id1 = _id2;
            _touch1 = _touch2;
            _id2 = -1;
            _previousDelta = 0;
        } else if (t->getID() == _id2) {
            _id2 = -1;
            _previousDelta = 0;
        }
        // Reset all keys that might have been set
        if (_ltouch.touchid == t->getID()) {
            _ltouch.touchid = -1;
            _ltouch.count = 0;
            _keyLeft = false;
        } else if (_rtouch.touchid == t->getID()) {
            _rtouch.touchid = -1;
            _rtouch.count = 0;
            _keyRight = false;
        } else if (_btouch.touchid == t->getID()) {
            _btouch.touchid = -1;
            _btouch.count = 0;
            // Fire is made false by update
        } else if (_mtouch.touchid == t->getID()) {
            _mtouch.count--;
            if (_mtouch.count == 0) {
                _mtouch.touchid = -1;
            }
            // Reset, debug is made false by update
        }
    }
}


/**
 * Callback for a touch movement event
 *
 * @param t     The touch information
 * @param event The associated event
 */
void InputController::touchesMovedCB(std::vector<Touch*> touches, timestamp_t current) {
    // process gesture recognition
    for (std::vector<Touch*>::iterator i = touches.begin(); i != touches.end(); i++) {
        Touch* t = *i;
        if (t->getID() == _id1) {
            _touch1 = t->getLocation();
        } else if (t->getID() == _id2) {
            _touch2 = t->getLocation();
        }
    }
    if (_id1 != -1 && _id2 != -1) {
        int pinch_spread = checkPinchSpread();
        if (pinch_spread == PINCH) {
            _keyShrink = true;
        } else if (pinch_spread == SPREAD) {
            _keyGrow = true;
        }
    }
    _previousDelta = _touch1.distance(_touch2);
}

/**
 * Callback for the cancellation of a touch event
 *
 * Cancellation occurs when an external event—for example,
 * an incoming phone call—disrupts the current app’s event
 * processing.
 *
 * @param t     The touch information
 * @param event The associated event
 */
void InputController::touchesCancelCB(std::vector<Touch*> touches, timestamp_t current) {
    _id1 = -1;
    _id2 = -1;
    _previousDelta = 0;
    // Update the timestamp
    _dbtaptime = current;
    _swipetime = current;
    _ltouch.touchid = -1;
    _rtouch.touchid = -1;
    _btouch.touchid = -1;
    _mtouch.touchid = -1;
    _ltouch.count = 0;
    _rtouch.count = 0;
    _btouch.count = 0;
    _mtouch.count = 0;

}
