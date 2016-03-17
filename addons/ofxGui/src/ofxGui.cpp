#include "ofxGui.h"

ofxGui::ofxGui(){
	document = std::make_unique<ofx::DOM::Document>();
}

ofxGui::~ofxGui(){
}

Document* ofxGui::getRoot(){
	return document.get();
}

ofxGuiGroup* ofxGui::addGroup(const std::string& name, const ofJson& config){
	return getRoot()->add<ofxGuiGroup>(name, config);
}

ofxGuiGroup* ofxGui::addGroup(const ofParameterGroup & parameters, const ofJson& config){
	return getRoot()->add<ofxGuiGroup>(parameters, config);
}

ofxPanel* ofxGui::addPanel(const std::string& name, const ofJson& config){
	return getRoot()->add<ofxPanel>(name, config);
}

ofxPanel* ofxGui::addPanel(const ofParameterGroup & parameters, const ofJson& config){
	return getRoot()->add<ofxPanel>(parameters, config);
}

ofxGuiTabs* ofxGui::addTabs(const std::string& name, const ofJson& config){
	return getRoot()->add<ofxGuiTabs>(name, config);
}
