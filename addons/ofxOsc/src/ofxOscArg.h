// copyright (c) openFrameworks team 2010-2017
// copyright (c) damian stewart 2007-2009
#pragma once

#include "ofConstants.h"
#include "ofFileUtils.h"

/// OSC 1.1 specification argument type enum values:
///   i - 32bit integer
///   h - 64bit integer
///   f - 32bit floating point number
///   d - 64bit (double) floating point number
///   s - string
///   S - symbol
///   c - char
///   m - 4 byte midi packet (8 digits hexadecimal)
///   T - TRUE (no value required)
///   F - FALSE (no value required)
///   N - NIL (no value required)
///   I - impulse TRIGGER, act as an trigger (no value required), aka IMPULSE &  INFINITUM
///   t - TIMETAG, an OSC timetag in NTP format, encoded in the data section
/// See: http://cnmat.berkeley.edu/system/files/attachments/Nime09OSCfinal.pdf
typedef enum _ofxOscArgType{
	OFXOSC_TYPE_INT32            = 'i',
	OFXOSC_TYPE_INT64            = 'h',
	OFXOSC_TYPE_FLOAT            = 'f',
	OFXOSC_TYPE_DOUBLE           = 'd',
	OFXOSC_TYPE_STRING           = 's',
	OFXOSC_TYPE_SYMBOL           = 'S',
	OFXOSC_TYPE_CHAR             = 'c',
	OFXOSC_TYPE_MIDI_MESSAGE     = 'm',
	OFXOSC_TYPE_TRUE             = 'T',
	OFXOSC_TYPE_FALSE            = 'F',
	OFXOSC_TYPE_NONE             = 'N',
	OFXOSC_TYPE_TRIGGER          = 'I',
	OFXOSC_TYPE_TIMETAG          = 't',
	OFXOSC_TYPE_BLOB             = 'b',
	OFXOSC_TYPE_BUNDLE           = 'B',
	OFXOSC_TYPE_RGBA_COLOR       = 'r',
	OFXOSC_TYPE_INDEXOUTOFBOUNDS = 0    //< bad index value
} ofxOscArgType;

/// \class ofxOscArg
/// \brief base class for arguments
class ofxOscArg{
public:
	ofxOscArg() {};
	virtual ~ofxOscArg() {};

	virtual ofxOscArgType getType() {return OFXOSC_TYPE_NONE;}
	virtual string getTypeName() {return "N";}
};

/// \class ofxOscArgInt32
/// \brief a 32-bit integer argument, type name "i"
class ofxOscArgInt32 : public ofxOscArg{
public:
	ofxOscArgInt32(int32_t value) : value(value) {};
	~ofxOscArgInt32() {};

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_INT32;}
	string getTypeName() {return "i";}

	/// return value
	int32_t get() const {return value;}
	/// set value
	void set(int32_t value) {this->value = value;};

private:
	int32_t value;
};

/// \class ofxOscArgInt
/// \brief a 32-bit integer argument, type name "i"
class ofxOscArgInt : public ofxOscArgInt32{
public:
	ofxOscArgInt(int32_t value) : ofxOscArgInt32(value) {};
	~ofxOscArgInt() {};
};

/// \class ofxOscArgInt64
/// \brief a 64-bit integer argument, type name "h"
class ofxOscArgInt64 : public ofxOscArg{
public:
	ofxOscArgInt64(int64_t value) : value(value) {};
	~ofxOscArgInt64() {};

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_INT64;}
	string getTypeName() {return "h";}

	/// return value
	int64_t get() const {return value;}
	/// set value
	void set(int64_t value) {this->value = value;}

private:
	int64_t value;
};

/// \class ofxOscArgFloat
/// \brief a 32-bit float argument, type name "f"
class ofxOscArgFloat : public ofxOscArg{
public:
	ofxOscArgFloat(float value) : value(value) {};
	~ofxOscArgFloat() {};

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_FLOAT;}
	string getTypeName() {return "f";}

	/// return value
	float get() const {return value;}
	/// set value
	void set(float value) {this->value = value;}

private:
	float value;
};

/// \class ofxOscArgDouble
/// \brief a 64-bit double argument, type name "d"
class ofxOscArgDouble : public ofxOscArg{
public:
	ofxOscArgDouble(double value) : value(value) {};
	~ofxOscArgDouble() {};

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_DOUBLE;}
	string getTypeName() {return "d";}

	/// return value
	double get() const {return value;}
	/// set value
	void set(double value) {this->value = value;}

private:
	double value;
};

/// \class ofxOscArgString
/// \brief a null-terminated string argument, type name "s"
class ofxOscArgString : public ofxOscArg{
public:
	ofxOscArgString(const string &value ) : value(value) {};
	~ofxOscArgString() {};

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_STRING;}
	string getTypeName() {return "s";}

	/// return value
	const string &get() const {return value;}
	/// set value
	void set(const char* value) {this->value = value;}
	void set(const string &value) {this->value = value;}

private:
	string value;
};

/// \class ofxOscArgSymbol
/// \brief a null-terminated symbol (string) argument, type name "S"
class ofxOscArgSymbol : public ofxOscArgString{
public:
	ofxOscArgSymbol(const string &value) : ofxOscArgString(value) {};
	~ofxOscArgSymbol() {};

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_SYMBOL;}
	string getTypeName() {return "S";}
};

/// \class ofxOscArgChar
/// \brief a null-terminated char argument, type name "c"
class ofxOscArgChar : public ofxOscArg{
public:
	ofxOscArgChar(char _value) : value(value) {};
	~ofxOscArgChar() {};

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_CHAR;}
	string getTypeName() {return "c";}

	/// return value
	char get() const {return value;}
	/// set value
	void set(char value) {this->value = value;}

private:
	char value;
};

/// \class ofxOscArgMidiMessage
/// \brief a 4-byte MIDI message argument, type name "m"
class ofxOscArgMidiMessage : public ofxOscArgInt32{
public:
	ofxOscArgMidiMessage(int32_t value) : ofxOscArgInt32(value) {};
	~ofxOscArgMidiMessage() {};

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_MIDI_MESSAGE;}
	string getTypeName() {return "m";}
};

/// \class ofxOscArgBool
/// \brief a boolean argument, either type name "T" or "F" based on value
class ofxOscArgBool : public ofxOscArg{
public:
	ofxOscArgBool(bool value) : value(value) {};
	~ofxOscArgBool() {};

	/// return the type of this argument
	ofxOscArgType getType() {
		return value ? OFXOSC_TYPE_TRUE : OFXOSC_TYPE_FALSE;
	}
	string getTypeName() {
		return value ? "T" : "F";
	}

	/// return value
	bool get() const {return value;}
	/// set value
	void set(bool value) {this->value = value;}

private:
	bool value;
};

/// \class ofxOscArgTrigger
/// \brief a trigger impulse, type name "I"
class ofxOscArgTrigger : public ofxOscArgBool{
public:
	ofxOscArgTrigger() : ofxOscArgBool(true) {};
	~ofxOscArgTrigger() {};

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_TRIGGER;}
	string getTypeName() {return "I";}
};

/// \class ofxOscArgTimetag
/// \brief a 64-bit NTP time tag argument, type name "t"
class ofxOscArgTimetag : public ofxOscArgInt64{
public:
	ofxOscArgTimetag(int64_t value) : ofxOscArgInt64(value) {};
	~ofxOscArgTimetag() {};

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_TIMETAG;}
	string getTypeName() {return "t";}
};

/// \class ofxOscArgBlob
/// \brief a binary blob argument, type name "b"
class ofxOscArgBlob : public ofxOscArg{
public:
	ofxOscArgBlob(const ofBuffer &value) : value(value){}
	~ofxOscArgBlob() {};

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_BLOB;}
	string getTypeName() {return "b";}

	/// return value
	const ofBuffer &get() const {return value;}
	/// set value
	void set(const char * value, unsigned int length) {
		this->value.set(value, length);
	}

private:
	ofBuffer value;
};

/// \class ofxOscArgRgbaColor
/// \brief a 32-bit RGBA color argument, type name "r"
class ofxOscArgRgbaColor : public ofxOscArg{
public:
	ofxOscArgRgbaColor(int32_t value) : value(value) {};
	~ofxOscArgRgbaColor() {};

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_RGBA_COLOR;}
	string getTypeName() {return "r";}

	/// return value
	int32_t get() const {return value;}
	/// set value
	void set(int32_t value) {this->value = value;}

private:
	int32_t value;
};
