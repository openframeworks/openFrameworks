#pragma once

#include "ofBaseGui.h"

string saveStencilToHex(ofImage& img) {
	stringstream strm;
	int width = img.getWidth();
	int height = img.getHeight();
	int n = width * height;
	unsigned char cur = 0;
	int shift = 0;
	strm << "{";
	for(int i = 0; i < n;) {
		if(img.getPixels()[i * 4 + 3] > 0) {
			cur |= 1 << shift;
		}		
		i++;
		if(i % 8 == 0) {
			strm << "0x" << hex << (unsigned int) cur;
			cur = 0;
			shift = 0;
			if(i < n) {
				strm << ",";
			}
		} else {
			shift++;
		}
	}
	strm << "}";
	return strm.str();
}

void loadStencilFromHex(ofImage& img, unsigned char* data) {
	int width = img.getWidth();
	int height = img.getHeight();
	int i = 0;
	ofColor on(255, 255);
	ofColor off(255, 0);
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int shift = i % 8;
			int offset = i / 8;
			bool cur = (data[offset] >> shift) & 1;
			img.setColor(x, y, cur ? on : off);
			i++;
		}
	}
	img.update();
}

class ofPanel : public ofBaseGui{
public:
	
	void setup(float x = 10, float y = 10) {
		setup("", x, y);
	}
	
	void setup(string collectionName, float x = 10, float y = 10){
		name = collectionName;
		b.x = x;
		b.y = y;
		header = defaultHeight;
		spacing  = 1;
		currentY = header + spacing;
		b.width = defaultWidth;
		b.height = 100; // weird to start out with something arbitrary like this
		filename = "settings.xml";
		ofRegisterMouseEvents(this);
				
		unsigned char loadIconData[] = {0x38,0x88,0xa,0x6,0x7e,0x60,0x50,0x11,0x1c};
		unsigned char saveIconData[] = {0xff,0x4a,0x95,0xea,0x15,0xa8,0x57,0xa9,0x7f};
		loadIcon.allocate(9, 8, OF_IMAGE_COLOR_ALPHA);
		saveIcon.allocate(9, 8, OF_IMAGE_COLOR_ALPHA);
		loadStencilFromHex(loadIcon, loadIconData);
		loadStencilFromHex(saveIcon, saveIconData);
		int iconSpacing = 6;
		loadBox.x = b.width - (loadIcon.getWidth() + saveIcon.getWidth() + iconSpacing + textPadding);
		loadBox.y = header / 2 - loadIcon.getHeight() / 2;
		loadBox.width = loadIcon.getWidth();
		loadBox.height = loadIcon.getHeight();
		saveBox.set(loadBox);
		saveBox.x += loadIcon.getWidth() + iconSpacing;
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
		
		ofTranslate(b.x, b.y);
		
		ofFill();
		ofSetColor(headerBackgroundColor);
		ofRect(0, 0, b.width, header);			
		
		ofSetColor(textColor);
		ofDrawBitmapString(name, textPadding, header / 2 + 4);
		
		ofPushMatrix();
		loadIcon.draw(loadBox.x, loadBox.y);
		saveIcon.draw(saveBox.x, saveBox.y);
		ofPopMatrix();
		
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
				} else{
					bGrabbed = false;
				}
				
				if(loadBox.inside(mx - b.x, my - b.y)) {
					loadFromFile(filename);
				}
				if(saveBox.inside(mx - b.x, my - b.y)) {
					saveToFile(filename);
				}
			}
		} else if( bGrabbed ){
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
	
	string filename;
	ofImage loadIcon, saveIcon;
	ofRectangle loadBox, saveBox;
};
