#pragma once

#include "ofEvents.h"
#include "ofTypes.h"

class ofxAbstractParameter{
public:
	virtual ~ofxAbstractParameter(){};

};

template<typename ParameterType>
class ofxParameter: public ofxAbstractParameter{
public:
	ofxParameter();
	ofxParameter(ParameterType v);
	ofxParameter(string name, ParameterType v);
	virtual ~ofxParameter(){};

	ParameterType operator=(ParameterType v);
	operator const ParameterType & () const;
	operator ParameterType & ();

	virtual ParameterType getValue();
	virtual void setValue(ParameterType v);

	void setName(string name);
	string getName();

	void setMin(ParameterType min);
	ParameterType getMin();

	void setMax(ParameterType max);
	ParameterType getMax();

	ofxParameter<ParameterType> & set(string name, ParameterType value);
	ofxParameter<ParameterType> & set(string name, ParameterType value, ParameterType min, ParameterType max);

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
		:value(v){}

		Value(string name, ParameterType v)
		:name(name)
		,value(v){}

		Value(string name, ParameterType v, ParameterType min, ParameterType max)
		:name(name)
		,value(v)
		,min(min)
		,max(max){};

		ParameterType value;
		string name;
		ParameterType min, max;
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
ofxParameter<ParameterType> & ofxParameter<ParameterType>::set(string name, ParameterType value, ParameterType min, ParameterType max){
	setName(name);
	setValue(value);
	setMin(min);
	setMax(max);
	return *this;
}

template<typename ParameterType>
ofxParameter<ParameterType> & ofxParameter<ParameterType>::set(string name, ParameterType value){
	setName(name);
	setValue(value);
	return *this;
}

template<typename ParameterType>
ParameterType ofxParameter<ParameterType>::getValue(){
	return obj->value;
}

template<typename ParameterType>
void ofxParameter<ParameterType>::setValue(ParameterType v){
	obj->value = v;
	ofNotifyEvent(obj->changedE,v,this);
}

template<typename ParameterType>
void ofxParameter<ParameterType>::setMin(ParameterType min){
	obj->min = min;
}

template<typename ParameterType>
ParameterType ofxParameter<ParameterType>::getMin(){
	return obj->min;
}

template<typename ParameterType>
void ofxParameter<ParameterType>::setMax(ParameterType max){
	obj->max = max;
}

template<typename ParameterType>
ParameterType ofxParameter<ParameterType>::getMax(){
	return obj->max;
}

template<typename ParameterType>
ofxParameter<ParameterType>::operator ParameterType & (){
	return obj->value;
}

template<typename ParameterType>
ofxParameter<ParameterType>::operator const ParameterType & () const{
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
