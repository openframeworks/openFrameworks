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
#include "OscPrintReceivedElements.h"

#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstring>


namespace osc{


std::ostream& operator<<( std::ostream & os,
        const ReceivedMessageArgument& arg )
{
    switch( arg.TypeTag() ){
        case TRUE_TYPE_TAG:
            os << "bool:true";
            break;
                
        case FALSE_TYPE_TAG:
            os << "bool:false";
            break;

        case NIL_TYPE_TAG:
            os << "(Nil)";
            break;

        case INFINITUM_TYPE_TAG:
            os << "(Infinitum)";
            break;

        case INT32_TYPE_TAG:
            os << "int32:" << arg.AsInt32Unchecked();
            break;

        case FLOAT_TYPE_TAG:
            os << "float32:" << arg.AsFloatUnchecked();
            break;

        case CHAR_TYPE_TAG:
            {
                char s[2] = {0};
                s[0] = arg.AsCharUnchecked();
                os << "char:'" << s << "'";
            }
            break;

        case RGBA_COLOR_TYPE_TAG:
            {
                uint32 color = arg.AsRgbaColorUnchecked();
                
                os << "RGBA:0x"
                        << std::hex << std::setfill('0')
                        << std::setw(2) << (int)((color>>24) & 0xFF)
                        << std::setw(2) << (int)((color>>16) & 0xFF)
                        << std::setw(2) << (int)((color>>8) & 0xFF)
                        << std::setw(2) << (int)(color & 0xFF)
                        << std::setfill(' ');
                os.unsetf(std::ios::basefield);
            }
            break;

        case MIDI_MESSAGE_TYPE_TAG:
            {
                uint32 m = arg.AsMidiMessageUnchecked();
                os << "midi (port, status, data1, data2):<<"
                        << std::hex << std::setfill('0')
                        << "0x" << std::setw(2) << (int)((m>>24) & 0xFF)
                        << " 0x" << std::setw(2) << (int)((m>>16) & 0xFF)
                        << " 0x" << std::setw(2) << (int)((m>>8) & 0xFF)
                        << " 0x" << std::setw(2) << (int)(m & 0xFF)
                        << std::setfill(' ') << ">>";
                os.unsetf(std::ios::basefield);
            }
            break;
				
        case INT64_TYPE_TAG:
            os << "int64:" << arg.AsInt64Unchecked();
            break;

        case TIME_TAG_TYPE_TAG:
            {
                os << "OSC-timetag:" << arg.AsTimeTagUnchecked();

                std::time_t t =
                        (unsigned long)( arg.AsTimeTagUnchecked() >> 32 );

                // strip trailing newline from string returned by ctime
                const char *timeString = std::ctime( &t );
                size_t len = strlen( timeString );
                char *s = new char[ len + 1 ];
                strcpy( s, timeString );
                if( len )
                    s[ len - 1 ] = '\0';
                    
                os << " " << s;
            }
            break;
                
        case DOUBLE_TYPE_TAG:
            os << "double:" << arg.AsDoubleUnchecked();
            break;

        case STRING_TYPE_TAG:
            os << "OSC-string:`" << arg.AsStringUnchecked() << "'";
            break;
                
        case SYMBOL_TYPE_TAG: 
            os << "OSC-string (symbol):`" << arg.AsSymbolUnchecked() << "'";
            break;

        case BLOB_TYPE_TAG:
            {
                unsigned long size;
                const void *data;
                arg.AsBlobUnchecked( data, size );
                os << "OSC-blob:<<" << std::hex << std::setfill('0');
                unsigned char *p = (unsigned char*)data;
                for( unsigned long i = 0; i < size; ++i ){
                    os << "0x" << std::setw(2) << int(p[i]);
                    if( i != size-1 )
                        os << ' ';
                }
                os.unsetf(std::ios::basefield);
                os << ">>" << std::setfill(' ');
            }
            break;

        default:
            os << "unknown";
    }

    return os;
}


std::ostream& operator<<( std::ostream & os, const ReceivedMessage& m )
{
    os << "[";
    if( m.AddressPatternIsUInt32() )
        os << m.AddressPatternAsUInt32();
    else
        os << m.AddressPattern();
    
    bool first = true;
    for( ReceivedMessage::const_iterator i = m.ArgumentsBegin();
            i != m.ArgumentsEnd(); ++i ){
        if( first ){
            os << " ";
            first = false;
        }else{
            os << ", ";
        }

        os << *i;
    }

    os << "]";

    return os;
}


std::ostream& operator<<( std::ostream & os, const ReceivedBundle& b )
{
    static int indent = 0;

    for( int j=0; j < indent; ++j )
        os << "  ";
    os << "{ ( ";
    if( b.TimeTag() == 1 )
        os << "immediate";
    else
        os << b.TimeTag();
    os << " )\n";

    ++indent;
    
    for( ReceivedBundle::const_iterator i = b.ElementsBegin();
            i != b.ElementsEnd(); ++i ){
        if( i->IsBundle() ){
            ReceivedBundle b(*i);
            os << b << "\n";
        }else{
            ReceivedMessage m(*i);
            for( int j=0; j < indent; ++j )
                os << "  ";
            os << m << "\n";
        }
    }

    --indent;

    for( int j=0; j < indent; ++j )
        os << "  ";
    os << "}";

    return os;
}


std::ostream& operator<<( std::ostream & os, const ReceivedPacket& p )
{
    if( p.IsBundle() ){
        ReceivedBundle b(p);
        os << b << "\n";
    }else{
        ReceivedMessage m(p);
        os << m << "\n";
    }

    return os;
}

} // namespace osc
