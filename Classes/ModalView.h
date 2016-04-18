//
//  LoseView.hpp
//  Pineapple
//
//  Created by Michael Patashnik on 4/18/16.
//
//

#ifndef __MODAL_VIEW_H__
#define __MODAL_VIEW_H__

#include <stdio.h>
#include <cornell.h>
#include "ui/CocosGUI.h"
#include "Const.h"
#include "Texture.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;

class ModalView {
protected:
    /** A reference to the root node for this splash */
    Node* _root;
    
    /** A reference to the asset manager */
    SceneManager *_assets;
    
    /** The translucent gray overlay */
    Node* _backgroundOverlay;
    
    /** The texture id for the splash image */
    string _splashTextureID;

    /** The splash image */
    Node* _splashImage;
    
    void init(Node* rootNode, SceneManager* assets, string textureID);
    
public:
    void position();
    
    void dispose();
    
};

#endif /* ModalView_h */
