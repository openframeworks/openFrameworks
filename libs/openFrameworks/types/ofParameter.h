#pragma once

#include "ofEvents.h"
#include "ofTypes.h"
#include "ofUtils.h"

template<typename ParameterType>
class ofParameter;

class ofParameterGroup;

class ofAbstractParameter{
public:
	ofAbstractParameter();
	virtual ~ofAbstractParameter();
	virtual string getName() const;
	virtual void setName(string name);
	virtual string toString() const;
	virtual void fromString(string str);
	virtual string type() const;

	void setParent(ofParameterGroup * _parent);
	const ofParameterGroup * getParent() const;
	ofParameterGroup * getParent();
	vector<string> getGroupHierarchyNames() const;

	template<typename ParameterType>
	ofParameter<ParameterType> & cast(){
		return static_cast<ofParameter<ParameterType> &>(*this);
	}

	template<typename ParameterType>
	const ofParameter<ParameterType> & cast() const{
		return static_cast<const ofParameter<ParameterType> &>(*this);
	}

	friend ostream& operator<<(ostream& os, const ofAbstractParameter& p);
	friend istream& operator>>(istream& is, ofAbstractParameter& p);

protected:
	void notifyParent();

private:
	ofParameterGroup * parent;
};


template <class T>
struct FriendMaker {typedef T Type;};


template<typename ParameterType,typename Friend>
class ofReadOnlyParameter: public ofAbstractParameter{
public:
	ofReadOnlyParameter();
	ofReadOnlyParameter(ParameterType v);
	ofReadOnlyParameter(string name, ParameterType v);
	ofReadOnlyParameter(string name, ParameterType v, ParameterType min, ParameterType max);
	virtual ~ofReadOnlyParameter(){};

	virtual const ParameterType & get() const;

	virtual void setName(string name);
	virtual string getName() const;

	virtual ParameterType getMin();

	virtual ParameterType getMax();

	virtual string toString() const;

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

	void enableEvents();
	void disableEvents();
protected:

	virtual ParameterType operator=(ParameterType v);
	virtual operator const ParameterType & () const;
	//virtual operator ParameterType & ();

	virtual void set(ParameterType v);

	virtual ofReadOnlyParameter<ParameterType,Friend> & set(string name, ParameterType value);
	virtual ofReadOnlyParameter<ParameterType,Friend> & set(string name, ParameterType value, ParameterType min, ParameterType max);

	virtual void setMin(ParameterType min);
	virtual void setMax(ParameterType max);

	virtual void fromString(string str);


	class Value{
	public:
		Value()
		:bInNotify(false){};

		Value(ParameterType v)
		:value(v)
		,bInNotify(false){};

		Value(string name, ParameterType v)
		:name(name)
		,value(v)
		,bInNotify(false){};

		Value(string name, ParameterType v, ParameterType min, ParameterType max)
		:name(name)
		,value(v)
		,min(min)
		,max(max)
		,bInNotify(false){};

		string name;
		ParameterType value;
		ParameterType min, max;
		ofEvent<ParameterType> changedE;
		bool bInNotify;
	};
	ofPtr<Value> obj;
	void (ofReadOnlyParameter<ParameterType,Friend>::*setMethod)(ParameterType v);

	void eventsSetValue(ParameterType v);
	void noEventsSetValue(ParameterType v);

	friend class FriendMaker<Friend>::Type;
};


template<typename ParameterType,typename Friend>
ofReadOnlyParameter<ParameterType,Friend>::ofReadOnlyParameter()
:obj(new Value)
,setMethod(&ofReadOnlyParameter<ParameterType,Friend>::eventsSetValue){}

template<typename ParameterType,typename Friend>
ofReadOnlyParameter<ParameterType,Friend>::ofReadOnlyParameter(ParameterType v)
:obj(ofPtr<Value>(new Value(v)))
,setMethod(&ofReadOnlyParameter<ParameterType,Friend>::eventsSetValue) {}

template<typename ParameterType,typename Friend>
ofReadOnlyParameter<ParameterType,Friend>::ofReadOnlyParameter(string name, ParameterType v)
:obj(ofPtr<Value>(new Value(name, v)))
,setMethod(&ofReadOnlyParameter<ParameterType,Friend>::eventsSetValue){}

template<typename ParameterType,typename Friend>
ofReadOnlyParameter<ParameterType,Friend>::ofReadOnlyParameter(string name, ParameterType v, ParameterType min, ParameterType max)
:obj(ofPtr<Value>(new Value(name, v, min, max)))
,setMethod(&ofReadOnlyParameter<ParameterType,Friend>::eventsSetValue){}


template<typename ParameterType,typename Friend>
inline ParameterType ofReadOnlyParameter<ParameterType,Friend>::operator=(ParameterType v){
	set(v);
	return obj->value;
}

template<typename ParameterType,typename Friend>
ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::set(string name, ParameterType value, ParameterType min, ParameterType max){
	setName(name);
	set(value);
	setMin(min);
	setMax(max);
	return *this;
}

template<typename ParameterType,typename Friend>
ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::set(string name, ParameterType value){
	setName(name);
	set(value);
	return *this;
}

template<typename ParameterType,typename Friend>
inline const ParameterType & ofReadOnlyParameter<ParameterType,Friend>::get() const{
	return obj->value;
}

template<typename ParameterType,typename Friend>
inline void ofReadOnlyParameter<ParameterType,Friend>::eventsSetValue(ParameterType v){
	if(obj->bInNotify) return;
	obj->bInNotify = true;
	obj->value = v;
	ofNotifyEvent(obj->changedE,v,this);
	notifyParent();
	obj->bInNotify = false;
}

template<typename ParameterType,typename Friend>
inline void ofReadOnlyParameter<ParameterType,Friend>::noEventsSetValue(ParameterType v){
	obj->value = v;
}


template<typename ParameterType,typename Friend>
inline void ofReadOnlyParameter<ParameterType,Friend>::set(ParameterType v){
	(this->*setMethod)(v);
}

template<typename ParameterType,typename Friend>
void ofReadOnlyParameter<ParameterType,Friend>::setMin(ParameterType min){
	obj->min = min;
}

template<typename ParameterType,typename Friend>
ParameterType ofReadOnlyParameter<ParameterType,Friend>::getMin(){
	return obj->min;
}

template<typename ParameterType,typename Friend>
void ofReadOnlyParameter<ParameterType,Friend>::setMax(ParameterType max){
	obj->max = max;
}

template<typename ParameterType,typename Friend>
ParameterType ofReadOnlyParameter<ParameterType,Friend>::getMax(){
	return obj->max;
}

/*template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend>::operator ParameterType & (){
	return obj->value;
}*/

template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend>::operator const ParameterType & () const{
	return obj->value;
}

template<typename ParameterType,typename Friend>
void ofReadOnlyParameter<ParameterType,Friend>::setName(string _name){
	obj->name = _name;
}

template<typename ParameterType,typename Friend>
string ofReadOnlyParameter<ParameterType,Friend>::getName() const{
	return obj->name;
}

template<typename ParameterType,typename Friend>
string ofReadOnlyParameter<ParameterType,Friend>::toString() const{
	return ofToString(obj->value);
}


template<typename ParameterType,typename Friend>
void ofReadOnlyParameter<ParameterType,Friend>::fromString(string str){
	stringstream sstr;
	sstr.str() = str;
	sstr >> obj->value;
}


template<typename ParameterType,typename Friend>
void ofReadOnlyParameter<ParameterType,Friend>::enableEvents(){
	setMethod = &ofReadOnlyParameter<ParameterType,Friend>::eventsSetValue;
}

template<typename ParameterType,typename Friend>
void ofReadOnlyParameter<ParameterType,Friend>::disableEvents(){
	setMethod = &ofReadOnlyParameter<ParameterType,Friend>::noEventsSetValue;
}





template<typename ParameterType>
class ofParameter: public ofReadOnlyParameter<ParameterType,ofAbstractParameter>{
public:
	ofParameter()
	:ofReadOnlyParameter<ParameterType,ofAbstractParameter>(){};

	ofParameter(ParameterType v)
	:ofReadOnlyParameter<ParameterType,ofAbstractParameter>(v){};

	ofParameter(string name, ParameterType v)
	:ofReadOnlyParameter<ParameterType,ofAbstractParameter>(name,v){};

	ofParameter(string name, ParameterType v, ParameterType min, ParameterType max)
	:ofReadOnlyParameter<ParameterType,ofAbstractParameter>(name,v,min,max){};

	ParameterType operator=(ParameterType v){
		return ofReadOnlyParameter<ParameterType,ofAbstractParameter>::operator=(v);
	}

	void setValue(ParameterType v){
		ofReadOnlyParameter<ParameterType,ofAbstractParameter>::setValue(v);
	}

	ofParameter<ParameterType> & set(string name, ParameterType value){
		return (ofParameter<ParameterType>&)ofReadOnlyParameter<ParameterType,ofAbstractParameter>::set(name,value);
	}

	ofParameter<ParameterType> & set(string name, ParameterType value, ParameterType min, ParameterType max){
		return (ofParameter<ParameterType>&)ofReadOnlyParameter<ParameterType,ofAbstractParameter>::set(name,value,min,max);
	}

	void setName(string name){
		ofReadOnlyParameter<ParameterType,ofAbstractParameter>::setName(name);
	}

	void setMin(ParameterType min){
		ofReadOnlyParameter<ParameterType,ofAbstractParameter>::setMin(min);
	}

	void setMax(ParameterType max){
		ofReadOnlyParameter<ParameterType,ofAbstractParameter>::setMax(max);
	}

	void fromString(string str){
		ofReadOnlyParameter<ParameterType,ofAbstractParameter>::fromString(str);
	}

	operator const ParameterType & () const {
		return ofReadOnlyParameter<ParameterType,ofAbstractParameter>::obj->value;
	};
};
