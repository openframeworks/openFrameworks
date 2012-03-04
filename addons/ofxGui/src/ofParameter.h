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

	template<class ListenerClass>
	void addListener(ListenerClass * listener, void ( ListenerClass::*method )(ParameterType&)){
		ofAddListener(obj->changedE,listener,method);
	}

	template<class ListenerClass>
	void removeListener(ListenerClass * listener, void ( ListenerClass::*method )(ParameterType&)){
		ofRemoveListener(obj->changedE,listener,method);
	}

	template<class ListenerClass>
	void addListener(ListenerClass * listener, void ( ListenerClass::*method )(const void*, ParameterType&)){
		ofAddListener(obj->changedE,listener,method);
	}

	template<class ListenerClass>
	void removeListener(ListenerClass * listener, void ( ListenerClass::*method )(const void*, ParameterType&)){
		ofRemoveListener(obj->changedE,listener,method);
	}

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
		ofEvent<ParameterType> changedE;
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
	ofNotifyEvent(obj->changedE,v,this);
	obj->value = v;
}

template<typename ParameterType>
ofxParameter<ParameterType>::operator ParameterType & (){
	return obj->value;
}

template<typename ParameterType>
void ofxParameter<ParameterType>::setName(string _name){
	obj->name = _name;
}

template<typename ParameterType>
string ofxParameter<ParameterType>::getName(){
	return obj->name;
}
