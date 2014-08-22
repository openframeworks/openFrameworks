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

    ThreadedObject threadedObject;
        ///< \brief Our threaded object.
    int mainAppsCount;
        ///< \brief A local count that is only accessed in the main thread

};
