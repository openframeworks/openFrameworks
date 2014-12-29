#pragma once


#include "ofMain.h"
#include "threadedObject.h" // include our ThreadedObject class.


class ofApp: public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);

    /// \brief Our threaded object.
    ThreadedObject threadedObject;

    /// \brief A local count that is only accessed in the main thread
    int mainAppsCount;

};
