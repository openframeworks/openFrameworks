/**
 *
 * OFDevCon Example Code Sprint
 * Simple Color Key example
 *
 * This example shows a very cheap and simple way to key out blue from an image
 *
 * Created by James George for openFrameworks workshop at ITP in September 2011
 * Adapted during ofDevCon on 2/23/2012
 */

#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

  public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    ofImage original;
	ofImage keyed;
    
};
