/*
 * ofxParameterGroup.h
 *
 *  Created on: 10/07/2012
 *      Author: arturo
 */

#ifndef OFXPARAMETERGROUP_H_
#define OFXPARAMETERGROUP_H_

#include <map>
#include "Poco/Any.h"
#include "ofConstants.h"
#include "ofParameter.h"

class ofxParameterGroup {
public:
	template<typename ParameterType>
	void add(ofxParameter<ParameterType> param);

	void add(ofxParameterGroup group);

	template<typename ParameterType>
	ofxParameter<ParameterType> get(string name, ParameterType def) const;

	ofxParameter<bool> getBool(string name, const bool& defaultValue=false) const;
	ofxParameter<int> getInt(string name, const int& defaultValue=0) const;
	ofxParameter<float> getFloat(string name, const float& defaultValue=0) const;
	ofxParameter<char> getChar(string name, const char& defaultValue=0) const;
	ofxParameter<string> getString(string name, const string& defaultValue="")	 const;
	ofxParameter<ofPoint> getPoint(string name, const ofPoint& defaultValue=ofPoint())	 const;
	ofxParameter<ofVec2f> getVec2f(string name, const ofVec2f& defaultValue=ofVec2f()) const;
	ofxParameter<ofVec3f> getVec3f(string name, const ofVec3f& defaultValue=ofVec3f()) const;
	ofxParameterGroup& getGroup(string name, const ofxParameterGroup& defaultValue=ofxParameterGroup()) const;

	template<typename ParameterType>
	ofxParameter<ParameterType> get(int pos, ParameterType def) const;

	ofxParameter<bool> getBool(int pos, const bool& defaultValue=false) const;
	ofxParameter<int> getInt(int pos, const int& defaultValue=0) const;
	ofxParameter<float> getFloat(int pos, const float& defaultValue=0) const;
	ofxParameter<char> getChar(int pos, const char& defaultValue=0) const;
	ofxParameter<string> getString(int pos, const string& defaultValue="")	 const;
	ofxParameter<ofPoint> getPoint(int pos, const ofPoint& defaultValue=ofPoint())	 const;
	ofxParameter<ofVec2f> getVec2f(int pos, const ofVec2f& defaultValue=ofVec2f()) const;
	ofxParameter<ofVec3f> getVec3f(int pos, const ofVec3f& defaultValue=ofVec3f()) const;
	ofxParameterGroup& getGroup(int pos, const ofxParameterGroup& defaultValue=ofxParameterGroup()) const;

	int size() const;
	//string getName(int position) const;
	string getType(int position) const;

	friend ostream& operator<<(ostream& os, const ofxParameterGroup& group) {
		os << "";
		return os;
	}

private:
	map<string,int> parametersIndex;
	vector<Poco::Any> parameters;
};


template<typename ParameterType>
void ofxParameterGroup::add(ofxParameter<ParameterType> param){
	parameters.push_back(param);
	parametersIndex[param.getName()] = parameters.size()-1;
}


template<typename ParameterType>
ofxParameter<ParameterType> ofxParameterGroup::get(string name, ParameterType def) const{
	map<string,int>::const_iterator it = parametersIndex.find(name);
	if(it==parametersIndex.end()){
		return ofxParameter<ParameterType>(def);
	}else{
		int index = it->second;
		return get(index,def);
	}
}



inline ofxParameter<bool> ofxParameterGroup::getBool(string name, const bool& defaultValue) const	{
	return get(name, defaultValue);
}

inline ofxParameter<int> ofxParameterGroup::getInt(string name, const int& defaultValue) const{
	return get(name, defaultValue);
}

inline ofxParameter<float> ofxParameterGroup::getFloat(string name, const float& defaultValue) const{
	return get(name, defaultValue);
}

inline ofxParameter<char> ofxParameterGroup::getChar(string name, const char& defaultValue) const{
	return get(name, defaultValue);
}

inline ofxParameter<string> ofxParameterGroup::getString(string name, const string& defaultValue) const{
	return get(name, defaultValue);
}

inline ofxParameter<ofPoint> ofxParameterGroup::getPoint(string name, const ofPoint& defaultValue) const{
	return get(name, defaultValue);
}

inline ofxParameter<ofVec2f> ofxParameterGroup::getVec2f(string name, const ofVec2f& defaultValue) const{
	return get(name, defaultValue);
}

inline ofxParameter<ofVec3f> ofxParameterGroup::getVec3f(string name, const ofVec3f& defaultValue) const{
	return get(name, defaultValue);
}

inline ofxParameterGroup& ofxParameterGroup::getGroup(string name, const ofxParameterGroup& defaultValue) const{
	return get(name, defaultValue);
}


template<typename ParameterType>
ofxParameter<ParameterType> ofxParameterGroup::get(int pos, ParameterType def) const{
	if(pos>=size()){
		return ofxParameter<ParameterType>(def);
	}else{
		if(parameters[pos].type()==typeid(ofxParameter<ParameterType>)){
			return Poco::RefAnyCast<ofxParameter<ParameterType> >(parameters[pos]);
		}else{
			ofLogError() << "ofParameterGroup::get asked for bad type, returning empty";
			return ofxParameter<ParameterType>(def);
		}
	}
}

inline ofxParameter<bool> ofxParameterGroup::getBool(int pos, const bool& defaultValue) const{
	return get(pos,defaultValue);
}

inline ofxParameter<int> ofxParameterGroup::getInt(int pos, const int& defaultValue) const{
	return get(pos,defaultValue);
}

inline ofxParameter<float> ofxParameterGroup::getFloat(int pos, const float& defaultValue) const{
	return get(pos,defaultValue);
}

inline ofxParameter<char> ofxParameterGroup::getChar(int pos, const char& defaultValue) const{
	return get(pos,defaultValue);
}

inline ofxParameter<string> ofxParameterGroup::getString(int pos, const string& defaultValue)	 const{
	return get(pos,defaultValue);
}

inline ofxParameter<ofPoint> ofxParameterGroup::getPoint(int pos, const ofPoint& defaultValue)	 const{
	return get(pos,defaultValue);
}

inline ofxParameter<ofVec2f> ofxParameterGroup::getVec2f(int pos, const ofVec2f& defaultValue) const{
	return get(pos,defaultValue);
}

inline ofxParameter<ofVec3f> ofxParameterGroup::getVec3f(int pos, const ofVec3f& defaultValue) const{
	return get(pos,defaultValue);
}

inline ofxParameterGroup& ofxParameterGroup::getGroup(int pos, const ofxParameterGroup& defaultValue) const{
	return get(pos,defaultValue);
}


inline int ofxParameterGroup::size() const{
	return parameters.size();
}

/*inline string ofxParameterGroup::getName(int position) const{
	if(position>=size()){
		return "";
	}else{
		const ofxAbstractParameter * p = Poco::UnsafeAnyCast<typeof parameters[position].type()>( &parameters[position] );
		return p->getName();
	}
}*/

inline string ofxParameterGroup::getType(int position) const{
	if(position>=size()) return "";
	else return parameters[position].type().name();
}

#endif /* OFXPARAMETERGROUP_H_ */
