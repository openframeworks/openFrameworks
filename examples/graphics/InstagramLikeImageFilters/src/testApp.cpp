#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	dir.allowExt("cube");
	dir.listDir("LUTs/");
	dir.sort();
	if (dir.size()>0) {
		dirLoadIndex=0;
		loadLUT(dir.getPath(dirLoadIndex));
		doLUT = true;
	}else{
		doLUT = false;
	}
	
	lutImg.allocate(640, 480, OF_IMAGE_COLOR);
	
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(640,480);

	
	thumbPos.set(lutImg.getWidth()*0.5f-80, -lutImg.getHeight()*0.5f - 60, 0);
	lutPos.set(-lutImg.getWidth()*0.5f, -lutImg.getHeight()*0.5f, 0);

	ofBackground(0);
	ofSetColor(255);
}

//--------------------------------------------------------------
void testApp::update(){
	vidGrabber.grabFrame();
	
	if (doLUT) {
		if (vidGrabber.isFrameNew()){
			applyLUT(vidGrabber.getPixelsRef());
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackgroundGradient(ofColor::gray, ofColor::black);
	
	ofTranslate(ofGetWidth()*0.5f, ofGetHeight()*0.5f, 0);
	
	if(doLUT){
		
		lutImg.draw(lutPos.x, lutPos.y);
		ofRect(thumbPos.x-3, thumbPos.y-3, 166, 126);
		vidGrabber.draw(thumbPos.x, thumbPos.y, 160, 120);
		
		ofDrawBitmapString(dir.getName(dirLoadIndex), lutPos.x, -lutPos.y+50);
		
		ofDrawBitmapString("Use the up and down arrows of your keyboard \nto go through the different filters", lutPos.x, -lutPos.y+100);
		
	}else {
			vidGrabber.draw(0,0);
	}
}

//--------------------------------------------------------------
void testApp::loadLUT(string path){
	LUTloaded=false;
	
	ofBuffer buffer = ofBufferFromFile(path);
	for (int i=0; i<5; i++) {
		cout << "Skipped line: " << buffer.getNextLine()<<endl;
	}
	
	for(int z=0; z<32; z++){
		for(int y=0; y<32; y++){
			for(int x=0; x<32; x++){
				string content = buffer.getNextLine();
				
				vector <string> splitString = ofSplitString(content, " ", true, true);
				if (splitString.size() >=3) {
					lut[x][y][z] = ofVec3f(ofToFloat(splitString[0]),ofToFloat(splitString[1]), ofToFloat(splitString[2]));
					cout << lut[x][y][z] << endl;	
				}
				
				if (buffer.isLastLine()) {
					cout << "ofxLUT load finished" << endl;
					
					return;
				}
			}
		}
	}    
	LUTloaded = true;
}
//--------------------------------------------------------------
void testApp::applyLUT(ofPixelsRef pix){
	if (LUTloaded) {
		
		ofPixelsRef pixels = pix;
		
		for(int y = 0; y < pix.getHeight(); y++){
			for(int x = 0; x < pix.getWidth(); x++){
				
				ofColor color = pix.getColor(x, y);
				
				int lutPos [3];
				for (int m=0; m<3; m++) {
					lutPos [m] = floor(color[m]/8.0f);
					if (lutPos[m]==31) {
						lutPos[m]=30;
					}
				}
				
				ofVec3f temp = lut[lutPos[0]][lutPos[1]][lutPos[2]];
				ofVec3f temp2 =lut[lutPos[0]+1][lutPos[1]+1][lutPos[2]+1]; 
				
				for (int k=0; k<3; k++) {
					color[k]= ( temp[k] + ((color[k]%8)/8.0f)* (temp2[k] - temp[k]))*255 ;
				}
				
				pixels.setColor(x, y, color);
				
			}
			lutImg.setFromPixels(pixels);
			
		}
	}
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	switch (key) {
		case ' ':
			doLUT^=true;
			break;
		case OF_KEY_UP:
			dirLoadIndex++;
			if (dirLoadIndex>=dir.size()) {
				dirLoadIndex=0;
			}
			loadLUT(dir.getPath(dirLoadIndex));
			
			break;
		case OF_KEY_DOWN:
			dirLoadIndex--;
			if (dirLoadIndex<0) {
				dirLoadIndex=dir.size()-1;
			}
			loadLUT(dir.getPath(dirLoadIndex));
			break;
		default:
			break;
	}
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

