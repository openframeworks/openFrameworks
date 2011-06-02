#include "ofTrueTypeFont.h"
//--------------------------

#include "ft2build.h"
#include "freetype2/freetype/freetype.h"
#include "freetype2/freetype/ftglyph.h"
#include "freetype2/freetype/ftoutln.h"
#include "freetype2/freetype/fttrigon.h"

#include <algorithm>

#include "ofUtils.h"
#include "ofGraphics.h"
#include "ofPixelUtils.h"

static bool printVectorInfo = false;

//--------------------------------------------------------
static ofTTFCharacter makeContoursForCharacter(FT_Face &face);
static ofTTFCharacter makeContoursForCharacter(FT_Face &face){

		//int num			= face->glyph->outline.n_points;
		int nContours	= face->glyph->outline.n_contours;
		int startPos	= 0;

		char * tags		= face->glyph->outline.tags;
		FT_Vector * vec = face->glyph->outline.points;

		ofTTFCharacter charOutlines;
		charOutlines.setUseShapeColor(false);

		for(int k = 0; k < nContours; k++){
			if( k > 0 ){
				startPos = face->glyph->outline.contours[k-1]+1;
			}
			int endPos = face->glyph->outline.contours[k]+1;

			if( printVectorInfo )printf("--NEW CONTOUR\n\n");

			//vector <ofPoint> testOutline;
			ofPoint lastPoint;

			for(int j = startPos; j < endPos; j++){

				if( FT_CURVE_TAG(tags[j]) == FT_CURVE_TAG_ON ){
					lastPoint.set((float)vec[j].x, (float)-vec[j].y, 0);
					if( printVectorInfo )printf("flag[%i] is set to 1 - regular point - %f %f \n", j, lastPoint.x, lastPoint.y);
					//testOutline.push_back(lastPoint);
					charOutlines.lineTo(lastPoint/64);

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
							ofPoint controlPoint1((float)vec[prevPoint].x,	(float)-vec[prevPoint].y);
							ofPoint controlPoint2((float)vec[j].x, (float)-vec[j].y);
							ofPoint nextPoint((float) vec[nextIndex].x,	-(float) vec[nextIndex].y);

							//cubic_bezier(testOutline, lastPoint.x, lastPoint.y, controlPoint1.x, controlPoint1.y, controlPoint2.x, controlPoint2.y, nextPoint.x, nextPoint.y, 8);
							charOutlines.bezierTo(controlPoint1.x/64, controlPoint1.y/64, controlPoint2.x/64, controlPoint2.y/64, nextPoint.x/64, nextPoint.y/64);
						}

					}else{

						ofPoint conicPoint( (float)vec[j].x,  -(float)vec[j].y );

						if( printVectorInfo )printf("- bit 2 is set to 0 - conic- \n");
						if( printVectorInfo )printf("--- conicPoint point is %f %f \n", conicPoint.x, conicPoint.y);

						//If the first point is connic and the last point is connic then we need to create a virutal point which acts as a wrap around
						if( j == startPos ){
							bool prevIsConnic = (  FT_CURVE_TAG( tags[endPos-1] ) != FT_CURVE_TAG_ON ) && ( FT_CURVE_TAG( tags[endPos-1]) != FT_CURVE_TAG_CUBIC );

							if( prevIsConnic ){
								ofPoint lastConnic((float)vec[endPos - 1].x, (float)-vec[endPos - 1].y);
								lastPoint = (conicPoint + lastConnic) / 2;

								if( printVectorInfo )	printf("NEED TO MIX WITH LAST\n");
								if( printVectorInfo )printf("last is %f %f \n", lastPoint.x, lastPoint.y);
							}
						}

						//bool doubleConic = false;

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

						//quad_bezier(testOutline, lastPoint.x, lastPoint.y, conicPoint.x, conicPoint.y, nextPoint.x, nextPoint.y, 8);
						charOutlines.quadBezierTo(lastPoint.x/64, lastPoint.y/64, conicPoint.x/64, conicPoint.y/64, nextPoint.x/64, nextPoint.y/64);

						if( nextIsConnic ){
							lastPoint = nextPoint;
						}
					}
				}

			//end for
			}
			charOutlines.close();
		}

	return charOutlines;
}

#ifdef TARGET_ANDROID
	#include <set>
	set<ofTrueTypeFont*> all_fonts;
	void ofUnloadAllFontTextures(){
		set<ofTrueTypeFont*>::iterator it;
		for(it=all_fonts.begin();it!=all_fonts.end();it++){
			(*it)->unloadTextures();
		}
	}
	void ofReloadAllFontTextures(){
		set<ofTrueTypeFont*>::iterator it;
		for(it=all_fonts.begin();it!=all_fonts.end();it++){
			(*it)->reloadTextures();
		}
	}

#endif

bool compare_cps(const charProps & c1, const charProps & c2){
	if(c1.tH == c2.tH) return c1.tW > c2.tW;
	else return c1.tH > c2.tH;
}

//------------------------------------------------------------------
ofTrueTypeFont::ofTrueTypeFont(){
	bLoadedOk		= false;
	bMakeContours	= false;
	#ifdef TARGET_ANDROID
		all_fonts.insert(this);
	#endif
	//cps				= NULL;
	letterSpacing = 1;
	spaceSize = 1;

	// 3 pixel border around the glyph
	// We show 2 pixels of this, so that blending looks good.
	// 1 pixels is hidden because we don't want to see the real edge of the texture

	border			= 3;
	//visibleBorder	= 2;
	stringQuads.setMode(OF_TRIANGLES_MODE);
	binded = false;
}

//------------------------------------------------------------------
ofTrueTypeFont::~ofTrueTypeFont(){

	if (bLoadedOk){
		unloadTextures();
	}

	#ifdef TARGET_ANDROID
		all_fonts.erase(this);
	#endif
}

void ofTrueTypeFont::unloadTextures(){
	if(!bLoadedOk) return;

	texAtlas.clear();
	bLoadedOk = false;
}

void ofTrueTypeFont::reloadTextures(){
	loadFont(filename,fontSize,bAntiAlised,bFullCharacterSet,false);
}

//------------------------------------------------------------------
void ofTrueTypeFont::loadFont(string filename, int fontsize, bool _bAntiAliased, bool _bFullCharacterSet, bool makeContours, float simplifyAmt){

	bMakeContours = makeContours;

	//------------------------------------------------
	if (bLoadedOk == true){

		// we've already been loaded, try to clean up :
		unloadTextures();
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
		ofLog(OF_LOG_ERROR," PROBLEM WITH FT lib");
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
	//ofLog(OF_LOG_NOTICE,"FT_HAS_KERNING ? %i", FT_HAS_KERNING(face));
	//------------------------------------------------------

	nCharacters = bFullCharacterSet ? 256 : 128 - NUM_CHARACTER_TO_START;

	//--------------- initialize character info and textures
	cps.resize(nCharacters);

	if(bMakeContours){
		charOutlines.clear();
		charOutlines.assign(nCharacters, ofTTFCharacter());
	}

	vector<ofPixels> expanded_data(nCharacters);

	long areaSum=0;

	//--------------------- load each char -----------------------
	for (int i = 0 ; i < nCharacters; i++){

		//------------------------------------------ anti aliased or not:
		if(FT_Load_Glyph( face, FT_Get_Char_Index( face, (unsigned char)(i+NUM_CHARACTER_TO_START) ), FT_LOAD_DEFAULT )){
			ofLog(OF_LOG_ERROR,"error with FT_Load_Glyph %i", i);
		}

		if (bAntiAlised == true) FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		else FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO);

		//------------------------------------------
		FT_Bitmap& bitmap= face->glyph->bitmap;


		// prepare the texture:
		/*int width  = ofNextPow2( bitmap.width + border*2 );
		int height = ofNextPow2( bitmap.rows  + border*2 );


		// ------------------------- this is fixing a bug with small type
		// ------------------------- appearantly, opengl has trouble with
		// ------------------------- width or height textures of 1, so we
		// ------------------------- we just set it to 2...
		if (width == 1) width = 2;
		if (height == 1) height = 2;*/


		if(bMakeContours){
			if( printVectorInfo )printf("\n\ncharacter %c: \n", char( i+NUM_CHARACTER_TO_START ) );

			//int character = i + NUM_CHARACTER_TO_START;
			charOutlines[i] = makeContoursForCharacter( face );
			if(simplifyAmt>0)
				charOutlines[i].simplify(simplifyAmt);
			charOutlines[i].getTessellation();
		}


		// -------------------------
		// info about the character:
		cps[i].character		= i;
		cps[i].height 			= face->glyph->bitmap_top;
		cps[i].width 			= face->glyph->bitmap.width;
		cps[i].setWidth 		= face->glyph->advance.x >> 6;
		cps[i].topExtent 		= face->glyph->bitmap.rows;
		cps[i].leftExtent		= face->glyph->bitmap_left;

		int width  = cps[i].width;
		int height = bitmap.rows;


		cps[i].tW				= width;
		cps[i].tH				= height;



		GLint fheight	= cps[i].height;
		GLint bwidth	= cps[i].width;
		GLint top		= cps[i].topExtent - cps[i].height;
		GLint lextent	= cps[i].leftExtent;

		GLfloat	corr, stretch;

		//this accounts for the fact that we are showing 2*visibleBorder extra pixels
		//so we make the size of each char that many pixels bigger
		stretch = 0;//(float)(visibleBorder * 2);

		corr	= (float)(( (fontSize - fheight) + top) - fontSize);

		cps[i].x1		= lextent + bwidth + stretch;
		cps[i].y1		= fheight + corr + stretch;
		cps[i].x2		= (float) lextent;
		cps[i].y2		= -top + corr;


		// Allocate Memory For The Texture Data.
		expanded_data[i].allocate(width, height, 2);
		//-------------------------------- clear data:
		expanded_data[i].set(0,255); // every luminance pixel = 255
		expanded_data[i].set(1,0);


		if (bAntiAlised == true){
			ofPixels bitmapPixels;
			bitmapPixels.setFromExternalPixels(bitmap.buffer,bitmap.width,bitmap.rows,1);
			expanded_data[i].setChannel(1,bitmapPixels);
		} else {
			//-----------------------------------
			// true type packs monochrome info in a
			// 1-bit format, hella funky
			// here we unpack it:
			unsigned char *src =  bitmap.buffer;
			for(int j=0; j <bitmap.rows;j++) {
				unsigned char b=0;
				unsigned char *bptr =  src;
				for(int k=0; k < bitmap.width ; k++){
					expanded_data[i][2*(k+j*width)] = 255;

					if (k%8==0){
						b = (*bptr++);
					}

					expanded_data[i][2*(k+j*width) + 1] = b&0x80 ? 255 : 0;
					b <<= 1;
				}
				src += bitmap.pitch;
			}
			//-----------------------------------
		}

		areaSum += (cps[i].width+border*2)*(cps[i].height+border*2);
	}


	vector<charProps> sortedCopy = cps;
	sort(sortedCopy.begin(),sortedCopy.end(),&compare_cps);

	// pack in a texture, algorithm to calculate min w/h from
	// http://upcommons.upc.edu/pfc/bitstream/2099.1/7720/1/TesiMasterJonas.pdf
	//cout << areaSum << endl;

	bool packed = false;
	float alpha = logf(areaSum)*1.44269;

	int w;
	int h;
	while(!packed){
		w = pow(2,floor((alpha/2.f) + 0.5)); // there doesn't seem to be a round in cmath for windows. 
		//w = pow(2,round(alpha/2.f));
		h = w;//pow(2,round(alpha - round(alpha/2.f)));
		int x=0;
		int y=0;
		int maxRowHeight = sortedCopy[0].tH + border*2;
		for(int i=0;i<(int)cps.size();i++){
			if(x+sortedCopy[i].tW + border*2>w){
				x = 0;
				y += maxRowHeight;
				maxRowHeight = sortedCopy[i].tH + border*2;
				if(y + maxRowHeight > h){
					alpha++;
					break;
				}
			}
			x+= sortedCopy[i].tW + border*2;
			if(i==(int)cps.size()-1) packed = true;
		}

	}



	ofPixels atlasPixels;
	atlasPixels.allocate(w,h,2);
	atlasPixels.set(0,255);
	atlasPixels.set(1,0);


	int x=0;
	int y=0;
	int maxRowHeight = sortedCopy[0].tH + border*2;
	for(int i=0;i<(int)cps.size();i++){
		ofPixels & charPixels = expanded_data[sortedCopy[i].character];

		if(x+sortedCopy[i].tW + border*2>w){
			x = 0;
			y += maxRowHeight;
			maxRowHeight = sortedCopy[i].tH + border*2;
		}

		cps[sortedCopy[i].character].t2		= float(x + border)/float(w);
		cps[sortedCopy[i].character].v2		= float(y + border)/float(h);
		cps[sortedCopy[i].character].t1		= float(cps[sortedCopy[i].character].tW + x + border)/float(w);
		cps[sortedCopy[i].character].v1		= float(cps[sortedCopy[i].character].tH + y + border)/float(h);
		ofPixelUtils::pasteInto(charPixels,atlasPixels,x+border,y+border);
		x+= sortedCopy[i].tW + border*2;
	}


	texAtlas.allocate(atlasPixels.getWidth(),atlasPixels.getHeight(),GL_LUMINANCE_ALPHA,false);
	if(bAntiAlised){
		texAtlas.setTextureMinMagFilter(GL_LINEAR,GL_LINEAR);
	}else{
		texAtlas.setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
	}

	texAtlas.loadData(atlasPixels.getPixels(),atlasPixels.getWidth(),atlasPixels.getHeight(),GL_LUMINANCE_ALPHA);

	// ------------- close the library and typeface
	FT_Done_Face(face);
	FT_Done_FreeType(library);
  	bLoadedOk = true;
}

//-----------------------------------------------------------
void ofTrueTypeFont::setLineHeight(float _newLineHeight) {
	lineHeight = _newLineHeight;
}

//-----------------------------------------------------------
float ofTrueTypeFont::getLineHeight(){
	return lineHeight;
}

//-----------------------------------------------------------
void ofTrueTypeFont::setLetterSpacing(float _newletterSpacing) {
	letterSpacing = _newletterSpacing;
}

//-----------------------------------------------------------
float ofTrueTypeFont::getLetterSpacing(){
	return letterSpacing;
}

//-----------------------------------------------------------
void ofTrueTypeFont::setSpaceSize(float _newspaceSize) {
	spaceSize = _newspaceSize;
}

//-----------------------------------------------------------
float ofTrueTypeFont::getSpaceSize(){
	return spaceSize;
}

//------------------------------------------------------------------
ofTTFCharacter ofTrueTypeFont::getCharacterAsPoints(int character){
	if( bMakeContours == false ){
		ofLog(OF_LOG_ERROR, "getCharacterAsPoints: contours not created,  call loadFont with makeContours set to true" );
	}

	if( bMakeContours && (int)charOutlines.size() > 0 && character >= NUM_CHARACTER_TO_START && character - NUM_CHARACTER_TO_START < (int)charOutlines.size() ){
		return charOutlines[character-NUM_CHARACTER_TO_START];
	}else{
		if(charOutlines.empty())charOutlines.push_back(ofTTFCharacter());
		return charOutlines[0];
	}
}

//-----------------------------------------------------------
void ofTrueTypeFont::drawChar(int c, float x, float y) {

	if (c >= nCharacters){
		//ofLog(OF_LOG_ERROR,"Error : char (%i) not allocated -- line %d in %s", (c + NUM_CHARACTER_TO_START), __LINE__,__FILE__);
		return;
	}

	GLfloat	x1, y1, x2, y2;
	GLfloat t1, v1, t2, v2;
	t2		= cps[c].t2;
	v2		= cps[c].v2;
	t1		= cps[c].t1;
	v1		= cps[c].v1;

	x1		= cps[c].x1+x;
	y1		= cps[c].y1+y;
	x2		= cps[c].x2+x;
	y2		= cps[c].y2+y;

	int firstIndex = stringQuads.getVertices().size();

	stringQuads.addVertex(ofVec3f(x1,y1));
	stringQuads.addVertex(ofVec3f(x2,y1));
	stringQuads.addVertex(ofVec3f(x2,y2));
	stringQuads.addVertex(ofVec3f(x1,y2));

	stringQuads.addTexCoord(ofVec2f(t1,v1));
	stringQuads.addTexCoord(ofVec2f(t2,v1));
	stringQuads.addTexCoord(ofVec2f(t2,v2));
	stringQuads.addTexCoord(ofVec2f(t1,v2));

	stringQuads.addIndex(firstIndex);
	stringQuads.addIndex(firstIndex+1);
	stringQuads.addIndex(firstIndex+2);
	stringQuads.addIndex(firstIndex+2);
	stringQuads.addIndex(firstIndex+3);
	stringQuads.addIndex(firstIndex);
}

//-----------------------------------------------------------
void ofTrueTypeFont::drawCharAsShape(int c, float x, float y) {
	if (c >= nCharacters){
		//ofLog(OF_LOG_ERROR,"Error : char (%i) not allocated -- line %d in %s", (c + NUM_CHARACTER_TO_START), __LINE__,__FILE__);
		return;
	}
	//-----------------------

	int cu = c;
	ofTTFCharacter & charRef = charOutlines[cu];
	charRef.setFilled(ofGetStyle().bFill);
	charRef.draw(x,y);
}

//-----------------------------------------------------------
float ofTrueTypeFont::stringWidth(string c) {
    ofRectangle rect = getStringBoundingBox(c, 0,0);
    return rect.width;
}


ofRectangle ofTrueTypeFont::getStringBoundingBox(string c, float x, float y){

    ofRectangle myRect;

    if (!bLoadedOk){
    	ofLog(OF_LOG_ERROR,"ofTrueTypeFont::getStringBoundingBox - font not allocated");
    	return myRect;
    }

	GLint		index	= 0;
	GLfloat		xoffset	= 0;
	GLfloat		yoffset	= 0;
    int         len     = (int)c.length();
    float       minx    = -1;
    float       miny    = -1;
    float       maxx    = -1;
    float       maxy    = -1;

    if ( len < 1 || cps.empty() ){
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
				 xoffset += cps[cy].width * letterSpacing * spaceSize;
				 // zach - this is a bug to fix -- for now, we don't currently deal with ' ' in calculating string bounding box
		  } else {
                GLint height	= cps[cy].height;
            	GLint bwidth	= cps[cy].width * letterSpacing;
            	GLint top		= cps[cy].topExtent - cps[cy].height;
            	GLint lextent	= cps[cy].leftExtent;
            	float	x1, y1, x2, y2, corr, stretch;
            	stretch = 0;//(float)visibleBorder * 2;
				corr = (float)(((fontSize - height) + top) - fontSize);
				x1		= (x + xoffset + lextent + bwidth + stretch);
            	y1		= (y + yoffset + height + corr + stretch);
            	x2		= (x + xoffset + lextent);
            	y2		= (y + yoffset + -top + corr);
				xoffset += cps[cy].setWidth * letterSpacing;
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
    	ofLog(OF_LOG_ERROR,"Error : font not allocated -- line %d in %s", __LINE__,__FILE__);
    	return;
    };

	GLint		index	= 0;
	GLfloat		X		= x;
	GLfloat		Y		= y;


	bool alreadyBinded = binded;

	if(!alreadyBinded) bind();

	int len = (int)c.length();

	while(index < len){
		int cy = (unsigned char)c[index] - NUM_CHARACTER_TO_START;
		if (cy < nCharacters){ 			// full char set or not?
		  if (c[index] == '\n') {

				Y += (float) lineHeight;
				X = x ; //reset X Pos back to zero

		  }else if (c[index] == ' ') {
				 int cy = (int)'p' - NUM_CHARACTER_TO_START;
				 X += cps[cy].width * letterSpacing * spaceSize;
		  } else {
				drawChar(cy, X, Y);
				X += cps[cy].setWidth * letterSpacing;
		  }
		}
		index++;
	}

	if(!alreadyBinded) unbind();

}

//-----------------------------------------------------------
void ofTrueTypeFont::bind(){
	if(!binded){
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
		// (a) record the current "alpha state, blend func, etc"
		#ifndef TARGET_OPENGLES
			glPushAttrib(GL_COLOR_BUFFER_BIT);
		#else
			blend_enabled = glIsEnabled(GL_BLEND);
			texture_2d_enabled = glIsEnabled(GL_TEXTURE_2D);
			glGetIntegerv( GL_BLEND_SRC, &blend_src );
			glGetIntegerv( GL_BLEND_DST, &blend_dst );
		#endif

	    // (b) enable our regular ALPHA blending!
	    glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		texAtlas.bind();
		stringQuads.clear();
		binded = true;
	}
}

//-----------------------------------------------------------
void ofTrueTypeFont::unbind(){
	if(binded){
		stringQuads.drawFaces();
		texAtlas.unbind();

		#ifndef TARGET_OPENGLES
			glPopAttrib();
		#else
			if( !blend_enabled )
				glDisable(GL_BLEND);
			if( !texture_2d_enabled )
				glDisable(GL_TEXTURE_2D);
			glBlendFunc( blend_src, blend_dst );
		#endif
		binded = false;
	}
}

//=====================================================================
void ofTrueTypeFont::drawStringAsShapes(string c, float x, float y) {

    if (!bLoadedOk){
    	ofLog(OF_LOG_ERROR,"Error : font not allocated -- line %d in %s", __LINE__,__FILE__);
    	return;
    };

	//----------------------- error checking
	if (!bMakeContours){
		ofLog(OF_LOG_ERROR,"Error : contours not created for this font - call loadFont with makeContours set to true");
		return;
	}

	GLint		index	= 0;
	GLfloat		X		= 0;
	GLfloat		Y		= 0;

	ofPushMatrix();
	ofTranslate(x, y);
	int len = (int)c.length();

	while(index < len){
		int cy = (unsigned char)c[index] - NUM_CHARACTER_TO_START;
		if (cy < nCharacters){ 			// full char set or not?
		  if (c[index] == '\n') {

				Y += lineHeight;
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

	ofPopMatrix();

}


