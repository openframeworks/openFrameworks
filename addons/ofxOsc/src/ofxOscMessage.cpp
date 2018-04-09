// copyright (c) openFrameworks team 2010-2017
// copyright (c) Damian Stewart 2007-2009
#include "ofxOscMessage.h"
#include "ofLog.h"
#include "ofUtils.h"

//--------------------------------------------------------------
ofxOscMessage::ofxOscMessage() : remoteHost(""), remotePort(0) {}

//--------------------------------------------------------------
ofxOscMessage::~ofxOscMessage(){
	clear();
}

//--------------------------------------------------------------
ofxOscMessage::ofxOscMessage(const ofxOscMessage &other){
	copy(other);
}

//--------------------------------------------------------------
ofxOscMessage& ofxOscMessage::operator=(const ofxOscMessage &other){
	return copy(other);
}

//--------------------------------------------------------------
ofxOscMessage& ofxOscMessage::copy(const ofxOscMessage &other){
	if(this == &other) return *this;
	clear();

	// copy address & remote info
	address = other.address;
	remoteHost = other.remoteHost;
	remotePort = other.remotePort;

	// copy arguments
    for(std::size_t i = 0; i < other.args.size(); ++i){
		switch(other.getArgType(i)){
			case OFXOSC_TYPE_TRUE: case OFXOSC_TYPE_FALSE:
				args.push_back(new ofxOscArgBool(other.getArgAsBool(i)));
				break;
			case OFXOSC_TYPE_INT32:
				args.push_back(new ofxOscArgInt32(other.getArgAsInt32(i)));
				break;
			case OFXOSC_TYPE_INT64:
				args.push_back(new ofxOscArgInt64(other.getArgAsInt64(i)));
				break;
			case OFXOSC_TYPE_FLOAT:
				args.push_back(new ofxOscArgFloat(other.getArgAsFloat(i)));
				break;
			case OFXOSC_TYPE_DOUBLE:
				args.push_back(new ofxOscArgDouble(other.getArgAsDouble(i)));
				break;
			case OFXOSC_TYPE_STRING:
				args.push_back(new ofxOscArgString(other.getArgAsString(i)));
				break;
			case OFXOSC_TYPE_SYMBOL:
				args.push_back(new ofxOscArgSymbol(other.getArgAsSymbol(i)));
				break;
			case OFXOSC_TYPE_CHAR:
				args.push_back(new ofxOscArgChar(other.getArgAsChar(i)));
				break;
			case OFXOSC_TYPE_MIDI_MESSAGE:
				args.push_back(new ofxOscArgMidiMessage(other.getArgAsMidiMessage(i)));
				break;
			case OFXOSC_TYPE_TRIGGER:
				args.push_back(new ofxOscArgTrigger());
				break;
			case OFXOSC_TYPE_TIMETAG:
				args.push_back(new ofxOscArgTimetag(other.getArgAsTimetag(i)));
				break;
			case OFXOSC_TYPE_BLOB:
				args.push_back(new ofxOscArgBlob(other.getArgAsBlob(i)));
				break;
			case OFXOSC_TYPE_RGBA_COLOR:
				args.push_back(new ofxOscArgRgbaColor(other.getArgAsRgbaColor(i)));
				break;
			default:
				ofLogError("ofxOscMessage") << "copy(): bad argument type "
					<< other.getArgType(i) << " '" << (char) other.getArgType(i) << "'";	
				break;
		}
	}
	
	return *this;
}

//--------------------------------------------------------------
void ofxOscMessage::clear(){
	address = "";
	remoteHost = "";
	remotePort = 0;
	for(unsigned int i = 0; i < args.size(); ++i){
		delete args[i];
	}
	args.clear();
}

//--------------------------------------------------------------
void ofxOscMessage::setAddress(const std::string &address){
	this->address = address;
}

//--------------------------------------------------------------
std::string ofxOscMessage::getAddress() const{
	return address;
}

//--------------------------------------------------------------
std::string ofxOscMessage::getRemoteIp() const{
	return remoteHost;
}

//--------------------------------------------------------------
std::string ofxOscMessage::getRemoteHost() const{
	return remoteHost;
}

//--------------------------------------------------------------
int ofxOscMessage::getRemotePort() const{
	return remotePort;
}

// get methods
//--------------------------------------------------------------
std::size_t ofxOscMessage::getNumArgs() const{
	return args.size();
}

//--------------------------------------------------------------
ofxOscArgType ofxOscMessage::getArgType(std::size_t index) const{
	if(index >= args.size()) {
		ofLogError("ofxOscMessage") << "getArgType(): index "
		                            << index << " out of bounds";
		return OFXOSC_TYPE_INDEXOUTOFBOUNDS;
	}
	else{
		return args[index]->getType();
	}
}

//--------------------------------------------------------------
std::string ofxOscMessage::getArgTypeName(std::size_t index) const{
	if(index >= args.size()) {
		ofLogError("ofxOscMessage") << "getArgTypeName(): index "
		                            << index << " out of bounds";
		return "INDEX OUT OF BOUNDS";
	}
	else{
		return args[index]->getTypeName();
	}
}

//--------------------------------------------------------------
std::string ofxOscMessage::getTypeString() const {
	std::string types = "";
	for(std::size_t i = 0; i < args.size(); ++i) {
		types += args[i]->getTypeName();
	}
	return types;
}

//--------------------------------------------------------------
std::int32_t ofxOscMessage::getArgAsInt(std::size_t index) const{
	return getArgAsInt32(index);
}

//--------------------------------------------------------------
std::int32_t ofxOscMessage::getArgAsInt32(std::size_t index) const{
	if(getArgType(index) != OFXOSC_TYPE_INT32){
		if(getArgType(index) == OFXOSC_TYPE_INT64){
			// warn about possible lack of precision
			ofLogWarning("ofxOscMessage")
				<< "getArgAsInt32(): converting int64 to int32 for argument "
				<< index;
			return (std::int32_t)((ofxOscArgInt64*)args[index])->get();
		}
		else if (getArgType(index) == OFXOSC_TYPE_FLOAT){
			return (std::int32_t)((ofxOscArgFloat*)args[index])->get();
		}
		else if (getArgType(index) == OFXOSC_TYPE_DOUBLE){
			// warn about possible lack of precision
			ofLogWarning("ofxOscMessage")
				<< "getArgAsInt32(): converting double to int32 for argument "
				<< index;
			return (std::int32_t)((ofxOscArgDouble*)args[index])->get();
		}
		else if(getArgType(index) == OFXOSC_TYPE_TRUE || 
			    getArgType(index) == OFXOSC_TYPE_FALSE){
			return (std::int32_t)((ofxOscArgBool*)args[index])->get();
		}
		else{
			ofLogError("ofxOscMessage") << "getArgAsInt32(): argument "
			                            << index << " is not a number";
			return 0;
		}
	}
	else{
		return ((ofxOscArgInt32*)args[index])->get();
	}
}

//--------------------------------------------------------------
std::int64_t ofxOscMessage::getArgAsInt64(std::size_t index) const{
	if(getArgType(index) != OFXOSC_TYPE_INT64){
		if(getArgType(index) == OFXOSC_TYPE_INT32){
			return (std::int64_t)((ofxOscArgInt32*)args[index])->get();
		}
		else if(getArgType(index) == OFXOSC_TYPE_FLOAT){
			return (std::int64_t)((ofxOscArgFloat*)args[index])->get();
		}
		else if(getArgType(index) == OFXOSC_TYPE_DOUBLE){
			return (std::int64_t)((ofxOscArgDouble*)args[index])->get();
		}
		else if(getArgType(index) == OFXOSC_TYPE_TRUE ||
			    getArgType(index) == OFXOSC_TYPE_FALSE){
			return (std::int64_t)((ofxOscArgBool*)args[index])->get();
		}
		else{
			ofLogError("ofxOscMessage") << "getArgAsInt64(): argument "
			                            << index << " is not a number";
			return 0;
		}
	}
	else{
		return ((ofxOscArgInt64*)args[index])->get();
	}
}

//--------------------------------------------------------------
float ofxOscMessage::getArgAsFloat(std::size_t index) const{
	if(getArgType(index) != OFXOSC_TYPE_FLOAT){
		if(getArgType(index) == OFXOSC_TYPE_INT32){
			return (float)((ofxOscArgInt32*)args[index])->get();
		}
		else if(getArgType(index) == OFXOSC_TYPE_INT64){
			// warn about possible lack of precision
			ofLogWarning("ofxOscMessage")
				<< "getArgAsFloat(): converting int64 to float for argument "
				<< index;
			return (float)((ofxOscArgInt64*)args[index])->get();
		}
		else if(getArgType(index) == OFXOSC_TYPE_DOUBLE){
			// warn about possible lack of precision
			ofLogWarning("ofxOscMessage")
				<< "getArgAsFloat(): converting double to float for argument "
				<< index;
			return (float)((ofxOscArgDouble*)args[index])->get();
		}
		else if(getArgType(index) == OFXOSC_TYPE_TRUE ||
			    getArgType(index) == OFXOSC_TYPE_FALSE){
			return (float)((ofxOscArgBool*)args[index])->get();
		}
		else{
			ofLogError("ofxOscMessage") << "getArgAsFloat(): argument "
			                            << index << " is not a number";
			return 0;
		}
	}
	else{
		return ((ofxOscArgFloat*)args[index])->get();
	}
}

//--------------------------------------------------------------
double ofxOscMessage::getArgAsDouble(std::size_t index) const{
	if(getArgType(index) != OFXOSC_TYPE_DOUBLE){
		if(getArgType(index) == OFXOSC_TYPE_INT32){
			return (double)((ofxOscArgInt32*)args[index])->get();
		}
		else if(getArgType(index) == OFXOSC_TYPE_INT64){
			return (double)((ofxOscArgInt64*)args[index])->get();
		}
		else if(getArgType(index) == OFXOSC_TYPE_FLOAT){
			return (double)((ofxOscArgFloat*)args[index])->get();
		}
		else if( getArgType(index) == OFXOSC_TYPE_TRUE ||
			     getArgType(index) == OFXOSC_TYPE_FALSE){
			return (double)((ofxOscArgBool*)args[index])->get();
		}
		else{
			ofLogError("ofxOscMessage") << "getArgAsDouble(): argument "
			                            << index << " is not a number";
			return 0;
		}
	}
	else{
		return ((ofxOscArgDouble*)args[index])->get();
	}
}

//--------------------------------------------------------------
std::string ofxOscMessage::getArgAsString(std::size_t index) const{
	if(getArgType(index) != OFXOSC_TYPE_STRING){
		// warn about string conversions
		if(getArgType(index) == OFXOSC_TYPE_INT32){
			ofLogWarning("ofxOscMessage")
				<< "getArgAsString(): converting int32 to string for argument "
				<< index;
			return ofToString(((ofxOscArgInt32*)args[index])->get());
		}
		else if(getArgType(index) == OFXOSC_TYPE_INT64){
			ofLogWarning("ofxOscMessage")
				<< "getArgAsString(): converting int64 to string for argument "
				<< index;
			return ofToString(((ofxOscArgInt64*)args[index])->get());
		}
		else if(getArgType(index) == OFXOSC_TYPE_FLOAT){
			ofLogWarning("ofxOscMessage")
				<< "getArgAsString(): converting float to string for argument "
				<< index;
			return ofToString(((ofxOscArgFloat*)args[index])->get());
		}
		else if(getArgType(index) == OFXOSC_TYPE_DOUBLE){
			ofLogWarning("ofxOscMessage")
				<< "getArgAsString(): converting double to string for argument "
				<< index;
			return ofToString(((ofxOscArgDouble*)args[index])->get());
		}
		else if(getArgType(index) == OFXOSC_TYPE_SYMBOL){
			return ((ofxOscArgSymbol*)args[index])->get();
		}
		else if(getArgType(index) == OFXOSC_TYPE_CHAR){
			ofLogWarning("ofxOscMessage")
				<< "getArgAsString(): converting char to string for argument "
				<< index;
			return ofToString(((ofxOscArgChar*)args[index])->get());
		}
		else{
			ofLogError("ofxOscMessage")
				<< "getArgAsString(): argument " << index
				<< " is not a string interpretable value";
			return "";
		}
	}
	else{
		return ((ofxOscArgString*)args[index])->get();
	}
}

//--------------------------------------------------------------
std::string ofxOscMessage::getArgAsSymbol(std::size_t index) const{
	if(getArgType(index) != OFXOSC_TYPE_SYMBOL){
		// warn about string conversions
		if(getArgType(index) == OFXOSC_TYPE_INT32){
			ofLogWarning("ofxOscMessage")
				<< "getArgAsSymbol(): converting int32 to symbol (string) "
				<< "for argument " << index;
			return ofToString(((ofxOscArgInt32*)args[index])->get());
		}
		else if(getArgType(index) == OFXOSC_TYPE_INT64){
			ofLogWarning("ofxOscMessage")
				<< "getArgAsSymbol(): converting int64 to symbol (string) "
				<< "for argument " << index;
			return ofToString(((ofxOscArgInt64*)args[index])->get());
		}
		else if(getArgType(index) == OFXOSC_TYPE_FLOAT){
			ofLogWarning("ofxOscMessage")
				<< "getArgAsSymbol(): converting float to symbol (string) "
				<< "for argument " << index;
			return ofToString(((ofxOscArgFloat*)args[index])->get());
		}
		else if(getArgType(index) == OFXOSC_TYPE_DOUBLE){
			ofLogWarning("ofxOscMessage")
				<< "getArgAsSymbol(): converting double to symbol (string) "
				<< "for argument " << index;
			return ofToString(((ofxOscArgDouble*)args[index])->get());
		}
		else if(getArgType(index) == OFXOSC_TYPE_STRING){
			return ((ofxOscArgString*)args[index])->get();
		}
		else if(getArgType(index) == OFXOSC_TYPE_CHAR){
			ofLogWarning("ofxOscMessage")
				<< "getArgAsSymbol(): converting char to symbol (string) "
				<< "for argument " << index;
			return ofToString(((ofxOscArgChar*)args[index])->get());
		}
		else{
			ofLogError("ofxOscMessage") << "getArgAsSymbol(): argument "
				<< index << " is not a symbol (string) interpretable value";
			return "";
		}
	}
	else{
		return ((ofxOscArgSymbol*)args[index])->get();
	}
}

//--------------------------------------------------------------
char ofxOscMessage::getArgAsChar(std::size_t index) const{
	if(getArgType(index) == OFXOSC_TYPE_CHAR){
		return ((ofxOscArgChar*)args[index])->get();
	}
	else{
		ofLogError("ofxOscMessage") << "getArgAsChar(): argument "
			<< index << " is not a char";
		return 0;
	}
}

//--------------------------------------------------------------
std::uint32_t ofxOscMessage::getArgAsMidiMessage(std::size_t index) const{
	if(getArgType(index) == OFXOSC_TYPE_MIDI_MESSAGE){
		return ((ofxOscArgMidiMessage*)args[index])->get();
	}
	else{
		ofLogError("ofxOscMessage") << "getArgAsMidiMessage(): argument "
			<< index << " is not a midi message";
		return 0;
	}
}

//--------------------------------------------------------------
bool ofxOscMessage::getArgAsBool(std::size_t index) const{
	switch(getArgType(index)){
		case OFXOSC_TYPE_TRUE: case OFXOSC_TYPE_FALSE:
			return ((ofxOscArgBool*)args[index])->get();
		case OFXOSC_TYPE_INT32:
			return ((ofxOscArgInt32*)args[index])->get() > 0;
		case OFXOSC_TYPE_INT64:
			return ((ofxOscArgInt64*)args[index])->get() > 0;
		case OFXOSC_TYPE_FLOAT:
			return ((ofxOscArgFloat*)args[index])->get() > 0;
		case OFXOSC_TYPE_DOUBLE:
			return ((ofxOscArgDouble*)args[index])->get() > 0;
		case OFXOSC_TYPE_STRING: case OFXOSC_TYPE_SYMBOL:
			return ((ofxOscArgString*)args[index])->get() == "true";
		default:
			ofLogError("ofxOscMessage") << "getArgAsBool(): argument "
				<< index << " is not a boolean interpretable value";
			return false;
	}
}

//--------------------------------------------------------------
bool ofxOscMessage::getArgAsNone(std::size_t index) const{
	if(getArgType(index) != OFXOSC_TYPE_NONE){
		ofLogError("ofxOscMessage") << "getArgAsNone(): argument "
			<< index << " is not a none/nil";
		return false;
	}
	else{
		return ((ofxOscArgNone*)args[index])->get();
	}
}

//--------------------------------------------------------------
bool ofxOscMessage::getArgAsTrigger(std::size_t index) const{
	if(getArgType(index) != OFXOSC_TYPE_TRIGGER){
		ofLogError("ofxOscMessage") << "getArgAsTrigger(): argument "
			<< index << " is not a trigger";
		return false;
	}
	else{
		return ((ofxOscArgTrigger*)args[index])->get();
	}
}

//--------------------------------------------------------------
bool ofxOscMessage::getArgAsImpulse(std::size_t index) const{
	return getArgAsTrigger(index);
}

//--------------------------------------------------------------
bool ofxOscMessage::getArgAsInfinitum(std::size_t index) const{
	return getArgAsTrigger(index);
}

//--------------------------------------------------------------
std::uint64_t ofxOscMessage::getArgAsTimetag(std::size_t index) const{
	if(getArgType(index) != OFXOSC_TYPE_TIMETAG){
		if(getArgType(index) == OFXOSC_TYPE_DOUBLE){
			// warn about possible conversion issue
			ofLogWarning("ofxOscMessage")
				<< "getArgAsTimetag(): converting double to Timetag "
				<< "for argument " << index;
			return (std::uint64_t)((ofxOscArgDouble*)args[index])->get();
		}
		else{
			ofLogError("ofxOscMessage") << "getArgAsTimetag(): argument "
				<< index << " is not a valid number";
			return 0;
		}
	}
	else{
		return ((ofxOscArgTimetag*)args[index])->get();
	}
}

//--------------------------------------------------------------
ofBuffer ofxOscMessage::getArgAsBlob(std::size_t index) const{
	if(getArgType(index) != OFXOSC_TYPE_BLOB){
		ofLogError("ofxOscMessage") << "getArgAsBlob(): argument "
			<< index << " is not a blob";
		return ofBuffer();
	}
	else{
		return ((ofxOscArgBlob*)args[index])->get();
	}
}

//--------------------------------------------------------------
std::uint32_t ofxOscMessage::getArgAsRgbaColor(std::size_t index) const{
	if(getArgType(index) != OFXOSC_TYPE_RGBA_COLOR){
		ofLogError("ofxOscMessage") << "getArgAsRgbaColor(): argument "
			<< index << " is not an rgba color";
		return 0;
	}
	else{
		return ((ofxOscArgRgbaColor*)args[index])->get();
	}
}

// set methods
//--------------------------------------------------------------
void ofxOscMessage::addIntArg(std::int32_t argument){
	args.push_back(new ofxOscArgInt32(argument));
}

//--------------------------------------------------------------
void ofxOscMessage::addInt32Arg(std::int32_t argument){
	args.push_back(new ofxOscArgInt32(argument));
}

//--------------------------------------------------------------
void ofxOscMessage::addInt64Arg(std::int64_t argument){
	args.push_back(new ofxOscArgInt64(argument));
}

//--------------------------------------------------------------
void ofxOscMessage::addFloatArg(float argument){
	args.push_back(new ofxOscArgFloat(argument));
}

//--------------------------------------------------------------
void ofxOscMessage::addDoubleArg(double argument){
	args.push_back(new ofxOscArgDouble(argument));
}

//--------------------------------------------------------------
void ofxOscMessage::addStringArg(const std::string &argument){
	args.push_back(new ofxOscArgString(argument));
}

//--------------------------------------------------------------
void ofxOscMessage::addSymbolArg(const std::string &argument){
	args.push_back(new ofxOscArgSymbol(argument));
}

//--------------------------------------------------------------
void ofxOscMessage::addCharArg( char argument){
	args.push_back(new ofxOscArgChar(argument));
}

//--------------------------------------------------------------
void ofxOscMessage::addMidiMessageArg(std::uint32_t argument){
	args.push_back(new ofxOscArgMidiMessage(argument));
}

//--------------------------------------------------------------
void ofxOscMessage::addBoolArg(bool argument){
	args.push_back(new ofxOscArgBool(argument));
}

//--------------------------------------------------------------
void ofxOscMessage::addNoneArg(){
	args.push_back(new ofxOscArgNone());
}

//--------------------------------------------------------------
void ofxOscMessage::addTriggerArg(){
	args.push_back(new ofxOscArgTrigger());
}

//--------------------------------------------------------------
void ofxOscMessage::addImpulseArg(){
	args.push_back(new ofxOscArgTrigger());
}

//--------------------------------------------------------------
void ofxOscMessage::addInfinitumArg(){
	args.push_back(new ofxOscArgTrigger());
}

//--------------------------------------------------------------
void ofxOscMessage::addTimetagArg(std::uint64_t argument){
	args.push_back(new ofxOscArgTimetag(argument));
}

//--------------------------------------------------------------
void ofxOscMessage::addBlobArg(const ofBuffer &argument){
	args.push_back(new ofxOscArgBlob(argument));
}

//--------------------------------------------------------------
void ofxOscMessage::addRgbaColorArg(std::uint32_t argument){
	args.push_back(new ofxOscArgRgbaColor(argument));
}

// util
//--------------------------------------------------------------
void ofxOscMessage::setRemoteEndpoint(const std::string &host, int port){
	remoteHost = host;
	remotePort = port;
}

// friend functions
//--------------------------------------------------------------
std::ostream& operator<<(std::ostream &os, const ofxOscMessage &message) {
	os << message.getAddress();
	for(std::size_t i = 0; i < message.getNumArgs(); ++i) {
		os << " ";
		switch(message.getArgType(i)) {
			case OFXOSC_TYPE_INT32:
				os << message.getArgAsInt32(i);
				break;
			case OFXOSC_TYPE_INT64:
				os << message.getArgAsInt64(i);
				break;
			case OFXOSC_TYPE_FLOAT:
				os << message.getArgAsFloat(i);
				break;
			case OFXOSC_TYPE_DOUBLE:
				os << message.getArgAsDouble(i);
				break;
			case OFXOSC_TYPE_STRING:
				os << message.getArgAsString(i);
				break;
			case OFXOSC_TYPE_SYMBOL:
				os << message.getArgAsSymbol(i);
				break;
			case OFXOSC_TYPE_CHAR:
				os << message.getArgAsChar(i);
				break;
			case OFXOSC_TYPE_MIDI_MESSAGE:
				os << ofToHex(message.getArgAsMidiMessage(i));
				break;
			case OFXOSC_TYPE_TRUE:
				os << "T";
				break;
			case OFXOSC_TYPE_FALSE:
				os << "F";
				break;
			case OFXOSC_TYPE_NONE:
				os << "NONE";
				break;
			case OFXOSC_TYPE_TRIGGER:
				os << "TRIGGER";
				break;
			case OFXOSC_TYPE_TIMETAG:
				os << "TIMETAG";
				break;
			case OFXOSC_TYPE_BLOB:
				os << "BLOB";
				break;
			case OFXOSC_TYPE_RGBA_COLOR:
				os << ofToHex(message.getArgAsRgbaColor(i));
				break;
			default:
				break;
		}
	}
	return os;
}
