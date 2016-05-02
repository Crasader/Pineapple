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


#define MODAL_MAIN_SCALE        1.35f

#define VERTICAL_MARGIN         -225
#define HORIZONTAL_MARGIN       150

#define BUTTON_UNPRESSED_TEXTURE_LOCATION   "textures/buttons/level_button.png"
#define BUTTON_PRESSED_TEXTURE_LOCATION     "textures/buttons/level_button_inverse.png"

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
    
    /** The draw scale used by the current resolution */
    Vec2 _scale;
    
    bool _transferToReset;
    bool _transferToLevelSelect;
    bool _transferToNextLevel;
    
    void init(Node* rootNode, SceneManager* assets, Vec2 scale, string textureID);
    
public:
    
    void initButton(Button* button, int fontSize, string text);
        
    bool shouldReset() { return _transferToReset; }
    
    bool shouldTransferToLevelSelect() { return _transferToLevelSelect; }
    
    bool shouldTransferToNextLevel() { return _transferToNextLevel; }
    
    void resetButtons() {
        _transferToReset = false;
        _transferToLevelSelect = false;
        _transferToNextLevel = false;
    }
    
    void position();
    
    void dispose();
};

#endif /* ModalView_h */
