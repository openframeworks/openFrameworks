/*
 *  ofxJSONFile.cpp
 *  asift
 *
 *  Created by Jeffrey Crouse on 12/17/10.
 *  Copyright 2010 Eyebeam. All rights reserved.
 *
 */

#include "ofxJSONElement.h"
#include "ofParameterGroup.h"
using namespace Json;

//--------------------------------------------------------------
ofxJSONElement::ofxJSONElement(const Json::Value& v) : Value(v) {

}

//--------------------------------------------------------------
ofxJSONElement::ofxJSONElement(string jsonString) {
	parse(jsonString);
}


ofxJSONElement::ofxJSONElement(Json::ValueType t):Value(t){

}

//--------------------------------------------------------------
bool ofxJSONElement::parse(string jsonString) {
	Reader reader;
	if(!reader.parse( jsonString, *this )) {
		ofLog(OF_LOG_WARNING, "Unable to parse string");
		return false;
	}
	return true;
}


//--------------------------------------------------------------
bool ofxJSONElement::load(string filename) {
	if(filename.find("http://")==0 ) {
		return openRemote(filename);
	}else if(filename.find("https://")==0) {
		return openRemote(filename, true);
	} else {
		return openLocal(filename);
	}
}


//--------------------------------------------------------------
bool ofxJSONElement::openLocal(string filename) {
	ofBuffer buffer;
	ofFile file(filename);
	file >> buffer;
	
	Reader reader;
	if(!reader.parse( buffer.getText(), *this )) {
		ofLog(OF_LOG_WARNING, "Unable to parse "+filename);
		return false;
	}
	return true;
}


//--------------------------------------------------------------
bool ofxJSONElement::openRemote(string filename, bool secure)
{
	string result = ofLoadURL(filename).data.getText();
	
	Reader reader;
	if(!reader.parse( result, *this )) {
		ofLog(OF_LOG_WARNING, "Unable to parse "+filename);
		return false;
	}
	return true;
}

//--------------------------------------------------------------
bool ofxJSONElement::save(string path){
	return save(path,false);
}


//--------------------------------------------------------------
bool ofxJSONElement::save(string filename, bool pretty)
{
	ofFile file_key(filename,ofFile::WriteOnly);
	if (!file_key.is_open()) {
		ofLog(OF_LOG_WARNING, "Unable to open "+filename);
		return false;
	}
	
	if(pretty) {
		StyledWriter writer;
		file_key << writer.write( *this ) << endl;
	} else {
		FastWriter writer;
		file_key << writer.write( *this ) << endl;
	}
	ofLog(OF_LOG_NOTICE, "JSON saved to "+filename);
	file_key.close();	
	return true;
}


//--------------------------------------------------------------
string ofxJSONElement::getRawString(bool pretty)
{
	string raw;
	if(pretty) {
		StyledWriter writer;
		raw = writer.write(*this);
	} else {
		FastWriter writer;
		raw = writer.write(*this);
	}
	return raw;
}

//--------------------------------------------------------------
Json::Value & ofxJSONElement::getJsonValue(){
	return *this;
}

//--------------------------------------------------------------
void ofxJSONElement::serialize(const ofAbstractParameter & parameter){
	if(parameter.type()==typeid(ofParameterGroup).name()){
		const ofParameterGroup & group = static_cast<const ofParameterGroup &>(parameter);
		ofxJSONElement e(objectValue);
		for(int i=0;i<group.size();i++){
			e.serialize(group.get(i));
		}
		Value & v = *this;
		v[group.getName()] = e;
	}else{
		Value & v = *this;
		string type = parameter.type();
		if(type==typeid(ofParameter<int>).name()){
			v[parameter.getName()] = parameter.cast<int>().getValue();
		}else if(type==typeid(ofParameter<float>).name()){
			v[parameter.getName()] = parameter.cast<float>().getValue();
		}else if(type==typeid(ofParameter<bool>).name()){
			v[parameter.getName()] = parameter.cast<bool>().getValue();
		}else{
			v[parameter.getName()] = parameter.toString();
		}
	}
}

//--------------------------------------------------------------
void ofxJSONElement::deserialize(ofAbstractParameter & parameter){
	ofxJSONElement & value = *this;
	switch(value.type()){
	case nullValue:
		break;
	case intValue:      ///< signed integer value
		if(parameter.type()==typeid(ofParameter<int>).name()){
			parameter.cast<int>().setValue(value.asInt());
		}
		break;
	case uintValue:     ///< unsigned integer value
		if(parameter.type()==typeid(ofParameter<unsigned int>).name()){
			parameter.cast<unsigned int>().setValue(value.asUInt());
		}
		break;
	case realValue:     ///< double value
		if(parameter.type()==typeid(ofParameter<float>).name()){
			parameter.cast<float>().setValue(value.asDouble());
		}
		break;

	case stringValue:   ///< UTF-8 string value
		if(parameter.type()==typeid(ofParameter<string>).name()){
			parameter.cast<string>().setValue(value.asString());
		}else{
			parameter.fromString(value.asString());
		}
		break;

	case booleanValue:  ///< bool value
		if(parameter.type()==typeid(ofParameter<bool>).name()){
			parameter.cast<bool>().setValue(value.asBool());
		}
		break;

	case arrayValue:    ///< array value (ordered list)
		break;

	case objectValue:    ///< object value (collection of name/value pairs).
		if(parameter.type()==typeid(ofParameterGroup).name()){
			const vector<string> & members = value.getMemberNames();
			ofParameterGroup & group = static_cast<ofParameterGroup &>(parameter);
			for(int j=0;j<(int)members.size();j++){
				string name = members[j];
				ofxJSONElement subvalue(value[name]);
				if(group.contains(name)){
					subvalue.deserialize(group[name]);
				}else if(group.getName()==name){
					subvalue.deserialize(group);

				}
			}
		}
		break;
	}

}
