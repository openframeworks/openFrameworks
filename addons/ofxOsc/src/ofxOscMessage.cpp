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

#include "ofxOscMessage.h"
#include "ofLog.h"
#include <iostream>
#include <assert.h>

ofxOscMessage::ofxOscMessage()

{
}

ofxOscMessage::~ofxOscMessage()
{
	clear();
}

void ofxOscMessage::clear()
{
	for ( unsigned int i=0; i<args.size(); ++i )
		delete args[i];
	args.clear();
	address = "";
}

/*

get methods

*/

int ofxOscMessage::getNumArgs() const
{
	return (int)args.size();
}

ofxOscArgType ofxOscMessage::getArgType( int index ) const
{
    if ( index >= (int)args.size() )
    {
    	ofLogError("ofxOscMessage") << "getArgType(): index " << index << " out of bounds";
        return OFXOSC_TYPE_INDEXOUTOFBOUNDS;
    }
    else
        return args[index]->getType();
}

string ofxOscMessage::getArgTypeName( int index ) const
{
    if ( index >= (int)args.size() )
    {
    	ofLogError("ofxOscMessage") << "getArgTypeName(): index " << index << " out of bounds";
        return "INDEX OUT OF BOUNDS";
    }
    else
        return args[index]->getTypeName();
}


int32_t ofxOscMessage::getArgAsInt32( int index ) const
{
	if ( getArgType(index) != OFXOSC_TYPE_INT32 )
	{
	    if ( getArgType( index ) == OFXOSC_TYPE_FLOAT )
        {
	    	ofLogWarning("ofxOscMessage") << "getArgAsInt32(): converting int32 to float for argument " << index;
            return ((ofxOscArgFloat*)args[index])->get();
        }
        else
        {
        	ofLogError("ofxOscMessage") << "getArgAsInt32(): argument " << index << " is not a number";
            return 0;
        }
	}
	else
        return ((ofxOscArgInt32*)args[index])->get();
}

uint64_t ofxOscMessage::getArgAsInt64( int index ) const
{
	if ( getArgType(index) != OFXOSC_TYPE_INT64 )
	{
	    if ( getArgType( index ) == OFXOSC_TYPE_FLOAT )
        {
	    	ofLogWarning("ofxOscMessage") << "getArgAsInt64(): converting int64 to float for argument " << index;
            return ((ofxOscArgFloat*)args[index])->get();
        }
        else
        {
        	ofLogError("ofxOscMessage") << "getArgAsInt64(): argument " << index << " is not a number";
            return 0;
        }
	}
	else
        return ((ofxOscArgInt64*)args[index])->get();
}


float ofxOscMessage::getArgAsFloat( int index ) const
{
	if ( getArgType(index) != OFXOSC_TYPE_FLOAT )
	{
	    if ( getArgType( index ) == OFXOSC_TYPE_INT32 )
        {
	    	ofLogWarning("ofxOscMessage") << "getArgAsFloat(): converting float to int32 for argument " << index;
            return ((ofxOscArgInt32*)args[index])->get();
        }
        else
        {
        	ofLogError("ofxOscMessage") << "getArgAsFloat(): argument " << index << " is not a number";
            return 0;
        }
	}
	else
        return ((ofxOscArgFloat*)args[index])->get();
}


string ofxOscMessage::getArgAsString( int index ) const
{
    if ( getArgType(index) != OFXOSC_TYPE_STRING )
	{
	    if ( getArgType( index ) == OFXOSC_TYPE_FLOAT )
        {
            char buf[1024];
            sprintf(buf,"%f",((ofxOscArgFloat*)args[index])->get() );
            ofLogWarning("ofxOscMessage") << "getArgAsString(): converting float to string for argument " << index;
            return buf;
        }
	    else if ( getArgType( index ) == OFXOSC_TYPE_INT32 )
        {
            char buf[1024];
            sprintf(buf,"%i",((ofxOscArgInt32*)args[index])->get() );
            ofLogWarning("ofxOscMessage") << "getArgAsString(): converting int32 to string for argument " << index;
            return buf;
        }
        else
        {
        	ofLogError("ofxOscMessage") << "getArgAsString(): argument " << index << " is not a string";
            return "";
        }
	}
	else
        return ((ofxOscArgString*)args[index])->get();
}

ofBuffer ofxOscMessage::getArgAsBlob( int index ) const
{
    if ( getArgType(index) != OFXOSC_TYPE_BLOB )
	{
        ofLogError("ofxOscMessage") << "getArgAsBlob(): argument " << index << " is not a blob";
        return ofBuffer();
	}
	else
        return ((ofxOscArgBlob*)args[index])->get();
}



/*

set methods

*/


void ofxOscMessage::addIntArg( int32_t argument )
{

	args.push_back( new ofxOscArgInt32( argument ) );
}

void ofxOscMessage::addInt64Arg( uint64_t argument )
{

	args.push_back( new ofxOscArgInt64( argument ) );
}


void ofxOscMessage::addFloatArg( float argument )
{
	args.push_back( new ofxOscArgFloat( argument ) );
}

void ofxOscMessage::addStringArg( string argument )
{
	args.push_back( new ofxOscArgString( argument ) );
}

void ofxOscMessage::addBlobArg( ofBuffer argument )
{
	args.push_back( new ofxOscArgBlob( argument ) );
}

/*

 housekeeping

 */

ofxOscMessage& ofxOscMessage::copy( const ofxOscMessage& other )
{
	clear();
	// copy address
	address = other.address;

	remote_host = other.remote_host;
	remote_port = other.remote_port;

	// copy arguments
	for ( int i=0; i<(int)other.args.size(); ++i )
	{
		ofxOscArgType argType = other.getArgType( i );
		if ( argType == OFXOSC_TYPE_INT32 )
			args.push_back( new ofxOscArgInt32( other.getArgAsInt32( i ) ) );
		else if ( argType == OFXOSC_TYPE_INT64 )
			args.push_back( new ofxOscArgInt64( other.getArgAsInt64( i ) ) );
		else if ( argType == OFXOSC_TYPE_FLOAT )
			args.push_back( new ofxOscArgFloat( other.getArgAsFloat( i ) ) );
		else if ( argType == OFXOSC_TYPE_STRING )
			args.push_back( new ofxOscArgString( other.getArgAsString( i ) ) );
		else if ( argType == OFXOSC_TYPE_BLOB )
			args.push_back( new ofxOscArgBlob( other.getArgAsBlob( i ) ) );
		else
		{
			assert( false && "bad argument type" );
		}
	}

	return *this;
}
