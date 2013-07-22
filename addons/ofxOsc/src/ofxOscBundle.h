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
