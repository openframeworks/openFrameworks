#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	renderer1.setup("renderer1");
	renderer2.setup("renderer2");
	bool useJson = false;
	if(useJson){
		gui.setDefaultSerializer(json);
		gui.setup("settings","settings.json");
	}else{
		gui.setup("settings","settings.xml");
	}

	// by setting the listener before the param values
	// the listener method will get called on gui.add
	vSync.addListener(this,&testApp::vSyncChanged);
	gui.add(vSync.set("vSync",true));
	gui.add(renderer1.parameters);
	gui.add(renderer2.parameters);

	gui.loadFromFile("settings.xml");

	font.loadFont(OF_TTF_SANS,9,true,true);
	ofEnableAlphaBlending();
}

void testApp::vSyncChanged(bool & vSync){
	ofSetVerticalSync(vSync);
}


//--------------------------------------------------------------
void testApp::update(){
	// frameNum is a readonly parameter so this will fail to compile
	// unless we are inside the CirclesRenderer class
	// renderer.frameNum = 5;
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::white, ofColor::gray);
	renderer1.draw();
	renderer2.draw();
	ofSetColor(255);
	gui.draw();
	font.drawString("frame: " + ofToString(renderer1.frameNum),ofGetWidth()-150,20);
	font.drawString("fps: " + ofToString((int)ofGetFrameRate()),ofGetWidth()-150,40);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key=='s'){
		settings.serialize(renderer1.parameters);
		settings.serialize(renderer2.parameters);
		settings.saveFile("settings.xml");
	}
	if(key=='o'){
		cout << renderer1.parameters;
		cout << renderer2.parameters;
	}
	if(key=='j'){
		json.serialize(renderer1.parameters);
		json.serialize(renderer2.parameters);
		json.save("settings.json");
	}
	if(key=='r'){
		renderer1.color = ofColor(127);
		renderer2.color = ofColor(127);
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
