#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
	
		ofScaleMode scaleMode;
		
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
		
		// generate a random subject rect
		void makeNewSubject();

		// generate a random target rect
		void makeNewTarget();
		
		// draw a rectangle with alignment marks, etc
		void drawAlignRect(const ofRectangle& rect,
											 const ofColor& color,
											 ofAlignHorz hAlign,
											 ofAlignVert vAlign,
											 bool useImage);
		
		// draw visual horizontal anchor marks
		void drawHorzAlignMark(const ofRectangle& rect, const ofColor& color, ofAlignHorz hAlign);
		void drawVertAlignMark(const ofRectangle& rect, const ofColor& color, ofAlignVert vAlign);
		
		
		// helper functions for text output
		ofScaleMode getNextScaleMode(ofScaleMode mode);
		ofAspectRatioMode getNextAspectRatioMode(ofAspectRatioMode mode);
		ofAlignHorz getNextHorzAlign(ofAlignHorz hAlign);
		ofAlignVert getNextVertAlign(ofAlignVert vAlign);
		
		string getHorzAlignString(ofAlignHorz hAlign);
		string getVertAlignString(ofAlignVert vAlign);
		
		string getAspectRatioModeString(ofAspectRatioMode mode);
		string getScaleModeString(ofScaleMode mode);
		
};
