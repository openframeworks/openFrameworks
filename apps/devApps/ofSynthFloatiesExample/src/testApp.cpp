#include "testApp.h"



//--------------------------------------------------------------
void testApp::setup(){

	ofBackground(0,0,0);

	// 2 output channels,
	// 0 input channels
	// 22050 samples per second
	// 256 samples per buffer
	// 4 num buffers (latency)

	sampleRate 			= 44100;
	volume				= 1.0f;
	lAudio = new float[1024];
	rAudio = new float[1024];
	
	baseNote = 61;
	scale = 0;
	
	ofSetLogLevel( OF_LOG_NOTICE );	
	ofSetCircleResolution( 60 );
	ofEnableSmoothing();
	ofEnableBlendMode( OF_BLENDMODE_ADD );
	ofSetFrameRate(60);
	
	bBackgroundAuto = false;
	ofSetBackgroundAuto( bBackgroundAuto );
	
	ofSoundStreamSetup(2,0, sampleRate,1024, 4);

	// construct FloatingSine objects, passing in the shared sound mixer
	int num_floating = 32;
	for ( int i=0; i<num_floating; i++ ) {
		FloatingSine* f = new FloatingSine();
		floating.push_back( f );
	}
	for ( int i=0; i<floating.size(); i++ ) {
		floating[i]->setup( &mixer, &floating, &particle );
	}

	// setup the lowpass filter
	filter.setup();
	filter.setCutoff(1.0f);
	filter.setRes(0.5f);
	// the filter takes input from the mixer
	filter.addInputFrom( &mixer );
	// the passthrough takes input from the filter (for visualisation)
	passthrough.addInputFrom( &filter );
	
	// pass the completed graph to ofSoundStream interface
	ofSoundStreamAddSoundSource( &passthrough );
	
	// set the audio scale to pentatonic
	setScale( 0 );

}


//--------------------------------------------------------------
void testApp::update(){

	for ( int i=0; i<floating.size(); i++ ) {
		floating[i]->update();
		mixer.setPan( floating[i]->getSoundOutputNodePtr(), (floating[i]->getPosition().x)/ofGetWidth() );
	}

	// fetch audio data, for visualisation
	const ofSoundBuffer& output = passthrough.getBuffer();
	output.copyChannel( 0, lAudio );
	output.copyChannel( 1, rAudio );
	
}

//--------------------------------------------------------------
void testApp::draw(){

	// draw the left audio:
	ofFill();
	ofSetColor( 0xff, 0xff, 0xff, 0x80 );
	for (int i = 0; i < 512; i++){
		ofLine(i,200,i,200+lAudio[i]*100.0f);
	}

	// draw the right audio:
	ofSetColor( 0xff, 0xff, 0xff, 0x80 );
	for (int i = 0; i < 512; i++){
		ofLine(512+i,200,512+i,200+rAudio[i]*100.0f);
	}

	// draw all the particles
	ofSetHexColor( 0x112255 );
	for ( int i=0; i<floating.size(); i++ )	{
		floating[i]->draw();
	}
	
	// draw text box
	ofFill();
	ofEnableBlendMode( OF_BLENDMODE_ALPHA );
	ofSetColor( 0, 0, 0, 3 );
	ofRect( 20, ofGetHeight()-160, ofGetWidth()-40, 140 );
	ofEnableBlendMode( OF_BLENDMODE_ADD );
	ofSetHexColor(0x333333);
	char reportString[1024];
	sprintf(reportString, "click to scatter\n\n\nvolume: %6.3f (modify with -/+ keys)\n"
			"scale: %s (change with 's' key)\nbase midi note: %i (change with up/down keys, 'r' for random) \n\n"
			"press 'w' to swap the waveform of one unit, 'v' to set all to sawtooth, 'o' to set all to sine,\n 'b' to toggle background clear" ,
			volume, scale==0?"pentatonic":"lydian", baseNote );

	ofDrawBitmapString(reportString,30,ofGetHeight()-140);

	
}

//--------------------------------------------------------------
void testApp::setScale( int scale ) {
	for ( int i=0; i<(int)floating.size(); i++ ) {
		floating[i]->setScale( scale );
	}
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){

	if (key == '-'){
		volume -= 0.05;
		volume = MAX(volume, 0);
		mixer.setMasterVolume( volume );
	} else if (key == '+'){
		volume += 0.05;
		mixer.setMasterVolume( volume );
	} else if ( key == 's' ) {
		scale = !scale;
		setScale( scale );
	} else if ( key == 'w' ) {
		int which = ofRandom( 0, 0.9999f*floating.size() );
		floating[which]->toggleWaveform();
	} else if ( key == 'v' ) {
		for ( int i=0; i<floating.size(); i++ ) {
			floating[i]->setWaveform( true );
		}
	} else if ( key == 'o' ) {
		for ( int i=0; i<floating.size(); i++ ) {
			floating[i]->setWaveform( false );
		}
	} else if ( key == OF_KEY_UP ) {
		baseNote++;
		for ( int i=0; i<floating.size(); i++ ) {
			floating[i]->setBaseMidiNote( baseNote );
		}
	} else if ( key == OF_KEY_DOWN ) {
		baseNote--;
		for ( int i=0; i<floating.size(); i++ ) {
			floating[i]->setBaseMidiNote( baseNote );
		}
	} else if ( key == 'r' ) {
		baseNote = ofRandom( -30, 400 );
		for ( int i=0; i<floating.size(); i++ ) {
			floating[i]->setBaseMidiNote( baseNote );
		}
	} else if ( key == 'b' ) {
		bBackgroundAuto = !bBackgroundAuto;
		ofSetBackgroundAuto( bBackgroundAuto );
		for ( int i=0; i<floating.size(); i++ ) {
			floating[i]->setBackgroundAuto( bBackgroundAuto );
		}
	}
	 
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	filter.setCutoff((float)x/(float)ofGetWidth());
	filter.setRes((float)y/(float)ofGetHeight());
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	for ( int i=0; i<floating.size(); i++ )
	{
		ofVec2f delta = (floating[i]->position - ofVec2f( x,y ));
		float distance = delta.length();
		ofVec2f deltaNorm = delta/distance;
		floating[i]->velocity += (deltaNorm*300.0f);
	}
}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

