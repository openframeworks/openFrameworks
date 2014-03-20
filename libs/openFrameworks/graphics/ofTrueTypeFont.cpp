#include "ofTrueTypeFont.h"
//--------------------------

#include "ft2build.h"

#ifdef TARGET_LINUX
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_TRIGONOMETRY_H
#include <fontconfig/fontconfig.h>
#else
#include "freetype2/freetype/freetype.h"
#include "freetype2/freetype/ftglyph.h"
#include "freetype2/freetype/ftoutln.h"
#include "freetype2/freetype/fttrigon.h"
#endif

#include <algorithm>

#include "ofUtils.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "Poco/TextConverter.h"
#include "Poco/UTF8Encoding.h"
#include "Poco/Latin1Encoding.h"
#include "Poco/Latin9Encoding.h"

static bool printVectorInfo = false;
static int ttfGlobalDpi = 96;
static bool librariesInitialized = false;
static FT_Library library;

//--------------------------------------------------------
void ofTrueTypeFont::setGlobalDpi(int newDpi){
	ttfGlobalDpi = newDpi;
}

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

			if(printVectorInfo){
				ofLogNotice("ofTrueTypeFont") << "--NEW CONTOUR";
			}

			//vector <ofPoint> testOutline;
			ofPoint lastPoint;

			for(int j = startPos; j < endPos; j++){

				if( FT_CURVE_TAG(tags[j]) == FT_CURVE_TAG_ON ){
					lastPoint.set((float)vec[j].x, (float)-vec[j].y, 0);
					if(printVectorInfo){
						ofLogNotice("ofTrueTypeFont") << "flag[" << j << "] is set to 1 - regular point - " << lastPoint.x <<  lastPoint.y;
					}
					charOutlines.lineTo(lastPoint/64);

				}else{
					if(printVectorInfo){
						ofLogNotice("ofTrueTypeFont") << "flag[" << j << "] is set to 0 - control point";
					}

					if( FT_CURVE_TAG(tags[j]) == FT_CURVE_TAG_CUBIC ){
						if(printVectorInfo){
							ofLogNotice("ofTrueTypeFont") << "- bit 2 is set to 2 - CUBIC";
						}

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

						if(printVectorInfo){
							ofLogNotice("ofTrueTypeFont") << "- bit 2 is set to 0 - conic- ";
							ofLogNotice("ofTrueTypeFont") << "--- conicPoint point is " << conicPoint.x << conicPoint.y;
						}

						//If the first point is connic and the last point is connic then we need to create a virutal point which acts as a wrap around
						if( j == startPos ){
							bool prevIsConnic = (  FT_CURVE_TAG( tags[endPos-1] ) != FT_CURVE_TAG_ON ) && ( FT_CURVE_TAG( tags[endPos-1]) != FT_CURVE_TAG_CUBIC );

							if( prevIsConnic ){
								ofPoint lastConnic((float)vec[endPos - 1].x, (float)-vec[endPos - 1].y);
								lastPoint = (conicPoint + lastConnic) / 2;

								if(printVectorInfo){
									ofLogNotice("ofTrueTypeFont") << "NEED TO MIX WITH LAST";
									ofLogNotice("ofTrueTypeFont") << "last is " << lastPoint.x << " " << lastPoint.y;
								}
							}
						}

						//bool doubleConic = false;

						int nextIndex = j+1;
						if( nextIndex >= endPos){
							nextIndex = startPos;
						}

						ofPoint nextPoint( (float)vec[nextIndex].x,  -(float)vec[nextIndex].y );

						if(printVectorInfo){
							ofLogNotice("ofTrueTypeFont") << "--- last point is " << lastPoint.x << " " <<  lastPoint.y;
						}

						bool nextIsConnic = (  FT_CURVE_TAG( tags[nextIndex] ) != FT_CURVE_TAG_ON ) && ( FT_CURVE_TAG( tags[nextIndex]) != FT_CURVE_TAG_CUBIC );

						//create a 'virtual on point' if we have two connic points
						if( nextIsConnic ){
							nextPoint = (conicPoint + nextPoint) / 2;
							if(printVectorInfo){
								ofLogNotice("ofTrueTypeFont") << "|_______ double connic!";
							}
						}
						if(printVectorInfo){
							ofLogNotice("ofTrueTypeFont") << "--- next point is " << nextPoint.x << " " << nextPoint.y;
						}

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

#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
	#include <set>
	static set<ofTrueTypeFont*> & all_fonts(){
		static set<ofTrueTypeFont*> *all_fonts = new set<ofTrueTypeFont*>;
		return *all_fonts;
	}

	void ofUnloadAllFontTextures(){
		set<ofTrueTypeFont*>::iterator it;
		for(it=all_fonts().begin();it!=all_fonts().end();it++){
			(*it)->unloadTextures();
		}
	}

	void ofReloadAllFontTextures(){
		set<ofTrueTypeFont*>::iterator it;
		for(it=all_fonts().begin();it!=all_fonts().end();it++){
			(*it)->reloadTextures();
		}
	}

#endif

bool compare_cps(const charProps & c1, const charProps & c2){
	if(c1.tH == c2.tH) return c1.tW > c2.tW;
	else return c1.tH > c2.tH;
}


#ifdef TARGET_OSX
static string osxFontPathByName( string fontname ){
	CFStringRef targetName = CFStringCreateWithCString(NULL, fontname.c_str(), kCFStringEncodingUTF8);
	CTFontDescriptorRef targetDescriptor = CTFontDescriptorCreateWithNameAndSize(targetName, 0.0);
	CFURLRef targetURL = (CFURLRef) CTFontDescriptorCopyAttribute(targetDescriptor, kCTFontURLAttribute);
	string fontPath = "";
	
	if(targetURL) {
		UInt8 buffer[PATH_MAX];
		CFURLGetFileSystemRepresentation(targetURL, true, buffer, PATH_MAX);
		fontPath = string((char *)buffer);
		CFRelease(targetURL);
	}
	
	CFRelease(targetName);
	CFRelease(targetDescriptor);

	return fontPath;
}
#endif

#ifdef TARGET_WIN32
#include <map>
// font font face -> file name name mapping
static map<string, string> fonts_table;
// read font linking information from registry, and store in std::map
void initWindows(){
	LONG l_ret;

	const wchar_t *Fonts = L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts";

	HKEY key_ft;
	l_ret = RegOpenKeyExW(HKEY_LOCAL_MACHINE, Fonts, 0, KEY_QUERY_VALUE, &key_ft);
	if (l_ret != ERROR_SUCCESS){
	    ofLogError("ofTrueTypeFont") << "initWindows(): couldn't find fonts registery key";
        return;
	}

	DWORD value_count;
	DWORD max_data_len;
	wchar_t value_name[2048];
	BYTE *value_data;


	// get font_file_name -> font_face mapping from the "Fonts" registry key

	l_ret = RegQueryInfoKeyW(key_ft, NULL, NULL, NULL, NULL, NULL, NULL, &value_count, NULL, &max_data_len, NULL, NULL);
	if(l_ret != ERROR_SUCCESS){
	    ofLogError("ofTrueTypeFont") << "initWindows(): couldn't query registery for fonts";
        return;
	}

	// no font installed
	if (value_count == 0){
	    ofLogError("ofTrueTypeFont") << "initWindows(): couldn't find any fonts in registery";
        return;
	}

	// max_data_len is in BYTE
	value_data = static_cast<BYTE *>(HeapAlloc(GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS, max_data_len));
	if(value_data == NULL) return;

	char value_name_char[2048];
	char value_data_char[2048];
	/*char ppidl[2048];
	char fontsPath[2048];
    SHGetKnownFolderIDList(FOLDERID_Fonts, 0, NULL, &ppidl);
    SHGetPathFromIDList(ppidl,&fontsPath);*/
    string fontsDir = getenv ("windir");
    fontsDir += "\\Fonts\\";
	for (DWORD i = 0; i < value_count; ++i)
	{
			DWORD name_len = 2048;
			DWORD data_len = max_data_len;

			l_ret = RegEnumValueW(key_ft, i, value_name, &name_len, NULL, NULL, value_data, &data_len);
			if(l_ret != ERROR_SUCCESS){
			     ofLogError("ofTrueTypeFont") << "initWindows(): couldn't read registry key for font type";
			     continue;
			}

            wcstombs(value_name_char,value_name,2048);
			wcstombs(value_data_char,reinterpret_cast<wchar_t *>(value_data),2048);
			string curr_face = value_name_char;
			string font_file = value_data_char;
			curr_face = curr_face.substr(0, curr_face.find('(') - 1);
			fonts_table[curr_face] = fontsDir + font_file;
	}


	HeapFree(GetProcessHeap(), 0, value_data);

	l_ret = RegCloseKey(key_ft);
}


static string winFontPathByName( string fontname ){
    if(fonts_table.find(fontname)!=fonts_table.end()){
        return fonts_table[fontname];
    }
    for(map<string,string>::iterator it = fonts_table.begin(); it!=fonts_table.end(); it++){
        if(ofIsStringInString(ofToLower(it->first),ofToLower(fontname))) return it->second;
    }
    return "";
}
#endif

#ifdef TARGET_LINUX
static string linuxFontPathByName(string fontname){
	string filename;
	FcPattern * pattern = FcNameParse((const FcChar8*)fontname.c_str());
	FcBool ret = FcConfigSubstitute(0,pattern,FcMatchPattern);
	if(!ret){
		ofLogError() << "linuxFontPathByName(): couldn't find font file or system font with name \"" << fontname << "\"";
		return "";
	}
	FcDefaultSubstitute(pattern);
	FcResult result;
	FcPattern * fontMatch=NULL;
	fontMatch = FcFontMatch(0,pattern,&result);

	if(!fontMatch){
		ofLogError() << "linuxFontPathByName(): couldn't match font file or system font with name \"" << fontname << "\"";
		return "";
	}
	FcChar8	*file;
	if (FcPatternGetString (fontMatch, FC_FILE, 0, &file) == FcResultMatch){
		filename = (const char*)file;
	}else{
		ofLogError() << "linuxFontPathByName(): couldn't find font match for \"" << fontname << "\"";
		return "";
	}
	return filename;
}
#endif

bool ofTrueTypeFont::initLibraries(){
	if(!librariesInitialized){
	    FT_Error err;
	    err = FT_Init_FreeType( &library );

	    if (err){
			ofLogError("ofTrueTypeFont") << "loadFont(): couldn't initialize Freetype lib: FT_Error " << err;
			return false;
		}
#ifdef TARGET_LINUX
		FcBool result = FcInit();
		if(!result){
			return false;
		}
#endif
#ifdef TARGET_WIN32
		initWindows();
#endif
		librariesInitialized = true;
	}
    return true;
}

void ofTrueTypeFont::finishLibraries(){
	if(librariesInitialized){
#ifdef TARGET_LINUX
		//FcFini();
#endif
		FT_Done_FreeType(library);
	}
}


//------------------------------------------------------------------
ofTrueTypeFont::ofTrueTypeFont(){
	bLoadedOk		= false;
	bMakeContours	= false;
	#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
		all_fonts().insert(this);
	#endif
	//cps				= NULL;
	letterSpacing = 1;
	spaceSize = 1;

	// 3 pixel border around the glyph
	// We show 2 pixels of this, so that blending looks good.
	// 1 pixels is hidden because we don't want to see the real edge of the texture

	border			= 3;
	//visibleBorder	= 2;
	stringQuads.setMode(OF_PRIMITIVE_TRIANGLES);
	binded = false;
}

//------------------------------------------------------------------
ofTrueTypeFont::~ofTrueTypeFont(){

	if (bLoadedOk){
		unloadTextures();
	}

	#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
		all_fonts().erase(this);
	#endif
}

void ofTrueTypeFont::unloadTextures(){
	if(!bLoadedOk) return;

	texAtlas.clear();
	bLoadedOk = false;
}

void ofTrueTypeFont::reloadTextures(){
	loadFont(filename, fontSize, bAntiAliased, bFullCharacterSet, bMakeContours, simplifyAmt, dpi);
}

static bool loadFontFace(string fontname, int _fontSize, FT_Face & face, string & filename){
	filename = ofToDataPath(fontname,true);
	ofFile fontFile(filename,ofFile::Reference);
	int fontID = 0;
	if(!fontFile.exists()){
#ifdef TARGET_LINUX
		filename = linuxFontPathByName(fontname);
#elif defined(TARGET_OSX)
		if(fontname==OF_TTF_SANS){
			fontname = "Helvetica Neue";
			fontID = 4;
		}else if(fontname==OF_TTF_SERIF){
			fontname = "Times New Roman";
		}else if(fontname==OF_TTF_MONO){
			fontname = "Menlo Regular";
		}
		filename = osxFontPathByName(fontname);
#elif defined(TARGET_WIN32)
		if(fontname==OF_TTF_SANS){
			fontname = "Arial";
		}else if(fontname==OF_TTF_SERIF){
			fontname = "Times New Roman";
		}else if(fontname==OF_TTF_MONO){
			fontname = "Courier New";
		}
        filename = winFontPathByName(fontname);
#endif
		if(filename == "" ){
			ofLogError("ofTrueTypeFont") << "loadFontFace(): couldn't find font \"" << fontname << "\"";
			return false;
		}
		ofLogVerbose("ofTrueTypeFont") << "loadFontFace(): \"" << fontname << "\" not a file in data loading system font from \"" << filename << "\"";
	}
	FT_Error err;
	err = FT_New_Face( library, filename.c_str(), fontID, &face );
	if (err) {
		// simple error table in lieu of full table (see fterrors.h)
		string errorString = "unknown freetype";
		if(err == 1) errorString = "INVALID FILENAME";
		ofLogError("ofTrueTypeFont") << "loadFontFace(): couldn't create new face for \"" << fontname << "\": FT_Error " << err << " " << errorString;
		return false;
	}

	return true;
}

//-----------------------------------------------------------
bool ofTrueTypeFont::loadFont(string _filename, int _fontSize, bool _bAntiAliased, bool _bFullCharacterSet, bool _makeContours, float _simplifyAmt, int _dpi) {

	initLibraries();

	//------------------------------------------------
	if (bLoadedOk == true){

		// we've already been loaded, try to clean up :
		unloadTextures();
	}
	//------------------------------------------------

	if( _dpi == 0 ){
		_dpi = ttfGlobalDpi;
	}



	bLoadedOk 			= false;
	bAntiAliased 		= _bAntiAliased;
	bFullCharacterSet 	= _bFullCharacterSet;
	fontSize			= _fontSize;
	bMakeContours 		= _makeContours;
	simplifyAmt			= _simplifyAmt;
	dpi 				= _dpi;

	//--------------- load the library and typeface


	FT_Face face;
	if(!loadFontFace(_filename,_fontSize,face,filename)){
        return false;
	}


	FT_Set_Char_Size( face, fontSize << 6, fontSize << 6, dpi, dpi);
	lineHeight = fontSize * 1.43f;

	//------------------------------------------------------
	//kerning would be great to support:
	//ofLogNotice("ofTrueTypeFont") << "FT_HAS_KERNING ? " <<  FT_HAS_KERNING(face);
	//------------------------------------------------------

	nCharacters = (bFullCharacterSet ? 256 : 128) - NUM_CHARACTER_TO_START;

	//--------------- initialize character info and textures
	cps.resize(nCharacters);

	if(bMakeContours){
		charOutlines.clear();
		charOutlines.assign(nCharacters, ofTTFCharacter());
		charOutlinesNonVFlipped.assign(nCharacters, ofTTFCharacter());
	}

	vector<ofPixels> expanded_data(nCharacters);

	long areaSum=0;
	FT_Error err;


	//--------------------- load each char -----------------------
	for (int i = 0 ; i < nCharacters; i++){

		//------------------------------------------ anti aliased or not:
		int glyph = (unsigned char)(i+NUM_CHARACTER_TO_START);
		if (glyph == 0xA4) glyph = 0x20AC; // hack to load the euro sign, all codes in 8859-15 match with utf-32 except for this one
		err = FT_Load_Glyph( face, FT_Get_Char_Index( face, glyph ), FT_LOAD_DEFAULT );
        if(err){
			ofLogError("ofTrueTypeFont") << "loadFont(): FT_Load_Glyph failed for char " << i << ": FT_Error " << err;

		}

		if (bAntiAliased == true) FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
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
			if(printVectorInfo){
				ofLogNotice("ofTrueTypeFont") <<  "character " << char(i+NUM_CHARACTER_TO_START);
			}

			//int character = i + NUM_CHARACTER_TO_START;
			charOutlines[i] = makeContoursForCharacter( face );

			charOutlinesNonVFlipped[i] = charOutlines[i];
			charOutlinesNonVFlipped[i].translate(ofVec3f(0,cps[i].height));
			charOutlinesNonVFlipped[i].scale(1,-1);


			if(simplifyAmt>0)
				charOutlines[i].simplify(simplifyAmt);
			charOutlines[i].getTessellation();

			if(simplifyAmt>0)
				charOutlinesNonVFlipped[i].simplify(simplifyAmt);
			charOutlinesNonVFlipped[i].getTessellation();
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


		if (bAntiAliased == true){
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
	//ofLogNotice("ofTrueTypeFont") << "loadFont(): areaSum: " << areaSum

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



	ofPixels atlasPixelsLuminanceAlpha;
	atlasPixelsLuminanceAlpha.allocate(w,h,2);
	atlasPixelsLuminanceAlpha.set(0,255);
	atlasPixelsLuminanceAlpha.set(1,0);


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
		charPixels.pasteInto(atlasPixelsLuminanceAlpha,x+border,y+border);
		x+= sortedCopy[i].tW + border*2;
	}

	ofPixels atlasPixels;
	atlasPixels.allocate(atlasPixelsLuminanceAlpha.getWidth(),atlasPixelsLuminanceAlpha.getHeight(),4);
	atlasPixels.setChannel(0,atlasPixelsLuminanceAlpha.getChannel(0));
	atlasPixels.setChannel(1,atlasPixelsLuminanceAlpha.getChannel(0));
	atlasPixels.setChannel(2,atlasPixelsLuminanceAlpha.getChannel(0));
	atlasPixels.setChannel(3,atlasPixelsLuminanceAlpha.getChannel(1));
	texAtlas.allocate(atlasPixels,false);

	if(bAntiAliased && fontSize>20){
		texAtlas.setTextureMinMagFilter(GL_LINEAR,GL_LINEAR);
	}else{
		texAtlas.setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
	}
	texAtlas.loadData(atlasPixels);

	// ------------- close the library and typeface
	FT_Done_Face(face);
  	bLoadedOk = true;
	return true;
}

ofTextEncoding ofTrueTypeFont::getEncoding() const {
	return encoding;
}

void ofTrueTypeFont::setEncoding(ofTextEncoding _encoding) {
	encoding = _encoding;
}

//-----------------------------------------------------------
bool ofTrueTypeFont::isLoaded() {
	return bLoadedOk;
}

//-----------------------------------------------------------
bool ofTrueTypeFont::isAntiAliased() {
	return bAntiAliased;
}

//-----------------------------------------------------------
bool ofTrueTypeFont::hasFullCharacterSet() {
	return bFullCharacterSet;
}

//-----------------------------------------------------------
int ofTrueTypeFont::getSize() {
	return fontSize;
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
ofTTFCharacter ofTrueTypeFont::getCharacterAsPoints(int character, bool vflip){
	if( bMakeContours == false ){
		ofLogError("ofxTrueTypeFont") << "getCharacterAsPoints(): contours not created, call loadFont() with makeContours set to true";
		return ofTTFCharacter();
	}
    if (character - NUM_CHARACTER_TO_START >= nCharacters || character < NUM_CHARACTER_TO_START){
        ofLogError("ofxTrueTypeFont") << "getCharacterAsPoint(): char " << character + NUM_CHARACTER_TO_START
		<< " not allocated: line " << __LINE__ << " in " << __FILE__;
        
        return ofTTFCharacter();
    }
    
    if(vflip){
    	return charOutlines[character - NUM_CHARACTER_TO_START];
    }else{
    	return charOutlinesNonVFlipped[character - NUM_CHARACTER_TO_START];
    }
}

//-----------------------------------------------------------
void ofTrueTypeFont::drawChar(int c, float x, float y) {

	if (c >= nCharacters){
		//ofLogError("ofTrueTypeFont") << "drawChar(): char " << c + NUM_CHARACTER_TO_START << " not allocated: line " << __LINE__ << " in " << __FILE__;
		return;
	}

	GLfloat	x1, y1, x2, y2;
	GLfloat t1, v1, t2, v2;
	t2		= cps[c].t2;
	v2		= cps[c].v2;
	t1		= cps[c].t1;
	v1		= cps[c].v1;

	x1		= cps[c].x1+x;
	y1		= cps[c].y1;
	x2		= cps[c].x2+x;
	y2		= cps[c].y2;

	int firstIndex = stringQuads.getVertices().size();

	if(!ofIsVFlipped()){
       y1 *= -1;
       y2 *= -1;  
	}

    y1 += y;
    y2 += y; 

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
vector<ofTTFCharacter> ofTrueTypeFont::getStringAsPoints(string str, bool vflip){
	if(bFullCharacterSet && encoding==OF_ENCODING_UTF8){
		string o;
		Poco::TextConverter(Poco::UTF8Encoding(),Poco::Latin9Encoding()).convert(str,o);
		str=o;
	}

	vector<ofTTFCharacter> shapes;

	if (!bLoadedOk){
		ofLogError("ofxTrueTypeFont") << "getStringAsPoints(): font not allocated: line " << __LINE__ << " in " << __FILE__;
		return shapes;
	};

	GLint		index	= 0;
	GLfloat		X		= 0;
	GLfloat		Y		= 0;
	int newLineDirection		= 1;

	if(vflip){
		// this would align multiline texts to the last line when vflip is disabled
		//int lines = ofStringTimesInString(c,"\n");
		//Y = lines*lineHeight;
		newLineDirection = -1;
	}


	int len = (int)str.length();

	while(index < len){
		int cy = (unsigned char)str[index] - NUM_CHARACTER_TO_START;
		if (cy < nCharacters){ 			// full char set or not?
			if (str[index] == '\n') {
				Y += lineHeight*newLineDirection;
				X = 0 ; //reset X Pos back to zero
			}else if (str[index] == ' ') {
				int cy = (int)'p' - NUM_CHARACTER_TO_START;
				X += cps[cy].setWidth * letterSpacing * spaceSize;
			} else if(cy > -1){
				shapes.push_back(getCharacterAsPoints((unsigned char)str[index],vflip));
				shapes.back().translate(ofPoint(X,Y));

				X += cps[cy].setWidth * letterSpacing;
			}
		}
		index++;
	}
	return shapes;

}

//-----------------------------------------------------------
void ofTrueTypeFont::drawCharAsShape(int c, float x, float y) {
	if (c - NUM_CHARACTER_TO_START >= nCharacters || c < NUM_CHARACTER_TO_START){
		//ofLogError("ofTrueTypeFont") << "drawCharAsShape(): char " << << c + NUM_CHARACTER_TO_START << " not allocated: line " << __LINE__ << " in " << __FILE__;
		return;
	}
	//-----------------------

	if(ofIsVFlipped()){
		ofTTFCharacter & charRef = charOutlines[c - NUM_CHARACTER_TO_START];
		charRef.setFilled(ofGetStyle().bFill);
		charRef.draw(x,y);
	}else{
		ofTTFCharacter & charRef = charOutlinesNonVFlipped[c - NUM_CHARACTER_TO_START];
		charRef.setFilled(ofGetStyle().bFill);
		charRef.draw(x,y);
	}
}

//-----------------------------------------------------------
float ofTrueTypeFont::stringWidth(string c) {
    ofRectangle rect = getStringBoundingBox(c, 0,0);
    return rect.width;
}


ofRectangle ofTrueTypeFont::getStringBoundingBox(string c, float x, float y){

    ofRectangle myRect;

    if (!bLoadedOk){
    	ofLogError("ofTrueTypeFont") << "getStringBoundingBox(): font not allocated";
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
				yoffset += lineHeight;
				xoffset = 0 ; //reset X Pos back to zero
	      } else if (c[index] == ' ') {
	     		int cy = (int)'p' - NUM_CHARACTER_TO_START;
				 xoffset += cps[cy].setWidth * letterSpacing * spaceSize;
				 // zach - this is a bug to fix -- for now, we don't currently deal with ' ' in calculating string bounding box
		  } else if(cy > -1){
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

void ofTrueTypeFont::createStringMesh(string c, float x, float y){
	GLint		index	= 0;
	GLfloat		X		= x;
	GLfloat		Y		= y;
	int newLineDirection		= 1;

	if(!ofIsVFlipped()){
		// this would align multiline texts to the last line when vflip is disabled
		//int lines = ofStringTimesInString(c,"\n");
		//Y = lines*lineHeight;
		newLineDirection = -1;
	}

	int len = (int)c.length();

	while(index < len){
		int cy = (unsigned char)c[index] - NUM_CHARACTER_TO_START;
		if (cy < nCharacters){ 			// full char set or not?
		  if (c[index] == '\n') {

				Y += lineHeight*newLineDirection;
				X = x ; //reset X Pos back to zero

		  }else if (c[index] == ' ') {
				 int cy = (int)'p' - NUM_CHARACTER_TO_START;
				 X += cps[cy].setWidth * letterSpacing * spaceSize;
		  } else if(cy > -1){
				drawChar(cy, X, Y);
				X += cps[cy].setWidth * letterSpacing;
		  }
		}
		index++;
	}
}

ofMesh & ofTrueTypeFont::getStringMesh(string c, float x, float y){
	stringQuads.clear();
	createStringMesh(c,x,y);
	return stringQuads;
}

ofTexture & ofTrueTypeFont::getFontTexture(){
	return texAtlas;
}

//=====================================================================
void ofTrueTypeFont::drawString(string c, float x, float y) {
	
	/*glEnable(GL_BLEND);
	 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	 texAtlas.draw(0,0);*/
	
	if(bFullCharacterSet && encoding==OF_ENCODING_UTF8){
		string o;
		Poco::TextConverter(Poco::UTF8Encoding(),Poco::Latin9Encoding()).convert(c,o);
		c=o;
	}
	
	if (!bLoadedOk){
		ofLogError("ofTrueTypeFont") << "drawString(): font not allocated: line " << __LINE__ << " in " << __FILE__;
		return;
	};
	
	bool alreadyBinded = binded;

	if(!alreadyBinded) bind();
	createStringMesh(c,x,y);
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

		blend_enabled = glIsEnabled(GL_BLEND);
		texture_2d_enabled = glIsEnabled(GL_TEXTURE_2D);
		glGetIntegerv( GL_BLEND_SRC, &blend_src );
		glGetIntegerv( GL_BLEND_DST, &blend_dst );

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

		if( !blend_enabled )
			glDisable(GL_BLEND);
		if( !texture_2d_enabled )
			glDisable(GL_TEXTURE_2D);
		glBlendFunc( blend_src, blend_dst );

		binded = false;
	}
}

//=====================================================================
void ofTrueTypeFont::drawStringAsShapes(string c, float x, float y) {

    if (!bLoadedOk){
    	ofLogError("ofTrueTypeFont") << "drawStringAsShapes(): font not allocated: line " << __LINE__ << " in " << __FILE__;
    	return;
    };

	//----------------------- error checking
	if (!bMakeContours){
		ofLogError("ofTrueTypeFont") << "drawStringAsShapes(): contours not created for this font, call loadFont() with makeContours set to true";
		return;
	}

	if(bFullCharacterSet && encoding==OF_ENCODING_UTF8){
		string o;
		Poco::TextConverter(Poco::UTF8Encoding(),Poco::Latin9Encoding()).convert(c,o);
		c=o;
	}

	GLint		index	= 0;
	GLfloat		X		= x;
	GLfloat		Y		= y;
	int newLineDirection		= 1;

	if(!ofIsVFlipped()){
		// this would align multiline texts to the last line when vflip is disabled
		//int lines = ofStringTimesInString(c,"\n");
		//Y = lines*lineHeight;
		newLineDirection = -1;
	}

	int len = (int)c.length();

	while(index < len){
		int cy = (unsigned char)c[index] - NUM_CHARACTER_TO_START;
		if (cy < nCharacters){ 			// full char set or not?
		  if (c[index] == '\n') {

				Y += lineHeight*newLineDirection;
				X = x ; //reset X Pos back to zero

		  }else if (c[index] == ' ') {
				 int cy = (int)'p' - NUM_CHARACTER_TO_START;
				 X += cps[cy].setWidth * letterSpacing * spaceSize;
				 //glTranslated(cps[cy].width, 0, 0);
		  } else if(cy > -1){
				drawCharAsShape((unsigned char)c[index], X, Y);
				X += cps[cy].setWidth * letterSpacing;
				//glTranslated(cps[cy].setWidth, 0, 0);
		  }
		}
		index++;
	}

}

//-----------------------------------------------------------
int ofTrueTypeFont::getNumCharacters() {
	return nCharacters;
}
