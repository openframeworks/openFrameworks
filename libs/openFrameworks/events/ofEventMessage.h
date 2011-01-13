#pragma once

#include "ofConstants.h"

class ofEventMessage{
public:
	string name;

	template<class C>
	void addParameter(const C & param){
		parameters.push_back(ofToString(param));
	}

private:
	vector<string> parameters;
};
