#pragma once

#include "json.hpp"
#include "ofFileUtils.h"
#include "ofParameter.h"

// for convenience
using ofJson = nlohmann::json;

inline ofJson ofLoadJson(const std::string & filename){
	ofJson json;
	ofFile jsonFile(filename);
	jsonFile >> json;
	return json;
}

inline void ofSaveJson(const std::string & filename, ofJson & json){
	ofFile jsonFile(filename, ofFile::WriteOnly);
	jsonFile << json;
}

inline void ofSerialize(ofJson & js, ofAbstractParameter & parameter){
	if(!parameter.isSerializable()){
		return;
	}
	string name = parameter.getEscapedName();
	if(name == ""){
		name = "UnknownName";
	}
	if(parameter.type() == typeid(ofParameterGroup).name()){
		const ofParameterGroup & group = static_cast <const ofParameterGroup &>(parameter);
		auto jsonGroup = js[name];
		for(auto & p: group){
			ofSerialize(jsonGroup, *p);
		}
		js[name] = jsonGroup;
	}else{
		string value = parameter.toString();
		js[name] = value;
	}
}

inline void ofDeserialize(const ofJson & json, ofAbstractParameter & parameter){
	if(!parameter.isSerializable()){
		return;
	}
	string name = parameter.getEscapedName();
	if(parameter.type() == typeid(ofParameterGroup).name()){
		ofParameterGroup & group = static_cast <ofParameterGroup &>(parameter);
		if(json.find(name) != json.end()){
			for(auto & p: group){
				ofDeserialize(json[name], *p);
			}
		}
	}else{
		if(json.find(name) != json.end()){
			if(parameter.type() == typeid(ofParameter <int> ).name() && json[name].is_number_integer()){
				parameter.cast <int>() = (int)json[name];
			}else if(parameter.type() == typeid(ofParameter <float> ).name() && json[name].is_number_float()){
				parameter.cast <float>() = (float)json[name];
			}else if(parameter.type() == typeid(ofParameter <bool> ).name() && json[name].is_boolean()){
				parameter.cast <bool>() = (bool)json[name];
			}else if(parameter.type() == typeid(ofParameter <int64_t> ).name() && json[name].is_number_integer()){
				parameter.cast <int64_t>() = (int64_t)json[name];
			}else if(parameter.type() == typeid(ofParameter <string> ).name()){
				parameter.cast <std::string>() = json[name].get<std::string>();
			}else{
				parameter.fromString(json[name]);
			}
		}
	}

}
