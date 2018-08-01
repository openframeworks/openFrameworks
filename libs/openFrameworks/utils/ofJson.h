#pragma once

#include "json.hpp"
#include "ofParameter.h"

// for convenience
using ofJson = nlohmann::json;


/// \brief Load Json from the given path.
/// \param filename The file to load from.
/// \returns loaded json, or an empty json object on failure.
inline ofJson ofLoadJson(const std::filesystem::path& filename){
	ofJson json;
	ofFile jsonFile(filename);
	if(jsonFile.exists()){
		try{
			jsonFile >> json;
		}catch(std::exception & e){
			ofLogError("ofLoadJson") << "Error loading json from " << filename.string() << ": " << e.what();
		}catch(...){
			ofLogError("ofLoadJson") << "Error loading json from " << filename.string();
		}
	}else{
		ofLogError("ofLoadJson") << "Error loading json from " << filename.string() << ": file doesn't exist";
	}
	return json;
}

/// \brief Save minified Json to the given path.
/// \param filename The destination path.
/// \param json The Json to save.
/// \returns true if the json was saved successfully.
inline bool ofSaveJson(const std::filesystem::path& filename, const ofJson & json){
	ofFile jsonFile(filename, ofFile::WriteOnly);
	try{
		jsonFile << json;
	}catch(std::exception & e){
		ofLogError("ofLoadJson") << "Error saving json to " << filename.string() << ": " << e.what();
		return false;
	}catch(...){
		ofLogError("ofLoadJson") << "Error saving json to " << filename.string();
		return false;
	}
	return true;
}

/// \brief Save "pretty" indented Json to the given path.
/// \param filename The destination path.
/// \param json The Json to save.
/// \returns true if the json was saved successfully.
inline bool ofSavePrettyJson(const std::filesystem::path& filename, const ofJson & json){
    ofFile jsonFile(filename, ofFile::WriteOnly);
    try{
        jsonFile << json.dump(4);
    }catch(std::exception & e){
        ofLogError("ofLoadJson") << "Error saving json to " << filename.string() << ": " << e.what();
        return false;
    }catch(...){
        ofLogError("ofLoadJson") << "Error saving json to " << filename.string();
        return false;
    }
    return true;
}

inline void ofSerialize(ofJson & js, const ofAbstractParameter & parameter){
	if(!parameter.isSerializable()){
		return;
	}
	std::string name = parameter.getEscapedName();
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
		std::string value = parameter.toString();
		js[name] = value;
	}
}

inline void ofDeserialize(const ofJson & json, ofAbstractParameter & parameter){
	if(!parameter.isSerializable()){
		return;
	}
	std::string name = parameter.getEscapedName();
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
			}else if(parameter.type() == typeid(ofParameter <std::string> ).name()){
				parameter.cast <std::string>() = json[name].get<std::string>();
			}else{
				parameter.fromString(json[name]);
			}
		}
	}
}
