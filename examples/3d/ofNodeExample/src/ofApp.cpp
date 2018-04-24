#include "ofApp.h"

/* Refer to the README.md in the example's root folder for more information on usage */

//--------------------------------------------------------------
void ofApp::setup(){
    light.setup();
    light.setPosition(-100, 200,0);
    // we set up a plane on which the car will move
    plane.set(10000, 10000);
    plane.rotateDeg(270, 1, 0 , 0);
    plane.move(0, -49, 0);
    roadMaterial.setDiffuseColor(ofFloatColor::gray);
    roadMaterial.setShininess(0.01);
    car.setup();
    
    
    ofEnableDepthTest();
    
    //Set our camera up in a nice location to view our awesome car
	cam.setPosition(-965, 586, -1084);
	cam.lookAt(car.getNode(), {0.f, 1.f, 0.f});
	cam.setFarClip(10000);
   
}

//--------------------------------------------------------------
void ofApp::update(){
	car.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
 
    cam.begin();
        roadMaterial.begin();
            plane.draw();
        roadMaterial.end();
        car.draw();
    cam.end();
    
    ofDrawBitmapStringHighlight("Use your Arrow keys to move your \"car\" around the scene. \nClick and drag your mouse to move the 3D scene.", 20,20);
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
	if(key == OF_KEY_UP){
        car.accelerate();
    }else if(key == OF_KEY_DOWN){
        car.brake();
    }else if(key== OF_KEY_LEFT){
        car.steer(+1);
    }else if(key==OF_KEY_RIGHT){
        car.steer(-1);
    }
    
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
