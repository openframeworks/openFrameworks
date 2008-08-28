/*
 *  ofxOscSender.cpp
 *  openFrameworks OSC addon
 *
 *  damian@frey.co.nz
 *
 */

#include "ofxOscSender.h"

#include "OscTypes.h"
#include "OscOutboundPacketStream.h"

#include <UdpSocket.h>

#include <assert.h>

ofxOscSender::ofxOscSender()
{
}

ofxOscSender::~ofxOscSender()
{
}

void ofxOscSender::setup( const char* hostname, int port )
{
	socket = new UdpTransmitSocket( IpEndpointName( hostname, port ) );
}

void ofxOscSender::sendMessage( const ofxOscMessage& message )
{
	static const int OUTPUT_BUFFER_SIZE = 16384;
	char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );

    p << osc::BeginBundleImmediate << osc::BeginMessage( message.getAddress() );
	for ( int i=0; i< message.getNumArgs(); ++i )
	{
		if ( message.getArgType(i) == OFXOSC_TYPE_INT32 )
			p << message.getArgAsInt32( i );
		else if ( message.getArgType( i ) == OFXOSC_TYPE_FLOAT )
			p << message.getArgAsFloat( i );
		else if ( message.getArgType( i ) == OFXOSC_TYPE_STRING )
			p << message.getArgAsString( i );
		else
		{
			assert( false && "bad argument type" );
		}
	}
	p << osc::EndMessage << osc::EndBundle;

	socket->Send( p.Data(), p.Size() );
}
