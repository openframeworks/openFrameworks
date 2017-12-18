#include "ofBaseApp.h"
#include "ofEvents.h"

ofBaseApp::ofBaseApp() {
	mouseX = mouseY = 0;
}

ofBaseApp::~ofBaseApp(){
}

void ofBaseApp::setup(){}
void ofBaseApp::update(){}
void ofBaseApp::draw(){}
void ofBaseApp::exit(){}

void ofBaseApp::windowResized(int w, int h){}

void ofBaseApp::keyPressed( int key ){}
void ofBaseApp::keyReleased( int key ){}

/// \brief Called on the active window when the mouse is moved
void ofBaseApp::mouseMoved( int x, int y ){}

/// \brief Called on the active window when the mouse is dragged, i.e.
/// moved with a button pressed
void ofBaseApp::mouseDragged( int x, int y, int button ){}

/// \brief Called on the active window when a mouse button is pressed
void ofBaseApp::mousePressed( int x, int y, int button ){}

/// \brief Called on the active window when a mouse button is released
void ofBaseApp::mouseReleased(int x, int y, int button ){}

void ofBaseApp::mouseScrolled(int x, int y, float scrollX, float scrollY ){}

void ofBaseApp::mouseEntered( int x, int y ){}

void ofBaseApp::mouseExited( int x, int y){}

void ofBaseApp::dragEvent(ofDragInfo dragInfo) { }
void ofBaseApp::gotMessage(ofMessage msg){ }

void ofBaseApp::setup(ofEventArgs & args){
	setup();
}
void ofBaseApp::update(ofEventArgs & args){
	update();
}
void ofBaseApp::draw(ofEventArgs & args){
	draw();
}
void ofBaseApp::exit(ofEventArgs & args){
	exit();
}

void ofBaseApp::windowResized(ofResizeEventArgs & resize){
	windowResized(resize.width,resize.height);
}

void ofBaseApp::keyPressed( ofKeyEventArgs & key ){
	keyPressed(key.key);
}
void ofBaseApp::keyReleased( ofKeyEventArgs & key ){
	keyReleased(key.key);
}

void ofBaseApp::mouseMoved( ofMouseEventArgs & mouse ){
	mouseX=mouse.x;
	mouseY=mouse.y;
	mouseMoved(mouse.x,mouse.y);
}
void ofBaseApp::mouseDragged( ofMouseEventArgs & mouse ){
	mouseX=mouse.x;
	mouseY=mouse.y;
	mouseDragged(mouse.x,mouse.y,mouse.button);
}
void ofBaseApp::mousePressed( ofMouseEventArgs & mouse ){
	mouseX=mouse.x;
	mouseY=mouse.y;
	mousePressed(mouse.x,mouse.y,mouse.button);
}
void ofBaseApp::mouseReleased(ofMouseEventArgs & mouse){
	mouseX=mouse.x;
	mouseY=mouse.y;
	mouseReleased(mouse.x,mouse.y,mouse.button);
}
void ofBaseApp::mouseScrolled( ofMouseEventArgs & mouse ){
	mouseScrolled(mouse.x, mouse.y, mouse.scrollX, mouse.scrollY);
}
void ofBaseApp::mouseEntered( ofMouseEventArgs & mouse ){
	mouseEntered(mouse.x,mouse.y);
}
void ofBaseApp::mouseExited( ofMouseEventArgs & mouse ){
	mouseExited(mouse.x,mouse.y);
}
void ofBaseApp::dragged(ofDragInfo & drag){
	dragEvent(drag);
}
void ofBaseApp::messageReceived(ofMessage & message){
	gotMessage(message);
}

void ofBaseApp::touchDown(int x, int y, int id) {};
void ofBaseApp::touchMoved(int x, int y, int id) {};
void ofBaseApp::touchUp(int x, int y, int id) {};
void ofBaseApp::touchDoubleTap(int x, int y, int id) {};
void ofBaseApp::touchCancelled(int x, int y, int id) {};

void ofBaseApp::touchDown(ofTouchEventArgs & touch) {
	touchDown(touch.x, touch.y, touch.id);
};
void ofBaseApp::touchMoved(ofTouchEventArgs & touch) {
	touchMoved(touch.x, touch.y, touch.id);
};
void ofBaseApp::touchUp(ofTouchEventArgs & touch) {
	touchUp(touch.x, touch.y, touch.id);
};
void ofBaseApp::touchDoubleTap(ofTouchEventArgs & touch) {
	touchDoubleTap(touch.x, touch.y, touch.id);
};
void ofBaseApp::touchCancelled(ofTouchEventArgs & touch){
	touchCancelled(touch.x, touch.y, touch.id);
}
