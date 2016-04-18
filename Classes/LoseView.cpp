//
//  LoseView.cpp
//  Pineapple
//
//  Created by Michael Patashnik on 4/18/16.
//
//

#include "LoseView.h"

#define GRADIENT_Z      0
#define SPLASH_Z        1
#define PINEAPPLE_Z     2
#define BUTTON_Z        3

LoseView* LoseView::create(Node* root, SceneManager *assets) {
    LoseView* view = new (std::nothrow) LoseView();
    
    view->init(root, assets);
    
    root->addChild(view->_backgroundOverlay, GRADIENT_Z);
    root->addChild(view->_splashImage, SPLASH_Z);
    
    return view;
}

void LoseView::init(Node *root, SceneManager *assets){
    ModalView::init(root, assets, LOSE_SPLASH);
}
