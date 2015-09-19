#include "ofApp.h"

void ofApp::setup()
{
	mainAppsCount = 0;
	threadedObject.start();
}

void ofApp::update()
{
	mainAppsCount++;
}

void ofApp::draw()
{
    ofBackground(ofColor::black);
	ofSetColor(ofColor::white);

    // We can call this from our main thread because we know that
    //  ofApp::draw() is being called by the main thread.
    threadedObject.draw();

    std::stringstream ss;
    ss << "I am a the main opengl thread." << std::endl;
    ss << "my current count is: " << mainAppsCount << std::endl;

    ofDrawBitmapString(ss.str(), 350, 56);

    ofSetHexColor(0xff0033);

    ss.str(""); // Clear the string stream.

    ss << "Press 's' to stop the thread." << std::endl;
    ss << "Press 'a' to start it." << std::endl;
    ss << "Press '!' to throw an exception." << std::endl;

    ofDrawBitmapString(ss.str(), 50, 140);

}

void ofApp::keyPressed(int key)
{
    if (key == 'a')
    {
        threadedObject.start();
    }
    else if (key == 's')
    {
        threadedObject.stop();
    }
    else if (key == '!')
    {
        threadedObject.throwTestException();
    }
}
