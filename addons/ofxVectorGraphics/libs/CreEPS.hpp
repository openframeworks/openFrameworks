/*******************************************************************************

CreEPS = Creating EPS  Version 1.32

Uwe Fabricius : http://uwefabricius.de/
Thomas Pohl   : http://thomas-pohl.info/

Copyright (C) 2002, 2003, 2004  Uwe Fabricius & Thomas Pohl

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.


*******************************************************************************/

#ifndef  CREEPS_CREATES_EPS_INCLUDED
#define  CREEPS_CREATES_EPS_INCLUDED

#include <stdio.h>

#ifndef CREEPS_MAX_FONT_STRING_LENGTH
#define CREEPS_MAX_FONT_STRING_LENGTH 100
#define CREEPS_MAX_DASH_STRING_LENGTH 100
#endif

//// interface class for all attributes ////

class CAt
{
	public :

		// constants for the attributes
		enum LINEJOIN       { LJ_KEEP = -1, MITER = 0, LJ_ROUND, BEVEL };
		enum LINECAP        { LC_KEEP = -1, BUTT  = 0, LC_ROUND, SQUARE };
		enum TEXTALIGNMENT  { TA_KEEP = -1, LEFT     = 12, HCENTER = 4, RIGHT   = 8,
		                      BOTTOM  =  1, BASELINE =  0, VCENTER = 2, TOP     = 3,
		                      CENTER  = HCENTER | VCENTER, DEFAULT = LEFT | BASELINE };
		enum LINEDASH       { LD_KEEP = -1, SOLID, DOT, DASH, DOTDASH };

		CAt();
		CAt( const CAt& A );
		~CAt() {}

		float getLineThickness()         const { return m_LineThickness; }
		int   getLineJoin()              const { return m_LineJoin; }
		int   getLineCap()               const { return m_LineCap; }
		const char * getLineDashString() const { return m_LineDashString; }
		float getLineDashOffset()        const { return m_LineDashOffset; }
		int   getLineDashType()          const { return m_LineDashType; }
		float getLineDashFactor()        const { return m_LineDashFactor; }
		float getRed()                   const { return m_Red; }
		float getGreen()                 const { return m_Green; }
		float getBlue()                  const { return m_Blue; }
		float getBackgroundRed()         const { return m_BgRed; }
		float getBackgroundGreen()       const { return m_BgGreen; }
		float getBackgroundBlue()        const { return m_BgBlue; }
		float getFontScale()             const { return m_FontScale; }
		const char * getFontString()     const { return m_FontString; }
		int   getTextAlignment()         const { return m_TextAlignment; }
		int   getFillingPattern()        const { return m_FillingPattern; }

		bool inheritAttributes( const CAt& A, FILE* file = NULL,
		                        bool gsave_written = false );
		bool writeChanges( const CAt& A, FILE* file );
		bool setAttributes( const CAt& A, FILE* file );

		CAt operator | ( const CAt& attributes );


	protected :

		enum FILLINGPATTERN { FP_KEEP = -1, HEXDOT, CHECKERBOARD, STRIPE };

		void startFillingBox( FILE* file, const float width, const float height );
		void endFillingBox( FILE* file );

		bool inheritLineThickness  ( const CAt& A, FILE* file = NULL, bool gsave_written = false );
		bool inheritLineJoin       ( const CAt& A, FILE* file = NULL, bool gsave_written = false );
		bool inheritLineCap        ( const CAt& A, FILE* file = NULL, bool gsave_written = false );
		bool inheritLineDash       ( const CAt& A, FILE* file = NULL, bool gsave_written = false );
		bool inheritColor          ( const CAt& A, FILE* file = NULL, bool gsave_written = false );
		bool inheritBackgroundColor( const CAt& A, FILE* file = NULL );
		bool inheritFont           ( const CAt& A, FILE* file = NULL, bool gsave_written = false );
		bool inheritTextAlignment  ( const CAt& A, FILE* file = NULL );
		bool inheritFillingPattern ( const CAt& A, FILE* file = NULL, bool gsave_written = false );

		float          m_LineThickness;
		LINEJOIN       m_LineJoin;
		LINECAP        m_LineCap;
		char           m_LineDashString[CREEPS_MAX_DASH_STRING_LENGTH];
		float          m_LineDashOffset;
		LINEDASH       m_LineDashType;
		float          m_LineDashFactor;
		float          m_Red, m_Green, m_Blue;
		float          m_BgRed, m_BgGreen, m_BgBlue;
		float          m_FontScale;
		char           m_FontString[CREEPS_MAX_FONT_STRING_LENGTH];
		TEXTALIGNMENT  m_TextAlignment;
		FILLINGPATTERN m_FillingPattern;
		float          m_ff1, m_ff2, m_ff3;
};

//// specialized attributes ////

class CAtLineThickness : public CAt
{
	public :
		CAtLineThickness( const float thickness,
		                  const CAt& = CAt() );
		~CAtLineThickness() {}
};


class CAtLineJoin : public CAt
{
	public :
		CAtLineJoin( const LINEJOIN linejoin,
		             const CAt& = CAt() );
		~CAtLineJoin() {}
};


class CAtLineCap : public CAt
{
public :
		CAtLineCap( const LINECAP linecap,
		            const CAt& = CAt() );
		~CAtLineCap() {}
};


class CAtLineDash : public CAt
{
	public :
		CAtLineDash( const char *const dashstring,
		             const float       offset,
		             const CAt& = CAt() );
		CAtLineDash( const LINEDASH dash,
		             const float    factor,
		             const CAt& = CAt() );
		~CAtLineDash() {}
};


class CAtColor : public CAt
{
	public :
		CAtColor( const float red, const float green, const float blue,
		          const CAt& A = CAt() );
		~CAtColor() {}
};


class CAtBackgroundColor : public CAt
{
	public :
		CAtBackgroundColor( const float red, const float green, const float blue,
		                    const CAt& A = CAt() );
		~CAtBackgroundColor() {}
};


class CAtTransparentBackground : public CAt
{
	public :
		CAtTransparentBackground( const CAt& A = CAt() );
		~CAtTransparentBackground() {};
};


class CAtFont : public CAt
{
	public :
		CAtFont( const char *fontstring, const float scale,
		         const CAt& = CAt() );
		CAtFont( const char *fontstring,
		         const CAt& = CAt() );
		CAtFont( const float scale,
		         const CAt& = CAt() );
		~CAtFont() {}
};


class CAtGrayScale : public CAt
{
	public :
		CAtGrayScale( const float grayscale,
		              const CAt& = CAt() );
		~CAtGrayScale() {}
};


class CAtTextAlignment : public CAt
{
	public :
		CAtTextAlignment( const int alignement,
		                  const CAt& = CAt() );
		~CAtTextAlignment() {}
};


class CAtHexDotFilling : public CAt
{
	public :
		CAtHexDotFilling( const float radius, const float distance,
		                  const CAt& = CAt() );
		~CAtHexDotFilling() {}
};


class CAtCheckerboardFilling : public CAt
{
	public :
		CAtCheckerboardFilling( const float x, const float y,
		                        const CAt& = CAt() );
		~CAtCheckerboardFilling() {}
};


class CAtStripeFilling : public CAt
{
	public :
		CAtStripeFilling( const float width, const float distance, const int angle,
		                  const CAt& = CAt() );
		~CAtStripeFilling() {}
};

/*********************************************************
 * CreEPS
 *********************************************************/

class CreEPS
{
	protected :
		enum Mode {IDLE, PATH};

	public :
		enum DrawMode {FILL = 0, EOFILL, STROKE, CLIP, EOCLIP};

		// constructors & destructor
		CreEPS( const char* filename, const float startX, const float startY,
		                              const float   endX, const float   endY,
		        const bool latex = false, const char* altEPSFilename = NULL );
		CreEPS( const char* filename, const float width, const float height,
		        const bool latex = false, const char* altEPSFilename = NULL );
		CreEPS(): m_initialized ( false ) {};
		~CreEPS() { finalize(); }

		// path drawing methods
		void startPath( const float x, const float y );
		void startPath( );
		void endPath( );
		void endPath( const DrawMode drawMode, const CAt& attr = CAt() );
		void endPath( const DrawMode drawMode1, const DrawMode drawMode2, const CAt& attr1 = CAt(),
		              const CAt& attr2 = CAt() );
		void usePath( const DrawMode drawMode, const CAt& attr = CAt() );
		void closeSubpath();
		void addMove( const float x, const float y );
		void addRelativeMove( const float dx, const float dy );
		void addLine( const float x, const float y );
		void addRelativeLine( const float dx, const float dy );
		void addArc( const float x, const float y, const float r,
		             const float a1, const float a2 );
		void addArcN( const float x, const float y, const float r,
		              const float a1, const float a2 );
		void addCircle( const float x, const float y, const float r );
		void addEllipseArc( const float x, const float y,
		                    const float a, const float b,
		                    const float a1, const float a2,
		                    const float alpha = 0 );
		void addEllipseArcN( const float x, const float y,
		                     const float a, const float b,
		                     const float a1, const float a2,
		                     const float alpha = 0 );
		void addEllipse( const float x, const float y,
		                 const float a, const float b, const float alpha = 0 );
		void addArcT( const float x1, const float y1,
		              const float x2, const float y2, const float r );
		void addArcTLine( const float x1, const float y1,
		                  const float x2, const float y2, const float r );
		void addCurve( const float x1, const float y1,
		               const float x2, const float y2,
		               const float x3, const float y3 );
		void addRelativeCurve( const float x1, const float y1,
		                       const float x2, const float y2,
		                       const float x3, const float y3 );

		// non-path drawing methods
		void line( const float x1, const float y1, const float x2, const float y2,
		           const CAt& attr = CAt() );
		void rectStroke( const float x, const float y,
		                 const float w, const float h, const CAt& attr = CAt() );
		void rectFill( const float x, const float y,
		               const float w, const float h, const CAt& attr = CAt() );
		void curve( const float x0, const float y0,
		            const float x1, const float y1,
		            const float x2, const float y2,
		            const float x3, const float y3, const CAt& attr = CAt() );
		void arc( const float x, const float y, const float r,
		          const float a1, const float a2, const CAt& attr = CAt() );
		void circle( const float x, const float y, const float r,
		             const CAt& attr = CAt() );
		void ellipseArc( const float x, const float y,
		                 const float a, const float b,
		                 const float a1, const float a2,
		                 const CAt& attr = CAt() );
		void ellipseArc( const float x, const float y,
		                 const float a, const float b,
		                 const float a1, const float a2,
		                 const float alpha,
		                 const CAt& attr = CAt() );
		void ellipse( const float x, const float y,
		              const float a, const float b,
		              const CAt& attr = CAt() );
		void ellipse( const float x, const float y,
		              const float a, const float b, const float alpha,
		              const CAt& attr = CAt() );
		void disk( const float x, const float y, const float r,
		           const CAt& attr = CAt() );
		void print( const float x, const float y, const char* text, const CAt& attr = CAt() );
		void print( const float x, const float y, const float alpha, const char* text, const CAt& attr = CAt() );
		void printf( const CAt& attr, const float x, const float y, const char* text, ...);
		void printf( const CAt& attr, const float x, const float y, const float alpha, const char* text, ...);
		void printf( const float x, const float y, const char* text, ...);
		void printf( const float x, const float y, const float alpha, const char* text, ...);

		// transformations
		void applyRotation( const float alpha );
		void applyTranslation( const float dx, const float dy );
		void applyScaling( const float s );
		void applyScaling( const float sx, const float sy );
		void applyTransformation( const float m[3][2] );
		void saveTransformation();
		bool loadTransformation();
		void resetTransformations();

		// misc
		void special( const char* text, ... );
		void setAttributes( const CAt& attr );
		void resetClipping();
		void newFile( const char* filename, const float startX, const float startY,
		                                    const float   endX, const float   endY,
		              const bool latex = false, const char* altEPSFilename = NULL );
		void newFile( const char* filename, const float width, const float height,
		              const bool latex = false, const char* altEPSFilename = NULL );
		void finalize();
		bool embedEPS( const char* const filename );


	protected :
		void initialize( const char* filename, const float startX, const float startY,
		                                       const float   endX, const float   endY,
		                 const bool latex, const char* altEPSFilename );
		const char* restoreAttributes( const bool change, const bool newpath = false );

		CAt m_Attr;
		Mode m_Mode;
		int m_nStoredTransformations;
		bool m_initialized;
		FILE* m_FileHandle, * m_LatexFileHandle;
		static const float m_INCH2MM;
		static const int m_BUFFERLENGTH;
};

/*********************************************************/

#endif // CREEPS_CREATES_EPS_INCLUDED
