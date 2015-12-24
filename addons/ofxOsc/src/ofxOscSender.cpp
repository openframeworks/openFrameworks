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
:broadcast(true)
,port(0)
{
}

ofxOscSender::ofxOscSender(const ofxOscSender & mom)
:broadcast(mom.broadcast)
,hostname(mom.hostname)
,port(mom.port){
	if(mom.socket){
		setup(hostname,port);
	}
}

ofxOscSender & ofxOscSender::operator=(const ofxOscSender & mom){
	if(this == &mom) return *this;

	broadcast = mom.broadcast;
	hostname = mom.hostname;
	port = mom.port;
	if(mom.socket){
		setup(hostname,port);
	}
	return *this;
}

void ofxOscSender::setup( const std::string &hostname, int port )
{
    if( osc::UdpSocket::GetUdpBufferSize() == 0 ){
    	osc::UdpSocket::SetUdpBufferSize(65535);
    }

    socket.reset(new osc::UdpTransmitSocket(osc::IpEndpointName( hostname.c_str(), port), broadcast));
    this->hostname = hostname;
    this->port = port;
}

void ofxOscSender::disableBroadcast()
{
	broadcast = false;
	if(socket){
		setup(hostname, port);
	}
}

void ofxOscSender::enableBroadcast()
{
	broadcast = true;
	if(socket){
		setup(hostname, port);
	}
}

void ofxOscSender::shutdown()
{
	socket.reset();
}

void ofxOscSender::sendBundle( ofxOscBundle& bundle )
{
	if(!socket){
		ofLogError("ofxOscSender") << "trying to send before setup";
	}
    //setting this much larger as it gets trimmed down to the size its using before being sent.
    //TODO: much better if we could make this dynamic? Maybe have ofxOscBundle return its size?
	static const int OUTPUT_BUFFER_SIZE = 327680;
	char buffer[OUTPUT_BUFFER_SIZE];
	osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE );

	// serialise the bundle
	appendBundle( bundle, p );

	socket->Send( p.Data(), p.Size() );
}

void ofxOscSender::sendMessage( ofxOscMessage& message, bool wrapInBundle )
{
	if(!socket){
		ofLogError("ofxOscSender") << "trying to send before setup";
	}
    //setting this much larger as it gets trimmed down to the size its using before being sent.
    //TODO: much better if we could make this dynamic? Maybe have ofxOscMessage return its size?
    static const int OUTPUT_BUFFER_SIZE = 327680;
	char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );

	// serialise the message
	if(wrapInBundle) p << osc::BeginBundleImmediate;
	appendMessage( message, p );
	if(wrapInBundle) p << osc::EndBundle;

	socket->Send( p.Data(), p.Size() );
}

void ofxOscSender::sendParameter( const ofAbstractParameter & parameter){
	if(!parameter.isSerializable()) return;
	if(parameter.type()==typeid(ofParameterGroup).name()){
        std::string address = "/";
        const std::vector<std::string> hierarchy = parameter.getGroupHierarchyNames();
		for(int i=0;i<(int)hierarchy.size()-1;i++){
			address+=hierarchy[i] + "/";
		}
		ofxOscBundle bundle;
		appendParameter(bundle,parameter,address);
		sendBundle(bundle);
	}else{
        std::string address = "";
        const std::vector<std::string> hierarchy = parameter.getGroupHierarchyNames();
		for(int i=0;i<(int)hierarchy.size()-1;i++){
			address+= "/" + hierarchy[i];
		}
		if(address.length()) address += "/";
		ofxOscMessage msg;
		appendParameter(msg,parameter,address);
		sendMessage(msg, false);
	}
}


void ofxOscSender::appendParameter( ofxOscBundle & _bundle, const ofAbstractParameter & parameter, const std::string &address){
	if(parameter.type()==typeid(ofParameterGroup).name()){
		ofxOscBundle bundle;
		const ofParameterGroup & group = static_cast<const ofParameterGroup &>(parameter);
		for(std::size_t i=0;i<group.size();i++){
			const ofAbstractParameter & p = group[i];
			if(p.isSerializable()){
				appendParameter(bundle,p,address+group.getEscapedName()+"/");
			}
		}
		_bundle.addBundle(bundle);
	}else{
		if(parameter.isSerializable()){
			ofxOscMessage msg;
			appendParameter(msg,parameter,address);
			_bundle.addMessage(msg);
		}
	}
}

void ofxOscSender::appendParameter( ofxOscMessage & msg, const ofAbstractParameter & parameter, const std::string &address){
	msg.setAddress(address+parameter.getEscapedName());
	if(parameter.type()==typeid(ofParameter<int>).name()){
		msg.addIntArg(parameter.cast<int>());
	}else if(parameter.type()==typeid(ofParameter<float>).name()){
		msg.addFloatArg(parameter.cast<float>());
	}else if(parameter.type()==typeid(ofParameter<double>).name()){
		msg.addDoubleArg(parameter.cast<double>());
	}else if(parameter.type()==typeid(ofParameter<bool>).name()){
		msg.addBoolArg(parameter.cast<bool>());
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
		else if ( message.getArgType( i ) == OFXOSC_TYPE_DOUBLE )
			p << message.getArgAsDouble( i );
		else if ( message.getArgType( i ) == OFXOSC_TYPE_STRING || message.getArgType( i ) == OFXOSC_TYPE_SYMBOL)
			p << message.getArgAsString( i ).c_str();
		else if ( message.getArgType( i ) == OFXOSC_TYPE_CHAR )
			p << message.getArgAsChar( i );
		else if ( message.getArgType( i ) == OFXOSC_TYPE_MIDI_MESSAGE )
			p << message.getArgAsMidiMessage( i );
		else if ( message.getArgType( i ) == OFXOSC_TYPE_TRUE || message.getArgType( i ) == OFXOSC_TYPE_FALSE )
			p << message.getArgAsBool( i );
		else if ( message.getArgType( i ) == OFXOSC_TYPE_TRIGGER )
			p << message.getArgAsTrigger( i );
		else if ( message.getArgType( i ) == OFXOSC_TYPE_TIMETAG )
			p << (osc::int64)message.getArgAsTimetag( i );
		//else if ( message.getArgType( i ) == OFXOSC_TYPE_RGBA_COLOR )
		//	p << message.getArgAsRgbaColor( i );
        else if ( message.getArgType( i ) == OFXOSC_TYPE_BLOB ){
            ofBuffer buff = message.getArgAsBlob(i);
            osc::Blob b(buff.getData(), (unsigned long)buff.size());
            p << b; 
		}else
		{
			ofLogError("ofxOscSender") << "appendMessage(): bad argument type " << message.getArgType( i );
			assert( false );
		}
	}
	p << osc::EndMessage;
}
