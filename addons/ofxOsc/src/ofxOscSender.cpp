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


#include "ofxOscSender.h"
#include "ofUtils.h"
#include "ofParameterGroup.h"


#include "UdpSocket.h"

#include <assert.h>

ofxOscSender::ofxOscSender()
{
	socket = NULL;
}

ofxOscSender::~ofxOscSender()
{
	if ( socket )
		shutdown();
}

void ofxOscSender::setup( std::string hostname, int port )
{
	if ( socket )
		shutdown();
	
	socket = new UdpTransmitSocket( IpEndpointName( hostname.c_str(), port ) );
}

void ofxOscSender::shutdown()
{
	if ( socket )
		delete socket;
	socket = NULL;
}

void ofxOscSender::sendBundle( ofxOscBundle& bundle )
{
	static const int OUTPUT_BUFFER_SIZE = 32768;
	char buffer[OUTPUT_BUFFER_SIZE];
	osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE );

	// serialise the bundle
	appendBundle( bundle, p );

	socket->Send( p.Data(), p.Size() );
}

void ofxOscSender::sendMessage( ofxOscMessage& message )
{
	static const int OUTPUT_BUFFER_SIZE = 16384;
	char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );

	// serialise the message
	p << osc::BeginBundleImmediate;
	appendMessage( message, p );
	p << osc::EndBundle;

	socket->Send( p.Data(), p.Size() );
}

void ofxOscSender::sendParameter( const ofAbstractParameter & parameter){
	if(parameter.type()==typeid(ofParameterGroup).name()){
		string address = "/";
		const vector<string> hierarchy = parameter.getGroupHierarchyNames();
		for(int i=0;i<(int)hierarchy.size()-1;i++){
			address+=hierarchy[i] + "/";
		}
		ofxOscBundle bundle;
		appendParameter(bundle,parameter,address);
		sendBundle(bundle);
	}else{
		string address = "";
		const vector<string> hierarchy = parameter.getGroupHierarchyNames();
		for(int i=0;i<(int)hierarchy.size()-1;i++){
			address+= "/" + hierarchy[i];
		}
		if(address.length()) address += "/";
		ofxOscMessage msg;
		appendParameter(msg,parameter,address);
		sendMessage(msg);
	}
}


void ofxOscSender::appendParameter( ofxOscBundle & _bundle, const ofAbstractParameter & parameter, string address){
	if(parameter.type()==typeid(ofParameterGroup).name()){
		ofxOscBundle bundle;
		const ofParameterGroup & group = static_cast<const ofParameterGroup &>(parameter);
		for(int i=0;i<group.size();i++){
			const ofAbstractParameter & p = group[i];
			appendParameter(bundle,p,address+group.getName()+"/");
		}
		_bundle.addBundle(bundle);
	}else{
		ofxOscMessage msg;
		appendParameter(msg,parameter,address);
		_bundle.addMessage(msg);
	}
}

void ofxOscSender::appendParameter( ofxOscMessage & msg, const ofAbstractParameter & parameter, string address){
	msg.setAddress(address+parameter.getName());
	if(parameter.type()==typeid(ofParameter<int>).name()){
		msg.addIntArg(parameter.cast<int>());
	}else if(parameter.type()==typeid(ofParameter<float>).name()){
		msg.addFloatArg(parameter.cast<float>());
	}else if(parameter.type()==typeid(ofParameter<bool>).name()){
		msg.addIntArg(parameter.cast<bool>());
	}else{
		msg.addStringArg(parameter.toString());
	}
}

void ofxOscSender::appendBundle( ofxOscBundle& bundle, osc::OutboundPacketStream& p )
{
	// recursively serialise the bundle
	p << osc::BeginBundleImmediate;
	for ( int i=0; i<bundle.getBundleCount(); i++ )
	{
		appendBundle( bundle.getBundleAt( i ), p );
	}
	for ( int i=0; i<bundle.getMessageCount(); i++ )
	{
		appendMessage( bundle.getMessageAt( i ), p );
	}
	p << osc::EndBundle;
}

void ofxOscSender::appendMessage( ofxOscMessage& message, osc::OutboundPacketStream& p )
{
    p << osc::BeginMessage( message.getAddress().c_str() );
	for ( int i=0; i< message.getNumArgs(); ++i )
	{
		if ( message.getArgType(i) == OFXOSC_TYPE_INT32 )
			p << message.getArgAsInt32( i );
		else if ( message.getArgType(i) == OFXOSC_TYPE_INT64 )
			p << (osc::int64)message.getArgAsInt64( i );
		else if ( message.getArgType( i ) == OFXOSC_TYPE_FLOAT )
			p << message.getArgAsFloat( i );
		else if ( message.getArgType( i ) == OFXOSC_TYPE_STRING )
			p << message.getArgAsString( i ).c_str();
		else
		{
			ofLogError("ofxOscSender") << "appendMessage(): bad argument type " << message.getArgType( i );
			assert( false );
		}
	}
	p << osc::EndMessage;
}
