#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0,0,0);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
	ofSetWindowTitle("color example");
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    
	ofSetFrameRate(60); // if vertical sync is off, we can go a bit fast... this caps the framerate at 60fps.
    
    // initialise member variables to the centre of the screen
    mouseXPercent = 0.5f;
    mouseYPercent = 0.5f;
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // here we demonstrate setting colors using HSB (Hue/Saturation/Brightness) rather than the 
    // more well-known RGB (Red/Green/Blue).
    
    // HSB allows colors to be specified in a way that is perhaps more natural to the understanding
    // of color that we have through language, using numerical values to describe 'hue', 
    // 'saturation' and 'brightness'. 
    
    // 'hue' refers to the 'color' in the rainbow sense, moving from red through yellow through
    //   green through blue through purple through red, looping around again.
    // 'saturation' refers to the intensity of the color. high saturation means intense color,
    //   low saturation means washed out or black and white.    
    // 'brightness' refers to how light or dark the color is. high brightness means a bright color,
    //   low brightness is dark. if the brightness is 0 the resulting color will be black, regardless 
    //   of the values for hue or saturation.
        
    // in ofColor all the values are specified as 0..255 .
    
    // for example, to specify an orange color that is strong and bright, you start by specifying the 
    // hue for the orange you want (a number from 15-30, 15 is more red and 30 is more yellow), then 
    // the saturation (something between 180 and 255 for high saturation) and the brightness (180 to 
    // 255 for bright).
    
    // for grays (and black and white) you can use any value for hue, set the saturation to 0 and 
    // control the level of gray with the brightness value: 255 is white, 0 is black, 128 is 50% gray.    

    
    
    // we want to draw a grid of 5 pixel x 5 pixel rectangles with a fixed hue, varying in 
    // saturation and brightness over X and Y
    
    // we use one hue (value from 0..255) for the whole grid. it changes over time. we use fmodf to
    // keep the hue value between 0 and 255, it works just like integer modulo (the % operator) but 
    // for floats.
    float hue = fmodf(ofGetElapsedTimef()*10,255);
    
    int step = 5;
    // step through horizontally
    for ( int i=0; i<ofGetWidth(); i+=step )
    {
        // step through vertically
        for ( int j=0; j<ofGetHeight(); j+=step )
        {
            // set HSB using our hue value that changes over time, saturation from the X position (i), 
            // and brightness from the Y position (j). we also invert the Y value since it looks 
            // nicer if the dark/black colors are along the bottom.
            ofColor c;
            // the range of each of the arguments here is 0..255 so we map i and j to that range.
            c.setHsb( hue, ofMap(i, 0,ofGetWidth(), 0,255), ofMap(j, ofGetHeight(),0, 0,255 ) );
            
            // assign the color and draw a rectangle
            ofSetColor( c );
            ofDrawRectangle( i, j, step-1, step-1 );
        }
    }
    
    // now we will draw a larger rectangle taking the color under the mouse
    
    // calculate the color under the mouse, using the same calculations as when drawing the grid, 
    // using mouseX and mouseY in place of i and j; draw a rectangle with this color. here we use 
    // ofColor::fromHsb which allows us to set the HSB color in a single line of code.
    ofColor color = ofColor::fromHsb(hue, 
                                     ofMap( mouseX, 0,ofGetWidth(), 0,255 ), 
                                     ofMap( mouseY, ofGetHeight(),0, 0,255 ) );
    ofSetColor( color );
    ofFill();
    ofDrawRectangle( mouseX, mouseY, 100, 100 );
    
    // now draw a white border around the rectangle
    ofNoFill();
    ofSetHexColor(0xFFFFFF);
    ofDrawRectangle( mouseX, mouseY, 100, 100 );
    ofFill();    
    
    // finally we draw text over the rectangle giving the resulting HSB and RGB values 
    // under the mouse
    ofSetHexColor(0xFFFFFF);
    ofDrawBitmapString("HSB: "+ofToString(int(hue))+
                       " "+ofToString(int(color.getSaturation()))+
                       " "+ofToString(int(color.getBrightness())),
                       10, ofGetHeight()-13 );
    ofDrawBitmapString("RGB: "+ofToString(int(color.r))+
                       " "+ofToString(int(color.g))+
                       " "+ofToString(int(color.b)), 
                       200, ofGetHeight()-13 );
    
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased  (int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    // update mouse x and y percent when the mouse moves
    mouseXPercent = float(x) / ofGetWidth();
    mouseYPercent = float(y) / ofGetHeight();
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}