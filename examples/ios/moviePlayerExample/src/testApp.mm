#include "testApp.h"

#import "VideoPlayerControls.h"
#import "VideoPlayerControlsDelegateForOF.h"
#import "AVFoundationVideoPlayer.h"

//-------------------------------------------------------------- video controls.
static VideoPlayerControls * controls = nil;
static bool bPlayingBeforeScrub = false;
static bool bScrubbing = false;

//--------------------------------------------------------------
void testApp::setup() {
	ofSetFrameRate(30);
	ofBackground(225, 225, 225);
    
	video.loadMovie("hands.m4v");
	video.play();
    
    AVFoundationVideoPlayer * avVideoPlayer;
    avVideoPlayer = (AVFoundationVideoPlayer *)video.getAVFoundationVideoPlayer();
    [avVideoPlayer setVideoPosition:CGPointMake(0, 240)];
    [ofxiPhoneGetViewController().view insertSubview:avVideoPlayer.playerView belowSubview:controls.view];
    avVideoPlayer.playerView.hidden = YES;
    
    controls = [[VideoPlayerControls alloc] init];
    controls.delegate = [[VideoPlayerControlsDelegateForOF alloc] initWithApp:this];
    [ofxiPhoneGetViewController().view addSubview:controls.view];
}

//--------------------------------------------------------------
void testApp::update(){
    if(!video.isLoaded()) {
        return;
    }
    
    video.update();

    [controls setNewFrame:video.isFrameNew()];
    
    float position = video.getPosition();
    int timeInSeconds = video.getDuration() * position;
    
    if(!bScrubbing) {   // only update slider position when not already scrubbing.
        [controls setPosition:position];
    }
    [controls setTimeInSeconds:timeInSeconds];
    
    if(video.getIsMovieDone()) {
        [controls setPlay:video.isPlaying()];
    }
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofSetColor(255);
    video.getTexture()->draw(0, 0);
    
	if(video.isLoaded()){
        
        // let's move through the "RGB" char array
        // using the red pixel to control the size of a circle.
        
        unsigned char * pixels = video.getPixels();
        int videoW = video.getWidth();
        int videoH = video.getHeight();
        
		ofSetColor(54);
		for (int i = 4; i < videoW; i+=8){
			for (int j = 4; j < videoH; j+=8){
				unsigned char r = pixels[(j * 320 + i)*3];
				float val = 1 - ((float)r / 255.0f);
				ofCircle(i, 240 + j, 4 * val);
			}
		}
    }
    
    ofSetColor(0);
    if(video.getIsMovieDone()){
        ofSetHexColor(0xFF0000);
        ofDrawBitmapString("end of movie", 110, 360);
    }
}

//--------------------------------------------------------------
void testApp::playPressed() {
    video.setPaused(false);
    [controls setPlay:video.isPlaying()];
}

void testApp::pausePressed() {
    video.setPaused(true);
    [controls setPlay:video.isPlaying()];
}

void testApp::scrubBegin() {
    bScrubbing = true;
    
    bPlayingBeforeScrub = video.isPlaying();    // save the last play state.
    video.setPaused(true);
    
    [controls setPlay:video.isPlaying()];
}

void testApp::scrubToPosition(float position) {
    video.setPosition(position);
}

void testApp::scrubEnd() {
    if(bPlayingBeforeScrub) {
        video.setPaused(false);
    }
    
    [controls setPlay:video.isPlaying()];
    
    bScrubbing = false;
}

void testApp::loadPressed() {
    video.loadMovie("hands.m4v");
    video.play();
    
    AVFoundationVideoPlayer * avVideoPlayer;
    avVideoPlayer = (AVFoundationVideoPlayer *)video.getAVFoundationVideoPlayer();
    [avVideoPlayer setVideoPosition:CGPointMake(0, 240)];
    [ofxiPhoneGetViewController().view insertSubview:avVideoPlayer.playerView belowSubview:controls.view];
    avVideoPlayer.playerView.hidden = YES;
    
    [controls setLoad:YES];
}

void testApp::unloadPressed() {
    video.close();
    [controls setLoad:NO];
}

void testApp::loopOnPressed() {
    video.setLoopState(OF_LOOP_NORMAL);
    [controls setLoop:YES];
}

void testApp::loopOffPressed() {
    video.setLoopState(OF_LOOP_NONE);
    [controls setLoop:NO];
}

void testApp::nativeOnPressed() {
    [(AVFoundationVideoPlayer *)video.getAVFoundationVideoPlayer() playerView].hidden = NO;
    [controls setNative:YES];
}

void testApp::nativeOffPressed() {
    [(AVFoundationVideoPlayer *)video.getAVFoundationVideoPlayer() playerView].hidden = YES;
    [controls setNative:NO];
}

void testApp::muteOnPressed() {
    video.setVolume(0.0f);
    [controls setMute:YES];
}

void testApp::muteOffPressed() {
    video.setVolume(1.0f);
    [controls setMute:NO];
}

//--------------------------------------------------------------
void testApp::exit(){
    if(controls) {
        [controls.view removeFromSuperview];
        controls.delegate = nil;
        [controls release];
        controls = nil;
    }
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::lostFocus(){
    
}

//--------------------------------------------------------------
void testApp::gotFocus(){
    
}

//--------------------------------------------------------------
void testApp::gotMemoryWarning(){
    
}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){
    
}

