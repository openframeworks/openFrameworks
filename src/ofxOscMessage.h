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
	ofxOscMessage( const ofxOscMessage& other ){ copy ( other ); }
	ofxOscMessage& operator= ( const ofxOscMessage& other ) { return copy( other ); }
	/// for operator= and copy constructor
	ofxOscMessage& copy( const ofxOscMessage& other );

	/// clear this message, erase all contents
	void clear();

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
