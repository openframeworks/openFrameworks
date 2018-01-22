//
//  ofxColorPicker.h
//  ofxColorPicker
//
//  Based on ofxColorPicker by Lukasz Karluk on 13/08/2015.
//
//

#pragma once

#include "ofColor.h"
#include "ofEvents.h"
#include "ofxBaseGui.h"
#include "ofxSlider.h"



template<typename ColorType>
class ofxColorPicker_: public ofxBaseGui {

public :
	
	ofxColorPicker_();
	ofxColorPicker_(ofParameter<ofColor_<ColorType>> parameter, float w = defaultWidth, float h = defaultWidth / 9. * 16.);
	ofxColorPicker_ * setup(ofParameter<ofColor_<ColorType>> parameter, float w = defaultWidth, float h = defaultWidth / 9. * 16.);

	void setShape(float x, float y, float w, float h);
	void setShape(ofRectangle r);
    
    void setColorScale(float value);
    float getColorScale();
    
    void setColorRadius(float value);
    float getColorRadius();
    
    void setColorAngle(float value);
	float getColorAngle();

	ofAbstractParameter & getParameter();
	
private :
	void render();
	bool setValue(float mx, float my, bool bCheckBounds);
	void generateDraw();

	ofMesh getBackground();
	ofMesh getColorPoint();
	ofMesh getColorWheel();
	ofMesh getColorScaleBar();

	bool mouseMoved(ofMouseEventArgs & args);
	bool mousePressed(ofMouseEventArgs & args);
	bool mouseDragged(ofMouseEventArgs & args);
	bool mouseReleased(ofMouseEventArgs & args);
	bool mouseScrolled(ofMouseEventArgs & args);

	bool mouseUpdate(ofMouseEventArgs& args);
	
	glm::vec2 colorPoint;
    float colorScale;
    float colorRadius;
	float colorAngle;
	bool bSettingColor = false;

    int colorWheelRadius;
    int colorWheelPad;

	ofRectangle rectBackground;
	ofRectangle rectColorWheel;
	ofRectangle rectColorScaleBar;

	ofVboMesh geometry;
	ofParameter<ofColor_<ColorType>> color;

	enum State{
		Waiting,
		ChangingWheel,
		ChangingScale,
	}state = Waiting;

	ofEventListener listener;
};
