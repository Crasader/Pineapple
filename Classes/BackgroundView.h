//
//  BackgroundView.hpp
//  Pineapple
//
//  Created by Michael Patashnik on 3/24/16.
//
//

#ifndef __BACKGROUND_VIEW_H__
#define __BACKGROUND_VIEW_H__

#include <cornell.h>
#include "Const.h"
#include "Texture.h"

using namespace cocos2d;

class BackgroundView {
protected:
    /** A reference to the root node */
    Node* _rootNode;
    /** A reference to the world node */
    Node* _worldNode;
    
    /** The front background (first copy), with no parallax */
    PolygonNode* _frontBackground_1;
    /** The front background (second copy), with no parallax */
    PolygonNode* _frontBackground_2;

    /** The middle background (first copy), with some parallax */
    PolygonNode* _middleBackground_1;
    /** The middle background (second copy), with some parallax */
    PolygonNode* _middleBackground_2;

    /** The back backBackground (first copy), with heavy parallax */
    PolygonNode* _backBackground_1;
    /** The back background (second copy), with heavy parallax */
    PolygonNode* _backBackground_2;
    
    /** For hills background */
    Node* _hillsnode;
    /** For clouds background */
    Node* _cloudsnode;
    
    /** Number of times front background has been flipped */
    int _frontFlip;
    /** Number of times middle background has been flipped */
    int _middleFlip;
    /** Number of times back background has been flipped */
    int _backFlip;
    
public:
    static BackgroundView* createAndAddTo(Node* rootNode, Node* worldNode, SceneManager* assets);
    
    int getFrontFlip() const { return _frontFlip; }
    
    int getMiddleFlip() const { return _middleFlip; }
    
    int getBackFlip() const { return _backFlip; }
    
    void handleScrolling (float offset, float levelOffset, float oldLevelOffset, Vec2 scale);
    
    void update(float dt);
    
    void removeAllChildren();
    
    void reset(Node* worldNode);
    
private:
    void init(Node* rootNode, Node* worldNode, SceneManager* assets);
};

#endif /* BackgroundView_hpp */
