/*  
 *  summary:Example of how to use GPU for data processing. The data it«s going to be stored
 *          on the color channels of the FBO«s textures. In this case we are going to use just
 *          RED and GREEN channels on two textures. One for the position and the other one for
 *          the velocity. For updating the informacion of those textures we are going to use 
 *          two FBO«s for each type of information. This pair of FBO will pass the information 
 *          from one to other in a techninc call PingPong.
 *          After updating this information, we are going to use the textures allocated on GPU memory
 *          for moving some vertex and then multiplied them in order to make little frames that hold 
 *          a texture of a spark of light.
 */

#pragma once

#include "ofMain.h"

// Struct for doing PingPong quickly and easy
//
// Because on GPU you can«t write over the texture that you are reading we are
// using to pair of ofFbo attached together on what we call pingPongBuffer 
// Learn more about Ping-Pong at:
//
// http://www.comp.nus.edu/~ashwinna/docs/PingPong_FBO.pdf
// http://www.seas.upenn.edu/~cis565/fbo.htm#setupgl4
//
struct pingPongBuffer {
public:
    void allocate( int _width, int _height, int _internalformat = GL_RGBA, float _dissipation = 1.0f){
        // Allocate
        for(int i = 0; i < 2; i++){
            FBOs[i].allocate(_width,_height, _internalformat );
            FBOs[i].getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
        }
        
        // Clean
        clear();
        
        // Set everything to 0
        flag = 0;
        swap();
        flag = 0;
    }
    
    void swap(){
        src = &(FBOs[(flag)%2]);
        dst = &(FBOs[++(flag)%2]);
    }
    
    void clear(){
        for(int i = 0; i < 2; i++){
            FBOs[i].begin();
            ofClear(0,255);
            FBOs[i].end();
        }
    }
    
    ofFbo& operator[]( int n ){ return FBOs[n];}
    
    ofFbo   *src;       // Source       ->  Ping
    ofFbo   *dst;       // Destination  ->  Pong
private:
    ofFbo   FBOs[2];    // Real addresses of ping/pong FBO«s  
    int     flag;       // Integer for making a quick swap
};


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
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofShader    updatePos;
    ofShader    updateVel;
    ofShader    updateRender;
		
    pingPongBuffer posPingPong;
    pingPongBuffer velPingPong;
    
    ofFbo   renderFBO;
    
    ofImage sparkImg;
    
    float   timeStep;
    float   particleSize;
    
    int     width, height;
    int     imgWidth, imgHeight;
    int     numParticles;
    int     textureRes;
    
    
    ofVboMesh mesh;
};
