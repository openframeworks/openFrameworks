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

#include <string>
#include "ofMain.h"

/*
OSC 1.1 specifications types:
          i - 32bit integer
          h - 64bit integer
          f - 32bit floating point number
          d - 64bit (double) floating point number
          s - string
          S - symbol
          c - char
          m - 4 byte midi packet (8 digits hexadecimal)
          T - TRUE (no value required)
          F - FALSE (no value required)
          N - NIL (no value required)
          I - IMPULSE, act as a trigger (no value required), previously named INFINITUM
          t - TIMETAG, an OSC timetag in NTP format, encoded in the data section

See: http://cnmat.berkeley.edu/system/files/attachments/Nime09OSCfinal.pdf
See also: https://code.google.com/p/oscpack/source/browse/trunk/osc/OscTypes.h#113
*/
typedef enum _ofxOscArgType
{
	OFXOSC_TYPE_INT32 = 'i',
	OFXOSC_TYPE_INT64 = 'h',
	OFXOSC_TYPE_FLOAT = 'f',
	OFXOSC_TYPE_DOUBLE = 'd',
	OFXOSC_TYPE_STRING = 's',
	OFXOSC_TYPE_SYMBOL = 'S',
	OFXOSC_TYPE_CHAR = 'c',
	OFXOSC_TYPE_MIDI_MESSAGE = 'm',
	OFXOSC_TYPE_TRUE = 'T',
	OFXOSC_TYPE_FALSE = 'F',
	OFXOSC_TYPE_NONE = 'N',
	OFXOSC_TYPE_TRIGGER = 'I',
	OFXOSC_TYPE_TIMETAG = 't',
	OFXOSC_TYPE_BLOB = 'b',
	OFXOSC_TYPE_BUNDLE = 'B',
	OFXOSC_TYPE_RGBA_COLOR = 'r',
	OFXOSC_TYPE_INDEXOUTOFBOUNDS = 0
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

	virtual ofxOscArgType getType() { return OFXOSC_TYPE_NONE; };
	virtual std::string getTypeName() { return "N"; };

private:
};


/*

subclasses for each possible argument type

*/

class ofxOscArgInt32 : public ofxOscArg
{
public:
	ofxOscArgInt32( std::int32_t _value ) { value = _value; };
	~ofxOscArgInt32() {};

	/// return the type of this argument
	ofxOscArgType getType() { return OFXOSC_TYPE_INT32; };
	std::string getTypeName() { return "i"; };

	/// return value
	std::int32_t get() const { return value; };
	/// set value
	void set( std::int32_t _value ) { value = _value; };

private:
	std::int32_t value;
};

class ofxOscArgInt : public ofxOscArgInt32
{
public:
	ofxOscArgInt( std::int32_t _value ) : ofxOscArgInt32(_value) {};
	~ofxOscArgInt(){};
};

class ofxOscArgInt64 : public ofxOscArg
{
public:
	ofxOscArgInt64( std::int64_t _value ) { value = _value; };
	~ofxOscArgInt64() {};

	/// return the type of this argument
	ofxOscArgType getType() { return OFXOSC_TYPE_INT64; };
	std::string getTypeName() { return "h"; };

	/// return value
	std::int64_t get() const { return value; };
	/// set value
	void set( std::int64_t _value ) { value = _value; };

private:
    std::int64_t value;
};

class ofxOscArgFloat : public ofxOscArg
{
public:
	ofxOscArgFloat( float _value ) { value = _value; };
	~ofxOscArgFloat() {};

	/// return the type of this argument
	ofxOscArgType getType() { return OFXOSC_TYPE_FLOAT; };
    std::string getTypeName() { return "f"; };

	/// return value
	float get() const { return value; };
	/// set value
	void set( float _value ) { value = _value; };

private:
	float value;
};

class ofxOscArgDouble : public ofxOscArg
{
public:
	ofxOscArgDouble( double _value ) { value = _value; };
	~ofxOscArgDouble() {};

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_DOUBLE;};
	std::string getTypeName() {return "d";};

	/// return value
	double get() const { return value; };
	/// set value
	void set( double _value ) { value = _value; };

private:
	double value;
};

class ofxOscArgString : public ofxOscArg
{
public:
	ofxOscArgString( const std::string &_value ) { value = _value; };
	~ofxOscArgString() {};

	/// return the type of this argument
	ofxOscArgType getType() { return OFXOSC_TYPE_STRING; };
	std::string getTypeName() { return "s"; };

	/// return value
	const std::string &get() const { return value; }
	/// set value
	void set( const char* _value ) { value = _value; };
	void set( const std::string &_value ) {value = _value; };

private:
	std::string value;
};

class ofxOscArgSymbol : public ofxOscArgString
{
public:
	ofxOscArgSymbol( const std::string &_value ) : ofxOscArgString(_value){};
	~ofxOscArgSymbol() {};

	/// return the type of this argument
	ofxOscArgType getType() { return OFXOSC_TYPE_SYMBOL; };
	std::string getTypeName() { return "S"; };
};

class ofxOscArgChar : public ofxOscArg
{
public:
	ofxOscArgChar( char _value ) { value = _value; };
	~ofxOscArgChar() {};

	/// return the type of this argument
	ofxOscArgType getType() { return OFXOSC_TYPE_CHAR; };
	std::string getTypeName() { return "c"; };

	/// return value
	char get() const { return value; };
	/// set value
	void set( char _value ) { value = _value; };

private:
	char value;
};

class ofxOscArgMidiMessage : public ofxOscArgInt32
{
public:
    ofxOscArgMidiMessage( std::int32_t _value ) : ofxOscArgInt32(_value) {};
	~ofxOscArgMidiMessage() {};

	/// return the type of this argument
	ofxOscArgType getType() { return OFXOSC_TYPE_MIDI_MESSAGE; };
	std::string getTypeName() { return "m"; };
};

class ofxOscArgBool : public ofxOscArg
{
public:
	ofxOscArgBool( bool _value ) { value = _value; };
	~ofxOscArgBool() {};

	/// return the type of this argument
	ofxOscArgType getType() {
		if(value)
			return OFXOSC_TYPE_TRUE;
		else
			return OFXOSC_TYPE_FALSE;
	};
	std::string getTypeName() {
		if(value)
			return "T";
		else
			return "F";
	};

	/// return value
	bool get() const { return value; };
	/// set value
	void set( bool _value ) { value = _value; };

private:
	bool value;
};

class ofxOscArgTrigger : public ofxOscArgBool
{
public:
	ofxOscArgTrigger() : ofxOscArgBool(true) {};
    ~ofxOscArgTrigger(){};

	/// return the type of this argument
	ofxOscArgType getType() { return OFXOSC_TYPE_TRIGGER; };
	std::string getTypeName() { return "I"; };
};

class ofxOscArgTimetag : public ofxOscArgInt64
{
public:
    ofxOscArgTimetag( std::int64_t _value ) : ofxOscArgInt64(_value) {};
	~ofxOscArgTimetag() {};

	/// return the type of this argument
	ofxOscArgType getType() { return OFXOSC_TYPE_TIMETAG; };
	std::string getTypeName() { return "t"; };
};

class ofxOscArgBlob : public ofxOscArg
{
public:
	ofxOscArgBlob( const ofBuffer &_value ){
        value = _value;
    }
    ~ofxOscArgBlob(){};

	/// return the type of this argument
	ofxOscArgType getType() { return OFXOSC_TYPE_BLOB; };
	std::string getTypeName() { return "b"; };

	/// return value
	const ofBuffer &get() const { return value; };
	/// set value
	void set( const char * _value, unsigned int length ) { value.set(_value, length); };

private:
	ofBuffer value;
};

class ofxOscArgRgbaColor : public ofxOscArg
{
public:
    ofxOscArgRgbaColor( std::int32_t _value ) { value = _value; };
	~ofxOscArgRgbaColor() {};

	/// return the type of this argument
	ofxOscArgType getType() { return OFXOSC_TYPE_RGBA_COLOR; };
	std::string getTypeName() { return "r"; };

	/// return value
    std::int32_t get() const { return value; };
	/// set value
    void set( std::int32_t _value ) { value = _value; };

private:
    std::int32_t value;
};

