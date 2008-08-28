/*
 *  ofxOscMessage.h
 *  openFrameworks OSC addon
 *
 *  damian@frey.co.nz
 *
 */

#ifndef _ofxOscMESSAGE_H
#define _ofxOscMESSAGE_H

#include "ofxOscArg.h"
#include <vector>
#include <string>

class ofxOscMessage
{
public:
	ofxOscMessage();
	~ofxOscMessage();
	ofxOscMessage( const ofxOscMessage& other ) { copy ( other ); }
	ofxOscMessage& operator= ( const ofxOscMessage& other ) { return copy( other ); }
	// for operator= and copy constructor
	ofxOscMessage& copy( const ofxOscMessage& other );

	/// return the address
	const char* getAddress() const { return address.c_str(); }

	/// return number of arguments
	int getNumArgs() const;
	/// return argument type code for argument # index
	ofxOscArgType getArgType( int index ) const;
	/// return argument type name as string
	/// - either "int", "float", or "string"
	const char* getArgTypeName( int index ) const;

	/// get the argument with the given index as an int, float, or string
	/// ensure that the type matches what you're requesting
	/// (eg for an int argument, getArgType(index)==OF_TYPE_INT32
	/// or getArgTypeName(index)=="int32")
	int32_t getArgAsInt32( int index ) const;
	float getArgAsFloat( int index ) const;
	const char* getArgAsString( int index ) const;

	/// message construction
	void setAddress( const char* _address ) { address = _address; };
	void addIntArg( int32_t argument );
	void addFloatArg( float argument );
	void addStringArg( const char* argument );

private:

	std::string address;
	std::vector<ofxOscArg*> args;
};

#endif
