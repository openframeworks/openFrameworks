#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // listen on the given port
    cout << "listening for osc messages on port " << PORT << "\n";
    receiver.setup(PORT);

    current_msg_string = 0;
    mouseX = 0;
    mouseY = 0;
    mouseButtonState = "";

    ofSetWindowTitle("osc receiver");
}

//--------------------------------------------------------------
void ofApp::update(){

    // hide old messages
    for(int i = 0; i < NUM_MSG_STRINGS; i++){
        if(timers[i] < ofGetElapsedTimef()){
            msg_strings[i] = "";
        }
    }

    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);

        // check for mouse moved message
        if(m.getAddress() == "/mouse/position"){
            // both the arguments are float's
            mouseXf = m.getArgAsFloat(0);
            mouseYf = m.getArgAsFloat(1);
        }
        // check for mouse button message
        else if(m.getAddress() == "/mouse/button"){
            // first argument is int32, second is a string
            mouseButtonInt = m.getArgAsInt32(0);
            mouseButtonState = m.getArgAsString(1);
        }
        // check for an image being sent (note: the size of the image depends greatly on your network buffer sizes - if an image is too big the message won't come through )
        else if(m.getAddress() == "/image" ){
            ofBuffer buffer = m.getArgAsBlob(0);
            receivedImage.load(buffer);
        }
        else{
            // unrecognized message: display on the bottom of the screen
            string msg_string;
            msg_string = m.getAddress();
            msg_string += ":";
            for(int i = 0; i < m.getNumArgs(); i++){
                // get the argument type
                msg_string += " ";
                msg_string += m.getArgTypeName(i);
                msg_string += ":";
                // display the argument - make sure we get the right type
                if(m.getArgType(i) == OFXOSC_TYPE_INT32){
                    msg_string += ofToString(m.getArgAsInt32(i));
                }
                else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
                    msg_string += ofToString(m.getArgAsFloat(i));
                }
                else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
                    msg_string += m.getArgAsString(i);
                }
                else{
                    msg_string += "unknown";
                }
            }
            // add to the list of strings to display
            msg_strings[current_msg_string] = msg_string;
            timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
            current_msg_string = (current_msg_string + 1) % NUM_MSG_STRINGS;
            // clear the next line
            msg_strings[current_msg_string] = "";
        }

    }
}


//--------------------------------------------------------------
void ofApp::draw(){

    ofBackgroundGradient(100, 0);

    //if image exists, draw it
    if(receivedImage.getWidth() > 0){
        ofSetColor(255);
        receivedImage.draw(ofGetWidth()/2-receivedImage.getWidth()/2,
         ofGetHeight()/2-receivedImage.getHeight()/2);
    }

    //draw recent unrecognized messages
    for(int i = 0; i < NUM_MSG_STRINGS; i++){
        ofDrawBitmapStringHighlight(msg_strings[i], 10, 40 + 15 * i);
    }

    string buf = "listening for osc messages on port : " + ofToString(PORT);
    ofDrawBitmapStringHighlight(buf, 10, 20);





    // draw mouse state
    ofPoint mouseIn(mouseXf*ofGetWidth(),mouseYf*ofGetHeight());
    if (mouseButtonInt == 0) {
        ofSetColor(255);
    }else ofSetColor(ofColor::salmon);
    ofDrawCircle(mouseIn, 20);
    ofDrawBitmapStringHighlight(mouseButtonState, mouseIn);


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
