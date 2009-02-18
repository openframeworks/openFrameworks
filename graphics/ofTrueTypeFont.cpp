#include "ofTrueTypeFont.h"
//--------------------------


#if defined (TARGET_WIN32) || defined (TARGET_LINUX)
	#include <ft2build.h>
	#include <freetype/freetype.h>
	#include <freetype/ftglyph.h>
	#include <freetype/ftoutln.h>
	#include <freetype/fttrigon.h>
#endif

#if defined (TARGET_OSX) || defined (TARGET_OF_IPHONE) 
	#include "ft2build.h"
	#include "freetype.h"
	#include "ftglyph.h"
	#include "ftoutln.h"
	#include "fttrigon.h"
#endif

static bool printVectorInfo = false;

//#include "polylineSimplify.h"
//
//
//static void simplify(vector <ofPoint> &contourIn, vector <ofPoint> &contourOut, float tolerance){
//	int length = contourIn.size();
//
//	//the polyLine simplify class needs data as a vector of ofxPoint3fs
//	ofPoint  polyLineIn[length];
//	ofPoint  polyLineOut[length];
//
//	//first we copy over the data to a 3d point array
//	for(int i = 0; i < length; i++){
//		polyLineIn[i].x = contourIn[i].x;
//		polyLineIn[i].y = contourIn[i].y;
//	}
//
//	int numPoints = poly_simplify(tolerance, polyLineIn, length, polyLineOut);
//	contourOut.clear();
//	contourOut.assign(numPoints, ofPoint());
//
//	//copy the data to our contourOut vector
//	for(int i = 0; i < numPoints; i++){
//		contourOut[i].x = polyLineOut[i].x;
//		contourOut[i].y = polyLineOut[i].y;
//	}
//
//}

//------------------------------------------------------------
static void quad_bezier(vector <ofPoint> &ptsList, float x1, float y1, float x2, float y2, float x3, float y3, int res){
	for(int i=0; i <= res; i++){
        double t = (double)i / (double)(res);
        double a = pow((1.0 - t), 2.0);
        double b = 2.0 * t * (1.0 - t);
        double c = pow(t, 2.0);
        double x = a * x1 + b * x2 + c * x3;
        double y = a * y1 + b * y2 + c * y3;
        ptsList.push_back(ofPoint(x, y));
    }
}

//-----------------------------------------------------------
static void cubic_bezier(vector <ofPoint> &ptsList, float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, int res){
	float   ax, bx, cx;
    float   ay, by, cy;
    float   t, t2, t3;
    float   x, y;

    // polynomial coefficients
    cx = 3.0f * (x1 - x0);
    bx = 3.0f * (x2 - x1) - cx;
    ax = x3 - x0 - cx - bx;

    cy = 3.0f * (y1 - y0);
    by = 3.0f * (y2 - y1) - cy;
    ay = y3 - y0 - cy - by;


    int resolution = res;

    for (int i = 0; i < resolution; i++){
    	t 	=  (float)i / (float)(resolution-1);
    	t2 = t * t;
    	t3 = t2 * t;
		x = (ax * t3) + (bx * t2) + (cx * t) + x0;
    	y = (ay * t3) + (by * t2) + (cy * t) + y0;
    	ptsList.push_back(ofPoint(x,y) );
    }
}

//--------------------------------------------------------
static ofTTFCharacter makeContoursForCharacter(FT_Face &face);
static ofTTFCharacter makeContoursForCharacter(FT_Face &face){
		
		int num			= face->glyph->outline.n_points;
		int nContours	= face->glyph->outline.n_contours;
		int startPos	= 0;

		char * tags		= face->glyph->outline.tags;
		FT_Vector * vec = face->glyph->outline.points;
		
		ofTTFCharacter charOutlines;

		for(int k = 0; k < nContours; k++){
			if( k > 0 ){
				startPos = face->glyph->outline.contours[k-1]+1;
			}
			int endPos = face->glyph->outline.contours[k]+1;
			
			if( printVectorInfo )printf("--NEW CONTOUR\n\n");
			
			vector <ofPoint> testOutline;
			ofPoint lastPoint;
			
			for(int j = startPos; j < endPos; j++){
			
				if( FT_CURVE_TAG(tags[j]) == FT_CURVE_TAG_ON ){
					lastPoint.set(  vec[j].x,  -vec[j].y, 0 );
					if( printVectorInfo )printf("flag[%i] is set to 1 - regular point - %f %f \n", j, lastPoint.x, lastPoint.y);
					testOutline.push_back(lastPoint);
					
				}else{
					if( printVectorInfo )printf("flag[%i] is set to 0 - control point \n", j);
					
					if( FT_CURVE_TAG(tags[j]) == FT_CURVE_TAG_CUBIC ){
						if( printVectorInfo )printf("- bit 2 is set to 2 - CUBIC\n");
						
						int prevPoint = j-1;
						if( j == 0){
							prevPoint = endPos-1;
						}
						
						int nextIndex = j+1; 
						if( nextIndex >= endPos){
							nextIndex = startPos;
						}
						
						ofPoint nextPoint( (float)vec[nextIndex].x,  -(float)vec[nextIndex].y );				
						
						//we need two control points to draw a cubic bezier
						bool lastPointCubic =  ( FT_CURVE_TAG(tags[prevPoint]) != FT_CURVE_TAG_ON ) && ( FT_CURVE_TAG(tags[prevPoint]) == FT_CURVE_TAG_CUBIC);
					
						if( lastPointCubic ){
							ofPoint controlPoint1( vec[prevPoint].x,  -vec[prevPoint].y );
							ofPoint controlPoint2( vec[j].x,  -vec[j].y );
							ofPoint nextPoint( (float)vec[nextIndex].x,  -(float)vec[nextIndex].y );				
					
							cubic_bezier(testOutline, lastPoint.x, lastPoint.y, controlPoint1.x, controlPoint1.y, controlPoint2.x, controlPoint2.y, nextPoint.x, nextPoint.y, 8);
						}
						
					}else{
						
						ofPoint conicPoint( (float)vec[j].x,  -(float)vec[j].y );
						
						if( printVectorInfo )printf("- bit 2 is set to 0 - conic- \n");
						if( printVectorInfo )printf("--- conicPoint point is %f %f \n", conicPoint.x, conicPoint.y);

						//If the first point is connic and the last point is connic then we need to create a virutal point which acts as a wrap around
						if( j == startPos ){
							bool prevIsConnic = (  FT_CURVE_TAG( tags[endPos-1] ) != FT_CURVE_TAG_ON ) && ( FT_CURVE_TAG( tags[endPos-1]) != FT_CURVE_TAG_CUBIC );
							
							if( prevIsConnic ){
								ofPoint lastConnic( vec[endPos-1].x,  -vec[endPos-1].y );
								lastPoint = (conicPoint + lastConnic) / 2;
							
								if( printVectorInfo )	printf("NEED TO MIX WITH LAST\n");
								if( printVectorInfo )printf("last is %f %f \n", lastPoint.x, lastPoint.y);
							}
						} 
						
						bool doubleConic = false;
						
						int nextIndex = j+1; 
						if( nextIndex >= endPos){
							nextIndex = startPos;
						}
						
						ofPoint nextPoint( (float)vec[nextIndex].x,  -(float)vec[nextIndex].y );	
											
						if( printVectorInfo )printf("--- last point is %f %f \n", lastPoint.x, lastPoint.y);
						
						bool nextIsConnic = (  FT_CURVE_TAG( tags[nextIndex] ) != FT_CURVE_TAG_ON ) && ( FT_CURVE_TAG( tags[nextIndex]) != FT_CURVE_TAG_CUBIC );
					
						//create a 'virtual on point' if we have two connic points
						if( nextIsConnic ){
							nextPoint = (conicPoint + nextPoint) / 2;
							if( printVectorInfo )printf("|_______ double connic!\n");
						}
						if( printVectorInfo )printf("--- next point is %f %f \n", nextPoint.x, nextPoint.y);
						
						quad_bezier(testOutline, lastPoint.x, lastPoint.y, conicPoint.x, conicPoint.y, nextPoint.x, nextPoint.y, 8);
						
						if( nextIsConnic ){
							lastPoint = nextPoint;
						}
					}
				}
			
			//end for
			} 
			
			for(int g =0; g < testOutline.size(); g++){
				testOutline[g] /= 64.0f;
			}
			
			charOutlines.contours.push_back(ofTTFContour());

//			if( testOutline.size() ){
//				vector <ofPoint> simplifiedPts;
//				simplify( testOutline, simplifiedPts, 0.3);
//
//				charOutlines.contours.back().pts = simplifiedPts;
//				//printf("removed %i points = %f % \n", testOutline.size()-simplifiedPts.size(), 100 * ( (float)(testOutline.size()-simplifiedPts.size())/testOutline.size()));
//
//			}else{
				charOutlines.contours.back().pts = testOutline;
//			}
		}
		
	return charOutlines;
}


//------------------------------------------------------------------
ofTrueTypeFont::ofTrueTypeFont(){
	bLoadedOk		= false;
	bMakeContours	= false;
}

//------------------------------------------------------------------
ofTrueTypeFont::~ofTrueTypeFont(){

	if (bLoadedOk){

		if (cps != NULL){
			delete cps;
		}

		if (texNames != NULL){
			for (int i = 0; i < nCharacters; i++){
				glDeleteTextures(1, &texNames[i]);
			}
			delete texNames;
		}
	}
}

//------------------------------------------------------------------
void ofTrueTypeFont::loadFont(string filename, int fontsize){
	// load anti-aliased, non-full character set:
	loadFont(filename, fontsize, true, false, false);
}

//------------------------------------------------------------------
void ofTrueTypeFont::loadFont(string filename, int fontsize, bool _bAntiAliased, bool _bFullCharacterSet, bool makeContours){

	bMakeContours = makeContours;

	//------------------------------------------------
	if (bLoadedOk == true){

		// we've already been loaded, try to clean up :

		if (cps != NULL){
			delete cps;
		}
		if (texNames != NULL){
			for (int i = 0; i < nCharacters; i++){
				glDeleteTextures(1, &texNames[i]);
			}
			delete texNames;
		}
		bLoadedOk = false;
	}
	//------------------------------------------------


	filename = ofToDataPath(filename);

	bLoadedOk 			= false;
	bAntiAlised 		= _bAntiAliased;
	bFullCharacterSet 	= _bFullCharacterSet;
	fontSize			= fontsize;

	//--------------- load the library and typeface
	FT_Library library;
	if (FT_Init_FreeType( &library )){
		ofLog(OF_ERROR," PROBLEM WITH FT lib");
		return;
	}

	FT_Face face;
	if (FT_New_Face( library, filename.c_str(), 0, &face )) {
		return;
	}

	FT_Set_Char_Size( face, fontsize << 6, fontsize << 6, 96, 96);
	lineHeight = fontsize * 1.43f;

	//------------------------------------------------------
	//kerning would be great to support:
	//ofLog(OF_NOTICE,"FT_HAS_KERNING ? %i", FT_HAS_KERNING(face));
	//------------------------------------------------------

	nCharacters = bFullCharacterSet ? 256 : 128 - NUM_CHARACTER_TO_START;

	//--------------- initialize character info and textures
	cps       = new charProps[nCharacters];
	texNames  = new GLuint[nCharacters];
	glGenTextures(nCharacters, texNames);

	if(bMakeContours){
		charOutlines.clear();
		charOutlines.assign(nCharacters, ofTTFCharacter());
	}
	//--------------------- load each char -----------------------
	for (int i = 0 ; i < nCharacters; i++){

		//------------------------------------------ anti aliased or not:
		if(FT_Load_Glyph( face, FT_Get_Char_Index( face, (unsigned char)(i+NUM_CHARACTER_TO_START) ), FT_LOAD_DEFAULT )){
			ofLog(OF_ERROR,"error with FT_Load_Glyph %i", i);
		}

		if (bAntiAlised == true) FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		else FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO);

		//------------------------------------------
		FT_Bitmap& bitmap= face->glyph->bitmap;

		// 3 pixel border around the glyph
		// We show 2 pixels of this, so that blending looks good.
		// 1 pixels is hidden because we don't want to see the real edge of the texture

		border			= 3;
		visibleBorder	= 2;
		
		if(bMakeContours){			
			if( printVectorInfo )printf("\n\ncharacter %c: \n", char( i+NUM_CHARACTER_TO_START ) );
		
			int character = i + NUM_CHARACTER_TO_START; 
			charOutlines[i] = makeContoursForCharacter( face );
		}

		// prepare the texture:
		int width  = ofNextPow2( bitmap.width + border*2 );
		int height = ofNextPow2( bitmap.rows  + border*2 );


		// ------------------------- this is fixing a bug with small type
		// ------------------------- appearantly, opengl has trouble with
		// ------------------------- width or height textures of 1, so we
		// ------------------------- we just set it to 2...
		if (width == 1) width = 2;
		if (height == 1) height = 2;

		// -------------------------
		// info about the character:
		cps[i].value 			= i;
		cps[i].height 			= face->glyph->bitmap_top;
		cps[i].width 			= face->glyph->bitmap.width;
		cps[i].setWidth 		= face->glyph->advance.x >> 6;
		cps[i].topExtent 		= face->glyph->bitmap.rows;
		cps[i].leftExtent		= face->glyph->bitmap_left;

		// texture internals
		cps[i].tTex             = (float)(bitmap.width + visibleBorder*2)  /  (float)width;
		cps[i].vTex             = (float)(bitmap.rows +  visibleBorder*2)   /  (float)height;

		cps[i].xOff             = (float)(border - visibleBorder) / (float)width;
		cps[i].yOff             = (float)(border - visibleBorder) / (float)height;


		/* sanity check:
		ofLog(OF_NOTICE,"%i %i %i %i %i %i",
		cps[i].value ,
		cps[i].height ,
		cps[i].width 	,
		cps[i].setWidth 	,
		cps[i].topExtent ,
		cps[i].leftExtent	);
		*/


		// Allocate Memory For The Texture Data.
		unsigned char* expanded_data = new unsigned char[ 2 * width * height];

		//-------------------------------- clear data:
		for(int j=0; j <height;j++) {
			for(int k=0; k < width; k++){
				expanded_data[2*(k+j*width)  ] = 255;   // every luminance pixel = 255
				expanded_data[2*(k+j*width)+1] = 0;
			}
		}


		if (bAntiAlised == true){
			//-----------------------------------
			for(int j=0; j <height; j++) {
				for(int k=0; k < width; k++){
					if ((k<bitmap.width) && (j<bitmap.rows)){
						expanded_data[2*((k+border)+(j+border)*width)+1] = bitmap.buffer[k + bitmap.width*(j)];
					}
				}
			}
			//-----------------------------------
		} else {
			//-----------------------------------
			// true type packs monochrome info in a
			// 1-bit format, hella funky
			// here we unpack it:
			unsigned char *src =  bitmap.buffer;
			for(int j=0; j <bitmap.rows;j++) {
				unsigned char b;
				unsigned char *bptr =  src;
				for(int k=0; k < bitmap.width ; k++){
					expanded_data[2*((k+1)+(j+1)*width)] = 255;
					if (k%8==0){ b = (*bptr++);}
					expanded_data[2*((k+1)+(j+1)*width) + 1] =
						b&0x80 ? 255 : 0;
				    b <<= 1;
				}
				src += bitmap.pitch;
			}
			//-----------------------------------
		}


		//Now we just setup some texture paramaters.
		glBindTexture( GL_TEXTURE_2D, texNames[i]);
#ifndef TARGET_OF_IPHONE // DAMIAN
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
#endif
   		if (bAntiAlised == true){
			glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		} else {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		}
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		//Here we actually create the texture itself, notice
		//that we are using GL_LUMINANCE_ALPHA to indicate that
		//we are using 2 channel data.
		
#ifndef TARGET_OF_IPHONE // ZACH G   - > gluBuild2DMipmaps doesn't seem to exist in anything i had in the iphone build... so i commented it out
		bool b_use_mipmaps = false;  // FOR now this is fixed to false, could be an option, left in for legacy...
		if (b_use_mipmaps){
   			gluBuild2DMipmaps(
   				GL_TEXTURE_2D, GL_LUMINANCE_ALPHA, width, height,
            	GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);
		} else 
#endif
		{
	    	glTexImage2D( GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, width, height,
			   0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data );
		}


		//With the texture created, we don't need to expanded data anymore

    	delete [] expanded_data;

   }

	// ------------- close the library and typeface
	FT_Done_Face(face);
	FT_Done_FreeType(library);
  	bLoadedOk = true;
}

//-----------------------------------------------------------
int ofTrueTypeFont::ofNextPow2 ( int a )
{
	int rval=1;
	while(rval<a) rval<<=1;
	return rval;
}


//-----------------------------------------------------------
void ofTrueTypeFont::setLineHeight(float _newLineHeight) {
	lineHeight = _newLineHeight;
}

//-----------------------------------------------------------
float ofTrueTypeFont::getLineHeight(){
	return lineHeight;
}

//------------------------------------------------------------------
ofTTFCharacter ofTrueTypeFont::getCharacterAsPoints(int character){
	if( bMakeContours == false ){
		ofLog(OF_ERROR, "getCharacterAsPoints: contours not created,  call loadFont with makeContours set to true" );
	}
	
	if( bMakeContours && charOutlines.size() > 0 && character >= NUM_CHARACTER_TO_START && character - NUM_CHARACTER_TO_START < charOutlines.size() ){
		return charOutlines[character-NUM_CHARACTER_TO_START];
	}else{
		return ofTTFCharacter();
	}
}

//-----------------------------------------------------------
void ofTrueTypeFont::drawChar(int c, float x, float y) {


	//----------------------- error checking
	if (!bLoadedOk){
		ofLog(OF_ERROR,"Error : font not allocated -- line %d in %s", __LINE__,__FILE__);
		return;
	}

	if (c >= nCharacters){
		//ofLog(OF_ERROR,"Error : char (%i) not allocated -- line %d in %s", (c + NUM_CHARACTER_TO_START), __LINE__,__FILE__);
		return;
	}
	//-----------------------

	int cu = c;

	GLint height	= cps[cu].height;
	GLint bwidth	= cps[cu].width;
	GLint top		= cps[cu].topExtent - cps[cu].height;
	GLint lextent	= cps[cu].leftExtent;

	GLint	x1, y1, x2, y2, corr, stretch;
	GLfloat t1, v1, t2, v2;

	//this accounts for the fact that we are showing 2*visibleBorder extra pixels
	//so we make the size of each char that many pixels bigger
	stretch = visibleBorder * 2;


	t2		= cps[cu].xOff;
	v2		= cps[cu].yOff;
	t1		= cps[cu].tTex + t2;
	v1		= cps[cu].vTex + v2;

	corr	= ( (fontSize - height) + top) - fontSize;

	x1		= lextent + bwidth + stretch;
	y1		= height + corr + stretch;
	x2		= lextent;
	y2		= -top + corr;



	if (glIsTexture(texNames[cu])) {
		glBindTexture(GL_TEXTURE_2D, texNames[cu]);
		glNormal3f(0, 0, 1);

//#ifndef TARGET_OF_IPHONE // DAMIAN
//		glBegin(GL_QUADS);
//			glTexCoord2d(t1,v1);	glVertex2i(x1, y1);
//			glTexCoord2d(t1,v2);	glVertex2i(x1, y2);
//			glTexCoord2d(t2,v2);	glVertex2i(x2, y2);
//			glTexCoord2d(t2,v1);	glVertex2i(x2, y1);
//		glEnd();
//#else
		GLfloat verts[] = { x2,y2, 
			x2, y1, 
			x1, y1,
		x1, y2 };
		GLfloat tex_coords[] = { t2, v2,
			t2, v1,
			t1, v1, 
		t1, v2 };

		glEnableClientState( GL_TEXTURE_COORD_ARRAY );		
		glTexCoordPointer(2, GL_FLOAT, 0, tex_coords );
		glEnableClientState( GL_VERTEX_ARRAY );		
		glVertexPointer(2, GL_FLOAT, 0, verts );
		glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
//#endif
	} else {
		//let's add verbosity levels somewhere...
		//this error, for example, is kind of annoying to see
		//all the time:
		//ofLog(OF_WARNING," texture not bound for character -- line %d in %s", __LINE__,__FILE__);
	}

}

//-----------------------------------------------------------
void ofTrueTypeFont::drawCharAsShape(int c, float x, float y) {


	//----------------------- error checking
	if (!bLoadedOk){
		ofLog(OF_ERROR,"Error : font not allocated -- line %d in %s", __LINE__,__FILE__);
		return;
	}
	
	//----------------------- error checking
	if (!bMakeContours){
		ofLog(OF_ERROR,"Error : contours not created for this font - call loadFont with makeContours set to true");
		return;
	}
	
	if (c >= nCharacters){
		//ofLog(OF_ERROR,"Error : char (%i) not allocated -- line %d in %s", (c + NUM_CHARACTER_TO_START), __LINE__,__FILE__);
		return;
	}
	//-----------------------

	int cu = c;
	ofTTFCharacter & charRef = charOutlines[cu];
	
	ofBeginShape();
		for(int k = 0; k < charRef.contours.size(); k++){
			if( k!= 0)ofNextContour(true);
			for(int i = 0; i < charRef.contours[k].pts.size(); i++){
				ofVertex(charRef.contours[k].pts[i].x + x, charRef.contours[k].pts[i].y + y);
			}
		}
	ofEndShape( true );
	
}

//-----------------------------------------------------------
float ofTrueTypeFont::stringWidth(string c) {
    ofRectangle rect = getStringBoundingBox(c, 0,0);
    return rect.width;
}


ofRectangle ofTrueTypeFont::getStringBoundingBox(string c, float x, float y){

    ofRectangle myRect;

	GLint		index	= 0;
	GLfloat		xoffset	= 0;
	GLfloat		yoffset	= 0;
    int         len     = (int)c.length();
    float       minx    = -1;
    float       miny    = -1;
    float       maxx    = -1;
    float       maxy    = -1;

    if (len < 1){
        myRect.x        = 0;
        myRect.y        = 0;
        myRect.width    = 0;
        myRect.height   = 0;
        return myRect;
    }

    bool bFirstCharacter = true;
	while(index < len){
		int cy = (unsigned char)c[index] - NUM_CHARACTER_TO_START;
 	    if (cy < nCharacters){ 			// full char set or not?
	       if (c[index] == '\n') {
				yoffset += (int)lineHeight;
				xoffset = 0 ; //reset X Pos back to zero
	      } else if (c[index] == ' ') {
	     		int cy = (int)'p' - NUM_CHARACTER_TO_START;
				 xoffset += cps[cy].width;
				 // zach - this is a bug to fix -- for now, we don't currently deal with ' ' in calculating string bounding box
		  } else {
                GLint height	= cps[cy].height;
            	GLint bwidth	= cps[cy].width;
            	GLint top		= cps[cy].topExtent - cps[cy].height;
            	GLint lextent	= cps[cy].leftExtent;
            	float	x1, y1, x2, y2, corr, stretch;
            	stretch = visibleBorder * 2;
	            corr	= ( (fontSize - height) + top) - fontSize;
            	x1		= (x + xoffset + lextent + bwidth + stretch);
            	y1		= (y + yoffset + height + corr + stretch);
            	x2		= (x + xoffset + lextent);
            	y2		= (y + yoffset + -top + corr);
				xoffset += cps[cy].setWidth;
				if (bFirstCharacter == true){
                    minx = x2;
                    miny = y2;
                    maxx = x1;
                    maxy = y1;
                    bFirstCharacter = false;
                } else {
                    if (x2 < minx) minx = x2;
                    if (y2 < miny) miny = y2;
                    if (x1 > maxx) maxx = x1;
                    if (y1 > maxy) maxy = y1;
            }
		  }
	  	}
    	index++;
    }

    myRect.x        = minx;
    myRect.y        = miny;
    myRect.width    = maxx-minx;
    myRect.height   = maxy-miny;
    return myRect;
}



//-----------------------------------------------------------
float ofTrueTypeFont::stringHeight(string c) {
    ofRectangle rect = getStringBoundingBox(c, 0,0);
    return rect.height;
}

//=====================================================================
void ofTrueTypeFont::drawString(string c, float x, float y) {

    if (!bLoadedOk){
    	ofLog(OF_ERROR,"Error : font not allocated -- line %d in %s", __LINE__,__FILE__);
    	return;
    };

    // we need transparency to draw text, but we don't know
    // if that is set up in outside of this function
    // we "pushAttrib", turn on alpha and "popAttrib"
    // http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/pushattrib.html

    // **** note ****
    // I have read that pushAttrib() is slow, if used often,
    // maybe there is a faster way to do this?
    // ie, check if blending is enabled, etc...
    // glIsEnabled().... glGet()...
    // http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/get.html
    // **************

	GLint		index	= 0;
	GLfloat		X		= 0;
	GLfloat		Y		= 0;

	// (a) record the current "alpha state, blend func, etc"
#ifndef TARGET_OF_IPHONE
    glPushAttrib(GL_COLOR_BUFFER_BIT);
#else
	GLboolean blend_enabled = glIsEnabled(GL_BLEND);
	GLboolean texture_2d_enabled = glIsEnabled(GL_TEXTURE_2D);
	GLint blend_src, blend_dst;
	glGetIntegerv( GL_BLEND_SRC, &blend_src );
	glGetIntegerv( GL_BLEND_DST, &blend_dst );
#endif
	
    // (b) enable our regular ALPHA blending!
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// (c) enable texture once before we start drawing each char (no point turning it on and off constantly)
	glEnable(GL_TEXTURE_2D);
	// (d) store the current matrix position and do a translation to the drawing position
	glPushMatrix();
	glTranslatef(x, y, 0);

	int len = (int)c.length();

	while(index < len){
		int cy = (unsigned char)c[index] - NUM_CHARACTER_TO_START;
		if (cy < nCharacters){ 			// full char set or not?
		  if (c[index] == '\n') {

				Y = (int)lineHeight;
				glTranslatef(-X, Y, 0);
				X = 0 ; //reset X Pos back to zero

		  }else if (c[index] == ' ') {
				 int cy = (int)'p' - NUM_CHARACTER_TO_START;
				 X += cps[cy].width;
				 glTranslatef(cps[cy].width, 0, 0);
		  } else {
				drawChar(cy, 0, 0);
				X += cps[cy].setWidth;
				glTranslatef(cps[cy].setWidth, 0, 0);
		  }
		}
		index++;
	}
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
    // (c) return back to the way things were (with blending, blend func, etc)
#ifndef TARGET_OF_IPHONE
    glPopAttrib();
#else
	if ( !blend_enabled ) 
		glDisable(GL_BLEND);
	if ( !texture_2d_enabled )
		glDisable(GL_TEXTURE_2D);
	glBlendFunc( blend_src, blend_dst );
#endif

}

//=====================================================================
void ofTrueTypeFont::drawStringAsShapes(string c, float x, float y) {

    if (!bLoadedOk){
    	ofLog(OF_ERROR,"Error : font not allocated -- line %d in %s", __LINE__,__FILE__);
    	return;
    };

	//----------------------- error checking
	if (!bMakeContours){
		ofLog(OF_ERROR,"Error : contours not created for this font - call loadFont with makeContours set to true");
		return;
	}

	GLint		index	= 0;
	GLfloat		X		= 0;
	GLfloat		Y		= 0;

	glPushMatrix();
	glTranslated(x, y, 0);

	int len = (int)c.length();

	while(index < len){
		int cy = (unsigned char)c[index] - NUM_CHARACTER_TO_START;
		if (cy < nCharacters){ 			// full char set or not?
		  if (c[index] == '\n') {

				Y = (int)lineHeight;
				X = 0 ; //reset X Pos back to zero

		  }else if (c[index] == ' ') {
				 int cy = (int)'p' - NUM_CHARACTER_TO_START;
				 X += cps[cy].width;
				 //glTranslated(cps[cy].width, 0, 0);
		  } else {
				drawCharAsShape(cy, X, Y);
				X += cps[cy].setWidth;
				//glTranslated(cps[cy].setWidth, 0, 0);
		  }
		}
		index++;
	}
	
	glPopMatrix();

}


