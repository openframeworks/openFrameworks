// Project Title: Slitscan Radial Clock
// Description: In this example we use the oF video grabber to sample camera input. Then the code accesses the pixels in each frame from the
// camera to create a slitscan video. The image is created in two styles, progressively across the screen or as a ribbon, pushing pixels
// sampled from the center of the screen. Rendered to the screen as a flat, horizontal image or using oftransform to display as a radial image.
// ©Daniel Buzzo 2020, 21, 22
// dan@buzzo.com
// http://buzzo.com
// https://github.com/danbz
// https://www.youtube.com/danbuzzo
//
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // set the width and height of the camera grabber to suit your particular camera //
    // common sizes are listed below //
    // camWidth =  640;  // try to grab at this size from the camera for Raspberry Pi
    // camHeight = 480;
    camWidth =  1280;  // try to grab at this size from an apple HDwebcam camera.
    camHeight = 720;
    // camWidth =  1280;  // try to grab at this size from a standard external 4x3 webcam camera.
    // camHeight = 1024;
    // camWidth= 1334; // stereo labs zed camera
    // camWidth= 1280;
    
    numOfSecs = 59;
    numOfMins = 59;
    numOfHours = 23;
    
    xSteps = 0; // use xSteps to keep count of where we are scanning across the screen
    scanStyle = 2; // start as clock style
    scanName = "radial";
    b_debug = false;
    b_radial = true;
    
    // load a custom truetype font as outline shapes to blend over video layer
    font.load("LiberationMono-Regular.ttf", 100, true, true, true);
    
    // ask the video grabber for a list of attached camera devices. & put it into a vector of devices
    // if you can't get your camera to work then check the debug output text to see what device ID your desired camera is
    vector<ofVideoDevice> devices = vidGrabber.listDevices();
    for(int i = 0; i < devices.size(); i++){ // loop through and print out the devices to the console log
        if(devices[i].bAvailable){
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }
    // set up our camera and video grabber object
    vidGrabber.setDeviceID(0); // set the ID of the camera you want to use
    vidGrabber.setDesiredFrameRate(30); // set how fast we will grab frames from the camera
    vidGrabber.setup(camWidth, camHeight); // set the width and height of the camera
    videoPixels.allocate(camWidth, camHeight, OF_PIXELS_RGB); // set up our pixel object to be the same size as our camera object
    videoTexture.allocate(videoPixels);
    
    ofSetBackgroundColor(0, 0, 0); // set the background colour to dark black
    ofDisableSmoothing();
    
    // set start time from system time
    seconds = ofGetSeconds();
    minutes = ofGetMinutes();
    hours = ofGetHours();
    calculateTime();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    ofColor color;
    // update the video grabber object
    vidGrabber.update();
    //check to see if there is a new frame from the camera to process, and if there is - process it.
    if (vidGrabber.isFrameNew()){
        pixels = vidGrabber.getPixels();
        pixels.mirror(false, true);
    }
    
    if (ofGetSeconds() > seconds){ // one second has elapsed
        seconds = ofGetSeconds();
        
        if (ofGetMinutes() >  minutes){
            minutes = ofGetMinutes();
        } else if (ofGetMinutes() == 0){
            minutes = 0;
        }
        
        if (ofGetHours() > hours){
            seconds = minutes = 0 ;
            hours = ofGetHours();
        } else if (ofGetHours() == 0){
            hours = 0;
        }
        
        if (numOfHours == 0){
            hours = seconds = minutes = 0;
        }
        
        calculateTime();
        
        xSteps = 0; // step on to the next line.
     } else if (ofGetSeconds() == 0){
         seconds = 0;
         calculateTime();
     }
    
    switch (scanStyle) {
        case 1: // scan horizontal 'push' ribbon from centre. grabbing only the vertical pixels at the centre of the camera image
            if (xSteps < camWidth/numOfSecs){
                for (int y=0; y<camHeight; y++ ) { // loop through all the pixels on a line to draw new line at 0 in target image
                    color = pixels.getColor( camWidth/2, y); // get the pixels on line
                    videoPixels.setColor(1, y, color);
                }
                
                for (int x = camWidth; x>=0; x-= 1){
                    for (int y=0; y<camHeight; y++ ) { // loop through all the pixels on a line
                        videoPixels.setColor(x, y, videoPixels.getColor( x-1, y )); // copy each pixel in the target to 1 pixel the right
                    }
                }
            }
            videoTexture.loadData(videoPixels);
            xSteps++;
            break;
            
        case 2: // slitscan clock. grabbing the pixels progressively from the camera image from left to right
            // this 'counts' in seconds by grabbing a chunk of the screenswdith / 60 each second to give a feeling of 'counting' across the screen
            if (xSteps < camWidth/numOfSecs){
                for (int y=0; y < camHeight; y++ ) { // loop through all the pixels on a line
                    color = pixels.getColor( xSteps + (camWidth/numOfSecs * seconds), y); // get the pixels on line 
                    videoPixels.setColor( xSteps + (camWidth/numOfSecs * seconds), y, color);
                }
            }
            videoTexture.loadData(videoPixels);
            xSteps++;
            break;
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (b_radial){ // draw radial ribbon in the middle of the screen
        for (int i =0; i<videoTexture.getWidth(); i++){
            ofPushMatrix(); // use progressive translate and rotate to draw the slitscan in a radial style
            ofTranslate(ofGetWidth()/2, ofGetHeight()/2); // centre in right portion of screen
            ofRotateZDeg( ofMap(i, 0, videoTexture.getWidth() , 0, 360) - 186); // rotate by the size of our image mapped to 360 degrees - 186 to start with zero seconds at the top.
            videoTexture.drawSubsection(0, 100,  3, videoTexture.getHeight()/2, i, 0);
            ofPopMatrix();
        }
    } else { // draw the video texture horizontally across the full screen
        videoTexture.draw( 0, 0, ofGetWidth(), ofGetHeight()); // draw the seconds slitscan video texture we have constructed
    }
    
    // display the time using our truetype font
    font.drawStringAsShapes( time, ofGetWidth() -650, ofGetHeight() -90);
    
    if (b_debug){ // draw camera debug and info to screen
        vidGrabber.draw(ofGetWidth()-camWidth/4 -10, ofGetHeight()-camHeight/4 -10, camWidth/4, camHeight/4); // draw our plain image
        ofDrawBitmapString(" scanning " + scanName + " , 1-ribbon horizontal, 2-slitscan clock, f-toggle fullscreen, r-draw radial, d-toggle debug, FPS:" + ofToString(ofGetFrameRate()) , 10, ofGetHeight() -10);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
            
        case '1':
            scanStyle = 1; // set the style to 'push' the scan across the screen
            scanName = "horizontal ribbon";
            break;
            
        case '2':
            scanStyle = 2; // set the style to 'count' the scan across the screen
            scanName = "let's be a slitscan clock";
            currTime = ofGetSystemTimeMillis();
            break;
            
        case 'd':
            b_debug =!b_debug; // toggle our debug info on screen
            break;
            
        case 'r':
            b_radial =!b_radial; // toggle to draw the scan in radial style
            break;
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::calculateTime(){
    // generate time text and draw clock time onscreen
    // used to pad a '0' infront of single digit numbers to make consistent line length.
    string clockHours, clockMins, clockSecs;
    if (hours < 10){
        clockHours = "0" +ofToString(hours);
    }  else {
        clockHours = ofToString(hours);
    }
    
    if (minutes < 10){
        clockMins = "0" +ofToString(minutes);
    }  else {
        clockMins = ofToString(minutes);
    }
    
    if (seconds < 10){
        clockSecs = "0" +ofToString(seconds);
    }  else {
        clockSecs = ofToString(seconds);
    }
    time = clockHours + ":" + clockMins + ":" + clockSecs;
}
