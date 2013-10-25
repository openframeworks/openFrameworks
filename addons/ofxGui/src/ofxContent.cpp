#include "ofxContent.h"
#include "ofGraphics.h"

ofBaseDraws* ofxGetCurrentContent(){
	return ofxContent::getCurrentContent();
}

ofBaseDraws* ofxContent::contentSelect = NULL;

ofxContent::ofxContent(ofParameter<string> _label, float width, float height){
	setup(_label,width,height);
}

ofxContent::~ofxContent(){
	//delete content;
    label.removeListener(this,&ofxContent::valueChanged);
}

ofxContent* ofxContent::setup(ofParameter<string> _label, float width, float height) {
    label.makeReferenceTo(_label);
	b.width  = defaultWidth;
	b.height = height * (defaultWidth/width) + defaultHeight;
    generateDraw();
    label.addListener(this,&ofxContent::valueChanged);
	ofRegisterMouseEvents(this,OF_EVENT_ORDER_BEFORE_APP);
    return this;
}

ofxContent* ofxContent::setup(string labelName, string _label, ofBaseDraws &_content, float width, float height) {
	content = &_content;
	contentSelect = &_content;
    label.set(labelName,_label);
    return setup(label,width,height);
}

void ofxContent::generateDraw(){
	bg.clear();
	bg.setFillColor(thisBackgroundColor);
	bg.setFilled(true);
	bg.rectangle(ofRectangle(b.x,b.y+b.height-defaultHeight,b.width,defaultHeight));
    string name;
    if(!getName().empty()){
    	name = getName();
    }
	textMesh = getTextMesh(name + (string)label, b.x + textPadding,b.y + defaultHeight/2 + 4 + b.height-defaultHeight);
}

void ofxContent::render() {
	ofColor c = ofGetStyle().color;
	content->draw(b.x , b.y,b.width,b.height - 18);
	bg.draw();
	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
    ofSetColor(textColor);
    bindFontTexture();
    textMesh.draw();
    unbindFontTexture();
    ofSetColor(c);
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}

ofAbstractParameter & ofxContent::getParameter(){
	return label;
}

void ofxContent::valueChanged(string & value){
	generateDraw();
}

bool ofxContent::mousePressed(ofMouseEventArgs & args){
	if (b.inside(args.x,args.y)){
		contentSelect = content;
	}
	return false;
}

ofBaseDraws* ofxContent::getCurrentContent(){
	return contentSelect;
}