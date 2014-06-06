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

#include "ofxOscArg.h"
#include <vector>
#include <string>

using namespace std;

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
	string getAddress() const { return address; }

	/// return the remote ip
	string getRemoteIp() const { return remote_host; }
	/// return the remote port
	int getRemotePort() const { return remote_port; }

	/// return number of argumentsļ
	int getNumArgs() const;
	/// return argument type code for argument # index
	ofxOscArgType getArgType( int index ) const;
	/// return argument type name as string
	/// - either "int", "float", or "string"
	string getArgTypeName( int index ) const;

	/// get the argument with the given index as an int, float, or string
	/// ensure that the type matches what you're requesting
	/// (eg for an int argument, getArgType(index)==OF_TYPE_INT32
	/// or getArgTypeName(index)=="int32")
	int32_t getArgAsInt32( int index ) const;
	uint64_t getArgAsInt64( int index ) const;
	float getArgAsFloat( int index ) const;
	string getArgAsString( int index ) const;
	ofBuffer getArgAsBlob( int index ) const;

	/// message construction
	void setAddress( string _address ) { address = _address; };
	/// host and port of the remote endpoint
	void setRemoteEndpoint( string host, int port ) { remote_host = host; remote_port = port; }
	void addIntArg( int32_t argument );
	void addInt64Arg( uint64_t argument );
	void addFloatArg( float argument );
	void addStringArg( string argument );
	void addBlobArg( ofBuffer argument );

private:

	string address;
	vector<ofxOscArg*> args;

	string remote_host;
	int remote_port;


};
