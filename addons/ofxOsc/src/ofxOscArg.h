/*

 Copyright 2007, 2008 Damian Stewart damian@frey.co.nz
 Distributed under the terms of the GNU Lesser General Public License v3

 This file is part of the ofxOsc openFrameworks OSC addon.

 ofxOsc is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 ofxOsc is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with ofxOsc.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _OFXOSCARG_H
#define _OFXOSCARG_H

#include <ofConstants.h>
#include <string>

typedef enum _ofxOscArgType
{
	OFXOSC_TYPE_NONE,
	OFXOSC_TYPE_INT32,
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

#if defined TARGET_WIN32 && defined _MSC_VER
// required because MSVC isn't ANSI-C compliant
typedef long int32_t;
#endif

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

#endif
