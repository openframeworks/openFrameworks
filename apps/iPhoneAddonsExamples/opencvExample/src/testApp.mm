#include "testApp.h"

#define _USE_LIVE_VIDEO

//--------------------------------------------------------------
void testApp::setup(){	
	//ofxiPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT);

	#ifdef _USE_LIVE_VIDEO
		vidGrabber.initGrabber(vidGrabber.getWidth(), vidGrabber.getHeight());
		colorImg.allocate(vidGrabber.getWidth(),vidGrabber.getHeight());
		grayImage.allocate(vidGrabber.getWidth(),vidGrabber.getHeight());
		grayBg.allocate(vidGrabber.getWidth(),vidGrabber.getHeight());
		grayDiff.allocate(vidGrabber.getWidth(),vidGrabber.getHeight());		
	#else
		vidPlayer.loadMovie("fingers.m4v");
		vidPlayer.play();
		vidGrabber.initGrabber(vidPlayer.getWidth(), vidPlayer.getHeight());
		colorImg.allocate(vidPlayer.getWidth(),vidPlayer.getHeight());
		grayImage.allocate(vidPlayer.getWidth(),vidPlayer.getHeight());
		grayBg.allocate(vidPlayer.getWidth(),vidPlayer.getHeight());
		grayDiff.allocate(vidPlayer.getWidth(),vidPlayer.getHeight());		
	#endif
	

	bLearnBakground = true;
	threshold = 80;
	
	ofSetFrameRate(20);
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(100,100,100);

    bool bNewFrame = false;

	#ifdef _USE_LIVE_VIDEO
       vidGrabber.update();
	   bNewFrame = vidGrabber.isFrameNew();
    #else
        vidPlayer.update();
        bNewFrame = vidPlayer.isFrameNew();
	#endif

	//if (bNewFrame){
	
		printf("bNewFrame \n");

	#ifdef _USE_LIVE_VIDEO
		if( vidGrabber.getPixels() != NULL ){
    #else
		if( vidPlayer.getPixels() != NULL && vidPlayer.getWidth() > 0 ){
	#endif

			printf("yo yo \n");

			#ifdef _USE_LIVE_VIDEO
				colorImg.setFromPixels(vidGrabber.getPixels(), vidGrabber.getWidth(),vidGrabber.getHeight());
			#else
				colorImg.setFromPixels(vidPlayer.getPixels(), vidPlayer.getWidth(), vidPlayer.getHeight());
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
			contourFinder.findContours(grayDiff, 20, (340*240)/3, 10, true);	// find holes
		
		}
	//}
}

//--------------------------------------------------------------
void testApp::draw(){	
	
	ofSetColor(255);
	ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, 20);
	
	ofScale(0.5, 0.5, 1);

	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetHexColor(0xffffff);
	colorImg.draw(20,20);
	grayImage.draw(360,20);
	grayBg.draw(20,280);
	grayDiff.draw(360,280);

	// then draw the contours:

	ofFill();
	ofSetHexColor(0x333333);
	ofRect(360,540,320,240);
	ofSetHexColor(0xffffff);

	// we could draw the whole contour finder
	//contourFinder.draw(360,540);

//	// or, instead we can draw each blob individually,
//	// this is how to get access to them:
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(360,540);
    }

	// finally, a report:

	ofSetHexColor(0xffffff);
	char reportStr[1024];
	sprintf(reportStr, "bg subtraction and blob detection\npress ' ' to capture bg\nthreshold %i (press: +/-)\nnum blobs found %i, fps: %f", threshold, contourFinder.nBlobs, ofGetFrameRate());
	ofDrawBitmapString(reportStr, 20, 600);
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){
	bLearnBakground = true;
}

//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch){

}

