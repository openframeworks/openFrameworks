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

#include "ofxOscReceiver.h"

#ifndef TARGET_WIN32
        #include <pthread.h>
#endif
#include <iostream>
#include <assert.h>

ofxOscReceiver::ofxOscReceiver()
:allowReuse(true)
,listen_port(0)
{
}

ofxOscReceiver::ofxOscReceiver(const ofxOscReceiver & mom)
:allowReuse(mom.allowReuse)
,listen_port(mom.listen_port){
	if(mom.listen_socket){
		setup(listen_port);
	}
}

ofxOscReceiver & ofxOscReceiver::operator=(const ofxOscReceiver & mom){
	if(this == &mom) return *this;

	allowReuse = mom.allowReuse;
	listen_port = mom.listen_port;
	if(mom.listen_socket){
		setup(listen_port);
	}
	return *this;
}

void ofxOscReceiver::setup( int listen_port )
{
    if( osc::UdpSocket::GetUdpBufferSize() == 0 ){
    	osc::UdpSocket::SetUdpBufferSize(65535);
    }
    
	// if we're already running, shutdown before running again
	if ( listen_socket ){
		shutdown();
	}
	
	// create socket
	auto socket = new osc::UdpListeningReceiveSocket( osc::IpEndpointName( osc::IpEndpointName::ANY_ADDRESS, listen_port ), this, allowReuse );
	auto deleter = [](osc::UdpListeningReceiveSocket*socket){
		// tell the socket to shutdown
		socket->Break();
		delete socket;
	};
	auto new_ptr = std::unique_ptr<osc::UdpListeningReceiveSocket, decltype(deleter)>(socket, deleter);
	listen_socket = std::move(new_ptr);

	listen_thread = std::thread([this]{
		listen_socket->Run();
	});

	// detach thread so we don't have to wait on it before creating a new socket
	// or on destruction, the custom deleter for the socket unique_ptr already
	// does the right thing
	listen_thread.detach();

	this->listen_port = listen_port;
}

void ofxOscReceiver::shutdown()
{
	listen_socket.reset();
}

void ofxOscReceiver::ProcessMessage( const osc::ReceivedMessage &m, const osc::IpEndpointName& remoteEndpoint )
{
	// convert the message to an ofxOscMessage
	ofxOscMessage msg;

	// set the address
	msg.setAddress( m.AddressPattern() );
    
	// set the sender ip/host
	char endpoint_host[ osc::IpEndpointName::ADDRESS_STRING_LENGTH ];
	remoteEndpoint.AddressAsString( endpoint_host );
    msg.setRemoteEndpoint( endpoint_host, remoteEndpoint.port );

	// transfer the arguments
	for ( osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
		  arg != m.ArgumentsEnd();
		  ++arg )
	{
		if ( arg->IsInt32() )
			msg.addIntArg( arg->AsInt32Unchecked() );
		else if ( arg->IsInt64() )
			msg.addInt64Arg( arg->AsInt64Unchecked() );
		else if ( arg->IsFloat() )
			msg.addFloatArg( arg->AsFloatUnchecked() );
		else if ( arg->IsDouble() )
			msg.addDoubleArg( arg->AsDoubleUnchecked() );
		else if ( arg->IsString() )
			msg.addStringArg( arg->AsStringUnchecked() );
		else if ( arg->IsSymbol() )
			msg.addSymbolArg( arg->AsSymbolUnchecked() );
		else if ( arg->IsChar() )
			msg.addCharArg( arg->AsCharUnchecked() );
		else if ( arg->IsMidiMessage() )
			msg.addMidiMessageArg( arg->AsMidiMessageUnchecked() );
		else if ( arg->IsBool())
			msg.addBoolArg( arg->AsBoolUnchecked() );
		else if ( arg->IsInfinitum() )
			msg.addTriggerArg();
		else if ( arg->IsTimeTag() )
			msg.addTimetagArg( arg->AsTimeTagUnchecked() );
		else if ( arg->IsRgbaColor() )
			msg.addRgbaColorArg( arg->AsRgbaColorUnchecked() );
		else if ( arg->IsBlob() ){
            const char * dataPtr;
            osc::osc_bundle_element_size_t len = 0;
            arg->AsBlobUnchecked((const void*&)dataPtr, len);
            ofBuffer buffer(dataPtr, len);
			msg.addBlobArg( buffer );
		}
		else
		{
			ofLogError("ofxOscReceiver") << "ProcessMessage: argument in message " << m.AddressPattern() << " is not an int, float, or string";
		}
	}

	// send msg to main thread
	messagesChannel.send( std::move(msg) );
}

bool ofxOscReceiver::hasWaitingMessages()
{
	return !messagesChannel.empty();
}

bool ofxOscReceiver::getNextMessage( ofxOscMessage * message )
{
	return getNextMessage(*message);
}

bool ofxOscReceiver::getNextMessage( ofxOscMessage & message )
{
	return messagesChannel.tryReceive(message);
}

bool ofxOscReceiver::getParameter(ofAbstractParameter & parameter)
{
	ofxOscMessage msg;
	while(messagesChannel.tryReceive(msg)){
		ofAbstractParameter * p = &parameter;
        std::vector<std::string> address = ofSplitString(msg.getAddress(),"/",true);
        for(unsigned int i=0;i<address.size();i++){
            if(p) {
                if(address[i]==p->getEscapedName()){
                    if(p->type()==typeid(ofParameterGroup).name()){
                        if(address.size()>=i+1){
                            p = &static_cast<ofParameterGroup*>(p)->get(address[i+1]);
                        }
                    }else if(p->type()==typeid(ofParameter<int>).name() && msg.getArgType(0)==OFXOSC_TYPE_INT32){
                        p->cast<int>() = msg.getArgAsInt32(0);
                    }else if(p->type()==typeid(ofParameter<float>).name() && msg.getArgType(0)==OFXOSC_TYPE_FLOAT){
                        p->cast<float>() = msg.getArgAsFloat(0);
                    }else if(p->type()==typeid(ofParameter<double>).name() && msg.getArgType(0)==OFXOSC_TYPE_DOUBLE){
                        p->cast<double>() = msg.getArgAsDouble(0);
                    }else if(p->type()==typeid(ofParameter<bool>).name() && (msg.getArgType(0)==OFXOSC_TYPE_TRUE
																			|| msg.getArgType(0)==OFXOSC_TYPE_FALSE
																			|| msg.getArgType(0)==OFXOSC_TYPE_INT32
																			|| msg.getArgType(0)==OFXOSC_TYPE_INT64
																			|| msg.getArgType(0)==OFXOSC_TYPE_FLOAT
																			|| msg.getArgType(0)==OFXOSC_TYPE_DOUBLE
																			|| msg.getArgType(0)==OFXOSC_TYPE_STRING
																			|| msg.getArgType(0)==OFXOSC_TYPE_SYMBOL)){
                        p->cast<bool>() = msg.getArgAsBool(0);
                    }else if(msg.getArgType(0)==OFXOSC_TYPE_STRING){
                        p->fromString(msg.getArgAsString(0));
                    }
                }
            }
        }
	}
	return true;
}

void ofxOscReceiver::disableReuse(){
	allowReuse = false;
	if(listen_socket){
		setup(listen_port);
	}
}

void ofxOscReceiver::enableReuse(){
	allowReuse = true;
	if(listen_socket){
		setup(listen_port);
	}
}
