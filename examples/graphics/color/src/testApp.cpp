#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
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
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
    
    // here we demonstrate setting colors using HSB (Hue/Saturation/Brightness) rather than the 
    // more well-known RGB (Red/Green/Blue).
    
    // HSB allows colors to be specified in a way that is perhaps more natural to the understanding
    // of color that we have through language, using numerical values to describe 'hue', 
    // 'saturation' and 'brightness'.
    
    // 'hue' refers to the 'color' in the rainbow sense, moving from red through orange through
    //   green through blue through purple through red, then looping around again.
    // 'saturation' refers to the intensity of the color. high saturation means intense color,
    //   low saturation means washed out or black and white.    
    // 'brightness' refers to how light or dark the color is. high brightness means a bright color,
    //   low brightness is dark. if the brightness is 0 the resulting color will be black, regardless 
    //   of the values for hue or saturation.
        
    // in ofColor all the values are specified as 0..255 .
    
    // for example, to specify an orange color that is strong and bright, you start by specifying the 
    // hue for the orange you need , then the saturation (180 to 255 for high saturation) 
    // and the brightness (180 to 255 for bright).
    
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
        // iPercent moves between 0 and 1 as we go from left to right
        float iPercent = float(i)/ofGetWidth();
        
        // step through vertically
        for ( int j=0; j<ofGetHeight(); j+=step )
        {
            // jPercent moves from 1 to 0 as we go from top to bottom
            float jPercent = 1.0f-float(j)/ofGetHeight();
            
            // set HSB using our hue value that changes over time, saturation from the
            // x position (iPercent), and brightness from the y position (jPercent)
            ofColor c;
            
            // the range of each of the arguments is here is 0..255
            c.setHsb( hue, iPercent*255, jPercent*255 );
            
            // assign the color and draw a rectangle
            ofSetColor( c );
            ofRect( i, j, step-1, step-1 );
        }
    }
    
    // now we will draw a larger rectangle taking the color under the mouse
    
    // find the color under the mouse, using the same calculations as when drawing the grid, 
    // using mouseX and mouseY in place of i and j
    ofColor color;
    color.setHsb( hue, float(mouseX)/ofGetWidth()*255, (1.0f-float(mouseY)/ofGetHeight())*255 );
    // set the color and draw the rectangle
    ofSetColor( color );
    ofFill();
    ofRect( mouseX, mouseY, 100, 100 );
    
    // now draw a white border around the rectangle
    ofNoFill();
    ofSetHexColor(0xFFFFFF);
    ofRect( mouseX, mouseY, 100, 100 );
    ofFill();    
    
    // finally we draw text over the rectangle giving the resulting HSB and RGB values 
    // under the mouse
    ofSetHexColor(0xFFFFFF);
    ofDrawBitmapString("HSB: "+ofToString(int(hue))+
                       " "+ofToString(int(color.getSaturation()))+
                       " "+ofToString(int(color.getBrightness())),
                       //mouseX-40, mouseY-10 );
                       10, ofGetHeight()-13 );
    ofDrawBitmapString("RGB: "+ofToString(int(color.r))+
                       " "+ofToString(int(color.g))+
                       " "+ofToString(int(color.b)), 
                       //mouseX-40,mouseY+25 );
                       200, ofGetHeight()-13 );
    
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    // update mouse x and y percent when the mouse moves
    mouseXPercent = float(x) / ofGetWidth();
    mouseYPercent = float(y) / ofGetHeight();
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