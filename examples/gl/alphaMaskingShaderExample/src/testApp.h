/*  of Detroit DevCon 2012
 date: 2012/02/24 10:00:00
 summary: simple example how to alpha mask using a shader 
 author: Patricio Gonzalez Vivo
 author_site: http://patriciogonzalezvivo.com
 */

#pragma once

#define WEBCAM

#include "ofMain.h"

class testApp : public ofBaseApp{
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
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
	
    ofImage     srcImg;
    ofImage     dstImg;
    ofImage     brushImg;
    
    ofFbo       maskFbo;
    ofFbo       fbo;
    
    ofShader    shader;
    
    bool        bBrushDown;
};
