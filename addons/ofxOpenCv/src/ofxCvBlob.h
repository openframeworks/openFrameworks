/*
* ofxCvBlob.h
* openFrameworks
*
* A blob is a homogenous patch represented by a polygonal contour.
* Typically a blob tracker uses the contour to figure out the blob's
* persistence and "upgrades" it with ids and other temporal
* information.
*
*/

#pragma once

#include "ofxCvConstants.h"


class ofxCvBlob {

    public:

        float               area;
        float               length;
        ofRectangle         boundingRect;
        ofPoint             centroid;
        bool                hole;

        vector <ofPoint>    pts;    // the contour of the blob
        int                 nPts;   // number of pts;

        //----------------------------------------
        ofxCvBlob() {
            area 		= 0.0f;
            length 		= 0.0f;
            hole 		= false;
            nPts        = 0;
        }

        //----------------------------------------
        void draw(float x = 0, float y = 0){
            ofNoFill();
            ofSetHexColor(0x00FFFF);
            ofBeginShape();
            for (int i = 0; i < nPts; i++){
                   ofVertex(x + pts[i].x, y + pts[i].y);
            }
            ofEndShape(true);
            ofSetHexColor(0xff0099);
            ofRect(x + boundingRect.x, y + boundingRect.y, boundingRect.width, boundingRect.height);
        }
};
