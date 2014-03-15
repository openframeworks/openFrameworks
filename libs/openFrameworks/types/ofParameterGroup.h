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
#include "ofLog.h"
#include "ofParameter.h"

class ofParameterGroup: public ofAbstractParameter {
public:
	ofParameterGroup();

	template<typename ParameterType>
	void add(ofParameter<ParameterType> param);

	template<typename ParameterType,typename Friend>
	void add(ofReadOnlyParameter<ParameterType,Friend> & param);

	void add(ofAbstractParameter & param);


	void clear();

	ofParameter<bool> getBool(string name) const;
	ofParameter<int> getInt(string name) const;
	ofParameter<float> getFloat(string name) const;
	ofParameter<char> getChar(string name) const;
	ofParameter<string> getString(string name)	 const;
	ofParameter<ofPoint> getPoint(string name)	 const;
	ofParameter<ofVec2f> getVec2f(string name) const;
	ofParameter<ofVec3f> getVec3f(string name) const;
	ofParameter<ofVec4f> getVec4f(string name) const;
	ofParameter<ofColor> getColor(string name) const;
	ofParameter<ofShortColor> getShortColor(string name) const;
	ofParameter<ofFloatColor> getFloatColor(string name) const;

	ofParameterGroup getGroup(string name) const;


	ofParameter<bool> getBool(int pos) const;
	ofParameter<int> getInt(int pos) const;
	ofParameter<float> getFloat(int pos) const;
	ofParameter<char> getChar(int pos) const;
	ofParameter<string> getString(int pos)	 const;
	ofParameter<ofPoint> getPoint(int pos)	 const;
	ofParameter<ofVec2f> getVec2f(int pos) const;
	ofParameter<ofVec3f> getVec3f(int pos) const;
	ofParameter<ofVec4f> getVec4f(int pos) const;
	ofParameter<ofColor> getColor(int pose) const;
	ofParameter<ofShortColor> getShortColor(int pos) const;
	ofParameter<ofFloatColor> getFloatColor(int pos) const;
	ofParameterGroup getGroup(int pos) const;

	ofAbstractParameter & get(string name) const;
	ofAbstractParameter & get(int pos) const;

	ofAbstractParameter & operator[](string name) const;
	ofAbstractParameter & operator[](int pos) const;

	template<typename ParameterType>
	ofParameter<ParameterType> get(string name) const;

	template<typename ParameterType>
	ofParameter<ParameterType> get(int pos) const;

	int size() const;
	string getName(int position) const;
	string getType(int position) const;
	int getPosition(string name) const;

	friend ostream& operator<<(ostream& os, const ofParameterGroup& group);

	string getName() const;
	void setName(string name);
	string getEscapedName() const;
	string toString() const;

	bool contains(string name);

	void notifyParameterChanged(ofAbstractParameter & param);

	ofEvent<ofAbstractParameter> parameterChangedE;

	ofAbstractParameter & back();
	ofAbstractParameter & front();
	const ofAbstractParameter & back() const;
	const ofAbstractParameter & front() const;

	void setSerializable(bool serializable);
	bool isSerializable() const;

private:
	class Value{
	public:
		Value():serializable(true){}

		map<string,int> parametersIndex;
		vector<ofAbstractParameter*> parameters;
		string name;
		bool serializable;
	};
	ofPtr<Value> obj;
};


template<typename ParameterType>
ofParameter<ParameterType> ofParameterGroup::get(string name) const{
	return static_cast<ofParameter<ParameterType>& >(get(name));
}

template<typename ParameterType>
ofParameter<ParameterType> ofParameterGroup::get(int pos) const{
	return static_cast<ofParameter<ParameterType>& >(get(pos));
}

template<class ParameterType>
void ofParameterGroup::add(ofParameter<ParameterType> param){
	ofParameter<ParameterType> * p = new ofParameter<ParameterType>;
	p->makeReferenceTo(param);
	obj->parameters.push_back(p);
	obj->parametersIndex[p->getEscapedName()] = obj->parameters.size()-1;
	p->setParent(this);
}

template<typename ParameterType,typename Friend>
void ofParameterGroup::add(ofReadOnlyParameter<ParameterType,Friend> & param){
	ofReadOnlyParameter<ParameterType,Friend> * p = new ofReadOnlyParameter<ParameterType,Friend>;
	p->makeReferenceTo(param);
	obj->parameters.push_back(p);
	obj->parametersIndex[p->getEscapedName()] = obj->parameters.size()-1;
	p->setParent(this);
}
#endif /* OFXPARAMETERGROUP_H_ */
