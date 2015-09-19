#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	gl->background(255,255,255);
	camera.setEvents(window->events());
	camera.setRenderer(gl);
	gui->radius.addListener(this,&ofApp::radiusChanged);
	gui->radius=200;
}

void ofApp::radiusChanged(float & radius){
	sphere.set(radius,radius*0.08);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	gl->bind(camera,gl->getCurrentViewport());
	gl->setColor(gui->color);
	gl->draw(sphere,OF_MESH_WIREFRAME);
	gl->setColor(gui->color,gui->color->a*0.1);
	gl->draw(sphere,OF_MESH_FILL);
	gl->unbind(camera);

	gl->setColor(0);
	gl->drawString(ofToString(window->events().getFrameRate()),20,20,0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
