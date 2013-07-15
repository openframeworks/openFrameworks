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

#ifndef  CREEPS_CREATES_EPS_INCLUDED
#define  CREEPS_CREATES_EPS_INCLUDED

#include <stdio.h>
#include <stdarg.h>
#include "CreEPS_Types.hpp"

namespace ns_creeps
{

#ifndef  CREEPS_MAX_FONT_STRING_LENGTH
#  define  CREEPS_MAX_FONT_STRING_LENGTH  100
#  define  CREEPS_MAX_DASH_STRING_LENGTH  100
#endif

//! \brief interface class for all attributes
class CAt
{
	public:

		// constants for the attributes
		enum LINEJOIN
		{
			LJ_KEEP = -1,
			MITER   =  0,
			LJ_ROUND,
			BEVEL
		};

		enum LINECAP
		{
			LC_KEEP = -1,
			BUTT    =  0,
			LC_ROUND,
			SQUARE
		};

		enum TEXTALIGNMENT
		{
			TA_KEEP  = -1,
			LEFT     = 12,
			HCENTER  =  4,
			RIGHT    =  8,
		    BOTTOM   =  1,
		    BASELINE =  0,
		    VCENTER  =  2,
		    TOP      =  3,
		    CENTER   = HCENTER | VCENTER,
		    DEFAULT  = LEFT | BASELINE
		};
		
		enum LINEDASH
		{
			LD_KEEP = -1,
			SOLID,
			DOT,
			DASH,
			DOTDASH
		};

		CAt();
		CAt( const CAt& A );
		~CAt() {}

		CreEPS_INT getLineJoin() const {
			return static_cast<CreEPS_INT>( m_LineJoin ); }
		
		CreEPS_INT getLineCap() const {
			return static_cast<CreEPS_INT>( m_LineCap ); }
		
		CreEPS_INT getLineDashType() const {
			return static_cast<CreEPS_INT>( m_LineDashType ); }
		
		CreEPS_INT getTextAlignment() const {
			return static_cast<CreEPS_INT>( m_TextAlignment ); }
		
		CreEPS_INT getFillingPattern() const {
			return static_cast<CreEPS_INT>( m_FillingPattern ); }
		
		CreEPS_FLOAT getLineThickness() const {
			return static_cast<CreEPS_FLOAT>( m_LineThickness ); }
		
		CreEPS_FLOAT getLineDashOffset() const {
			return static_cast<CreEPS_FLOAT>( m_LineDashOffset ); }
		
		CreEPS_FLOAT getLineDashFactor() const {
			return static_cast<CreEPS_FLOAT>( m_LineDashFactor ); }
		
		CreEPS_FLOAT getRed() const {
			return static_cast<CreEPS_FLOAT>( m_Red ); }
		
		CreEPS_FLOAT getGreen() const {
			return static_cast<CreEPS_FLOAT>( m_Green ); }
		
		CreEPS_FLOAT getBlue() const {
			return static_cast<CreEPS_FLOAT>( m_Blue ); }
		
		CreEPS_FLOAT getBackgroundRed() const {
			return static_cast<CreEPS_FLOAT>( m_BgRed ); }
		
		CreEPS_FLOAT getBackgroundGreen() const {
			return static_cast<CreEPS_FLOAT>( m_BgGreen ); }
			
		CreEPS_FLOAT getBackgroundBlue() const {
			return static_cast<CreEPS_FLOAT>( m_BgBlue ); }
			
		CreEPS_FLOAT getFontScale() const {
			return static_cast<CreEPS_FLOAT>( m_FontScale ); }

		const CreEPS_CHAR* getLineDashString() const {
			return static_cast<const CreEPS_CHAR*>( m_LineDashString ); }
		
		const CreEPS_CHAR* getFontString() const {
			return static_cast<const CreEPS_CHAR*>( m_FontString ); }

		CreEPS_BOOL inheritAttributes( const CAt& A, FILE* file = NULL,
		                               CreEPS_BOOL gsave_written = CreEPS_FALSE );
		
		CreEPS_BOOL writeChanges( const CAt& A, FILE* file );
		
		CreEPS_BOOL setAttributes( const CAt& A, FILE* file );

		CAt operator | ( const CAt& attributes );


	protected:

		enum FILLINGPATTERN
		{
			FP_KEEP = -1,
			HEXDOT,
			CHECKERBOARD,
			STRIPE
		};

		void startFillingBox( FILE* file,
		                      const CreEPS_FLOAT width,
		                      const CreEPS_FLOAT height );

		void endFillingBox( FILE* file );

		CreEPS_BOOL inheritLineThickness  ( const CAt& A, FILE* file = NULL,
		                                    CreEPS_BOOL gsave_written = false );
											
		CreEPS_BOOL inheritLineJoin       ( const CAt& A, FILE* file = NULL,
		                                    CreEPS_BOOL gsave_written = false );
											
		CreEPS_BOOL inheritLineCap        ( const CAt& A, FILE* file = NULL,
		                                    CreEPS_BOOL gsave_written = false );
											
		CreEPS_BOOL inheritLineDash       ( const CAt& A, FILE* file = NULL,
		                                    CreEPS_BOOL gsave_written = false );
											
		CreEPS_BOOL inheritColor          ( const CAt& A, FILE* file = NULL,
		                                    CreEPS_BOOL gsave_written = false );
											
		CreEPS_BOOL inheritBackgroundColor( const CAt& A, FILE* file = NULL );
		
		CreEPS_BOOL inheritFont           ( const CAt& A, FILE* file = NULL,
		                                    CreEPS_BOOL gsave_written = false );
											
		CreEPS_BOOL inheritTextAlignment  ( const CAt& A, FILE* file = NULL );
		
		CreEPS_BOOL inheritFillingPattern ( const CAt& A, FILE* file = NULL,
		                                    CreEPS_BOOL gsave_written = false );

		char   m_LineDashString[CREEPS_MAX_DASH_STRING_LENGTH],
		       m_FontString[CREEPS_MAX_FONT_STRING_LENGTH];
		float  m_LineThickness,
		       m_LineDashOffset,
		       m_LineDashFactor,
		       m_Red, m_Green, m_Blue,
		       m_BgRed, m_BgGreen, m_BgBlue,
		       m_FontScale,
		       m_ff1, m_ff2, m_ff3;

		LINEJOIN       m_LineJoin;
		LINECAP        m_LineCap;
		LINEDASH       m_LineDashType;
		TEXTALIGNMENT  m_TextAlignment;
		FILLINGPATTERN m_FillingPattern;
};

//// specialized attributes ////

class CAtLineThickness : public CAt
{
	public:
	
		CAtLineThickness( const CreEPS_FLOAT thickness,
		                  const CAt& = CAt() );
		
		~CAtLineThickness() {}
};


class CAtLineJoin : public CAt
{
	public:
	
		CAtLineJoin( const LINEJOIN linejoin,
		             const CAt& = CAt() );
		
		~CAtLineJoin() {}
};


class CAtLineCap : public CAt
{
	public:
	
		CAtLineCap( const LINECAP linecap,
		            const CAt& = CAt() );
		
		~CAtLineCap() {}
};


class CAtLineDash : public CAt
{
	public:
	
		CAtLineDash( const CreEPS_CHAR *const dashstring,
		             const CreEPS_FLOAT       offset,
		             const CAt& = CAt() );
		
		CAtLineDash( const LINEDASH dash,
		             const CreEPS_FLOAT    factor,
		             const CAt& = CAt() );
		
		~CAtLineDash() {}
};


class CAtColor : public CAt
{
	public:
	
		CAtColor( const CreEPS_FLOAT red,
		          const CreEPS_FLOAT green,
		          const CreEPS_FLOAT blue,
		          const CAt& A = CAt() );
		
		~CAtColor() {}
};


class CAtBackgroundColor : public CAt
{
	public:
	
		CAtBackgroundColor( const CreEPS_FLOAT red,
		                    const CreEPS_FLOAT green,
		                    const CreEPS_FLOAT blue,
		                    const CAt& A = CAt() );
		
		~CAtBackgroundColor() {}
};


class CAtTransparentBackground : public CAt
{
	public:
	
		CAtTransparentBackground( const CAt& A = CAt() );
		
		~CAtTransparentBackground() {};
};


class CAtFont : public CAt
{
	public:

		CAtFont( const CreEPS_CHAR *fontstring,
		         const CreEPS_FLOAT scale,
		         const CAt& = CAt() );
		
		CAtFont( const CreEPS_CHAR *fontstring,
		         const CAt& = CAt() );
		
		CAtFont( const CreEPS_FLOAT scale,
		         const CAt& = CAt() );
		
		~CAtFont() {}
};


class CAtGrayScale : public CAt
{
	public:
	
		CAtGrayScale( const CreEPS_FLOAT grayscale,
		              const CAt& = CAt() );
		
		~CAtGrayScale() {}
};


class CAtTextAlignment : public CAt
{
	public:
	
		CAtTextAlignment( const CreEPS_INT alignement,
		                  const CAt& = CAt() );
		
		~CAtTextAlignment() {}
};


class CAtHexDotFilling : public CAt
{
	public:
	
		CAtHexDotFilling( const CreEPS_FLOAT radius,
		                  const CreEPS_FLOAT distance,
		                  const CAt& = CAt() );
		
		~CAtHexDotFilling() {}
};


class CAtCheckerboardFilling : public CAt
{
	public:
	
		CAtCheckerboardFilling( const CreEPS_FLOAT x,
		                        const CreEPS_FLOAT y,
		                        const CAt& = CAt() );
		
		~CAtCheckerboardFilling() {}
};


class CAtStripeFilling : public CAt
{
	public:
	
		CAtStripeFilling( const CreEPS_FLOAT width,
		                  const CreEPS_FLOAT distance,
		                  const CreEPS_INT   angle,
		                  const CAt& = CAt() );
		
		~CAtStripeFilling() {}
};

/*********************************************************
 * CreEPS
 *********************************************************/

class CreEPS
{
	public:

		typedef void (*MessageHandler)( const CreEPS_CHAR* const msg, ... );
	
	protected:
	
		enum Mode
		{
			IDLE,
			PATH
		};

	public:
	
		enum DrawMode
		{
			FILL = 0,
			EOFILL,
			STROKE,
			CLIP,
			EOCLIP
		};

		// constructors & destructor
		CreEPS();
		
		CreEPS( const CreEPS_CHAR  *filename,
		        const CreEPS_FLOAT  startX,
		        const CreEPS_FLOAT  startY,
		        const CreEPS_FLOAT  endX,
		        const CreEPS_FLOAT  endY,
		        const CreEPS_BOOL   latex = CreEPS_FALSE,
		        const CreEPS_CHAR  *altEPSFilename = NULL );
		
		CreEPS( const CreEPS_CHAR  *filename,
		        const CreEPS_FLOAT  width,
		        const CreEPS_FLOAT  height,
		        const CreEPS_BOOL   latex = CreEPS_FALSE,
		        const CreEPS_CHAR  *altEPSFilename = NULL );
		
		~CreEPS() { finalize(); }

		// message handler
		static MessageHandler getWarningMessageHandler() {
			return m_warningMessageHandler; }
		static MessageHandler getErrorMessageHandler() {
			return m_errorMessageHandler; }
		static MessageHandler setWarningMessageHandler( MessageHandler msgHandler );
		static MessageHandler setErrorMessageHandler( MessageHandler msgHandler );

		// path drawing methods
		void startPath( const CreEPS_FLOAT x,
		                const CreEPS_FLOAT y );
		
		void startPath( );

		void endPath( );

		void endPath( const DrawMode drawMode,
		              const CAt& attr = CAt() );
		
		void endPath( const DrawMode drawMode1,
		              const DrawMode drawMode2,
		              const CAt& attr1 = CAt(),
		              const CAt& attr2 = CAt() );
		
		void usePath( const DrawMode drawMode,
		              const CAt& attr = CAt() );
		
		void closeSubpath();
		
		void addMove( const CreEPS_FLOAT x, const CreEPS_FLOAT y );
		
		void addRelativeMove( const CreEPS_FLOAT dx, const CreEPS_FLOAT dy );
		
		void addLine( const CreEPS_FLOAT x, const CreEPS_FLOAT y );
		
		void addRelativeLine( const CreEPS_FLOAT dx, const CreEPS_FLOAT dy );
		
		void addArc( const CreEPS_FLOAT x,  const CreEPS_FLOAT y,
		             const CreEPS_FLOAT r,
		             const CreEPS_FLOAT a1, const CreEPS_FLOAT a2 );
		
		void addArcN( const CreEPS_FLOAT x,  const CreEPS_FLOAT y,
		              const CreEPS_FLOAT r,
		              const CreEPS_FLOAT a1, const CreEPS_FLOAT a2 );
		
		void addCircle( const CreEPS_FLOAT x, const CreEPS_FLOAT y,
		                const CreEPS_FLOAT r );
		
		void addEllipseArc( const CreEPS_FLOAT x, const CreEPS_FLOAT y,
		                    const CreEPS_FLOAT a, const CreEPS_FLOAT b,
		                    const CreEPS_FLOAT a1, const CreEPS_FLOAT a2,
		                    const CreEPS_FLOAT alpha = 0 );
		
		void addEllipseArcN( const CreEPS_FLOAT x, const CreEPS_FLOAT y,
		                     const CreEPS_FLOAT a, const CreEPS_FLOAT b,
		                     const CreEPS_FLOAT a1, const CreEPS_FLOAT a2,
		                     const CreEPS_FLOAT alpha = 0 );
		
		void addEllipse( const CreEPS_FLOAT x, const CreEPS_FLOAT y,
		                 const CreEPS_FLOAT a, const CreEPS_FLOAT b,
		                 const CreEPS_FLOAT alpha = 0 );
		
		void addArcT( const CreEPS_FLOAT x1, const CreEPS_FLOAT y1,
		              const CreEPS_FLOAT x2, const CreEPS_FLOAT y2,
		              const CreEPS_FLOAT r );
		
		void addArcTLine( const CreEPS_FLOAT x1, const CreEPS_FLOAT y1,
		                  const CreEPS_FLOAT x2, const CreEPS_FLOAT y2,
		                  const CreEPS_FLOAT r );
		
		void addCurve( const CreEPS_FLOAT x1, const CreEPS_FLOAT y1,
		               const CreEPS_FLOAT x2, const CreEPS_FLOAT y2,
		               const CreEPS_FLOAT x3, const CreEPS_FLOAT y3 );
		
		void addRelativeCurve( const CreEPS_FLOAT x1, const CreEPS_FLOAT y1,
		                       const CreEPS_FLOAT x2, const CreEPS_FLOAT y2,
		                       const CreEPS_FLOAT x3, const CreEPS_FLOAT y3 );

		// non-path drawing methods
		void line( const CreEPS_FLOAT x1, const CreEPS_FLOAT y1,
		           const CreEPS_FLOAT x2, const CreEPS_FLOAT y2,
		           const CAt& attr = CAt() );
		
		void rectStroke( const CreEPS_FLOAT x, const CreEPS_FLOAT y,
		                 const CreEPS_FLOAT w, const CreEPS_FLOAT h,
		                 const CAt& attr = CAt() );
		
		void rectFill( const CreEPS_FLOAT x, const CreEPS_FLOAT y,
		               const CreEPS_FLOAT w, const CreEPS_FLOAT h,
		               const CAt& attr = CAt() );
		
		void curve( const CreEPS_FLOAT x0, const CreEPS_FLOAT y0,
		            const CreEPS_FLOAT x1, const CreEPS_FLOAT y1,
		            const CreEPS_FLOAT x2, const CreEPS_FLOAT y2,
		            const CreEPS_FLOAT x3, const CreEPS_FLOAT y3,
		            const CAt& attr = CAt() );
		
		void arc( const CreEPS_FLOAT x,  const CreEPS_FLOAT y,
		          const CreEPS_FLOAT r,
		          const CreEPS_FLOAT a1, const CreEPS_FLOAT a2,
		          const CAt& attr = CAt() );
		
		void circle( const CreEPS_FLOAT x, const CreEPS_FLOAT y,
		             const CreEPS_FLOAT r,
		             const CAt& attr = CAt() );
		
		void ellipseArc( const CreEPS_FLOAT x,  const CreEPS_FLOAT y,
		                 const CreEPS_FLOAT a,  const CreEPS_FLOAT b,
		                 const CreEPS_FLOAT a1, const CreEPS_FLOAT a2,
		                 const CAt& attr = CAt() );
		
		void ellipseArc( const CreEPS_FLOAT x,  const CreEPS_FLOAT y,
		                 const CreEPS_FLOAT a,  const CreEPS_FLOAT b,
		                 const CreEPS_FLOAT a1, const CreEPS_FLOAT a2,
		                 const CreEPS_FLOAT alpha,
		                 const CAt& attr = CAt() );
		
		void ellipse( const CreEPS_FLOAT x, const CreEPS_FLOAT y,
		              const CreEPS_FLOAT a, const CreEPS_FLOAT b,
		              const CAt& attr = CAt() );
		
		void ellipse( const CreEPS_FLOAT x, const CreEPS_FLOAT y,
		              const CreEPS_FLOAT a, const CreEPS_FLOAT b,
		              const CreEPS_FLOAT alpha,
		              const CAt& attr = CAt() );
		
		void disk( const CreEPS_FLOAT x, const CreEPS_FLOAT y,
		           const CreEPS_FLOAT r,
		           const CAt& attr = CAt() );
		
		void print( const CreEPS_FLOAT x, const CreEPS_FLOAT y,
		            const CreEPS_CHAR* text,
		            const CAt& attr = CAt() );
		
		void print( const CreEPS_FLOAT x, const CreEPS_FLOAT y,
		            const CreEPS_FLOAT alpha,
		            const CreEPS_CHAR* text,
		            const CAt& attr = CAt() );
		
		void printf( const CAt& attr,
		             const CreEPS_FLOAT x, const CreEPS_FLOAT y,
		             const CreEPS_CHAR* text, ... );
		
		void printf( const CAt& attr,
		             const CreEPS_FLOAT x, const CreEPS_FLOAT y,
		             const CreEPS_FLOAT alpha,
		             const CreEPS_CHAR* text, ... );
		
		void printf( const CreEPS_FLOAT x, const CreEPS_FLOAT y,
		             const CreEPS_CHAR* text, ... );
		
		void printf( const CreEPS_FLOAT x, const CreEPS_FLOAT y,
		             const CreEPS_FLOAT alpha,
		             const CreEPS_CHAR* text, ...);

		// transformations
		void applyRotation( const CreEPS_FLOAT alpha );
		void applyTranslation( const CreEPS_FLOAT dx, const CreEPS_FLOAT dy );
		void applyScaling( const CreEPS_FLOAT s );
		void applyScaling( const CreEPS_FLOAT sx, const CreEPS_FLOAT sy );
		void applyTransformation( const CreEPS_FLOAT m[3][2] );
		void saveTransformation();
		CreEPS_BOOL loadTransformation();
		void resetTransformations();

		// misc
		void special( const CreEPS_CHAR* text, ... );
		void setAttributes( const CAt& attr );
		void resetClipping();
		
		void newFile( const CreEPS_CHAR* filename,
		              const CreEPS_FLOAT startX, const CreEPS_FLOAT startY,
		              const CreEPS_FLOAT   endX, const CreEPS_FLOAT   endY,
		              const CreEPS_BOOL latex = CreEPS_FALSE,
		              const CreEPS_CHAR* altEPSFilename = NULL );
		
		void newFile( const CreEPS_CHAR* filename,
		              const CreEPS_FLOAT width,
		              const CreEPS_FLOAT height,
		              const CreEPS_BOOL latex = CreEPS_FALSE,
		              const CreEPS_CHAR* altEPSFilename = NULL );
		
		void finalize();
		CreEPS_BOOL embedEPS( const CreEPS_CHAR* const filename );
		
		static CreEPS_INT getMajorVersion();
		static CreEPS_INT getMinorVersion();


	protected:
	
		void initialize( const CreEPS_CHAR  *filename,
		                 const CreEPS_FLOAT  startX,
		                 const CreEPS_FLOAT  startY,
		                 const CreEPS_FLOAT  endX,
		                 const CreEPS_FLOAT  endY,
		                 const CreEPS_BOOL   latex,
		                 const CreEPS_CHAR  *altEPSFilename );
		
		const char* restoreAttributes( const bool change,
		                               const bool newpath = false );

		static void StdMessageHandler( const char* const msg, ... );

		static MessageHandler m_warningMessageHandler;
		static MessageHandler m_errorMessageHandler;

		bool   m_initialized;
		int    m_nStoredTransformations;
		Mode   m_Mode;
		CAt    m_Attr;
		FILE  *m_FileHandle,
		      *m_LatexFileHandle;
		
		static const float m_INCH2MM;
		static const int   m_BUFFERLENGTH;
};

/*********************************************************/

} // end of namespace 'ns_CreEPS'

#endif // CREEPS_CREATES_EPS_INCLUDED
