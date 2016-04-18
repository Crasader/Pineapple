//
//  LevelSelectController.cpp
//  PlatformerDemo
//
//  This is the most important class in this demo.  This class manages the gameplay
//  for this demo.  It also handles collision detection. There is not much to do for
//  collisions; our WorldController class takes care of all of that for us.  This
//  controller mainly transforms input into gameplay.
//
//  WARNING: There are a lot of shortcuts in this design that will do not adapt well
//  to data driven design.  This demo has a lot of simplifications to make it a bit
//  easier to see how everything fits together.  However, the model classes and how
//  they are initialized will need to be changed if you add dynamic level loading.
//
//  This file is based on the CS 3152 PhysicsDemo Lab by Don Holden, 2007
//
//  Author: Walker White
//  Version: 1/15/15
//
// This is the root, so there are a lot of includes
#include <cornell.h>
#include "LevelSelectController.h"
#include "CollisionController.h"
#include "Const.h"
#include "Texture.h"
#include "Levels.h"

#define LEVELS_CREATED              2
const string LevelSelectController::LEVEL_FILES[NUM_LEVELS] = {LEVEL_ONE_FILE, LEVEL_TWO_FILE};
const string LevelSelectController::LEVEL_KEYS[NUM_LEVELS] = {LEVEL_ONE_KEY, LEVEL_TWO_KEY};

/** Level Select Z indexes */
#define LEVEL_SELECT_BACKGROUND_Z   1
#define LEVEL_SELECT_BUTTON_Z       2
#define LEVEL_SELECT_TEXT_Z         3

/** Button texture paths */
#define LEVEL_SELECT_BUTTON_OFF_FILEPATH  "textures/buttons/level_button.png"
#define LEVEL_SELECT_BUTTON_ON_FILEPATH  "textures/buttons/level_button_inverse.png"

/** Graphics scaling constants for button layout */
#define BUTTONS_PER_ROW_TOP         5
#define BUTTONS_PER_ROW_MIDDLE      5
#define BUTTONS_PER_ROW_BOTTOM      3

#define LEVEL_SELECT_TOP_MARGIN     100
#define BUTTON_WIDTH_MARGIN         0.3f //As a percentage of button width, distributed to both sides
#define BUTTON_HEIGHT_MARGIN        0.3f //As a percentage of button width, distributed to both sides
#define BUTTON_FONT_SIZE            38


//using namespace std;


#pragma mark -
#pragma mark Initialization

/**
 * Creates a new game world with the default values.
 *
 * This constructor does not allocate any objects or start the controller.
 * This allows us to use a controller without a heap pointer.
 */
LevelSelectController::LevelSelectController() :
_worldnode(nullptr),
_debugnode(nullptr),
_backgroundNode(nullptr),
_world(nullptr),
_levelSelected(NO_LEVEL_SELECTED),
_debug(false){}

/**
 * Initializes the controller contents, and starts the game
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * The game world is scaled so that the screen coordinates do not agree
 * with the Box2d coordinates.  This initializer uses the default scale.
 *
 * @retain a reference to the root layer
 * @return  true if the controller is initialized properly, false otherwise.
 */
bool LevelSelectController::init(Node* root, InputController* input) {
    return init(root,input,SCREEN);
}

Button* LevelSelectController::initButton(Size dimen, int i) {
    Button* button = Button::create();
    button->loadTextureNormal(LEVEL_SELECT_BUTTON_OFF_FILEPATH);
    button->loadTexturePressed(LEVEL_SELECT_BUTTON_ON_FILEPATH);
    button->setAnchorPoint(Vec2(0.5f, 0.5f));
    
    float row = i / BUTTONS_PER_ROW_TOP;
    float col;
    if (row == 0) {
        col = i % BUTTONS_PER_ROW_TOP;
    } else if (row == 1) {
        col = (i - BUTTONS_PER_ROW_TOP) % BUTTONS_PER_ROW_MIDDLE;
    } else {
        col = ((i - BUTTONS_PER_ROW_TOP - BUTTONS_PER_ROW_MIDDLE) % BUTTONS_PER_ROW_BOTTOM) + 1;
    }
    
    row += 0.5f;
    col += 0.6f;
    
    int w = button->getContentSize().width * (1 + BUTTON_WIDTH_MARGIN);
    int h = button->getContentSize().height * (1 + BUTTON_HEIGHT_MARGIN);
    
    button->setPosition(Vec2(w * col ,dimen.height - (h * row + LEVEL_SELECT_TOP_MARGIN)));
    button->setVisible(true);
    button->setTag(i);
    
    button->setTitleText("Lvl " + cocos2d::to_string(i + 1));
    button->setTitleFontSize(BUTTON_FONT_SIZE);
    button->setTitleFontName(ELECTRIC_CIRCUS_FONT_LOCATION);
    button->setTitleColor(Color3B::WHITE);
    
    button->setEnabled(i < LEVELS_CREATED);
    
    button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::BEGAN) {
            ((Button*)sender)->setTitleColor(Color3B::BLACK);
        } else if (type == ui::Widget::TouchEventType::CANCELED) {
            ((Button*)sender)->setTitleColor(Color3B::WHITE);
        } else if (type == ui::Widget::TouchEventType::ENDED) {
            _levelSelected = ((Node*)sender)->getTag();
            ((Button*)sender)->setTitleColor(Color3B::WHITE);
        }
    });
    
    button->retain();
    return button;
}

/**
 * Initializes the controller contents, and starts the game
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * The game world is scaled so that the screen coordinates do not agree
 * with the Box2d coordinates.  The bounds are in terms of the Box2d
 * world, not the screen.
 *
 * @param bounds The game bounds in Box2d coordinates
 * @param scale  The difference between screen and Box2d coordinates
 * @param gravity The gravitational force on this Box2d world
 *
 * @retain a reference to the root layer
 * @return  true if the controller is initialized properly, false otherwise.
 */
bool LevelSelectController::init(Node* root, InputController* input, const Rect& rect) {
    _assets = AssetManager::getInstance()->getCurrent();
    _input = input;
    
    // Determine the center of the screen
    Size dimen  = root->getContentSize();
    Vec2 center(dimen.width/2.0f,dimen.height/2.0f);
    
    // Create the scale and notify the input handler
    Vec2 scale = Vec2(root->getContentSize().width/rect.size.width,
                      root->getContentSize().height/rect.size.height);
    Rect screen = rect;
    screen.origin.x *= scale.x;    screen.origin.y *= scale.y;
    screen.size.width *= scale.x;  screen.size.height *= scale.y;
    
    // Create the scene graph
    _debugnode = _debugnode = Node::create();
    
    _rootnode = root;
    _rootnode->retain();
    
    //Create the background image
    Texture2D* image = _assets->get<Texture2D>(LEVEL_SELECT_BACKGROUND);
    _backgroundNode = PolygonNode::createWithTexture(image);
    _backgroundNode->setPosition(center);
    _backgroundNode->setAnchorPoint(Vec2(0.5f, 0.5f));
    _backgroundNode->setScale(dimen.width/image->getContentSize().width, dimen.height/image->getContentSize().height);
    
    _rootnode->addChild(_backgroundNode, LEVEL_SELECT_BACKGROUND_Z);
    
    
    //Lay out the buttons
    for(int i = 0; i < NUM_LEVELS; i++) {
        _buttons[i] = initButton(dimen, i);
        _rootnode->addChild(_buttons[i], LEVEL_SELECT_BUTTON_Z);
    }
    
    _isInitted = true;
    setDebug(false);
    
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 *
 * This method is different from dispose() in that it ALSO deletes the
 * input controller.
 */
LevelSelectController::~LevelSelectController() {
    dispose();
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void LevelSelectController::dispose() {
    for(int i = 0; i < NUM_LEVELS; i++) {
        if (_buttons[i] != nullptr) {
            _buttons[i]->release();
            _buttons[i] = nullptr;
        }
    }
    _backgroundNode = nullptr;
    _worldnode = nullptr;
    _debugnode = nullptr;
    if(_rootnode != nullptr) {
        _rootnode->removeAllChildren();
        _rootnode->release();
    }
    _rootnode = nullptr;
}

#pragma mark -
#pragma mark Gameplay Handling

/**
 * Executes the core gameplay loop of this world.
 *
 * This method contains the specific update code for this mini-game. It does
 * not handle collisions, as those are managed by the parent class WorldController.
 * This method is called after input is read, but before collisions are resolved.
 * The very last thing that it should do is apply forces to the appropriate objects.
 *
 * @param  delta    Number of seconds since last animation frame
 */
void LevelSelectController::update(float dt) {
    
    _input->update(dt);
    
    if (_levelSelected != NO_LEVEL_SELECTED) {
        setTransitionStatus(TRANSITION_TO_GAME);
        return;
    }
}
