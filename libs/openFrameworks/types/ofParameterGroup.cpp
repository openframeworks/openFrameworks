#include "ofUtils.h"
#include "ofParameter.h"

using namespace std;

ofParameterGroup::ofParameterGroup()
:obj(new Value)
{

}

void ofParameterGroup::add(ofAbstractParameter & parameter){
	shared_ptr<ofAbstractParameter> param = parameter.newReference();
	const std::string name = param->getEscapedName();
	if(obj->parametersIndex.find(name) != obj->parametersIndex.end()){
		ofLogWarning() << "Adding another parameter with same name '" << param->getName() << "' to group '" << getName() << "'";
	}
	obj->parameters.push_back(param);
	obj->parametersIndex[name] = obj->parameters.size()-1;
	param->setParent(*this);
}

void ofParameterGroup::remove(ofAbstractParameter &param){
	for(auto & p: obj->parameters){
		if(p->isReferenceTo(param)){
			remove(param.getName());
			return;
		}
	}
}

void ofParameterGroup::remove(size_t index){
	if(index>obj->parameters.size()){
		return;
	}
	remove(obj->parameters[index]->getName());
}

void ofParameterGroup::remove(const string &name){
	auto escaped = escape(name);
	if(!contains(escaped)){
		return;
	}
	size_t paramIndex = obj->parametersIndex[escaped];
	obj->parameters.erase(obj->parameters.begin() + paramIndex);
	obj->parametersIndex.erase(escaped);
	std::for_each(obj->parameters.begin() + paramIndex, obj->parameters.end(), [&](shared_ptr<ofAbstractParameter>& p){
		obj->parametersIndex[p->getEscapedName()] -= 1;
	});
}

void ofParameterGroup::clear(){
	auto name = this->getName();
	obj.reset(new Value);
	setName(name);
}

string ofParameterGroup::valueType() const{
	return typeid(*this).name();
}

const ofParameter<void> & ofParameterGroup::getVoid(const string& name) const	{
	return get<void>(name);
}

const ofParameter<bool> & ofParameterGroup::getBool(const string& name) const	{
	return get<bool>(name);
}

const ofParameter<int> & ofParameterGroup::getInt(const string& name) const{
	return get<int>(name);
}

const ofParameter<float> & ofParameterGroup::getFloat(const string& name) const{
	return get<float>(name);
}

const ofParameter<char> & ofParameterGroup::getChar(const string& name) const{
	return get<char>(name);
}

const ofParameter<string> & ofParameterGroup::getString(const string& name) const{
	return get<string>(name);
}

const ofParameter<ofPoint> & ofParameterGroup::getPoint(const string& name) const{
	return get<ofPoint>(name);
}

const ofParameter<ofDefaultVec2> & ofParameterGroup::getVec2f(const string& name) const{
	return get<ofDefaultVec2>(name);
}

const ofParameter<ofDefaultVec3> & ofParameterGroup::getVec3f(const string& name) const{
	return get<ofDefaultVec3>(name);
}

const ofParameter<ofDefaultVec4> & ofParameterGroup::getVec4f(const string& name) const{
	return get<ofDefaultVec4>(name);
}

const ofParameter<ofColor> & ofParameterGroup::getColor(const string& name) const{
	return get<ofColor>(name);
}

const ofParameter<ofShortColor> & ofParameterGroup::getShortColor(const string& name) const{
	return get<ofShortColor>(name);
}

const ofParameter<ofFloatColor> & ofParameterGroup::getFloatColor(const string& name) const{
	return get<ofFloatColor>(name);
}

const ofParameter<ofRectangle> & ofParameterGroup::getRectangle(const string& name) const{
	return get<ofRectangle>(name);
}

const ofParameterGroup & ofParameterGroup::getGroup(const string& name) const{
	return static_cast<const ofParameterGroup&>(get(name));
}

const ofParameter<void> & ofParameterGroup::getVoid(std::size_t pos) const{
	return get<void>(pos);
}

const ofParameter<bool> & ofParameterGroup::getBool(std::size_t pos) const{
	return get<bool>(pos);
}

const ofParameter<int> & ofParameterGroup::getInt(std::size_t pos) const{
	return get<int>(pos);
}

const ofParameter<float> & ofParameterGroup::getFloat(std::size_t pos) const{
	return get<float>(pos);
}

const ofParameter<char> & ofParameterGroup::getChar(std::size_t pos) const{
	return get<char>(pos);
}

const ofParameter<string> & ofParameterGroup::getString(std::size_t pos) const{
	return get<string>(pos);
}

const ofParameter<ofPoint> & ofParameterGroup::getPoint(std::size_t pos)	 const{
	return get<ofPoint>(pos);
}

const ofParameter<ofDefaultVec2> & ofParameterGroup::getVec2f(std::size_t pos) const{
	return get<ofDefaultVec2>(pos);
}

const ofParameter<ofDefaultVec3> & ofParameterGroup::getVec3f(std::size_t pos) const{
	return get<ofDefaultVec3>(pos);
}

const ofParameter<ofDefaultVec4> & ofParameterGroup::getVec4f(std::size_t pos) const{
	return get<ofDefaultVec4>(pos);
}

const ofParameter<ofColor> & ofParameterGroup::getColor(std::size_t pos) const{
	return get<ofColor>(pos);
}

const ofParameter<ofShortColor> & ofParameterGroup::getShortColor(std::size_t pos) const{
	return get<ofShortColor>(pos);
}

const ofParameter<ofFloatColor> & ofParameterGroup::getFloatColor(std::size_t pos) const{
	return get<ofFloatColor>(pos);
}

const ofParameter<ofRectangle> & ofParameterGroup::getRectangle(std::size_t pos) const{
	return get<ofRectangle>(pos);
}

const ofParameterGroup & ofParameterGroup::getGroup(std::size_t pos) const{
	if(pos>=size()){
		throw std::out_of_range(("get(): " + ofToString(pos) + "out of bounds").c_str());
	}else{
		if(getType(pos)==typeid(ofParameterGroup).name()){
			return *static_cast<ofParameterGroup* >(obj->parameters[pos].get());
		}else{
			throw std::runtime_error(("get():  bad type for pos " + ofToString(pos) + ", returning empty group").c_str());
		}
	}
}

ofParameter<void> & ofParameterGroup::getVoid(const string& name){
	return get<void>(name);
}

ofParameter<bool> & ofParameterGroup::getBool(const string& name){
	return get<bool>(name);
}

ofParameter<int> & ofParameterGroup::getInt(const string& name){
	return get<int>(name);
}

ofParameter<float> & ofParameterGroup::getFloat(const string& name){
	return get<float>(name);
}

ofParameter<char> & ofParameterGroup::getChar(const string& name){
	return get<char>(name);
}

ofParameter<string> & ofParameterGroup::getString(const string& name){
	return get<string>(name);
}

ofParameter<ofPoint> & ofParameterGroup::getPoint(const string& name){
	return get<ofPoint>(name);
}

ofParameter<ofDefaultVec2> & ofParameterGroup::getVec2f(const string& name){
	return get<ofDefaultVec2>(name);
}

ofParameter<ofDefaultVec3> & ofParameterGroup::getVec3f(const string& name){
	return get<ofDefaultVec3>(name);
}

ofParameter<ofDefaultVec4> & ofParameterGroup::getVec4f(const string& name){
	return get<ofDefaultVec4>(name);
}

ofParameter<ofColor> & ofParameterGroup::getColor(const string& name){
	return get<ofColor>(name);
}

ofParameter<ofShortColor> & ofParameterGroup::getShortColor(const string& name){
	return get<ofShortColor>(name);
}

ofParameter<ofFloatColor> & ofParameterGroup::getFloatColor(const string& name){
	return get<ofFloatColor>(name);
}
ofParameter<ofRectangle> & ofParameterGroup::getRectangle(const string& name){
	return get<ofRectangle>(name);
}

ofParameterGroup & ofParameterGroup::getGroup(const string& name){
	return static_cast<ofParameterGroup& >(get(name));
}

ofParameter<void> & ofParameterGroup::getVoid(std::size_t pos){
	return get<void>(pos);
}

ofParameter<bool> & ofParameterGroup::getBool(std::size_t pos){
	return get<bool>(pos);
}

ofParameter<int> & ofParameterGroup::getInt(std::size_t pos){
	return get<int>(pos);
}

ofParameter<float> & ofParameterGroup::getFloat(std::size_t pos){
	return get<float>(pos);
}

ofParameter<char> & ofParameterGroup::getChar(std::size_t pos){
	return get<char>(pos);
}

ofParameter<string> & ofParameterGroup::getString(std::size_t pos){
	return get<string>(pos);
}

ofParameter<ofPoint> & ofParameterGroup::getPoint(std::size_t pos){
	return get<ofPoint>(pos);
}

ofParameter<ofDefaultVec2> & ofParameterGroup::getVec2f(std::size_t pos){
	return get<ofDefaultVec2>(pos);
}

ofParameter<ofDefaultVec3> & ofParameterGroup::getVec3f(std::size_t pos){
	return get<ofDefaultVec3>(pos);
}

ofParameter<ofDefaultVec4> & ofParameterGroup::getVec4f(std::size_t pos){
	return get<ofDefaultVec4>(pos);
}

ofParameter<ofColor> & ofParameterGroup::getColor(std::size_t pos){
	return get<ofColor>(pos);
}

ofParameter<ofShortColor> & ofParameterGroup::getShortColor(std::size_t pos){
	return get<ofShortColor>(pos);
}

ofParameter<ofFloatColor> & ofParameterGroup::getFloatColor(std::size_t pos){
	return get<ofFloatColor>(pos);
}

ofParameter<ofRectangle> & ofParameterGroup::getRectangle(std::size_t pos){
	return get<ofRectangle>(pos);
}

ofParameterGroup & ofParameterGroup::getGroup(std::size_t pos){
	if(pos>=size()){
		throw std::out_of_range(("get(): " + ofToString(pos) + "out of bounds").c_str());
	}else{
		if(getType(pos)==typeid(ofParameterGroup).name()){
			return *static_cast<ofParameterGroup* >(obj->parameters[pos].get());
		}else{
			throw std::runtime_error(("get():  bad type for pos " + ofToString(pos) + ", returning empty group").c_str());
		}
	}
}


std::size_t ofParameterGroup::size() const{
	return obj->parameters.size();
}

string ofParameterGroup::getName(std::size_t position) const{
	if(position>=size()){
		return "";
	}else{
		return obj->parameters[position]->getName();
	}
}

string ofParameterGroup::getType(std::size_t position) const{
	if(position>=size()) return "";
	else return obj->parameters[position]->type();
}


int ofParameterGroup::getPosition(const string& name) const{
	if(obj->parametersIndex.find(escape(name))!=obj->parametersIndex.end())
		return obj->parametersIndex.find(escape(name))->second;
	return -1;
}

string ofParameterGroup::getName() const{
	return obj->name;
}

void ofParameterGroup::setName(const string & name){
	obj->name = name;
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

void ofParameterGroup::fromString(const string & name){
	ofLogWarning() << "ofParameterGroup doesn't implement fromString yet";
}


const ofAbstractParameter & ofParameterGroup::get(const string& name) const{
	map<string,std::size_t>::const_iterator it = obj->parametersIndex.find(escape(name));
	std::size_t index = it->second;
	return get(index);
}

const ofAbstractParameter & ofParameterGroup::get(std::size_t pos) const{
	return *obj->parameters[pos];
}


const ofAbstractParameter & ofParameterGroup::operator[](const string& name) const{
	return get(name);
}

const ofAbstractParameter & ofParameterGroup::operator[](std::size_t pos) const{
	return get(pos);
}

ofAbstractParameter & ofParameterGroup::get(const string& name){
	map<string,std::size_t>::const_iterator it = obj->parametersIndex.find(escape(name));
	std::size_t index = it->second;
	return get(index);
}

ofAbstractParameter & ofParameterGroup::get(std::size_t pos){
	return *obj->parameters[pos];
}


ofAbstractParameter & ofParameterGroup::operator[](const string& name){
	return get(name);
}

ofAbstractParameter & ofParameterGroup::operator[](std::size_t pos){
	return get(pos);
}

ostream& operator<<(ostream& os, const ofParameterGroup& group) {
	std::streamsize width = os.width();
	for(std::size_t i=0;i<group.size();i++){
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

bool ofParameterGroup::contains(const string& name) const{
	return obj->parametersIndex.find(escape(name))!=obj->parametersIndex.end();
}

void ofParameterGroup::Value::notifyParameterChanged(ofAbstractParameter & param){
	ofNotifyEvent(parameterChangedE,param);
	parents.erase(std::remove_if(parents.begin(),parents.end(),[&param](const weak_ptr<Value> & p){
		auto parent = p.lock();
		if(parent) parent->notifyParameterChanged(param);
		return !parent;
	}),parents.end());
}

const ofParameterGroup ofParameterGroup::getFirstParent() const{
	auto first = std::find_if(obj->parents.begin(),obj->parents.end(),[](const weak_ptr<Value> & p){return p.lock()!=nullptr;});
	if(first!=obj->parents.end()){
		return first->lock();
	}else{
		return shared_ptr<Value>(nullptr);
	}
}

ofEvent<ofAbstractParameter> & ofParameterGroup::parameterChangedE(){
	return obj->parameterChangedE;
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

bool ofParameterGroup::isReadOnly() const{
	return false;
}

const void* ofParameterGroup::getInternalObject() const{
	return obj.get();
}

shared_ptr<ofAbstractParameter> ofParameterGroup::newReference() const{
	return std::make_shared<ofParameterGroup>(*this);
}

void ofParameterGroup::setParent(ofParameterGroup & parent){
	obj->parents.emplace_back(parent.obj);
}

ofParameterGroup::operator bool() const{
	return obj != nullptr;
}

vector<shared_ptr<ofAbstractParameter> >::iterator ofParameterGroup::begin(){
	return obj->parameters.begin();
}

vector<shared_ptr<ofAbstractParameter> >::iterator ofParameterGroup::end(){
	return obj->parameters.end();
}

vector<shared_ptr<ofAbstractParameter> >::const_iterator ofParameterGroup::begin() const{
	return obj->parameters.begin();
}

vector<shared_ptr<ofAbstractParameter> >::const_iterator ofParameterGroup::end() const{
	return obj->parameters.end();
}

vector<shared_ptr<ofAbstractParameter> >::reverse_iterator ofParameterGroup::rbegin(){
	return obj->parameters.rbegin();
}

vector<shared_ptr<ofAbstractParameter> >::reverse_iterator ofParameterGroup::rend(){
	return obj->parameters.rend();
}

vector<shared_ptr<ofAbstractParameter> >::const_reverse_iterator ofParameterGroup::rbegin() const{
	return obj->parameters.rbegin();
}

vector<shared_ptr<ofAbstractParameter> >::const_reverse_iterator ofParameterGroup::rend() const{
	return obj->parameters.rend();
}


