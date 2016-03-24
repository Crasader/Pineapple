#ifndef __TILEDLOADER_H__
#define __TILEDLOADER_H__

#include <cornell.h>
#include "LoadingScreenController.h"
#include "Level.h"

#include "CrushableModel.h"
#include "Pineapple.h"
#include "JelloModel.h"
#include "KidModel.h"

//Forward class declarations

using namespace cocos2d;

class LevelController {
protected:
    /** The level that is being built */
    Level* _level;
    /** Reference to the root node of the scene graph */
    RootLayer* _rootnode;
    /** Reference to the physics root of the scene graph */
    Node* _worldnode;
    /** Reference to the debug root of the scene graph */
    Node* _debugnode;
    /** The Box2D world */
    WorldController* _world;
    
    /** The scene manager for this game demo */
    SceneManager* _assets;
    
    /** The draw scale to apply to all game elements created by this loader */
    Vec2 _scale;
    
    void addGoal(float goalPos[]);
    
    void addWalls(int wallCount, float** wallPos);
    
    void addPlatforms(int platformCount, float** platformPos);
    
    void addPineapple(float pineapplePos[]);
    
    void addKids(float* kidPos[]);
    
    void addJello(int jelloCount, float* jelloPos[]);
    
    void addCups(int cupCount, float* cupPos[]);
    
    void addSpikes(int spikeCount, float* spikePos[]);
    
    void addBlender(float blenderPos[]);
    
public:

    static LevelController* create(SceneManager* assets,  RootLayer* rootnode,
                               Node* worldnode, Node* debugnode,
                               WorldController* world, Vec2 scale);
                 
    /** Reads in a level from a .tmx file into a Level object
     */
    Level* read(std::string fileName);
    
    /** Return a reference to the current level */
    Level* getLevel() { return _level; }

private:
    LevelController();

};
#endif