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

namespace of {
    namespace json_like_utils {
        inline std::vector<std::uint8_t> loadBinary(const std::filesystem::path &filename) {
            ofFile file{filename};
            if(file.exists()) {
                std::vector<uint8_t> binary{std::istream_iterator<std::uint8_t>(file), std::istream_iterator<std::uint8_t>()};
                return binary;
            } else {
                ofLogError() << "Error loading binary from " << filename.string() << ": file doesn't exist";
                return {};
            }
        }
    }
}

#pragma mark BSON

inline ofJson ofParseBson(const std::vector<std::uint8_t> &bson)
{
    ofJson json;
    try {
        json = ofJson::from_bson(bson);
    } catch(std::exception &e) {
        ofLogError("ofParseBson") << "Error can't parse bson to json: " << e.what();
    } catch(...) {
        ofLogError("ofParseBson") << "Error can't parse bson to json";
    }
    return json;
}

inline ofJson ofLoadBson(const std::filesystem::path& filename) {
    return ofParseBson(of::json_like_utils::loadBinary(filename));
}

inline std::vector<std::uint8_t> ofConvertToBson(const ofJson &json) {
    std::vector<std::uint8_t> bson;
    try {
        bson = ofJson::to_bson(json);
    } catch(std::exception &e) {
       ofLogError("ofConvertToBson") << "Error can't parse bson from json: " << e.what();
    } catch(...) {
       ofLogError("ofConvertToBson") << "Error can't parse bson from json";
    }
    return bson;
}

inline bool ofSaveJsonAsBson(const std::filesystem::path& filename, const ofJson & json){
    ofFile bsonFile(filename, ofFile::WriteOnly);
    try {
        auto &&bson = ofConvertToBson(json);
        bsonFile.write((char *)bson.data(), bson.size());
    } catch(std::exception & e) {
        ofLogError("ofSaveJsonAsBson") << "Error saving json as bson to " << filename.string() << ": " << e.what();
        return false;
    } catch(...) {
        ofLogError("ofSaveJsonAsBson") << "Error saving json as bson to " << filename.string();
        return false;
    }
    return true;
}

#pragma mark CBOR

inline ofJson ofParseCbor(const std::vector<std::uint8_t> &cbor)
{
    ofJson json;
    try {
        json = ofJson::from_cbor(cbor);
    } catch(std::exception &e) {
        ofLogError("ofParseCbor") << "Error can't parse cbor to json: " << e.what();
    } catch(...) {
        ofLogError("ofParseCbor") << "Error can't parse cbor to json";
    }
    return json;
}

inline ofJson ofLoadCbor(const std::filesystem::path& filename) {
    return ofParseCbor(of::json_like_utils::loadBinary(filename));
}

inline std::vector<std::uint8_t> ofConvertToCbor(const ofJson &json) {
    std::vector<std::uint8_t> bson;
    try {
        bson = ofJson::to_cbor(json);
    } catch(std::exception &e) {
       ofLogError("ofConvertToCbor") << "Error can't parse cbor from json: " << e.what();
    } catch(...) {
       ofLogError("ofConvertToCbor") << "Error can't parse cbor from json";
    }
    return bson;
}

inline bool ofSaveJsonAsCbor(const std::filesystem::path& filename, const ofJson & json){
    ofFile cborFile(filename, ofFile::WriteOnly);
    try {
        auto &&cbor = ofConvertToCbor(json);
        cborFile.write((char *)cbor.data(), cbor.size());
    } catch(std::exception & e) {
        ofLogError("ofSaveJsonAsCbor") << "Error saving json as cbor to " << filename.string() << ": " << e.what();
        return false;
    } catch(...) {
        ofLogError("ofSaveJsonAsCbor") << "Error saving json as cbor to " << filename.string();
        return false;
    }
    return true;
}

#pragma mark MessagePack

inline ofJson ofParseMessagePack(const std::vector<std::uint8_t> &cbor)
{
    ofJson json;
    try {
        json = ofJson::from_msgpack(cbor);
    } catch(std::exception &e) {
        ofLogError("ofParseMessagePack") << "Error can't parse msgpack to json: " << e.what();
    } catch(...) {
        ofLogError("ofParseMessagePack") << "Error can't parse msgpack to json";
    }
    return json;
}

inline ofJson ofLoadMessagePack(const std::filesystem::path& filename) {
    return ofParseMessagePack(of::json_like_utils::loadBinary(filename));
}

inline std::vector<std::uint8_t> ofConvertToMessagePack(const ofJson &json) {
    std::vector<std::uint8_t> bson;
    try {
        bson = ofJson::to_msgpack(json);
    } catch(std::exception &e) {
       ofLogError("ofConvertToMessagePack") << "Error can't parse msgpack from json: " << e.what();
    } catch(...) {
       ofLogError("ofConvertToMessagePack") << "Error can't parse msgpack from json";
    }
    return bson;
}

inline bool ofSaveJsonAsMessagePack(const std::filesystem::path& filename, const ofJson & json){
    ofFile msgpackFile(filename, ofFile::WriteOnly);
    try {
        auto &&msgpack = ofConvertToMessagePack(json);
        msgpackFile.write((char *)msgpack.data(), msgpack.size());
    } catch(std::exception & e) {
        ofLogError("ofSaveJsonAsMessagePack") << "Error saving json as msgpack to " << filename.string() << ": " << e.what();
        return false;
    } catch(...) {
        ofLogError("ofSaveJsonAsMessagePack") << "Error saving json as msgpack to " << filename.string();
        return false;
    }
    return true;
}

#pragma mark UBJSON

inline ofJson ofParseUBJson(const std::vector<std::uint8_t> &cbor)
{
    ofJson json;
    try {
        json = ofJson::from_ubjson(cbor);
    } catch(std::exception &e) {
        ofLogError("ofParseUBJson") << "Error can't parse ubjson to json: " << e.what();
    } catch(...) {
        ofLogError("ofParseUBJson") << "Error can't parse ubjson to json";
    }
    return json;
}

inline ofJson ofLoadUBJson(const std::filesystem::path& filename) {
    return ofParseMessagePack(of::json_like_utils::loadBinary(filename));
}

inline std::vector<std::uint8_t> ofConvertToUBJson(const ofJson &json) {
    std::vector<std::uint8_t> bson;
    try {
        bson = ofJson::to_ubjson(json);
    } catch(std::exception &e) {
       ofLogError("ofConvertToUBJson") << "Error can't parse ubjson from json: " << e.what();
    } catch(...) {
       ofLogError("ofConvertToUBJson") << "Error can't parse ubjson from json";
    }
    return bson;
}

inline bool ofSaveJsonAsUBJson(const std::filesystem::path& filename, const ofJson & json){
    ofFile ubjsonFile(filename, ofFile::WriteOnly);
    try {
        auto &&ubjson = ofConvertToUBJson(json);
        ubjsonFile.write((char *)ubjson.data(), ubjson.size());
    } catch(std::exception & e) {
        ofLogError("ofSaveJsonAsUBJson") << "Error saving json as ubjson to " << filename.string() << ": " << e.what();
        return false;
    } catch(...) {
        ofLogError("ofSaveJsonAsUBJson") << "Error saving json as ubjson to " << filename.string();
        return false;
    }
    return true;
}
