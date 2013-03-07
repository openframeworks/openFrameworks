#include "ofxToggle.h"
#include "ofGraphics.h"

ofxToggle::ofxToggle(ofParameter<bool> _bVal, float width, float height){
	setup(_bVal,width,height);
}

ofxToggle::~ofxToggle(){
	value.removeListener(this,&ofxToggle::valueChanged);
}

ofxToggle * ofxToggle::setup(ofParameter<bool> _bVal, float width, float height){
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	currentFrame = 0;
	bGuiActive = false;
	value.makeReferenceTo(_bVal);
	checkboxRect.set(1, 1, b.height - 2, b.height - 2);

	value.addListener(this,&ofxToggle::valueChanged);
	ofRegisterMouseEvents(this);
	generateDraw();

	return this;

}

ofxToggle * ofxToggle::setup(string toggleName, bool _bVal, float width, float height){
	value.set(toggleName,_bVal);
	return setup(value,width,height);
}


void ofxToggle::mouseMoved(ofMouseEventArgs & args){
}

void ofxToggle::mousePressed(ofMouseEventArgs & args){
	setValue(args.x, args.y, true);
}

void ofxToggle::mouseDragged(ofMouseEventArgs & args){
}

void ofxToggle::mouseReleased(ofMouseEventArgs & args){
	bGuiActive = false;
}

void ofxToggle::generateDraw(){
	bg.clear();
	bg.setFillColor(thisBackgroundColor);
	bg.moveTo(b.x,b.y);
	bg.lineTo(b.x+b.width,b.y);
	bg.lineTo(b.x+b.width,b.y+b.height);
	bg.lineTo(b.x,b.y+b.height);
	bg.close();

	fg.clear();
	if(value){
		fg.setFilled(true);
		fg.setFillColor(thisFillColor);
	}else{
		fg.setFilled(false);
		fg.setStrokeWidth(1);
		fg.setStrokeColor(thisFillColor);
	}
	fg.moveTo(b.getPosition()+checkboxRect.getTopLeft());
	fg.lineTo(b.getPosition()+checkboxRect.getTopRight());
	fg.lineTo(b.getPosition()+checkboxRect.getBottomRight());
	fg.lineTo(b.getPosition()+checkboxRect.getBottomLeft());
	fg.close();

	cross.clear();
	cross.setStrokeColor(thisTextColor);
	cross.setStrokeWidth(1);
	cross.setFilled(false);
	cross.moveTo(b.getPosition()+checkboxRect.getTopLeft());
	cross.lineTo(b.getPosition()+checkboxRect.getBottomRight());
	cross.moveTo(b.getPosition()+checkboxRect.getTopRight());
	cross.lineTo(b.getPosition()+checkboxRect.getBottomLeft());

	textMesh = font.getStringMesh(getName(), b.x+textPadding + checkboxRect.width, b.y+b.height / 2 + 4);
}

void ofxToggle::draw(){
	currentFrame = ofGetFrameNum();

	bg.draw();
	fg.draw();

	if( value ){
		cross.draw();
	}

	ofColor c = ofGetStyle().color;
	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
	ofSetColor(thisTextColor);
	font.getFontTexture().bind();
	textMesh.draw();
	font.getFontTexture().unbind();
	ofSetColor(c);
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}

bool ofxToggle::operator=(bool v){
	value = v;
	return v;
}

ofxToggle::operator const bool & (){
	return value;
}

void ofxToggle::setValue(float mx, float my, bool bCheck){

	if( ofGetFrameNum() - currentFrame > 1 ){
		bGuiActive = false;
		return;
	}
	if( bCheck ){
		ofRectangle checkRect = checkboxRect;
		checkRect.x += b.x;
		checkRect.y += b.y;

		if( checkRect.inside(mx, my) ){
			bGuiActive = true;
		}else{
			bGuiActive = false;

		}
	}
	if( bGuiActive ){
		value = !value;
	}
}

ofAbstractParameter & ofxToggle::getParameter(){
	return value;
}

void ofxToggle::valueChanged(bool & value){
	generateDraw();
}
