#include "ofxBaseGui.h"
#include "ofImage.h"
#include "ofBitmapFont.h"
#ifndef TARGET_EMSCRIPTEN
#include "ofXml.h"
#endif

#include "JsonConfigParser.h"

using namespace std;


void ofxGuiSetFont(const string & fontPath, int fontsize, bool _bAntiAliased, bool _bFullCharacterSet, int dpi){
	ofxBaseGui::loadFont(fontPath, fontsize, _bAntiAliased, _bFullCharacterSet, dpi);
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

void ofxGuiSetDefaultMargin(float margin){
	ofxBaseGui::setDefaultMargin(margin);
}

ofColor
ofxBaseGui::defaultHeaderBackgroundColor(64),
ofxBaseGui::defaultBackgroundColor(0),
ofxBaseGui::defaultBorderColor(120, 100),
ofxBaseGui::defaultTextColor(255),
ofxBaseGui::defaultFillColor(128);

float ofxBaseGui::defaultBorderWidth = 0;
int ofxBaseGui::textPadding = 4;
int ofxBaseGui::defaultWidth = 200;
int ofxBaseGui::defaultHeight = 25;

float ofxBaseGui::defaultMarginLeft = 2;
float ofxBaseGui::defaultMarginRight = 2;
float ofxBaseGui::defaultMarginBottom = 2;
float ofxBaseGui::defaultMarginTop = 2;

ofTrueTypeFont ofxBaseGui::font;
bool ofxBaseGui::fontLoaded = false;
bool ofxBaseGui::useTTF = false;
ofBitmapFont ofxBaseGui::bitmapFont;

ofxBaseGui::ofxBaseGui()
	:Element("",0,0,defaultWidth, defaultHeight){

	setup();

}

ofxBaseGui::ofxBaseGui(const ofJson &config)
	:ofxBaseGui(){

	_setConfig(config);

}

void ofxBaseGui::setup(){

#ifndef TARGET_EMSCRIPTEN
	serializer = std::make_shared<ofXml>();
#endif

	bRegisteredForMouseEvents = false;

	headerBackgroundColor.set("header-background-color", defaultHeaderBackgroundColor);
	backgroundColor.set("background-color", defaultBackgroundColor);
	borderColor.set("border-color", defaultBorderColor);
	textColor.set("text-color", defaultTextColor);
	fillColor.set("fill-color", defaultFillColor);
	borderWidth.set("border-width", defaultBorderWidth);
	textAlignment.set("text-align", TextAlignment::LEFT);
	showName.set("show-name", true);

	// parameter won't be saved to file
	parameter.setSerializable(false);

	setFloat(LayoutFloat::NONE);
	setLayoutPosition(LayoutPosition::STATIC);

	setMargin(defaultMarginTop, defaultMarginRight, defaultMarginBottom, defaultMarginLeft);

	ofAddListener(resize, this, &ofxBaseGui::onResize);

	registerMouseEvents();

	setHeight(defaultHeight);
	setPercentalWidth(true, 1);

}

void ofxBaseGui::registerMouseEvents(int priority){
	if(bRegisteredForMouseEvents == true){
		return; // already registered.
	}
	bRegisteredForMouseEvents = true;
	ofRegisterMouseEvents(this, priority);
}

void ofxBaseGui::unregisterMouseEvents(int priority){
	if(bRegisteredForMouseEvents == false){
		return; // not registered.
	}
	ofUnregisterMouseEvents(this, priority);
	bRegisteredForMouseEvents = false;
}

void ofxBaseGui::setConfig(const ofJson &config, bool recursive){
	_setConfig(config);
	if(recursive){
		for(auto& e: children()){
			ofxBaseGui* _e = dynamic_cast<ofxBaseGui*>(e);
			if(_e){
				_e->setConfig(config, recursive);
			}
		}
	}
}

void ofxBaseGui::_setConfig(const ofJson &config){

	//parse colors
	JsonConfigParser::parse(config, backgroundColor);
	JsonConfigParser::parse(config, borderColor);
	JsonConfigParser::parse(config, textColor);
	JsonConfigParser::parse(config, fillColor);
	JsonConfigParser::parse(config, headerBackgroundColor);
	JsonConfigParser::parse(config, showName);

	//parse size
	JsonConfigParser::parse(config, this);

	//parse position type
	LayoutPosition _position = getAttribute<LayoutPosition>("position");
	JsonConfigParser::parse(config, "position", _position);
	if(_position != getLayoutPosition()){
		setLayoutPosition(_position);
		invalidateChildShape();
	}

	//parse floating
	LayoutFloat _floating = getAttribute<LayoutFloat>("float");
	JsonConfigParser::parse(config, "float", _floating);
	if(_floating != getFloat()){
		setFloat(_floating);
		invalidateChildShape();
	}

	//parse margin
	float _marginLeft = getAttribute<float>("margin-left");
	JsonConfigParser::parse(config, "margin-left", _marginLeft);
	if(_marginLeft != getMarginLeft()){
		setMarginLeft(_marginLeft);
		invalidateChildShape();
	}
	float _marginRight = getAttribute<float>("margin-right");
	JsonConfigParser::parse(config, "margin-right", _marginRight);
	if(_marginRight != getMarginRight()){
		setMarginRight(_marginRight);
		invalidateChildShape();
	}
	float _marginBottom = getAttribute<float>("margin-bottom");
	JsonConfigParser::parse(config, "margin-bottom", _marginBottom);
	if(_marginBottom != getMarginBottom()){
		setMarginBottom(_marginBottom);
		invalidateChildShape();
	}
	float _marginTop = getAttribute<float>("margin-top");
	JsonConfigParser::parse(config, "margin-top", _marginTop);
	if(_marginTop != getMarginTop()){
		setMarginTop(_marginTop);
		invalidateChildShape();
	}

	//parse text alignment
	if (config.find(textAlignment.getName()) != config.end()) {
		std::string val = config[textAlignment.getName()];
		setTextAlignment(val);
	}

	setNeedsRedraw();

}

ofAbstractParameter& ofxBaseGui::getParameter(){
	return parameter;
}

void ofxBaseGui::loadFont(const std::string& filename, int fontsize, bool _bAntiAliased, bool _bFullCharacterSet, int dpi){
	font.load(filename, fontsize, _bAntiAliased, _bFullCharacterSet, false, 0, dpi);
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
	ofRemoveListener(resize, this, &ofxBaseGui::onResize);
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

ofMesh ofxBaseGui::getTextMesh(const string & text, ofPoint p){
	return getTextMesh(text, p.x, p.y);
}

ofMesh ofxBaseGui::getTextMesh(const string & text, float x, float y){
	if(useTTF){
		return font.getStringMesh(text, x, y);
	}else{
		return bitmapFont.getMesh(text, x, y);
	}
}

ofRectangle ofxBaseGui::getTextBoundingBox(const string & text, float x, float y){
	if(useTTF){
		return font.getStringBoundingBox(text, x, y);
	}else{
		return bitmapFont.getBoundingBox(text, x, y);
	}
}

void ofxBaseGui::saveToFile(const std::string& filename){
	if(serializer){
		serializer->load(filename);
		saveTo(*serializer);
		serializer->save(filename);
	}else{
		ofLogError("ofxGui") << "element has no serializer to save to";
	}
}

void ofxBaseGui::loadFromFile(const std::string& filename){
	if(serializer){
		serializer->load(filename);
		loadFrom(*serializer);
	}else{
		ofLogError("ofxGui") << "element has no serializer to load from";
	}
}


void ofxBaseGui::saveTo(ofBaseSerializer & serializer){
	serializer.serialize(getParameter());
}

void ofxBaseGui::loadFrom(ofBaseSerializer & serializer){
	serializer.deserialize(getParameter());
}


void ofxBaseGui::setDefaultSerializer(std::shared_ptr <ofBaseFileSerializer> _serializer){
	serializer = _serializer;
}

string ofxBaseGui::getName(){
	return getParameter().getName();
}

void ofxBaseGui::setName(const std::string& _name){
	getParameter().setName(_name);
}

void ofxBaseGui::setTextAlignment(const TextAlignment &textLayout){
	this->textAlignment = textLayout;
}

void ofxBaseGui::setTextAlignment(const std::string &textLayout){
	if(textLayout == "left"){
		setTextAlignment(TextAlignment::LEFT);
	}
	else if(textLayout == "right"){
		setTextAlignment(TextAlignment::RIGHT);
	}
	else if(textLayout == "center"){
		setTextAlignment(TextAlignment::CENTERED);
	}
}

TextAlignment ofxBaseGui::getTextAlignment() const {
	return textAlignment;
}

ofColor ofxBaseGui::getHeaderBackgroundColor() const {
	return headerBackgroundColor;
}

ofColor ofxBaseGui::getBackgroundColor() const {
	return backgroundColor;
}

ofColor ofxBaseGui::getBorderColor() const {
	return borderColor;
}

ofColor ofxBaseGui::getTextColor() const {
	return textColor;
}

ofColor ofxBaseGui::getFillColor() const {
	return fillColor;
}

bool ofxBaseGui::getShowName() const {
	return showName;
}

float ofxBaseGui::getBorderWidth() const {
	return borderWidth;
}

void ofxBaseGui::setHeaderBackgroundColor(const ofColor & color){
	setNeedsRedraw();
	headerBackgroundColor = color;
}

void ofxBaseGui::setBackgroundColor(const ofColor & color){
	setNeedsRedraw();
	backgroundColor = color;
}

void ofxBaseGui::setBorderColor(const ofColor & color){
	setNeedsRedraw();
	borderColor = color;
}

void ofxBaseGui::setTextColor(const ofColor & color){
	setNeedsRedraw();
	textColor = color;
}

void ofxBaseGui::setFillColor(const ofColor & color){
	setNeedsRedraw();
	fillColor = color;
}

void ofxBaseGui::setBorderWidth(float width){
	setNeedsRedraw();
	borderWidth = width;
}

void ofxBaseGui::setMargin(float margin){
	setMargin(margin, margin, margin, margin);
}

void ofxBaseGui::setMargin(float margin_topbottom, float margin_leftright){
	setMargin(margin_topbottom, margin_leftright, margin_topbottom, margin_leftright);
}

void ofxBaseGui::setMargin(float margin_top, float margin_right, float margin_bottom, float margin_left){
	setAttribute("margin-top", margin_top);
	setAttribute("margin-right", margin_right);
	setAttribute("margin-bottom", margin_bottom);
	setAttribute("margin-left", margin_left);
	invalidateChildShape();
}

void ofxBaseGui::setMarginTop(float margin){
	setAttribute("margin-top", margin);
	invalidateChildShape();
}

void ofxBaseGui::setMarginBottom(float margin){
	setAttribute("margin-bottom", margin);
	invalidateChildShape();
}

void ofxBaseGui::setMarginLeft(float margin){
	setAttribute("margin-left", margin);
	invalidateChildShape();
}

void ofxBaseGui::setMarginRight(float margin){
	setAttribute("margin-right", margin);
	invalidateChildShape();
}

float ofxBaseGui::getMarginBottom(){
	if(hasAttribute("margin-bottom")){
		return getAttribute<float>("margin-bottom");
	}else {
		return 0;
	}
}

float ofxBaseGui::getMarginTop(){
	if(hasAttribute("margin-top")){
		return getAttribute<float>("margin-top");
	}else {
		return 0;
	}
}

float ofxBaseGui::getMarginLeft(){
	if(hasAttribute("margin-left")){
		return getAttribute<float>("margin-left");
	}else {
		return 0;
	}
}

float ofxBaseGui::getMarginRight(){
	if(hasAttribute("margin-right")){
		return getAttribute<float>("margin-right");
	}else {
		return 0;
	}
}

void ofxBaseGui::setDefaultHeaderBackgroundColor(const ofColor & color){
	defaultHeaderBackgroundColor = color;
}

void ofxBaseGui::setDefaultBackgroundColor(const ofColor & color){
	defaultBackgroundColor = color;
}

void ofxBaseGui::setDefaultBorderColor(const ofColor & color){
	defaultBorderColor = color;
}

void ofxBaseGui::setDefaultTextColor(const ofColor & color){
	defaultTextColor = color;
}

void ofxBaseGui::setDefaultFillColor(const ofColor & color){
	defaultFillColor = color;
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

void ofxBaseGui::setDefaultMargin(float margin){
	defaultMarginBottom = margin;
	defaultMarginLeft = margin;
	defaultMarginRight = margin;
	defaultMarginTop = margin;
}

void ofxBaseGui::setFloat(LayoutFloat type){
	setAttribute("float", type);
}

LayoutFloat ofxBaseGui::getFloat() {
	if(!hasAttribute("float")){
		setAttribute("float", LayoutFloat::NONE);
	}
	return getAttribute<LayoutFloat>("float");
}

void ofxBaseGui::setLayoutPosition(LayoutPosition type){
	setAttribute("position", type);
}

LayoutPosition ofxBaseGui::getLayoutPosition() {
	if(!hasAttribute("position")){
		setAttribute("position", LayoutPosition::STATIC);
	}
	return getAttribute<LayoutPosition>("position");
}

void ofxBaseGui::setShowName(bool show){
	showName = show;
	setNeedsRedraw();
}

void ofxBaseGui::generateDraw(){

	bg.clear();

	bg.setFillColor(backgroundColor);
	bg.setFilled(true);
	bg.setStrokeColor(borderColor);
	bg.setStrokeWidth(borderWidth);
	bg.rectangle(0,0,getWidth(),getHeight());

}

void ofxBaseGui::render(){

	bg.draw();

}

string ofxBaseGui::saveStencilToHex(const ofImage & img){
	stringstream strm;
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
			strm << "0x" << hex << (unsigned int)cur;
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

float ofxBaseGui::getTextWidth(const std::string & text, float _height){
	float _width = 0;
	ofVboMesh mesh = getTextMesh(text, 0, _height / 2 + 4);
	for(unsigned int i = 0; i < mesh.getVertices().size(); i++){
		if(mesh.getVertex(i).x > _width){
			_width = mesh.getVertex(i).x;
		}
	}
	_width += textPadding * 2;
	return _width;
}

bool ofxBaseGui::isMouseOver() const{
	return _isMouseOver;
}

void ofxBaseGui::setDraggable(bool draggable){
	_isDraggable = draggable;
}

bool ofxBaseGui::isDraggable() const{
	return _isDraggable;
}

bool ofxBaseGui::isDragging() const{
	return _isDragging;
}

bool ofxBaseGui::mouseDragged(ofMouseEventArgs & args){
	if(!isHidden()){

		if(localToScreen(ofRectangle(0,0,getWidth(),getHeight())).inside(args.x, args.y)){
			_isMouseOver = true;
		}else {
			_isMouseOver = false;
		}

		if(_isDragging){
			setPosition(args - grabPoint - getScreenPosition());
			return true;
		}

	}else {
		_isDragging = false;
		_isMouseOver= false;
	}
	return false;
}

bool ofxBaseGui::mousePressed(ofMouseEventArgs & args){
	if(!isHidden()){
		if(localToScreen(ofRectangle(0,0,getWidth(),getHeight())).inside(args.x, args.y)){
			if(_isDraggable){
				_isDragging = true;
				grabPoint = ofPoint(args.x, args.y) - getScreenPosition();
				return true;
			}
		}else {
			_isDragging = false;
		}
	}else {
		_isDragging = false;
		_isMouseOver= false;
	}
	return false;
}

bool ofxBaseGui::mouseMoved(ofMouseEventArgs & args){
	if(!isHidden()){
		if(localToScreen(ofRectangle(0,0,getWidth(),getHeight())).inside(args.x, args.y)){
			_isMouseOver = true;
		}else {
			_isMouseOver = false;
		}
	}else{
		_isDragging = false;
		_isMouseOver= false;
	}
	return false;
}

bool ofxBaseGui::mouseReleased(ofMouseEventArgs & args){
	if(!isHidden()){
		if(localToScreen(ofRectangle(0,0,getWidth(),getHeight())).inside(args.x, args.y)){
			_isMouseOver = true;
		}else {
			_isMouseOver = false;
		}
	}
	_isDragging = false;
	return false;
}

void ofxBaseGui::onResize(ResizeEventArgs &args){

}
