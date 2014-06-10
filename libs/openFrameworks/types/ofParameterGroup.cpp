#include "ofParameterGroup.h"
#include "ofUtils.h"
#include "ofParameter.h"

ofParameterGroup::ofParameterGroup()
:obj(new Value)
{

}

void ofParameterGroup::add(ofAbstractParameter & param){
	shared_ptr<ofAbstractParameter> group = param.newReference();
	obj->parameters.push_back(group);
	obj->parametersIndex[group->getEscapedName()] = obj->parameters.size()-1;
	group->setParent(this);
}

void ofParameterGroup::clear(){
	obj->parameters.clear();
	obj->parametersIndex.clear();
}

ofParameter<bool> ofParameterGroup::getBool(string name) const	{
	return get<bool>(name);
}

ofParameter<int> ofParameterGroup::getInt(string name) const{
	return get<int>(name);
}

ofParameter<float> ofParameterGroup::getFloat(string name) const{
	return get<float>(name);
}

ofParameter<char> ofParameterGroup::getChar(string name) const{
	return get<char>(name);
}

ofParameter<string> ofParameterGroup::getString(string name) const{
	return get<string>(name);
}

ofParameter<ofPoint> ofParameterGroup::getPoint(string name) const{
	return get<ofPoint>(name);
}

ofParameter<ofVec2f> ofParameterGroup::getVec2f(string name) const{
	return get<ofVec2f>(name);
}

ofParameter<ofVec3f> ofParameterGroup::getVec3f(string name) const{
	return get<ofVec3f>(name);
}

ofParameter<ofVec4f> ofParameterGroup::getVec4f(string name) const{
	return get<ofVec4f>(name);
}

ofParameter<ofColor> ofParameterGroup::getColor(string name) const{
	return get<ofColor>(name);
}

ofParameter<ofShortColor> ofParameterGroup::getShortColor(string name) const{
	return get<ofShortColor>(name);
}

ofParameter<ofFloatColor> ofParameterGroup::getFloatColor(string name) const{
	return get<ofFloatColor>(name);
}

ofParameterGroup ofParameterGroup::getGroup(string name) const{
	return static_cast<ofParameterGroup& >(get(name));
}

ofParameter<bool> ofParameterGroup::getBool(int pos) const{
	return get<bool>(pos);
}

ofParameter<int> ofParameterGroup::getInt(int pos) const{
	return get<int>(pos);
}

ofParameter<float> ofParameterGroup::getFloat(int pos) const{
	return get<float>(pos);
}

ofParameter<char> ofParameterGroup::getChar(int pos) const{
	return get<char>(pos);
}

ofParameter<string> ofParameterGroup::getString(int pos) const{
	return get<string>(pos);
}

ofParameter<ofPoint> ofParameterGroup::getPoint(int pos)	 const{
	return get<ofPoint>(pos);
}

ofParameter<ofVec2f> ofParameterGroup::getVec2f(int pos) const{
	return get<ofVec2f>(pos);
}

ofParameter<ofVec3f> ofParameterGroup::getVec3f(int pos) const{
	return get<ofVec3f>(pos);
}

ofParameter<ofVec4f> ofParameterGroup::getVec4f(int pos) const{
	return get<ofVec4f>(pos);
}

ofParameter<ofColor> ofParameterGroup::getColor(int pos) const{
	return get<ofColor>(pos);
}

ofParameter<ofShortColor> ofParameterGroup::getShortColor(int pos) const{
	return get<ofShortColor>(pos);
}

ofParameter<ofFloatColor> ofParameterGroup::getFloatColor(int pos) const{
	return get<ofFloatColor>(pos);
}


ofParameterGroup ofParameterGroup::getGroup(int pos) const{
	if(pos>=size()){
		return ofParameterGroup();
	}else{
		if(getType(pos)==typeid(ofParameterGroup).name()){
			return *static_cast<ofParameterGroup* >(obj->parameters[pos].get());
		}else{
			ofLogError("ofParameterGroup") << "get(): bad type for pos " << pos << ", returning empty group";
			return ofParameterGroup();
		}
	}
}


int ofParameterGroup::size() const{
	return obj->parameters.size();
}

string ofParameterGroup::getName(int position) const{
	if(position>=size()){
		return "";
	}else{
		return obj->parameters[position]->getName();
	}
}

string ofParameterGroup::getType(int position) const{
	if(position>=size()) return "";
	else return obj->parameters[position]->type();
}


int ofParameterGroup::getPosition(string name) const{
	if(obj->parametersIndex.find(escape(name))!=obj->parametersIndex.end())
		return obj->parametersIndex.find(escape(name))->second;
	return -1;
}

string ofParameterGroup::getName() const{
	return obj->name;
}

void ofParameterGroup::setName(string _name){
	obj->name = _name;
}

string ofParameterGroup::getEscapedName() const{
	if(getName()==""){
		return "group";
	}else{
		return ofAbstractParameter::getEscapedName();
	}
}

string ofParameterGroup::toString() const{
	stringstream out;
	out << *this;
	return out.str();
}


ofAbstractParameter & ofParameterGroup::get(string name) const{
	map<string,int>::const_iterator it = obj->parametersIndex.find(escape(name));
	int index = it->second;
	return get(index);
}

ofAbstractParameter & ofParameterGroup::get(int pos) const{
	return *obj->parameters[pos];
}


ofAbstractParameter & ofParameterGroup::operator[](string name) const{
	return get(name);
}

ofAbstractParameter & ofParameterGroup::operator[](int pos) const{
	return get(pos);
}

ostream& operator<<(ostream& os, const ofParameterGroup& group) {
	std::streamsize width = os.width();
	for(int i=0;i<group.size();i++){
		if(group.getType(i)==typeid(ofParameterGroup).name()){
			os << group.getName(i) << ":" << endl;
			os << setw(width+4);
			os << group.getGroup(i);
		}else{
			os << group.getName(i) << ":" << group.get(i) << endl;
			os << setw(width);
		}
	}
	return os;
}

bool ofParameterGroup::contains(string name){
	return obj->parametersIndex.find(escape(name))!=obj->parametersIndex.end();
}

void ofParameterGroup::notifyParameterChanged(ofAbstractParameter & param){
	ofNotifyEvent(parameterChangedE,param);
	if(getParent()) getParent()->notifyParameterChanged(param);
}

ofAbstractParameter & ofParameterGroup::back(){
	return *obj->parameters.back();
}

ofAbstractParameter & ofParameterGroup::front(){
	return *obj->parameters.front();
}

const ofAbstractParameter & ofParameterGroup::back() const{
	return *obj->parameters.back();
}

const ofAbstractParameter & ofParameterGroup::front() const{
	return *obj->parameters.front();
}

void ofParameterGroup::setSerializable(bool _serializable){
	obj->serializable = _serializable;
}

bool ofParameterGroup::isSerializable() const{
	return obj->serializable;
}

shared_ptr<ofAbstractParameter> ofParameterGroup::newReference() const{
	return shared_ptr<ofAbstractParameter>(new ofParameterGroup(*this));
}
