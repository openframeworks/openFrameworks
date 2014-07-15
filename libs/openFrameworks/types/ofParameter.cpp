#include "ofParameter.h"
#include "ofParameterGroup.h"

ofAbstractParameter::ofAbstractParameter(){
	parent = NULL;
}

ofAbstractParameter::~ofAbstractParameter(){

}

string ofAbstractParameter::getName() const {
	return "";
}

void ofAbstractParameter::setName(string name) {

}

string ofAbstractParameter::getEscapedName() const{
	return escape(getName());
}


string ofAbstractParameter::escape(string str) const{
	ofStringReplace(str, " ", "_");
	ofStringReplace(str, "<", "_");
	ofStringReplace(str, ">", "_");
	ofStringReplace(str, "{", "_");
	ofStringReplace(str, "}", "_");
	ofStringReplace(str, "[", "_");
	ofStringReplace(str, "]", "_");
	ofStringReplace(str, ",", "_");
	ofStringReplace(str, "(", "_");
	ofStringReplace(str, ")", "_");
	ofStringReplace(str, "/", "_");
	ofStringReplace(str, "\\", "_");
	return str;
}

string ofAbstractParameter::toString() const {
	return "";
}

void ofAbstractParameter::fromString(string str) {

}

string ofAbstractParameter::type() const{
	return typeid(*this).name();
}

void ofAbstractParameter::setParent(ofParameterGroup * _parent){
	parent = _parent;
}

const ofParameterGroup * ofAbstractParameter::getParent() const{
	return parent;
}

ofParameterGroup * ofAbstractParameter::getParent(){
	return parent;
}

vector<string> ofAbstractParameter::getGroupHierarchyNames() const{
	vector<string> hierarchy;
	if(getParent()){
		hierarchy = getParent()->getGroupHierarchyNames();
	}
	hierarchy.push_back(getEscapedName());
	return hierarchy;
}


void ofAbstractParameter::notifyParent(){
	if(getParent()) getParent()->notifyParameterChanged(*this);
}

void ofAbstractParameter::setSerializable(bool serializable){

}

bool ofAbstractParameter::isSerializable() const{
	return true;
}

shared_ptr<ofAbstractParameter> ofAbstractParameter::newReference() const{
	return shared_ptr<ofAbstractParameter>(new ofAbstractParameter(*this));
}

ostream& operator<<(ostream& os, const ofAbstractParameter& p){
	os << p.toString();
	return os;
}

istream& operator>>(istream& is, ofAbstractParameter& p){
	string str;
	is >> str;
	p.fromString(str);
	return is;
}
