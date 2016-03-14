#include "ofxGuiSpacer.h"
#include "ofGraphics.h"
using namespace std;

ofxGuiSpacer::ofxGuiSpacer(const ofJson & config) :
	ofxBaseGui(config){

	setup(config);

}

void ofxGuiSpacer::setup(const ofJson& config){
	// TODO how to set this as default BEFORE config processing?
	backgroundColor = ofColor(0, 0, 0, 0);
}

ofAbstractParameter & ofxGuiSpacer::getParameter(){
	return parameter;
}
