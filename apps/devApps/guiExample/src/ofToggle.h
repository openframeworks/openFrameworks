#pragma once

#include "ofBaseGui.h"

class ofToggle : public ofBaseGui{
	friend class ofGuiCollection;
	
public:
	
	void setup(string toggleName, bool _bVal, float width = 200, float height = 20){
		name		= toggleName;
		b.x			= 0;
		b.y			= 0;
		b.width		= width;
		b.height	= height;
		currentFrame = 0;			
		bGuiActive	= false;
		bVal		= _bVal;
		checkboxRect.width = b.height - 3;
		checkboxRect.height = b.height - 3;
	}
	
	
	virtual void mouseMoved(ofMouseEventArgs & args){
	}
	
	virtual void mousePressed(ofMouseEventArgs & args){
		setValue(args.x, args.y, true);
	}
	
	virtual void mouseDragged(ofMouseEventArgs & args){
	}
	
	virtual void mouseReleased(ofMouseEventArgs & args){
		bGuiActive = false;		
	}	
	
	bool getValue(){
		return bVal;
	}
	
	void draw(){
		ofPushStyle();
		
		currentFrame = ofGetFrameNum();
		ofSetColor(30, 30, 80);
		ofRect(b);
		
		checkboxRect.x = b.x + 2;
		checkboxRect.y = b.y + 1;
		
		ofNoFill();
		ofSetColor(180, 180, 180);			
		ofRect(checkboxRect);
		
		if( bVal ){
			ofFill();
			ofSetColor(180, 180, 180);			
			ofRect(checkboxRect);
			
			ofSetColor(30, 30, 80);
			ofLine(checkboxRect.x, checkboxRect.y, checkboxRect.x + checkboxRect.width, checkboxRect.y + checkboxRect.height);
			ofLine(checkboxRect.x, checkboxRect.y+ checkboxRect.height, checkboxRect.x + checkboxRect.width, checkboxRect.y);
		}
		
		ofSetColor(230, 230, 230, 255);			
		
		float stringY = b.y + 14;
		
		ofDrawBitmapString(name, b.x + 4 + checkboxRect.width, stringY);
		string valStr = bVal ? "true" : "false";
		
		ofDrawBitmapString( valStr, (b.x + b.width) - 3 - valStr.length() * 8, stringY );
		
		ofPopStyle();
	}
	
protected:
	bool bVal;
	ofRectangle checkboxRect;
	
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
			bVal = !bVal;
		}
	}
	
};