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

std::string ofxOscMessage::getArgTypeName( int index ) const
{
    if ( index >= (int)args.size() )
    {
    	ofLogError("ofxOscMessage") << "getArgTypeName(): index " << index << " out of bounds";
        return "INDEX OUT OF BOUNDS";
    }
    else
        return args[index]->getTypeName();
}

std::int32_t ofxOscMessage::getArgAsInt( int index ) const
{
	return getArgAsInt32(index);
}

std::int32_t ofxOscMessage::getArgAsInt32( int index ) const
{
	if ( getArgType(index) != OFXOSC_TYPE_INT32 )
	{
	    if ( getArgType( index ) == OFXOSC_TYPE_FLOAT )
        {
	    	ofLogWarning("ofxOscMessage") << "getArgAsInt32(): converting float to int32 for argument " << index;
            return (std::int32_t)((ofxOscArgFloat*)args[index])->get();
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

std::int64_t ofxOscMessage::getArgAsInt64( int index ) const
{
	if ( getArgType(index) != OFXOSC_TYPE_INT64 )
	{
	    if ( getArgType( index ) == OFXOSC_TYPE_FLOAT )
        {
	    	ofLogWarning("ofxOscMessage") << "getArgAsInt64(): converting float to int64 for argument " << index;
            return (std::int64_t)((ofxOscArgFloat*)args[index])->get();
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
	    	ofLogWarning("ofxOscMessage") << "getArgAsFloat(): converting int32 to float for argument " << index;
            return (float)((ofxOscArgInt32*)args[index])->get();
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

double ofxOscMessage::getArgAsDouble( int index ) const
{
	if ( getArgType(index) != OFXOSC_TYPE_DOUBLE )
	{
	    if ( getArgType( index ) == OFXOSC_TYPE_INT32 )
        {
	    	ofLogWarning("ofxOscMessage") << "getArgAsDouble(): converting int32 to double for argument " << index;
            return (double)((ofxOscArgInt32*)args[index])->get();
        }
        else
        {
        	ofLogError("ofxOscMessage") << "getArgAsDouble(): argument " << index << " is not a number";
            return 0;
        }
	}
	else
        return ((ofxOscArgDouble*)args[index])->get();
}

std::string ofxOscMessage::getArgAsString( int index ) const
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

std::string ofxOscMessage::getArgAsSymbol(int index) const
{
    if ( getArgType(index) != OFXOSC_TYPE_SYMBOL )
	{
	    if ( getArgType( index ) == OFXOSC_TYPE_FLOAT )
        {
            char buf[1024];
            sprintf(buf,"%f",((ofxOscArgFloat*)args[index])->get() );
            ofLogWarning("ofxOscMessage") << "getArgAsSymbol(): converting float to symbol (string) for argument " << index;
            return buf;
        }
	    else if ( getArgType( index ) == OFXOSC_TYPE_INT32 )
        {
            char buf[1024];
            sprintf(buf,"%i",((ofxOscArgInt32*)args[index])->get() );
            ofLogWarning("ofxOscMessage") << "getArgAsSymbol(): converting int32 to symbol (string) for argument " << index;
            return buf;
        }
        else
        {
        	ofLogError("ofxOscMessage") << "getArgAsSymbol(): argument " << index << " is not a symbol (string)";
            return "";
        }
	}
	else
        return ((ofxOscArgSymbol*)args[index])->get();
}

char ofxOscMessage::getArgAsChar(int index) const
{
    if ( getArgType(index) == OFXOSC_TYPE_CHAR )
	{
		return ((ofxOscArgChar*)args[index])->get();
	}
	else
    {
		ofLogError("ofxOscMessage") << "getArgAsSymbol(): argument " << index << " is not a symbol (string)";
		return NULL;
	}
}

std::int32_t ofxOscMessage::getArgAsMidiMessage(int index) const
{
	if ( getArgType(index) != OFXOSC_TYPE_INT32 )
	{
	    if ( getArgType( index ) == OFXOSC_TYPE_FLOAT )
        {
	    	ofLogWarning("ofxOscMessage") << "getArgAsInt32(): converting float to int32 for argument " << index;
            return (std::int32_t)((ofxOscArgFloat*)args[index])->get();
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

bool ofxOscMessage::getArgAsBool(int index) const
{
	ofxOscArgType incomingArgType = getArgType( index );
	if(incomingArgType == OFXOSC_TYPE_TRUE || incomingArgType == OFXOSC_TYPE_FALSE)
	{
		return ((ofxOscArgBool*)args[index])->get();
	}
	else if(incomingArgType == OFXOSC_TYPE_INT32)
	{
		return ((ofxOscArgInt32*)args[index])->get() > 0;
	}
	else if(incomingArgType == OFXOSC_TYPE_INT64)
	{
		return ((ofxOscArgInt64*)args[index])->get() > 0;
	}
	else if(incomingArgType == OFXOSC_TYPE_FLOAT)
	{
		return ((ofxOscArgFloat*)args[index])->get() > 0;
	}
	else if(incomingArgType == OFXOSC_TYPE_DOUBLE)
	{
		return ((ofxOscArgDouble*)args[index])->get() > 0;
	}
	else if(incomingArgType == OFXOSC_TYPE_STRING || incomingArgType == OFXOSC_TYPE_SYMBOL)
	{
		return ((ofxOscArgString*)args[index])->get() == "true";
	}
	else
	{
		ofLogError("ofxOscMessage") << "getArgAsBool(): argument " << index << " is not a boolean interpretable value";
		return false;
	}
}

bool ofxOscMessage::getArgAsTrigger(int index) const
{
	if ( getArgType(index) != OFXOSC_TYPE_TRIGGER )
	{
		ofLogError("ofxOscMessage") << "getArgAsTrigger(): argument " << index << " is not a trigger";
		return NULL;
	}
	else
        return ((ofxOscArgTrigger*)args[index])->get();
}

bool ofxOscMessage::getArgAsImpulse(int index) const
{
	return getArgAsTrigger(index);
}

bool ofxOscMessage::getArgAsInfinitum(int index) const
{
	return getArgAsTrigger(index);
}

std::int64_t ofxOscMessage::getArgAsTimetag(int index) const
{
	if ( getArgType(index) != OFXOSC_TYPE_TIMETAG )
	{
	    if ( getArgType( index ) == OFXOSC_TYPE_DOUBLE )
        {
	    	ofLogWarning("ofxOscMessage") << "getArgAsTimetag(): converting double to Timetag for argument " << index;
            return (std::int32_t)((ofxOscArgFloat*)args[index])->get();
        }
        else
        {
        	ofLogError("ofxOscMessage") << "getArgAsTimetag(): argument " << index << " is not a valid number";
            return 0;
        }
	}
	else
        return ((ofxOscArgTimetag*)args[index])->get();
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

std::int32_t ofxOscMessage::getArgAsRgbaColor( int index ) const
{
    if ( getArgType(index) != OFXOSC_TYPE_RGBA_COLOR )
    {
        ofLogError("ofxOscMessage") << "getArgAsRgbaColor(): argument " << index << " is not a rgba color";
        return 0;
    }
    else
        return ((ofxOscArgRgbaColor*)args[index])->get();
}

/*

set methods

*/

void ofxOscMessage::addIntArg( std::int32_t argument )
{
	args.push_back( new ofxOscArgInt32( argument ) );
}

void ofxOscMessage::addInt32Arg( std::int32_t argument )
{
	args.push_back( new ofxOscArgInt32( argument ) );
}

void ofxOscMessage::addInt64Arg( std::int64_t argument )
{
	args.push_back( new ofxOscArgInt64( argument ) );
}

void ofxOscMessage::addFloatArg( float argument )
{
	args.push_back( new ofxOscArgFloat( argument ) );
}

void ofxOscMessage::addDoubleArg( double argument )
{
	args.push_back( new ofxOscArgDouble( argument ) );
}

void ofxOscMessage::addStringArg( const std::string &argument )
{
	args.push_back( new ofxOscArgString( argument ) );
}

void ofxOscMessage::addSymbolArg( const std::string &argument )
{
	args.push_back( new ofxOscArgSymbol( argument ) );
}

void ofxOscMessage::addCharArg( char argument )
{
	args.push_back( new ofxOscArgChar( argument ) );
}

void ofxOscMessage::addMidiMessageArg( std::int32_t argument )
{
	args.push_back( new ofxOscArgMidiMessage( argument ) );
}

void ofxOscMessage::addBoolArg( bool argument )
{
	args.push_back( new ofxOscArgBool( argument ) );
}

void ofxOscMessage::addTriggerArg()
{
	args.push_back( new ofxOscArgTrigger() );
}

void ofxOscMessage::addImpulseArg()
{
	args.push_back( new ofxOscArgTrigger() );
}

void ofxOscMessage::addInfinitumArg()
{
	args.push_back( new ofxOscArgTrigger() );
}

void ofxOscMessage::addTimetagArg( std::int64_t argument )
{
	args.push_back( new ofxOscArgBool( argument ) );
}

void ofxOscMessage::addBlobArg( const ofBuffer &argument )
{
	args.push_back( new ofxOscArgBlob( argument ) );
}

void ofxOscMessage::addRgbaColorArg( std::int32_t argument )
{
	args.push_back( new ofxOscArgRgbaColor( argument ) );
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
		if ( argType == OFXOSC_TYPE_TRUE || argType == OFXOSC_TYPE_FALSE )
			args.push_back( new ofxOscArgBool( other.getArgAsBool( i ) ) );
		else if ( argType == OFXOSC_TYPE_INT32 )
			args.push_back( new ofxOscArgInt32( other.getArgAsInt32( i ) ) );
		else if ( argType == OFXOSC_TYPE_INT64 )
			args.push_back( new ofxOscArgInt64( other.getArgAsInt64( i ) ) );
		else if ( argType == OFXOSC_TYPE_FLOAT )
			args.push_back( new ofxOscArgFloat( other.getArgAsFloat( i ) ) );
		else if ( argType == OFXOSC_TYPE_DOUBLE )
			args.push_back( new ofxOscArgDouble( other.getArgAsDouble( i ) ) );
		else if ( argType == OFXOSC_TYPE_STRING )
			args.push_back( new ofxOscArgString( other.getArgAsString( i ) ) );
		else if ( argType == OFXOSC_TYPE_SYMBOL )
			args.push_back( new ofxOscArgSymbol( other.getArgAsSymbol( i ) ) );
		else if ( argType == OFXOSC_TYPE_CHAR )
			args.push_back( new ofxOscArgChar( other.getArgAsChar( i ) ) );
		else if ( argType == OFXOSC_TYPE_MIDI_MESSAGE )
			args.push_back( new ofxOscArgMidiMessage( other.getArgAsMidiMessage( i ) ) );
		else if ( argType == OFXOSC_TYPE_TRIGGER )
			args.push_back( new ofxOscArgTrigger() );
		else if ( argType == OFXOSC_TYPE_TIMETAG )
			args.push_back( new ofxOscArgTimetag( other.getArgAsTimetag( i ) ) );
		else if ( argType == OFXOSC_TYPE_BLOB )
			args.push_back( new ofxOscArgBlob( other.getArgAsBlob( i ) ) );
		else if ( argType == OFXOSC_TYPE_RGBA_COLOR )
			args.push_back( new ofxOscArgRgbaColor( other.getArgAsRgbaColor( i ) ) );
		else
		{
			assert( false && "bad argument type" );
		}
	}

	return *this;
}
