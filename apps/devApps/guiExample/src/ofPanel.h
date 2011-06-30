#pragma once

#include "ofBaseGui.h"

class ofPanel : public ofBaseGui{
public:
	
	void setup(float x = 10, float y = 10) {
		setup("", x, y);
	}
	
	void setup(string collectionName, float x = 10, float y = 10){
		name = collectionName;
		b.x = x;
		b.y = y;
		header = 18;
		spacing  = 1;
		currentY = header + spacing;
		b.width = 200;
		b.height = 100;
		ofRegisterMouseEvents(this);
	}
	
	virtual void saveToXml(ofxXmlSettings& xml) {
		for(int i = 0; i < collection.size(); i++){
			collection[i]->saveToXml(xml);
		}
	}
	
	virtual void loadFromXml(ofxXmlSettings& xml) {
		for(int i = 0; i < collection.size(); i++){
			collection[i]->loadFromXml(xml);
		}
	}
	
	void add(ofBaseGui * element){
		collection.push_back( element );
		
		element->b.y = currentY;
		element->b.x = 0;
		currentY += element->b.height + spacing;
		
		if( currentY >= b.height ){
			b.height += 40;
		}
	}
	
	void clear(){
		collection.clear();
	}
	
	virtual void mouseMoved(ofMouseEventArgs & args){
		ofMouseEventArgs a = args;
		a.x -= b.x;
		a.y -= b.y;		
		for(int i = 0; i < collection.size(); i++){
			collection[i]->mouseMoved(a);
		}		
	}
	
	virtual void mousePressed(ofMouseEventArgs & args){
		setValue(args.x, args.y, true);
		if( bGuiActive ){
			ofMouseEventArgs a = args;
			a.x -= b.x;
			a.y -= b.y;
			for(int i = 0; i < collection.size(); i++){
				collection[i]->mousePressed(a);
			}
		}
	}
	
	virtual void mouseDragged(ofMouseEventArgs & args){
		setValue(args.x, args.y, false);	
		if( bGuiActive ){
			ofMouseEventArgs a = args;
			a.x -= b.x;
			a.y -= b.y;			
			for(int i = 0; i < collection.size(); i++){
				collection[i]->mouseDragged(a);
			}
		}
	}
	
	virtual void mouseReleased(ofMouseEventArgs & args){
		bGuiActive = false;
		for(int k = 0; k < collection.size(); k++){
			ofMouseEventArgs a = args;
			a.x -= b.x;
			a.y -= b.y;			
			collection[k]->mouseReleased(a);
		}
	}		
	
	void draw(){
		ofPushStyle();
		ofPushMatrix();
		
		currentFrame = ofGetFrameNum();
		
		ofFill();
		ofSetColor(headerBackgroundColor);
		ofRect(b.x, b.y, b.width, header);			
		
		ofSetColor(textColor);
		ofDrawBitmapString(name, b.x + 4, b.y + 12);
		
		ofTranslate(b.x, b.y, 0);		
		for(int i = 0; i < collection.size(); i++){
			collection[i]->draw();
		}
		
		ofPopMatrix();
		ofPopStyle();
	}
	
protected:
	
	void setValue(float mx, float my, bool bCheck){
		
		if( ofGetFrameNum() - currentFrame > 1 ){
			bGrabbed = false;
			bGuiActive = false;
			return; 
		}
		if( bCheck ){
			if( b.inside(mx, my) ){
				bGuiActive = true;
				
				if( my > b.y && my <= b.y + header ){
					bGrabbed = true;					
					grabPt.set(mx-b.x, my-b.y);
				}else{
					bGrabbed = false;
				}
			}
		}else if( bGrabbed ){
			b.x = mx - grabPt.x;
			b.y = my - grabPt.y;
		}
	}		
	
	ofPoint grabPt;
	bool bGrabbed;
	float currentY;
	float spacing;
	float header;
	vector <ofBaseGui *> collection;
};
