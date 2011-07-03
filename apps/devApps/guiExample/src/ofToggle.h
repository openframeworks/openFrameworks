#pragma once

#include "ofPanel.h"

class ofToggle : public ofBaseGui{
	friend class ofPanel;
	
public:
	ofToggle* setup(string toggleName, bool _bVal, float width = defaultWidth, float height = defaultHeight){
		name = toggleName;
		b.x = 0;
		b.y = 0;
		b.width = width;
		b.height = height;
		currentFrame = 0;			
		bGuiActive = false;
		bVal = _bVal;
		checkboxRect.set(1, 1, b.height - 2, b.height - 2);
		return this;
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
	
	virtual void saveToXml(ofxXmlSettings& xml) {
		cout << "warning we need to check for spaces in a name" << endl;	
		xml.addValue(name, bVal);
	}
	
	virtual void loadFromXml(ofxXmlSettings& xml) {
		cout << "warning we need to check for spaces in a name" << endl;		
		bVal = xml.getValue(name, bVal);
	}
	
	bool getValue(){
		return bVal;
	}
	
	void draw(){
		currentFrame = ofGetFrameNum();
		
		ofPushStyle();
		ofPushMatrix();
		
		ofSetColor(backgroundColor);
		ofRect(b);
		
		ofTranslate(b.x, b.y);
				
		ofFill();
		ofSetColor(fillColor);
		ofRect(checkboxRect);
		
		if( bVal ){			
			ofSetColor(textColor);
			ofLine(checkboxRect.x, checkboxRect.y, checkboxRect.x + checkboxRect.width, checkboxRect.y + checkboxRect.height);
			ofLine(checkboxRect.x, checkboxRect.y+ checkboxRect.height, checkboxRect.x + checkboxRect.width, checkboxRect.y);
		} else {
			ofSetColor(backgroundColor);
			ofRect(checkboxRect.x + 1, checkboxRect.y + 1, checkboxRect.width - 2, checkboxRect.height - 2);
		}
		
		ofSetColor(textColor);		
		ofTranslate(0, b.height / 2 + 4);
		ofDrawBitmapString(name, textPadding + checkboxRect.width, 0);
		string valStr = bVal ? "true" : "false";		
		ofDrawBitmapString(valStr, b.width - textPadding - valStr.length() * 8, 0);
		
		ofPopMatrix();
		ofPopStyle();
	}
	
protected:
	ofRectangle checkboxRect;
	bool bVal;
	
	void setValue(float mx, float my, bool bCheck){
	
		if( ofGetFrameNum() - currentFrame > 1 ){
			bGuiActive = false;
			return; 
		}
		if( bCheck ){
			ofRectangle checkRect = checkboxRect;
			checkRect.x += b.x;
			checkRect.y += b.y;
		
			if( checkRect.inside(mx, my) ){
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