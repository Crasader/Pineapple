//
//  HelloWorld.h
//  HelloWorld
//
//  This is the root class for a single game scene.  It shows how to subclass RootLayer to
//  make a new game.  This is a standard design pattern that we have used in all our samples.
//
#ifndef __HELLO_WORLD_H__
#define __HELLO_WORLD_H__

#include <cocos2d.h>
#include <cornell/CURootLayer.h>

/**
 * Class represents the root node for the game scene.
 *
 * As we have mentioned in class, Cocos2D has decided to violate MVC separation
 * in horrible, horrible ways.  To get a game on a 2D device, you NEED to have
 * a layer.  The layer is what (1) displays the graphics, (2) receives the touch
 * inputs, and (3) has an update method that is called regularly.
 *
 * Instead of fighting it, we simply make the Layer class the root scene of our
 * game engine.  However, from this point onward, we obey MVC separation as much
 * as possible.
 */
class HelloWorld : public cocos2d::RootLayer {
    
public:
    /**
     * Starts the layer, allocating initial resources
     *
     * This method is used to bootstrap the game.  It should start up an asset manager
     * and load initial assets.
     */
    void start() override;
    
    /**
     * Updates the game for a single animation frame
     *
     * This method is called every animation frame.  There is no draw() or render()
     * counterpoint to this method; drawing is done automatically in the scene graph.
     * However, this method is responsible for updating any transforms in the scene graph.
     *
     * @param  dt   the time in seconds since last update
     */
    void update(float dt) override;
    
    /**
     * Stops the layer, releasing all resources
     *
     * This method is used to clean-up any allocation that occurred in either start or
     * update.  While Cocos2d does have rudimentary garbage collection, you still have
     * to release any objects that you have retained.
     */
    void stop() override;
    
    /**
     * Callback function for the UI widgit in this demo.
     *
     * This function is called when the close button is pressed.  It quits the application.
     *
     * @param  pSender the widgit issuing the command.
     */
    void menuCloseCallback(Ref* pSender);
};

#endif // __HELLO_WORLD_H__
