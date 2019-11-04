#include "ofxGuiGroup.h"
#include "ofxPanel.h"
#include "ofxSliderGroup.h"
#include "ofGraphics.h"
#include "ofxLabel.h"
#include "ofxInputField.h"

using namespace std;

ofxGuiGroup::ofxGuiGroup(){
	minimized = false;
	spacing = 1;
	spacingNextElement = 3;
//	header = defaultHeight;
	headerRect.height = defaultHeight; 
	bGuiActive = false;
}

ofxGuiGroup::ofxGuiGroup(const ofParameterGroup & parameters, const std::string& filename, float x, float y){
	minimized = false;
	parent = nullptr;
	setup(parameters, filename, x, y);
}

ofxGuiGroup * ofxGuiGroup::setup(const std::string& collectionName, const std::string& filename, float x, float y){
	ofParameterGroup parameters(collectionName);
	return setup(parameters, filename, x, y);
}

ofxGuiGroup * ofxGuiGroup::setup(const ofParameterGroup & _parameters, const std::string& _filename, float x, float y){
	b.x = x;
	b.y = y;
//	header = defaultHeight;
	headerRect.height = defaultHeight; 
	spacing = 1;
	spacingNextElement = 3;
	if(parent != nullptr){
		b.width = parent->getWidth();
	}else{
		b.width = defaultWidth;
	}
	clear();
	filename = _filename;
	bGuiActive = false;

	for(std::size_t i = 0; i < _parameters.size(); i++){
		string type = _parameters.getType(i);
		if(type == typeid(ofParameter <int32_t> ).name()){
			auto p = _parameters.getInt(i);
			add(p);
		}else if(type == typeid(ofParameter <uint32_t> ).name()){
			auto p = _parameters.get<uint32_t>(i);
			add(p);
		}else if(type == typeid(ofParameter <int64_t> ).name()){
			auto p = _parameters.get<int64_t>(i);
			add(p);
		}else if(type == typeid(ofParameter <uint64_t> ).name()){
			auto p = _parameters.get<uint64_t>(i);
			add(p);
		}else if(type == typeid(ofParameter <int8_t> ).name()){
			auto p = _parameters.get<int8_t>(i);
			add(p);
		}else if(type == typeid(ofParameter <uint8_t> ).name()){
			auto p = _parameters.get<uint8_t>(i);
			add(p);
		}else if(type == typeid(ofParameter <int16_t> ).name()){
			auto p = _parameters.get<int16_t>(i);
			add(p);
		}else if(type == typeid(ofParameter <uint16_t> ).name()){
			auto p = _parameters.get<uint16_t>(i);
			add(p);
		}else if(type == typeid(ofParameter <size_t> ).name()){
			auto p = _parameters.get<size_t>(i);
			add(p);
		}else if(type == typeid(ofParameter <float> ).name()){
			auto p = _parameters.getFloat(i);
			add(p);
		}else if(type == typeid(ofParameter <double> ).name()){
			auto p = _parameters.get<double>(i);
			add(p);
		}else if(type == typeid(ofParameter <bool> ).name()){
			auto p = _parameters.getBool(i);
			add(p);
		}else if(type == typeid(ofParameter <void> ).name()){
			auto p = _parameters.getVoid(i);
			add(p);
		}else if(type == typeid(ofParameter <ofVec2f> ).name()){
			auto p = _parameters.get<ofVec2f>(i);
			add(p);
		}else if(type == typeid(ofParameter <ofVec3f> ).name()){
			auto p = _parameters.get<ofVec3f>(i);
			add(p);
		}else if(type == typeid(ofParameter <ofVec4f> ).name()){
			auto p = _parameters.get<ofVec4f>(i);
			add(p);
		}else if(type == typeid(ofParameter <glm::vec2> ).name()){
			auto p = _parameters.get<glm::vec2>(i);
			add(p);
		}else if(type == typeid(ofParameter <glm::vec3> ).name()){
			auto p = _parameters.get<glm::vec3>(i);
			add(p);
		}else if(type == typeid(ofParameter <glm::vec4> ).name()){
			auto p = _parameters.get<glm::vec4>(i);
			add(p);
		}else if(type == typeid(ofParameter <ofColor> ).name()){
			auto p = _parameters.getColor(i);
			add(p);
		}else if(type == typeid(ofParameter <ofShortColor> ).name()){
			auto p = _parameters.getShortColor(i);
			add(p);
		}else if(type == typeid(ofParameter <ofFloatColor> ).name()){
			auto p = _parameters.getFloatColor(i);
			add(p);
		}else if(_parameters[i].valueType() == typeid(string).name()){
			if(_parameters[i].isReadOnly()){
				auto p = _parameters.get(i).castReadOnly<std::string, void>();
				add(p);
			}else{
				auto p = _parameters.getString(i);
				add(p);
			}
		}else if(type == typeid(ofParameterGroup).name()){
			auto p = _parameters.getGroup(i);
			ofxGuiGroup * panel = new ofxGuiGroup(p);
			add(panel);
		}else{
			ofLogWarning() << "ofxBaseGroup; no control for parameter of type " << type;
		}
	}

	parameters = _parameters;
	registerMouseEvents();

	setNeedsRedraw();

	return this;
}

void ofxGuiGroup::setWidthElements(float w){
	for(std::size_t i = 0; i < collection.size(); i++){
		collection[i]->setSize(w, collection[i]->getHeight());
		collection[i]->setPosition(b.x + b.width - w, collection[i]->getPosition().y);
		ofxGuiGroup * subgroup = dynamic_cast<ofxGuiGroup*>(collection[i]);
		if(subgroup != nullptr){
			subgroup->setWidthElements(w * .98);
		}
	}
	sizeChangedCB();
	setNeedsRedraw();
}

void ofxGuiGroup::add(ofxBaseGui * element){
	collection.push_back(element);

	element->setPosition(b.x, b.y + b.height  + spacing);

	b.height += element->getHeight() + spacing;

	//if(b.width<element->getWidth()) b.width = element->getWidth();

	element->unregisterMouseEvents();

	element->setParent(this);

	ofxGuiGroup * subgroup = dynamic_cast <ofxGuiGroup *>(element);
	if(subgroup != nullptr){
		subgroup->filename = filename;
		subgroup->setWidthElements(b.width * .98);
	}else{
		if(parent != nullptr){
			element->setSize(b.width * .98, element->getHeight());
			element->setPosition(b.x + b.width - element->getWidth(), element->getPosition().y);
		}
	}

	parameters.add(element->getParameter());
	setNeedsRedraw();
}

void ofxGuiGroup::add(const ofParameterGroup & parameters){
	ofxGuiGroup * panel = new ofxGuiGroup(parameters);
	add(panel);
}

void ofxGuiGroup::add(ofParameter <void> & parameter){
	add(new ofxButton(parameter, b.width));
}

void ofxGuiGroup::add(ofParameter <bool> & parameter){
	add(new ofxToggle(parameter, b.width));
}

void ofxGuiGroup::add(ofParameter <string> & parameter){
	add(new ofxInputField<std::string>(parameter, b.width));
}

void ofxGuiGroup::add(ofParameter <ofVec2f> & parameter){
	add(new ofxVecSlider_ <ofVec2f>(parameter, b.width));
}

void ofxGuiGroup::add(ofParameter <ofVec3f> & parameter){
	add(new ofxVecSlider_ <ofVec3f>(parameter, b.width));
}

void ofxGuiGroup::add(ofParameter <ofVec4f> & parameter){
	add(new ofxVecSlider_ <ofVec4f>(parameter, b.width));
}

void ofxGuiGroup::add(ofParameter <glm::vec2> & parameter){
	add(new ofxVecSlider_ <glm::vec2>(parameter, b.width));
}

void ofxGuiGroup::add(ofParameter <glm::vec3> & parameter){
	add(new ofxVecSlider_ <glm::vec3>(parameter, b.width));
}

void ofxGuiGroup::add(ofParameter <glm::vec4> & parameter){
	add(new ofxVecSlider_ <glm::vec4>(parameter, b.width));
}

void ofxGuiGroup::add(ofParameter <ofColor> & parameter){
	add(new ofxColorSlider_ <unsigned char>(parameter, b.width));
}

void ofxGuiGroup::add(ofParameter <ofShortColor> & parameter){
	add(new ofxColorSlider_ <unsigned short>(parameter, b.width));
}

void ofxGuiGroup::add(ofParameter <ofFloatColor> & parameter){
	add(new ofxColorSlider_ <float>(parameter, b.width));
}
void ofxGuiGroup::add(ofParameter <ofRectangle> & parameter){
	add(new ofxRectangleSlider(parameter, b.width));
}
void ofxGuiGroup::clear(){
	collection.clear();
	parameters.clear();
	sizeChangedCB();
}

bool ofxGuiGroup::mouseMoved(ofMouseEventArgs & args){
	if(!isGuiDrawing())return false;
	ofMouseEventArgs a = args;
	for(std::size_t i = 0; i < collection.size(); i++){
		if(collection[i]->mouseMoved(a)){
			return true;
		}
	}
	if(b.inside(args)){
		return true;
	}else{
		return false;
	}
}

bool ofxGuiGroup::mousePressed(ofMouseEventArgs & args){
	if(!isGuiDrawing())return false;
	
	if(setValue(args.x, args.y, true)){
		return true;
	}
	auto attended = false;
	ofMouseEventArgs a = args;
	for(std::size_t i = 0; i < collection.size(); i++){
		if(collection[i]->mousePressed(a)){
//			return true;
			attended = true;
		}
	}
	return attended || b.inside(args);
}

bool ofxGuiGroup::mouseDragged(ofMouseEventArgs & args){
	if(!isGuiDrawing())return false;
	if(bGuiActive){
		if(setValue(args.x, args.y, false)){
			return true;
		}
		ofMouseEventArgs a = args;
		for(std::size_t i = 0; i < collection.size(); i++){
			if(collection[i]->mouseDragged(a)){
				return true;
			}
		}
	}
	return false;
}

bool ofxGuiGroup::mouseReleased(ofMouseEventArgs & args){
	if(!isGuiDrawing()){
		bGuiActive = false;
		return false;
	}
	if(bGuiActive){
		bGuiActive = false;
		for(std::size_t k = 0; k < collection.size(); k++){
			ofMouseEventArgs a = args;
			if(collection[k]->mouseReleased(a)){
				return true;
			}
		}
		if(b.inside(ofPoint(args.x, args.y))){
			return true;
		}else{
			return false;
		}
	}
	return false;
}

bool ofxGuiGroup::mouseScrolled(ofMouseEventArgs & args){
	if(!isGuiDrawing())return false;
	ofMouseEventArgs a = args;
	for(std::size_t i = 0; i < collection.size(); i++){
		if(collection[i]->mouseScrolled(a)){
			return true;
		}
	}
	if(b.inside(args)){
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


	if(bHeaderEnabled){
		headerRect.set(b.x, b.y + 1 + spacingNextElement, b.width, defaultHeight);
		headerBg.clear();
		headerBg.setFillColor(thisHeaderBackgroundColor);
		headerBg.setFilled(true);
		headerBg.rectangle(headerRect);
		auto y = getTextVCenteredInRect(headerRect);
		
		textMesh = getTextMesh(getName(), textPadding + b.x, y);
		if(minimized){
			textMesh.append(getTextMesh("+", b.width - textPadding - 8 + b.x, y));
		}else{
			textMesh.append(getTextMesh("-", b.width - textPadding - 8 + b.x, y));
		}
	}
}

void ofxGuiGroup::render(){
	border.draw();
	if(bHeaderEnabled){
		headerBg.draw();
	}
	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode != OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
	ofColor c = ofGetStyle().color;
	if(bHeaderEnabled){
		ofSetColor(thisTextColor);
		
		bindFontTexture();
		textMesh.draw();
		unbindFontTexture();
	}
	if(!minimized){
		for(std::size_t i = 0; i < collection.size(); i++){
			collection[i]->draw();
		}
	}
	if(bHeaderEnabled){
		ofSetColor(c);
	}
	if(blendMode != OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}

vector <string> ofxGuiGroup::getControlNames() const{
	vector <string> names;
	for(std::size_t i = 0; i < collection.size(); i++){
		names.push_back(collection[i]->getName());
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
    for(std::size_t i = 0; i < collection.size(); i++){
		if(collection[i]->getName() == name){
			return collection[i];
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
			if(bHeaderEnabled){
//				ofRectangle minButton(b.x, b.y, b.width, header);
				if(headerRect.inside(mx, my)){
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
	}

	return false;
}

void ofxGuiGroup::minimize(){
	minimized = true;
	b.height = (bHeaderEnabled?headerRect.height:0) + spacing + spacingNextElement + 1 /*border*/;
	if(parent){
		parent->sizeChangedCB();
	}
	setNeedsRedraw();
	onMinimize();
}

void ofxGuiGroup::maximize(){
	minimized = false;
    for(std::size_t i = 0; i < collection.size(); i++){
		b.height += collection[i]->getHeight() + spacing;
	}
	if(parent){
		parent->sizeChangedCB();
	}
	setNeedsRedraw();
	onMaximize();
}

void ofxGuiGroup::minimizeAll(){
	for(std::size_t i = 0; i < collection.size(); i++){
		ofxGuiGroup * group = dynamic_cast <ofxGuiGroup *>(collection[i]);
		if(group){
			group->minimize();
		}
	}
}

void ofxGuiGroup::maximizeAll(){
	for(std::size_t i = 0; i < collection.size(); i++){
		ofxGuiGroup * group = dynamic_cast <ofxGuiGroup *>(collection[i]);
		if(group){
			group->maximize();
		}
	}
}

bool ofxGuiGroup::isMinimized() const{
	return minimized;
}

void ofxGuiGroup::onMinimize(){

}

void ofxGuiGroup::onMaximize(){

}

void ofxGuiGroup::sizeChangedCB(){
	float y = b.y  + (bHeaderEnabled?headerRect.height:0) + spacing;
	if(parent){
		y += spacingNextElement;
	}
	for(std::size_t i = 0; i < collection.size(); i++){
		collection[i]->setPosition(collection[i]->getPosition().x, y + spacing);
		y += collection[i]->getHeight() + spacing;
	}
	if(minimized){
		b.height = (bHeaderEnabled?headerRect.height:0) + spacing + spacingNextElement + 1 /*border*/;
	}else{
		b.height = y - b.y;
	}
	if(parent){
		parent->sizeChangedCB();
	}
	setNeedsRedraw();
}

std::size_t ofxGuiGroup::getNumControls() const {
	return collection.size();
}

ofxBaseGui * ofxGuiGroup::getControl(std::size_t num){
	if(num < collection.size()){
		return collection[num];
	}else{
		return nullptr;
	}
}

ofAbstractParameter & ofxGuiGroup::getParameter(){
	return parameters;
}

void ofxGuiGroup::setPosition(const glm::vec3& p){
	auto diff = p - b.getPosition();

	for(std::size_t i = 0; i < collection.size(); i++){
		collection[i]->setPosition(collection[i]->getPosition() + diff);
	}

	b.setPosition(p);
	setNeedsRedraw();
}

void ofxGuiGroup::setPosition(float x, float y){
	setPosition({x, y, 0});
}

void ofxGuiGroup::enableHeader(){
	bHeaderEnabled = true;
	spacingNextElement = 3;
	setNeedsRedraw();
}

void ofxGuiGroup::disableHeader(){
	bHeaderEnabled = false;
	spacingNextElement = 2;
	setNeedsRedraw();
}

bool ofxGuiGroup::isHeaderEnabled(){
	return bHeaderEnabled;
}
