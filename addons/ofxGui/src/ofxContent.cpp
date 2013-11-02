#include "ofxContent.h"
#include "ofGraphics.h"

ofBaseDraws* ofxContent::contentSelect = NULL;

ofBaseDraws* ofxGetCurrentContent(){
	return ofxContent::getCurrentContent();
}

ofxContent::ofxContent(string contentName, ofBaseDraws &_content, float width, float height){
	setup(contentName,_content,width,height);
}

ofxContent::~ofxContent(){
    value.removeListener(this,&ofxContent::valueChanged);
}

ofxContent* ofxContent::setup(string contentName, ofBaseDraws &_content, float width, float height) {
	b.width  = defaultWidth;
	b.height = height * (defaultWidth/width) + defaultHeight;
	name = contentName;
	content = &_content;
	//contentSelect = &_content;
	ofRegisterMouseEvents(this,OF_EVENT_ORDER_BEFORE_APP);
	value.set("select",false);
	value.addListener(this,&ofxContent::valueChanged);
	generateDraw();
    return this;
}

void ofxContent::generateDraw(){
	bg.clear();
	bg.setFillColor(thisBackgroundColor);
	bg.setFilled(true);
	bg.rectangle(ofRectangle(b.x,b.y+b.height-defaultHeight,b.width,defaultHeight));

	fg.clear();
	if(value){
		fg.setFilled(true);
		fg.setFillColor(thisFillColor);
	}else{
		fg.setFilled(false);
		fg.setStrokeWidth(1);
		fg.setStrokeColor(thisFillColor);
	}
	fg.rectangle(ofRectangle(b.x,b.y+b.height-defaultHeight,b.width,defaultHeight));

	textMesh = getTextMesh(name, b.x + textPadding,b.y + defaultHeight/2 + 4 + b.height-defaultHeight);
}

void ofxContent::render() {
	ofColor c = ofGetStyle().color;
	content->draw(b.x , b.y,b.width,b.height - 18);
	bg.draw();
	fg.draw();
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
	label.set("Content",name);
	return label;
}

void ofxContent::valueChanged(bool & value){
	generateDraw();
}

bool ofxContent::mousePressed(ofMouseEventArgs & args){
	value = false;

	if (contentSelect == content){
		value = true;
	}
	if (b.inside(args.x,args.y)){
		if (args.button == 0){
			contentSelect = content;
			value = true;
		}
	    if (args.button == 2){
			if (contentSelect == content){
				contentSelect = NULL;
			}
		}
	}
	return false;
}

ofBaseDraws* ofxContent::getCurrentContent(){
	return contentSelect;
}