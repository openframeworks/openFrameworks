#pragma once

#include "ofPanel.h"

class ofSlider : public ofBaseGui{
	friend class ofPanel;
	
public:	
	ofSlider* setup(string sliderName, double _val, double _min, double _max, bool _bInt = false, float width = defaultWidth, float height = defaultHeight){
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
		return this;
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
		ofPushStyle();
		ofPushMatrix();
	
		currentFrame = ofGetFrameNum();
		ofFill();
		ofSetColor(backgroundColor);
		ofRect(b);
		
		ofTranslate(b.x, b.y);		
		float valAsPct = ofMap( val, min, max, 0, b.width-2, true );
		ofEnableAlphaBlending();
		ofSetColor(fillColor);		
		ofRect(1, 1, valAsPct, b.height-2);
		
		ofTranslate(0, b.height / 2 + 4);
		ofSetColor(textColor);
		ofDrawBitmapString(name, textPadding, 0);
		string valStr = ofToString(val, bInt ? 0 : 2);
		ofDrawBitmapString(valStr, b.width - textPadding - valStr.length() * 8, 0);
		
		ofPopMatrix();
		ofPopStyle();
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
