#pragma once

#include "ofPanel.h"

class ofSlider : public ofBaseGui{
	friend class ofPanel;
	
public:
	
	void setup(string sliderName, double _val, double _min, double _max, bool _bInt = false, float width = 200, float height = 20){
		name = sliderName;
		val = _val;
		min = _min;
		max = _max;
		b.x = 0;
		b.y = 0;
		b.width = width;
		b.height = height;
		currentFrame = 0;			
		bGuiActive = false;
		bInt = _bInt;
	}
	
	
	virtual void mouseMoved(ofMouseEventArgs & args){
	}
	
	virtual void mousePressed(ofMouseEventArgs & args){
		setValue(args.x, args.y, true);
	}
	
	virtual void mouseDragged(ofMouseEventArgs & args){
		setValue(args.x, args.y, false);			
	}
	
	virtual void mouseReleased(ofMouseEventArgs & args){
		bGuiActive = false;		
	}	
	
	double getValue(){
		if( bInt ){
			return (int)val;
		}	
		return val;
	}
	
	void draw(){	
		currentFrame = ofGetFrameNum();
		ofFill();
		ofSetColor(backgroundColor);
		ofRect(b);
		
		float valAsPct = ofMap( val, min, max, 0, b.width, true );
		ofEnableAlphaBlending();
		ofSetColor(fillColor);		
		ofRect(b.x+1, b.y+1, valAsPct-1, b.height-2);
		
		ofSetColor(textColor);			
		
		float stringY = b.y + 14;
		
		ofDrawBitmapString(name, b.x + 4, stringY);
		string valStr;
		if( bInt ){
			valStr = ofToString(val, 0);
		}else{
			valStr = ofToString(val, 2);
		}
		ofDrawBitmapString( valStr, (b.x + b.width) - 3 - valStr.length() * 8, stringY );
	}
	
protected:
	double val, min, max;
	bool bInt;
	
	void setValue(float mx, float my, bool bCheck){
		if( ofGetFrameNum() - currentFrame > 1 ){
			bGuiActive = false;
			return; 
		}
		if( bCheck ){
			if( b.inside(mx, my) ){
				bGuiActive = true;
			}else{
				bGuiActive = false;
			}
		}
		if( bGuiActive ){
			val = ofMap(mx, b.x, b.x + b.width, min, max, true);
		}
	}
};
