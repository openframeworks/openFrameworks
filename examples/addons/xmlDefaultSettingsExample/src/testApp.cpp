#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	
	// initialize the default Xml settings file.
	// this method load or create our default xml to set openframeworks settings.
	// It will saved to ofFilePath::getCurrentWorkingDirectory()
	XML.init();
	// also you can choose a name for your default file.
	//defaultXml.init("wngTest_1.xml");
	
	
	// set the openFrameworks core variables like:
	// - window position,
	// - window size,
	// - framerate,
	XML.setSettings();
	
	
	// add custom tags
	/*if(!XML.tagExists("customSettings", 0)){
		cout << "### No\n";
		XML.load();
		XML.addTag("customSettings");
		XML.pushTag("customSettings", 0);
			XML.addValue("counterRuns", 0);
			XML.addValue("lastRun", 0);
			XML.addValue("myTag", 0);
		XML.popTag();
		XML.save();
	}*/
}

//--------------------------------------------------------------
void testApp::update(){
	
}

//--------------------------------------------------------------
void testApp::draw(){

	
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

	
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

//--------------------------------------------------------------
void testApp::exit(){
	
	XML.getSettings();
}
