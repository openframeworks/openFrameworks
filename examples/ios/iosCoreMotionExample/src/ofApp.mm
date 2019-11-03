#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	

    ofSetFrameRate(60);
    ofBackground(255, 255, 0);
    

    coreMotion.setupMagnetometer();
    coreMotion.setupGyroscope();
    coreMotion.setupAccelerometer();
    coreMotion.setupAttitude(CMAttitudeReferenceFrameXMagneticNorthZVertical);
}

//--------------------------------------------------------------
void ofApp::update(){

    coreMotion.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

    
    // attitude- quaternion
    ofDrawBitmapStringHighlight("Attitude: (quaternion x,y,z,w)", 20, 25);
    ofSetColor(0);
    glm::quat quat = coreMotion.getQuaternion();
    ofDrawBitmapString(ofToString(quat.x(),3), 20, 50);
    ofDrawBitmapString(ofToString(quat.y(),3), 90, 50);
    ofDrawBitmapString(ofToString(quat.z(),3), 160, 50);
    ofDrawBitmapString(ofToString(quat.w(),3), 230, 50);
    
    // attitude- roll,pitch,yaw
    ofDrawBitmapStringHighlight("Attitude: (roll,pitch,yaw)", 20, 75);
    ofSetColor(0);
    ofDrawBitmapString(ofToString(coreMotion.getRoll(),3), 20, 100);
    ofDrawBitmapString(ofToString(coreMotion.getPitch(),3), 120, 100);
    ofDrawBitmapString(ofToString(coreMotion.getYaw(),3), 220, 100);
    
    // accelerometer
    glm::vec3 a = coreMotion.getAccelerometerData();
    ofDrawBitmapStringHighlight("Accelerometer: (x,y,z)", 20, 125);
    ofSetColor(0);
    ofDrawBitmapString(ofToString(a.x,3), 20, 150);
    ofDrawBitmapString(ofToString(a.y,3), 120, 150);
    ofDrawBitmapString(ofToString(a.z,3), 220, 150);
    
    // gyroscope
    glm::vec3 g = coreMotion.getGyroscopeData();
    ofDrawBitmapStringHighlight("Gyroscope: (x,y,z)", 20, 175);
    ofSetColor(0);
    ofDrawBitmapString(ofToString(g.x,3), 20, 200 );
    ofDrawBitmapString(ofToString(g.y,3), 120, 200 );
    ofDrawBitmapString(ofToString(g.z,3), 220, 200 );
    
    // magnetometer
    glm::vec3 m = coreMotion.getMagnetometerData();
    ofDrawBitmapStringHighlight("Magnetometer: (x,y,z)", 20, 225);
    ofSetColor(0);
    ofDrawBitmapString(ofToString(m.x,3), 20, 250);
    ofDrawBitmapString(ofToString(m.y,3), 120, 250);
    ofDrawBitmapString(ofToString(m.z,3), 220, 250);
    
    
    
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    
    // 1) quaternion rotations
    float angle;
    glm::vec3 axis;//(0,0,1.0f);
    quat.getRotate(angle, axis);
    ofRotate(angle, axis.x, -axis.y, axis.z); // rotate with quaternion
    
    // 2) rotate by multiplying matrix directly
    //ofMatrix4x4 mat = coreMotion.getRotationMatrix();
    //mat.rotate(180, 0, -1.0f, 0);
    //ofMultMatrix(mat); // OF 0.74: glMultMatrixf(mat.getPtr());
    
    // 3) rotate with eulers
    //ofRotateX( ofRadToDeg( coreMotion.getPitch() ) );
    //ofRotateY( -ofRadToDeg( coreMotion.getRoll() ) );
    //ofRotateZ( ofRadToDeg( coreMotion.getYaw() ) );
    
    ofNoFill();
	ofDrawBox(0, 0, 0, 220); // OF 0.74: ofBox(0, 0, 0, 220);
    ofDrawAxis(100);
    ofPopMatrix();
    
    ofFill();
    ofDrawBitmapString(ofToString("Double tap to reset \nAttitude reference frame"), 20, ofGetHeight() - 50);
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){

    // resets attitude to current
    coreMotion.resetAttitude();
}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::lostFocus(){

}

//--------------------------------------------------------------
void ofApp::gotFocus(){

}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){

}
