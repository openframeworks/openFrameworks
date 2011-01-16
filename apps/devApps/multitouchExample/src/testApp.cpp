#include "testApp.h"


vector < Finger > fingers;


int callback(int device, Finger *data, int nFingers,
					  double timestamp, int frame)
{
	
	
	fingers.clear();
	
	
	 for (int i=0; i<nFingers; i++) {
	 Finger *f = &data[i];
		 
		 
		 fingers.push_back(data[i]);	 
	/* printf("Frame %7d: Angle %6.2f, ellipse %6.3f x%6.3f; "
	 "position (%6.3f,%6.3f) vel (%6.3f,%6.3f) "
	 "ID %d, state %d [%d %d?] size %6.3f, %6.3f?\n",
	 f->frame,
	 f->angle * 90 / atan2(1,0),
	 f->majorAxis,
	 f->minorAxis,
	 f->normalized.pos.x,
	 f->normalized.pos.y,
	 f->normalized.vel.x,
	 f->normalized.vel.y,
	 f->identifier, f->state,0,0,
	 f->size, 0);
	 }*/
	 //printf("\n");
	 }
	
    
    return 0;
}




//--------------------------------------------------------------
void testApp::setup(){
	
	//ofSetEscapeQuitsApp(false);
	
	_mt_device = MTDeviceCreateDefault();
    MTRegisterContactFrameCallback(_mt_device, callback);
    MTDeviceStart(_mt_device);
	
	
}

void testApp::exit(){
	
	
	MTDeviceStop(_mt_device);
    MTUnregisterContactFrameCallback(_mt_device, callback);
    MTDeviceRelease(_mt_device);	
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

	for (int i = 0; i < fingers.size(); i++){
	
		ofPushMatrix();
		
		ofTranslate(fingers[i].normalized.pos.x * ofGetWidth(), ofGetHeight() - fingers[i].normalized.pos.y * ofGetHeight(), 0);
		
		ofRotateZ(-fingers[i].angle * RAD_TO_DEG);
		ofScale(6,6,1);
		
		ofEllipse(-fingers[i].majorAxis/2, -fingers[i].minorAxis/2, fingers[i].majorAxis, fingers[i].minorAxis);
		
		ofPopMatrix();
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	

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

