// in ofApp.cpp
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    cout << "OpenCV version: " << cv::getVersionMajor() << "." << cv::getVersionMinor() << "."<< cv::getVersionRevision() << "\n";

    // 1. Setup Live Video
    camWidth = 640;
    camHeight = 480;
    grabber.setup(camWidth, camHeight);
    
    // 2. Load the YuNet Model via the FaceDetectorYN wrapper

    string modelPath;

    if (cv::getVersionMajor()>=4) {
        if (cv::getVersionMinor()<=6) {
            // opencv 4.6.0
            modelPath = ofToDataPath("face_detection_yunet_2022mar.onnx"); 
        } else {
            // opencv 4.9.0 and greater
            modelPath = ofToDataPath("face_detection_yunet_2023mar.onnx");

        }
    }

    
    
    //string modelPath = ofToDataPath("yunet.onnx");
    cv::Size inputSize = cv::Size(320, 320); // The size the network prefers
    float scoreThreshold = 0.6; // Confidence threshold
    float nmsThreshold = 0.3;   // Non-Maximum Suppression threshold
    int topK = 5000;            // Keep top 5000 detections before NMS

    // Call the static 'create' method to initialize the detector
    detector = cv::FaceDetectorYN::create(
        modelPath,
        "", // config file (not needed for this model)
        inputSize,
        scoreThreshold,
        nmsThreshold,
        topK
    );
    
    if (detector.empty()) {
        ofLogError() << "Failed to load YuNet model. Make sure 'face_detection_yunet_2023mar.onnx' is in bin/data/";
    } else {
        ofLogNotice() << "cv::FaceDetectorYN loaded successfully.";
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    grabber.update();
    
    if (grabber.isFrameNew()) {
        // Clear previous detections
        detectedFaces.clear();

        //
        // 1. Load pixels from the grabber into our ofxCvColorImage
        colorImg.setFromPixels(grabber.getPixels());
        
        // 2. Get the cv::Mat from the wrapper image
        cv::Mat frame = colorImg.getCvMat();
        
        // 3. Resize the detector's input.
        detector->setInputSize(cv::Size(frame.cols, frame.rows));

        // 4. Detect!
        cv::Mat faces;
        detector->detect(frame, faces);
        if (faces.rows > 0) {
            for (int i = 0; i < faces.rows; i++) {
                // The format is:
                // [0-3] = bbox (x, y, w, h)
                // [4-13] = 10 facial landmarks
                // [14] = score
                
                int x = static_cast<int>(faces.at<float>(i, 0));
                int y = static_cast<int>(faces.at<float>(i, 1));
                int w = static_cast<int>(faces.at<float>(i, 2));
                int h = static_cast<int>(faces.at<float>(i, 3));
                
                detectedFaces.push_back(ofRectangle(x, y, w, h));
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    grabber.draw(0, 0); // Draw the live video

    // Draw the detected faces
    ofNoFill();
    ofSetLineWidth(2);
    ofSetColor(ofColor::red);

    for (auto& rect : detectedFaces) {
        ofDrawRectangle(rect);
    }
    
    // Draw info
    ofSetColor(ofColor::white);
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate(), 0), 20, 30);
    ofDrawBitmapString("Detections: " + ofToString(detectedFaces.size()), 20, 50);
}