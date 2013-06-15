#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(255,255,255);

	testFont.loadFont("Batang.ttf", 160, true, true, true);
	testFont2.loadFont("cooperBlack.ttf", 52, true, true, true);

	letter = '$';
	testChar = testFont.getCharacterAsPoints(letter);
	ofSetFullscreen(false);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0, 90, 60);
	ofFill();

//	ofBeginShape();
//		ofVertex(100, 100);
//		ofVertex(200, 200);
//		ofVertex(100, 200);
//	ofEndShape(true);
//
	ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate()), 10, 10);
	ofDrawBitmapString("press a key to see it as a texture and as a vector. ", 10, 24);

	testFont2.drawString("Hello - I am bitmap", 15, 400);

	ofFill();
	testFont2.drawStringAsShapes("Hello - I am vector", 15, 480);
	ofNoFill();
	testFont2.drawStringAsShapes("Hello - I am vector", 15, 550);

	//lets draw the key pressed as a tex and a vector both fill and no fill
	//here we show how easy it is to get

	string str = "";
	str += char(letter);

	testFont.drawString(str, 50, 250);

	//okay lets get the character back as shapes
	testChar.setFilled(true);
    testChar.draw(200,250);
    testChar.setFilled(false);
    testChar.draw(350,250);


    // we can also access the individual points
	ofFill();
	ofPushMatrix();
		ofTranslate(550, 250, 0);
		ofBeginShape();
			for(int k = 0; k <(int)testChar.getOutline().size(); k++){
				if( k!= 0)ofNextContour(true) ;
				for(int i = 0; i < (int)testChar.getOutline()[k].size(); i++){
					ofVertex(testChar.getOutline()[k].getVertices()[i].x, testChar.getOutline()[k].getVertices()[i].y);
				}
			}
		ofEndShape( true );
	ofPopMatrix();

	ofNoFill();
	ofPushMatrix();
		ofTranslate(700, 250, 0);
		ofBeginShape();
			for(int k = 0; k <(int)testChar.getOutline().size(); k++){
				if( k!= 0)ofNextContour(true) ;
				for(int i = 0; i < (int)testChar.getOutline()[k].size(); i++){
					ofVertex(testChar.getOutline()[k].getVertices()[i].x, testChar.getOutline()[k].getVertices()[i].y);
				}
			}
		ofEndShape( true );
	ofPopMatrix();


}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	if(key==OF_KEY_ESC) return;
	testChar = testFont.getCharacterAsPoints(key);
	letter = key;
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){
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
