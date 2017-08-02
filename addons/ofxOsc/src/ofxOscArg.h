// copyright (c) openFrameworks team 2010-2017
// copyright (c) Damian Stewart 2007-2009
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
///   I - impulse TRIGGER (no value required), aka IMPULSE & INFINITUM
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
	OFXOSC_TYPE_INDEXOUTOFBOUNDS = 0 //< bad index value
} ofxOscArgType;

/// \class ofxOscArg
/// \brief base class for arguments
class ofxOscArg{
public:
	virtual ~ofxOscArg() {}
	virtual ofxOscArgType getType() {return OFXOSC_TYPE_NONE;}
	virtual std::string getTypeName() {return "N";}
};

/// \class ofxOscArgInt32
/// \brief a 32-bit integer argument, type name "i"
class ofxOscArgInt32 : public ofxOscArg{
public:
	ofxOscArgInt32(std::int32_t value) : value(value) {}

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_INT32;}
	std::string getTypeName() {return "i";}

	/// return value
	std::int32_t get() const {return value;}
	/// set value
	void set(std::int32_t value) {this->value = value;}

private:
	std::int32_t value;
};

/// \class ofxOscArgInt
/// \brief a 32-bit integer argument, type name "i"
class ofxOscArgInt : public ofxOscArgInt32{
public:
	ofxOscArgInt(std::int32_t value) : ofxOscArgInt32(value) {}
};

/// \class ofxOscArgInt64
/// \brief a 64-bit integer argument, type name "h"
class ofxOscArgInt64 : public ofxOscArg{
public:
	ofxOscArgInt64(std::int64_t value) : value(value) {}

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_INT64;}
	std::string getTypeName() {return "h";}

	/// return value
	std::int64_t get() const {return value;}
	/// set value
	void set(std::int64_t value) {this->value = value;}

private:
	std::int64_t value;
};

/// \class ofxOscArgFloat
/// \brief a 32-bit float argument, type name "f"
class ofxOscArgFloat : public ofxOscArg{
public:
	ofxOscArgFloat(float value) : value(value) {}

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_FLOAT;}
	std::string getTypeName() {return "f";}

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
	ofxOscArgDouble(double value) : value(value) {}

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_DOUBLE;}
	std::string getTypeName() {return "d";}

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
	ofxOscArgString(const std::string &value ) : value(value) {}

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_STRING;}
	std::string getTypeName() {return "s";}

	/// return value
	const std::string &get() const {return value;}
	/// set value
	void set(const char *value) {this->value = value;}
	void set(const std::string &value) {this->value = value;}

private:
	std::string value;
};

/// \class ofxOscArgSymbol
/// \brief a null-terminated symbol (string) argument, type name "S"
class ofxOscArgSymbol : public ofxOscArgString{
public:
	ofxOscArgSymbol(const std::string &value) : ofxOscArgString(value) {}

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_SYMBOL;}
	std::string getTypeName() {return "S";}
};

/// \class ofxOscArgChar
/// \brief a null-terminated char argument, type name "c"
class ofxOscArgChar : public ofxOscArg{
public:
	ofxOscArgChar(char value) : value(value) {}

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_CHAR;}
	std::string getTypeName() {return "c";}

	/// return value
	char get() const {return value;}
	/// set value
	void set(char value) {this->value = value;}

private:
	char value;
};

/// \class ofxOscArgMidiMessage
/// \brief a 4-byte MIDI message argument, type name "m"
class ofxOscArgMidiMessage : public ofxOscArg{
public:
	ofxOscArgMidiMessage(std::uint32_t value) : value(value) {}

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_MIDI_MESSAGE;}
	std::string getTypeName() {return "m";}
	
	/// return value
	std::uint32_t get() const {return value;}
	/// set value
	void set(std::uint32_t value) {this->value = value;}

private:
	std::uint32_t value;
};

/// \class ofxOscArgBool
/// \brief a boolean argument, either type name "T" or "F" based on value
class ofxOscArgBool : public ofxOscArg{
public:
	ofxOscArgBool(bool value) : value(value) {}

	/// return the type of this argument
	ofxOscArgType getType() {
		return value ? OFXOSC_TYPE_TRUE : OFXOSC_TYPE_FALSE;
	}
	std::string getTypeName() {
		return value ? "T" : "F";
	}

	/// return value
	bool get() const {return value;}
	/// set value
	void set(bool value) {this->value = value;}

private:
	bool value;
};

/// \class ofxOscArgNone
/// \brief a none/nil (has no value), type name "N"
class ofxOscArgNone : public ofxOscArgBool{
public:
	ofxOscArgNone() : ofxOscArgBool(true) {}

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_NONE;}
	std::string getTypeName() {return "N";}
};

/// \class ofxOscArgTrigger
/// \brief a trigger impulse (has no value), type name "I"
class ofxOscArgTrigger : public ofxOscArgBool{
public:
	ofxOscArgTrigger() : ofxOscArgBool(true) {}

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_TRIGGER;}
	std::string getTypeName() {return "I";}
};

/// \class ofxOscArgTimetag
/// \brief a 64-bit NTP time tag argument, type name "t"
class ofxOscArgTimetag : public ofxOscArg{
public:
	ofxOscArgTimetag(std::uint64_t value) : value(value) {}

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_TIMETAG;}
	std::string getTypeName() {return "t";}
	
	/// return value
	std::uint64_t get() const {return value;}
	/// set value
	void set(std::uint64_t value) {this->value = value;}

private:
	std::uint64_t value;
};

/// \class ofxOscArgBlob
/// \brief a binary blob argument, type name "b"
class ofxOscArgBlob : public ofxOscArg{
public:
	ofxOscArgBlob(const ofBuffer &value) : value(value) {}

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_BLOB;}
	std::string getTypeName() {return "b";}

	/// return value
	const ofBuffer &get() const {return value;}
	/// set value
	void set(const char *value, unsigned int length) {
		this->value.set(value, length);
	}

private:
	ofBuffer value;
};

/// \class ofxOscArgRgbaColor
/// \brief a 32-bit RGBA color argument, type name "r"
class ofxOscArgRgbaColor : public ofxOscArgMidiMessage{
public:
	ofxOscArgRgbaColor(std::uint32_t value) : ofxOscArgMidiMessage(value) {}

	/// return the type of this argument
	ofxOscArgType getType() {return OFXOSC_TYPE_RGBA_COLOR;}
	std::string getTypeName() {return "r";}
};
