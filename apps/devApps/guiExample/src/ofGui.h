
#include "ofProperty.h"

class ofBaseGui{
	public:
		ofBaseGui(){
			bGuiActive = false;
		}
		
		virtual void mouseMoved(ofMouseEventArgs & args) = 0;
		virtual void mousePressed(ofMouseEventArgs & args) = 0;
		virtual void mouseDragged(ofMouseEventArgs & args) = 0;
		virtual void mouseReleased(ofMouseEventArgs & args) = 0;
		
		virtual void setValue(float mx, float my, bool bCheckBounds) = 0;
		virtual void draw() = 0;
		
		string name;
		unsigned long currentFrame;			
		ofRectangle b;
		bool bGuiActive;
}; 

class ofGuiCollection : public ofBaseGui{
	public:
	
		void setup(string collectionName, float x, float y){
			name = collectionName;
			b.x	 = x;
			b.y	 = y;
			header = 18;
			spacing  = 4;
			currentY = header + spacing;
			b.width = 200;
			b.height = 100;
			ofRegisterMouseEvents(this);			
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
			for(int k = 0; k < collection.size(); k++){
				collection[k]->mouseMoved(a);
			}		
		}
		
		virtual void mousePressed(ofMouseEventArgs & args){
			setValue(args.x, args.y, true);
			if( bGuiActive ){
				ofMouseEventArgs a = args;
				a.x -= b.x;
				a.y -= b.y;
				for(int k = 0; k < collection.size(); k++){
					collection[k]->mousePressed(a);
				}
			}
		}
		
		virtual void mouseDragged(ofMouseEventArgs & args){
			setValue(args.x, args.y, false);	
			if( bGuiActive ){
				ofMouseEventArgs a = args;
				a.x -= b.x;
				a.y -= b.y;			
				for(int k = 0; k < collection.size(); k++){
					collection[k]->mouseDragged(a);
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
			currentFrame = ofGetFrameNum();
		
			ofPushStyle();
			ofFill();
			
			//ofSetColor(10, 10, 10, 170);
			//ofRect(b);	

			ofSetColor(30, 30, 80);
			ofRect(b.x, b.y, b.width, header);			
			
			ofSetColor(230);
			ofDrawBitmapString(name, b.x + 4, b.y + 12);

			ofPushMatrix();
				ofTranslate(b.x, b.y, 0);		
				for(int k = 0; k < collection.size(); k++){
					collection[k]->draw();
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


class ofSlider : public ofBaseGui{
	friend class ofGuiCollection;

	public:
	
		void setup(string sliderName, double _val, double _min, double _max, bool _bInt = false, float width = 200, float height = 20){
			name		= sliderName;
			val			= _val;
			min			= _min;
			max			= _max;
			b.x			= 0;
			b.y			= 0;
			b.width		= width;
			b.height	= height;
			currentFrame = 0;			
			bGuiActive = false;
			bInt	   = _bInt;
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
			ofSetColor(30, 30, 80);
			ofRect(b);

			float valAsPct = ofMap( val, min, max, 0, b.width, true );
			ofEnableAlphaBlending();
			ofSetColor(180, 180, 180);			
			ofRect(b.x+1, b.y+1, valAsPct-1, b.height-2);

			ofSetColor(230, 230, 230, 255);			
			
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
			string valStr = ofToString(bVal, 0);
			
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


class ofColorSlider : public ofBaseGui{
	public:

};

