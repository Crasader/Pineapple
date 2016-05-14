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

const string LevelSelectController::LEVEL_FILES[NUM_LEVELS] = {LEVEL_ONE_FILE, LEVEL_TWO_FILE, LEVEL_THREE_FILE,
    LEVEL_FOUR_FILE, LEVEL_FIVE_FILE, LEVEL_SIX_FILE, LEVEL_SEVEN_FILE, LEVEL_EIGHT_FILE,
    LEVEL_NINE_FILE, LEVEL_TEN_FILE, LEVEL_ELEVEN_FILE, LEVEL_TWELVE_FILE, LEVEL_THIRTEEN_FILE,
    LEVEL_FOURTEEN_FILE, LEVEL_FIFTEEN_FILE};
const string LevelSelectController::LEVEL_KEYS[NUM_LEVELS] = {LEVEL_ONE_KEY, LEVEL_TWO_KEY, LEVEL_THREE_KEY,
    LEVEL_FOUR_KEY, LEVEL_FIVE_KEY, LEVEL_SIX_KEY, LEVEL_SEVEN_KEY, LEVEL_EIGHT_KEY,
    LEVEL_NINE_KEY,LEVEL_TEN_KEY, LEVEL_ELEVEN_KEY, LEVEL_TWELVE_KEY, LEVEL_THIRTEEN_KEY,
    LEVEL_FOURTEEN_KEY, LEVEL_FIFTEEN_KEY};

/** Level Select Z indexes */
#define LEVEL_SELECT_BACKGROUND_Z   1
#define LEVEL_SELECT_BUTTON_Z       2
#define LEVEL_SELECT_TEXT_Z         3
#define LEVEL_SELECT_PAGINATION_Z   4

/** Graphics scaling constants for button layout */
#define BUTTONS_PER_ROW             5

#define LEVEL_SELECT_TOP_MARGIN     100
#define BUTTON_WIDTH_MARGIN         0.05f //As a percentage of button width, distributed to both sides
#define BUTTON_HEIGHT_MARGIN        0.3f //As a percentage of button width, distributed to both sides
#define BUTTON_SCALE                0.8f
#define BUTTON_FONT_SIZE            38

#define LEVELS_PER_PAGE             15


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
_debug(false),
_currentPage(0){}

/** 
 * Returns the maximum level page - the maximum number of pages needed to
 * show all of the levels 
 */
int LevelSelectController::getMaxPage() {
    return NUM_LEVELS / LEVELS_PER_PAGE;
}

/** Return the page that button i goes on */
int LevelSelectController::getPage(int buttonIndex) {
    return buttonIndex / LEVELS_PER_PAGE;
}

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

// helper to get score texture filepaths
const string getScoreTexture(int score) {
    switch (score) {
        case 0:
            return LEVEL_SELECT_SCORE_FILEPATH_0;
        case 1:
            return LEVEL_SELECT_SCORE_FILEPATH_1;
        case 2:
            return LEVEL_SELECT_SCORE_FILEPATH_2;
        case 3:
            return LEVEL_SELECT_SCORE_FILEPATH_3;
        case 4:
            return LEVEL_SELECT_SCORE_FILEPATH_4;
        default:
            CC_ASSERT(false);
            return "";
    }
}

Button* LevelSelectController::initButton(int i) {
    float cscale = Director::getInstance()->getContentScaleFactor();
    Button* button = Button::create();
    button->setScale(cscale * BUTTON_SCALE);
    button->loadTextureNormal(LEVEL_SELECT_BUTTON_OFF_FILEPATH);
    button->loadTexturePressed(LEVEL_SELECT_BUTTON_ON_FILEPATH);
    button->setAnchorPoint(Vec2(0.5f, 0.5f));
    
    initButtonTextOps(button);
    
    button->setTag(i);
    
    button->setTitleText("Lvl " + cocos2d::to_string(i + 1));
    button->setTitleFontSize(BUTTON_FONT_SIZE);
    
    button->setEnabled(i < LEVELS_CREATED && i <= _levelsComplete);
    
    button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            _levelSelected = ((Node*)sender)->getTag();
        }
    });

    button->retain();
    return button;
}

void LevelSelectController::fixPosition(Button *b, Node* score, int index) {
    float cscale = Director::getInstance()->getContentScaleFactor();
    int difPage = getPage(index) - _currentPage;
    index = index % LEVELS_PER_PAGE;
    
    float row = index / BUTTONS_PER_ROW;
    float col = index % BUTTONS_PER_ROW;
    
    row += 0.5f;
    
    int w = b->getContentSize().width * (1 + BUTTON_WIDTH_MARGIN) * cscale;
    int h = b->getContentSize().height * (1 + BUTTON_HEIGHT_MARGIN) * cscale;
    
    b->setPosition(Vec2(_dimen.width * difPage + _dimen.width/2 + w * (col - 2),_dimen.height - (h * row + LEVEL_SELECT_TOP_MARGIN)));
    score->setPosition(b->getPosition().x, b->getPosition().y + SCORE_V_OFFSET);
}

PolygonNode* LevelSelectController::initScore(Button* button, int i) {
    _scores[i] = UserDefault::getInstance()->getIntegerForKey(LevelSelectController::LEVEL_KEYS[i].c_str());
    PolygonNode* node = PolygonNode::createWithFile(getScoreTexture(_scores[i]));
    node->setTexture(getScoreTexture(_scores[i]));
    node->setScale(Director::getInstance()->getContentScaleFactor() * SCORE_SCALE);
    
    node->retain();
    return node;
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
    _dimen  = root->getContentSize();
    Vec2 center(_dimen.width/2.0f,_dimen.height/2.0f);
    
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
    _backgroundNode->setScale(_dimen.width/image->getContentSize().width, _dimen.height/image->getContentSize().height);
    
    _rootnode->addChild(_backgroundNode, LEVEL_SELECT_BACKGROUND_Z);
    
    
    //Get the number of levels completed, if it doesn't exist it returns 0, perfect!
    _levelsComplete = UserDefault::getInstance()->getIntegerForKey(LEVELS_COMPLETED_KEY);
    //Lay out the buttons and scores
    for(int i = 0; i < NUM_LEVELS; i++) {
        _buttons[i] = initButton(i);
        _scoreNodes[i] = initScore(_buttons[i], i);
        fixPosition(_buttons[i], _scoreNodes[i], i);
        _rootnode->addChild(_scoreNodes[i], LEVEL_SELECT_BUTTON_Z);
        _rootnode->addChild(_buttons[i], LEVEL_SELECT_BUTTON_Z);
    }
    
    float cscale = Director::getInstance()->getContentScaleFactor();
    float navScale = cscale * 0.4f;
    float navScaleX = navScale * 0.8f;
    float navXOffset = 65;
    
    _prevPageButton = Button::create();
    _prevPageButton->setScaleY(navScale);
    _prevPageButton->setScaleX(navScaleX);
    _prevPageButton->loadTextureNormal(LEVEL_SELECT_PREV_OFF_FILEPATH);
    _prevPageButton->loadTexturePressed(LEVEL_SELECT_PREV_ON_FILEPATH);
    _prevPageButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    
    _prevPageButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            pageDown();
        }
    });
    _prevPageButton->setEnabled(false);
    
    _prevPageButton->retain();
    int w = _prevPageButton->getContentSize().width * (1 + BUTTON_WIDTH_MARGIN) * cscale;
    int h = _prevPageButton->getContentSize().height * (1 + BUTTON_HEIGHT_MARGIN) * cscale;
    
    _prevPageButton->setPosition(Vec2(navXOffset, _dimen.height/2));
    _rootnode->addChild(_prevPageButton, LEVEL_SELECT_PAGINATION_Z);
    
    _nextPageButton = Button::create();
    _nextPageButton->setScaleY(navScale);
    _nextPageButton->setScaleX(navScaleX);
    _nextPageButton->loadTextureNormal(LEVEL_SELECT_NEXT_OFF_FILEPATH);
    _nextPageButton->loadTexturePressed(LEVEL_SELECT_NEXT_ON_FILEPATH);
    _nextPageButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    
    _nextPageButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            pageUp();
        }
    });
    
    _nextPageButton->setEnabled(NUM_LEVELS > LEVELS_PER_PAGE);
    _nextPageButton->retain();
    w = _nextPageButton->getContentSize().width * (1 + BUTTON_WIDTH_MARGIN) * navScale;
    h = _nextPageButton->getContentSize().height * (1 + BUTTON_HEIGHT_MARGIN) * navScale;
    
    _nextPageButton->setPosition(Vec2(_dimen.width - navXOffset, _dimen.height/2));
    _rootnode->addChild(_nextPageButton, LEVEL_SELECT_PAGINATION_Z);
    
    _isInitted = true;
    setDebug(false);
    
    return true;
}

void LevelSelectController::update() {
    int newLevelsComplete = UserDefault::getInstance()->getIntegerForKey(LEVELS_COMPLETED_KEY);
    _levelsComplete = newLevelsComplete;
    // update scores
    for(int i = 0; i < NUM_LEVELS; i++) {
        int newScore = UserDefault::getInstance()->getIntegerForKey(LEVEL_KEYS[i].c_str());
        // scores are only udpated if greater, so don't need to check that
        if (newScore != _scores[i]) {
            _scores[i] = newScore;
            _rootnode->removeChild(_scoreNodes[i]);
            _scoreNodes[i]->release();
            _scoreNodes[i] = initScore(_buttons[i], i);
            _rootnode->addChild(_scoreNodes[i], LEVEL_SELECT_BUTTON_Z);
            fixPosition(_buttons[i], _scoreNodes[i], i);
        }
    }
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
    _backgroundNode = nullptr;
    _worldnode = nullptr;
    _debugnode = nullptr;
    if(_rootnode != nullptr) {
        _rootnode->removeAllChildren();
        _rootnode->release();
    }
    _rootnode = nullptr;
    
    for(int i = 0; i < NUM_LEVELS; i++) {
        if (_buttons[i] != nullptr) {
            _buttons[i]->release();
            _buttons[i] = nullptr;
        }
        if (_scoreNodes[i] != nullptr) {
            _scoreNodes[i]->release();
            _scoreNodes[i] = nullptr;
        }
    }
    
    if (_prevPageButton != nullptr) {
        _prevPageButton->release();
        _prevPageButton = nullptr;
    }
    
    if (_nextPageButton != nullptr) {
        _nextPageButton->release();
        _nextPageButton = nullptr;
    }
}

#pragma mark -
#pragma mark Gameplay Handling

bool LevelSelectController::pageUp() {
    if (_currentPage == getMaxPage()) {
        return false;
    }
    
    _currentPage++;
    for(int i = 0; i < NUM_LEVELS; i++) {
        fixPosition(_buttons[i], _scoreNodes[i], i);
    }
    
    _prevPageButton->setEnabled(_currentPage > 0);
    _nextPageButton->setEnabled(_currentPage < getMaxPage());
    
    return true;
}

bool LevelSelectController::pageDown() {
    if (_currentPage == 0) {
        return false;
    }
    
    _currentPage--;
    for(int i = 0; i < NUM_LEVELS; i++) {
        fixPosition(_buttons[i], _scoreNodes[i], i);
    }
    
    _prevPageButton->setEnabled(_currentPage > 0);
    _nextPageButton->setEnabled(_currentPage < getMaxPage());
    return true;
}

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
