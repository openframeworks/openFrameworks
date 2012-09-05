#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    ofSetFrameRate(30);
    ofEnableAlphaBlending();

    isScaling = false;
    isAligning = true;
    isRectScaling = false;
    
    rectScaleMode    = OF_RECTSCALEMODE_FIT;
    aspectRatioMode  = OF_ASPECT_RATIO_KEEP;
    
    target_hAlign = OF_ALIGN_HORZ_CENTER;
    target_vAlign = OF_ALIGN_VERT_CENTER;
    
    subject_hAlign = OF_ALIGN_HORZ_CENTER;
    subject_vAlign = OF_ALIGN_VERT_CENTER;
    
    bUseImage = true;
    
    img.loadImage("resolution_test_1080_mini.png");

    targetColor  = ofColor(255,0,0);
    subjectColor = ofColor(255,255,0);

    makeNewTarget();
    makeNewSubject();


}

//--------------------------------------------------------------
void testApp::update(){
    
    workingSubjectRect = subjectRect;
        
    if(!isRectScaling) {
        if(isScaling) {
            if(isAligning) {
                workingSubjectRect.scaleTo(targetRect,
                                           aspectRatioMode,
                                           target_hAlign,
                                           target_vAlign,
                                           subject_hAlign,
                                           subject_vAlign);
            } else {
                workingSubjectRect.scaleTo(targetRect,
                                           aspectRatioMode);
            }
        } else {
            if(isAligning) {
                workingSubjectRect.alignTo(targetRect,
                                           target_hAlign,
                                           target_vAlign,
                                           subject_hAlign,
                                           subject_vAlign);
            } else {
                workingSubjectRect.alignTo(targetRect);
            }
        }
    } else {
        workingSubjectRect.scaleTo(targetRect,
                                   rectScaleMode);
    }
    
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);

    // draw original subject in corner
    if(!bUseImage) {
        ofFill(); ofSetColor(subjectColor, 80);
        ofRect(10, ofGetHeight() - subjectRect.height - 10, subjectRect.width, subjectRect.height);
    } else {
        ofFill(); ofSetColor(255);
        img.draw(10, ofGetHeight() - subjectRect.height - 10, subjectRect.width, subjectRect.height);
    }
    // draw original subject frame in corner
    ofNoFill(); ofSetColor(subjectColor, 120);
    ofRect(10, ofGetHeight() - subjectRect.height - 10, subjectRect.width, subjectRect.height);
    // draw original subject label
    ofSetColor(255);
    ofDrawBitmapStringHighlight("SUBJECT", 16, ofGetHeight() - 20);
    
    // draw original target in corner
    ofFill(); ofSetColor(targetColor, 80);
    ofRect(ofGetWidth() - targetRect.width - 10, ofGetHeight() - targetRect.height - 10, targetRect.width, targetRect.height);
    // draw original target frame in corner
    ofNoFill(); ofSetColor(targetColor, 120);
    ofRect(ofGetWidth() - targetRect.width - 10, ofGetHeight() - targetRect.height - 10, targetRect.width, targetRect.height);
    // draw original target label
    ofSetColor(255);
    ofDrawBitmapStringHighlight("TARGET", ofGetWidth() - 65, ofGetHeight() - 20);
    
    
    // draw target rectangle in center
    drawAlignRect(targetRect,  targetColor,  target_hAlign,  target_vAlign, false);
    
    // draw aligned / scaled subject with respect to the target
    drawAlignRect(workingSubjectRect, subjectColor, subject_hAlign, subject_vAlign, bUseImage);

    // make the menu
    stringstream ss;
    
    int tab = 20;
    ss << "Keys:" << endl;
    ss << "----------------------------------------------------------" << endl;
    ss << " New Subject / Target (space) " << endl;
    ss << " Use An Image Subject (i) = " << (bUseImage ? "YES" : "NO") << endl;
    ss << "----------------------------------------------------------" << endl;
    ss << "  Enable Custom Align (A) = " << (isAligning && !isRectScaling ? "YES" : "NO") << endl;
    ss << "  Subject ofAlignHorz (h) = " << (isAligning && !isRectScaling ? getHorzAlignString(subject_hAlign) : "-") << endl;
    ss << "    Model ofAlignHorz (H) = " << (isAligning && !isRectScaling ? getHorzAlignString(target_hAlign)  : "-") << endl;
    ss << "  Subject ofAlignVert (v) = " << (isAligning && !isRectScaling ? getVertAlignString(subject_vAlign) : "-") << endl;
    ss << "    Model ofAlignVert (V) = " << (isAligning && !isRectScaling ? getVertAlignString(target_vAlign)  : "-") << endl;
    ss << "----------------------------------------------------------" << endl;
    ss << "Enable Custom Scaling (S) = " << (isScaling && !isRectScaling ? "YES" : "NO") << endl;
    ss << "    ofAspectRatioMode (a) = " << (isScaling && !isRectScaling ? getAspectRatioModeString(aspectRatioMode) : "-") << endl;
    ss << "----------------------------------------------------------" << endl;
    ss << " Override Scale/Align (r) = " << (isRectScaling ? "YES" : "NO") << endl;
    ss << "      ofRectScaleMode (s) = " << (isRectScaling ? getRectScaleModeString(rectScaleMode) : "-") << endl;

    // draw the menu
    ofSetColor(255);
    ofDrawBitmapString(ss.str(), 10, 14);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    // define keyboard interactions
    if(key == ' ') {
        makeNewTarget();
        makeNewSubject();
    } else if(key == 'h' && !isRectScaling && isAligning) {
        subject_hAlign = getNextHorzAlign(subject_hAlign);
    } else if(key == 'H' && !isRectScaling && isAligning) {
        target_hAlign = getNextHorzAlign(target_hAlign);
    } else if(key == 'v' && !isRectScaling && isAligning) {
        subject_vAlign = getNextVertAlign(subject_vAlign);
    } else if(key == 'V' && !isRectScaling && isAligning) {
        target_vAlign = getNextVertAlign(target_vAlign);
    } else if(key == 'a' && !isRectScaling && isScaling) {
        aspectRatioMode = getNextAspectRatioMode(aspectRatioMode);
    } else if(key == 's' && isRectScaling) {
        rectScaleMode = getNextRectScaleMode(rectScaleMode);
    } else if(key == 'S') {
        if(isRectScaling) {
            isScaling = true;
            isRectScaling = false;
        } else {
            isScaling = !isScaling;
        }
    } else if(key == 'r') {
        isRectScaling = !isRectScaling;
        if(isRectScaling) {
            isScaling = false;
            isAligning = false;
        }
    } else if(key == 'A') {
        if(isRectScaling) {
            isRectScaling = false;
            isAligning = true;
        } else {
            isAligning = !isAligning;
        }
    } else if(key == 'i') {
        bUseImage = !bUseImage;
        if(bUseImage) {
            makeNewSubject();
        }
    }
}

//--------------------------------------------------------------
void testApp::makeNewSubject() {
    // if we are not using an image, make a random subject rectangle
    if(!bUseImage) {
        subjectRect.setFromCenter(ofGetWidth()  / 2.0f,
                                  ofGetHeight() / 2.0f,
                                  ofRandom(30.0f,300.0f),
                                  ofRandom(30.0f,300.0f));
    } else {
        // if we are using the image, then match the image size
        subjectRect.setFromCenter(ofGetWidth()  / 2.0f,
                                  ofGetHeight() / 2.0f,
                                  img.getWidth(),
                                  img.getHeight());
    }
    
    // copy the subject to the working subject so it can be modified.
    workingSubjectRect = subjectRect;
    
}

//--------------------------------------------------------------
void testApp::makeNewTarget() {
    
    // create a random target rectangle aligned to the center of the screen
    targetRect.setFromCenter(ofGetWidth()  / 2.0f,
                             ofGetHeight() / 2.0f,
                             ofRandom(30.0f,300.0f),
                             ofRandom(30.0f,300.0f));
}

//--------------------------------------------------------------
void testApp::drawAlignRect(const ofRectangle& rect,
                            const ofColor& color,
                            ofAlignHorz hAlign,
                            ofAlignVert vAlign,
                            bool drawImage) {
    
    // draw the rect -- draw the image if using an image
    ofFill();
    if(drawImage) {
        ofSetColor(255,127);
        img.draw(rect);
    } else {
        ofSetColor(color, 80);
        ofRect(rect);
    }
    
    ofNoFill();
    ofSetColor(color, 120);
    ofRect(rect);
    
    // draw the alignment marks if applicable
    if(isAligning && !isRectScaling) {
        drawHorzAlignMark(rect, color, hAlign);
        drawVertAlignMark(rect, color, vAlign);
    }
}

//--------------------------------------------------------------
void testApp::drawHorzAlignMark(const ofRectangle& rect, const ofColor& color, ofAlignHorz hAlign) {
    if(hAlign != OF_ALIGN_HORZ_IGNORE) {
        float hAnchor = rect.getHorzAnchor(hAlign);
        ofSetColor(color,120);
        ofLine(hAnchor, rect.getTop()    - 13, hAnchor, rect.getTop()    - 3);
        ofLine(hAnchor, rect.getBottom() + 13, hAnchor, rect.getBottom() + 3);
    }
}

//--------------------------------------------------------------
void testApp::drawVertAlignMark(const ofRectangle& rect, const ofColor& color, ofAlignVert vAlign) {
    if(vAlign != OF_ALIGN_VERT_IGNORE) {
        float vAnchor = rect.getVertAnchor(vAlign);
        ofSetColor(color,120);
        ofLine(rect.getLeft()  - 13, vAnchor, rect.getLeft()  - 3, vAnchor);
        ofLine(rect.getRight() + 13, vAnchor, rect.getRight() + 3, vAnchor);
    }
}

//--------------------------------------------------------------
ofRectScaleMode testApp::getNextRectScaleMode(ofRectScaleMode mode) {
    if(mode == OF_RECTSCALEMODE_FIT) {
        mode = OF_RECTSCALEMODE_FILL;
    } else if(mode == OF_RECTSCALEMODE_FILL) {
        mode = OF_RECTSCALEMODE_CENTER;
    } else if(mode == OF_RECTSCALEMODE_CENTER) {
        mode = OF_RECTSCALEMODE_STRETCH_TO_FILL;
    } else if(mode == OF_RECTSCALEMODE_STRETCH_TO_FILL) {
        mode = OF_RECTSCALEMODE_FIT;
    }
    return mode;
}

//--------------------------------------------------------------
ofAspectRatioMode testApp::getNextAspectRatioMode(ofAspectRatioMode mode) {
    if(mode == OF_ASPECT_RATIO_IGNORE) {
        mode = OF_ASPECT_RATIO_KEEP;
    } else if(mode == OF_ASPECT_RATIO_KEEP) {
        mode = OF_ASPECT_RATIO_KEEP_BY_EXPANDING;
    } else if(mode == OF_ASPECT_RATIO_KEEP_BY_EXPANDING) {
        mode = OF_ASPECT_RATIO_IGNORE;
    }
    return mode;
}


//--------------------------------------------------------------
ofAlignHorz testApp::getNextHorzAlign(ofAlignHorz hAlign) {
    if(hAlign == OF_ALIGN_HORZ_LEFT) {
        hAlign = OF_ALIGN_HORZ_CENTER;
    } else if(hAlign == OF_ALIGN_HORZ_CENTER) {
        hAlign = OF_ALIGN_HORZ_RIGHT;
    } else if(hAlign == OF_ALIGN_HORZ_RIGHT) {
        hAlign = OF_ALIGN_HORZ_LEFT;
    }
    return hAlign;
}

//--------------------------------------------------------------
ofAlignVert testApp::getNextVertAlign(ofAlignVert vAlign) {
    if(vAlign == OF_ALIGN_VERT_TOP) {
        vAlign = OF_ALIGN_VERT_CENTER;
    } else if(vAlign == OF_ALIGN_VERT_CENTER) {
        vAlign = OF_ALIGN_VERT_BOTTOM;
    } else if(vAlign == OF_ALIGN_VERT_BOTTOM) {
        vAlign = OF_ALIGN_VERT_TOP;
    }
    return vAlign;
}

//--------------------------------------------------------------
string testApp::getHorzAlignString(ofAlignHorz hAlign) {
    switch (hAlign) {
        case OF_ALIGN_HORZ_LEFT:
            return "OF_ALIGN_HORZ_LEFT";
        case OF_ALIGN_HORZ_CENTER:
            return "OF_ALIGN_HORZ_CENTER";
        case OF_ALIGN_HORZ_RIGHT:
            return "OF_ALIGN_HORZ_RIGHT";
        case OF_ALIGN_HORZ_IGNORE:
            return "OF_ALIGN_HORZ_IGNORE";
    }
}

//--------------------------------------------------------------
string testApp::getVertAlignString(ofAlignVert vAlign) {
    switch (vAlign) {
        case OF_ALIGN_VERT_TOP:
            return "OF_ALIGN_VERT_TOP";
        case OF_ALIGN_VERT_CENTER:
            return "OF_ALIGN_VERT_CENTER";
        case OF_ALIGN_VERT_BOTTOM:
            return "OF_ALIGN_VERT_BOTTOM";
        case OF_ALIGN_VERT_IGNORE:
            return "OF_ALIGN_VERT_IGNORE";
    }
}

//--------------------------------------------------------------
string testApp::getAspectRatioModeString(ofAspectRatioMode mode) {
    switch (mode) {
        case OF_ASPECT_RATIO_IGNORE:
            return "OF_ASPECT_RATIO_IGNORE";
        case OF_ASPECT_RATIO_KEEP:
            return "OF_ASPECT_RATIO_KEEP";
        case OF_ASPECT_RATIO_KEEP_BY_EXPANDING:
            return "OF_ASPECT_RATIO_KEEP_BY_EXPANDING";
    }
}

//--------------------------------------------------------------
string testApp::getRectScaleModeString(ofRectScaleMode mode) {
    switch (mode) {
        case OF_RECTSCALEMODE_FIT:
            return "OF_RECTSCALEMODE_FIT";
        case OF_RECTSCALEMODE_FILL:
            return "OF_RECTSCALEMODE_FILL";
        case OF_RECTSCALEMODE_CENTER:
            return "OF_RECTSCALEMODE_CENTER";
        case OF_RECTSCALEMODE_STRETCH_TO_FILL:
            return "OF_RECTSCALEMODE_STRETCH_TO_FILL";
    }
}

