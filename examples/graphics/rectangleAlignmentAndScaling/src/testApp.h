#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
  
    ofRectScaleMode rectScaleMode;
    
    ofAspectRatioMode aspectRatioMode;
    
    ofAlignHorz target_hAlign;
    ofAlignVert target_vAlign;
    
    ofAlignHorz subject_hAlign;
    ofAlignVert subject_vAlign;

    ofRectangle targetRect;
    ofRectangle subjectRect;
    ofRectangle workingSubjectRect;
    
    ofColor targetColor;
    ofColor subjectColor;

    bool bUseImage;
    
    bool isRectScaling;
    bool isScaling;
    bool isAligning;
    
    ofImage img;
    
    /////////////////////////
    // Helper functions below
    
    void makeNewSubject();
    void makeNewTarget();
    
    void drawAlignRect(const ofRectangle& rect,
                       const ofColor& color,
                       ofAlignHorz hAlign,
                       ofAlignVert vAlign,
                       bool useImage);
    
    void drawHorzAlignMark(const ofRectangle& rect, const ofColor& color, ofAlignHorz hAlign);
    void drawVertAlignMark(const ofRectangle& rect, const ofColor& color, ofAlignVert vAlign);
    
    ofRectScaleMode getNextRectScaleMode(ofRectScaleMode mode);
    ofAspectRatioMode getNextAspectRatioMode(ofAspectRatioMode mode);
    ofAlignHorz getNextHorzAlign(ofAlignHorz hAlign);
    ofAlignVert getNextVertAlign(ofAlignVert vAlign);
    
    string getHorzAlignString(ofAlignHorz hAlign);
    string getVertAlignString(ofAlignVert vAlign);
    
    string getAspectRatioModeString(ofAspectRatioMode mode);
    string getRectScaleModeString(ofRectScaleMode mode);
    
};
