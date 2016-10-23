#include "ofTrueTypeFont.h"
//--------------------------

#include <ft2build.h>

#ifdef TARGET_LINUX
#include <fontconfig/fontconfig.h>
#endif

#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_TRIGONOMETRY_H

#include <algorithm>

#include "ofUtils.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"

static bool printVectorInfo = false;
static int ttfGlobalDpi = 96;
static bool librariesInitialized = false;
static FT_Library library;

//--------------------------------------------------------
void ofTrueTypeShutdown(){
#ifdef TARGET_LINUX
	// This crashes if gtk was called at any time cause pango leaks
	// its fc related objects: https://github.com/openframeworks/openFrameworks/issues/5061
	//FcFini();
#endif
}

//--------------------------------------------------------
void ofTrueTypeFont::setGlobalDpi(int newDpi){
	ttfGlobalDpi = newDpi;
}

//--------------------------------------------------------
static ofTTFCharacter makeContoursForCharacter(FT_Face face){

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

#if defined(TARGET_ANDROID)
#include "ofxAndroidUtils.h"
#endif

//------------------------------------------------------------------
bool compare_cps(const charProps & c1, const charProps & c2){
	if(c1.tH == c2.tH) return c1.tW > c2.tW;
	else return c1.tH > c2.tH;
}


#ifdef TARGET_OSX
//------------------------------------------------------------------
static std::string osxFontPathByName(const std::string& fontname){
	CFStringRef targetName = CFStringCreateWithCString(nullptr, fontname.c_str(), kCFStringEncodingUTF8);
	CTFontDescriptorRef targetDescriptor = CTFontDescriptorCreateWithNameAndSize(targetName, 0.0);
	CFURLRef targetURL = (CFURLRef) CTFontDescriptorCopyAttribute(targetDescriptor, kCTFontURLAttribute);
	string fontPath = "";
	
	if(targetURL) {
		UInt8 buffer[PATH_MAX];
		CFURLGetFileSystemRepresentation(targetURL, true, buffer, PATH_MAX);
		fontPath = std::string((char *)buffer);
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
static map<std::string, std::string> fonts_table;
// read font linking information from registry, and store in std::map
//------------------------------------------------------------------
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

	l_ret = RegQueryInfoKeyW(key_ft, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &value_count, nullptr, &max_data_len, nullptr, nullptr);
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
	if(value_data == nullptr) return;

	char value_name_char[2048];
	char value_data_char[2048];
	/*char ppidl[2048];
	char fontsPath[2048];
    SHGetKnownFolderIDList(FOLDERID_Fonts, 0, nullptr, &ppidl);
    SHGetPathFromIDList(ppidl,&fontsPath);*/
	std::string fontsDir = getenv ("windir");
    fontsDir += "\\Fonts\\";
	for (DWORD i = 0; i < value_count; ++i)
	{
			DWORD name_len = 2048;
			DWORD data_len = max_data_len;

			l_ret = RegEnumValueW(key_ft, i, value_name, &name_len, nullptr, nullptr, value_data, &data_len);
			if(l_ret != ERROR_SUCCESS){
			     ofLogError("ofTrueTypeFont") << "initWindows(): couldn't read registry key for font type";
			     continue;
			}

            wcstombs(value_name_char,value_name,2048);
			wcstombs(value_data_char,reinterpret_cast<wchar_t *>(value_data),2048);
			std::string curr_face = value_name_char;
			std::string font_file = value_data_char;
			curr_face = curr_face.substr(0, curr_face.find('(') - 1);
			fonts_table[curr_face] = fontsDir + font_file;
	}


	HeapFree(GetProcessHeap(), 0, value_data);

	l_ret = RegCloseKey(key_ft);
}


static std::string winFontPathByName(const std::string& fontname ){
    if(fonts_table.find(fontname)!=fonts_table.end()){
        return fonts_table[fontname];
    }
    for(map<std::string,std::string>::iterator it = fonts_table.begin(); it!=fonts_table.end(); it++){
        if(ofIsStringInString(ofToLower(it->first),ofToLower(fontname))) return it->second;
    }
    return "";
}
#endif

#ifdef TARGET_LINUX
//------------------------------------------------------------------
static std::string linuxFontPathByName(const std::string& fontname){
	std::string filename;
	FcPattern * pattern = FcNameParse((const FcChar8*)fontname.c_str());
	FcBool ret = FcConfigSubstitute(0,pattern,FcMatchPattern);
	if(!ret){
		ofLogError() << "linuxFontPathByName(): couldn't find font file or system font with name \"" << fontname << "\"";
		return "";
	}
	FcDefaultSubstitute(pattern);
	FcResult result;
	FcPattern * fontMatch=nullptr;
	fontMatch = FcFontMatch(0,pattern,&result);

	if(!fontMatch){
		ofLogError() << "linuxFontPathByName(): couldn't match font file or system font with name \"" << fontname << "\"";
		FcPatternDestroy(fontMatch);
		FcPatternDestroy(pattern);
		return "";
	}
	FcChar8	*file;
	if (FcPatternGetString (fontMatch, FC_FILE, 0, &file) == FcResultMatch){
		filename = (const char*)file;
	}else{
		ofLogError() << "linuxFontPathByName(): couldn't find font match for \"" << fontname << "\"";
		FcPatternDestroy(fontMatch);
		FcPatternDestroy(pattern);
		return "";
	}
	FcPatternDestroy(fontMatch);
	FcPatternDestroy(pattern);
	return filename;
}
#endif

//------------------------------------------------------------------
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


//------------------------------------------------------------------
ofTrueTypeFont::ofTrueTypeFont(){
	bLoadedOk		= false;
	bMakeContours	= false;
	letterSpacing = 1;
    spaceSize = 0;

    stringQuads.setMode(OF_PRIMITIVE_TRIANGLES);
	ascenderHeight = 0;
	bAntiAliased = 0;
	bFullCharacterSet = 0;
	descenderHeight = 0;
	dpi = 96;
	fontSize = 0;
	lineHeight = 0;
	nCharacters = 0;
	simplifyAmt = 0;
	useKerning = false;
}

//------------------------------------------------------------------
ofTrueTypeFont::~ofTrueTypeFont(){
#if defined(TARGET_ANDROID)
	ofRemoveListener(ofxAndroidEvents().unloadGL,this,&ofTrueTypeFont::unloadTextures);
	ofRemoveListener(ofxAndroidEvents().reloadGL,this,&ofTrueTypeFont::reloadTextures);
#endif
}

//------------------------------------------------------------------
ofTrueTypeFont::ofTrueTypeFont(const ofTrueTypeFont& mom){
#if defined(TARGET_ANDROID)
    if(mom.isLoaded()){
        ofAddListener(ofxAndroidEvents().unloadGL,this,&ofTrueTypeFont::unloadTextures);
        ofAddListener(ofxAndroidEvents().reloadGL,this,&ofTrueTypeFont::reloadTextures);
    }
#endif
    bLoadedOk = mom.bLoadedOk;
    bAntiAliased = mom.bAntiAliased;
    bFullCharacterSet = mom.bFullCharacterSet;
    nCharacters = mom.nCharacters;

    charOutlines = mom.charOutlines;
    charOutlinesNonVFlipped = mom.charOutlinesNonVFlipped;
    charOutlinesContour = mom.charOutlinesContour;
    charOutlinesNonVFlippedContour = mom.charOutlinesNonVFlippedContour;

    lineHeight = mom.lineHeight;
    ascenderHeight = mom.ascenderHeight;
    descenderHeight = mom.descenderHeight;
    glyphBBox = mom.glyphBBox;
    letterSpacing = mom.letterSpacing;
    spaceSize = mom.spaceSize;

    cps = mom.cps; // properties for each character

    fontSize = mom.fontSize;
    bMakeContours = mom.bMakeContours;
    simplifyAmt = mom.simplifyAmt;
    dpi = mom.dpi;

    filename = mom.filename;

    texAtlas = mom.texAtlas;
    useKerning = mom.useKerning;
    face = mom.face;
}

//------------------------------------------------------------------
ofTrueTypeFont & ofTrueTypeFont::operator=(const ofTrueTypeFont& mom){
    if(this == &mom) return *this;
#if defined(TARGET_ANDROID)
    if(mom.isLoaded()){
        ofAddListener(ofxAndroidEvents().unloadGL,this,&ofTrueTypeFont::unloadTextures);
        ofAddListener(ofxAndroidEvents().reloadGL,this,&ofTrueTypeFont::reloadTextures);
    }
#endif
    bLoadedOk = mom.bLoadedOk;
    bAntiAliased = mom.bAntiAliased;
    bFullCharacterSet = mom.bFullCharacterSet;
    nCharacters = mom.nCharacters;

    charOutlines = mom.charOutlines;
    charOutlinesNonVFlipped = mom.charOutlinesNonVFlipped;
    charOutlinesContour = mom.charOutlinesContour;
    charOutlinesNonVFlippedContour = mom.charOutlinesNonVFlippedContour;

    lineHeight = mom.lineHeight;
    ascenderHeight = mom.ascenderHeight;
    descenderHeight = mom.descenderHeight;
    glyphBBox = mom.glyphBBox;
    letterSpacing = mom.letterSpacing;
    spaceSize = mom.spaceSize;

    cps = mom.cps; // properties for each character

    fontSize = mom.fontSize;
    bMakeContours = mom.bMakeContours;
    simplifyAmt = mom.simplifyAmt;
    dpi = mom.dpi;

    filename = mom.filename;

    texAtlas = mom.texAtlas;
    useKerning = mom.useKerning;
    face = mom.face;
    return *this;
}

//------------------------------------------------------------------
ofTrueTypeFont::ofTrueTypeFont(ofTrueTypeFont&& mom){
#if defined(TARGET_ANDROID)
    if(mom.isLoaded()){
        ofAddListener(ofxAndroidEvents().unloadGL,this,&ofTrueTypeFont::unloadTextures);
        ofAddListener(ofxAndroidEvents().reloadGL,this,&ofTrueTypeFont::reloadTextures);
    }
#endif
    bLoadedOk = mom.bLoadedOk;
    bAntiAliased = mom.bAntiAliased;
    bFullCharacterSet = mom.bFullCharacterSet;
    nCharacters = mom.nCharacters;

    charOutlines = std::move(mom.charOutlines);
    charOutlinesNonVFlipped = std::move(mom.charOutlinesNonVFlipped);
    charOutlinesContour = std::move(mom.charOutlinesContour);
    charOutlinesNonVFlippedContour = std::move(mom.charOutlinesNonVFlippedContour);

    lineHeight = mom.lineHeight;
    ascenderHeight = mom.ascenderHeight;
    descenderHeight = mom.descenderHeight;
    glyphBBox = mom.glyphBBox;
    letterSpacing = mom.letterSpacing;
    spaceSize = mom.spaceSize;

    cps = mom.cps; // properties for each character

    fontSize = mom.fontSize;
    bMakeContours = mom.bMakeContours;
    simplifyAmt = mom.simplifyAmt;
    dpi = mom.dpi;

    filename = mom.filename;

    texAtlas = mom.texAtlas;
    useKerning = mom.useKerning;
    face = mom.face;
}

//------------------------------------------------------------------
ofTrueTypeFont & ofTrueTypeFont::operator=(ofTrueTypeFont&& mom){
    if(this == &mom) return *this;
#if defined(TARGET_ANDROID)
    if(mom.isLoaded()){
        ofAddListener(ofxAndroidEvents().unloadGL,this,&ofTrueTypeFont::unloadTextures);
        ofAddListener(ofxAndroidEvents().reloadGL,this,&ofTrueTypeFont::reloadTextures);
    }
#endif
    bLoadedOk = mom.bLoadedOk;
    bAntiAliased = mom.bAntiAliased;
    bFullCharacterSet = mom.bFullCharacterSet;
    nCharacters = mom.nCharacters;

    charOutlines = std::move(mom.charOutlines);
    charOutlinesNonVFlipped = std::move(mom.charOutlinesNonVFlipped);
    charOutlinesContour = std::move(mom.charOutlinesContour);
    charOutlinesNonVFlippedContour = std::move(mom.charOutlinesNonVFlippedContour);

    lineHeight = mom.lineHeight;
    ascenderHeight = mom.ascenderHeight;
    descenderHeight = mom.descenderHeight;
    glyphBBox = mom.glyphBBox;
    letterSpacing = mom.letterSpacing;
    spaceSize = mom.spaceSize;

    cps = mom.cps; // properties for each character

    fontSize = mom.fontSize;
    bMakeContours = mom.bMakeContours;
    simplifyAmt = mom.simplifyAmt;
    dpi = mom.dpi;

    filename = mom.filename;

    texAtlas = mom.texAtlas;
    useKerning = mom.useKerning;
    face = mom.face;
    return *this;
}

//------------------------------------------------------------------
void ofTrueTypeFont::unloadTextures(){
	if(!bLoadedOk) return;
	texAtlas.clear();
}

//------------------------------------------------------------------
void ofTrueTypeFont::reloadTextures(){
	if(bLoadedOk){
		load(filename, fontSize, bAntiAliased, bFullCharacterSet, bMakeContours, simplifyAmt, dpi);
	}
}

//------------------------------------------------------------------
static std::shared_ptr<FT_FaceRec_> loadFontFace(const std::string& _fontname, int _fontSize, std::string& filename){
    FT_Face face;
	std::string fontname = _fontname;
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
            return nullptr;
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
        return nullptr;
	}

    return std::shared_ptr<FT_FaceRec_>(face, FT_Done_Face);
}

//-----------------------------------------------------------
bool ofTrueTypeFont::loadFont(string _filename, int _fontSize, bool _bAntiAliased, bool _bFullCharacterSet, bool _makeContours, float _simplifyAmt, int _dpi) {
	return load(_filename, _fontSize, _bAntiAliased, _bFullCharacterSet, _makeContours, _simplifyAmt, _dpi);
}

//-----------------------------------------------------------
bool ofTrueTypeFont::load(const std::string& _filename, int _fontSize, bool _bAntiAliased, bool _bFullCharacterSet, bool _makeContours, float _simplifyAmt, int _dpi) {
	#if defined(TARGET_ANDROID)
	ofAddListener(ofxAndroidEvents().unloadGL,this,&ofTrueTypeFont::unloadTextures);
	ofAddListener(ofxAndroidEvents().reloadGL,this,&ofTrueTypeFont::reloadTextures);
	#endif
	int border = 1;
	initLibraries();

	// if we've already been loaded, try to clean up :
	unloadTextures();

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


    face = loadFontFace(_filename,_fontSize,filename);
    if(!face){
        return false;
	}


    FT_Set_Char_Size( face.get(), fontSize << 6, fontSize << 6, dpi, dpi);
	float fontUnitScale = ((float)fontSize * dpi) / (72 * face->units_per_EM);
	lineHeight = face->height * fontUnitScale;
	ascenderHeight = face->ascender * fontUnitScale;
	descenderHeight = face->descender * fontUnitScale;
	glyphBBox.set(face->bbox.xMin * fontUnitScale,
				  face->bbox.yMin * fontUnitScale,
				  (face->bbox.xMax - face->bbox.xMin) * fontUnitScale,
				  (face->bbox.yMax - face->bbox.yMin) * fontUnitScale);
	useKerning = FT_HAS_KERNING( face );

	//------------------------------------------------------
	//kerning would be great to support:
	//ofLogNotice("ofTrueTypeFont") << "FT_HAS_KERNING ? " <<  FT_HAS_KERNING(face);
	//------------------------------------------------------

	nCharacters = (bFullCharacterSet ? 256 : 128) - NUM_CHARACTER_TO_START;

	//--------------- initialize character info and textures
	cps.resize(nCharacters);

	if(bMakeContours){
		charOutlines.assign(nCharacters, ofTTFCharacter());
		charOutlinesNonVFlipped.assign(nCharacters, ofTTFCharacter());
		charOutlinesContour.assign(nCharacters, ofTTFCharacter());
		charOutlinesNonVFlippedContour.assign(nCharacters, ofTTFCharacter());
	}else{
		charOutlines.resize(1);
	}

	vector<ofPixels> expanded_data(nCharacters);

	long areaSum=0;
	FT_Error err;


	//--------------------- load each char -----------------------
	for (int i = 0 ; i < nCharacters; i++){

		//------------------------------------------ anti aliased or not:
		int glyph = (unsigned char)(i+NUM_CHARACTER_TO_START);
		if (glyph == 0xA4) glyph = 0x20AC; // hack to load the euro sign, all codes in 8859-15 match with utf-32 except for this one
        err = FT_Load_Glyph( face.get(), FT_Get_Char_Index( face.get(), glyph ), bAntiAliased ?  FT_LOAD_FORCE_AUTOHINT : FT_LOAD_DEFAULT );
        if(err){
			ofLogError("ofTrueTypeFont") << "loadFont(): FT_Load_Glyph failed for char " << i << ": FT_Error " << err;

		}

		if (bAntiAliased == true) FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		else FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO);

		//------------------------------------------


		if(bMakeContours){
			if(printVectorInfo){
				ofLogNotice("ofTrueTypeFont") <<  "character " << char(i+NUM_CHARACTER_TO_START);
			}

			//int character = i + NUM_CHARACTER_TO_START;
            charOutlines[i] = makeContoursForCharacter( face.get() );
			charOutlinesContour[i] = charOutlines[i];
			charOutlinesContour[i].setFilled(false);
			charOutlinesContour[i].setStrokeWidth(1);

			charOutlinesNonVFlipped[i] = charOutlines[i];
			charOutlinesNonVFlipped[i].translate(ofVec3f(0,cps[i].height));
			charOutlinesNonVFlipped[i].scale(1,-1);
			charOutlinesNonVFlippedContour[i] = charOutlines[i];
			charOutlinesNonVFlippedContour[i].setFilled(false);
			charOutlinesNonVFlippedContour[i].setStrokeWidth(1);


			if(simplifyAmt>0){
				charOutlines[i].simplify(simplifyAmt);
				charOutlinesNonVFlipped[i].simplify(simplifyAmt);
				charOutlinesContour[i].simplify(simplifyAmt);
				charOutlinesNonVFlippedContour[i].simplify(simplifyAmt);
			}
		}


		// -------------------------
		// info about the character:
		FT_Bitmap& bitmap= face->glyph->bitmap;

		// Note: Using decltype here to avoid warnings across
		// platforms using differing versions of freetype 2.
		decltype(bitmap.width) width  = bitmap.width;
		decltype(bitmap.rows) height = bitmap.rows;

		cps[i].characterIndex	= i;
		cps[i].glyph			= glyph;
		cps[i].height 			= face->glyph->metrics.height>>6;
		cps[i].width 			= face->glyph->metrics.width>>6;
		cps[i].bearingX			= face->glyph->metrics.horiBearingX>>6;
		cps[i].bearingY			= face->glyph->metrics.horiBearingY>>6;
		cps[i].xmin				= face->glyph->bitmap_left;
		cps[i].xmax				= cps[i].xmin + cps[i].width;
		cps[i].ymin				= -face->glyph->bitmap_top;
		cps[i].ymax				= cps[i].ymin + cps[i].height;
		cps[i].advance			= face->glyph->metrics.horiAdvance>>6;


		cps[i].tW				= cps[i].width;
		cps[i].tH				= cps[i].height;


		areaSum += (cps[i].tW+border*2)*(cps[i].tH+border*2);

		if(width==0 || height==0) continue;

		// Allocate Memory For The Texture Data.
		expanded_data[i].allocate(width, height, OF_PIXELS_GRAY_ALPHA);
		//-------------------------------- clear data:
		expanded_data[i].set(0,255); // every luminance pixel = 255
		expanded_data[i].set(1,0);


		if (bAntiAliased == true){
			ofPixels bitmapPixels;
			bitmapPixels.setFromExternalPixels(bitmap.buffer,width,height,OF_PIXELS_GRAY);
			expanded_data[i].setChannel(1,bitmapPixels);
		} else {
			//-----------------------------------
			// true type packs monochrome info in a
			// 1-bit format, hella funky
			// here we unpack it:
			unsigned char *src =  bitmap.buffer;
			for(decltype(height) j=0; j < height; j++) {
				unsigned char b=0;
				unsigned char *bptr =  src;
				for(decltype(width) k=0; k < width; k++){
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
	atlasPixelsLuminanceAlpha.allocate(w,h,OF_PIXELS_GRAY_ALPHA);
	atlasPixelsLuminanceAlpha.set(0,255);
	atlasPixelsLuminanceAlpha.set(1,0);


	int x=0;
	int y=0;
	int maxRowHeight = sortedCopy[0].tH + border*2;
	for(int i=0;i<(int)cps.size();i++){
		ofPixels & charPixels = expanded_data[sortedCopy[i].characterIndex];

		if(x+sortedCopy[i].tW + border*2>w){
			x = 0;
			y += maxRowHeight;
			maxRowHeight = sortedCopy[i].tH + border*2;
		}

		cps[sortedCopy[i].characterIndex].t1		= float(x + border)/float(w);
		cps[sortedCopy[i].characterIndex].v1		= float(y + border)/float(h);
		cps[sortedCopy[i].characterIndex].t2		= float(cps[sortedCopy[i].characterIndex].tW + x + border)/float(w);
		cps[sortedCopy[i].characterIndex].v2		= float(cps[sortedCopy[i].characterIndex].tH + y + border)/float(h);
		charPixels.pasteInto(atlasPixelsLuminanceAlpha,x+border,y+border);
		x+= sortedCopy[i].tW + border*2;
	}
	texAtlas.allocate(atlasPixelsLuminanceAlpha,false);

	if(bAntiAliased && fontSize>20){
		texAtlas.setTextureMinMagFilter(GL_LINEAR,GL_LINEAR);
	}else{
		texAtlas.setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
	}
	texAtlas.loadData(atlasPixelsLuminanceAlpha);

	// ------------- close the library and typeface
  	bLoadedOk = true;
	return true;
}

//-----------------------------------------------------------
bool ofTrueTypeFont::isLoaded() const{
	return bLoadedOk;
}

//-----------------------------------------------------------
bool ofTrueTypeFont::isAntiAliased() const{
	return bAntiAliased;
}

//-----------------------------------------------------------
bool ofTrueTypeFont::hasFullCharacterSet() const{
	return bFullCharacterSet;
}

//-----------------------------------------------------------
int ofTrueTypeFont::getSize() const{
	return fontSize;
}

//-----------------------------------------------------------
void ofTrueTypeFont::setLineHeight(float _newLineHeight) {
	lineHeight = _newLineHeight;
}

//-----------------------------------------------------------
float ofTrueTypeFont::getLineHeight() const{
	return lineHeight;
}

//-----------------------------------------------------------
float ofTrueTypeFont::getAscenderHeight() const {
	return ascenderHeight;
}

//-----------------------------------------------------------
float ofTrueTypeFont::getDescenderHeight() const {
	return descenderHeight;
}

//-----------------------------------------------------------
const ofRectangle & ofTrueTypeFont::getGlyphBBox() const {
	return glyphBBox;
}

//-----------------------------------------------------------
void ofTrueTypeFont::setLetterSpacing(float _newletterSpacing) {
	letterSpacing = _newletterSpacing;
}

//-----------------------------------------------------------
float ofTrueTypeFont::getLetterSpacing() const{
	return letterSpacing;
}

//-----------------------------------------------------------
void ofTrueTypeFont::setSpaceSize(float _newspaceSize) {
	spaceSize = _newspaceSize;
}

//-----------------------------------------------------------
float ofTrueTypeFont::getSpaceSize() const{
	return spaceSize;
}

//------------------------------------------------------------------
ofTTFCharacter ofTrueTypeFont::getCharacterAsPoints(int character, bool vflip, bool filled) const{
	if( bMakeContours == false ){
		ofLogError("ofxTrueTypeFont") << "getCharacterAsPoints(): contours not created, call loadFont() with makeContours set to true";
		return ofTTFCharacter();
	}
    if (character - NUM_CHARACTER_TO_START >= nCharacters || character < NUM_CHARACTER_TO_START){
        //ofLogError("ofxTrueTypeFont") << "getCharacterAsPoint(): char " << character + NUM_CHARACTER_TO_START << " not allocated: line " << __LINE__ << " in " << __FILE__;

        return ofTTFCharacter();
    }
    
    if(vflip){
    	if(filled){
    		return charOutlines[character - NUM_CHARACTER_TO_START];
    	}else{
    		return charOutlinesContour[character - NUM_CHARACTER_TO_START];
    	}
    }else{
    	if(filled){
    		return charOutlinesNonVFlipped[character - NUM_CHARACTER_TO_START];
    	}else{
    		return charOutlinesNonVFlippedContour[character - NUM_CHARACTER_TO_START];
    	}
    }
}

//-----------------------------------------------------------
void ofTrueTypeFont::drawChar(int c, float x, float y, bool vFlipped) const{

	if (c >= nCharacters){
		//ofLogError("ofTrueTypeFont") << "drawChar(): char " << c + NUM_CHARACTER_TO_START << " not allocated: line " << __LINE__ << " in " << __FILE__;
		return;
	}


	int	xmin, ymin, xmax, ymax;
	float t1, v1, t2, v2;
	t2		= cps[c].t2;
	v2		= cps[c].v2;
	t1		= cps[c].t1;
	v1		= cps[c].v1;

	xmin		= cps[c].xmin+x;
	ymin		= cps[c].ymin;
	xmax		= cps[c].xmax+x;
	ymax		= cps[c].ymax;

	if(!vFlipped){
       ymin *= -1;
       ymax *= -1;
	}

    ymin += y;
    ymax += y;

	int firstIndex = stringQuads.getVertices().size();

	stringQuads.addVertex(ofVec3f(xmin,ymin));
	stringQuads.addVertex(ofVec3f(xmax,ymin));
	stringQuads.addVertex(ofVec3f(xmax,ymax));
	stringQuads.addVertex(ofVec3f(xmin,ymax));

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
int ofTrueTypeFont::getKerning(int c, int prevC) const{
    if(useKerning){
        FT_Vector kerning;
        FT_Get_Kerning(face.get(), FT_Get_Char_Index(face.get(), prevC), FT_Get_Char_Index(face.get(), c), FT_KERNING_UNFITTED, &kerning);
        return kerning.x>>6;
    }else{
        return 0;
    }
}

//-----------------------------------------------------------
vector<ofTTFCharacter> ofTrueTypeFont::getStringAsPoints(const std::string& str, bool vflip, bool filled) const{
	vector<ofTTFCharacter> shapes;

	if (!bLoadedOk){
		ofLogError("ofxTrueTypeFont") << "getStringAsPoints(): font not allocated: line " << __LINE__ << " in " << __FILE__;
		return shapes;
	};

	GLfloat		X		= 0;
	GLfloat		Y		= 0;
	int newLineDirection		= 1;

	if(vflip){
		// this would align multiline texts to the last line when vflip is disabled
		//int lines = ofStringTimesInString(c,"\n");
		//Y = lines*lineHeight;
		newLineDirection = -1;
	}

	try{
		int prevC = -1;
		for(auto c: ofUTF8Iterator(str)){
			int cy = c - NUM_CHARACTER_TO_START;
			if (c == '\n') {
				Y += lineHeight*newLineDirection;
				X = 0 ; //reset X Pos back to zero
            } else if( c == ' ' && spaceSize>0 ) {
                X += spaceSize;
                if(prevC > -1) {
                    X += getKerning(c,prevC) * letterSpacing;
                }
            } else if(cy >=0 && cy<nCharacters) {
				shapes.push_back(getCharacterAsPoints(c,vflip,filled));

				if(prevC > -1) {
					X += getKerning(c,prevC) * letterSpacing;
				}

				shapes.back().translate(ofPoint(X,Y));

				X += cps[cy].advance * letterSpacing;
			}
			prevC = c;
		}
	}catch(...){

	}
	return shapes;

}

//-----------------------------------------------------------
void ofTrueTypeFont::drawCharAsShape(int c, float x, float y, bool vFlipped, bool filled) const{
	if (c - NUM_CHARACTER_TO_START >= nCharacters || c < NUM_CHARACTER_TO_START){
		//ofLogError("ofTrueTypeFont") << "drawCharAsShape(): char " << << c + NUM_CHARACTER_TO_START << " not allocated: line " << __LINE__ << " in " << __FILE__;
		return;
	}
	//-----------------------
    if(vFlipped){
    	if(filled){
    		charOutlines[c - NUM_CHARACTER_TO_START].draw(x,y);
    	}else{
    		charOutlinesContour[c - NUM_CHARACTER_TO_START].draw(x,y);
    	}
    }else{
    	if(filled){
    		charOutlinesNonVFlipped[c - NUM_CHARACTER_TO_START].draw(x,y);
    	}else{
    		charOutlinesNonVFlippedContour[c - NUM_CHARACTER_TO_START].draw(x,y);
    	}
    }
}

//-----------------------------------------------------------
float ofTrueTypeFont::stringWidth(const std::string& c) const{
    ofRectangle rect = getStringBoundingBox(c, 0,0);
    return rect.width;
}

//-----------------------------------------------------------
ofRectangle ofTrueTypeFont::getStringBoundingBox(const std::string& c, float x, float y, bool vflip) const{
    if(c.empty()){
        return ofRectangle(x,y,0,0);
    }

    ofMesh mesh = getStringMesh(c,x,y,vflip);
	ofRectangle bb(std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),0,0);
	float maxX = std::numeric_limits<float>::min();
	float maxY = std::numeric_limits<float>::min();
	for(const auto & v: mesh.getVertices()){
		bb.x = min(v.x,bb.x);
		bb.y = min(v.y,bb.y);
		maxX = max(v.x,maxX);
		maxY = max(v.y,maxY);
	}
	bb.width = maxX - bb.x;
	bb.height = maxY - bb.y;
	return bb;
}

//-----------------------------------------------------------
float ofTrueTypeFont::stringHeight(const std::string& c) const{
    ofRectangle rect = getStringBoundingBox(c, 0,0);
    return rect.height;
}

//-----------------------------------------------------------
void ofTrueTypeFont::createStringMesh(const std::string& str, float x, float y, bool vFlipped) const{
	stringQuads.clear();
	GLfloat		X		= x;
	GLfloat		Y		= y;
	int newLineDirection		= 1;

	if(!vFlipped){
		// this would align multiline texts to the last line when vflip is disabled
		//int lines = ofStringTimesInString(c,"\n");
		//Y = lines*lineHeight;
		newLineDirection = -1;
	}

	int prevC = -1;
	try{
		for(auto c: ofUTF8Iterator(str)){
			int cy = c - NUM_CHARACTER_TO_START;
			if (c == '\n') {
				Y += lineHeight*newLineDirection;
				X = x ; //reset X Pos back to zero
				prevC = -1;
            } else if( c == ' ' && spaceSize>0 ) {
                X += spaceSize;
                if(prevC > -1) {
                    X += getKerning(c,prevC) * letterSpacing;
                }
            } else if(cy >=0 && cy<nCharacters) {
				if(prevC > -1) {
					X += getKerning(c,prevC) * letterSpacing;
				}
				drawChar(cy, X, Y, vFlipped);
				X += cps[cy].advance * letterSpacing;
			}
			prevC = c;
		}
	}catch(...){

	}
}

//-----------------------------------------------------------
const ofMesh & ofTrueTypeFont::getStringMesh(const std::string& c, float x, float y, bool vFlipped) const{
	createStringMesh(c,x,y,vFlipped);
	return stringQuads;
}

//-----------------------------------------------------------
const ofTexture & ofTrueTypeFont::getFontTexture() const{
	return texAtlas;
}

//-----------------------------------------------------------
void ofTrueTypeFont::drawString(const std::string& c, float x, float y) const{
	if (!bLoadedOk){
		ofLogError("ofTrueTypeFont") << "drawString(): font not allocated";
		return;
	}
	
	ofGetCurrentRenderer()->drawString(*this,c,x,y);

}

//-----------------------------------------------------------
void ofTrueTypeFont::drawStringAsShapes(const std::string& str, float x, float y) const{
    if (!bLoadedOk){
    	ofLogError("ofTrueTypeFont") << "drawStringAsShapes(): font not allocated: line " << __LINE__ << " in " << __FILE__;
    	return;
    };

	//----------------------- error checking
	if (!bMakeContours){
		ofLogError("ofTrueTypeFont") << "drawStringAsShapes(): contours not created for this font, call loadFont() with makeContours set to true";
		return;
	}

	GLfloat		X		= x;
	GLfloat		Y		= y;
	int newLineDirection		= 1;

	if(!ofIsVFlipped()){
		// this would align multiline texts to the last line when vflip is disabled
		//int lines = ofStringTimesInString(c,"\n");
		//Y = lines*lineHeight;
		newLineDirection = -1;
	}

    int prevC = -1;
    try{
		for(auto c: ofUTF8Iterator(str)){
			int cy = c - NUM_CHARACTER_TO_START;
			if (c == '\n') {
				Y += lineHeight*newLineDirection;
				X = x ; //reset X Pos back to zero
				prevC = -1;
            } else if( c == ' ' && spaceSize>0 ) {
                X += spaceSize;
                if(prevC > -1) {
                    X += getKerning(c,prevC) * letterSpacing;
                }
            } else if(cy >=0 && cy<nCharacters){
				if(prevC > -1) {
					X += getKerning(c,prevC) * letterSpacing;
				}
				drawCharAsShape(c, X, Y, ofIsVFlipped(), ofGetStyle().bFill);
				X += cps[cy].advance * letterSpacing;
			}
			prevC = c;
		}
    }catch(...){
    }

}

//-----------------------------------------------------------
int ofTrueTypeFont::getNumCharacters() const{
	return nCharacters;
}
