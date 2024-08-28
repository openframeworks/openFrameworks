#include "ofxBaseGui.h"
#include "ofImage.h"
#include "ofBitmapFont.h"
#include "ofXml.h"
#include "ofJson.h"

using std::string;

void ofxGuiSetFont(const string & fontPath, int fontsize, bool _bAntiAliased, bool _bFullCharacterSet, int dpi){
	ofxBaseGui::loadFont(fontPath, fontsize, _bAntiAliased, _bFullCharacterSet, dpi);
}

void ofxGuiSetFont(const ofTrueTypeFontSettings & fontSettings){
	ofxBaseGui::loadFont(fontSettings);
}

void ofxGuiSetBitmapFont(){
	ofxBaseGui::setUseTTF(false);
}

void ofxGuiSetHeaderColor(const ofColor & color){
	ofxBaseGui::setDefaultHeaderBackgroundColor(color);
}

void ofxGuiSetBackgroundColor(const ofColor & color){
	ofxBaseGui::setDefaultBackgroundColor(color);
}

void ofxGuiSetBorderColor(const ofColor & color){
	ofxBaseGui::setDefaultBorderColor(color);
}

void ofxGuiSetTextColor(const ofColor & color){
	ofxBaseGui::setDefaultTextColor(color);
}

void ofxGuiSetFillColor(const ofColor & color){
	ofxBaseGui::setDefaultFillColor(color);
}

void ofxGuiSetTextPadding(int padding){
	ofxBaseGui::setDefaultTextPadding(padding);
}

void ofxGuiSetDefaultWidth(int width){
	ofxBaseGui::setDefaultWidth(width);

}

void ofxGuiSetDefaultHeight(int height){
	ofxBaseGui::setDefaultHeight(height);
}

void ofxGuiSetDefaultEventsPriority(ofEventOrder eventsPriority){
	ofxBaseGui::setDefaultEventsPriority(eventsPriority);
}
void ofxGuiEnableHiResDisplay(){
	ofxBaseGui::enableHiDpi();
}
void ofxGuiDisableHiResDisplay(){
	ofxBaseGui::disableHiDpi();
}


ofColor
ofxBaseGui::headerBackgroundColor(80),
ofxBaseGui::backgroundColor(0),
ofxBaseGui::borderColor(120, 100),
ofxBaseGui::textColor(255),
ofxBaseGui::fillColor(128);

int ofxBaseGui::textPadding = 4;
int ofxBaseGui::defaultWidth = 200;
int ofxBaseGui::defaultHeight = 18;

ofTrueTypeFont ofxBaseGui::font;
bool ofxBaseGui::fontLoaded = false;
bool ofxBaseGui::useTTF = false;
ofBitmapFont ofxBaseGui::bitmapFont;
ofEventOrder ofxBaseGui::defaultEventsPriority = OF_EVENT_ORDER_BEFORE_APP;

float ofxBaseGui::hiDpiScale =1;

ofxBaseGui::ofxBaseGui(){
	parent = nullptr;
	currentFrame = ofGetFrameNum();
	thisHeaderBackgroundColor = headerBackgroundColor;
	thisBackgroundColor = backgroundColor;
	thisBorderColor = borderColor;
	thisTextColor = textColor;
	thisFillColor = fillColor;

	bRegisteredForMouseEvents = false;
	needsRedraw = true;

	/*if(!fontLoaded){
	    loadFont(OF_TTF_MONO,10,true,true);
	    useTTF=false;
	}*/

}

void ofxBaseGui::loadFont(const std::string& filename, int fontsize, bool _bAntiAliased, bool _bFullCharacterSet, int dpi){
	font.load(filename, fontsize, _bAntiAliased, _bFullCharacterSet, false, 0, dpi);
	fontLoaded = true;
	useTTF = true;
}

void ofxBaseGui::loadFont(const ofTrueTypeFontSettings & fontSettings){
	font.load(fontSettings);
	fontLoaded = true;
	useTTF = true;
}

void ofxBaseGui::setUseTTF(bool bUseTTF){
	if(bUseTTF && !fontLoaded){
		loadFont(OF_TTF_MONO, 10, true, true);
	}
	useTTF = bUseTTF;
}

ofxBaseGui::~ofxBaseGui(){
	unregisterMouseEvents();
}

void ofxBaseGui::registerMouseEvents(){
	if(bRegisteredForMouseEvents == true){
		return; // already registered.
	}
	bRegisteredForMouseEvents = true;
	ofRegisterMouseEvents(this, defaultEventsPriority);
}

void ofxBaseGui::unregisterMouseEvents(){
	if(bRegisteredForMouseEvents == false){
		return; // not registered.
	}
	ofUnregisterMouseEvents(this, defaultEventsPriority);
	bRegisteredForMouseEvents = false;
}
void ofxBaseGui::setEvents(ofCoreEvents & _events){
	if(&_events != events){
		bool wasMouseInputEnabled = bRegisteredForMouseEvents;// || !events;
		unregisterMouseEvents();
		events = &_events;
		if (wasMouseInputEnabled) {
			// note: this will set bMouseInputEnabled to true as a side-effect.
			registerMouseEvents();
		}
	}
}
void ofxBaseGui::draw(){
	setEvents(ofEvents());
	if(needsRedraw){
		generateDraw();
		needsRedraw = false;
	}
	currentFrame = ofGetFrameNum();
	render();
}

bool ofxBaseGui::isGuiDrawing(){
	if(ofGetFrameNum() - currentFrame > 1){
		return false;
	}else{
		return true;
	}
}

void ofxBaseGui::bindFontTexture(){
	if(useTTF){
		font.getFontTexture().bind();
	}else{
		bitmapFont.getTexture().bind();
	}
}

void ofxBaseGui::unbindFontTexture(){
	if(useTTF){
		font.getFontTexture().unbind();
	}else{
		bitmapFont.getTexture().unbind();
	}
}


ofMesh ofxBaseGui::getTextMesh(const string & text, float x, float y){
	if(useTTF){
		return font.getStringMesh(text, x, y);
	}else{
		auto m = bitmapFont.getMesh(text, x, y);
		if(isHiDpiEnabled()){
			auto bb = getTextBoundingBox(text, x, y);
			bb.standardize();
			for(auto& v: m.getVertices()){
				v = (v - bb.getPosition())*hiDpiScale + bb.getPosition();
			}
		}
		return m;
	}
}

ofRectangle ofxBaseGui::getTextBoundingBox(const string & text, float x, float y){
	if(useTTF){
		return font.getStringBoundingBox(text, x, y);
	}else{
		auto r = bitmapFont.getBoundingBox(text, x, y);
		if(isHiDpiEnabled()){
			r.width *= hiDpiScale;
			r.height *= hiDpiScale;
		}
		return r;
	}
}
float ofxBaseGui::getTextVCenteredInRect(const ofRectangle& container){
		
	if(useTTF){
		return container.getCenter().y  + (font.getAscenderHeight()  + font.getDescenderHeight()) *0.5;
	}else{
		// The bitmap font does not provide a getAscenderHeight() method.
		// However,it can be found by calling `getTextBoundingBox(" ",0,0)` 
		// which returns the ascender value in the rectangle's Y as a negative value. 
		// It does not matter which string is passed to it, the value will be always the same.
		// Fix. It centers the text properly with `getTextBoundingBox(" ",0,0)` as it takes into account the screen pixel scaling.
		auto bb = getTextBoundingBox(" ",0,0);
		return container.getCenter().y - bb.height*0.5 - bb.y;
	}
}


void ofxBaseGui::saveToFile(const std::string& filename){
	auto extension = ofToLower(ofFilePath::getFileExt(filename));
	if(extension == "xml"){
		ofXml xml;
		if(ofFile(filename, ofFile::Reference).exists()){
			xml.load(filename);
		}
		saveTo(xml);
		xml.save(filename);
    }else
    if(extension == "json"){
        ofJson json = ofLoadJson(filename);
		saveTo(json);
        ofSavePrettyJson(filename, json);
	}else{
		ofLogError("ofxGui") << extension << " not recognized, only .xml and .json supported by now";
	}
}

void ofxBaseGui::loadFromFile(const std::string& filename){
	auto extension = ofToLower(ofFilePath::getFileExt(filename));
	if(extension == "xml"){
		ofXml xml;
		xml.load(filename);
		loadFrom(xml);
    }else
    if(extension == "json"){
		ofFile jsonFile(filename);
		ofJson json = ofLoadJson(jsonFile);
		loadFrom(json);
	}else{
		ofLogError("ofxGui") << extension << " not recognized, only .xml and .json supported by now";
	}
}

string ofxBaseGui::getName(){
	return getParameter().getName();
}

void ofxBaseGui::setName(const std::string& _name){
	getParameter().setName(_name);
	setNeedsRedraw();
}

void ofxBaseGui::setPosition(const glm::vec3 & p){
	setPosition(p.x, p.y);
}

void ofxBaseGui::setPosition(float x, float y){
	b.x = x;
	b.y = y;
	setNeedsRedraw();
}

void ofxBaseGui::setSize(float w, float h){
	b.width = w;
	b.height = h;
	sizeChangedCB();
}

void ofxBaseGui::setShape(ofRectangle r){
	b = r;
	sizeChangedCB();
}

void ofxBaseGui::setShape(float x, float y, float w, float h){
	b.set(x, y, w, h);
	sizeChangedCB();
}
void ofxBaseGui::setShapeNoNotification(const ofRectangle& r){
	b = r;
	setNeedsRedraw();
}
void ofxBaseGui::setShapeNoNotification(float x, float y, float w, float h){
	b.set(x, y, w, h);
	setNeedsRedraw();
}
glm::vec3 ofxBaseGui::getPosition() const {
	return glm::vec3(b.x, b.y, 0);
}

ofRectangle ofxBaseGui::getShape() const {
	return b;
}

float ofxBaseGui::getWidth() const {
	return b.width;
}

float ofxBaseGui::getHeight() const {
	return b.height;
}

ofColor ofxBaseGui::getHeaderBackgroundColor() const {
	return thisHeaderBackgroundColor;
}

ofColor ofxBaseGui::getBackgroundColor() const {
	return thisBackgroundColor;
}

ofColor ofxBaseGui::getBorderColor() const {
	return thisBorderColor;
}

ofColor ofxBaseGui::getTextColor() const {
	return thisTextColor;
}

ofColor ofxBaseGui::getFillColor() const {
	return thisFillColor;
}

void ofxBaseGui::setHeaderBackgroundColor(const ofColor & color){
	setNeedsRedraw();
	thisHeaderBackgroundColor = color;
}

void ofxBaseGui::setBackgroundColor(const ofColor & color){
	setNeedsRedraw();
	thisBackgroundColor = color;
}

void ofxBaseGui::setBorderColor(const ofColor & color){
	setNeedsRedraw();
	thisBorderColor = color;
}

void ofxBaseGui::setTextColor(const ofColor & color){
	setNeedsRedraw();
	thisTextColor = color;
}

void ofxBaseGui::setFillColor(const ofColor & color){
	setNeedsRedraw();
	thisFillColor = color;
}

void ofxBaseGui::setDefaultHeaderBackgroundColor(const ofColor & color){
	headerBackgroundColor = color;
}

void ofxBaseGui::setDefaultBackgroundColor(const ofColor & color){
	backgroundColor = color;
}

void ofxBaseGui::setDefaultBorderColor(const ofColor & color){
	borderColor = color;
}

void ofxBaseGui::setDefaultTextColor(const ofColor & color){
	textColor = color;
}

void ofxBaseGui::setDefaultFillColor(const ofColor & color){
	fillColor = color;
}

void ofxBaseGui::setDefaultTextPadding(int padding){
	textPadding = padding;
}

void ofxBaseGui::setDefaultWidth(int width){
	defaultWidth = width;
}

void ofxBaseGui::setDefaultHeight(int height){
	defaultHeight = height;
}

void ofxBaseGui::setDefaultEventsPriority(ofEventOrder eventsPriority){
	defaultEventsPriority = eventsPriority;
}

void ofxBaseGui::sizeChangedCB(){
	if(parent){
		parent->sizeChangedCB();
	}
	setNeedsRedraw();
}

void ofxBaseGui::setNeedsRedraw(){
	needsRedraw = true;
}

string ofxBaseGui::saveStencilToHex(const ofImage & img){
	std::stringstream strm;
	int width = img.getWidth();
	int height = img.getHeight();
	int n = width * height;
	unsigned char cur = 0;
	int shift = 0;
	strm << "{";
	for(int i = 0; i < n;){
		if(img.getPixels()[i * 4 + 3] > 0){
			cur |= 1 << shift;
		}
		i++;
		if(i % 8 == 0){
			strm << "0x" << std::hex << (unsigned int)cur;
			cur = 0;
			shift = 0;
			if(i < n){
				strm << ",";
			}
		}else{
			shift++;
		}
	}
	strm << "}";
	return strm.str();
}

void ofxBaseGui::loadStencilFromHex(ofImage & img, unsigned char * data){
	int width = img.getWidth();
	int height = img.getHeight();
	int i = 0;
	ofColor on(255, 255);
	ofColor off(255, 0);
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			int shift = i % 8;
			int offset = i / 8;
			bool cur = (data[offset] >> shift) & 1;
			img.setColor(x, y, cur ? on : off);
			i++;
		}
	}
	img.update();
}

void ofxBaseGui::setParent(ofxBaseGui * parent){
	this->parent = parent;
}

ofxBaseGui * ofxBaseGui::getParent(){
	return parent;
}
void ofxBaseGui::enableHiDpi(){
	
	ofxBaseGui::hiDpiScale = 2;
	ofxBaseGui::textPadding = 8;
	ofxBaseGui::defaultWidth = 400;
	ofxBaseGui::defaultHeight = 36;

}
void ofxBaseGui::disableHiDpi(){
	ofxBaseGui::hiDpiScale = 1;
	ofxBaseGui::textPadding = 4;
	ofxBaseGui::defaultWidth = 200;
	ofxBaseGui::defaultHeight = 18;
}
bool ofxBaseGui::isHiDpiEnabled(){
	return hiDpiScale == 2;
}
