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

#include "ofxOscBundle.h"


ofxOscBundle::ofxOscBundle()
{
}

ofxOscBundle::~ofxOscBundle()
{
}

ofxOscBundle& ofxOscBundle::copy( const ofxOscBundle& other )
{
	for ( int i=0; i<other.bundles.size(); i++ )
	{
		bundles.push_back( other.bundles[i] );
	}
	for ( int i=0; i<other.messages.size(); i++ )
	{
		messages.push_back( other.messages[i] );
	}
	return *this;
}



void ofxOscBundle::addBundle( const ofxOscBundle& bundle )
{
	bundles.push_back( bundle );
}

void ofxOscBundle::addMessage( const ofxOscMessage& message )
{
	messages.push_back( message );
}

