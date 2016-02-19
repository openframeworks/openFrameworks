#pragma once

#include "ofMain.h"
#include "ofxCv.h"

// single-person smile strength detector
// not robust against rotation
class SmileDetector {
protected:
    ofRectangle roi;
    
public:
    ofxCv::ObjectFinder faceFinder, smileFinder;
    
    void setup() {
        faceFinder.setup("haarcascade_frontalface_default.xml");
        faceFinder.setPreset(ofxCv::ObjectFinder::Accurate);
        faceFinder.setFindBiggestObject(true);
        smileFinder.setup("smiled_05.xml");
        smileFinder.setPreset(ofxCv::ObjectFinder::Sensitive);
        smileFinder.setMinNeighbors(0);
    }
    template <class T>
    void update(T& img) {
        update(ofxCv::toCv(img));
    }
    void update(const cv::Mat& mat) {
        faceFinder.update(mat);
        if(faceFinder.size()) {
            roi = faceFinder.getObject(0);
            float lowerRatio = .35;
            roi.y += roi.height * (1 - lowerRatio);
            roi.height *= lowerRatio;
            cv::Mat faceMat(mat, ofxCv::toCv(roi));
            smileFinder.update(faceMat);
        }
    }
    void draw() const {
        faceFinder.draw();
        if(faceFinder.size()) {
            ofPushMatrix();
            ofTranslate(roi.position);
            smileFinder.draw();
            ofPopMatrix();
        }
    }
    bool getFaceFound() const {
        return faceFinder.size();
    }
    ofRectangle getFace() const {
        return faceFinder.getObject(0);
    }
    int getSmileAmount() const {
        if(faceFinder.size()) {
            return smileFinder.size();
        } else {
            return 0;
        }
    }
};

class LineGraph {
protected:
    ofMesh mesh;
    ofVec2f min, max;
    int n;
    
public:
    LineGraph()
    :n(0) {
    }
    void reset() {
        n = 0;
        min = ofVec2f();
        max = ofVec2f();
    }
    void add(float y) {
        ofVec2f cur(n, y);
        mesh.addVertex(cur);
        if(n == 0) {
            min = cur;
            max = cur;
        } else {
            min.set(MIN(min.x, cur.x), MIN(min.y, cur.y));
            max.set(MAX(max.x, cur.x), MAX(max.y, cur.y));
        }
        n++;
    }
    float getNormalized(float y) {
        return ofNormalize(y, min.y, max.y);
    }
    void draw(float w, float h) {
        if(n > 2) {
            ofPushMatrix();
            ofTranslate(0, h);
            ofScale(w / (max.x - min.x), -h / (max.y - min.y));
            ofTranslate(-min);
            mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
            mesh.draw();
            ofPopMatrix();
        }
    }
};

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    
    ofVideoGrabber cam;
    SmileDetector smile;
    LineGraph graph;
};
