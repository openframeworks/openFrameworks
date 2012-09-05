#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    //load the image object
    original.loadImage("myImage.jpg");
    
    //get the pixels from the image
    ofPixelsRef pixels = original.getPixelsRef();
	
	//let's manipulate the pixels by iterating through them
    //or a rough bluescreen
    for(int y = 0; y < original.getHeight(); y++){
		for(int x = 0; x < original.getWidth(); x++){
			
			//if the blue is greater than the combination of  red and green together, delete that pixel
			ofColor color = original.getColor(x, y);
			if(color.b > color.g + color.r){
				pixels.setColor(x, y, ofColor::black);
			}
		}
	}

    //after we're done we need to put the pixels into the keyed image
    keyed.setFromPixels(pixels);
}

//--------------------------------------------------------------
void testApp::update(){
    //no need to use this
}

//--------------------------------------------------------------
void testApp::draw(){
    
    //draw the image every frame, the changes you made in setup() will be there
    original.draw(0,0);
	keyed.draw(original.getWidth(),0);
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == 's'){
		//we can save out the modified version here
		keyed.saveImage("keyed_image.png");
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