#pragma once

#include "ofParameter.h"
#include "ofxBaseGui.h"

class ofxToggle : public ofxBaseGui{
	friend class ofPanel;
	
public:
	ofxToggle * setup(string toggleName, ofxParameter<bool> _bVal, float width = defaultWidth, float height = defaultHeight){
		name = toggleName;
		b.x = 0;
		b.y = 0;
		b.width = width;
		b.height = height;
		currentFrame = 0;			
		bGuiActive = false;
		value = _bVal;
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
		string xmlName = name;
		ofStringReplace(xmlName," ","_");
		xml.setValue(xmlName, value);
	}
	
	virtual void loadFromXml(ofxXmlSettings& xml) {
		string xmlName = name;
		ofStringReplace(xmlName," ","_");
		value = xml.getValue(xmlName, value);
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
		
		if( value ){
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
		string valStr = value ? "true" : "false";
		ofDrawBitmapString(valStr, b.width - textPadding - valStr.length() * 8, 0);
		
		ofPopMatrix();
		ofPopStyle();
	}

	template<class ListenerClass>
	void addListener(ListenerClass * listener, void ( ListenerClass::*method )(bool&)){
		ofAddListener(value.changedE,listener,method);
	}

	template<class ListenerClass>
	void removeListener(ListenerClass * listener, void ( ListenerClass::*method )(bool&)){
		ofRemoveListener(value.changedE,listener,method);
	}


	double operator=(bool v){
		value = v;
		return v;
	}

	operator bool & (){
		return value;
	}

	ofxParameter<bool> value;
protected:
	ofRectangle checkboxRect;
	
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
			value = !value;
		}
	}
	
};
