// copyright (c) openFrameworks team 2010-2017
// copyright (c) damian stewart 2007-2009
#pragma once

#include "ofxOscArg.h"

/// \class ofxOscMessage
/// \brief an OSC message with address and arguments
class ofxOscMessage{
public:

	ofxOscMessage();
	~ofxOscMessage();
	ofxOscMessage(const ofxOscMessage& other);
	ofxOscMessage& operator=(const ofxOscMessage& other);
	/// for operator= and copy constructor
	ofxOscMessage& copy(const ofxOscMessage& other);

	/// clear this message
	void clear();

	/// set the message address, must start with a /
	void setAddress(const string &_address);

	/// \return the OSC address
	string getAddress() const;

	/// \return the remote host name/ip (deprecated)
	OF_DEPRECATED_MSG("Use getRemoteHost() instead", string getRemoteIp() const);
	
	/// \return the remote host name/ip or "" if not set
	string getRemoteHost() const;
	
	/// \return the remote port or 0 if not set
	int getRemotePort() const;
	
	/// \section Argument Getters
	///
	/// get the argument with the given index as an int, float, string, etc
	///
	/// some types can be automatically converted to a requested type,
	/// (ie. int to float) however it is best to ensure that the type matches
	/// what you're requesting:
	///
	///     int i = 0;
	///     if(message.getArgType(index) == OFXOSC_TYPE_INT32) {
	///         i = message.getArgAsInt32(index);
	///     }
	///
	/// or use the type tag char:
	///
	///		int i = 0;
	///     if(message.getArgTypeName(index) == "i") {
	///         i = message.getArgAsInt32(index);
	///     }
	///
	/// see ofxOscArg.h for argument type tag char values

	/// \return number of arguments
	int getNumArgs() const;
	
	/// \return argument type code for a given index
	ofxOscArgType getArgType(int index) const;
	
	/// \return argument type tag char as a string
	string getArgTypeName(int index) const;
	
	/// get argument as an integer, converts numeric types automatically
	/// prints a warning when converting higher precision types
	/// \return given argument value as a 32-bit int
	int32_t getArgAsInt(int index) const;
	
	/// get argument as an integer, converts numeric types automatically
	/// prints a warning when converting higher precision types
	/// \return given argument value as a 32-bit int
	int32_t getArgAsInt32(int index) const;
	
	/// get argument as a 64-bit integer, converts numeric types automatically
	/// \return given argument value as a 64-bit int
	int64_t getArgAsInt64(int index) const;
	
	/// get argument as a float, converts numeric types automatically
	/// prints a warning when converting higher precision types
	/// \return given argument value as a float
	float getArgAsFloat(int index) const;
	
	/// get argument as a double, converts numeric types automatically
	/// \return given argument value as a double
	double getArgAsDouble(int index) const;
	
	/// get argument as a string, converts numeric types with a warning
	/// \return given argument value as a string
	string getArgAsString(int index) const;
	
	/// get argument as a symbol (string), converts numeric types with a warning
	/// \return given argument value as a symbol (string)
	string getArgAsSymbol(int index) const;
	
	/// get argument as a string, converts numeric types with a warning
	/// \return given argument value as a string
	char getArgAsChar(int index) const;
	
	/// \return given argument value as a 4-byte midi message
	int32_t getArgAsMidiMessage(int index) const;
	
	/// get argument as a bool, converts numeric types automatically
	/// this argument type matches both OFXOSC_TYPE_TRUE & OFXOSC_TYPE_FALSE
	/// \return given argument value as a bool
	bool getArgAsBool(int index) const;
	
	/// get argument as a none/nil
	/// \return true if argument was a none/nil
	bool getArgAsNone(int index) const;
	
	/// get argument as a trigger impulse
	/// \return true if argument was a trigger
	bool getArgAsTrigger(int index) const;
	
	/// get argument as a trigger impulse
	/// this is an alias for getArgAsTrigger()
	/// \return true if argument was a trigger
	bool getArgAsImpulse(int index) const;
	
	/// get argument as a trigger impulse
	/// this is an alias for getArgAsTrigger()
	/// \return true if argument was a trigger
	bool getArgAsInfinitum(int index) const;
	
	/// \return given argument as a 64-bit NTP time tag
	int64_t getArgAsTimetag(int index) const;
	
	/// \return given argument as a binary blob
	ofBuffer getArgAsBlob(int index) const;
	
	/// get an argument as an RGBA color, converts int32 automatically
	/// \return given argument as a 32-bit color value
	int32_t getArgAsRgbaColor(int index) const;

	/// \section Argument Setters

	/// add a 32-bit integer
	void addIntArg(int32_t argument);
	
	/// add a 32-bit integer
	void addInt32Arg(int32_t argument);
	
	/// add a 64-bit integer
	void addInt64Arg(int64_t argument);
	
	/// add a 32-bit float
	void addFloatArg(float argument);
	
	/// add a 64-bit double
	void addDoubleArg(double argument);
	
	/// add a string
	void addStringArg(const string &argument);
	
	/// add a symbol (string)
	void addSymbolArg(const string &argument);
	
	/// add a char
	void addCharArg(char argument);
	
	/// add a 4-byte MIDI message
	void addMidiMessageArg(int32_t argument);
	
	/// add a bool
	/// true sends a OFXOSC_TYPE_TRUE & false sends an OFXOSC_TYPE_FALSE
	void addBoolArg(bool argument);
	
	/// add a none/nil (has no value)
	void addNoneArg();
	
	/// add a trigger impulse (has no value)
	void addTriggerArg();
	
	/// add a trigger impulse (has no value)
	/// alias for addTriggerArg()
	void addImpulseArg();
	
	/// add a trigger impulse (has no value)
	/// alias for addTriggerArg()
	void addInfinitumArg();
	
	/// add a 64-bt NTP time tag
	void addTimetagArg(int64_t argument);
	
	/// add a binary blog
	void addBlobArg(const ofBuffer &argument);
	
	/// add a 32-bit color
	void addRgbaColorArg(int32_t argument);

	/// set host and port of the remote endpoint,
	/// this is mainly used by ofxOscReceiver
	void setRemoteEndpoint(const string &host, int port);

private:

	string address; //< OSC address, must start with a /
	vector<ofxOscArg*> args; //< current arguments

	string remoteHost; //< host name/ip the message was sent from
	int remotePort; //< port the message was sent from
};
