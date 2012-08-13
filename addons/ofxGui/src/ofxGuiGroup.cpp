#include "ofxGuiGroup.h"
#include "ofxPanel.h"
#include "ofxSliderGroup.h"

ofxGuiGroup::ofxGuiGroup(string collectionName, string filename, float x, float y){
	minimized = false;
	parent = NULL;
    setup(collectionName, filename, x, y);
}

ofxGuiGroup::ofxGuiGroup(const ofParameterGroup & parameters, string filename, float x, float y){
	minimized = false;
	parent = NULL;
    setup(parameters, filename, x, y);
}

ofxGuiGroup * ofxGuiGroup::setup(string collectionName, string filename, float x, float y){
	name = collectionName;
	parameters.setName(name);
	b.x = x;
	b.y = y;
	header = defaultHeight;
	spacing  = 1;
	b.width = defaultWidth;
	b.height = spacing;
    this->clear();
	this->filename = filename;
    
	return this;
}


ofxGuiGroup * ofxGuiGroup::setup(const ofParameterGroup & _parameters, string _filename, float x, float y){
	name = _parameters.getName();
	parameters.setName(name);
	b.x = x;
	b.y = y;
	header = defaultHeight;
	spacing  = 1;
	b.width = defaultWidth;
	b.height = spacing;
    clear();
	filename = _filename;
    
	for(int i=0;i<_parameters.size();i++){
		string type = _parameters.getType(i);
		if(type==typeid(ofParameter<int>).name()){
			ofParameter<int> p = _parameters.getInt(i);
			add(p);
		}else if(type==typeid(ofParameter<float>).name()){
			ofParameter<float> p = _parameters.getFloat(i);
			add(p);
		}else if(type==typeid(ofParameter<bool>).name()){
			ofParameter<bool> p = _parameters.getBool(i);
			add(p);
		}else if(type==typeid(ofParameter<ofVec2f>).name()){
			ofParameter<ofVec2f> p = _parameters.getVec2f(i);
			add(p);
		}else if(type==typeid(ofParameter<ofVec3f>).name()){
			ofParameter<ofVec3f> p = _parameters.getVec3f(i);
			add(p);
		}else if(type==typeid(ofParameter<ofVec4f>).name()){
			ofParameter<ofVec4f> p = _parameters.getVec4f(i);
			add(p);
		}else if(type==typeid(ofParameter<ofColor>).name()){
			ofParameter<ofColor> p = _parameters.getColor(i);
			add(p);
		}else if(type==typeid(ofParameter<ofShortColor>).name()){
			ofParameter<ofShortColor> p = _parameters.getShortColor(i);
			add(p);
		}else if(type==typeid(ofParameter<ofFloatColor>).name()){
			ofParameter<ofFloatColor> p = _parameters.getFloatColor(i);
			add(p);
		}else if(type==typeid(ofParameterGroup).name()){
			ofParameterGroup p = _parameters.getGroup(i);
			ofxGuiGroup * panel = new ofxGuiGroup(p);
			add(panel);
		}else{
			ofLogError() << "ofxBaseGroup; can't add control of type " << type;
		}
	}

	ofRegisterMouseEvents(this);
    
	return this;
}

void ofxGuiGroup::add(ofxBaseGui * element){
	collection.push_back( element );
    
	element->setPosition(0, b.height);
    
	b.height += element->getHeight() + spacing;
	if(b.width<element->getWidth()) b.width = element->getWidth();
    
	ofUnregisterMouseEvents(element);
    
	ofxPanel * subpanel = dynamic_cast<ofxPanel*>(element);
	if(subpanel!=NULL){
		subpanel->filename = filename;
		subpanel->parent = this;
	}
    
	parameters.add(element->getParameter());
}

void ofxGuiGroup::add(const ofParameterGroup & parameters){
	ofxGuiGroup * panel = new ofxGuiGroup(parameters);
	panel->parent = this;
	add(panel);
}

void ofxGuiGroup::add(ofParameter<float> & parameter){
	add(new ofxFloatSlider(parameter.getName(),parameter));
}

void ofxGuiGroup::add(ofParameter<int> & parameter){
	add(new ofxIntSlider(parameter.getName(),parameter));
}

void ofxGuiGroup::add(ofParameter<bool> & parameter){
	add(new ofxToggle(parameter.getName(),parameter));
}

void ofxGuiGroup::add(ofParameter<ofVec2f> & parameter){
	add(new ofxVecSlider<ofVec2f>(parameter.getName(),parameter));
}

void ofxGuiGroup::add(ofParameter<ofVec3f> & parameter){
	add(new ofxVecSlider<ofVec3f>(parameter.getName(),parameter));
}

void ofxGuiGroup::add(ofParameter<ofVec4f> & parameter){
	add(new ofxVecSlider<ofVec4f>(parameter.getName(),parameter));
}

void ofxGuiGroup::add(ofParameter<ofColor> & parameter){
	add(new ofxColorSlider<unsigned char>(parameter.getName(),parameter));
}

void ofxGuiGroup::add(ofParameter<ofShortColor> & parameter){
	add(new ofxColorSlider<unsigned short>(parameter.getName(),parameter));
}

void ofxGuiGroup::add(ofParameter<ofFloatColor> & parameter){
	add(new ofxColorSlider<float>(parameter.getName(),parameter));
}

void ofxGuiGroup::clear(){
	collection.clear();
	b.height = header + spacing; // weird to start out with something arbitrary like this
}

void ofxGuiGroup::mouseMoved(ofMouseEventArgs & args){
	ofMouseEventArgs a = args;
	a.x -= b.x;
	a.y -= b.y;
	for(int i = 0; i < (int)collection.size(); i++){
		collection[i]->mouseMoved(a);
	}
}

void ofxGuiGroup::mousePressed(ofMouseEventArgs & args){
	setValue(args.x, args.y, true);
	if( bGuiActive ){
		ofMouseEventArgs a = args;
		a.x -= b.x;
		a.y -= b.y;
		for(int i = 0; i < (int)collection.size(); i++){
			collection[i]->mousePressed(a);
		}
	}
}

void ofxGuiGroup::mouseDragged(ofMouseEventArgs & args){
	setValue(args.x, args.y, false);
	if( bGuiActive ){
		ofMouseEventArgs a = args;
		a.x -= b.x;
		a.y -= b.y;
		for(int i = 0; i < (int)collection.size(); i++){
			collection[i]->mouseDragged(a);
		}
	}
}

void ofxGuiGroup::mouseReleased(ofMouseEventArgs & args){
	bGuiActive = false;
	for(int k = 0; k < (int)collection.size(); k++){
		ofMouseEventArgs a = args;
		a.x -= b.x;
		a.y -= b.y;
		collection[k]->mouseReleased(a);
	}
}

void ofxGuiGroup::draw(){
	ofPushStyle();
	ofPushMatrix();
    
	currentFrame = ofGetFrameNum();
    
	ofTranslate(b.x, b.y);
    
	ofFill();
	ofSetColor(headerBackgroundColor);
	ofRect(0, 0, b.width, header);
    
	ofSetColor(textColor);
	ofDrawBitmapString(name, textPadding, header / 2 + 4);
	if(minimized){
		ofDrawBitmapString("+", b.width-textPadding-8, header / 2 + 4);
	}else{
		ofDrawBitmapString("-", b.width-textPadding-8, header / 2 + 4);
	}
    
	if(!minimized){
		for(int i = 0; i < (int)collection.size(); i++){
			collection[i]->draw();
		}
	}
    
	ofPopMatrix();
	ofPopStyle();
}

vector<string> ofxGuiGroup::getControlNames(){
	vector<string> names;
	for(int i=0; i<(int)collection.size(); i++){
		names.push_back(collection[i]->getName());
	}
	return names;
}

ofxIntSlider ofxGuiGroup::getIntSlider(string name){
	return getControlType<ofxIntSlider>(name);
}

ofxFloatSlider ofxGuiGroup::getFloatSlider(string name){
	return getControlType<ofxFloatSlider>(name);
}

ofxToggle ofxGuiGroup::getToggle(string name){
	return getControlType<ofxToggle>(name);
}

ofxButton ofxGuiGroup::getButton(string name){
	return getControlType<ofxButton>(name);
}

ofxBaseGui * ofxGuiGroup::getControl(string name){
	for(int i=0; i<(int)collection.size(); i++){
		if(collection[i]->getName()==name){
			return collection[i];
		}
	}
	return NULL;
}

void ofxGuiGroup::registerMouseEvents(){
    ofRegisterMouseEvents(this);
}

void ofxGuiGroup::setValue(float mx, float my, bool bCheck){
    
	if( ofGetFrameNum() - currentFrame > 1 ){
		bGuiActive = false;
		return;
	}

	ofRectangle minButton(b.x+b.width-textPadding-10,b.y,10,header);
	if(minButton.inside(mx,my)){
		minimized = !minimized;
		if(minimized){
			b.height = header + spacing;
		}else{
			for(int i=0;i<collection.size();i++){
				b.height += collection[i]->getHeight() + spacing;
			}
		}
		if(parent) parent->sizeChangedCB();
	}

	if( bCheck ){
		if( b.inside(mx, my) ){
			bGuiActive = true;
        }
	}

}

void ofxGuiGroup::sizeChangedCB(){
	float y = header + spacing;
	for(int i=0;i<collection.size();i++){
		collection[i]->setPosition(collection[i]->getPosition().x,y);
		y += collection[i]->getHeight();
	}
}


int ofxGuiGroup::getNumControls(){
	return collection.size();
}

ofxBaseGui * ofxGuiGroup::getControl(int num){
	if(num<(int)collection.size())
		return collection[num];
	else
		return NULL;
}

ofAbstractParameter & ofxGuiGroup::getParameter(){
	return parameters;
}
