#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	renderer.setup();
	bool useJson = false;
	if(useJson){
		gui.setDefaultSerializer(json);
		gui.setup(renderer.parameters,"settings.json");
	}else{
		gui.setup(renderer.parameters,"settings.xml");
	}
	ofEnableAlphaBlending();
	renderer.frameNum.addListener(this,&testApp::frameNumChanged);
}

void testApp::frameNumChanged(int & frame){
	cout << frame << endl;
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
	gui.draw();
	renderer.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key=='s'){
		settings.serialize(renderer.parameters);
		settings.saveFile("settings.xml");
	}
	if(key=='o'){
		cout << renderer.parameters;
	}
	if(key=='j'){
		json.serialize(renderer.parameters);
		json.save("settings.json");
	}
	if(key=='r') renderer.color = ofColor(127);
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
