// copyright (c) openFrameworks team 2010-2023
// copyright (c) Damian Stewart 2007-2009
#pragma once

#include "ofxOscArg.h"

/// \class ofxOscMessage
/// \brief an OSC message with address and arguments
class ofxOscMessage {
public:
	ofxOscMessage();
	ofxOscMessage(std::string address)
		: ofxOscMessage() {
		setAddress(address);
	}
	~ofxOscMessage();
	ofxOscMessage(const ofxOscMessage & other);
	ofxOscMessage(std::string & address);
	ofxOscMessage & operator=(const ofxOscMessage & other);
	/// for operator= and copy constructor
	ofxOscMessage & copy(const ofxOscMessage & other);

	/// clear this message
	void clear();

	/// set the message address, must start with a /
	void setAddress(const std::string & address);

	/// \return the OSC address
	std::string getAddress() const;

	[[deprecated("Use getRemoteHost()")]]
	std::string getRemoteIp() const;

	/// \return the remote host name/ip or "" if not set
	std::string getRemoteHost() const;

	/// \return the remote port or 0 if not set
	int getRemotePort() const;

	/// \section Argument Getters
	///
	/// get the argument with the given index as an int, float, string, etc
	///
	/// some types can be automatically converted to a requested type,
	/// (ie. int to float) however it is best to ensure that the type matches
	/// what you are requesting:
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
	/// you can also check against the type string for all arguments:
	///
	///     int i = 0; float f = 0.0; std::string s = "";
	///     if(message.getTypeString() == "ifs") {
	///         i = message.getArgAsInt32(0);
	///         f = message.getArgAsFloat(1);
	///         s = message.getArgAsString(2);
	///     }
	///
	/// see ofxOscArg.h for argument type tag char values

	/// \return number of arguments
	std::size_t getNumArgs() const;

	/// \param index The index of the queried item.
	/// \return argument type code for a given index
	ofxOscArgType getArgType(std::size_t index) const;

	/// \param index The index of the queried item.
	/// \return argument type tag char as a string
	std::string getArgTypeName(std::size_t index) const;

	/// \param index The index of the queried item.
	/// \return type tags for all arguments as a string, 1 char for each argument
	std::string getTypeString() const;

	/// get argument as an integer, converts numeric types automatically
	/// prints a warning when converting higher precision types
	/// \param index The index of the queried item.
	/// \return given argument value as a 32-bit int
	std::int32_t getArgAsInt(std::size_t index) const;

	/// get argument as an integer, converts numeric types automatically
	/// prints a warning when converting higher precision types
	/// \param index The index of the queried item.
	/// \return given argument value as a 32-bit int
	std::int32_t getArgAsInt32(std::size_t index) const;

	/// get argument as a 64-bit integer, converts numeric types automatically
	/// \return given argument value as a 64-bit int
	/// \param index The index of the queried item.
	std::int64_t getArgAsInt64(std::size_t index) const;

	/// get argument as a float, converts numeric types automatically
	/// prints a warning when converting higher precision types
	/// \param index The index of the queried item.
	/// \return given argument value as a float
	float getArgAsFloat(std::size_t index) const;

	/// get argument as a double, converts numeric types automatically
	/// \param index The index of the queried item.
	/// \return given argument value as a double
	double getArgAsDouble(std::size_t index) const;

	/// get argument as a string, converts numeric types with a warning
	/// \param index The index of the queried item.
	/// \return given argument value as a string
	std::string getArgAsString(std::size_t index) const;

	/// get argument as a symbol (string), converts numeric types with a warning
	/// \param index The index of the queried item.
	/// \return given argument value as a symbol (string)
	std::string getArgAsSymbol(std::size_t index) const;

	/// get argument as a string, converts numeric types with a warning
	/// \param index The index of the queried item.
	/// \return given argument value as a string
	char getArgAsChar(std::size_t index) const;

	/// \param index The index of the queried item.
	/// \return given argument value as a 4-byte midi message
	std::uint32_t getArgAsMidiMessage(std::size_t index) const;

	/// get argument as a bool, converts numeric types automatically
	/// this argument type matches both OFXOSC_TYPE_TRUE & OFXOSC_TYPE_FALSE
	/// \param index The index of the queried item.
	/// \return given argument value as a bool
	bool getArgAsBool(std::size_t index) const;

	/// get argument as a none/nil
	/// \param index The index of the queried item.
	/// \return true if argument was a none/nil
	bool getArgAsNone(std::size_t index) const;

	/// get argument as a trigger impulse
	/// \param index The index of the queried item.
	/// \return true if argument was a trigger
	bool getArgAsTrigger(std::size_t index) const;

	/// get argument as a trigger impulse
	/// this is an alias for getArgAsTrigger()
	/// \param index The index of the queried item.
	/// \return true if argument was a trigger
	bool getArgAsImpulse(std::size_t index) const;

	/// get argument as a trigger impulse
	/// this is an alias for getArgAsTrigger()
	/// \param index The index of the queried item.
	/// \return true if argument was a trigger
	bool getArgAsInfinitum(std::size_t index) const;

	/// \param index The index of the queried item.
	/// \return given argument as a 64-bit NTP time tag
	std::uint64_t getArgAsTimetag(std::size_t index) const;

	/// \param index The index of the queried item.
	/// \return given argument as a binary blob
	ofBuffer getArgAsBlob(std::size_t index) const;

	/// get an argument as an RGBA color, converts int32 automatically
	/// \param index The index of the queried item.
	/// \return given argument as a 32-bit color value
	std::uint32_t getArgAsRgbaColor(std::size_t index) const;

	/// \section Argument Setters

	template <typename T, typename... Args>
	ofxOscMessage & add(T first, Args... args) {
		add(first);
		add(args...);
		return *this;
	}

	/// add a 32-bit integer
	/// \return a reference to this ofxOscMessage
	ofxOscMessage & addIntArg(std::int32_t argument);
	ofxOscMessage & addInt32Arg(std::int32_t argument);
	ofxOscMessage & add(std::int32_t argument) { return addInt32Arg(argument); }

	/// add a 64-bit integer
	/// \return a reference to this ofxOscMessage
	ofxOscMessage & addInt64Arg(std::int64_t argument);
	ofxOscMessage & add(std::int64_t argument) { return addInt64Arg(argument); }

	/// add a 32-bit float
	/// \return a reference to this ofxOscMessage
	ofxOscMessage & addFloatArg(float argument);
	ofxOscMessage & add(float argument) { return addFloatArg(argument); }

	/// add a 64-bit double
	/// \return a reference to this ofxOscMessage
	ofxOscMessage & addDoubleArg(double argument);
	ofxOscMessage & add(double argument) { return addDoubleArg(argument); }

	/// add a string
	/// \return a reference to this ofxOscMessage
	ofxOscMessage & addStringArg(const std::string & argument);
	ofxOscMessage & add(const std::string & argument) { return addStringArg(argument); }

	/// add a symbol (string)
	/// \return a reference to this ofxOscMessage
	ofxOscMessage & addSymbolArg(const std::string & argument);

	/// add a char
	/// \return a reference to this ofxOscMessage
	ofxOscMessage & addCharArg(char argument);
	ofxOscMessage & add(char & argument) { return addCharArg(argument); }

	/// add a 4-byte MIDI message
	/// \return a reference to this ofxOscMessage
	ofxOscMessage & addMidiMessageArg(std::uint32_t argument);

	/// add a bool
	/// true sends a OFXOSC_TYPE_TRUE & false sends a OFXOSC_TYPE_FALSE
	/// \return a reference to this ofxOscMessage
	ofxOscMessage & addBoolArg(bool argument);
	ofxOscMessage & add(bool & argument) { return addBoolArg(argument); }

	/// add a none/nil (has no value)
	/// \return a reference to this ofxOscMessage
	ofxOscMessage & addNoneArg();
	ofxOscMessage & add() { return addNoneArg(); }

	/// add a trigger impulse (has no value)
	/// \return a reference to this ofxOscMessage
	ofxOscMessage & addTriggerArg();

	/// add a trigger impulse (has no value)
	/// alias for addTriggerArg()
	/// \return a reference to this ofxOscMessage
	ofxOscMessage & addImpulseArg();

	/// add a trigger impulse (has no value)
	/// alias for addTriggerArg()
	/// \return a reference to this ofxOscMessage
	ofxOscMessage & addInfinitumArg();

	/// add a 64-bit NTP time tag
	/// \return a reference to this ofxOscMessage
	ofxOscMessage & addTimetagArg(std::uint64_t argument);

	/// add a binary blog
	/// \return a reference to this ofxOscMessage
	ofxOscMessage & addBlobArg(const ofBuffer & argument);
	ofxOscMessage & add(const ofBuffer & argument) { return addBlobArg(argument); }

	/// add a 32-bit color
	/// \return a reference to this ofxOscMessage
	ofxOscMessage & addRgbaColorArg(std::uint32_t argument);

	/// set host and port of the remote endpoint,
	/// this is mainly used by ofxOscReceiver
	void setRemoteEndpoint(const std::string & host, int port);

	/// output stream operator for string conversion and printing
	/// converts argument contents to strings with following caveats per type:
	///   * true: printed as T
	///   * false: printed as F
	///   * midi message: printed as 4 byte hex
	///   * none/nil: printed as NONE (has no value)
	///   * trigger impulse: printed as TRIGGER (has no value)
	///   * timetag: printed as TIMETAG (does not show value)
	///   * blob: printed as BLOB (does not show value)
	///   * rgba color: printed as 4 byte hex
	/// \return message address & arguments separated by spaces
	friend std::ostream & operator<<(std::ostream & os, const ofxOscMessage & message);

private:
	std::string address; ///< OSC address, must start with a /
	std::vector<ofxOscArg *> args; ///< current arguments

	std::string remoteHost; ///< host name/ip the message was sent from
	int remotePort; ///< port the message was sent from
};
