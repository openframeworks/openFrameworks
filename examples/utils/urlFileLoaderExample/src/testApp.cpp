#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    progress = 0;
    done = false;
    
    // synchronous loading
	html = ofLoadURL("http://www.openframeworks.cc").data;
    
    // add listeners + asynchronous saving to disk
    ofAddListener(ofURLProgressEvent(),this,&testApp::onProgress);
    ofAddListener(ofURLResponseEvent(),this,&testApp::onFinished);
    ofSaveURLAsync("http://www.openframeworks.cc/ofvideo.mov","ofvideo.mov");
}


//--------------------------------------------------------------
void testApp::onProgress(ofHttpResponse & response){
    progress = response.getProgressPct();
}

//--------------------------------------------------------------
void testApp::onFinished(ofHttpResponse & response){
    done = true;
    video.loadMovie(response.request.name);
    video.play();
}

//--------------------------------------------------------------
void testApp::update(){
    if(done) video.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    if(!done){
        if(progress>-1){
            ofNoFill();
            ofRect(20,20,100,20);
            ofFill();
            ofRect(20,20,progress*100,20);
        }else{
            ofDrawBitmapString("can't retrieve full size of download, downloading...", 20,20);
        }
    }else{
        ofDrawBitmapString("downloaded!", 20,20);
        video.draw(20,40);
    }
    
    
    ofDrawBitmapString(html, 500,20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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
