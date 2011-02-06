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
#include "Poco/Any.h"

using namespace std;


class ofDictionary {
public:
	void addBool(string key, const bool& v)					{ add(key, v); }
	void addInt(string key, const int& v)					{ add(key, v); }
	void addFloat(string key, const float& v)				{ add(key, v); }
	void addChar(string key, const char& v)					{ add(key, v); }
	void addString(string key, const string& v)				{ add(key, v); }
	void addPoint(string key, const ofPoint& v)				{ add(key, v); }
	void addVec2f(string key, const ofVec2f& v)				{ add(key, v); }
	void addVec3f(string key, const ofVec3f& v)				{ add(key, v); }
	void addDictionary(string key, const ofDictionary& v)	{ add(key, v); }

    template <typename ValueType>
    void add(string key, const ValueType& v)				{ _map[key] = v; }
	

	
	const bool& getBool(string key, const bool& def=false)									{ return get(key, def); }	
	const int& getInt(string key, const int& def=0)											{ return get(key, def); }
	const float& getFloat(string key, const float& def=0)									{ return get(key, def); }
	const char& getChar(string key, const char& def=0)										{ return get(key, def); }
	const string& getString(string key, const string& def="")								{ return get(key, def); }
	const ofPoint& getPoint(string key, const ofPoint& def=ofPoint())						{ return get(key, def); }
	const ofVec2f& getVec2f(string key, const ofVec2f& def=ofVec2f())						{ return get(key, def); }
	const ofVec3f& getVec3f(string key, const ofVec3f& def=ofVec3f())						{ return get(key, def); }
	const ofDictionary& getDictionary(string key, const ofDictionary& def=ofDictionary())	{ return get(key, def); }
	
	template <typename ValueType>
    const ValueType& get(string key, const ValueType& def) { 
		try {
			return Poco::RefAnyCast<ValueType>(_map[key]);
		} catch(...) {
			ofLog(OF_LOG_ERROR, "ofDictionary: key does not exist for that type");
			return def;
		}
	}
	

	
	string toJson() const {
		ostringstream os;
		os << endl << "{" << endl;
		map<string, Poco::Any>::const_iterator end = _map.end();
		map<string, Poco::Any>::const_iterator begin = _map.begin();
		for(map<string, Poco::Any>::const_iterator it = begin; it != end; ++it) {
			const std::type_info& type = it->second.type();
			os << "  " << it->first << " : ";
			
			// ugly, is there a better way?
			if(type == typeid(bool)) os << Poco::RefAnyCast<bool>(it->second);
			else if(type == typeid(int)) os << Poco::RefAnyCast<int>(it->second);
			else if(type == typeid(float)) os << Poco::RefAnyCast<float>(it->second);
			else if(type == typeid(char)) os << Poco::RefAnyCast<char>(it->second);
			else if(type == typeid(string)) os << Poco::RefAnyCast<string>(it->second);
			else if(type == typeid(ofPoint)) os << Poco::RefAnyCast<ofPoint>(it->second);
			else if(type == typeid(ofVec2f)) os << Poco::RefAnyCast<ofVec2f>(it->second);
			else if(type == typeid(ofVec3f)) os << Poco::RefAnyCast<ofVec3f>(it->second);
//			else if(type == typeid(ofDictionary)) os << Poco::RefAnyCast<ofVec3f>(it->second);
			else os << "unknown type";
			
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

	
	friend ostream& operator<<(ostream& os, ofDictionary& dict) {
		os << dict.toJson();
		return os;
	}
	
protected:
	map<string, Poco::Any> _map;
};
