/*
 
 Copyright (c) 2007-2009, Damian Stewart
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * Neither the name of the developer nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY DAMIAN STEWART ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL DAMIAN STEWART BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "ofConstants.h"
#include <string>

typedef enum _ofxOscArgType
{
	OFXOSC_TYPE_NONE,
	OFXOSC_TYPE_INT32,
	OFXOSC_TYPE_INT64,
	OFXOSC_TYPE_FLOAT,
	OFXOSC_TYPE_STRING,
	OFXOSC_TYPE_BLOB,
	OFXOSC_TYPE_BUNDLE,
	OFXOSC_TYPE_INDEXOUTOFBOUNDS
} ofxOscArgType;

/*

ofxOscArg

base class for arguments

*/

class ofxOscArg
{
public:
	ofxOscArg() {};
	virtual ~ofxOscArg() {};

	virtual ofxOscArgType getType() { return OFXOSC_TYPE_NONE; }
	virtual string getTypeName() { return "none"; }

private:
};


/*

subclasses for each possible argument type

*/

class ofxOscArgInt32 : public ofxOscArg
{
public:
	ofxOscArgInt32( int32_t _value ) { value = _value; }
	~ofxOscArgInt32() {};

	/// return the type of this argument
	ofxOscArgType getType() { return OFXOSC_TYPE_INT32; }
	string getTypeName() { return "int32"; }

	/// return value
	int32_t get() const { return value; }
	/// set value
	void set( int32_t _value ) { value = _value; }

private:
	int32_t value;
};

class ofxOscArgInt64 : public ofxOscArg
{
public:
	ofxOscArgInt64( uint64_t _value ) { value = _value; }
	~ofxOscArgInt64() {};

	/// return the type of this argument
	ofxOscArgType getType() { return OFXOSC_TYPE_INT64; }
	string getTypeName() { return "int64"; }

	/// return value
	uint64_t get() const { return value; }
	/// set value
	void set( uint64_t _value ) { value = _value; }

private:
	uint64_t value;
};

class ofxOscArgFloat : public ofxOscArg
{
public:
	ofxOscArgFloat( float _value ) { value = _value; }
	~ofxOscArgFloat() {};

	/// return the type of this argument
	ofxOscArgType getType() { return OFXOSC_TYPE_FLOAT; }
	string getTypeName() { return "float"; }

	/// return value
	float get() const { return value; }
	/// set value
	void set( float _value ) { value = _value; }

private:
		float value;
};

class ofxOscArgString : public ofxOscArg
{
public:
	ofxOscArgString( string _value ) { value = _value; }
	~ofxOscArgString() {};

	/// return the type of this argument
	ofxOscArgType getType() { return OFXOSC_TYPE_STRING; }
	string getTypeName() { return "string"; }

	/// return value
	string get() const { return value; }
	/// set value
	void set( const char* _value ) { value = _value; }

private:
	std::string value;
};
