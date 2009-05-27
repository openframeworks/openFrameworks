/*
    unicap
    Copyright (C) 2004  Arne Caspari

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __STATUS_H__
#define __STATUS_H__


#define STATUS_SUCCESS   0x00000000
#define STATUS_INTERNAL  0x10000000
#define STATUS_WARNING   0x40000000 
#define STATUS_FAILURE   0x80000000

#define STATUS_NO_DEVICE                         ( STATUS_FAILURE | 0x1 )
#define STATUS_CHANNEL_ALREADY_FREE              ( STATUS_FAILURE | 0x2 )
#define STATUS_INSUFFICIENT_BANDWIDTH            ( STATUS_FAILURE | 0x3 )
#define STATUS_INVALID_PARAMETER                 ( STATUS_FAILURE | 0x4 )
#define STATUS_IS_RECEIVING                      ( STATUS_FAILURE | 0x5 )
#define STATUS_IS_STOPPED                        ( STATUS_FAILURE | 0x6 )
#define STATUS_NO_VIDEO_MODE                     ( STATUS_FAILURE | 0x7 )
#define STATUS_NO_CHANNEL                        ( STATUS_FAILURE | 0x8 )
#define STATUS_TIMEOUT                           ( STATUS_FAILURE | 0x9 )
#define STATUS_NO_BUFFERS                        ( STATUS_FAILURE | 0xa )
#define STATUS_CPI_OPEN_FAILED                   ( STATUS_FAILURE | 0xb )
#define STATUS_NO_MEM                            ( STATUS_FAILURE | 0xc )
#define STATUS_INVALID_CPI                       ( STATUS_FAILURE | 0xd )
#define STATUS_UNSUPPORTED_CPI_VERSION           ( STATUS_FAILURE | 0xe )
#define STATUS_INVALID_HANDLE                    ( STATUS_FAILURE | 0xf )

#define STATUS_NOT_IMPLEMENTED                   ( STATUS_FAILURE | 0x1c )
#define STATUS_NO_MATCH                          ( STATUS_FAILURE | 0x1e )
#define STATUS_NO_FORMAT                         ( STATUS_FAILURE | 0x1f )
#define STATUS_PROPERTY_WRONG_STEPPING           ( STATUS_FAILURE | 0x20 )
#define STATUS_PROPERTY_VALUE_NOT_IN_LIST        ( STATUS_FAILURE | 0x21 )
#define STATUS_BUFFER_TOO_SMALL                  ( STATUS_FAILURE | 0x22 )
#define STATUS_UNSUPPORTED_BUFFER_TYPE           ( STATUS_FAILURE | 0x23 )

#define STATUS_INSUFFICIENT_RESOURCES            ( STATUS_FAILURE | 0x25 )
#define STATUS_FRAME_RATE_NOT_AVAILABLE          ( STATUS_FAILURE | 0x26 )
#define STATUS_PERMISSION_DENIED                 ( STATUS_FAILURE | 0x27 ) 
#define STATUS_FILE_NOT_FOUND                    ( STATUS_FAILURE | 0x28 )

#define STATUS_UNSUPPORTED_CODEC                 ( STATUS_FAILURE | 0x101 )

#define STATUS_RAW1394_NO_KERNEL_SUPPORT         ( STATUS_FAILURE | 0x80 )
#define STATUS_RAW1394_NO_CARDS                  ( STATUS_FAILURE | 0x81 )

#define STATUS_INCOMPATIBLE_MAJOR_VERSION        ( STATUS_FAILURE | 0xf00 )
#define STATUS_INCOMPATIBLE_MINOR_VERSION        ( STATUS_FAILURE | 0xf01 )
#define STATUS_INCOMPATIBLE_MICRO_VERSION        ( STATUS_FAILURE | 0xf02 )






#define STATUS_CAPTURE_ALREADY_STARTED           ( STATUS_WARNING | 0x1 )
#define STATUS_CAPTURE_ALREADY_STOPPED           ( STATUS_WARNING | 0x2 )
#define STATUS_WARN_DEVICE_QUERY_FAIL            ( STATUS_WARNING | 0x3 )


#define STATUS_SKIP_CTRL                         ( STATUS_INTERNAL | 0x1 )



#define SUCCESS( x ) ( ( (x) & 0xff000000 ) == 0x0 )

typedef int unicap_status_t;

#endif //__STATUS_H__
