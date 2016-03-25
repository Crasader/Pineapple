//
//  BackgroundView.cpp
//  Pineapple
//
//  Created by Michael Patashnik on 3/24/16.
//
//

#include "BackgroundView.h"

/** Scale factor for background images */
#define FRONT_BACKGROUND_SCALE   1.1f
#define MIDDLE_BACKGROUND_SCALE  1.1f
#define BACK_BACKGROUND_SCALE    1.0f
/** The width of background assets */
#define FRONT_BACKGROUND_WIDTH   1000.0f
#define MIDDLE_BACKGROUND_WIDTH  1000.0f
#define BACK_BACKGROUND_WIDTH    1500.0f
/** Height of background assets */
#define FRONT_BACKGROUND_HEIGHT  500.0f
#define MIDDLE_BACKGROUND_HEIGHT 300.0f
#define BACK_BACKGROUND_HEIGHT   300.0f
/** Vertical offset of background assets */
#define FRONT_BACKGROUND_VERTICAL_OFFSET  -190.0f
#define MIDDLE_BACKGROUND_VERTICAL_OFFSET  120.0f
#define BACK_BACKGROUND_VERTICAL_OFFSET    170.0f
/** Damping factor for parallax scrolling */
#define HILLS_DAMPING_FACTOR     4.0f
#define CLOUDS_DAMPING_FACTOR    6.0f
/** Cloud velocity */
#define CLOUD_VELOCITY           0.05f

BackgroundView* BackgroundView::createAndAddTo(Node* rootNode, Node* worldNode, Node* debugNode, SceneManager *assets) {
    BackgroundView* view = new (std::nothrow) BackgroundView();
    view->_hillsnode = Node::create();
    view->_cloudsnode = Node::create();
    view->init(rootNode, worldNode, debugNode, assets);
    return view;
}

void BackgroundView::init(Node* rootNode, Node* worldNode, Node* debugNode, SceneManager* assets) {
    // We need to know the content scale for resolution independence
    // If the device is higher resolution than 1024x576, Cocos2d will scale it
    // This was set as the design resolution in AppDelegate
    // To convert from design resolution to real, divide positions by cscale
    float cscale = Director::getInstance()->getContentScaleFactor();
    _frontFlip = 1;
    _middleFlip = 1;
    _backFlip = 1;
    _hillsnode->setPositionX(0.0f);
    _cloudsnode->setPositionX(0.0f);

    Texture2D* image;
    
    // Middle background
    image = assets->get<Texture2D>(MIDDLE_BACKGROUND);
    _middleBackground_1 = PolygonNode::createWithTexture(image);
    _middleBackground_1->setScale(cscale*MIDDLE_BACKGROUND_SCALE);
    _middleBackground_1->setPosition(MIDDLE_BACKGROUND_WIDTH/2 * cscale*MIDDLE_BACKGROUND_SCALE,
                                     MIDDLE_BACKGROUND_HEIGHT + MIDDLE_BACKGROUND_VERTICAL_OFFSET);
    _hillsnode->addChild(_middleBackground_1);
    _middleBackground_2 = PolygonNode::createWithTexture(image);
    _middleBackground_2->setScale(cscale*MIDDLE_BACKGROUND_SCALE);
    _middleBackground_2->setPosition(MIDDLE_BACKGROUND_WIDTH*3/2 * cscale*MIDDLE_BACKGROUND_SCALE,
                                     MIDDLE_BACKGROUND_HEIGHT + MIDDLE_BACKGROUND_VERTICAL_OFFSET);
    _hillsnode->addChild(_middleBackground_2);
    
    // Back background
    image = assets->get<Texture2D>(BACK_BACKGROUND);
    _backBackground_1 = PolygonNode::createWithTexture(image);
    _backBackground_1->setScale(cscale*BACK_BACKGROUND_SCALE);
    _backBackground_1->setPosition(BACK_BACKGROUND_WIDTH/2 * cscale*BACK_BACKGROUND_SCALE,
                                   BACK_BACKGROUND_HEIGHT + BACK_BACKGROUND_VERTICAL_OFFSET);
    _cloudsnode->addChild(_backBackground_1);
    _backBackground_2 = PolygonNode::createWithTexture(image);
    _backBackground_2->setScale(cscale*BACK_BACKGROUND_SCALE);
    _backBackground_2->setPosition(BACK_BACKGROUND_WIDTH*3/2 * cscale*BACK_BACKGROUND_SCALE,
                                   BACK_BACKGROUND_HEIGHT + BACK_BACKGROUND_VERTICAL_OFFSET);
    _cloudsnode->addChild(_backBackground_2);
    
    // Front background
    image = assets->get<Texture2D>(FRONT_BACKGROUND);
    _frontBackground_1 = PolygonNode::createWithTexture(image);
    _frontBackground_1->setScale(cscale*FRONT_BACKGROUND_SCALE);
    _frontBackground_1->setPosition(FRONT_BACKGROUND_WIDTH/2 * cscale*FRONT_BACKGROUND_SCALE,
                                    FRONT_BACKGROUND_HEIGHT + FRONT_BACKGROUND_VERTICAL_OFFSET);
    worldNode->addChild(_frontBackground_1);
    _frontBackground_2 = PolygonNode::createWithTexture(image);
    _frontBackground_2->setScale(cscale*FRONT_BACKGROUND_SCALE);
    _frontBackground_2->setPosition(FRONT_BACKGROUND_WIDTH*3/2 * cscale*FRONT_BACKGROUND_SCALE,
                                    FRONT_BACKGROUND_HEIGHT + FRONT_BACKGROUND_VERTICAL_OFFSET);
    worldNode->addChild(_frontBackground_2);
    
    rootNode->addChild(_hillsnode,0);
    rootNode->addChild(_cloudsnode,1);
}

void BackgroundView::handleScrolling(float levelOffset, float oldLevelOffset, Vec2 scale) {
    // Do parallax scrolling in _hillsnode and _cloudsnode
    _hillsnode->setPositionX(_hillsnode->getPositionX() - (scale.x*levelOffset/HILLS_DAMPING_FACTOR));
    _cloudsnode->setPositionX(_cloudsnode->getPositionX() - (scale.x*levelOffset/CLOUDS_DAMPING_FACTOR));
    
    // Tile background layers when necessary
    float tolerance = 0.05f;
    bool scrollRight = levelOffset >= oldLevelOffset; // true = right; false = left
    
    // Front
    float frontR = (_frontFlip + tolerance) * FRONT_BACKGROUND_WIDTH * FRONT_BACKGROUND_SCALE / scale.x;
    float frontL = (_frontFlip - tolerance) * FRONT_BACKGROUND_WIDTH * FRONT_BACKGROUND_SCALE / scale.x;
    bool frontOrder = _frontBackground_1->getPositionX() < _frontBackground_2->getPositionX(); // true = |1|2|; false = |2|1|
    if (scrollRight && levelOffset > frontR && frontOrder) {
        _frontBackground_1->setPositionX(_frontBackground_2->getPositionX() + FRONT_BACKGROUND_WIDTH * FRONT_BACKGROUND_SCALE);
        _frontFlip++;
    }
    else if (scrollRight && levelOffset > frontR && !frontOrder) {
        _frontBackground_2->setPositionX(_frontBackground_1->getPositionX() + FRONT_BACKGROUND_WIDTH * FRONT_BACKGROUND_SCALE);
        _frontFlip++;
    }
    else if (!scrollRight && levelOffset + DEFAULT_WIDTH < frontL && frontOrder) {
        _frontBackground_2->setPositionX(_frontBackground_1->getPositionX() - FRONT_BACKGROUND_WIDTH * FRONT_BACKGROUND_SCALE);
        _frontFlip--;
    }
    else if (!scrollRight && levelOffset + DEFAULT_WIDTH < frontL && !frontOrder) {
        _frontBackground_1->setPositionX(_frontBackground_2->getPositionX() - FRONT_BACKGROUND_WIDTH * FRONT_BACKGROUND_SCALE);
        _frontFlip--;
    }
    
    // Middle
    float middleR = (_middleFlip + tolerance) * MIDDLE_BACKGROUND_WIDTH * MIDDLE_BACKGROUND_SCALE / scale.x;
    float middleL = (_middleFlip - tolerance) * MIDDLE_BACKGROUND_WIDTH * MIDDLE_BACKGROUND_SCALE / scale.x;
    bool middleOrder = _middleBackground_1->getPositionX() < _middleBackground_2->getPositionX(); // true = |1|2|; false = |2|1|
    if (scrollRight && abs(_hillsnode->getPositionX() / scale.x) > middleR && middleOrder) {
        _middleBackground_1->setPositionX(_middleBackground_2->getPositionX() + MIDDLE_BACKGROUND_WIDTH * MIDDLE_BACKGROUND_SCALE);
        _middleFlip++;
    }
    else if (scrollRight && abs(_hillsnode->getPositionX() / scale.x) > middleR && !middleOrder) {
        _middleBackground_2->setPositionX(_middleBackground_1->getPositionX() + MIDDLE_BACKGROUND_WIDTH * MIDDLE_BACKGROUND_SCALE);
        _middleFlip++;
    }
    else if (!scrollRight && (abs(_hillsnode->getPositionX() / scale.x) + DEFAULT_WIDTH) < middleL && middleOrder) {
        _middleBackground_2->setPositionX(_middleBackground_1->getPositionX() - MIDDLE_BACKGROUND_WIDTH * MIDDLE_BACKGROUND_SCALE);
        _middleFlip--;
    }
    else if (!scrollRight && (abs(_hillsnode->getPositionX() / scale.x) + DEFAULT_WIDTH) < middleL && !middleOrder) {
        _middleBackground_1->setPositionX(_middleBackground_2->getPositionX() - MIDDLE_BACKGROUND_WIDTH * MIDDLE_BACKGROUND_SCALE);
        _middleFlip--;
    }
    
    // Back
    float backR = (_backFlip + tolerance) * BACK_BACKGROUND_WIDTH * BACK_BACKGROUND_SCALE / scale.x;
    float backL = (_backFlip - tolerance) * BACK_BACKGROUND_WIDTH * BACK_BACKGROUND_SCALE / scale.x;
    bool backOrder = _backBackground_1->getPositionX() < _backBackground_2->getPositionX(); // true = |1|2|; false = |2|1|
    if (scrollRight && abs(_cloudsnode->getPositionX() / scale.x) > backR && backOrder) {
        _backBackground_1->setPositionX(_backBackground_2->getPositionX() + BACK_BACKGROUND_WIDTH * BACK_BACKGROUND_SCALE);
        _backFlip++;
    }
    else if (scrollRight && abs(_cloudsnode->getPositionX() / scale.x) > backR && !backOrder) {
        _backBackground_2->setPositionX(_backBackground_1->getPositionX() + BACK_BACKGROUND_WIDTH * BACK_BACKGROUND_SCALE);
        _backFlip++;
    }
    else if (!scrollRight && (abs(_cloudsnode->getPositionX() / scale.x) + DEFAULT_WIDTH) < backL && backOrder) {
        _backBackground_2->setPositionX(_backBackground_1->getPositionX() - BACK_BACKGROUND_WIDTH * BACK_BACKGROUND_SCALE);
        _backFlip--;
    }
    else if (!scrollRight && (abs(_cloudsnode->getPositionX() / scale.x) + DEFAULT_WIDTH) < backL && !backOrder) {
        _backBackground_1->setPositionX(_backBackground_2->getPositionX() - BACK_BACKGROUND_WIDTH * BACK_BACKGROUND_SCALE);
        _backFlip--;
    }
}

void BackgroundView::update(float dt) {
    _cloudsnode->setPositionX(_cloudsnode->getPositionX() - CLOUD_VELOCITY);
}

void BackgroundView::removeAllChildren() {
    _hillsnode->removeAllChildren();
    _cloudsnode->removeAllChildren();
}
