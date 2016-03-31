#include "ofxGui.h"

ofxGui::ofxGui(){
	document = std::make_unique<ofx::DOM::Document>();
	document->createLayout<ofx::DOM::FloatingBoxLayout>(document.get(), ofx::DOM::Orientation::VERTICAL);
}

ofxGui::~ofxGui(){
}

Document* ofxGui::getRoot(){
	return document.get();
}

ofxGuiGroup* ofxGui::addGroup(const std::string& name, const ofJson& config){
	return getRoot()->add<ofxGuiGroup>(name, rootGroupConfig(config));
}

ofxGuiGroup* ofxGui::addGroup(const ofParameterGroup & parameters, const ofJson& config){
	return getRoot()->add<ofxGuiGroup>(parameters, rootGroupConfig(config));
}

ofxPanel* ofxGui::addPanel(const std::string& name, const ofJson& config){
	return getRoot()->add<ofxPanel>(name, rootGroupConfig(config));
}

ofxPanel* ofxGui::addPanel(const ofParameterGroup & parameters, const ofJson& config){
	return getRoot()->add<ofxPanel>(parameters, rootGroupConfig(config));
}

ofxGuiTabs* ofxGui::addTabs(const std::string& name, const ofJson& config){
	return getRoot()->add<ofxGuiTabs>(name, rootGroupConfig(config));
}

ofJson ofxGui::rootGroupConfig(const ofJson& config){
	ofJson res = config;
	if(config.find("width") == config.end()){
		res["width"] = 200;
	}
	if(config.find("position") == config.end()){
		res["position"] = "absolute";
	}
	return res;
}
