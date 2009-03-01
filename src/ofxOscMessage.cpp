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
	assert( index < (int)args.size() && "index out of bounds" );
	return args[index]->getType();
}

const char* ofxOscMessage::getArgTypeName( int index ) const
{
	assert( index >=0 && index < (int)args.size() );
	return args[index]->getTypeName();
}


int32_t ofxOscMessage::getArgAsInt32( int index ) const
{
	assert( index >=0 && index < (int)args.size() );
	assert( getArgType(index) == OFXOSC_TYPE_INT32 && "wrong argument type" );
	return ((ofxOscArgInt32*)args[index])->get();
}


float ofxOscMessage::getArgAsFloat( int index ) const
{
	assert( index >=0 && index < (int)args.size() );
	assert( getArgType(index) == OFXOSC_TYPE_FLOAT && "wrong argument type" );
	return ((ofxOscArgFloat*)args[index])->get();
}


const char* ofxOscMessage::getArgAsString( int index ) const
{
	assert( index >=0 && index < (int)args.size() );
	assert( getArgType(index) == OFXOSC_TYPE_STRING && "wrong argument type" );
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

void ofxOscMessage::addStringArg( const char* argument )
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
