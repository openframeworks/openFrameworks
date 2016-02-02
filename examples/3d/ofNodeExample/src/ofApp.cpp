#include "ofApp.h"

/*
 This tutorial shows how to use the ofNode to build a gerarchy of objects, it was
 originally developed in a workshop held by Arturo Castro at Lacuna Lab.
 Most 3D classes in OF inheritate from ofNode. ofNode is simply a class that defines
 a point in a 3D space and allows to:
 - Chain a point with another point
 - Apply matrices transformation to it
 
 We will never define an ofNode directly in this example, that's why you will 
 not find the word `ofNode` in the code but both ofLight and ofBoxPrimitive
 inherith from ofNode, allowing us to access all its powerful methods.
 
 In this example we want to create a car with 2 headlamps, and we want to move the lights
 together with the car, without re-calculating the position of the 2 light.
 We define only how the car should move into the space, the light just need to stay on the front
 of the car, their movement it's just a consequence of the movement of the car.
 Have a look at the Car.cpp file to see how we attach the lights on the front of the car and how to move
 object using the handy rotate, move and tilt methods, without any need to use ofPushMatrix and ofPopMatrix.
 */



//--------------------------------------------------------------
void ofApp::setup(){
    light.setup();
    light.setPosition(-100, 200,0);
    // we set up a plane on which the car will move
    plane.set(10000, 10000);
    plane.rotate(270, 1, 0 , 0);
    plane.move(0, -49, 0);
    roadMaterial.setDiffuseColor(ofFloatColor::gray);
    roadMaterial.setShininess(0.01);
    car.setup();
    
    ofEnableDepthTest();
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
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == OF_KEY_UP){
        car.accellerate();
    }else if(key == OF_KEY_DOWN){
        car.brake();
    }
    
    switch (key) {
        case OF_KEY_UP:
            car.accellerate();
            break;
        case OF_KEY_DOWN:
            car.brake();
            break;
        case OF_KEY_LEFT:
			car.steer(+1);
            break;
        case OF_KEY_RIGHT:
			car.steer(-1);
            break;
        default:
            break;
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
