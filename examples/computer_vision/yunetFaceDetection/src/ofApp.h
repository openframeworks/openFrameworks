// in ofApp.h
#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofVideoGrabber.h"

// include the dnn and objdetect headers
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    ofVideoGrabber grabber;
    int camWidth;
    int camHeight;
    

    //
    // We use a Ptr (a smart pointer) to hold the detector
    cv::Ptr<cv::FaceDetectorYN> detector;
    //
    // -----------------------------
    //
    
    // Vector to store the final, processed face rectangles
    std::vector<ofRectangle> detectedFaces;

	ofxCvColorImage colorImg;
};