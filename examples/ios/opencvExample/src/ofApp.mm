#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){	
	//ofSetOrientation(OF_ORIENTATION_90_RIGHT);//Set iOS to Orientation Landscape Right

	capW = 320;
	capH = 240;

	#ifdef _USE_LIVE_VIDEO
		vidGrabber.initGrabber(capW, capH);
		capW = vidGrabber.getWidth();
		capH = vidGrabber.getHeight();
    #else	
        vidPlayer.loadMovie("fingers.m4v");
        vidPlayer.setLoopState(OF_LOOP_NORMAL);
		vidPlayer.play();
	#endif

    colorImg.allocate(capW,capH);
    grayImage.allocate(capW,capH);
    grayBg.allocate(capW,capH);
    grayDiff.allocate(capW,capH);	

	bLearnBakground = true;
	threshold = 80;
	
	ofSetFrameRate(20);
}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(100,100,100);

    bool bNewFrame = false;

	#ifdef _USE_LIVE_VIDEO
       vidGrabber.update();
	   bNewFrame = vidGrabber.isFrameNew();
    #else
        vidPlayer.update();
        bNewFrame = vidPlayer.isFrameNew();    
    #endif


    
	if (bNewFrame){
	
		#ifdef _USE_LIVE_VIDEO
			if( vidGrabber.getPixels() != NULL ){
		#else
			if( vidPlayer.getPixels() != NULL && vidPlayer.getWidth() > 0 ){
		#endif

			#ifdef _USE_LIVE_VIDEO
				colorImg.setFromPixels(vidGrabber.getPixels(), capW, capH);
			#else
				colorImg.setFromPixels(vidPlayer.getPixels(), capW, capH);
			#endif

			grayImage = colorImg;
			if (bLearnBakground == true){
				grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
				bLearnBakground = false;
			}

			// take the abs value of the difference between background and incoming and then threshold:
			grayDiff.absDiff(grayBg, grayImage);
			grayDiff.threshold(threshold);

			// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
			// also, find holes is set to true so we will get interior contours as well....
			contourFinder.findContours(grayDiff, 20, (capW*capH)/3, 10, true);	// find holes
		
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){	
	
	ofSetColor(255);
	ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, 20);
	
	ofPushMatrix();
	ofScale(0.5, 0.5, 1);

	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetHexColor(0xffffff);
	grayImage.draw(0,0);
	grayBg.draw(capW+4, 0);
	grayDiff.draw(0, capH + 4);

	// lets draw the contours. 
	// this is how to get access to them:
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(0, capH + 4);
    }

	ofPopMatrix();
	// finally, a report:

	ofSetHexColor(0xffffff);
	char reportStr[1024];
	sprintf(reportStr, "bg subtraction and blob detection\npress ' ' to capture bg\nthreshold %i\nnum blobs found %i, fps: %f", threshold, contourFinder.nBlobs, ofGetFrameRate());
	ofDrawBitmapString(reportStr, 4, 380);
}
    
//--------------------------------------------------------------
void ofApp::exit(){
        
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
	bLearnBakground = true;
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){
        
}
    
//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){
        
}
    
//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){
        
}
    
//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){
        
}
    
//--------------------------------------------------------------
void ofApp::lostFocus(){
        
}
    
//--------------------------------------------------------------
void ofApp::gotFocus(){
        
}
    
//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){
        
}
    
//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){
        
}
