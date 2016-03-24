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
	ofJson c = config;
	if(config.find("width") == config.end()){
		c["width"] = 200;
	}
	return getRoot()->add<ofxGuiGroup>(name, c);
}

ofxGuiGroup* ofxGui::addGroup(const ofParameterGroup & parameters, const ofJson& config){
	ofJson c = config;
	if(config.find("width") == config.end()){
		c["width"] = 200;
	}
	return getRoot()->add<ofxGuiGroup>(parameters, c);
}

ofxPanel* ofxGui::addPanel(const std::string& name, const ofJson& config){
	ofJson c = config;
	if(config.find("width") == config.end()){
		c["width"] = 200;
	}
	return getRoot()->add<ofxPanel>(name, c);
}

ofxPanel* ofxGui::addPanel(const ofParameterGroup & parameters, const ofJson& config){
	ofJson c = config;
	if(config.find("width") == config.end()){
		c["width"] = 200;
	}
	return getRoot()->add<ofxPanel>(parameters, c);
}

ofxGuiTabs* ofxGui::addTabs(const std::string& name, const ofJson& config){
	ofJson c = config;
	if(config.find("width") == config.end()){
		c["width"] = 200;
	}
	return getRoot()->add<ofxGuiTabs>(name, c);
}
