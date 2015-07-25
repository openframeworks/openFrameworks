#include "ofxGuiGroup.h"
#include "ofxPanel.h"
#include "ofxSliderGroup.h"
#include "ofGraphics.h"
#include "ofxLabel.h"
using namespace std;

ofxGuiGroup::ofxGuiGroup()
:ofxBaseGui(Config())
,spacing(Config().spacing)
,spacingNextElement(Config().spacingNextElement)
,spacingFirstElement(Config().spacingFirstElement)
,header(Config().header)
,filename(Config().filename)
,minimized(Config().minimized)
,bGuiActive(false){
}

ofxGuiGroup::ofxGuiGroup(const ofParameterGroup & _parameters)
:ofxBaseGui(Config())
,spacing(Config().spacing)
,spacingNextElement(Config().spacingNextElement)
,spacingFirstElement(Config().spacingFirstElement)
,header(Config().header)
,filename(Config().filename)
,minimized(Config().minimized)
,bGuiActive(false){
	addParametersFrom(_parameters);
	parameters = _parameters;
	registerMouseEvents();
	setNeedsRedraw();
}

ofxGuiGroup::ofxGuiGroup(const ofParameterGroup & _parameters, const Config & config)
:ofxBaseGui(config)
,spacing(config.spacing)
,spacingNextElement(config.spacingNextElement)
,spacingFirstElement(config.spacingFirstElement)
,header(config.header)
,filename(config.filename)
,minimized(config.minimized)
,bGuiActive(false){
	addParametersFrom(_parameters);
	parameters = _parameters;
	registerMouseEvents();
	setNeedsRedraw();
}

ofxGuiGroup::ofxGuiGroup(const ofParameterGroup & parameters, const std::string& filename, float x, float y){
	minimized = false;
	spacing = 1;
	spacingNextElement = 3;
	spacingFirstElement = 0;
	header = defaultHeight;
	setup(parameters, filename, x, y);
}

ofxGuiGroup::~ofxGuiGroup(){
	for(auto e: collection){
		ofRemoveListener(e->sizeChangedE,this,&ofxGuiGroup::sizeChangedCB);
	}
}

ofxGuiGroup & ofxGuiGroup::setup(const std::string& collectionName, const std::string& filename, float x, float y){
	parameters.setName(collectionName);
	return setup(parameters, filename, x, y);
}

ofxGuiGroup & ofxGuiGroup::setup(const ofParameterGroup & _parameters, const std::string& _filename, float x, float y){
	b.x = x;
	b.y = y;
	spacing = 1;
	b.width = defaultWidth;
	clear();
	filename = _filename;
	bGuiActive = false;

	addParametersFrom(_parameters);
	parameters = _parameters;
	registerMouseEvents();
	setNeedsRedraw();

	return *this;
}


void ofxGuiGroup::addParametersFrom(const ofParameterGroup & parameters){
	for(auto & p: parameters){
		if(p->isReadOnly()){
			ofLogWarning("ofxGui") << "Trying to add " << p->getName() << ": read only parameters not supported yet in ofxGui";
			continue;
		}
		string type = p->type();
		if(type == typeid(ofParameter<int>).name()){
			add(p->cast<int>());
		}else if(type == typeid(ofParameter<float>).name()){
			add(p->cast<float>());
		}else if(type == typeid(ofParameter<bool>).name()){
			add(p->cast<bool>());
		}else if(type == typeid(ofParameter<ofVec2f>).name()){
			add(p->cast<ofVec2f>());
		}else if(type == typeid(ofParameter<ofVec3f>).name()){
			add(p->cast<ofVec3f>());
		}else if(type == typeid(ofParameter<ofVec4f>).name()){
			add(p->cast<ofVec4f>());
		}else if(type == typeid(ofParameter<ofColor>).name()){
			add(p->cast<ofColor>());
		}else if(type == typeid(ofParameter<ofShortColor>).name()){
			add(p->cast<ofShortColor>());
		}else if(type == typeid(ofParameter<ofFloatColor>).name()){
			add(p->cast<ofFloatColor>());
		}else if(type == typeid(ofParameter<string>).name()){
			add(p->cast<string>());
		}else if(type == typeid(ofParameterGroup).name()){
			add(p->castGroup());
		}else{
			ofLogWarning("ofxGui") << "Trying to add " << p->getName() << ": ofxBaseGroup; no control for parameter of type " << type;
		}
	}
}

void ofxGuiGroup::add(ofxBaseGui & element){
	add(&element);
}


void ofxGuiGroup::add(ofxGuiGroup & element){
	add(&element);
}

void ofxGuiGroup::add(ofxBaseGui * element){
	collection.push_back(element);
	element->setPosition(b.x, b.y + b.height  + spacing);
	element->setSize(getWidth(), element->getHeight());
	b.height += element->getHeight() + spacing;

	element->unregisterMouseEvents();
	ofAddListener(element->sizeChangedE,this,&ofxGuiGroup::sizeChangedCB);

	parameters.add(element->getParameter());
	setNeedsRedraw();
}

void ofxGuiGroup::add(ofxGuiGroup * element){
	element->spacingFirstElement = 3;
	element->filename = filename;
	add(static_cast<ofxBaseGui*>(element));

}

void ofxGuiGroup::addOwned(ofxBaseGui * element){
	collectionOwned.emplace_back(element);
	add(element);
}

void ofxGuiGroup::addOwned(ofxGuiGroup * element){
	collectionOwned.emplace_back(element);
	add(element);
}

void ofxGuiGroup::setWidthElements(float w){
    for(auto & e: collection){
		e->setSize(w, e->getHeight());
		e->setPosition(b.x + b.width - w, e->getPosition().y);
	}
	sizeChangedCB();
	setNeedsRedraw();
}

void ofxGuiGroup::add(const ofParameterGroup & parameters){
	addOwned(new ofxGuiGroup(parameters));
}

void ofxGuiGroup::add(ofParameter <float> & parameter){
	addOwned(new ofxFloatSlider(parameter, b.width));
}

void ofxGuiGroup::add(ofParameter <int> & parameter){
	addOwned(new ofxIntSlider(parameter, b.width));
}

void ofxGuiGroup::add(ofParameter <bool> & parameter){
	addOwned(new ofxToggle(parameter, b.width));
}

void ofxGuiGroup::add(ofParameter <string> & parameter){
	addOwned(new ofxLabel(parameter, b.width));
}

void ofxGuiGroup::add(ofParameter <ofVec2f> & parameter){
	addOwned(new ofxVecSlider_ <ofVec2f>(parameter, b.width));
}

void ofxGuiGroup::add(ofParameter <ofVec3f> & parameter){
	addOwned(new ofxVecSlider_ <ofVec3f>(parameter, b.width));
}

void ofxGuiGroup::add(ofParameter <ofVec4f> & parameter){
	addOwned(new ofxVecSlider_ <ofVec4f>(parameter, b.width));
}

void ofxGuiGroup::add(ofParameter <ofColor> & parameter){
	addOwned(new ofxColorSlider_ <unsigned char>(parameter, b.width));
}

void ofxGuiGroup::add(ofParameter <ofShortColor> & parameter){
	addOwned(new ofxColorSlider_ <unsigned short>(parameter, b.width));
}

void ofxGuiGroup::add(ofParameter <ofFloatColor> & parameter){
	addOwned(new ofxColorSlider_ <float>(parameter, b.width));
}

void ofxGuiGroup::clear(){
	collection.clear();
	parameters.clear();
	b.height = header + spacing + spacingNextElement;
	sizeChangedCB();
}

bool ofxGuiGroup::mouseMoved(ofMouseEventArgs & args){
	ofMouseEventArgs a = args;
	for(auto & e: collection){
		if(e->mouseMoved(a)){
			return true;
		}
	}
	if(isGuiDrawing() && b.inside(ofPoint(args.x, args.y))){
		return true;
	}else{
		return false;
	}
}

bool ofxGuiGroup::mousePressed(ofMouseEventArgs & args){
	if(setValue(args.x, args.y, true)){
		return true;
	}
	if(bGuiActive){
		ofMouseEventArgs a = args;
		for(auto & e: collection){
			if(e->mousePressed(a)){
				return true;
			}
		}
	}
	return false;
}

bool ofxGuiGroup::mouseDragged(ofMouseEventArgs & args){
	if(setValue(args.x, args.y, false)){
		return true;
	}
	if(bGuiActive){
		ofMouseEventArgs a = args;
		for(auto & e: collection){
			if(e->mouseDragged(a)){
				return true;
			}
		}
	}
	return false;
}

bool ofxGuiGroup::mouseReleased(ofMouseEventArgs & args){
	bGuiActive = false;
	for(auto & e: collection){
		ofMouseEventArgs a = args;
		if(e->mouseReleased(a)){
			return true;
		}
	}
	if(isGuiDrawing() && b.inside(ofPoint(args.x, args.y))){
		return true;
	}else{
		return false;
	}
}

bool ofxGuiGroup::mouseScrolled(ofMouseEventArgs & args){
	ofMouseEventArgs a = args;
	for(auto & e: collection){
		if(e->mouseScrolled(a)){
			return true;
		}
	}
	if(isGuiDrawing() && b.inside(ofPoint(args.x, args.y))){
		return true;
	}else{
		return false;
	}
}

void ofxGuiGroup::generateDraw(){
	border.clear();
	border.setFillColor(ofColor(thisBorderColor, 180));
	border.setFilled(true);
	border.rectangle(b.x, b.y + spacingNextElement, b.width + 1, b.height);


	headerBg.clear();
	headerBg.setFillColor(thisHeaderBackgroundColor);
	headerBg.setFilled(true);
	headerBg.rectangle(b.x, b.y + 1 + spacingNextElement, b.width, header);

	textMesh = getTextMesh(getName(), textPadding + b.x, header / 2 + 4 + b.y + spacingNextElement);
	if(minimized){
		textMesh.append(getTextMesh("+", b.width - textPadding - 8 + b.x, header / 2 + 4 + b.y + spacingNextElement));
	}else{
		textMesh.append(getTextMesh("-", b.width - textPadding - 8 + b.x, header / 2 + 4 + b.y + spacingNextElement));
	}
}

void ofxGuiGroup::render(){
	border.draw();
	headerBg.draw();

	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode != OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
	ofColor c = ofGetStyle().color;
	ofSetColor(thisTextColor);

	bindFontTexture();
	textMesh.draw();
	unbindFontTexture();

	if(!minimized){
		for(std::size_t i = 0; i < collection.size(); i++){
			collection[i]->draw();
		}
	}

	ofSetColor(c);
	if(blendMode != OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}

vector <string> ofxGuiGroup::getControlNames() const{
	vector <string> names;
	for(auto & e: collection){
		names.push_back(e->getName());
	}
	return names;
}

ofxIntSlider & ofxGuiGroup::getIntSlider(const std::string& name){
	return getControlType <ofxIntSlider>(name);
}

ofxFloatSlider & ofxGuiGroup::getFloatSlider(const std::string& name){
	return getControlType <ofxFloatSlider>(name);
}

ofxToggle & ofxGuiGroup::getToggle(const std::string& name){
	return getControlType <ofxToggle>(name);
}

ofxButton & ofxGuiGroup::getButton(const std::string& name){
	return getControlType <ofxButton>(name);
}

ofxGuiGroup & ofxGuiGroup::getGroup(const std::string& name){
	return getControlType <ofxGuiGroup>(name);
}

ofxBaseGui * ofxGuiGroup::getControl(const std::string& name){
	for(auto & e: collection){
		if(e->getName() == name){
			return e;
		}
	}
	return nullptr;
}

bool ofxGuiGroup::setValue(float mx, float my, bool bCheck){
	if(!isGuiDrawing()){
		bGuiActive = false;
		return false;
	}


	if(bCheck){
		if(b.inside(mx, my)){
			bGuiActive = true;

			ofRectangle minButton(b.x + b.width - textPadding * 3, b.y, textPadding * 3, header);
			if(minButton.inside(mx, my)){
				minimized = !minimized;
				if(minimized){
					minimize();
				}else{
					maximize();
				}
				return true;
			}
		}
	}

	return false;
}

void ofxGuiGroup::minimize(){
	minimized = true;
	b.height = header + spacing + spacingNextElement + 1 /*border*/;
	sizeChangedE.notify(this);
	setNeedsRedraw();
}

void ofxGuiGroup::maximize(){
	minimized = false;
	for(auto & e: collection){
		b.height += e->getHeight() + spacing;
	}
	sizeChangedE.notify(this);
	setNeedsRedraw();
}

void ofxGuiGroup::minimizeAll(){
	for(auto & e: collection){
		ofxGuiGroup * group = dynamic_cast <ofxGuiGroup *>(e);
		if(group){
			group->minimize();
		}
	}
}

void ofxGuiGroup::maximizeAll(){
	for(auto & e: collection){
		ofxGuiGroup * group = dynamic_cast <ofxGuiGroup *>(e);
		if(group){
			group->maximize();
		}
	}
}

void ofxGuiGroup::sizeChangedCB(){
	float y = b.y  + header + spacing + spacingFirstElement;
	for(auto & e: collection){
		e->setPosition(e->getPosition().x, y + spacing);
		y += e->getHeight() + spacing;
	}
	b.height = y - b.y;
	sizeChangedE.notify(this);
	setNeedsRedraw();
}


std::size_t ofxGuiGroup::getNumControls() const {
	return collection.size();
}

ofxBaseGui * ofxGuiGroup::getControl(std::size_t num){
	if(num < collection.size()){
		return collection[num];
	}else{
		return NULL;
	}
}

ofAbstractParameter & ofxGuiGroup::getParameter(){
	return parameters;
}

void ofxGuiGroup::setPosition(ofPoint p){
	ofVec2f diff = p - b.getPosition();

	for(auto & e: collection){
		e->setPosition(e->getPosition() + diff);
	}

	b.setPosition(p);
	setNeedsRedraw();
}

void ofxGuiGroup::setPosition(float x, float y){
	setPosition(ofVec2f(x, y));
}

void ofxGuiGroup::setSize(float w, float h){
	ofxBaseGui::setSize(w,h);
	setWidthElements(w * .98);
}

void ofxGuiGroup::setShape(ofRectangle r){
	ofxBaseGui::setShape(r);
	setWidthElements(r.width * .98);
}

void ofxGuiGroup::setShape(float x, float y, float w, float h){
	ofxBaseGui::setShape(x,y,w,h);
	setWidthElements(w * .98);
}
