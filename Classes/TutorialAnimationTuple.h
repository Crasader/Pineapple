//
//  TutorialAnimationTuple.hpp
//  Pineapple
//
//  Created by Michael Patashnik on 5/1/16.
//
//

#ifndef TutorialAnimationTuple_h
#define TutorialAnimationTuple_h

#include <cornell.h>

using namespace std;

class TutorialAnimationTuple {
    
protected:
    
    /** The texture id of this animation */
    string _textureID;
    
    /** The frame count for this animation */
    int _frameCount;
    
    /** The position of this animation node on the screen */
    Vec2 _position;
    
    /** The scale of this animation node */
    float _scale;
    
    /** The actual animation node. Null until it is created */
    AnimationNode* _node;
    
    /** Tuple constructor - assigns the fields. Keeps node null until it is later initialized */
    TutorialAnimationTuple(string textureID, int frameCount, Vec2 position, float scale) {
        _textureID = textureID;
        _frameCount = frameCount;
        _position = position;
        _scale = scale;
        _node = nullptr;
    }
    
public:
    
    string getTextureID() { return _textureID; }
    
    int getFrameCount() { return _frameCount; }
    
    Vec2 getPosition() { return _position; }
    
    float getScale() { return _scale; }
    
    /** Creates the animation node represented by this tuple. If called multiple times, just returns the created node */
    AnimationNode* createNode(SceneManager* assets) {
        if (_node == nullptr) {
            _node = AnimationNode::create(assets->get<Texture2D>(_textureID), 1, _frameCount, _frameCount);
            _node->retain();
        }
        return _node;
    }
    
    /** Returns the animation node represented by this tuple, if it exists */
    AnimationNode* getNode() {
        return _node;
    }
    
    /** Creates a tuple with the given arguments. */
    static TutorialAnimationTuple* create(string textureID, int frameCount, Vec2 position, float scale) {
        TutorialAnimationTuple* t = new (std::nothrow) TutorialAnimationTuple(textureID, frameCount, position, scale);
        
        return t;
    }
    
    /** Destructor. If the node exists, releases and sets to null */
    ~TutorialAnimationTuple() {
        dispose();
    }
    
    /** Destructor. If the node exists, releases and sets to null */
    void dispose() {
        if (_node != nullptr) {
            _node->release();
            _node = nullptr;
        }
    }
};

#endif /* TutorialAnimationTuple_hpp */
