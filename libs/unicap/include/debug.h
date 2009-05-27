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

#ifndef __DEBUG_H__
#define __DEBUG_H__

// define DEBUG if you want debug output

#ifdef DEBUG
#include <stdio.h>
#define TRACE( x... ) {printf( "%s %s (%d) :", __FILE__, __FUNCTION__, __LINE__ ); printf( x );}
#define ASSERT( x ) {if(!x) { printf( "assertion failed: %s %s (%d)", __FILE__, __FUNCTION__, __LINE__ ); exit( -1 );} }
#define DBGOUT( x... ) {printf(x);}
#else
#define TRACE( x... )
#define ASSERT( x )
#define DBGOUT( x... )
#endif

#define ERROR( x... ) fprintf( stderr, "% %s (%d) :", __FILE__, __FUNCTION__, __LINE__ ); printf( x )

#endif//__DEBUG_H__
