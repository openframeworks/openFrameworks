#pragma once

#include "json.hpp"
#include "ofFileUtils.h"
#include "ofParameter.h"

// for convenience
using ofJson = nlohmann::json;

inline ofJson ofLoadJson(const std::string & filename){
	ofJson json;
	ofFile jsonFile(filename);
	if(jsonFile.exists()){
		try{
			jsonFile >> json;
		}catch(std::exception & e){
			ofLogError("ofLoadJson") << "error loading json from " + filename + ": " + e.what();
		}catch(...){
			ofLogError("ofLoadJson") << "error loading json from " + filename;
		}
	}else{
		ofLogError("ofLoadJson") << "error loading json from " + filename + ": file doesn't exist";
	}
	return json;
}

inline bool ofSaveJson(const std::string & filename, ofJson & json){
	ofFile jsonFile(filename, ofFile::WriteOnly);
	try{
		jsonFile << json;
	}catch(std::exception & e){
		ofLogError("ofLoadJson") << "error saving json to " + filename + ": " + e.what();
		return false;
	}catch(...){
		ofLogError("ofLoadJson") << "error saving json to " + filename;
		return false;
	}
	return true;
}

inline bool ofSavePrettyJson(const std::string & filename, ofJson & json){
    ofFile jsonFile(filename, ofFile::WriteOnly);
    try{
        jsonFile << json.dump(4);
    }catch(std::exception & e){
        ofLogError("ofLoadJson") << "error saving json to " + filename + ": " + e.what();
        return false;
    }catch(...){
        ofLogError("ofLoadJson") << "error saving json to " + filename;
        return false;
    }
    return true;
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
	if(json.find(name) != json.end()){
		if(parameter.type() == typeid(ofParameterGroup).name()){
			ofParameterGroup & group = static_cast <ofParameterGroup &>(parameter);
			for(auto & p: group){
				ofDeserialize(json[name], *p);
			}
		}else{
			if(parameter.type() == typeid(ofParameter <int> ).name() && json[name].is_number_integer()){
				parameter.cast <int>() = json[name].get<int>();
			}else if(parameter.type() == typeid(ofParameter <float> ).name() && json[name].is_number_float()){
				parameter.cast <float>() = json[name].get<float>();
			}else if(parameter.type() == typeid(ofParameter <bool> ).name() && json[name].is_boolean()){
				parameter.cast <bool>() = json[name].get<bool>();
			}else if(parameter.type() == typeid(ofParameter <int64_t> ).name() && json[name].is_number_integer()){
				parameter.cast <int64_t>() = json[name].get<int64_t>();
			}else if(parameter.type() == typeid(ofParameter <string> ).name()){
				parameter.cast <std::string>() = json[name].get<std::string>();
			}else{
				parameter.fromString(json[name]);
			}
		}
	}
}
