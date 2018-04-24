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

	std::copy(other.bundles.begin(),
	          other.bundles.end(),
	          std::back_inserter(bundles));

	std::copy(other.messages.begin(),
	          other.messages.end(),
	          std::back_inserter(messages));

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
const ofxOscBundle& ofxOscBundle::getBundleAt(std::size_t i) const{
	return bundles[i];
}

//--------------------------------------------------------------
ofxOscBundle& ofxOscBundle::getBundleAt(std::size_t i){
	return bundles[i];
}

//--------------------------------------------------------------
const ofxOscMessage& ofxOscBundle::getMessageAt(std::size_t i) const{
	return messages[i];
}

//--------------------------------------------------------------
ofxOscMessage& ofxOscBundle::getMessageAt(std::size_t i){
	return messages[i];
}

// friend functions
//--------------------------------------------------------------
std::ostream& operator<<(std::ostream &os, const ofxOscBundle &bundle) {
	os << bundle.getMessageCount() << " message(s) "
	   << bundle.getBundleCount() << " bundle(s)";
	return os;
}
