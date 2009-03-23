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

#include "ofxOscMessage.h"
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
        fprintf(stderr,"ofxOscMessage::getArgType: index %d out of bounds\n", index );
        return OFXOSC_TYPE_INDEXOUTOFBOUNDS;
    }
    else
        return args[index]->getType();
}

string ofxOscMessage::getArgTypeName( int index ) const
{
    if ( index >= (int)args.size() )
    {
        fprintf(stderr,"ofxOscMessage::getArgTypeName: index %d out of bounds\n", index );
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
            fprintf(stderr, "ofxOscMessage:getArgAsInt32: warning: converting int32 to float for argument %i\n", index );
            return ((ofxOscArgFloat*)args[index])->get();
        }
        else
        {
            fprintf(stderr, "ofxOscMessage:getArgAsInt32: error: argument %i is not a number\n", index );
            return 0;
        }
	}
	else
        return ((ofxOscArgInt32*)args[index])->get();
}


float ofxOscMessage::getArgAsFloat( int index ) const
{
	if ( getArgType(index) != OFXOSC_TYPE_FLOAT )
	{
	    if ( getArgType( index ) == OFXOSC_TYPE_INT32 )
        {
            fprintf(stderr, "ofxOscMessage:getArgAsFloat: warning: converting float to int32 for argument %i\n", index );
            return ((ofxOscArgInt32*)args[index])->get();
        }
        else
        {
            fprintf(stderr, "ofxOscMessage:getArgAsFloat: error: argument %i is not a number\n", index );
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
            fprintf(stderr, "ofxOscMessage:getArgAsString: warning: converting float to string for argument %i\n", index );
            return buf;
        }
	    else if ( getArgType( index ) == OFXOSC_TYPE_INT32 )
        {
            char buf[1024];
            sprintf(buf,"%i",((ofxOscArgInt32*)args[index])->get() );
            fprintf(stderr, "ofxOscMessage:getArgAsString: warning: converting int32 to string for argument %i\n", index );
            return buf;
        }
        else
        {
            fprintf(stderr, "ofxOscMessage:getArgAsString: error: argument %i is not a string\n", index );
            return "";
        }
	}
	else
        return ((ofxOscArgString*)args[index])->get();
}



/*

set methods

*/


void ofxOscMessage::addIntArg( int32_t argument )
{

	args.push_back( new ofxOscArgInt32( argument ) );
}

void ofxOscMessage::addFloatArg( float argument )
{
	args.push_back( new ofxOscArgFloat( argument ) );
}

void ofxOscMessage::addStringArg( string argument )
{
	args.push_back( new ofxOscArgString( argument ) );
}


/*

 housekeeping

 */

ofxOscMessage& ofxOscMessage::copy( const ofxOscMessage& other )
{
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
		else if ( argType == OFXOSC_TYPE_FLOAT )
			args.push_back( new ofxOscArgFloat( other.getArgAsFloat( i ) ) );
		else if ( argType == OFXOSC_TYPE_STRING )
			args.push_back( new ofxOscArgString( other.getArgAsString( i ) ) );
		else
		{
			assert( false && "bad argument type" );
		}
	}

	return *this;
}
