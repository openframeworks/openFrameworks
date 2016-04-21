#include "ofxMinimalButton.h"
#include "ofGraphics.h"
using namespace std;

ofxMinimalButton::ofxMinimalButton(){}

ofxMinimalButton::ofxMinimalButton(const Config & config) :
	ofxMinimalToggle(value, config){
	value.setName(config.name);
	value.set(false);
	value.setSerializable(false);
	registerMouseEvents();
	value.addListener(this, &ofxMinimalButton::valueChanged);
}
ofxMinimalButton::~ofxMinimalButton(){
	value.removeListener(this, &ofxMinimalButton::valueChanged);
}

ofxMinimalButton & ofxMinimalButton::setup(const std::string& buttonName, const Config & config){
	setName(buttonName);
	ofxBaseGui::setup(config);
	if(b.width == 0){
		b.width = ofxBaseGui::getTextWidth(buttonName, b.height);
	}
	bGuiActive = false;
	value = false;
	checkboxRect.set(1, 1, b.width - 2, b.height - 2);
	registerMouseEvents();
	value.addListener(this,&ofxMinimalButton::valueChanged);

	return *this;
}

ofxMinimalButton & ofxMinimalButton::setup(const string & toggleName, float width, float height){
	if(width == 0){
		width = ofxBaseGui::getTextWidth(toggleName, height);
	}
	setName(toggleName);
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	bGuiActive = false;
	value = false;
	checkboxRect.set(1, 1, b.width - 2, b.height - 2);

	registerMouseEvents();

	value.addListener(this, &ofxMinimalButton::valueChanged);

	return *this;
}

bool ofxMinimalButton::mouseReleased(ofMouseEventArgs & args){
	bool attended = setValue(args.x, args.y, false);
	bGuiActive = false;
	if(attended){
		return true;
	}else{
		return false;
	}
}

void ofxMinimalButton::valueChanged(bool & value){
	ofxMinimalToggle::valueChanged(value);
	if(!value){
		ofNotifyEvent(triggerEvent, this);
	}
}
