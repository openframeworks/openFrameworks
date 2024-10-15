#include "ofxButton.h"

ofxButton::ofxButton(){
	value.setSerializable(false);
}

ofxButton::ofxButton(ofParameter<void> _bVal, float width, float height){
	setup(_bVal, width, height);
}

ofxButton::~ofxButton(){
	//
}

ofxButton* ofxButton::setup(ofParameter<void> _bVal, float width, float height){
	parameter = _bVal;
	value = false;
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	bGuiActive = false;
	checkboxRect.set(1, 1, b.height - 2, b.height - 2);

	registerMouseEvents();

	value.addListener(this,&ofxButton::valueChanged);

	return this;
}

ofxButton* ofxButton::setup(const std::string& toggleName, float width, float height){
	setName(toggleName);
	value = false;
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	bGuiActive = false;
	checkboxRect.set(1, 1, b.height - 2, b.height - 2);

	registerMouseEvents();

	value.addListener(this,&ofxButton::valueChanged);

	return this;
}

void ofxButton::generateDraw(){
	bg.clear();
	bg.setFillColor(thisBackgroundColor);
	bg.rectangle(b);

	fg.clear();
	fg.setFilled(false);
	fg.setStrokeWidth(1);
	fg.setStrokeColor(thisFillColor);
	fg.rectRounded(b.getPosition()+checkboxRect.getTopLeft()+glm::vec2{1,1},checkboxRect.width-2,checkboxRect.height-2, 5);

	cross.clear();
	cross.setStrokeColor(thisTextColor);
	cross.setFillColor(thisFillColor);
	cross.setStrokeWidth(2);
	cross.setFilled(true);
	cross.rectRounded(b.getPosition()+checkboxRect.getTopLeft()+glm::vec2{1,1},checkboxRect.width-2,checkboxRect.height-2, 5);

	std::string name;
	auto textX = b.x + textPadding + checkboxRect.width;
	if(getTextBoundingBox(getName(), textX, 0).getMaxX() > b.getMaxX() - textPadding){
		for(auto c: ofUTF8Iterator(getName())){
			auto next = name;
			ofUTF8Append(next, c);
			if(getTextBoundingBox(next,textX,0).getMaxX() > b.getMaxX() - textPadding){
				break;
			}else{
				name = next;
			}
		}
	}else{
		name = getName();
	}

	textMesh = getTextMesh(name, textX, getTextVCenteredInRect(b));
}


bool ofxButton::mouseReleased(ofMouseEventArgs & args){
	bool attended = setValue(args.x, args.y, false);
	bGuiActive = false;
	if(attended){
		return true;
	}else{
		return false;
	}
}

bool ofxButton::mouseMoved(ofMouseEventArgs & args){
	return ofxToggle::mouseMoved(args);
}

bool ofxButton::mousePressed(ofMouseEventArgs & args){
	return ofxToggle::mousePressed(args);
}

bool ofxButton::mouseDragged(ofMouseEventArgs & args){
	return ofxToggle::mouseDragged(args);
}

ofAbstractParameter & ofxButton::getParameter(){
	return parameter;
}

void ofxButton::valueChanged(bool & v){
	if(!v){
		parameter.trigger();
	}
}
