#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(255,255,255);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();

    // ofSetOrientation(OF_ORIENTATION_90_LEFT);

    // SHADERS
    bool loadok = font.load("verdana.ttf", 200, true, false, true, 0.4, 72);
	shader.load("shaders/noise.vert", "shaders/noise.frag");

	auto textShapes = font.getStringAsPoints("openFrameworks");
	for (auto glyph : textShapes) {
		text.append(glyph);
	}

	ofRectangle boundingBox;
	for (auto outline : text.getOutline()) {
		boundingBox = boundingBox.getUnion(outline.getBoundingBox());
	}

	boundingBox.alignTo(ofGetCurrentViewport(), OF_ALIGN_HORZ_CENTER, OF_ALIGN_VERT_CENTER);
	glm::vec2 textPos(boundingBox.getX(), boundingBox.getMaxY());

	text.translate(textPos);
	text.setFillColor(ofColor(245, 58, 135));

	doShader = false;


    // SOUNDPLAYER
	loadok = synth.loadSound("sounds/synth.wav");
	loadok = beats.loadSound("sounds/1085.mp3");
	loadok = vocals.loadSound("sounds/Violet.mp3");
    synth.setVolume(1.0f);
    beats.setVolume(0.75f);
    vocals.setVolume(1.0f);
    synth.setMultiPlay(true);
    beats.setMultiPlay(false);
    vocals.setMultiPlay(true);


    // SOUNDSTREAM

//     Ask for permission to record audio,
//     not needed if no in channels used
    ofxAndroidRequestPermission(OFX_ANDROID_PERMISSION_RECORD_AUDIO);
    bool ok = ofxAndroidCheckPermission(OFX_ANDROID_PERMISSION_RECORD_AUDIO);

    sampleRate 			= 44100;
    phase 				= 0;
    phaseAdder 			= 0.0f;
    phaseAdderTarget 	= 0.0f;
    volume				= 0.1f;
    bNoise 				= false;
	initialBufferSize = 256;

	lAudio = new float[initialBufferSize];
	rAudio = new float[initialBufferSize];

	memset(lAudio, 0, initialBufferSize * sizeof(float));
	memset(rAudio, 0, initialBufferSize * sizeof(float));


    ofSoundStreamSettings settings;
    settings.setOutListener(this);
    settings.setInListener(this);
    settings.numOutputChannels = 2;
    settings.numInputChannels = 2;
    settings.numBuffers = 4;
    settings.bufferSize = initialBufferSize;
    soundStream.setup(settings);

}

void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::update(){
    // update the sound playing system:
    //ofSoundUpdate();
}

//--------------------------------------------------------------
void ofApp::draw(){

    int r = 128 + 50 * cosf(ofGetElapsedTimef());
    int g = 0;
    int b = 128 + 50 * sinf(ofGetElapsedTimef());

    ofBackground(r,g,b);

	if( doShader ){
		shader.begin();
		//we want to pass in some varrying values to animate our type / color
		shader.setUniform1f("timeValX", ofGetElapsedTimef() * 0.1 );
		shader.setUniform1f("timeValY", -ofGetElapsedTimef() * 0.18 );

		//we also pass in the mouse position
		//we have to transform the coords to what the shader is expecting which is 0,0 in the center and y axis flipped.
		shader.setUniform2f("mouse", ofGetMouseX() - ofGetWidth()/2, ofGetHeight()/2-ofGetMouseY() );

	}


    // draw the SoudStream audio waves
    // draw the left:
    ofBeginShape();
    for (int i = 0; i < initialBufferSize; i++){
        ofVertex(20+i*10,ofGetHeight() / 2 - 250 + lAudio[i]*500.0f);
    }
    ofEndShape(false);

    ofBeginShape();
    for (int i = 0; i < initialBufferSize; i++){
        ofVertex(20+i*10,ofGetHeight() / 2 + 250 + rAudio[i]*500.0f);
    }
    ofEndShape(false);

    //finally draw our text
    text.draw();

    if( doShader ){
        shader.end();
    }

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
	doShader = true;

    float widthStep = ofGetWidth() / 3.0f;
    if (x < widthStep){
        float pct = x / widthStep;
        synth.play();
        synth.setSpeed( 0.1f + ((float)(ofGetHeight() - y) / (float)ofGetHeight())*2.0f);
        synth.setPan(2.0*pct-1.0);
    }
    else if (x >= widthStep && x < widthStep*2){
        beats.play();
    }
    else {
        vocals.play();
        vocals.setSpeed( 0.1f + ((float)(ofGetHeight() - y) / (float)ofGetHeight())*2.0f);
        vocals.setPan(2.0*(float)(x - widthStep*2) / (float)widthStep-1.0);
    }
}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){
	// continuously control the speed of the beat sample via drag,
	// when in the "beat" region:
	float widthStep = ofGetWidth() / 3.0f;
	if (x >= widthStep && x < widthStep*2){
		beats.setSpeed( 0.5f + ((float)(ofGetHeight() - y) / (float)ofGetHeight())*1.0f);
	}

	int width = ofGetWidth();
	pan = (float)x / (float)width;
	float height = (float)ofGetHeight();
	float heightPct = ((height-y) / height);
	targetFrequency = 2000.0f * heightPct;
	phaseAdderTarget = (targetFrequency / (float) sampleRate) * glm::two_pi<float>();
}

//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){
	//doShader = false;

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

void ofApp::audioOut(ofSoundBuffer & buffer){
	//pan = 0.5f;
	float leftScale = 1 - pan;
	float rightScale = pan;

	// sin (n) seems to have trouble when n is very large, so we
	// keep phase in the range of 0-TWO_PI like this:
	while (phase > glm::two_pi<float>()){
		phase -= glm::two_pi<float>();
	}

	if ( bNoise == true){
		// ---------------------- noise --------------
		for (int i = 0; i < buffer.getNumFrames(); i++){
			lAudio[i] = buffer.getSample(i, 0) = ofRandomf() * volume * leftScale;
			rAudio[i] = buffer.getSample(i, 1) = ofRandomf() * volume * rightScale;
		}
	} else {

		for (int i = 0; i < buffer.getNumFrames(); i++){
			phaseAdder = 0.6f * phaseAdder + 0.4f * phaseAdderTarget;
			phase += phaseAdder;
			float sample = sin(phase);
			lAudio[i%256] = buffer.getSample(i, 0) = sample * volume * leftScale;
			buffer.getSample(i, 1) = sample * volume * rightScale;
		}
	}
}


void ofApp::audioIn(ofSoundBuffer & buffer){
	for (int i = 0; i < buffer.getNumFrames(); i++){
		rAudio[i%256] = buffer.getSample(i, 0) + buffer.getSample(i, 1) ;
	}
}

//--------------------------------------------------------------
void ofApp::deviceRefreshRateChanged(int refreshRate) {
}

//--------------------------------------------------------------
void ofApp::deviceHighestRefreshRateChanged(int refreshRate) {
}

//--------------------------------------------------------------
void ofApp::deviceRefreshRateChangedEvent(int &refreshRate) {
}

//--------------------------------------------------------------
void ofApp::deviceHighestRefreshRateChangedEvent(int &refreshRate) {
}

