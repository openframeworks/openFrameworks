#include "testApp.h"


/*
 
 Image Sequence Example:
 In this example we are going to load a sequence of images from a folder.
 We know that the images are in order and the we saved them out at 24fps. 
 the goal is to play them back at 24fps independent of the fps of the app.\
 
 note: if you bring the frame-rate lower then 24 it will look a bit slow
 
 
 Topics:
 - ofDirectory
 - ofImage
 - timing
 
 gif from: http://probertson.livejournal.com/32350.html
 
 */


//--------------------------------------------------------------
void testApp::setup() {
    
    ofBackground(0);
    ofSetWindowTitle("The Moon is made of plops");
                     
    // read the directory for the images
    // we know that they are named in seq
    ofDirectory dir;
    
    int nFiles = dir.listDir("plops");
    if(nFiles) {
        
        for(int i=0; i<dir.numFiles(); i++) { 
            
            // add the image to the vector
            string filePath = dir.getPath(i);
            images.push_back(ofImage());
            images.back().loadImage(filePath);
            
        }
        
    } 
    else printf("Could not find folder\n");
    
    
    // we have have a frame count that 
    // is used to progress to the next
    frameCount = 0;
    
    // if we toggle this the image seq
    // will play backwards - hit space
    bReverse = false;
    
    // this will set the speed to play 
    // the animation back we set the
    // default to 24fps
    frameSpeed = 24;
    
    
    // set the fps 
    appFPS = 60;
    ofSetFrameRate(appFPS);
    
}

//--------------------------------------------------------------
void testApp::update() {
    
}
bool bRev=false;
//--------------------------------------------------------------
void testApp::draw() {
    
    // get the time since the last frame change
    float time = ofGetElapsedTimeMillis() - timeSinceLastFrame;
    
    // if it is greater then the desired frame speed 
    // then we need to increment to the next frame
    if(time > frameSpeed) {
        
        // if we are going backward the decrement
        // watch out for zero...
        if(bRev) {
            frameCount --;
            if(frameCount < 0) frameCount = images.size()-1;  
        }
        else {
            frameCount ++;
        }
    
        // we modulate the framecount if we hit the end
        frameCount %= (int)images.size();
        timeSinceLastFrame = ofGetElapsedTimeMillis();
    }
    
    
    // draw the image sequence at the new frame coujnt
    ofSetColor(255);
    images[frameCount].draw(256, 56);
    
    
    // how fast is the app running and some other info
    ofSetColor(50);
    ofRect(0, 0, 200, 200);
    ofSetColor(200);
    string info;
    info += ofToString(appFPS)+"/"+ofToString(ofGetFrameRate(), 0)+" fps\n";
    info += ofToString(images.size())+"/"+ofToString(frameCount)+" frames\n\n";
    info += ofToString(frameSpeed)+" Sequence speed\n\n";
    info += "Keys:\nup & down arrows to\nchange fps\n\n";
    info += "left & right arrows to\nchange sequence speed";
    ofDrawBitmapString(info, 15, 30);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    
    if(key == OF_KEY_UP)      appFPS ++;
    if(key == OF_KEY_DOWN)    appFPS --;
    if(key == OF_KEY_LEFT)    frameSpeed --;
    if(key == OF_KEY_RIGHT)   frameSpeed ++;
    if(key == ' ')            bRev = !bRev;
    
    
    // check for less than zero...
    frameSpeed = MAX(frameSpeed, 0);
    appFPS     = MAX(appFPS, 1);
    
    ofSetFrameRate(appFPS);
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
}