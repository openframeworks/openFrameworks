// copyright (c) openFrameworks team 2010-2017
// copyright (c) Damian Stewart 2007-2009
#include "ofxOscBundle.h"

//--------------------------------------------------------------
ofxOscBundle::ofxOscBundle(const ofxOscBundle &other){ 
	copy(other);
}

//--------------------------------------------------------------
ofxOscBundle& ofxOscBundle::operator=(const ofxOscBundle &other){
	return copy(other);
}

//--------------------------------------------------------------
ofxOscBundle& ofxOscBundle::copy(const ofxOscBundle &other){
	if(this == &other) return *this;
	for(int i = 0; i < (int)other.bundles.size(); i++){
		bundles.push_back(other.bundles[i]);
	}
	for(int i = 0; i < (int)other.messages.size(); i++){
		messages.push_back(other.messages[i]);
	}
	return *this;
}

//--------------------------------------------------------------
void ofxOscBundle::clear(){
	bundles.clear();
	messages.clear();
}

//--------------------------------------------------------------
void ofxOscBundle::addBundle(const ofxOscBundle &bundle){
	bundles.push_back(bundle);
}

//--------------------------------------------------------------
void ofxOscBundle::addMessage(const ofxOscMessage &message){
	messages.push_back(message);
}

//--------------------------------------------------------------
int ofxOscBundle::getBundleCount() const{ 
	return bundles.size();
}

//--------------------------------------------------------------
int ofxOscBundle::getMessageCount() const{
	return messages.size();
}

//--------------------------------------------------------------
const ofxOscBundle& ofxOscBundle::getBundleAt(int i) const{
	return bundles[i];
}

//--------------------------------------------------------------
ofxOscBundle& ofxOscBundle::getBundleAt(int i){
	return bundles[i];
}

//--------------------------------------------------------------
const ofxOscMessage& ofxOscBundle::getMessageAt(int i) const{
	return messages[i];
}

//--------------------------------------------------------------
ofxOscMessage& ofxOscBundle::getMessageAt(int i){
	return messages[i];
}

// friend functions
//--------------------------------------------------------------
std::ostream& operator<<(std::ostream &os, const ofxOscBundle &bundle) {
	os << bundle.getMessageCount() << " message(s) "
	   << bundle.getBundleCount() << " bundle(s)";
	return os;
}
