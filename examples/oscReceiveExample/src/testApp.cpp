#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	// listen on the given port
	std::cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup( PORT );

	current_msg_string = 0;
	mouseX = 0;
	mouseY = 0;
	strcpy( mouseButtonState, "" );

	ofBackground( 30, 30, 130 );

}

//--------------------------------------------------------------
void testApp::update(){

	// hide old messages
	for ( int i=0; i<NUM_MSG_STRINGS; i++ )
	{
		if ( timers[i] < ofGetElapsedTimef() )
			msg_strings[i] = "";
	}

	// check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage( &m );

		// check for mouse moved message
		if ( strcmp( m.getAddress(), "/mouse/position" ) == 0 )
		{
			// both the arguments are int32's
			mouseX = m.getArgAsInt32( 0 );
			mouseY = m.getArgAsInt32( 1 );
		}
		// check for mouse button message
		else if ( strcmp( m.getAddress(), "/mouse/button" ) == 0 )
		{
			// the single argument is a string
			strcpy( mouseButtonState, m.getArgAsString( 0 ) );
		}
		else
		{
			// unrecognized message: display on the bottom of the screen
			char msg_string[16384];
			strcpy( msg_string, m.getAddress() );
			strcat( msg_string, ": " );
			for ( int i=0; i<m.getNumArgs(); i++ )
			{
				// get the argument type
				strcat( msg_string, m.getArgTypeName( i ) );
				strcat( msg_string, ":" );
				// display the argument - make sure we get the right type
				if( m.getArgType( i ) == OFXOSC_TYPE_INT32 )
					sprintf( msg_string, "%s%d ", msg_string, m.getArgAsInt32( i ) );
				else if( m.getArgType( i ) == OFXOSC_TYPE_FLOAT )
					sprintf( msg_string, "%s%f ", msg_string, m.getArgAsFloat( i ) );
				else if( m.getArgType( i ) == OFXOSC_TYPE_STRING )
					sprintf( msg_string, "%s\"%s\" ", msg_string, m.getArgAsString( i ) );
				else
					strcat( msg_string, "unknown" );
			}
			// add to the list of strings to display
			msg_strings[current_msg_string] = msg_string;
			timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
			current_msg_string = ( current_msg_string + 1 ) % NUM_MSG_STRINGS;
			// clear the next line
			msg_strings[current_msg_string] = "";
		}

	}
}


//--------------------------------------------------------------
void testApp::draw(){

	char buf[256];
	sprintf( buf, "listening for osc messages on port %d", PORT );
	ofDrawBitmapString( buf, 10, 20 );

	// draw mouse state
	sprintf( buf, "mouse: % 4d % 4d", mouseX, mouseY );
	ofDrawBitmapString( buf, 430, 20 );
	ofDrawBitmapString( mouseButtonState, 580, 20 );

	for ( int i=0; i<NUM_MSG_STRINGS; i++ )
	{
		ofDrawBitmapString( (char*)msg_strings[i].c_str(), 10, 40+15*i );
	}



}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){

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
void testApp::resized(int w, int h){

}

