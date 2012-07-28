#include "ofParameterGroup.h"
#include "ofUtils.h"
#include "ofParameter.h"

void ofParameterGroup::add(ofAbstractParameter & param){
	parameters.push_back(&param);
	parametersIndex[param.getName()] = parameters.size()-1;
	param.setParent(this);
}

void ofParameterGroup::clear(){
    this->parameters.clear();
    this->parametersIndex.clear();
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

ofParameterGroup ofParameterGroup::getGroup(int pos) const{
	if(pos>=size()){
		return ofParameterGroup();
	}else{
		if(getType(pos)==typeid(ofParameterGroup).name()){
			return *static_cast<ofParameterGroup* >(parameters[pos]);
		}else{
			ofLogError() << "ofParameterGroup::get asked for bad type, returning empty";
			return ofParameterGroup();
		}
	}
}


int ofParameterGroup::size() const{
	return parameters.size();
}

string ofParameterGroup::getName(int position) const{
	if(position>=size()){
		return "";
	}else{
		return parameters[position]->getName();
	}
}

string ofParameterGroup::getType(int position) const{
	if(position>=size()) return "";
	else return parameters[position]->type();
}

string ofParameterGroup::getName() const{
	return name;
}

void ofParameterGroup::setName(string _name){
	name = _name;
}

string ofParameterGroup::toString() const{
	stringstream out;
	out << *this;
	return out.str();
}


ofAbstractParameter & ofParameterGroup::get(string name) const{
	map<string,int>::const_iterator it = parametersIndex.find(name);
	int index = it->second;
	return get(index);
}

ofAbstractParameter & ofParameterGroup::get(int pos) const{
	return *parameters[pos];
}


ofAbstractParameter & ofParameterGroup::operator[](string name) const{
	return get(name);
}

ofAbstractParameter & ofParameterGroup::operator[](int pos) const{
	return get(pos);
}

ostream& operator<<(ostream& os, const ofParameterGroup& group) {
	int width = os.width();
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
	return parametersIndex.find(name)!=parametersIndex.end();
}

void ofParameterGroup::notifyParameterChanged(ofAbstractParameter & param){
	ofNotifyEvent(parameterChangedE,param);
	if(getParent()) getParent()->notifyParameterChanged(param);
}
