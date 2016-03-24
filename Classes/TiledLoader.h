#ifndef __TILEDLOADER_H__
#define __TILEDLOADER_H__

#include <cornell.h>
#include "Vec2.h"
#include "Level.h"

#include "CrushableModel.h"
#include "Pineapple.h"
#include "JelloModel.h"
#include "KidModel.h"

//Forward class declarations

using namespace cocos2d;

class TiledLoader {
protected:
    /** The level that is being built */
    Level* _level;
    
    /** The scene manager for this game demo */
    SceneManager* _assets;
    
    /** The draw scale to apply to all game elements created by this loader */
    Vec2 _scale;
    
    void addGoal(Vec2 goalPos);
    
    void addWalls(int wallCount, float** wallPos);
    
    void addPlatforms(int platformCount, float** platformPos);
    
    void addPineapple(Vec2 pineapplePos);
    
    void addKids(Vec2 kidPos[]);
    
    void addJello(int jelloCount, Vec2 jelloPos[]);
    
    void addCups(int cupCount, Vec2 cupPos[]);
    
    void addSpikes(int spikeCount, Vec2 spikePos[]);
    
    void addBlender(Vec2 blenderPos);
    
public:

    static TiledLoader* create(SceneManager* assets, Vec2 scale);
                 
    /** Reads in a level from a .tmx file into a Level object
     */
    Level* read(std::string fileName);

private:
    TiledLoader();

};
#endif