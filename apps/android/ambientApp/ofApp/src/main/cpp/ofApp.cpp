#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofBackground(40);
    ofSetVerticalSync(false);
    ofEnableAlphaBlending();
    ofSetLogLevel(OF_LOG_VERBOSE);

    shader.load("shaders/noise.vert", "shaders/noise.frag");
    bUseShader = true;

    //we load a font and tell OF to make outlines so we can draw it as GL shapes rather than textures
    fontSize = ofGetWidth() / 4;
    font.load("type/verdana.ttf", fontSize, true, false, true, 0.4, 72);

    ofRectangle rect = font.getStringBoundingBox("meditate", 0, 0);   // size of text.
    mousePoint.x = (ofGetWidth() - rect.width) * 0.5 + 200;  // position in center screen
    mousePoint.y = (ofGetHeight() - rect.height) * 0.5;

    phase = 1;

    //----- Loading sound player begin -------.

    ofDirectory dir;
    sizedir1 = dir.listDir("sounds");
    for (int i = 0; i < sizedir1; i++) {
        ofLogNotice() << ofToDataPath(dir.getPath(i), true);
    }

    bool bLoadAsync = false;

    players.resize(dir.size());

    for (int i = 0; i < sizedir1; i++) {
        cout << dir.getPath(i) << endl;
        players[i] = make_unique<ofxSoundPlayerObject>();
        players[i]->setLoop(true);
        if (bLoadAsync) {
            //when you use loadAsync you will not be able to call play immediately after calling loadAsync, as it will not allow for such.
            // so if you want to play the file immediately after it has finished loading you can pass a boolean as a second argument to loadAsync. True means play once loaded
            players[i]->loadAsync(ofToDataPath(dir.getPath(i)), true);
        } else {
            players[i]->load(ofToDataPath(dir.getPath(i)));
            players[i]->play();
            players[i]->setLoop(true);
        }
        players[i]->connectTo(mixer);
    }
    /*   */

    noise.connectTo(filter).connectTo(delay).connectTo(mixer);

    ofDirectory dir2;
    dir2.listDir("sounds2");
    sizedir2 = dir2.size();
    for(int i = 0; i < sizedir2; i ++){
        ofLogNotice() << ofToDataPath(dir.getPath(i), true );
    }

    playersphase2.resize(sizedir2);

    for(int i = 0; i < dir2.size(); i++){
        cout << dir2.getPath(i) << endl;
        playersphase2[i] = make_unique<ofxSoundPlayerObject>();
        playersphase2[i]->load(ofToDataPath(dir2.getPath(i)));
        playersphase2[i]->play();
        playersphase2[i]->setLoop(true);
        playersphase2[i]->connectTo(mixer);
    }


    //----- Loading sound player end -------.

    delay.setFeedback(0.8);
    delay.setDelay(8192);


    mixer.setConnectionVolume(0, 1.); // medidate
    mixer.setConnectionVolume(1, 1.); // gamelan
    mixer.setConnectionVolume(2, 0.5); // noise
    mixer.setConnectionVolume(3, 0.); // be here now
    mixer.setConnectionVolume(4, 0.); //

    //----- Sound stream setup begin -------.
    ofSoundStreamSettings soundSettings;
    soundSettings.numInputChannels = 0;
    soundSettings.numOutputChannels = 2;
    soundSettings.sampleRate = 44100; //player.getSoundFile().getSampleRate();
    soundSettings.bufferSize = 256;
    soundSettings.numBuffers = 1;
    stream.setup(soundSettings);

    stream.setOutput(mixer);


}
//--------------------------------------------------------------
void ofApp::playerEnded(size_t & id){
    // This function gets called when the player ends. You can do whatever you need to here.
    // This event happens in the main thread, not in the audio thread.
    cout << "the player's instance " << id << "finished playing" << endl;
    
}
//--------------------------------------------------------------
void ofApp::exit(){
    stream.close();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	    
    //ofSetOrientation(OF_ORIENTATION_90_RIGHT);//Set iOS to Orientation Landscape Right
    
    ofPushStyle();
    ofSetColor(245, 58, 135);
    ofFill();
    
    if(bUseShader) {
    
        shader.begin();
        shader.setUniform1f("timeValX", ofGetElapsedTimef() * 0.1 );     // we want to pass in some varrying 
                                                                        // values to animate our type / color
        shader.setUniform1f("timeValY", -ofGetElapsedTimef() * 0.18 );
        shader.setUniform2f("mouse", mousePoint.x, mousePoint.y);       // we also pass in the mouse position
    }


    if (phase==1) {
        ofRectangle rect = font.getStringBoundingBox("meditate", 0, 0);   // size of text.
        int y_offset = 1.1*rect.height;
        int x = (ofGetWidth() - rect.width) * 0.5;                              // position in center screen.
        int padding = 1.1*fontSize;
        for(int y=y_offset; y<ofGetHeight(); y+=padding) {
            font.drawStringAsShapes("meditate", x, y);
        }

    }
    else {
        ofRectangle rect = font.getStringBoundingBox("Be Here Now", 0, 0);   // size of text.
        int x = (ofGetWidth() - rect.width) * 0.5;                              // position in center screen
        int y = (ofGetHeight() - rect.height) * 0.5 + fontSize/2;

        int x_step = ofGetWidth() / 4;
        int y_step = ofGetHeight() / 4;
        x = 20;
        int y_offset = fmax(1.1*fontSize, ofGetHeight() / 6);
        int delta_y = 1.2*fontSize;
        font.drawStringAsShapes("Be", x, y_offset);

        x = x_step;
        y = y_offset + delta_y;
        font.drawStringAsShapes("Here", x, y);

        x = x_step*2 ;
        y = y + delta_y;
        font.drawStringAsShapes("Now", x, y);


        x = 20 ; y = y_step*2.5;
        font.drawStringAsShapes("Mars", x, y);

        x = x_step / 2 ; y = y + delta_y;
        font.drawStringAsShapes("College", x, y);


        //font.drawStringAsShapes("Be Here Now", x, y);
    }

    if(bUseShader) {
        shader.end();
    }

    ofPopStyle();

}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::touchDown(int x, int y, int id){
    bUseShader = true;

}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){
    // we have to transform the coords to what the shader is expecting which is 0,0 in the center and y axis flipped.
    float height = ofGetHeight();

    mousePoint.x = (x * 2 - ofGetWidth())*2;      // strange : in Portrait mode => multiply by 2
    //mousePoint.x = (x * 2 - ofGetWidth())*0.5;  //           in Landscape mode => multiply by 0.5
    mousePoint.y = ofGetHeight() - y * 2;
	
    float f = ofMap(x, 0, ofGetWidth(), 0, 1, true);
    // give the frequency a logarithmic scale.
    f = f * f;
    filterCutoff = ofMap(f, 0, 1, 50, 7000, true);
    
    filter.setParameters(filterCutoff, 0.9);
    
    delayFeedback = ofMap(y, 0, ofGetHeight(), 0.99, 0.1, true);
    delay.setFeedback(delayFeedback);
    
    if ( x<160 && y>(ofGetHeight() - 160) && phase==1) {
        phase = 2;
        mixer.setConnectionVolume(2, 0.);
    }

    if (phase == 2) {
        float vol1 = (ofGetHeight() - y) / height ;
        mixer.setConnectionVolume(0,  (1.0 - vol1)*(1.0 - vol1));
        mixer.setConnectionVolume(1,  (1.0 - vol1)*(1.0 - vol1));
        mixer.setConnectionVolume(3,  vol1);
        mixer.setConnectionVolume(4,  vol1);

        if (x>(ofGetWidth()-160) && y>(ofGetHeight()-160)) {
            phase = 1;
            mixer.setConnectionVolume(0, 1.); // medidate
            mixer.setConnectionVolume(1, 1.); // gamelan
            mixer.setConnectionVolume(2, 0.5); // noise
            mixer.setConnectionVolume(3, 0.); // be here now
            mixer.setConnectionVolume(4, 0.); //
        }
    }
    

}

//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

//--------------------------------------------------------------
void ofApp::pause(){

}

//--------------------------------------------------------------
void ofApp::stop(){

}

//--------------------------------------------------------------
void ofApp::resume(){

}

//--------------------------------------------------------------
void ofApp::reloadTextures(){

}

//--------------------------------------------------------------
bool ofApp::backPressed(){
	return false;
}

//--------------------------------------------------------------
void ofApp::okPressed(){

}

//--------------------------------------------------------------
void ofApp::cancelPressed(){

}

void ofApp::deviceRefreshRateChanged(int refreshRate) {

}

void ofApp::deviceHighestRefreshRateChanged(int refreshRate) {

}

void ofApp::deviceRefreshRateChangedEvent(int &refreshRate) {

}

void ofApp::deviceHighestRefreshRateChangedEvent(int &refreshRate) {

}
