/*******************************************************************************
 
CreEPS = Creating EPS  Version 2.00
 
Uwe Fabricius : http://uwefabricius.de/
Thomas Pohl   : http://thomas-pohl.info/
 
Copyright (c) 2002 - 2009  Uwe Fabricius, Thomas Pohl

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*******************************************************************************/

#include "CreEPS.hpp"

namespace ns_creeps
{

// Comment in the following line to get umlaut support for many common
// umlauts. If you plan to use umlauts only with LaTeX font output,
// leave it disabled. Enabling umlaut support increases the size of
// the resulting EPS files by a constant size of about 500 bytes.

#define ENABLE_UMLAUT

// Size of the buffer, that is used to embed an external
// EPS file. Used in function CreEPS::embedEPS

#ifndef  CREEPS_EMBEDEPS_BUFF_SIZE
#define  CREEPS_EMBEDEPS_BUFF_SIZE  1024
#endif

// The comment below is used by the Makefile. Do not change layout and
// keep version string consistent. 
// MAKE_RELEASE CREEPS_VERSION  2.00
#define  CREEPS_MAJOR_VERSION  2
#define  CREEPS_MINOR_VERSION  0

/*********************************************************/

#define  CREATE_VERSION_STRING( MAJORVERSIOM, MINORVERSION ) \
	#MAJORVERSIOM "." #MINORVERSION
#define  CREEPS_VERSION_STR \
	CREATE_VERSION_STRING(CREEPS_MAJOR_VERSION, CREEPS_MINOR_VERSION)

/*********************************************************/

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

/*********************************************************
 * The attribute classes for CreEPS
 *********************************************************/

CAt::CAt()
	: m_LineThickness(-1),
	  m_LineDashOffset(-1),
	  m_LineDashFactor(-1),
	  m_Red(-1), m_Green(-1), m_Blue(-1),
	  m_BgRed(-1), m_BgGreen(-1), m_BgBlue(-1),
	  m_FontScale(-1),
	  m_ff1(-1), m_ff2(-1), m_ff3(-1),
	  m_LineJoin(LJ_KEEP),
	  m_LineCap(LC_KEEP),
	  m_LineDashType(LD_KEEP),
	  m_TextAlignment(TA_KEEP),
	  m_FillingPattern(FP_KEEP)
{
	m_FontString[0] = '-';
	m_FontString[1] = '1';
	m_FontString[2] =   0;
	
	m_LineDashString[0] = 0;
}

CAt::CAt( const CAt& A )
	: m_LineThickness( A.m_LineThickness ),
	  m_LineDashOffset( A.m_LineDashOffset ),
	  m_LineDashFactor( A.m_LineDashFactor ),
	  m_Red( A.m_Red ),
	  m_Green( A.m_Green ),
	  m_Blue( A.m_Blue ),
	  m_BgRed( A.m_BgRed ),
	  m_BgGreen( A.m_BgGreen ),
	  m_BgBlue( A.m_BgBlue ),
	  m_FontScale( A.m_FontScale ),
	  m_ff1( A.m_ff1 ),
	  m_ff2( A.m_ff2 ),
	  m_ff3( A.m_ff3 ),
	  m_LineJoin( A.m_LineJoin ),
	  m_LineCap( A.m_LineCap ),
	  m_LineDashType( A.m_LineDashType ),
	  m_TextAlignment( A.m_TextAlignment ),
	  m_FillingPattern( A.m_FillingPattern )
{
	for( int i = 0; i < CREEPS_MAX_FONT_STRING_LENGTH; i++ ) {
		m_FontString[i] = A.m_FontString[i];
	}
	
	for( int j = 0; j < CREEPS_MAX_DASH_STRING_LENGTH; j++ ) {
		m_LineDashString[j] = A.m_LineDashString[j];
	}
}

/*********************************************************/

bool
CAt::inheritAttributes( const CAt &A,
                        FILE      *file,
                        bool       gsave_written )
{
    bool changed = gsave_written;

	changed |= inheritLineThickness( A, file, changed );
	changed |= inheritLineJoin( A, file, changed );
	changed |= inheritLineCap( A, file, changed );
	changed |= inheritLineDash( A, file, changed );
	
	// do not use the return value of inheritBackgroundColor,
	// because there no "gsave" is written
	
	inheritBackgroundColor( A, file );
	changed |= inheritColor( A, file, changed );
	changed |= inheritFont( A, file, changed );
	
	// do not use the return value of inheritTextAlignment,
	// because even if the alignment has been changed, no
	// "gsave" has been written, because the alignment is
	// not set by a PostScript command
	
	inheritTextAlignment( A, file );
	changed |= inheritFillingPattern( A, file, changed );
    
    return changed;
}

/*********************************************************/

bool
CAt::writeChanges( const CAt& A,
                   FILE* file )
{
    CAt temp( *this );
    return  temp.inheritAttributes( A, file, false );
}

/*********************************************************/

bool
CAt::setAttributes( const CAt& A,
                    FILE* file )
{
	return inheritAttributes( A, file, true );
}

/*********************************************************/

CAt CAt::operator | ( const CAt& attributes )
{
	CAt TempAttributes( *this );
	TempAttributes.inheritAttributes( attributes );
	return TempAttributes;
}

/*********************************************************/

void CAt::startFillingBox( FILE* file,
                           const CreEPS_FLOAT width,
                           const CreEPS_FLOAT height )
{
	fprintf( file,
	         CreEPS_CC("<<\n/PatternType 1 /PaintType 1 /TilingType 1\n")
	         CreEPS_CC("/BBox[0  0 %g %g]\n")
	         CreEPS_CC("/XStep %g /YStep %g\n")
	         CreEPS_CC("/PaintProc { begin x "),
	         width, height,
	         width, height  );
	
	if( m_BgRed >= 0 && m_BgGreen >= 0 && m_BgBlue >= 0 )
	{
		fprintf( file,
		         CreEPS_CC("%g %g %g sc 0 0 %g %g rf "),
		         m_BgRed, m_BgGreen, m_BgBlue, width, height );
	}

	if( m_Red >= 0 && m_Green >= 0 && m_Blue >= 0 )
	{
	    fprintf( file,
	             CreEPS_CC("%g %g %g sc "),
	             m_Red, m_Green, m_Blue );
	}
}

/*********************************************************/

void CAt::endFillingBox( FILE* file )
{
	fprintf( file, CreEPS_CC(" y end }\n") );
}

/*********************************************************/

bool
CAt::inheritLineThickness( const CAt &A,
                           FILE      *file,
                           bool       gsave_written )
{
	bool changed = false;

	if( A.m_LineThickness != -1              &&
	    A.m_LineThickness != m_LineThickness    )
	{
		m_LineThickness = A.m_LineThickness;
		
		if( file != NULL ) {
			if( gsave_written == false ) {
				fprintf( file, CreEPS_CC("x\n") );
			}
			fprintf( file, CreEPS_CC("%g sw\n"), m_LineThickness );
		}
		
		changed = true;
	}

	return changed;
}

/*********************************************************/

bool
CAt::inheritLineJoin( const CAt &A,
                      FILE      *file,
                      bool       gsave_written )
{
	bool changed = false;

	if( A.m_LineJoin != LJ_KEEP    &&
	    A.m_LineJoin != m_LineJoin    )
	{
		m_LineJoin = A.m_LineJoin;
		
		if( file != NULL ) {
			if( gsave_written == false ) {
				fprintf( file, CreEPS_CC("x\n") );
			}
			fprintf( file, CreEPS_CC("%d sj\n"), m_LineJoin );
		}
		
		changed = true;
	}

	return changed;
}

/*********************************************************/

bool
CAt::inheritLineCap( const CAt &A,
                     FILE      *file,
                     bool       gsave_written )
{
	bool changed = false;

	if( A.m_LineCap != LC_KEEP   &&
	    A.m_LineCap != m_LineCap    )
	{
		m_LineCap = A.m_LineCap;
		
		if( file != NULL ) {
			if( gsave_written == false ) {
				fprintf( file, CreEPS_CC("x\n") );
			}
			fprintf( file, CreEPS_CC("%d sp\n"), m_LineCap );
		}
		
		changed = true;
	}

	return changed;
}

/*********************************************************/

bool
CAt::inheritLineDash( const CAt &A,
                      FILE      *file,
                      bool       gsave_written )
{
	bool changed = false;
	CAt tempA( A );
	
	// line dashes set with the enum-constant
	if( (tempA.m_LineDashType   != LD_KEEP            ) &&
	    (tempA.m_LineDashType   != m_LineDashType   ||
	     tempA.m_LineDashFactor != m_LineDashFactor ||
	     tempA.m_LineThickness  != m_LineThickness  ||
	     tempA.m_LineCap        != m_LineCap          )    )
	{
		m_LineDashType = tempA.m_LineDashType;
		if( tempA.m_LineDashFactor >= 1 ) {
			m_LineDashFactor = tempA.m_LineDashFactor;
		}
		m_LineDashOffset = 0;
		
		float solid_dot     = 0,
		      zero_dist     = 0,
		      linethickness = tempA.m_LineThickness,
		      factor        = m_LineDashFactor > 1 ? m_LineDashFactor : 1;
		int   linecap       = tempA.m_LineCap >= 0 ? tempA.m_LineCap : m_LineCap;
		
		if( linethickness < 0 )  linethickness = m_LineThickness;
		if( linecap       < 0 )  linecap       = (LINECAP)BUTT;
		
		switch( linecap )
		{
			case BUTT :
				solid_dot = linethickness;
				break;
			case LC_ROUND :
				zero_dist = linethickness;
				break;
			case SQUARE :
				solid_dot = 0.0001;
				zero_dist = linethickness;
				break;
			default : break;
		}
		
		switch( m_LineDashType )
		{
			case SOLID :
				sprintf( tempA.m_LineDashString, " " );
				break;
				
			case DOT :
				sprintf( tempA.m_LineDashString,
				         CreEPS_CC("%g %g"),
				         solid_dot,
				         factor * linethickness + zero_dist );
				break;
				
			case DASH :
				sprintf( tempA.m_LineDashString,
				         CreEPS_CC("%g %g"),
				         factor * linethickness + solid_dot - linethickness,
				         factor * linethickness + zero_dist );
				break;
				
			case DOTDASH :
				sprintf( tempA.m_LineDashString,
				         CreEPS_CC("%g %g %g %g"),
				         solid_dot,
				         0.5 * factor * linethickness + zero_dist,
				               factor * linethickness + solid_dot - linethickness ,
				         0.5 * factor * linethickness + zero_dist );
				break;
				
			default : break;
		}
	}
	
	// inheriting the string
	if( ( tempA.m_LineDashString[0] != 0 ||
	      tempA.m_LineDashOffset    >= 0    )
	    &&
	    ( tempA.m_LineDashOffset != m_LineDashOffset ||
	      strncmp(tempA.m_LineDashString,
	              m_LineDashString,
	              CREEPS_MAX_DASH_STRING_LENGTH) )
	  )
	{
		memcpy( m_LineDashString,
		        tempA.m_LineDashString,
		        CREEPS_MAX_DASH_STRING_LENGTH );
		
		m_LineDashOffset = tempA.m_LineDashOffset;
		if( file != NULL ) {
			if( gsave_written == false ) {
				fprintf( file, CreEPS_CC("x\n") );
			}
			fprintf( file,
			         CreEPS_CC("[%s] %g sd\n"),
			         m_LineDashString,
			         m_LineDashOffset >= 0 ? m_LineDashOffset : 0 );
		}
		changed = true;
	}

	return changed;
}

/*********************************************************/

bool
CAt::inheritColor( const CAt &A,
                   FILE      *file,
                   bool       gsave_written )
{
	bool changed = false;

	if( A.m_Red >= 0 && m_Red != A.m_Red ) {
		m_Red = A.m_Red;
		changed = true;
	}
	if( A.m_Green >= 0 && m_Green != A.m_Green ) {
		m_Green = A.m_Green;
		changed = true;
	}
	if( A.m_Blue >= 0 && m_Blue != A.m_Blue ) {
		m_Blue = A.m_Blue;
		changed = true;
	}

	if( changed == true && file != NULL )
	{
		if( gsave_written == false )
		{
			fprintf( file, CreEPS_CC("x\n") );
		}
		if( m_Red == m_Green && m_Green == m_Blue )
		{
			fprintf( file,
			         CreEPS_CC("%g sg\n"),
			         m_Red );
		} else {
			fprintf( file,
			         CreEPS_CC("%g %g %g sc\n"),
			         m_Red, m_Green, m_Blue );
		}
	}

	return changed;
}

/*********************************************************/

bool
CAt::inheritBackgroundColor( const CAt &A,
                             FILE      *file )
{
	bool changed = false;

	// the background in A is explicitly transparent, so make
	// the background in *this attribute transparent, too
	
	if( A.m_BgRed < -1 )
	{
		if( m_BgRed != -1 )  changed = true;
		m_BgRed = m_BgGreen = m_BgBlue = -2;
	}

	// the background in A has a specifyed color, accept it

	else if( A.m_BgRed != -1 )
	{
		if( m_BgRed != A.m_BgRed ) {
			m_BgRed = A.m_BgRed;
			changed = true;
		}
		if( m_BgGreen != A.m_BgGreen ) {
			m_BgGreen = A.m_BgGreen;
			changed   = true;
		}
		if( m_BgBlue != A.m_BgBlue ) {
			m_BgBlue = A.m_BgBlue;
			changed  = true;
		}
	}

	// don't write anything, because the background color is
	// only used inside the filling pattern function

	return changed;
}

/*********************************************************/

bool
CAt::inheritFont( const CAt &A,
                  FILE      *file,
                  bool       gsave_written )
{
	bool changed = false;
	
	// check, if the font strings are different
	if( A.m_FontString[0] != '-' ) {
		for( int i = 0; i < CREEPS_MAX_FONT_STRING_LENGTH; i++ ) {
			if( m_FontString[i] != A.m_FontString[i] ) {
				changed = true;
				break;
			}
		}
	}
	
	// if the font strings are different copy the string
	if( changed ) {
		for( int j = 0; j < CREEPS_MAX_FONT_STRING_LENGTH; j++ ) {
			m_FontString[j] = A.m_FontString[j];
		}
	}
	
	// set font scale
	if( A.m_FontScale >= 1           &&
	    A.m_FontScale != m_FontScale    )
	{
		m_FontScale = A.m_FontScale;
		changed     = true;
	}
	
	if( changed == true && file != NULL )
	{
		if( gsave_written == false ) {
			fprintf( file, CreEPS_CC("x\n") );
		}
		
		fprintf( file, CreEPS_CC("%g /%s fss\n"),
		         m_FontScale < 0 ? 0.35277778 : m_FontScale*0.35277778,
		         m_FontString );
	}
	
	return changed;
}

/*********************************************************/

bool
CAt::inheritTextAlignment( const CAt &A,
                           FILE      *file )
{
	bool changed = false;
	
	if( A.m_TextAlignment != -1              &&
		A.m_TextAlignment != m_TextAlignment    )
	{
		m_TextAlignment = A.m_TextAlignment;
		changed = true;
	}

	return changed;
}

/*********************************************************/

bool
CAt::inheritFillingPattern( const CAt &A,
                            FILE      *file,
                            bool       gsave_written )
{
	bool  changed   = false;
	float BoxWidth  = 0,
	      BoxHeight = 0;

	if(    ( A.m_FillingPattern != -1               &&
	         A.m_FillingPattern != m_FillingPattern    )
	    || ( (A.m_FillingPattern == HEXDOT       ||
	          A.m_FillingPattern == CHECKERBOARD   ) &&
	         (m_ff1 != A.m_ff1 || m_ff2 != A.m_ff2 )    )
	    || ( (A.m_FillingPattern == STRIPE         ) &&
	         (m_ff1 != A.m_ff1 || m_ff2 != A.m_ff2 ||
	          m_ff3 != A.m_ff3                         ))  )
	{
		m_FillingPattern = A.m_FillingPattern;
		m_ff1 = A.m_ff1;
		m_ff2 = A.m_ff2;
		m_ff3 = A.m_ff3;

		if( file != NULL )
		{
			if( gsave_written == false )  fprintf( file, "x\n" );
			
			switch( m_FillingPattern ) {
				
				case HEXDOT :
					// m_ff1 == distance between the balls
					// m_ff2 == radius of the balls
					BoxWidth  = m_ff1;
					BoxHeight = sqrtf(3)*m_ff1;
					startFillingBox( file, BoxWidth, BoxHeight );
					fprintf( file, CreEPS_CC("%g %g %g o f ")
					               CreEPS_CC("%g %g %g o f ")
					               CreEPS_CC("%g %g %g o f ")
					               CreEPS_CC("%g %g %g o f ")
					               CreEPS_CC("%g %g %g o f "),
					                    0.0,             0.0, m_ff2,
					               BoxWidth,             0.0, m_ff2,
					         0.5 * BoxWidth, 0.5 * BoxHeight, m_ff2,
					                    0.0,       BoxHeight, m_ff2,
					               BoxWidth,       BoxHeight, m_ff2  );
					endFillingBox( file );
					break;

				case CHECKERBOARD :
					// m_ff1 == width of one field
					// m_ff2 == height of one field
					BoxWidth  = 2*m_ff1;
					BoxHeight = 2*m_ff2;
					startFillingBox( file, BoxWidth, BoxHeight );
					fprintf( file,
					         CreEPS_CC("0 0 %g %g rf %g %g %g %g rf"),
					         m_ff1, m_ff2, m_ff1, m_ff2, m_ff1, m_ff2 );
					endFillingBox( file );
					break;

				case STRIPE :
					// m_ff1 == width
					// m_ff2 == distance
					// m_ff3 == angle
					if( m_ff3 == 0 ) {
						BoxWidth  = 5,
						BoxHeight = m_ff1 + m_ff2;
						startFillingBox( file, BoxWidth, BoxHeight );
						fprintf( file, CreEPS_CC("x %g sw ")
						               CreEPS_CC("0 %g m %g %g l s y "),
						         m_ff1, BoxHeight - 0.5*m_ff1,
						         BoxWidth, BoxHeight - 0.5*m_ff1 );
					} else {
						float diff = m_ff3 - 3.14159265358979323846 / 2;
						if( diff < 0 )  diff = -diff;
						// vertical lines
						if( diff < 0.0087266 ) {
							BoxWidth  = m_ff1 + m_ff2;
							BoxHeight = 5;
							startFillingBox( file, BoxWidth, BoxHeight );
							fprintf( file, CreEPS_CC("x %g sw ")
							               CreEPS_CC("%g 0 m %g %g l s y "),
							         m_ff1, 0.5*m_ff1, 0.5*m_ff1, BoxHeight );
						// regular angle
						} else {
							BoxWidth  = (m_ff1 + m_ff2) / sin(m_ff3);
							BoxHeight = (m_ff1 + m_ff2) / cos(m_ff3);
							if( BoxHeight > 0 ) {
								startFillingBox( file, BoxWidth, BoxHeight );
								fprintf( file,
								         CreEPS_CC("x %g sw ")
								         CreEPS_CC("0 0 m %g %g l s ")
								         CreEPS_CC("%g 0 m %g %g l s ")
								         CreEPS_CC("0 %g m %g %g l s ")
								         CreEPS_CC("y "),
								          m_ff1,
								          BoxWidth,    BoxHeight,
								         -BoxWidth,    BoxWidth,  2*BoxHeight,
								         -BoxHeight, 2*BoxWidth,    BoxHeight );
							} else {
								BoxHeight = -BoxHeight;
								startFillingBox( file, BoxWidth, BoxHeight );
								fprintf( file,
								         CreEPS_CC("x %g sw ")
								         CreEPS_CC("0 %g m %g 0 l s ")
								         CreEPS_CC("0 %g m %g 0 l s ")
								         CreEPS_CC("%g %g m %g %g l s ")
								         CreEPS_CC("y "),
								          m_ff1,
								          BoxHeight,   BoxWidth,
								        2*BoxHeight, 2*BoxWidth,
								         -BoxWidth ,   BoxHeight, BoxWidth, -BoxHeight );
							}
						}
					}
					endFillingBox( file );

					break;

				default : break;
			}
			fprintf( file,
			         CreEPS_CC(">>\nmatrix makepattern\n/Pattern setcolorspace setcolor\n") );
		}
		
		changed = true;
	}
	
	return changed;
}

/*********************************************************/

CAtLineThickness::CAtLineThickness( const CreEPS_FLOAT thickness,
                                    const CAt& A )
{
	inheritAttributes( A );
	m_LineThickness = thickness >= 0 ? static_cast<float>(thickness) : -1;
}

/*********************************************************/

CAtLineJoin::CAtLineJoin( const LINEJOIN linejoin,
                          const CAt& A )
{
	inheritAttributes( A );
	m_LineJoin = linejoin;
}

/*********************************************************/

CAtLineCap::CAtLineCap( const LINECAP linecap,
                        const CAt& A )
{
	inheritAttributes( A );
	m_LineCap = linecap;
}

/*********************************************************/

CAtLineDash::CAtLineDash( const char *const dashstring,
                          const CreEPS_FLOAT offset,
                          const CAt& A )
{
	inheritAttributes( A );
	strncpy( m_LineDashString, dashstring, CREEPS_MAX_FONT_STRING_LENGTH );
	m_LineDashOffset = static_cast<float>( offset );
	m_LineDashType = LD_KEEP;
}


CAtLineDash::CAtLineDash( const LINEDASH dash,
                          const CreEPS_FLOAT factor,
                          const CAt& A )
{
	inheritAttributes( A );
	m_LineDashType      = dash;
	m_LineDashFactor    = factor;
	m_LineDashString[0] = 0;
}

/*********************************************************/

CAtColor::CAtColor( const CreEPS_FLOAT red,
                    const CreEPS_FLOAT green,
                    const CreEPS_FLOAT blue,
                    const CAt& A  )
{
	inheritAttributes( A );

	if( red   < 0 || 1 < red   ||
	    green < 0 || 1 < green ||
	    blue  < 0 || 1 < blue     )
	{
		CreEPS::getWarningMessageHandler()(
			CreEPS_CC("CreEPS: at least one of the RGB components ")
		    CreEPS_CC("(%.2f,%.2f,%.2f) exceeds the range [0,1] and will ")
		    CreEPS_CC("be truncated\n"),
		    red, green, blue );
	}

	// truncate the colors to range [0,1]
	if     ( red < 0 )  m_Red = 0;
	else if( red > 1 )  m_Red = 1;
	else                m_Red = red;
	
	if     ( green < 0 )  m_Green = 0;
	else if( green > 1 )  m_Green = 1;
	else                  m_Green = green;
	
	if     ( blue < 0 )  m_Blue = 0;
	else if( blue > 1 )  m_Blue = 1;
	else                 m_Blue = blue;
}

/*********************************************************/

CAtBackgroundColor::CAtBackgroundColor( const CreEPS_FLOAT red,
                                        const CreEPS_FLOAT green,
                                        const CreEPS_FLOAT blue,
                                        const CAt& A  )
{
	inheritAttributes( A );

	if( red   < 0 || 1 < red   ||
	    green < 0 || 1 < green ||
	    blue  < 0 || 1 < blue     )
	{
		CreEPS::getWarningMessageHandler()(
			CreEPS_CC("CreEPS: at least one of the RGB components ")
		    CreEPS_CC("(%.2f,%.2f,%.2f) exceeds the range [0,1] and will ")
		    CreEPS_CC("be truncated\n"),
		    red, green, blue );
	}

	// truncate the colors to range [0,1]
	if     ( red < 0 )  m_BgRed   = 0;
	else if( red > 1 )  m_BgRed   = 1;
	else                m_BgRed   = red;
	
	if     ( green < 0 )  m_BgGreen = 0;
	else if( green > 1 )  m_BgGreen = 1;
	else                  m_BgGreen = green;
	
	if     ( blue < 0 )  m_BgBlue  = 0;
	else if( blue > 1 )  m_BgBlue  = 1;
	else                 m_BgBlue  = blue;
}

/*********************************************************/

CAtTransparentBackground::CAtTransparentBackground( const CAt& A )
{
	inheritAttributes( A );

	m_BgRed = m_BgGreen = m_BgBlue = -2;
}

/*********************************************************/

CAtFont::CAtFont( const char *fontstring,
                  const CreEPS_FLOAT scale,
                  const CAt& A )
{
	inheritAttributes( A );
	
	if( NULL != fontstring ) {
		for( int i = 0; i < CREEPS_MAX_FONT_STRING_LENGTH; i++ ) {
			if( !(m_FontString[i] = fontstring[i]) )  break;
		}
	}

	if( scale >= 0 )  m_FontScale = scale;
}


CAtFont::CAtFont( const char *fontstring,
                  const CAt& A )
{
	inheritAttributes( A );

	if( NULL != fontstring ) {
		for( int i = 0; i < CREEPS_MAX_FONT_STRING_LENGTH; i++ ) {
			if( !(m_FontString[i] = fontstring[i]) )  break;
		}
	}
}


CAtFont::CAtFont( const CreEPS_FLOAT scale,
                  const CAt& A )
{
	inheritAttributes( A );
	if( scale >= 0 )  m_FontScale = scale;
}

/*********************************************************/

CAtGrayScale::CAtGrayScale( const CreEPS_FLOAT grayscale,
                            const CAt& A )
{
	inheritAttributes( A );
	
	if( grayscale < 0 || 1 < grayscale )
	{
		CreEPS::getWarningMessageHandler()(
			CreEPS_CC("CreEPS: grayscale value %.2f is out of range ")
		    CreEPS_CC("and will be truncated\n"),
		    grayscale );
	}
	
	if     ( grayscale < 0 )  m_Red = m_Green = m_Blue = 0;
	else if( grayscale > 1 )  m_Red = m_Green = m_Blue = 1;
	else                      m_Red = m_Green = m_Blue = grayscale;
}

/*********************************************************/

CAtTextAlignment::CAtTextAlignment( const CreEPS_INT alignment,
                                    const CAt& A )
{
	inheritAttributes( A );
	m_TextAlignment = (TEXTALIGNMENT)(alignment & 15);
}

/*********************************************************/

CAtHexDotFilling::CAtHexDotFilling( const CreEPS_FLOAT radius,
                                    const CreEPS_FLOAT distance,
                                    const CAt& A )
{
	inheritAttributes( A );
	m_FillingPattern = HEXDOT;
	m_ff1 = distance;
	m_ff2 = radius;
}

/*********************************************************/

CAtCheckerboardFilling::CAtCheckerboardFilling( const CreEPS_FLOAT x,
                                                const CreEPS_FLOAT y,
                                                const CAt& A )
{
	inheritAttributes( A );

	m_FillingPattern = CHECKERBOARD;
	m_ff1 = x;
	m_ff2 = y;
}

/*********************************************************/

CAtStripeFilling::CAtStripeFilling( const CreEPS_FLOAT width,
                                    const CreEPS_FLOAT distance,
                                    const CreEPS_INT   angle,
                                    const CAt& A )
{
	inheritAttributes( A );

	m_FillingPattern = STRIPE;

	m_ff1 = width;
	m_ff2 = distance;
	m_ff3 = angle;

	// make angle ranging in [0,180)
	while( m_ff3 <    0 )  m_ff3 += 180;
	while( m_ff3 >= 180 )  m_ff3 -= 180;
	// -> rad
	m_ff3 *= (3.14159265358979323846 / 180);
}


/*******************************************************************************
 * CreEPS                                                                      *
 ******************************************************************************/

const float CreEPS::m_INCH2MM      = 72.0 / 25.4; // = 2.834645669291...
const int   CreEPS::m_BUFFERLENGTH = 1024;

CreEPS::MessageHandler CreEPS::m_warningMessageHandler = CreEPS::StdMessageHandler;
CreEPS::MessageHandler CreEPS::m_errorMessageHandler   = CreEPS::StdMessageHandler;

/******************************************************************************/

CreEPS::CreEPS()
	: m_initialized ( false ),
	  m_FileHandle( 0x0 ),
	  m_LatexFileHandle( 0x0 )
{
}

CreEPS::CreEPS( const char  *filename,
                const CreEPS_FLOAT  startX,
                const CreEPS_FLOAT  startY,
                const CreEPS_FLOAT  endX,
                const CreEPS_FLOAT  endY,
                const CreEPS_BOOL   latex,
                const char  *altEPSFilename )

	: m_initialized ( false ),
	  m_FileHandle( 0x0 ),
	  m_LatexFileHandle( 0x0 )
{
	initialize( filename, startX, startY, endX, endY, latex, altEPSFilename );
}

/******************************************************************************/

CreEPS::CreEPS( const char  *filename,
                const CreEPS_FLOAT  width,
                const CreEPS_FLOAT  height,
		        const bool   latex,
		        const char  *altEPSFilename )

	: m_initialized ( false ),
	  m_FileHandle( 0x0 ),
	  m_LatexFileHandle( 0x0 )
{
	initialize( filename, 0, 0, width, height, latex, altEPSFilename );
}

/******************************************************************************/

void CreEPS::initialize( const char  *filename,
                         const CreEPS_FLOAT  startX,
                         const CreEPS_FLOAT  startY,
                         const CreEPS_FLOAT  endX,
                         const CreEPS_FLOAT  endY,
                         const bool   latex,
                         const char  *altEPSFilename )
{
	m_FileHandle = m_LatexFileHandle = NULL;

	m_FileHandle = fopen( filename, "w" );
	if( ! m_FileHandle )
	{
		m_errorMessageHandler( CreEPS_CC("CreEPS: Could not open output file!\n") );
		exit( 1 );
	}

	if( latex ) {
		const int length = strlen( filename );
		char* latexFilename = new char[length+3];
		strcpy( latexFilename, filename );
		latexFilename[length  ] = '_';
		latexFilename[length+1] = 't';
		latexFilename[length+2] = 0;
		
		m_LatexFileHandle = fopen( latexFilename, "w" );
		if( ! m_LatexFileHandle )
		{
			m_errorMessageHandler( CreEPS_CC("Could not open latex file!\n") );
			exit( 1 );
		}
		delete [] latexFilename;
	}

	time_t currentTime;
	time( &currentTime );

	fprintf( m_FileHandle,
	         "%%!PS-Adobe-3.0 EPSF-3.0\n"
	         "%%%%BoundingBox: %d %d %d %d\n"
	         "%%%%HiResBoundingBox: %g %g %g %g\n"
	         "%%%%Creator: CreEPS" CREEPS_VERSION_STR "\n"
	         "%%%%CreationDate: %s"
	         "%%%%EndComments\n"
	         "%%CreEPS by Uwe Fabricius, http://uwefabricius.de/ &\n"
	         "%%		  Thomas Pohl,   http://thomas-pohl.info/\n"
	         "gsave\n"
	         "/a {arc} bind def\n"
	         "/an {arcn} bind def\n"
	         "/at {arct} bind def\n"
	         "/c {closepath} bind def\n"
	         "/cc {concat} bind def\n"
	         "/cp {clip} bind def\n"
	         "/ct {curveto} bind def\n"
	         "/ec {eoclip} bind def\n"
	         "/ef {eofill} bind def\n"
	         "/f {fill} bind def\n"
	         "/fss {findfont exch scalefont setfont} bind def\n"
	         "/ic {initclip} bind def\n"
	         "/im {/OriginalCTM load setmatrix} bind def\n"
	         "/l {lineto} bind def\n"
	         "/m {moveto} bind def\n"
	         "/n {newpath} bind def\n"
	         "/o {dup 3 index add 2 index m 0 360 a c} bind def\n"
	         "/rct {curveto} bind def\n"
	         "/rf {rectfill} bind def\n"
	         "/rl {rlineto} bind def\n"
	         "/rm {rmoveto} bind def\n"
	         "/rs {rectstroke} bind def\n"
	         "/r {rotate} bind def\n"
	         "/s {stroke} bind def\n"
	         "/sc {setrgbcolor} bind def\n"
	         "/sd {setdash} bind def\n"
	         "/sg {setgray} bind def\n"
	         "/sj {setlinejoin} bind def\n"
	         "/sl {scale} bind def\n"
	         "/sp {setlinecap} bind def\n"
	         "/sw {setlinewidth} bind def\n"
	         "/t {translate} bind def\n"
	         "/x {gsave} bind def\n"
	         "/y {grestore} bind def\n"
	         "/ms {matrix currentmatrix} bind def\n"
	         "/ml {setmatrix} bind def\n"
	         "/ea {ms 8 1 roll t r sl 0 0 1 5 -2 roll a ml} bind def\n"
	         "/ean {ms 8 1 roll t r sl 0 0 1 5 -2 roll an ml} bind def\n"
	         "/e {ms 6 1 roll t r sl 1 0 m 0 0 1 0 360 a c ml} bind def\n",
	         (int)(startX*m_INCH2MM), (int)(startY*m_INCH2MM),
	         (int)(endX*m_INCH2MM), (int)(endY*m_INCH2MM),
	         startX*m_INCH2MM, startY*m_INCH2MM, endX*m_INCH2MM, endY*m_INCH2MM,
	         ctime( &currentTime ));
	
	if( ! m_LatexFileHandle /*|| true*/ )
	{ //! only for testing
		fprintf( m_FileHandle,
		         "/tacwh {x n 0 0 m dup true charpath pathbbox 3 -1 roll 4 2 roll exch sub 3 1 roll y} bind def\n"
		         "/ta {m tacwh} bind def\n"
		         "/talb {ta neg 0 exch rmoveto pop pop show} bind def\n"
		         "/tacb {ta neg 0 exch rmoveto pop 2 div neg 0 rmoveto show} bind def\n"
		         "/tarb {ta neg 0 exch rmoveto pop neg 0 rmoveto show} bind def\n"
		         "/taln {ta pop pop pop show} bind def\n"
		         "/tacn {ta pop pop 2 div neg 0 rmoveto show} bind def\n"
		         "/tarn {ta pop pop neg 0 rmoveto show} bind def\n"
		         "/talc {ta add neg 2 div 0 exch rmoveto pop show} bind def\n"
		         "/tacc {ta add neg 2 div 0 exch rmoveto 2 div neg 0 rmoveto show} bind def\n"
		         "/tarc {ta add neg 2 div 0 exch rmoveto neg 0 rmoveto show} bind def\n"
		         "/talt {ta pop neg 0 exch rmoveto pop show} bind def\n"
		         "/tact {ta pop neg 0 exch rmoveto 2 div neg 0 rmoveto show} bind def\n"
		         "/tart {ta pop neg 0 exch rmoveto neg 0 rmoveto show} bind def\n"

		          "/tar {m tacwh x 5 -1 roll r} bind def\n"
		         "/tarlb {tar neg 0 exch rmoveto pop pop show y} bind def\n"
		         "/tarcb {tar neg 0 exch rmoveto pop 2 div neg 0 rmoveto show y} bind def\n"
		         "/tarrb {tar neg 0 exch rmoveto pop neg 0 rmoveto show y} bind def\n"
		         "/tarln {tar pop pop pop show y} bind def\n"
		         "/tarcn {tar pop pop 2 div neg 0 rmoveto show y} bind def\n"
		         "/tarrn {tar pop pop neg 0 rmoveto show y} bind def\n"
		         "/tarlc {tar add neg 2 div 0 exch rmoveto pop show y} bind def\n"
		         "/tarcc {tar add neg 2 div 0 exch rmoveto 2 div neg 0 rmoveto show y} bind def\n"
		         "/tarrc {tar add neg 2 div 0 exch rmoveto neg 0 rmoveto show y} bind def\n"
		         "/tarlt {tar pop neg 0 exch rmoveto pop show y} bind def\n"
		         "/tarct {tar pop neg 0 exch rmoveto 2 div neg 0 rmoveto show y} bind def\n"
		         "/tarrt {tar pop neg 0 exch rmoveto neg 0 rmoveto show y} bind def\n"

		         "/epsb {/b4_Inc_state save def /dict_count countdictstack def /op_count count 1 sub def userdict begin /showpage { } def 0 setgray 0 setlinecap 1 setlinewidth 0 setlinejoin 10 setmiterlimit [ ] 0 setdash newpath /languagelevel where {pop languagelevel 1 ne {false setstrokeadjust false setoverprint } if } if} bind def\n"
		         "/epse {count op_count sub {pop} repeat countdictstack dict_count sub {end} repeat b4_Inc_state restore} bind def\n"
#ifdef ENABLE_UMLAUT
		         "/latin {dup findfont dup length dict begin {1 index /FID ne {def} {pop pop} ifelse} forall /Encoding ISOLatin1Encoding def currentdict end definefont pop} bind def\n"
		         "/Courier-Bold latin\n"
		         "/Times-Roman latin\n"
		         "/Times-Italic latin\n"
		         "/Times-Bold latin\n"
		         "/Times-BoldItalic latin\n"
		         "/Helvetica latin\n"
		         "/Helvetica-Oblique latin\n"
		         "/Helvetica-Bold latin\n"
		         "/Helvetica-BoldOblique latin\n"
		         "/Courier latin\n"
		         "/Courier-Oblique latin\n"
		         "/Courier-Bold latin\n"
		         "/Courier-BoldOblique latin\n"
#endif
		);
	}
	fprintf( m_FileHandle, "%g %g sl\n"
	                       "<< /OriginalCTM matrix currentmatrix >> begin\n",
	         m_INCH2MM, m_INCH2MM );

	if( m_LatexFileHandle ) {
		fprintf( m_LatexFileHandle,
		         "%%File created with CreEPS v" CREEPS_VERSION_STR "\n"
		         "%%CreEPS by Uwe Fabricius, http://uwefabricius.de/ &\n"
		         "%%		  Thomas Pohl,   http://thomas-pohl.info/\n"
		         "%%LaTeX file structure shamelessly stolen from xfig... :-)\n"
		         "\\setlength{\\unitlength}{1mm}%%\n"
		         "\\begin{picture}(0,0)%%\n"
		         "\\includegraphics{%s}%%\n"
		         "\\end{picture}%%\n"
		         "%%\n"
		         "\\begingroup\\makeatletter\\ifx\\SetFigFont\\undefined%%\n"
		         "\\gdef\\SetFigFont#1#2#3#4#5{%%\n"
		         "  \\reset@font\\fontsize{#1}{#2pt}%%\n"
		         "  \\fontfamily{#3}\\fontseries{#4}\\fontshape{#5}%%\n"
		         "  \\selectfont}%%\n"
		         "\\fi\\endgroup%%\n"
		         "\\begin{picture}(%f,%f)(%f,%f)%%\n",
		         (altEPSFilename == NULL) ? filename : altEPSFilename,
		         endX-startX, endY-startY, startX, startY );
	}

	setAttributes( CAtTextAlignment( CAtTextAlignment::DEFAULT ));
	setAttributes( CAtFont( "Times-Roman", 10 ));
	setAttributes( CAtLineThickness( 1 ));
	m_Attr.setAttributes( CAtGrayScale( 0 ), NULL);
	m_Mode = IDLE;
	m_nStoredTransformations = 0;

	m_initialized = true;
}

/******************************************************************************/

CreEPS::MessageHandler
CreEPS::setWarningMessageHandler( MessageHandler msgHandler )
{
	MessageHandler prevHandler = m_warningMessageHandler;

	m_warningMessageHandler = msgHandler;

	return prevHandler;
}

/******************************************************************************/

CreEPS::MessageHandler
CreEPS::setErrorMessageHandler( MessageHandler msgHandler )
{
	MessageHandler prevHandler = m_errorMessageHandler;

	m_errorMessageHandler = msgHandler;

	return prevHandler;
}

/******************************************************************************/

void CreEPS::finalize()
{
	if( m_initialized )
	{
		if( m_nStoredTransformations != 0 )
		{
			m_warningMessageHandler(
				CreEPS_CC("CreEPS: Finalizing file although %i transformations ")
				CreEPS_CC("are still on stack!\n"),
				m_nStoredTransformations );
				
			while( m_nStoredTransformations > 0 ) {
				loadTransformation();
			}
		}

		if( m_FileHandle )
		{
			fprintf( m_FileHandle, "y\nend\nshowpage\n%%End of CreEPS generated "
			         "file\n%%%%EOF\n" );
			fclose( m_FileHandle );
			m_FileHandle = NULL;
		}

		if( m_LatexFileHandle )
		{
			fprintf( m_LatexFileHandle, "\\end{picture}%%\n" );
			fclose( m_LatexFileHandle );
			m_LatexFileHandle = NULL;
		}

		m_initialized = false;
	}
}

/******************************************************************************/

bool CreEPS::embedEPS( const char* const filename )
{
	if( m_Mode != IDLE )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Embedding external EPS in non-IDLE mode is ")
			CreEPS_CC("forbidden!\n") );
		
		return false;
	}
	
	// open external EPS file
	FILE* file = fopen( filename, "r" );
	if( file == NULL ) {
		::printf( "CreEPS: Could not open extern EPS-file \"%s\"\n",
		          filename );
		return false;
	}

	fprintf( m_FileHandle, "%% CreEPS starts embedding \"%s\"\nepsb\n",
	           filename );

	// compensate scaling for INCH2MM
	applyScaling( 1.0/m_INCH2MM );
	fprintf( m_FileHandle, "%%%%BeginDocument: %s\n", filename );

	// allocate buffer
	char *buff = new char [CREEPS_EMBEDEPS_BUFF_SIZE];
	if( buff == NULL )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Cannot allocate buffer for embedding \"%s\"\n"),
		    filename );
		
		fclose( file );
		return false;
	}

	// copy file
	size_t nread = 0;
	do {
		nread = fread( buff, sizeof(char), CREEPS_EMBEDEPS_BUFF_SIZE, file );
		fwrite( buff, sizeof(char), nread, m_FileHandle );
	} while( nread > 0 );

	// free buffer
	delete [] buff;

	fprintf( m_FileHandle,
	           "epse\n%%%%EndDocument\n%% CreEPS ends embedding %s\n",
	           filename );
	
	fclose( file );
	return true;
}

/******************************************************************************/

const char*
CreEPS::restoreAttributes( const bool changed,
                           const bool newpath )
{
	static const char* DORESTORE        = "y\n";
	static const char* DORESTORENEWPATH = "y n\n";
	static const char* NORESTORE        = "";
	
	return (changed) ? ((newpath) ? DORESTORENEWPATH : DORESTORE) : NORESTORE;
}

/******************************************************************************/

void CreEPS::startPath( const CreEPS_FLOAT x,
                        const CreEPS_FLOAT y )
{
	if( m_Mode != IDLE )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Starting path in non-IDLE mode is forbidden!\n") );
		
		exit( 1 );
	}

	fprintf( m_FileHandle, "%g %g m\n", x, y );
	m_Mode = PATH;
}

/******************************************************************************/

void CreEPS::startPath()
{
	if( m_Mode != IDLE )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Starting path in non-IDLE mode is forbidden!\n") );
		
		exit( 1 );
	}

	m_Mode = PATH;
}

/******************************************************************************/

void CreEPS::addMove( const CreEPS_FLOAT x,
                      const CreEPS_FLOAT y )
{
	if( m_Mode != PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Adding move in non-PATH mode is forbidden!\n") );
		
		exit( 1 );
	}

	fprintf( m_FileHandle, "%g %g m\n", x, y );
}

/******************************************************************************/

void CreEPS::addRelativeMove( const CreEPS_FLOAT dx,
                              const CreEPS_FLOAT dy )
{
	if( m_Mode != PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Adding relative move in non-PATH mode is forbidden!\n") );
		
		exit( 1 );
	}

	fprintf( m_FileHandle, "%g %g rm\n", dx, dy );
}

/******************************************************************************/

void CreEPS::addLine( const CreEPS_FLOAT x,
                      const CreEPS_FLOAT y )
{
	if( m_Mode != PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Adding line in non-PATH mode is forbidden!\n") );
		
		exit( 1 );
	}

	fprintf( m_FileHandle, "%g %g l\n", x, y );
}

/******************************************************************************/

void CreEPS::addRelativeLine( const CreEPS_FLOAT dx,
                              const CreEPS_FLOAT dy )
{
	if( m_Mode != PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Adding relative line in non-PATH mode is forbidden!\n") );
		
		exit( 1 );
	}

	fprintf( m_FileHandle, "%g %g rl\n", dx, dy );
}

/******************************************************************************/

void CreEPS::addArc( const CreEPS_FLOAT x,
                     const CreEPS_FLOAT y,
                     const CreEPS_FLOAT r,
                     const CreEPS_FLOAT a1,
                     const CreEPS_FLOAT a2 )
{
	if( m_Mode != PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Adding arc in non-PATH mode is forbidden!\n") );
		
		exit( 1 );
	}

	fprintf( m_FileHandle, "%g %g %g %g %g a\n", x, y, r, a1, a2 );
}

/******************************************************************************/

void CreEPS::addArcN( const CreEPS_FLOAT x,
                      const CreEPS_FLOAT y,
                      const CreEPS_FLOAT r,
                      const CreEPS_FLOAT a1,
                      const CreEPS_FLOAT a2 )
{
	if( m_Mode != PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Adding arcN in non-PATH mode is forbidden!\n") );
		
		exit( 1 );
	}

	fprintf( m_FileHandle, "%g %g %g %g %g an\n", x, y, r, a1, a2 );
}

/******************************************************************************/

void CreEPS::addCircle( const CreEPS_FLOAT x,
                        const CreEPS_FLOAT y,
                        const CreEPS_FLOAT r )
{
	if( m_Mode != PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Adding circle in non-PATH mode is forbidden!\n") );
		
		exit( 1 );
	}

	// addMove( x+r, y ); // should no longer be necessary
	fprintf( m_FileHandle, "%g %g %g o\n", x, y, r );
}

/******************************************************************************/

void CreEPS::addEllipseArc( const CreEPS_FLOAT x,
                            const CreEPS_FLOAT y,
                            const CreEPS_FLOAT a,
                            const CreEPS_FLOAT b,
                            const CreEPS_FLOAT a1,
                            const CreEPS_FLOAT a2,
                            const CreEPS_FLOAT alpha )
{
	if( m_Mode != PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Adding ellipse arc in non-PATH mode is forbidden!\n") );
		
		exit( 1 );
	}

	fprintf( m_FileHandle, "%g %g %g %g %g %g %g ea\n",
	         a1, a2, a, b, alpha, x, y );
}

/******************************************************************************/

void CreEPS::addEllipseArcN( const CreEPS_FLOAT x,
                             const CreEPS_FLOAT y,
                             const CreEPS_FLOAT a,
                             const CreEPS_FLOAT b,
                             const CreEPS_FLOAT a1,
                             const CreEPS_FLOAT a2,
                             const CreEPS_FLOAT alpha )
{
	if( m_Mode != PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Adding ellipse arcN in non-PATH mode is forbidden!\n") );
		
		exit( 1 );
	}

	fprintf( m_FileHandle, "%g %g %g %g %g %g %g ean\n",
	         a1, a2, a, b, alpha, x, y );
}

/******************************************************************************/

void CreEPS::addEllipse( const CreEPS_FLOAT x,
                         const CreEPS_FLOAT y,
                         const CreEPS_FLOAT a,
                         const CreEPS_FLOAT b,
                         const CreEPS_FLOAT alpha )
{
	if( m_Mode != PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Adding ellipse in non-PATH mode is forbidden!\n") );
		
		exit( 1 );
	}

	fprintf( m_FileHandle, "%g %g %g %g %g e\n",
	         a, b, alpha, x, y );
}

/******************************************************************************/

void CreEPS::addArcT( const CreEPS_FLOAT x1,
                      const CreEPS_FLOAT y1,
                      const CreEPS_FLOAT x2,
                      const CreEPS_FLOAT y2,
                      const CreEPS_FLOAT r )
{
	if( m_Mode != PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Adding arcT in non-PATH mode is forbidden!\n") );
		
		exit( 1 );
	}

	fprintf( m_FileHandle, "%g %g %g %g %g at\n", x1, y1, x2, y2, r );
}

/******************************************************************************/

void CreEPS::addArcTLine( const CreEPS_FLOAT x1,
                          const CreEPS_FLOAT y1,
                          const CreEPS_FLOAT x2,
                          const CreEPS_FLOAT y2,
                          const CreEPS_FLOAT r )
{
	addArcT( x1, y1, x2, y2, r );
	addLine( x2, y2 );
}

/******************************************************************************/

void CreEPS::addCurve( const CreEPS_FLOAT x1,
                       const CreEPS_FLOAT y1,
                       const CreEPS_FLOAT x2,
                       const CreEPS_FLOAT y2,
                       const CreEPS_FLOAT x3,
                       const CreEPS_FLOAT y3 )
{
	if( m_Mode != PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Adding curve in non-PATH mode is forbidden!\n") );
		
		exit( 1 );
	}

	fprintf( m_FileHandle, "%g %g %g %g %g %g ct\n", x1, y1, x2, y2, x3, y3 );
}

/******************************************************************************/

void CreEPS::addRelativeCurve( const CreEPS_FLOAT x1,
                               const CreEPS_FLOAT y1,
                               const CreEPS_FLOAT x2,
                               const CreEPS_FLOAT y2,
                               const CreEPS_FLOAT x3,
                               const CreEPS_FLOAT y3 )
{
	m_errorMessageHandler(
		CreEPS_CC("CreEPS: addRelativeCurve is not yet completely implemented!")
		CreEPS_CC(" Do not use it!\n") );
	
	if( m_Mode != PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Adding relative curve in non-PATH mode is ")
			CreEPS_CC("forbidden!\n") );
		
		exit( 1 );
	}

	fprintf( m_FileHandle, "%g %g %g %g %g %g rct\n", x1, y1, x2, y2, x3, y3 );
}

/******************************************************************************/

void CreEPS::endPath()
{
	if( m_Mode != PATH ) {
		::printf( "CreEPS: Ending path in non-PATH mode is forbidden!\n" );
		exit( 1 );
	}

	fprintf( m_FileHandle, "n\n" );
	m_Mode = IDLE;
}

/******************************************************************************/

void CreEPS::endPath( const DrawMode drawMode, const CAt& attr  )
{
	static const char* DRAWMODE[] = {"f", "ef", "s", "cp n", "ec n"};

	if( m_Mode != PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Ending path in non-PATH mode is forbidden!\n") );
		
		exit( 1 );
	}

	bool changed = m_Attr.writeChanges( attr, m_FileHandle );
	fprintf( m_FileHandle, "%s\n%s", DRAWMODE[drawMode],
	         restoreAttributes( changed, true ));
	m_Mode = IDLE;
}

/******************************************************************************/

void CreEPS::endPath( const DrawMode drawMode1,
                      const DrawMode drawMode2,
                      const CAt& attr1,
                      const CAt& attr2 )
{
	usePath( drawMode1, attr1 );
	endPath( drawMode2, attr2 );
}

/******************************************************************************/

void CreEPS::usePath( const DrawMode drawMode,
                      const CAt& attr )
{
	static const char* DRAWMODE[] = {"f y", "ef y", "s y", "cp", "ec"};

	if( m_Mode != PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Using path in non-PATH mode is forbidden!\n") );
		
		exit( 1 );
	}

	if( drawMode != CLIP && drawMode != EOCLIP ) fprintf( m_FileHandle, "x\n" );

	CAt tmp( m_Attr );
	tmp.setAttributes( attr, m_FileHandle );
	fprintf( m_FileHandle, "%s\n", DRAWMODE[drawMode] );
}

/******************************************************************************/

void CreEPS::closeSubpath()
{
	if( m_Mode != PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Closing subpath in non-PATH mode is forbidden!\n") );
		
		exit( 1 );
	}

	fprintf( m_FileHandle, "c\n" );

}

/******************************************************************************/

void CreEPS::line( const CreEPS_FLOAT x1,
                   const CreEPS_FLOAT y1,
                   const CreEPS_FLOAT x2,
                   const CreEPS_FLOAT y2,
                   const CAt& attr )
{
	if( m_Mode == PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Drawing a line is not allowed in PATH mode)! ")
			CreEPS_CC("Use addLine() instead.\n") );
		
		exit( 1 );
	}

	bool changed = m_Attr.writeChanges( attr, m_FileHandle );
	fprintf( m_FileHandle, "%g %g m %g %g l s\n%s", x1, y1, x2, y2,
	         restoreAttributes( changed ) );
}

/******************************************************************************/

void CreEPS::rectStroke( const CreEPS_FLOAT x,
                         const CreEPS_FLOAT y,
                         const CreEPS_FLOAT w,
                         const CreEPS_FLOAT h,
                         const CAt& attr )
{
	if( m_Mode == PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Drawing a rectangular is not allowed in PATH mode!\n") );
		
		exit( 1 );
	}

	bool changed = m_Attr.writeChanges( attr, m_FileHandle );
	fprintf( m_FileHandle, "%g %g %g %g rs\n%s", x, y, w, h,
	         restoreAttributes( changed ) );
}

/******************************************************************************/

void CreEPS::rectFill( const CreEPS_FLOAT x,
                       const CreEPS_FLOAT y,
                       const CreEPS_FLOAT w,
                       const CreEPS_FLOAT h,
                       const CAt& attr )
{
	if( m_Mode == PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Drawing a filled rectangular is not allowed in PATH ")
			CreEPS_CC("mode!\n") );
		
		exit( 1 );
	}

	bool changed = m_Attr.writeChanges( attr, m_FileHandle );
	fprintf( m_FileHandle, "%g %g %g %g rf\n%s", x, y, w, h,
	         restoreAttributes( changed ) );
}

/******************************************************************************/

void CreEPS::curve( const CreEPS_FLOAT x0, const CreEPS_FLOAT y0,
                    const CreEPS_FLOAT x1, const CreEPS_FLOAT y1,
                    const CreEPS_FLOAT x2, const CreEPS_FLOAT y2,
                    const CreEPS_FLOAT x3, const CreEPS_FLOAT y3,
                    const CAt& attr )
{
	if( m_Mode == PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Drawing a curve is not allowed in  PATH mode)! ")
			CreEPS_CC("Use addCurve() instead.\n") );
		
		exit( 1 );
	}

	bool changed = m_Attr.writeChanges( attr, m_FileHandle );
	fprintf( m_FileHandle, "%g %g m %g %g %g %g %g %g ct s\n%s", x0, y0, x1, y1,
	         x2, y2, x3, y3, restoreAttributes( changed ) );
}

/******************************************************************************/

void CreEPS::arc( const CreEPS_FLOAT x,
                  const CreEPS_FLOAT y,
                  const CreEPS_FLOAT r,
                  const CreEPS_FLOAT a1,
                  const CreEPS_FLOAT a2,
                  const CAt& attr )
{
	if( m_Mode == PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Drawing an arc is not allowed in PATH mode)! ")
			CreEPS_CC("Use addArc() instead.\n") );
		
		exit( 1 );
	}

	bool changed = m_Attr.writeChanges( attr, m_FileHandle );
	fprintf( m_FileHandle, "%g %g %g %g %g a s\n%s", x, y, r, a1, a2,
	         restoreAttributes( changed ) );
}

/******************************************************************************/

void CreEPS::circle( const CreEPS_FLOAT x, const CreEPS_FLOAT y, const CreEPS_FLOAT r,
                     const CAt& attr )
{
	if( m_Mode == PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Drawing an circle is not allowed in PATH mode)! ")
			CreEPS_CC("Use addCircle() instead.\n") );
		
		exit( 1 );
	}

	bool changed = m_Attr.writeChanges( attr, m_FileHandle );
	fprintf( m_FileHandle, "%g %g %g o s\n%s", x, y, r,
	         restoreAttributes( changed ) );
}

/******************************************************************************/

void CreEPS::ellipseArc( const CreEPS_FLOAT x,
                         const CreEPS_FLOAT y,
                         const CreEPS_FLOAT a,
                         const CreEPS_FLOAT b,
                         const CreEPS_FLOAT a1,
                         const CreEPS_FLOAT a2,
                         const CreEPS_FLOAT alpha,
                         const CAt& attr )
{
	if( m_Mode == PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Drawing an ellipse arc is not allowed in PATH mode)! ")
			CreEPS_CC("Use addEllipseArc() instead.\n") );
		
		exit( 1 );
	}

	bool changed = m_Attr.writeChanges( attr, m_FileHandle );
	fprintf( m_FileHandle, "%g %g %g %g %g %g %g ea s\n%s",
	         a1, a2, a, b, alpha, x, y, restoreAttributes( changed ) );
}

/******************************************************************************/

void CreEPS::ellipseArc( const CreEPS_FLOAT x,
                         const CreEPS_FLOAT y,
                         const CreEPS_FLOAT a,
                         const CreEPS_FLOAT b,
                         const CreEPS_FLOAT a1,
                         const CreEPS_FLOAT a2,
                         const CAt& attr )
{
	ellipseArc( x, y, a, b, a1, a2, 0, attr );
}

/******************************************************************************/

void CreEPS::ellipse( const CreEPS_FLOAT x,
                      const CreEPS_FLOAT y,
                      const CreEPS_FLOAT a,
                      const CreEPS_FLOAT b,
                      const CreEPS_FLOAT alpha,
                      const CAt& attr )
{
	if( m_Mode == PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Drawing an ellipse is not allowed in PATH mode)! ")
			CreEPS_CC("Use addEllipse() instead.\n") );
		
		exit( 1 );
	}

	bool changed = m_Attr.writeChanges( attr, m_FileHandle );
	fprintf( m_FileHandle, "%g %g %g %g %g e s\n%s",
	         a, b, alpha, x, y, restoreAttributes( changed ) );
}

/******************************************************************************/

void CreEPS::ellipse( const CreEPS_FLOAT x,
                      const CreEPS_FLOAT y,
                      const CreEPS_FLOAT a,
                      const CreEPS_FLOAT b,
                      const CAt& attr )
{
	ellipse( x, y, a, b, 0, attr );
}

/******************************************************************************/

void CreEPS::disk( const CreEPS_FLOAT x,
                   const CreEPS_FLOAT y,
                   const CreEPS_FLOAT r,
                   const CAt& attr )
{
	startPath();
	addCircle( x, y, r );
	endPath( FILL, attr );
}

/******************************************************************************/

void CreEPS::print( const CreEPS_FLOAT x,
                    const CreEPS_FLOAT y,
                    const CreEPS_FLOAT alpha,
                    const char* text,
                    const CAt& attr )
{
	static const char* TEXTALIGNMENT[] =
	{
		"ln", "lb", "lc", "lt",
	    "cn", "cb", "cc", "ct",
	    "rn", "rb", "rc", "rt",
	    "ln", "lb", "lc", "lt"
	};
	
	static const char* LATEXTEXTALIGNMENT[] =
	{
		"[l]{\\smash", "[lb]{", "[l]{", "[lt]{",
	    "{\\smash",  "[b]{",    "{",  "[t]{",
	    "[r]{\\smash", "[rb]{", "[r]{", "[rt]{",
	    "[l]{\\smash", "[lb]{", "[l]{", "[lt]{"
	};

	if( m_Mode == PATH )
	{
		m_errorMessageHandler(
			CreEPS_CC("CreEPS: Printing is not allowed in PATH mode!\n") );
		
		exit( 1 );
	}

	if( m_LatexFileHandle ) {
		CAt tmp( m_Attr );
		tmp.inheritAttributes( attr, NULL );

		if (alpha > -0.01 && alpha < 0.01)
		{
			fprintf( m_LatexFileHandle,
			       "\\put(%f,%f){\\makebox(0,0)%s{\\SetFigFont{%f}{%f}{"
			       "\\familydefault}{\\mddefault}{\\updefault}{"
			       "\\color[rgb]{%f,%f,%f}%s}}}}%%\n",
			       x, y,
			       LATEXTEXTALIGNMENT[tmp.getTextAlignment()],
			       tmp.getFontScale(),
			       tmp.getFontScale()*1.2,
			       tmp.getRed(), tmp.getGreen(), tmp.getBlue(),
			       text );
		}
		else
		{
			fprintf( m_LatexFileHandle,
			       "\\put(%f,%f){\\rotatebox{%f}{\\makebox(0,0)%s{\\SetFigFont{%f}"
			       "{%f}{\\familydefault}{\\mddefault}{\\updefault}{\\color[rgb]"
			       "{%f,%f,%f}%s}}}}}%%\n",
			       x, y, alpha,
			       LATEXTEXTALIGNMENT[tmp.getTextAlignment()],
			       tmp.getFontScale(),
			       tmp.getFontScale()*1.2,
			       tmp.getRed(), tmp.getGreen(), tmp.getBlue(),
			       text );
		}
		
		return; //! comment out for testing both text outputs simultaneously
	}

	int o = 0, n = 0;
	int bufferLength = 0;
	
	while( text[o] != 0 ) {
		switch( text[o++] ) {
			case '(': case ')': case '\\':
				bufferLength++;
			default:
				bufferLength++;
		}
	}

	o = 0;
	char* buffer = new char[bufferLength+1];

	while( text[o] != 0 ) {
		switch( text[o++] ) {
			case '(': case ')': case '\\': buffer[n++] = '\\';
			default:
				buffer[n++] = text[o-1];
		}
	}
	buffer[n] = 0;
	
	CAt tmp( m_Attr );
	bool changed = tmp.inheritAttributes( attr, m_FileHandle );
	if (alpha > -0.01 && alpha < 0.01) {
		fprintf( m_FileHandle, "(%s) %g %g ta%s\n%s", buffer, x, y,
		         TEXTALIGNMENT[tmp.getTextAlignment()],
		         restoreAttributes( changed ) );
	} else {
		fprintf( m_FileHandle, "%g (%s) %g %g tar%s\n%s", alpha, buffer, x, y,
		         TEXTALIGNMENT[tmp.getTextAlignment()],
		         restoreAttributes( changed ) );
	}

	delete [] buffer;
}

/******************************************************************************/

void CreEPS::print( const CreEPS_FLOAT x,
                    const CreEPS_FLOAT y,
                    const char* text,
                    const CAt& attr )
{
	print( x, y, 0, text, attr );
}

/******************************************************************************/

void CreEPS::printf( const CAt& attr,
                     const CreEPS_FLOAT x,
                     const CreEPS_FLOAT y,
                     const char* text, ... )
{
	char buffer[m_BUFFERLENGTH];
	va_list arg;

	va_start( arg, text );
	vsprintf( buffer, text, arg );
	va_end( arg );

	print( x, y, buffer, attr );
}

/******************************************************************************/

void CreEPS::printf( const CAt          &attr,
                     const CreEPS_FLOAT  x,
                     const CreEPS_FLOAT  y,
                     const CreEPS_FLOAT  alpha,
                     const char  *text, ... )
{
	char buffer[m_BUFFERLENGTH];
	va_list arg;

	va_start( arg, text );
	vsprintf( buffer, text, arg );
	va_end( arg );

	print( x, y, alpha, buffer, attr );
}

/******************************************************************************/

void CreEPS::printf( const CreEPS_FLOAT x,
                     const CreEPS_FLOAT y,
                     const char* text, ... )
{
	char buffer[m_BUFFERLENGTH];
	va_list arg;

	va_start( arg, text );
	vsprintf( buffer, text, arg );
	va_end( arg );

	print( x, y, buffer );
}

/******************************************************************************/

void CreEPS::printf( const CreEPS_FLOAT  x,
                     const CreEPS_FLOAT  y,
                     const CreEPS_FLOAT  alpha,
                     const char  *text, ... )
{
	char buffer[m_BUFFERLENGTH];
	va_list  arg;

	va_start( arg, text );
	vsprintf( buffer, text, arg );
	va_end( arg );

	print( x, y, alpha, buffer );
}

/******************************************************************************/

void CreEPS::applyRotation( const CreEPS_FLOAT alpha )
{
	fprintf( m_FileHandle, "%g r\n", alpha );	
}

/******************************************************************************/

void CreEPS::applyTranslation( const CreEPS_FLOAT dx,
                               const CreEPS_FLOAT dy )
{
	fprintf( m_FileHandle, "%g %g t\n", dx, dy );
}

/******************************************************************************/

void CreEPS::applyScaling( const CreEPS_FLOAT s )
{
	applyScaling( s, s );	
}

/******************************************************************************/

void CreEPS::applyScaling( const CreEPS_FLOAT sx,
                           const CreEPS_FLOAT sy )
{
	fprintf( m_FileHandle, "%g %g sl\n", sx, sy );	
}

/******************************************************************************/

void CreEPS::applyTransformation( const CreEPS_FLOAT m[3][2] )
{
	fprintf( m_FileHandle, "[ %g %g %g %g %g %g ] cc\n",
	         m[0][0], m[0][1],
	         m[1][0], m[1][1],
	         m[2][0], m[2][1] ); 
}

/******************************************************************************/

void CreEPS::resetTransformations()
{
	fprintf( m_FileHandle, "im\n" );	
}

/******************************************************************************/

void CreEPS::resetClipping()
{
	fprintf( m_FileHandle, "ic\n" );	
}

/******************************************************************************/

void CreEPS::saveTransformation()
{
	fprintf( m_FileHandle, "ms\n" );
	m_nStoredTransformations++;
}

/******************************************************************************/

bool CreEPS::loadTransformation()
{
	if( m_nStoredTransformations <= 0 )
	{
		m_warningMessageHandler(
			CreEPS_CC("CreEPS: Tried to load transformation, but no transformatio)n ")
			CreEPS_CC("on stack!\n") );
		
		return false;
	}
	else {
		fprintf( m_FileHandle, "ml\n" );
		m_nStoredTransformations--;
		return true;
	}
}

/******************************************************************************/

void CreEPS::setAttributes( const CAt& attr )
{
	m_Attr.setAttributes( attr, m_FileHandle );
}

/******************************************************************************/

void CreEPS::special( const char* text, ... )
{
	va_list  arg;

	va_start( arg, text );
	vfprintf( m_FileHandle, text, arg );
	va_end( arg );
}

/******************************************************************************/

void CreEPS::newFile( const char  *filename,
                      const CreEPS_FLOAT  startX,
                      const CreEPS_FLOAT  startY,
                      const CreEPS_FLOAT  endX,
                      const CreEPS_FLOAT  endY,
                      const bool   latex,
                      const char  *altEPSFilename )
{
	finalize();
	m_Attr = CAt();
	initialize( filename, startX, startY, endX, endY, latex, altEPSFilename );
}

/******************************************************************************/

void CreEPS::newFile( const char  *filename,
                      const CreEPS_FLOAT  width,
                      const CreEPS_FLOAT  height,
                      const bool   latex,
                      const char  *altEPSFilename )
{
	finalize();
	m_Attr = CAt();
	initialize( filename, 0, 0, width, height, latex, altEPSFilename );
}

/******************************************************************************/

void CreEPS::StdMessageHandler( const CreEPS_CHAR* const msg, ... )
{
	va_list pargs;

	va_start( pargs, msg );
	::vprintf( (const char*)msg, pargs );
	va_end( pargs );

}

/******************************************************************************/

CreEPS_INT CreEPS::getMajorVersion()
{
	return CREEPS_MAJOR_VERSION;
}

/******************************************************************************/

CreEPS_INT CreEPS::getMinorVersion()
{
	return CREEPS_MINOR_VERSION;
}

/******************************************************************************/

} // end of namespace 'ns_CreEPS'
