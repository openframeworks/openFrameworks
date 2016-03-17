#include "ofxGuiGroup.h"
#include "ofxPanel.h"
#include "ofGraphics.h"
#include "ofxSliderGroup.h"
#include "ofxGuiTabs.h"
#include "ofxFpsPlotter.h"
#include "JsonConfigParser.h"

ofxGuiGroupHeader::ofxGuiGroupHeader(const ofJson &config):ofxBaseGui(config){
	registerMouseEvents();
}

void ofxGuiGroupHeader::generateDraw(){
	ofxBaseGui::generateDraw();

	ofxGuiGroup* _parent = dynamic_cast<ofxGuiGroup*>(parent());
	if(_parent){
		textMesh.clear();
		if(_parent->getShowName()){
			textMesh.append(getTextMesh(_parent->getName(), textPadding, getHeight()/ 2 + 4));
		}
		if(_parent->getMinimized()){
			textMesh.append(getTextMesh("+", getWidth() - textPadding - 8, getHeight() / 2 + 4));
		}else{
			textMesh.append(getTextMesh("-", getWidth()- textPadding - 8, getHeight() / 2 + 4));
		}
	}
}

ofxGuiGroupHeader::~ofxGuiGroupHeader(){
}

void ofxGuiGroupHeader::render() {
	ofxBaseGui::render();

	ofSetColor(textColor);

	ofxGuiGroup* _parent = dynamic_cast<ofxGuiGroup*>(parent());
	if(_parent){
		if(_parent->getShowName()){
			ofSetColor(textColor);
			bindFontTexture();
			textMesh.draw();
			unbindFontTexture();
		}
	}

}

bool ofxGuiGroupHeader::mousePressed(ofMouseEventArgs & args){

	if(!isHidden()){
		ofRectangle minButton(getWidth() - textPadding * 3, 0, textPadding * 3, getHeight());
		minButton.setPosition(localToScreen(minButton.getPosition()));
		if(minButton.inside(args.x, args.y)){
			ofxGuiGroup* _parent = dynamic_cast<ofxGuiGroup*>(parent());
			if(_parent){
				_parent->toggleMinimize();
				this->setNeedsRedraw();
				return true;
			}
		}
	}

	return ofxBaseGui::mousePressed(args);

}

ofxGuiGroup::ofxGuiGroup()
	:ofxBaseGui(){

	setup();

}

ofxGuiGroup::ofxGuiGroup(const std::string& collectionName)
	:ofxBaseGui(){

	setup();
	parameters.setName(collectionName);

}

ofxGuiGroup::ofxGuiGroup(const std::string& collectionName, const ofJson & config)
	:ofxGuiGroup(collectionName){

	_setConfig(config);

}

ofxGuiGroup::ofxGuiGroup(const ofParameterGroup & _parameters, const ofJson & config)
	:ofxGuiGroup(_parameters.getName()){

	addParametersFrom(_parameters);
	_setConfig(config);

}

ofxGuiGroup::ofxGuiGroup(const std::string& collectionName, const std::string& _filename, float x, float y)
	:ofxGuiGroup(collectionName){

	filename = _filename;
	setPosition(x,y);

}

ofxGuiGroup::ofxGuiGroup(const ofParameterGroup & _parameters, const std::string& _filename, float x, float y)
	:ofxGuiGroup(_parameters.getName()){

	filename = _filename;
	setPosition(x,y);
	addParametersFrom(_parameters);

}


ofxGuiGroup::~ofxGuiGroup(){

	showHeader.removeListener(this, &ofxGuiGroup::onHeaderVisibility);
	headerHeight.removeListener(this, &ofxGuiGroup::onHeaderHeight);
	ofRemoveListener(resize, this, &ofxGuiGroup::onResize);

}

void ofxGuiGroup::setup(){

	header = nullptr;

	filename.set("filename","settings.xml");

	exclusiveToggles.set("exclusive toggles", false);
	minimized.set("minimized", false);

	createLayout<FloatingBoxLayout>(this, Orientation::VERTICAL);
	headerHeight.set("header-height", defaultHeight);
	showHeader.set("show-header", true);

	showHeader.addListener(this, &ofxGuiGroup::onHeaderVisibility);
	headerHeight.addListener(this, &ofxGuiGroup::onHeaderHeight);
	ofAddListener(resize, this, &ofxGuiGroup::onResize);

	clear();

	registerMouseEvents();

}

void ofxGuiGroup::_setConfig(const ofJson &config){

	ofxBaseGui::_setConfig(config);

	JsonConfigParser::parse(config, headerHeight);
	JsonConfigParser::parse(config, showHeader);

}

void ofxGuiGroup::addParametersFrom(const ofParameterGroup & parameters){
	for(auto & p: parameters){
		if(p->isReadOnly()){
			ofLogWarning("ofxGui") << "Trying to add " << p->getName() << ": read only parameters not supported yet in ofxGui";
			continue;
		}
		string type = p->type();
		// TODO is this neccessary?
		if(type == typeid(ofParameter <int32_t> ).name()){
			add(p->cast<int>());
		}else if(type == typeid(ofParameter <uint32_t> ).name()){
			add(p->cast<uint32_t>());
		}else if(type == typeid(ofParameter <int64_t> ).name()){
			add(p->cast<int64_t>());
		}else if(type == typeid(ofParameter <uint64_t> ).name()){
			add(p->cast<uint64_t>());
		}else if(type == typeid(ofParameter <int8_t> ).name()){
			add(p->cast<int8_t>());
		}else if(type == typeid(ofParameter <uint8_t> ).name()){
			add(p->cast<uint8_t>());
		}else if(type == typeid(ofParameter <int16_t> ).name()){
			add(p->cast<int16_t>());
		}else if(type == typeid(ofParameter <uint16_t> ).name()){
			add(p->cast<uint16_t>());
		}else if(type == typeid(ofParameter<float>).name()){
			add(p->cast<float>());
		}else if(type == typeid(ofParameter <double> ).name()){
			add(p->cast<double>());
		}else if(type == typeid(ofParameter<void>).name()){
			add(p->cast<void>());
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
			add<ofxGuiGroup>(p->castGroup());
		}else{
			ofLogWarning("ofxGui") << "Trying to add " << p->getName() << ": ofxBaseGroup; no control for parameter of type " << type;

		}
	}
}

ofxButton* ofxGuiGroup::add(ofParameter <void> & parameter, const ofJson & config){
	return add<ofxButton>(parameter, config);
}

ofxToggle* ofxGuiGroup::add(ofParameter <bool> & parameter, const ofJson & config){
	return add<ofxToggle>(parameter, config);
}

ofxLabel *ofxGuiGroup::add(ofParameter <std::string> & parameter, const ofJson & config){
	return add<ofxLabel>(parameter, config);
}

ofxVec2Slider* ofxGuiGroup::add(ofParameter <ofVec2f> & parameter, const ofJson & config){
	return add<ofxVec2Slider>(parameter, config);
}

ofxVec3Slider* ofxGuiGroup::add(ofParameter <ofVec3f> & parameter, const ofJson & config){
	return add<ofxVec3Slider>(parameter, config);
}

ofxVec4Slider* ofxGuiGroup::add(ofParameter <ofVec4f> & parameter, const ofJson & config){
	return add<ofxVec4Slider>(parameter, config);
}

ofxColorSlider* ofxGuiGroup::add(ofParameter <ofColor> & parameter, const ofJson & config){
	return add<ofxColorSlider>(parameter, config);
}

ofxShortColorSlider* ofxGuiGroup::add(ofParameter <ofShortColor> & parameter, const ofJson & config){
	return add<ofxShortColorSlider>(parameter, config);
}

ofxFloatColorSlider* ofxGuiGroup::add(ofParameter <ofFloatColor> & parameter, const ofJson & config){
	return add<ofxFloatColorSlider>(parameter, config);
}

ofxBaseGui* ofxGuiGroup::addSpacer(float width, float height){
	ofxBaseGui* e = add<ofxBaseGui>();
	e->setSize(width, height);
	e->setBorderWidth(0);
	e->setBackgroundColor(ofColor(0,0,0,0));
	return e;
}

ofxBaseGui *ofxGuiGroup::addSpacer(const ofJson& config){
	ofxBaseGui* e = add<ofxBaseGui>();
	e->setBorderWidth(0);
	e->setBackgroundColor(ofColor(0,0,0,0));
	e->setConfig(config);
	return e;
}

ofxFpsPlotter* ofxGuiGroup::addFpsPlotter(const ofJson &config){
	return add<ofxFpsPlotter>(config);
}

ofxGuiGroup* ofxGuiGroup::addGroup(const string &name, const ofJson &config){
	return add<ofxGuiGroup>(name, config);
}

ofxGuiGroup* ofxGuiGroup::addGroup(const ofParameterGroup & parameters, const ofJson &config){
	return add<ofxGuiGroup>(parameters, config);
}

ofxPanel* ofxGuiGroup::addPanel(const string &name, const ofJson &config){
	return add<ofxPanel>(name, config);
}

ofxPanel* ofxGuiGroup::addPanel(const ofParameterGroup & parameters, const ofJson &config){
	return add<ofxPanel>(parameters, config);
}

ofxGuiTabs* ofxGuiGroup::addTabs(const string &name, const ofJson &config){
	return add<ofxGuiTabs>(name, config);
}

void ofxGuiGroup::clear(){
	Element::clear();
	header = nullptr;
	active_toggle_index = -1;
	header = add<ofxGuiGroupHeader>();
	header->setPercentalWidth(true, 1);
	header->setSize(getWidth(), headerHeight);
	header->setBackgroundColor(headerBackgroundColor);
	header->setBorderWidth(0);
}

//vector <string> ofxGuiGroup::getControlNames() const{
//	vector <string> names;
//	// TODO
////	for(auto & e: collection){
////		names.push_back(e->getName());
////	}
//	return names;
//}

ofxIntSlider* ofxGuiGroup::getIntSlider(const std::string& name){
	return getControlType <ofxIntSlider>(name);
}

ofxFloatSlider* ofxGuiGroup::getFloatSlider(const std::string& name){
	return getControlType <ofxFloatSlider>(name);
}

ofxToggle* ofxGuiGroup::getToggle(const std::string& name){
	return getControlType <ofxToggle>(name);
}

ofxButton* ofxGuiGroup::getButton(const std::string& name){
	return getControlType <ofxButton>(name);
}

ofxGuiGroup* ofxGuiGroup::getGroup(const std::string& name){
	return getControlType <ofxGuiGroup>(name);
}

ofxBaseGui* ofxGuiGroup::getControl(const std::string& name){
	for(auto & e: children()){
		ofxBaseGui *f = dynamic_cast<ofxBaseGui*>(e);
		if(f){
			if(f->getName() == name){
				return f;
			}
		}
	}
	return nullptr;
}

int ofxGuiGroup::getControlIndex(const std::string& name){

	for(int i = 0; i < (int)children().size(); i++){
		Element* e = children().at(i);
		ofxBaseGui *f = dynamic_cast<ofxBaseGui*>(e);
		if(f){
			if(f->getName() == name){
				return i;
			}
		}
	}
	return -1;
}

void ofxGuiGroup::minimize(){
	minimized = true;

	for(auto& child : children()){
		if(child != header){
			child->setHidden(true);
		}
	}

	invalidateChildShape();
	setNeedsRedraw();
}

void ofxGuiGroup::maximize(){
	minimized = false;

	for(auto& child : children()){
		if(child != header){
			child->setHidden(false);
		}
	}

	invalidateChildShape();
	setNeedsRedraw();
}

void ofxGuiGroup::minimizeAll(){
	for(auto & e: children()){
		if(e != header){
			ofxGuiGroup * group = dynamic_cast <ofxGuiGroup *>(e);
			if(group){
				group->minimize();
			}
		}
	}
}

void ofxGuiGroup::maximizeAll(){
	for(auto & e: children()){
		if(e != header){
			ofxGuiGroup * group = dynamic_cast <ofxGuiGroup *>(e);
			if(group){
				group->maximize();
			}
		}
	}
}

bool ofxGuiGroup::getMinimized(){
	return minimized;
}

void ofxGuiGroup::toggleMinimize(){
	if(minimized){
		maximize();
	}else {
		minimize();
	}
}

void ofxGuiGroup::setShowHeader(bool show) {
	if(show == false){
		if(minimized)
			maximize();
	}
	showHeader = show;
	invalidateChildShape();
 }

std::size_t ofxGuiGroup::getNumControls() {
	return children().size();
}

ofxBaseGui * ofxGuiGroup::getControl(std::size_t num){
	if(num < children().size()){
		return dynamic_cast <ofxBaseGui *>(children().at(num));
	}else{
		return nullptr;
	}
}

bool ofxGuiGroup::getTogglesExclusive() const {
	return exclusiveToggles;
}

void ofxGuiGroup::setExclusiveToggles(bool exclusive) {
	exclusiveToggles = exclusive;
	if(exclusiveToggles) {
		setOneToggleActive();
	}
}

bool ofxGuiGroup::setActiveToggle(ofxToggle* toggle) {
	if(!(*toggle)) {
		*toggle = true;
		deactivateAllOtherToggles(toggle);
		return true;
	}
	return false;
}

bool ofxGuiGroup::setActiveToggle(int index) {
	//+1 because of header element TODO there should be a better solution
	index ++;
	if(index >= 0 && index < (int)children().size()){
		if(ofxToggle* toggle = dynamic_cast<ofxToggle*>(children().at(index))) {
			return setActiveToggle(toggle);
		}
		else {
			ofLogError("ofxGuiGroup", "cannot activate control " + ofToString(index) + " because it's no ofxToggle.");
			return false;
		}
	}
	return false;
}

void ofxGuiGroup::deactivateAllOtherToggles(ofxToggle *toggle) {
	if(exclusiveToggles) {
		int active_index = -1;
		for(int i = 0; i < (int)children().size(); i++){
			if(ofxToggle* t = dynamic_cast<ofxToggle*>(children()[i])) {
				active_index++;
				if(t != toggle) {
				   *t = false;
				}
				else {
					active_toggle_index.set(active_index);
				}
			}
		}
	}
}

void ofxGuiGroup::setOneToggleActive() {
	if(active_toggle_index == -1){
		for(auto &e : children()){
			if(ofxToggle* t = dynamic_cast<ofxToggle*>(e)) {
				setActiveToggle(t);
				return;
			}
		}
	}
}


ofParameter<int>& ofxGuiGroup::getActiveToggleIndex() {
	return active_toggle_index;
}

ofAbstractParameter & ofxGuiGroup::getParameter(){
	parameters.clear();
	for(auto child : children()){
		ofxBaseGui* e = dynamic_cast<ofxBaseGui*>(child);
		if(e){
			parameters.add(e->getParameter());
		}
	}
	return parameters;
}

string ofxGuiGroup::getName(){
	return parameters.getName();
}

void ofxGuiGroup::setName(const std::string& _name){
	parameters.setName(_name);
}

ofxBaseGui* ofxGuiGroup::getHeader(){
	return header;
}

void ofxGuiGroup::onHeaderVisibility(bool &showing){
	if(header){
		header->setHidden(!showing);
	}
}

void ofxGuiGroup::onHeaderHeight(float &height){
	if(header){
		header->setHeight(height);
	}
}

void ofxGuiGroup::onResize(ResizeEventArgs & re){

}

void ofxGuiGroup::setHeaderBackgroundColor(const ofColor & color){
	ofxBaseGui::setHeaderBackgroundColor(color);
	if(header){
		header->setBackgroundColor(color);
	}
}
