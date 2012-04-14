/*
	oscpack -- Open Sound Control packet manipulation library
	http://www.audiomulch.com/~rossb/oscpack

	Copyright (c) 2004-2005 Ross Bencina <rossb@audiomulch.com>

	Permission is hereby granted, free of charge, to any person obtaining
	a copy of this software and associated documentation files
	(the "Software"), to deal in the Software without restriction,
	including without limitation the rights to use, copy, modify, merge,
	publish, distribute, sublicense, and/or sell copies of the Software,
	and to permit persons to whom the Software is furnished to do so,
	subject to the following conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	Any person wishing to distribute modifications to the Software is
	requested to send the modifications to the original developer so that
	they can be incorporated into the canonical version.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
	ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
	CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef INCLUDED_OSCOUTBOUNDPACKET_H
#define INCLUDED_OSCOUTBOUNDPACKET_H

#include "OscTypes.h"
#include "OscException.h"


namespace osc{

class OutOfBufferMemoryException : public Exception{
public:
    OutOfBufferMemoryException( const char *w="out of buffer memory" )
        : Exception( w ) {}
};

class BundleNotInProgressException : public Exception{
public:
    BundleNotInProgressException(
            const char *w="call to EndBundle when bundle is not in progress" )
        : Exception( w ) {}
};

class MessageInProgressException : public Exception{
public:
    MessageInProgressException(
            const char *w="opening or closing bundle or message while message is in progress" )
        : Exception( w ) {}
};

class MessageNotInProgressException : public Exception{
public:
    MessageNotInProgressException(
            const char *w="call to EndMessage when message is not in progress" )
        : Exception( w ) {}
};


class OutboundPacketStream{
public:
	OutboundPacketStream( char *buffer, unsigned long capacity );
	~OutboundPacketStream();

    void Clear();

    unsigned int Capacity() const;

    // invariant: size() is valid even while building a message.
    unsigned int Size() const;

    const char *Data() const;

    // indicates that all messages have been closed with a matching EndMessage
    // and all bundles have been closed with a matching EndBundle
    bool IsReady() const;

    bool IsMessageInProgress() const;
    bool IsBundleInProgress() const;

    OutboundPacketStream& operator<<( const BundleInitiator& rhs );
    OutboundPacketStream& operator<<( const BundleTerminator& rhs );
    
    OutboundPacketStream& operator<<( const BeginMessage& rhs );
    OutboundPacketStream& operator<<( const MessageTerminator& rhs );

    OutboundPacketStream& operator<<( bool rhs );
    OutboundPacketStream& operator<<( const NilType& rhs );
    OutboundPacketStream& operator<<( const InfinitumType& rhs );
    OutboundPacketStream& operator<<( int32 rhs );

#ifndef __x86_64__
    OutboundPacketStream& operator<<( int rhs )
            { *this << (int32)rhs; return *this; }
#endif

    OutboundPacketStream& operator<<( float rhs );
    OutboundPacketStream& operator<<( char rhs );
    OutboundPacketStream& operator<<( const RgbaColor& rhs );
    OutboundPacketStream& operator<<( const MidiMessage& rhs );
    OutboundPacketStream& operator<<( int64 rhs );
    OutboundPacketStream& operator<<( const TimeTag& rhs );
    OutboundPacketStream& operator<<( double rhs );
    OutboundPacketStream& operator<<( const char* rhs );
    OutboundPacketStream& operator<<( const Symbol& rhs );
    OutboundPacketStream& operator<<( const Blob& rhs );

private:

    char *BeginElement( char *beginPtr );
    void EndElement( char *endPtr );

    bool ElementSizeSlotRequired() const;
    void CheckForAvailableBundleSpace();
    void CheckForAvailableMessageSpace( const char *addressPattern );
    void CheckForAvailableArgumentSpace( long argumentLength );

    char *data_;
    char *end_;

    char *typeTagsCurrent_; // stored in reverse order
    char *messageCursor_;
    char *argumentCurrent_;

    // elementSizePtr_ has two special values: 0 indicates that a bundle
    // isn't open, and elementSizePtr_==data_ indicates that a bundle is
    // open but that it doesn't have a size slot (ie the outermost bundle)
    uint32 *elementSizePtr_;

    bool messageIsInProgress_;
};

} // namespace osc

#endif /* INCLUDED_OSC_OUTBOUND_PACKET_H */
