#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    //This example shows how to use the OS X specific
    //video grabber to record video and audio synced to disk
    
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //create a new recorder objects
    vidRecorder = ofPtr<ofQTKitGrabber>( new ofQTKitGrabber() );
    //make this our internal grabber
    vidGrabber.setGrabber(vidRecorder);
    
    videoDevices = vidRecorder->listVideoDevices();
    audioDevices = vidRecorder->listAudioDevices();
    
    //optionally add audio to the recording stream
//    vidRecorder->setAudioDeviceID(2);
    vidRecorder->setUseAudio(true);
    
	//register for events so we'll know when videos finish saving
	ofAddListener(vidRecorder->videoSavedEvent, this, &testApp::videoSaved);
	
	//use this to report all the available video codecs on your system
//	vector<string> videoCodecs = vidRecorder->listVideoCodecs();
//	for(int i = 0; i < videoCodecs(); i++){
//		cout << videoCodecs[i] << endl;
//	}
	
	//you can set a custom codec if you want like this, it'll change how the final video is saved
//	vidRecorder->setVideoCodec("QTCompressionOptionsJPEGVideo");
//	vidRecorder->setVideoCodec(videoCodecs[2]);
	

    //optionally remove preview
	//if you don't need to show the video on screen
	//or are seeingglitchy recordings this may help
	previewPixels = true;
    if(previewPixels){
        vidGrabber.initGrabber(640, 480);
    }
    else{
        //passing -1 let's use default the width and height to the native camera resolution
        vidRecorder->initGrabberWithoutPreview();
    }
	
	//need to set up recording for this grabber
	//call this once after you've initialized the grabber
	vidRecorder->initRecording();


}


//--------------------------------------------------------------
void testApp::update(){
	
	ofBackground(60,60,60);
	
	vidGrabber.update();
    if(recordedVideoPlayback.isLoaded()){
        recordedVideoPlayback.update();
    }
}

//--------------------------------------------------------------
void testApp::draw(){
	if(vidRecorder->hasPreview()){
		vidGrabber.draw(20,20);
    }
	else{
		ofPushStyle();
		//x out to show there is no video preview
		ofSetLineWidth(4);
		ofLine(20, 20, 640+20, 480+20);
		ofLine(20+640, 20, 20, 480+20);
		ofPopStyle();
	}
    //no pixels to draw
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(4);
    if(vidRecorder->isRecording()){
        //make a nice flashy red record color
        int flashRed = powf(1 - (sin(ofGetElapsedTimef()*10)*.5+.5),2)*255;
		ofSetColor(255, 255-flashRed, 255-flashRed);
    }
    else{
    	ofSetColor(0);
    }
    ofRect(20,20,640,480);
    ofPopStyle();
    
    ofRect(20+640,20,640,480);
    if(recordedVideoPlayback.isLoaded()){
        recordedVideoPlayback.draw(20+640, 20);
    }
    
    //draw instructions
    ofPushStyle();
    ofSetColor(255);
    ofDrawBitmapString("' ' space bar to toggle recording", 680, 540);
    ofDrawBitmapString("'v' switches video device", 680, 560);
    ofDrawBitmapString("'a' swiches audio device", 680, 580);
    
    //draw video device selection
    ofDrawBitmapString("VIDEO DEVICE", 20, 540);
    for(int i = 0; i < videoDevices.size(); i++){
        if(i == vidRecorder->getVideoDeviceID()){
			ofSetColor(255, 100, 100);
        }
        else{
            ofSetColor(255);
        }
        ofDrawBitmapString(videoDevices[i], 20, 560+i*20);
    }
    
    //draw audio device;
    int startY = 580+20*videoDevices.size();
    ofDrawBitmapString("AUDIO DEVICE", 20, startY);
    startY += 20;
    for(int i = 0; i < audioDevices.size(); i++){
        if(i == vidRecorder->getAudioDeviceID()){
			ofSetColor(255, 100, 100);
        }
        else{
            ofSetColor(255);
        }
        ofDrawBitmapString(audioDevices[i], 20, startY+i*20);
    }
    ofPopStyle();
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

	if(key == ' '){
        
        //if it is recording, stop
        if(vidRecorder->isRecording()){
            vidRecorder->stopRecording();
        }
        //otherwise start a new recording.
        //Also we have to close the current video or we wont be able to write over it
        else {
            if(recordedVideoPlayback.isLoaded()){
                recordedVideoPlayback.close();
            }
	        vidRecorder->startRecording("MyMovieFile.mov");
        }
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	if(key == 'v'){
		vidRecorder->setVideoDeviceID( (vidRecorder->getVideoDeviceID()+1) % videoDevices.size() );
    }
	if(key == 'a'){
        vidRecorder->setAudioDeviceID( (vidRecorder->getAudioDeviceID()+1) % audioDevices.size() );
    }
}

//--------------------------------------------------------------
void testApp::videoSaved(ofVideoSavedEventArgs& e){
	// the ofQTKitGrabber sends a message with the file name when the video is done
	if(e.error == ""){
	    recordedVideoPlayback.loadMovie(e.videoPath);
    	recordedVideoPlayback.setLoopState(OF_LOOP_NORMAL);
	    recordedVideoPlayback.play();
	}
	else {
		ofLogError("videoSaved") << "Video save error " << e.error << endl;
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

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
