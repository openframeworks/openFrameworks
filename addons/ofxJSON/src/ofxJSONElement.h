/*
 *  ofxJSONFile.h
 *  asift
 *
 *  Created by Jeffrey Crouse on 12/17/10.
 *  Copyright 2010 Eyebeam. All rights reserved.
 *
 */

#ifndef _OFX_JSON
#define _OFX_JSON

#include <json/json.h>
#include "ofMain.h"

class ofxJSONElement: public ofBaseFileSerializer, public Json::Value {
public:
	ofxJSONElement() {};
	ofxJSONElement(string jsonString);
	ofxJSONElement(const Json::Value& v);
	ofxJSONElement(Json::ValueType t);
	
	bool parse(string jsonString);
	bool load(string filename);
	bool openLocal(string filename);
	bool openRemote(string filename, bool secure=false);
	bool save(string path);
	bool save(string filename, bool pretty);
	string getRawString(bool pretty=true);

	Json::Value & getJsonValue();

	void serialize(const ofAbstractParameter & parameter);
	void deserialize(ofAbstractParameter & parameter);

};

#endif
