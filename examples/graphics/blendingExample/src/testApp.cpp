#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	 
    alpha = 0;
	counter = 0;

    sprintf(eventString, "Alpha"); 

	vagRounded.loadFont("vag.ttf", 32);
	ofBackground(50,50,50);	

    rainbow.allocate(256, 256, OF_IMAGE_COLOR_ALPHA);
    rainbow.loadImage("rainbow.tiff");
}


//--------------------------------------------------------------
void testApp::update(){
	counter = counter + 0.033f;

    alpha += 0.01;
   
    alpha = (alpha > 1.0) ? 1.0 : alpha;
}

//--------------------------------------------------------------
void testApp::draw(){
	
    sprintf(timeString, "Press 1 - 5 to switch blend modes");
	
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
	ofSetHexColor(0xffffff);
	vagRounded.drawString(eventString, 98,198);
	
	ofSetColor(255,122,220);
	vagRounded.drawString(eventString, 100,200);
	
	ofSetHexColor(0xffffff);
	vagRounded.drawString(timeString, 98,98);
	
	ofSetColor(255,122,220);
	vagRounded.drawString(timeString, 100,100);

    ofSetColor(255, 255, 255,255);
    
    
    ofEnableBlendMode(blendMode);
    
    rainbow.draw(mouseX, mouseY);
    
    ofDisableBlendMode();
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 

    switch (key) {
        case 49:
            blendMode = OF_BLENDMODE_ALPHA;
            sprintf(eventString, "Alpha"); 
            break;
        case 50:
            blendMode = OF_BLENDMODE_ADD;
            sprintf(eventString, "Add"); 
            break;
        case 51:
            blendMode = OF_BLENDMODE_MULTIPLY;
            sprintf(eventString, "Multiply"); 
            break;
        case 52:
            blendMode = OF_BLENDMODE_SUBTRACT;
            sprintf(eventString, "Subtract"); 
            break;
        case 53:
            blendMode = OF_BLENDMODE_SCREEN;
            sprintf(eventString, "Screen"); 
            break;
        default:
            break;
    }
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
