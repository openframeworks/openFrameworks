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

#ifndef _OFXOSCBUNDLE_H
#define _OFXOSCBUNDLE_H

#include <vector>
#include "ofxOscMessage.h"

class ofxOscBundle
{
public:	
	ofxOscBundle();
	~ofxOscBundle();
	ofxOscBundle( const ofxOscBundle& other ) { copy ( other ); }
	ofxOscBundle& operator= ( const ofxOscBundle& other ) { return copy( other ); }
	/// for operator= and copy constructor
	ofxOscBundle& copy( const ofxOscBundle& other );
	
	/// erase contents
	void clear() { messages.clear(); bundles.clear(); }

	/// add bundle elements
	void addBundle( const ofxOscBundle& element );
	void addMessage( const ofxOscMessage& message );
	
	/// get bundle elements
	int getBundleCount() const { return bundles.size(); }
	int getMessageCount() const { return messages.size(); }
	/// return the bundle or message at the given index
	ofxOscBundle& getBundleAt( int i ) { return bundles[i]; }
	ofxOscMessage& getMessageAt( int i ) { return messages[i]; }
	
private:
		
	std::vector< ofxOscMessage > messages;
	std::vector< ofxOscBundle > bundles;
};


#endif

