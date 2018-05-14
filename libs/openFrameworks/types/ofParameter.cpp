#include "ofParameter.h"
#include "ofParameterGroup.h"
using namespace std;

string ofAbstractParameter::getEscapedName() const{
	return escape(getName());
}


string ofAbstractParameter::escape(const string& _str) const{

	std::string str(_str);

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
	ofStringReplace(str, ".", "_");
	ofStringReplace(str, "#", "_");

	return str;
}


string ofAbstractParameter::type() const{
	return typeid(*this).name();
}

vector<string> ofAbstractParameter::getGroupHierarchyNames() const{
	vector<string> hierarchy;
	auto p = getFirstParent();
	if(p){
		hierarchy = p.getGroupHierarchyNames();
	}
	hierarchy.push_back(getEscapedName());
	return hierarchy;
}

bool ofAbstractParameter::isReferenceTo(const ofAbstractParameter &other) const{
	return getInternalObject() == other.getInternalObject();
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

ofParameterGroup & ofAbstractParameter::castGroup(){
	return static_cast<ofParameterGroup &>(*this);
}

const ofParameterGroup & ofAbstractParameter::castGroup() const{
	return static_cast<const ofParameterGroup &>(*this);
}


ofParameter<void>::ofParameter()
:obj(new Value){

}

ofParameter<void>::ofParameter(const string& name)
:obj(new Value(name)){

}

void ofParameter<void>::setName(const string & name){
	obj->name = name;
}

string ofParameter<void>::getName() const{
	return obj->name;
}

std::string ofParameter<void>::toString() const{
	return "";
}

string ofParameter<void>::valueType() const{
	return typeid(void).name();
}

void ofParameter<void>::fromString(const std::string & name){

}

ofParameter<void>& ofParameter<void>::set(const std::string & name){
	setName(name);
	return *this;
}

void ofParameter<void>::trigger(){
	ofNotifyEvent(obj->changedE,this);
}

void ofParameter<void>::trigger(const void * sender){
	ofNotifyEvent(obj->changedE,sender);
}

void ofParameter<void>::enableEvents(){
	obj->changedE.enable();
}

void ofParameter<void>::disableEvents(){
	obj->changedE.disable();
}

bool ofParameter<void>::isSerializable() const{
	return obj->serializable;
}

bool ofParameter<void>::isReadOnly() const{
	return false;
}

void ofParameter<void>::makeReferenceTo(ofParameter<void> & mom){
	*this = mom;
}

void ofParameter<void>::setSerializable(bool serializable){
	obj->serializable = serializable;
}

shared_ptr<ofAbstractParameter> ofParameter<void>::newReference() const{
	return std::make_shared<ofParameter<void>>(*this);
}

void ofParameter<void>::setParent(ofParameterGroup & parent){
	obj->parents.emplace_back(parent.obj);
}

size_t ofParameter<void>::getNumListeners() const{
	return obj->changedE.size();
}
