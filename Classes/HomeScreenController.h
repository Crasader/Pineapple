//
//  HomeScreenController.hpp
//  Pineapple
//
//  Created by Michael Patashnik on 4/27/16.
//
//

#ifndef HomeScreenController_h
#define HomeScreenController_h

#include <stdio.h>
#include "AbsScreenController.h"
#include "InputController.h"
#include "Const.h"

class HomeScreenController : public AbsScreenController {
protected:
    
    /** The button that begins the game */
    Button* _playButton;
    
    /** The background image displayed for the home screen */
    Node* _backgroundNode;
    
public:
    /**
     * Constructor
     */
    HomeScreenController() {};
    
    void update(float dt) override;
    
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
    bool init(Node* root, InputController* input);
    
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
     *
     * @retain a reference to the root layer
     * @return  true if the controller is initialized properly, false otherwise.
     */
    bool init(Node* root, InputController* input, const Rect& rect);
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     */
    void dispose();
};

#endif /* HomeScreenController_h */
