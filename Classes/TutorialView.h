//
//  TutorialView.hpp
//  Pineapple
//
//  Created by Michael Patashnik on 5/1/16.
//
//

#ifndef TutorialView_h
#define TutorialView_h

#include <cornell.h>
#include "ModalView.h"

class TutorialView : public ModalView {
protected:
    /** The animations drawn on this tutorial view */
    std::vector<AnimationNode*> _animations;
    
    /** Button that dismisses the tutorial view */
    Button* _dismissButton;
    
    
        
};

#endif /* TutorialView_hpp */
