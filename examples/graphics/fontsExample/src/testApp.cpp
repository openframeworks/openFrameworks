#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	 
	ofBackground(54, 54, 54, 255);
	
	//old OF default is 96 - but this results in fonts looking larger than in other programs. 
	ofTrueTypeFont::setGlobalDpi(72);

	verdana14.loadFont("verdana.ttf", 14, true, true);
	verdana14.setLineHeight(18.0f);
	verdana14.setLetterSpacing(1.037);

	verdana30.loadFont("verdana.ttf", 30, true, true);
	verdana30.setLineHeight(34.0f);
	verdana30.setLetterSpacing(1.035);
	
	verdana14A.loadFont("verdana.ttf", 14, false);
	verdana14A.setLineHeight(18.0f);
	verdana14A.setLetterSpacing(1.037);

	franklinBook14.loadFont("frabk.ttf", 14);
	franklinBook14.setLineHeight(18.0f);
	franklinBook14.setLetterSpacing(1.037);

	franklinBook14A.loadFont("frabk.ttf", 14, false);
	franklinBook14A.setLineHeight(18.0f);
	franklinBook14A.setLetterSpacing(1.037);

	bFirst  = true;
	typeStr = "ABCDEFGHIJKLMNOPQRSTUVWXYZ\nabcdefghijklmnopqrstuvwxyz\n0123456789,:&!?";

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofSetColor(225);
	verdana14.drawString("Font Example - use keyboard to type", 30, 35);

	ofSetColor(245, 58, 135);
	verdana14.drawString("anti aliased", 155, 92);
	verdana14.drawString("anti aliased", 155, 195);
	verdana14A.drawString("aliased", 545, 92);
	
	ofSetColor(225);
	verdana14.drawString("verdana 14pt - ", 30, 92);
	verdana14.drawString(typeStr, 30, 111);

	verdana14A.drawString("verdana 14pt - ", 422, 92);
	ofRect(420, 97, 292, 62);
	ofSetColor(54, 54, 54);	
	verdana14A.drawString(typeStr, 422, 111);
	

	ofSetColor(29,29,29);
	ofLine(30, 169, ofGetWidth()-4, 169);

	ofSetColor(225);
	verdana14.drawString("verdana 30pt - ", 30, 195);
	verdana30.drawString(typeStr, 30, 229);

	ofSetColor(29,29,29);
	ofLine(30, 312, ofGetWidth()-4, 312);

	ofSetColor(245, 58, 135);
	franklinBook14.drawString("anti aliased", 169, 338);
	franklinBook14A.drawString("aliased", 560, 338);
	
	ofSetColor(225);
	franklinBook14.drawString("franklin book 14pt - ", 30, 338);
	franklinBook14.drawString(typeStr, 30, 358);

	franklinBook14A.drawString("franklin book 14pt - ", 422, 338);
	ofRect(420, 345, 292, 62);
	ofSetColor(54, 54, 54);	
	franklinBook14A.drawString(typeStr, 422, 358);

	ofSetColor(29,29,29);
	ofLine(30, 418, ofGetWidth()-4, 418);

	ofSetColor(225);	
	verdana14.drawString("ROTATION", 30, 445);
	verdana14.drawString("SCALE", 422, 445);
	
	ofPushMatrix();
		string rotZ = "Rotate Z";
		ofRectangle bounds = verdana30.getStringBoundingBox(rotZ, 0, 0);
		
		ofTranslate(110 + bounds.width/2, 500 + bounds.height / 2, 0);
		ofRotateZ(ofGetElapsedTimef() * -30.0);
				
		verdana30.drawString(rotZ, -bounds.width/2, bounds.height/2 );
	ofPopMatrix();

	ofPushMatrix();
		string scaleAA = "SCALE AA";
		bounds = verdana14.getStringBoundingBox(scaleAA, 0, 0);
		
		ofTranslate(500 + bounds.width/2, 480 + bounds.height / 2, 0);
		ofScale(2.0 + sin(ofGetElapsedTimef()), 2.0 + sin(ofGetElapsedTimef()), 1.0);
				
		verdana14.drawString(scaleAA, -bounds.width/2, bounds.height/2 );
	ofPopMatrix();	

	ofPushMatrix();
		string scaleA = "SCALE ALIASED";
		bounds = verdana14A.getStringBoundingBox(scaleA, 0, 0);
		
		ofTranslate(500 + bounds.width/2, 530 + bounds.height / 2, 0);
		ofScale(2.0 + cos(ofGetElapsedTimef()), 2.0 + cos(ofGetElapsedTimef()), 1.0);
				
		verdana14A.drawString(scaleA, -bounds.width/2, bounds.height/2 );
	ofPopMatrix();	
	
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){ 

	if(key == OF_KEY_DEL || key == OF_KEY_BACKSPACE){
		typeStr = typeStr.substr(0, typeStr.length()-1);
	}
	else if(key == OF_KEY_RETURN ){
		typeStr += "\n";
	}else{
		if( bFirst ){
			typeStr.clear();
			bFirst = false;
		}
		typeStr.append(1, (char)key);
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
