#pragma once

#include "ofEvents.h"
#include "ofTypes.h"

template<typename ParameterType>
class ofxParameter{
public:
	ofxParameter();
	ofxParameter(ParameterType v);
	ofxParameter(string name, ParameterType v);
	virtual ~ofxParameter(){};

	ParameterType operator=(ParameterType v);
	operator ParameterType & ();

	virtual ParameterType getValue();
	virtual void setValue(ParameterType v);

	void setName(string name);
	string getName();

	ofEvent<ParameterType> changedE;
private:
	class Value{
	public:
		Value(){};

		Value(ParameterType v)
		:value(v){};

		Value(string name, ParameterType v)
		:name(name)
		,value(v){};

		ParameterType value;
		string name;
	};
	ofPtr<Value> obj;
};

template<typename ParameterType>
ofxParameter<ParameterType>::ofxParameter(){
	obj = ofPtr<Value>(new Value);
}

template<typename ParameterType>
ofxParameter<ParameterType>::ofxParameter(ParameterType v)
:obj(ofPtr<Value>(new Value(v))){}

template<typename ParameterType>
ofxParameter<ParameterType>::ofxParameter(string name, ParameterType v)
:obj(ofPtr<Value>(new Value(name, v))){}

template<typename ParameterType>
ParameterType ofxParameter<ParameterType>::operator=(ParameterType v){
	setValue(v);
	return obj->value;
}

template<typename ParameterType>
ParameterType ofxParameter<ParameterType>::getValue(){
	return obj->value;
}

template<typename ParameterType>
void ofxParameter<ParameterType>::setValue(ParameterType v){
	ofNotifyEvent(changedE,v);
	obj->value = v;
}

template<typename ParameterType>
ofxParameter<ParameterType>::operator ParameterType & (){
	return obj->value;
}
