#include "ofxMinimalButton.h"
#include "ofGraphics.h"
using namespace std;

ofxMinimalButton::ofxMinimalButton(const string &buttonName, const ofJson &config)
	:ofxMinimalToggle(buttonName, config){

	setup(config);

}

ofxMinimalButton::ofxMinimalButton(const string & toggleName, float width, float height)
	:ofxMinimalButton(toggleName){

	if(width == 0){
		width = ofxBaseGui::getTextWidth(toggleName, height);
	}
	setSize(width, height);

}

ofxMinimalButton::~ofxMinimalButton(){

	unregisterPointerEvents();
	value.removeListener(this, &ofxMinimalButton::valueChanged);

}

void ofxMinimalButton::setup(const ofJson&){

	value = false;
	value.setSerializable(false);
	checkboxRect.set(1, 1, getShape().width - 2, getShape().height - 2);
	if(getWidth() == 0){
		setSize(ofxBaseGui::getTextWidth(value.getName(), getHeight()), getHeight());
	}

	value.addListener(this,&ofxMinimalButton::valueChanged);
	registerPointerEvents();

}

void ofxMinimalButton::pointerReleased(PointerUIEventArgs& e){
	setValue(e.screenPosition().x, e.screenPosition().y);
}

void ofxMinimalButton::valueChanged(bool & value){
	ofxMinimalToggle::valueChanged(value);
	if(!value){
		ofNotifyEvent(triggerEvent, this);
	}
}
