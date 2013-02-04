#include "ofxGuiGroup.h"
#include "ofxPanel.h"
#include "ofxSliderGroup.h"
#include "ofGraphics.h"
#include "ofxLabel.h"

ofxGuiGroup::ofxGuiGroup(){
	minimized = false;
	parent = NULL;
	spacing  = 1;
	spacingNextElement = 3;
	header = defaultHeight;
}

ofxGuiGroup::ofxGuiGroup(const ofParameterGroup & parameters, string filename, float x, float y){
	minimized = false;
	parent = NULL;
    setup(parameters, filename, x, y);
}

ofxGuiGroup * ofxGuiGroup::setup(string collectionName, string filename, float x, float y){
	parameters.setName(collectionName);
	return setup(parameters,filename,x,y);
}


ofxGuiGroup * ofxGuiGroup::setup(const ofParameterGroup & _parameters, string _filename, float x, float y){
	b.x = x;
	b.y = y;
	header = defaultHeight;
	spacing  = 1;
	spacingNextElement = 3;
	if(parent!=NULL){
		b.width = parent->getWidth();
	}else{
		b.width = defaultWidth;
	}
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

	parameters = _parameters;
	ofRegisterMouseEvents(this);

	generateDraw();
    
	return this;
}

void ofxGuiGroup::add(ofxBaseGui * element){
	collection.push_back( element );

	element->setPosition(b.x, b.y + b.height  + spacing);

	b.height += element->getHeight() + spacing;

	//if(b.width<element->getWidth()) b.width = element->getWidth();
    
	ofUnregisterMouseEvents(element);
    
	ofxGuiGroup * subgroup = dynamic_cast<ofxGuiGroup*>(element);
	if(subgroup!=NULL){
		subgroup->filename = filename;
		subgroup->parent = this;
		subgroup->setWidthElements(b.width*.98);
	}else{
		if(parent!=NULL){
			element->setSize(b.width*.98,element->getHeight());
			element->setPosition(b.x + b.width-element->getWidth(),element->getPosition().y);
		}
	}
    
	parameters.add(element->getParameter());
	generateDraw();
}

void ofxGuiGroup::setWidthElements(float w){
	for(int i=0;i<(int)collection.size();i++){
		collection[i]->setSize(w,collection[i]->getHeight());
		collection[i]->setPosition(b.x + b.width-w,collection[i]->getPosition().y);
		ofxGuiGroup * subgroup = dynamic_cast<ofxGuiGroup*>(collection[i]);
		if(subgroup!=NULL){
			subgroup->setWidthElements(w*.98);
		}
	}
	sizeChangedCB();
	generateDraw();
}

void ofxGuiGroup::add(const ofParameterGroup & parameters){
	ofxGuiGroup * panel = new ofxGuiGroup(parameters);
	panel->parent = this;
	add(panel);
}

void ofxGuiGroup::add(ofParameter<float> & parameter){
	add(new ofxFloatSlider(parameter,b.width));
}

void ofxGuiGroup::add(ofParameter<int> & parameter){
	add(new ofxIntSlider(parameter,b.width));
}

void ofxGuiGroup::add(ofParameter<bool> & parameter){
	add(new ofxToggle(parameter,b.width));
}

void ofxGuiGroup::add(ofParameter<string> & parameter){
	add(new ofxLabel(parameter,b.width));
}

void ofxGuiGroup::add(ofParameter<ofVec2f> & parameter){
	add(new ofxVecSlider_<ofVec2f>(parameter,b.width));
}

void ofxGuiGroup::add(ofParameter<ofVec3f> & parameter){
	add(new ofxVecSlider_<ofVec3f>(parameter,b.width));
}

void ofxGuiGroup::add(ofParameter<ofVec4f> & parameter){
	add(new ofxVecSlider_<ofVec4f>(parameter,b.width));
}

void ofxGuiGroup::add(ofParameter<ofColor> & parameter){
	add(new ofxColorSlider_<unsigned char>(parameter,b.width));
}

void ofxGuiGroup::add(ofParameter<ofShortColor> & parameter){
	add(new ofxColorSlider_<unsigned short>(parameter,b.width));
}

void ofxGuiGroup::add(ofParameter<ofFloatColor> & parameter){
	add(new ofxColorSlider_<float>(parameter,b.width));
}

void ofxGuiGroup::clear(){
	collection.clear();
	b.height = header + spacing + spacingNextElement ;
}

void ofxGuiGroup::mouseMoved(ofMouseEventArgs & args){
	ofMouseEventArgs a = args;
	for(int i = 0; i < (int)collection.size(); i++){
		collection[i]->mouseMoved(a);
	}
}

void ofxGuiGroup::mousePressed(ofMouseEventArgs & args){
	setValue(args.x, args.y, true);
	if( bGuiActive ){
		ofMouseEventArgs a = args;
		for(int i = 0; i < (int)collection.size(); i++){
			collection[i]->mousePressed(a);
		}
	}
}

void ofxGuiGroup::mouseDragged(ofMouseEventArgs & args){
	setValue(args.x, args.y, false);
	if( bGuiActive ){
		ofMouseEventArgs a = args;
		for(int i = 0; i < (int)collection.size(); i++){
			collection[i]->mouseDragged(a);
		}
	}
}

void ofxGuiGroup::mouseReleased(ofMouseEventArgs & args){
	bGuiActive = false;
	for(int k = 0; k < (int)collection.size(); k++){
		ofMouseEventArgs a = args;
		collection[k]->mouseReleased(a);
	}
}

void ofxGuiGroup::generateDraw(){
	border.clear();
	border.setFillColor(ofColor(thisBorderColor,180));
	border.setFilled(true);
	border.moveTo(b.x,b.y+ spacingNextElement);
	border.lineTo(b.x+b.width+1,b.y+ spacingNextElement);
	border.lineTo(b.x+b.width+1,b.y+b.height+ spacingNextElement);
	border.lineTo(b.x,b.y+b.height+ spacingNextElement);
	border.close();


	headerBg.clear();
	headerBg.setFillColor(thisHeaderBackgroundColor);
	headerBg.setFilled(true);
	headerBg.moveTo(b.x,b.y +1 + spacingNextElement);
	headerBg.lineTo(b.x+b.width,b.y+1+ spacingNextElement);
	headerBg.lineTo(b.x+b.width,b.y+header+1+ spacingNextElement);
	headerBg.lineTo(b.x,b.y+header+1+ spacingNextElement);
	headerBg.close();

	textMesh = font.getStringMesh(getName(), textPadding + b.x, header / 2 + 4 + b.y+ spacingNextElement);
	if(minimized){
		textMesh.append(font.getStringMesh("+", b.width-textPadding-8 + b.x, header / 2 + 4+ b.y+ spacingNextElement));
	}else{
		textMesh.append(font.getStringMesh("-", b.width-textPadding-8 + b.x, header / 2 + 4 + b.y+ spacingNextElement));
	}
}

void ofxGuiGroup::draw(){

	currentFrame = ofGetFrameNum();

	border.draw();
	headerBg.draw();

	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
	ofColor c = ofGetStyle().color;
	ofSetColor(thisTextColor);
	font.getFontTexture().bind();
	/*font.drawString(getName(), textPadding + b.x, header / 2 + 4 + b.y+ spacingNextElement);
	if(minimized){
		font.drawString("+", b.width-textPadding-8 + b.x, header / 2 + 4+ b.y+ spacingNextElement);
	}else{
		font.drawString("-", b.width-textPadding-8 + b.x, header / 2 + 4 + b.y+ spacingNextElement);
	}*/
	textMesh.draw();
	font.getFontTexture().unbind();
    
	if(!minimized){
		for(int i = 0; i < (int)collection.size(); i++){
			collection[i]->draw();
		}
	}

	ofSetColor(c);
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}

vector<string> ofxGuiGroup::getControlNames(){
	vector<string> names;
	for(int i=0; i<(int)collection.size(); i++){
		names.push_back(collection[i]->getName());
	}
	return names;
}

ofxIntSlider & ofxGuiGroup::getIntSlider(string name){
	return getControlType<ofxIntSlider>(name);
}

ofxFloatSlider & ofxGuiGroup::getFloatSlider(string name){
	return getControlType<ofxFloatSlider>(name);
}

ofxToggle & ofxGuiGroup::getToggle(string name){
	return getControlType<ofxToggle>(name);
}

ofxButton & ofxGuiGroup::getButton(string name){
	return getControlType<ofxButton>(name);
}

ofxGuiGroup & ofxGuiGroup::getGroup(string name){
	return getControlType<ofxGuiGroup>(name);
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


	if( bCheck ){
		ofRectangle minButton(b.x+b.width-textPadding-10,b.y,10,header);
		if(minButton.inside(mx,my)){
			minimized = !minimized;
			if(minimized){
				minimize();
			}else{
				maximize();
			}
		}
		if( b.inside(mx, my) ){
			bGuiActive = true;
        }
	}

}

void ofxGuiGroup::minimize(){
	minimized=true;
	b.height = header + spacing + spacingNextElement + 1 /*border*/;
	if(parent) parent->sizeChangedCB();
	generateDraw();
}

void ofxGuiGroup::maximize(){
	minimized=false;
	for(int i=0;i<(int)collection.size();i++){
		b.height += collection[i]->getHeight() + spacing;
	}
	if(parent) parent->sizeChangedCB();
	generateDraw();
}

void ofxGuiGroup::minimizeAll(){
	for(int i=0;i<(int)collection.size();i++){
		ofxGuiGroup * group = dynamic_cast<ofxGuiGroup*>(collection[i]);
		if(group)group->minimize();
	}
}

void ofxGuiGroup::maximizeAll(){
	for(int i=0;i<(int)collection.size();i++){
		ofxGuiGroup * group = dynamic_cast<ofxGuiGroup*>(collection[i]);
		if(group)group->maximize();
	}
}

void ofxGuiGroup::sizeChangedCB(){
	float y;
	if(parent){
		y = b.y  + header + spacing + spacingNextElement;
	}else{
		y = b.y  + header + spacing;
	}
	for(int i=0;i<(int)collection.size();i++){
		collection[i]->setPosition(collection[i]->getPosition().x,y + spacing);
		y += collection[i]->getHeight() + spacing;
	}
	b.height = y - b.y;
	if(parent) parent->sizeChangedCB();
	generateDraw();
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

void ofxGuiGroup::setPosition(ofPoint p){
	ofVec2f diff = p - b.getPosition();

	for(int i=0;i<(int)collection.size();i++){
		collection[i]->setPosition(collection[i]->getPosition()+diff);
	}

	b.setPosition(p);
	generateDraw();
}

void ofxGuiGroup::setPosition(float x, float y){
	setPosition(ofVec2f(x,y));
}
