/*
 *  ofDictionary.h
 *  DictionaryTester
 *
 *  Created by Memo Akten on 06/02/2011.
 *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
 *
 */

// TODO:
// initialization methods (like ObjC) so you can initialize in one line (use va_list?)
// read/write from disk (As XML, JSON even plist!!!)
// get() is currently returning by reference but default is passed in by value, causes complications
// when iterating and dumping, can't dump unknown types or dictionaries

#pragma once


#include "ofPoint.h"
#include "ofVec2f.h"
#include "ofVec3f.h"
#include <map>
#include <string>
#include "SerializableAny.h"

using namespace std;


class ofDictionary {
public:
	
	// setter for known types
	void addBool(string key, const bool& v)					{ add(key, v); }
	void addInt(string key, const int& v)					{ add(key, v); }
	void addFloat(string key, const float& v)				{ add(key, v); }
	void addChar(string key, const char& v)					{ add(key, v); }
	void addString(string key, const string& v)				{ add(key, v); }
	void addPoint(string key, const ofPoint& v)				{ add(key, v); }
	void addVec2f(string key, const ofVec2f& v)				{ add(key, v); }
	void addVec3f(string key, const ofVec3f& v)				{ add(key, v); }
	void addDictionary(string key, const ofDictionary& v)	{ add(key, v); }

	// setter for unknown types
    template <typename ValueType>
    void add(string key, const ValueType& v)				{ _map[key] = v; }
	
	
	// getters for known types
	// pass in default value if it can't be found
	bool& getBool(string key, const bool& defaultValue=false)				{ return getRef(key, defaultValue); }	
	int& getInt(string key, const int& defaultValue=0)						{ return getRef(key, defaultValue); }
	float& getFloat(string key, const float& defaultValue=0)				{ return getRef(key, defaultValue); }
	char& getChar(string key, const char& defaultValue=0)					{ return getRef(key, defaultValue); }
	string& getString(string key, const string& defaultValue="")			{ return getRef(key, defaultValue); }
	ofPoint& getPoint(string key, const ofPoint& defaultValue=ofPoint())	{ return getRef(key, defaultValue); }
	ofVec2f& getVec2f(string key, const ofVec2f& defaultValue=ofVec2f())	{ return getRef(key, defaultValue); }
	ofVec3f& getVec3f(string key, const ofVec3f& defaultValue=ofVec3f())	{ return getRef(key, defaultValue); }
	ofDictionary& getDictionary(string key, const ofDictionary& defaultValue=ofDictionary())	{ return getRef(key, defaultValue); }
	

	// getter for unknown type (returns by reference)
	template <typename ValueType>
    ValueType& getRef(string key, const ValueType& defaultValue) { 
		try {
			return Poco::RefAnyCast<ValueType>(_map[key]);
		} catch(...) {
			ofLog(OF_LOG_ERROR, "ofDictionary: key does not exist for that type");
			return const_cast<ValueType&>(defaultValue);
		}
	}
	
	// getter for unknown type (returns by value)
	template <typename ValueType>
    ValueType get(string key, ValueType defaultValue) { 
		try {
			return Poco::RefAnyCast<ValueType>(_map[key]);
		} catch(...) {
			ofLog(OF_LOG_ERROR, "ofDictionary: key does not exist for that type");
			return const_cast<ValueType&>(defaultValue);
		}
	}
	
	
	string toJson() const {
		ostringstream os;
		os << endl << "{" << endl;
		map<string, Poco::SerializableAny>::const_iterator end = _map.end();
		map<string, Poco::SerializableAny>::const_iterator begin = _map.begin();
		for(map<string, Poco::SerializableAny>::const_iterator it = begin; it != end; ++it) {
			os << "  " << it->first << " : ";
			
//			// ugly, is there a better way?
//			const std::type_info& type = it->second.type();
//			if(type == typeid(bool)) os << Poco::RefAnyCast<bool>(it->second);
//			else if(type == typeid(int)) os << Poco::RefAnyCast<int>(it->second);
//			else if(type == typeid(float)) os << Poco::RefAnyCast<float>(it->second);
//			else if(type == typeid(char)) os << Poco::RefAnyCast<char>(it->second);
//			else if(type == typeid(string)) os << Poco::RefAnyCast<string>(it->second);
//			else if(type == typeid(ofPoint)) os << Poco::RefAnyCast<ofPoint>(it->second);
//			else if(type == typeid(ofVec2f)) os << Poco::RefAnyCast<ofVec2f>(it->second);
//			else if(type == typeid(ofVec3f)) os << Poco::RefAnyCast<ofVec3f>(it->second);
//			else if(type == typeid(ofDictionary)) os << Poco::RefAnyCast<ofDictionary>(it->second);
//			else os << "unknown type";
			
			try{
				os << Poco::AnyCast<std::string>(it->second);
			}catch(...){
				os << "data type does not overload << operator";
				ofLog(OF_LOG_WARNING,"value with key " + it->first + " couldn't be converted to string");
			}
			
			
			os << endl;
		}
		os<< "}" << endl;
		return os.str();
	}
	
	string toXml() const {
		ostringstream os;
		return os.str();
	}
	
	string toPList() const {
		ostringstream os;
		return os.str();
	}

	
	friend ostream& operator<<(ostream& os, const ofDictionary& dict) {
		os << dict.toJson();
		return os;
	}

protected:
	map<string, Poco::SerializableAny> _map;
};
