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
	vidGrabber.update();
	
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
	
	ofFile file(path);
	string line;
	for(int i = 0; i < 5; i++) {
		getline(file, line);
		ofLog() << "Skipped line: " << line;	
	}
	for(int z=0; z<32; z++){
		for(int y=0; y<32; y++){
			for(int x=0; x<32; x++){
				ofVec3f cur;
				file >> cur.x >> cur.y >> cur.z;
				lut[x][y][z] = cur;
			}
		}
	}
	
	LUTloaded = true;
}
//--------------------------------------------------------------
void testApp::applyLUT(ofPixelsRef pix){
	if (LUTloaded) {
		
		for(int y = 0; y < pix.getHeight(); y++){
			for(int x = 0; x < pix.getWidth(); x++){
				
				ofColor color = pix.getColor(x, y);
				
				int lutPos [3];
				for (int m=0; m<3; m++) {
					lutPos[m] = color[m] / 8;
					if (lutPos[m]==31) {
						lutPos[m]=30;
					}
				}
				
				ofVec3f start = lut[lutPos[0]][lutPos[1]][lutPos[2]];
				ofVec3f end = lut[lutPos[0]+1][lutPos[1]+1][lutPos[2]+1]; 
				
				for (int k=0; k<3; k++) {
					float amount = (color[k] % 8) / 8.0f;
					color[k]= (start[k] + amount * (end[k] - start[k])) * 255;
				}
				
				lutImg.setColor(x, y, color);
				
			}			
		}
		
		lutImg.update();
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
			if (dirLoadIndex>=(int)dir.size()) {
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

