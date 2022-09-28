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
#include <numeric>

#include "ofGraphics.h"

using std::max;
using std::vector;
using std::string;
using std::min;

const ofUnicode::range ofUnicode::Space {32, 32};
const ofUnicode::range ofUnicode::IdeographicSpace {0x3000, 0x3000};
const ofUnicode::range ofUnicode::Latin {32, 0x007F};
const ofUnicode::range ofUnicode::Latin1Supplement {32,0x00FF};
const ofUnicode::range ofUnicode::LatinA {0x0100,0x017F};
const ofUnicode::range ofUnicode::Greek {0x0370, 0x03FF};
const ofUnicode::range ofUnicode::Cyrillic {0x0400, 0x04FF};
const ofUnicode::range ofUnicode::Arabic {0x0600, 0x077F};
const ofUnicode::range ofUnicode::ArabicSupplement {0x0750, 0x077F};
const ofUnicode::range ofUnicode::ArabicExtendedA {0x08A0, 0x08FF};
const ofUnicode::range ofUnicode::Devanagari {0x0900, 0x097F};
const ofUnicode::range ofUnicode::HangulJamo {0x1100, 0x11FF};
const ofUnicode::range ofUnicode::VedicExtensions {0x1CD0, 0x1CFF};
const ofUnicode::range ofUnicode::LatinExtendedAdditional {0x1E00, 0x1EFF};
const ofUnicode::range ofUnicode::GreekExtended {0x1F00, 0x1FFF};
const ofUnicode::range ofUnicode::GeneralPunctuation {0x2000, 0x206F};
const ofUnicode::range ofUnicode::SuperAndSubScripts {0x2070, 0x209F};
const ofUnicode::range ofUnicode::CurrencySymbols {0x20A0, 0x20CF};
const ofUnicode::range ofUnicode::LetterLikeSymbols {0x2100, 0x214F};
const ofUnicode::range ofUnicode::NumberForms {0x2150, 0x218F};
const ofUnicode::range ofUnicode::Arrows {0x2190, 0x21FF};
const ofUnicode::range ofUnicode::MathOperators {0x2200, 0x22FF};
const ofUnicode::range ofUnicode::MiscTechnical {0x2300, 0x23FF};
const ofUnicode::range ofUnicode::BoxDrawing {0x2500, 0x257F};
const ofUnicode::range ofUnicode::BlockElement {0x2580, 0x259F};
const ofUnicode::range ofUnicode::GeometricShapes {0x25A0, 0x25FF};
const ofUnicode::range ofUnicode::MiscSymbols {0x2600, 0x26FF};
const ofUnicode::range ofUnicode::Dingbats {0x2700, 0x27BF};
const ofUnicode::range ofUnicode::CJKSymbolAndPunctuation {0x3001, 0x303F};
const ofUnicode::range ofUnicode::Hiragana {0x3040, 0x309F};
const ofUnicode::range ofUnicode::Katakana {0x30A0, 0x30FF};
const ofUnicode::range ofUnicode::HangulCompatJamo {0x3130, 0x318F};
const ofUnicode::range ofUnicode::KatakanaPhoneticExtensions {0x31F0, 0x31FF};
const ofUnicode::range ofUnicode::CJKLettersAndMonths {0x3200, 0x32FF};
const ofUnicode::range ofUnicode::CJKUnified {0x4E00, 0x9FD5};
const ofUnicode::range ofUnicode::DevanagariExtended {0xA8E0, 0xA8FF};
const ofUnicode::range ofUnicode::HangulExtendedA {0xA960, 0xA97F};
const ofUnicode::range ofUnicode::HangulSyllables {0xAC00, 0xD7AF};
const ofUnicode::range ofUnicode::HangulExtendedB {0xD7B0, 0xD7FF};
const ofUnicode::range ofUnicode::AlphabeticPresentationForms {0xFB00, 0xFB4F};
const ofUnicode::range ofUnicode::ArabicPresFormsA {0xFB50, 0xFDFF};
const ofUnicode::range ofUnicode::ArabicPresFormsB {0xFE70, 0xFEFF};
const ofUnicode::range ofUnicode::KatakanaHalfAndFullwidthForms {0xFF00, 0xFFEF};
const ofUnicode::range ofUnicode::KanaSupplement {0x1B000, 0x1B0FF};
const ofUnicode::range ofUnicode::RumiNumericalSymbols {0x10E60, 0x10E7F};
const ofUnicode::range ofUnicode::ArabicMath {0x1EE00, 0x1EEFF};
const ofUnicode::range ofUnicode::MiscSymbolsAndPictographs {0x1F300, 0x1F5FF};
const ofUnicode::range ofUnicode::Emoticons {0x1F601, 0x1F64F};
const ofUnicode::range ofUnicode::TransportAndMap {0x1F680, 0x1F6FF};
const ofUnicode::range ofUnicode::EnclosedCharacters {0x24C2, 0x1F251};
const ofUnicode::range ofUnicode::Uncategorized {0x00A9, 0x1F5FF};
const ofUnicode::range ofUnicode::AdditionalEmoticons {0x1F600, 0x1F636};
const ofUnicode::range ofUnicode::AdditionalTransportAndMap {0x1F681, 0x1F6C5};
const ofUnicode::range ofUnicode::OtherAdditionalSymbols {0x1F30D, 0x1F567};
const ofUnicode::range ofUnicode::UppercaseLatin {65, 90};
const ofUnicode::range ofUnicode::LowercaseLatin {97, 122};
const ofUnicode::range ofUnicode::Braces {123, 127};
const ofUnicode::range ofUnicode::Numbers {48, 57};
const ofUnicode::range ofUnicode::Symbols {33, 47};
const ofUnicode::range ofUnicode::GenericSymbols {58, 64};

const std::initializer_list<ofUnicode::range> ofAlphabet::Emoji {
	ofUnicode::Space,
	ofUnicode::Emoticons,
	ofUnicode::Dingbats,
	ofUnicode::Uncategorized,
	ofUnicode::TransportAndMap,
	ofUnicode::EnclosedCharacters,
	ofUnicode::OtherAdditionalSymbols,

};

const std::initializer_list<ofUnicode::range> ofAlphabet::Japanese {
	ofUnicode::Space,
	ofUnicode::IdeographicSpace,
	ofUnicode::CJKSymbolAndPunctuation,
	ofUnicode::Hiragana,
	ofUnicode::Katakana,
	ofUnicode::KatakanaPhoneticExtensions,
	ofUnicode::CJKLettersAndMonths,
	ofUnicode::CJKUnified
};

const std::initializer_list<ofUnicode::range> ofAlphabet::Chinese {
	ofUnicode::Space,
	ofUnicode::IdeographicSpace,
	ofUnicode::CJKSymbolAndPunctuation,
	ofUnicode::CJKLettersAndMonths,
	ofUnicode::CJKUnified
};

const std::initializer_list<ofUnicode::range> ofAlphabet::Korean {
	ofUnicode::Space,
	ofUnicode::IdeographicSpace,
	ofUnicode::CJKSymbolAndPunctuation,
	ofUnicode::HangulJamo,
	ofUnicode::HangulCompatJamo,
	ofUnicode::HangulExtendedA,
	ofUnicode::HangulExtendedB,
	ofUnicode::HangulSyllables
};

const std::initializer_list<ofUnicode::range> ofAlphabet::Arabic {
	ofUnicode::Space,
	ofUnicode::Arabic,
	ofUnicode::ArabicExtendedA,
	ofUnicode::ArabicMath,
	ofUnicode::ArabicPresFormsA,
	ofUnicode::ArabicPresFormsB
};

const std::initializer_list<ofUnicode::range> ofAlphabet::Devanagari {
	ofUnicode::Devanagari,
	ofUnicode::DevanagariExtended,
	ofUnicode::VedicExtensions
};

const std::initializer_list<ofUnicode::range> ofAlphabet::Latin {
	ofUnicode::Latin1Supplement,
	ofUnicode::LatinExtendedAdditional,
	ofUnicode::Latin,
	ofUnicode::LatinA,
};

const std::initializer_list<ofUnicode::range> ofAlphabet::Greek {
	ofUnicode::Space,
	ofUnicode::Greek,
	ofUnicode::GreekExtended
};

const std::initializer_list<ofUnicode::range> ofAlphabet::Cyrillic {
	ofUnicode::Space,
	ofUnicode::Cyrillic
};

const ofTrueTypeFont::glyphProps ofTrueTypeFont::invalidProps{
	0,
	0,
	0,
	0,
	0,0,
	0,0,0,0,
	0,
	0.0f,0.0f,
	0.0f,0.0f,0.0f,0.0f
};

const size_t TAB_WIDTH = 4; /// Number of spaces per tab

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
static ofPath makeContoursForCharacter(FT_Face face){
	ofPath charOutlines;
	charOutlines.setUseShapeColor(false);
	charOutlines.setPolyWindingMode(OF_POLY_WINDING_NONZERO);
	auto moveTo = [](const FT_Vector*to, void * userData){
		ofPath * charOutlines = static_cast<ofPath*>(userData);
		charOutlines->moveTo(ofTrueTypeFont::int26p6_to_dbl(to->x), ofTrueTypeFont::int26p6_to_dbl(-to->y));
		return 0;
	};
	auto lineTo = [](const FT_Vector*to, void * userData){
		ofPath * charOutlines = static_cast<ofPath*>(userData);
		charOutlines->lineTo(ofTrueTypeFont::int26p6_to_dbl(to->x), ofTrueTypeFont::int26p6_to_dbl(-to->y));
		return 0;
	};
	auto conicTo = [](const FT_Vector*cp, const FT_Vector*to, void * userData){
		ofPath * charOutlines = static_cast<ofPath*>(userData);
		auto lastP = charOutlines->getCommands().back().to;
		charOutlines->quadBezierTo(lastP, { ofTrueTypeFont::int26p6_to_dbl(cp->x), ofTrueTypeFont::int26p6_to_dbl(-cp->y) },
								   {ofTrueTypeFont::int26p6_to_dbl(to->x), ofTrueTypeFont::int26p6_to_dbl(-to->y)});
		return 0;
	};
	auto cubicTo = [](const FT_Vector*cp1, const FT_Vector*cp2, const FT_Vector*to, void * userData){
		ofPath * charOutlines = static_cast<ofPath*>(userData);
		charOutlines->bezierTo({ofTrueTypeFont::int26p6_to_dbl(cp1->x), ofTrueTypeFont::int26p6_to_dbl(-cp1->y)},
							   {ofTrueTypeFont::int26p6_to_dbl(cp2->x), ofTrueTypeFont::int26p6_to_dbl(-cp2->y)},
							   {ofTrueTypeFont::int26p6_to_dbl(to->x),  ofTrueTypeFont::int26p6_to_dbl(-to->y)});
		return 0;
	};
	FT_Outline_Funcs funcs{
		moveTo,
		lineTo,
		conicTo,
		cubicTo,
		0,
		0,
	};

	FT_Outline_Decompose(&face->glyph->outline, &funcs, &charOutlines);
	charOutlines.close();

	return charOutlines;
}

#ifdef TARGET_OSX
//------------------------------------------------------------------
static string osxFontPathByName(const string& fontname){
	CFStringRef targetName = CFStringCreateWithCString(nullptr, fontname.c_str(), kCFStringEncodingUTF8);
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
static std::map<string, string> fonts_table;
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
	string fontsDir = getenv ("windir");
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
			string curr_face = value_name_char;
			string font_file = value_data_char;
			curr_face = curr_face.substr(0, curr_face.find('(') - 1);
			fonts_table[curr_face] = fontsDir + font_file;
	}


	HeapFree(GetProcessHeap(), 0, value_data);

	l_ret = RegCloseKey(key_ft);
}


static string winFontPathByName(const string& fontname ){
	if(fonts_table.find(fontname)!=fonts_table.end()){
		return fonts_table[fontname];
	}
	for(std::map<string,string>::iterator it = fonts_table.begin(); it!=fonts_table.end(); it++){
		if(ofIsStringInString(ofToLower(it->first),ofToLower(fontname))) return it->second;
	}
	return "";
}
#endif

#ifdef TARGET_LINUX
//------------------------------------------------------------------
static string linuxFontPathByName(const string& fontname){
	string filename;
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

//-----------------------------------------------------------
static bool loadFontFace(const std::filesystem::path& _fontname, FT_Face & face, std::filesystem::path & filename, int index){
	std::filesystem::path fontname = _fontname;
	filename = ofToDataPath(_fontname,true);
	ofFile fontFile(filename,ofFile::Reference);
	int fontID = index;
	if(!fontFile.exists()){
#ifdef TARGET_LINUX
		filename = linuxFontPathByName(fontname.string());
#elif defined(TARGET_OSX)
		if(fontname==OF_TTF_SANS){
			fontname = "Helvetica Neue";
			#if MAC_OS_X_VERSION_10_13 && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_13
				fontID = 0;
			#else
				fontID = 4;
			#endif
		}else if(fontname==OF_TTF_SERIF){
			fontname = "Times New Roman";
		}else if(fontname==OF_TTF_MONO){
			fontname = "Menlo Regular";
		}
		filename = osxFontPathByName(fontname.string());
#elif defined(TARGET_WIN32)
		if(fontname==OF_TTF_SANS){
			fontname = "Arial";
		}else if(fontname==OF_TTF_SERIF){
			fontname = "Times New Roman";
		}else if(fontname==OF_TTF_MONO){
			fontname = "Courier New";
		}
		filename = winFontPathByName(fontname.string());
#endif
		if(filename == "" ){
			ofLogError("ofTrueTypeFont") << "loadFontFace(): couldn't find font \"" << fontname << "\"";
			return false;
		}
		ofLogVerbose("ofTrueTypeFont") << "loadFontFace(): \"" << fontname << "\" not a file in data loading system font from \"" << filename << "\"";
	}
	FT_Error err;
	err = FT_New_Face( library, filename.string().c_str(), fontID, &face );
	if (err) {
		// simple error table in lieu of full table (see fterrors.h)
		string errorString = "unknown freetype";
		if(err == 1) errorString = "INVALID FILENAME";
		ofLogError("ofTrueTypeFont") << "loadFontFace(): couldn't create new face for \"" << fontname << "\": FT_Error " << err << " " << errorString;
		return false;
	}

	return true;
}

//--------------------------------------------------------
void ofTrueTypeFont::setGlobalDpi(int newDpi){
	ttfGlobalDpi = newDpi;
}


#if defined(TARGET_ANDROID)
#include "ofxAndroidUtils.h"
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
ofTrueTypeFont::ofTrueTypeFont()
:settings("",0){
	bLoadedOk		= false;
	letterSpacing = 1;
	spaceSize = 1;
	fontUnitScale = 1;
	stringQuads.setMode(OF_PRIMITIVE_TRIANGLES);
	ascenderHeight = 0;
	descenderHeight = 0;
	lineHeight = 0;
}

//------------------------------------------------------------------
ofTrueTypeFont::~ofTrueTypeFont(){
	#if defined(TARGET_ANDROID)
	ofRemoveListener(ofxAndroidEvents().unloadGL,this,&ofTrueTypeFont::unloadTextures);
	ofRemoveListener(ofxAndroidEvents().reloadGL,this,&ofTrueTypeFont::reloadTextures);
	#endif
}

//------------------------------------------------------------------
ofTrueTypeFont::ofTrueTypeFont(const ofTrueTypeFont& mom)
:settings(mom.settings){
#if defined(TARGET_ANDROID)
	if(mom.isLoaded()){
		ofAddListener(ofxAndroidEvents().unloadGL,this,&ofTrueTypeFont::unloadTextures);
		ofAddListener(ofxAndroidEvents().reloadGL,this,&ofTrueTypeFont::reloadTextures);
	}
#endif
	bLoadedOk = mom.bLoadedOk;

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
	fontUnitScale = mom.fontUnitScale;

	cps = mom.cps; // properties for each character
	settings = mom.settings;
	glyphIndexMap = mom.glyphIndexMap;
	texAtlas = mom.texAtlas;
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
	settings = mom.settings;
	bLoadedOk = mom.bLoadedOk;

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
	fontUnitScale = mom.fontUnitScale;

	cps = mom.cps; // properties for each character
	settings = mom.settings;
	glyphIndexMap = mom.glyphIndexMap;
	texAtlas = mom.texAtlas;
	face = mom.face;

	return *this;
}

//------------------------------------------------------------------
ofTrueTypeFont::ofTrueTypeFont(ofTrueTypeFont&& mom)
:settings(std::move(mom.settings)){
#if defined(TARGET_ANDROID)
	if(mom.isLoaded()){
		ofAddListener(ofxAndroidEvents().unloadGL,this,&ofTrueTypeFont::unloadTextures);
		ofAddListener(ofxAndroidEvents().reloadGL,this,&ofTrueTypeFont::reloadTextures);
	}
#endif
	bLoadedOk = mom.bLoadedOk;

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
	fontUnitScale = mom.fontUnitScale;

	cps = mom.cps; // properties for each character
	settings = mom.settings;
	glyphIndexMap = std::move(mom.glyphIndexMap);
	texAtlas = mom.texAtlas;
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
	fontUnitScale = mom.fontUnitScale;

	cps = mom.cps; // properties for each character
	settings = mom.settings;
	glyphIndexMap = std::move(mom.glyphIndexMap);
	texAtlas = mom.texAtlas;
	face = mom.face;
	return *this;
}

//-----------------------------------------------------------
void ofTrueTypeFont::unloadTextures(){
	if(!bLoadedOk) return;
	texAtlas.clear();
}

//-----------------------------------------------------------
void ofTrueTypeFont::reloadTextures(){
	if(bLoadedOk) load(settings);
}

//-----------------------------------------------------------
bool ofTrueTypeFont::loadFont(string filename, int fontSize, bool bAntiAliased, bool bFullCharacterSet, bool makeContours, float simplifyAmt, int dpi) {
	return load(filename, fontSize, bAntiAliased, bFullCharacterSet, makeContours, simplifyAmt, dpi);
}

//-----------------------------------------------------------
ofTrueTypeFont::glyph ofTrueTypeFont::loadGlyph(uint32_t utf8) const{
	glyph aGlyph;
	auto err = FT_Load_Glyph( face.get(), FT_Get_Char_Index( face.get(), utf8 ), settings.antialiased ?  FT_LOAD_FORCE_AUTOHINT : FT_LOAD_DEFAULT );
	if(err){
		ofLogError("ofTrueTypeFont") << "loadFont(): FT_Load_Glyph failed for utf8 code " << utf8 << ": FT_Error " << err;
		return aGlyph;
	}

	if (settings.antialiased) FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
	else FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO);


	// -------------------------
	// info about the character:
	aGlyph.props.glyph		= utf8;
	aGlyph.props.height 	= int26p6_to_dbl(face->glyph->metrics.height);
	aGlyph.props.width 		= int26p6_to_dbl(face->glyph->metrics.width);
	aGlyph.props.bearingX	= int26p6_to_dbl(face->glyph->metrics.horiBearingX);
	aGlyph.props.bearingY	= int26p6_to_dbl(face->glyph->metrics.horiBearingY);
	aGlyph.props.xmin		= face->glyph->bitmap_left;
	aGlyph.props.xmax		= aGlyph.props.xmin + aGlyph.props.width;
	aGlyph.props.ymin		= -face->glyph->bitmap_top;
	aGlyph.props.ymax		= aGlyph.props.ymin + aGlyph.props.height;
	aGlyph.props.advance	= int26p6_to_dbl(face->glyph->metrics.horiAdvance);
	aGlyph.props.tW			= aGlyph.props.width;
	aGlyph.props.tH			= aGlyph.props.height;

	FT_Bitmap& bitmap= face->glyph->bitmap;
	int width  = bitmap.width;
	int height = bitmap.rows;
	if(width==0 || height==0) return aGlyph;

	// Allocate Memory For The Texture Data.
	aGlyph.pixels.allocate(width, height, OF_PIXELS_GRAY_ALPHA);
	//-------------------------------- clear data:
	aGlyph.pixels.set(0,255); // every luminance pixel = 255
	aGlyph.pixels.set(1,0);


	if (settings.antialiased == true){
		ofPixels bitmapPixels;
		bitmapPixels.setFromExternalPixels(bitmap.buffer,bitmap.width,bitmap.rows,OF_PIXELS_GRAY);
		aGlyph.pixels.setChannel(1,bitmapPixels);
	} else {
		//-----------------------------------
		// true type packs monochrome info in a
		// 1-bit format, hella funky
		// here we unpack it:
		unsigned char *src =  bitmap.buffer;
		for(unsigned int j=0; j <bitmap.rows;j++) {
			unsigned char b=0;
			unsigned char *bptr =  src;
			for(unsigned int k=0; k < bitmap.width ; k++){
				aGlyph.pixels[2*(k+j*width)] = 255;

				if (k%8==0){
					b = (*bptr++);
				}

				aGlyph.pixels[2*(k+j*width) + 1] = b&0x80 ? 255 : 0;
				b <<= 1;
			}
			src += bitmap.pitch;
		}
		//-----------------------------------
	}

	return aGlyph;
}

//-----------------------------------------------------------
bool ofTrueTypeFont::load(const std::filesystem::path& filename, int fontSize, bool antialiased, bool fullCharacterSet, bool makeContours, float simplifyAmt, int dpi) {
	
	ofTrueTypeFontSettings settings(filename,fontSize);
	settings.antialiased = antialiased;
	settings.contours = makeContours;
	settings.simplifyAmt = simplifyAmt;
	settings.dpi = dpi;
	if(fullCharacterSet){
		settings.ranges = {ofUnicode::Latin1Supplement};
	}else{
		settings.ranges = {ofUnicode::Latin};
	}
	return load(settings);
}

bool ofTrueTypeFont::load(const ofTrueTypeFontSettings & _settings){
	#if defined(TARGET_ANDROID)
	ofAddListener(ofxAndroidEvents().unloadGL,this,&ofTrueTypeFont::unloadTextures);
	ofAddListener(ofxAndroidEvents().reloadGL,this,&ofTrueTypeFont::reloadTextures);
	#endif

	initLibraries();
	settings = _settings;
	if( settings.dpi == 0 ){
		settings.dpi = ttfGlobalDpi;
	}

	bLoadedOk = false;

	//--------------- load the library and typeface
	FT_Face loadFace;
	if(!loadFontFace(settings.fontName, loadFace, settings.fontName, settings.index)){
		return false;
	}
	face = std::shared_ptr<struct FT_FaceRec_>(loadFace,FT_Done_Face);

	if(settings.ranges.empty()){
		settings.ranges.push_back(ofUnicode::Latin1Supplement);
	}
	int border = 1;


	FT_Set_Char_Size( face.get(), dbl_to_int26p6(settings.fontSize), dbl_to_int26p6(settings.fontSize), settings.dpi, settings.dpi);
	fontUnitScale = (float(settings.fontSize * settings.dpi)) / (72 * face->units_per_EM);
	lineHeight = face->height * fontUnitScale;
	ascenderHeight = face->ascender * fontUnitScale;
	descenderHeight = face->descender * fontUnitScale;
	glyphBBox.set(face->bbox.xMin * fontUnitScale,
				   face->bbox.yMin * fontUnitScale,
				  (face->bbox.xMax - face->bbox.xMin) * fontUnitScale,
				  (face->bbox.yMax - face->bbox.yMin) * fontUnitScale);

	//--------------- initialize character info and textures
	auto nGlyphs = std::accumulate(settings.ranges.begin(), settings.ranges.end(), 0u,
			[](uint32_t acc, ofUnicode::range range){
				return acc + range.getNumGlyphs();
			});
	cps.resize(nGlyphs);
	if(settings.contours){
		charOutlines.resize(nGlyphs);
		charOutlinesNonVFlipped.resize(nGlyphs);
		charOutlinesContour.resize(nGlyphs);
		charOutlinesNonVFlippedContour.resize(nGlyphs);
	}else{
		charOutlines.resize(1);
	}

	vector<ofTrueTypeFont::glyph> all_glyphs;

	uint32_t areaSum=0;

	//--------------------- load each char -----------------------
	auto i = 0u;
	for(auto & range: settings.ranges){
		for (uint32_t g = range.begin; g <= range.end; g++, i++){
			all_glyphs.push_back(loadGlyph(g));
			all_glyphs[i].props.characterIndex	= i;
			glyphIndexMap[g] = i;
			cps[i] = all_glyphs[i].props;
			areaSum += (cps[i].tW+border*2)*(cps[i].tH+border*2);

			if(settings.contours){
				if(printVectorInfo){
					string str;
					ofUTF8Append(str,g);
					ofLogNotice("ofTrueTypeFont") <<  "character " << str;
				}

				//int character = i + NUM_CHARACTER_TO_START;
				charOutlines[i] = makeContoursForCharacter( face.get() );
				charOutlinesContour[i] = charOutlines[i];
				charOutlinesContour[i].setFilled(false);
				charOutlinesContour[i].setStrokeWidth(1);

				charOutlinesNonVFlipped[i] = charOutlines[i];
				charOutlinesNonVFlipped[i].translate({0,cps[i].height,0.f});
				charOutlinesNonVFlipped[i].scale(1,-1);
				charOutlinesNonVFlippedContour[i] = charOutlines[i];
				charOutlinesNonVFlippedContour[i].setFilled(false);
				charOutlinesNonVFlippedContour[i].setStrokeWidth(1);


				if(settings.simplifyAmt>0){
					charOutlines[i].simplify(settings.simplifyAmt);
					charOutlinesNonVFlipped[i].simplify(settings.simplifyAmt);
					charOutlinesContour[i].simplify(settings.simplifyAmt);
					charOutlinesNonVFlippedContour[i].simplify(settings.simplifyAmt);
				}
			}
		}
	}

	vector<ofTrueTypeFont::glyphProps> sortedCopy = cps;
	sort(sortedCopy.begin(),sortedCopy.end(),[](const ofTrueTypeFont::glyphProps & c1, const ofTrueTypeFont::glyphProps & c2){
		if(c1.tH == c2.tH) return c1.tW > c2.tW;
		else return c1.tH > c2.tH;
	});

	// pack in a texture, algorithm to calculate min w/h from
	// http://upcommons.upc.edu/pfc/bitstream/2099.1/7720/1/TesiMasterJonas.pdf
	//ofLogNotice("ofTrueTypeFont") << "loadFont(): areaSum: " << areaSum

	bool packed = false;
	float alpha = logf(areaSum)*1.44269f;
	int w;
	int h;
	while(!packed){
		w = pow(2,floor((alpha/2.f) + 0.5f)); // there doesn't seem to be a round in cmath for windows.
		//w = pow(2,round(alpha/2.f));
		h = w;//pow(2,round(alpha - round(alpha/2.f)));
		int x=0;
		int y=0;
		auto maxRowHeight = sortedCopy[0].tH + border*2;
		packed = true;
		for(auto & glyph: sortedCopy){
			if(x+glyph.tW + border*2>w){
				x = 0;
				y += maxRowHeight;
				maxRowHeight = glyph.tH + border*2;
				if(y + maxRowHeight > h){
					alpha++;
					packed = false;
					break;
				}
			}
			x+= glyph.tW + border*2;
		}

	}



	ofPixels atlasPixelsLuminanceAlpha;
	atlasPixelsLuminanceAlpha.allocate(w,h,OF_PIXELS_GRAY_ALPHA);
	atlasPixelsLuminanceAlpha.set(0,255);
	atlasPixelsLuminanceAlpha.set(1,0);


	float x=0;
	float y=0;
	auto maxRowHeight = sortedCopy[0].tH + border*2.0;
	for(auto & glyph: sortedCopy){
		ofPixels & charPixels = all_glyphs[glyph.characterIndex].pixels;

		if(x+glyph.tW + border*2>w){
			x = 0;
			y += maxRowHeight;
			maxRowHeight = glyph.tH + border*2.0;
		}

		cps[glyph.characterIndex].t1		= float(x + border)/float(w);
		cps[glyph.characterIndex].v1		= float(y + border)/float(h);
		cps[glyph.characterIndex].t2		= float(cps[glyph.characterIndex].tW + x + border)/float(w);
		cps[glyph.characterIndex].v2		= float(cps[glyph.characterIndex].tH + y + border)/float(h);
		charPixels.pasteInto(atlasPixelsLuminanceAlpha,x+border,y+border);
		x+= glyph.tW + border*2.0;
	}

	int maxSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
	if(w > maxSize || h > maxSize){
		ofLogError("ofTruetypeFont") << "Trying to allocate texture of " << w << "x" << h << " which is bigger than supported in current platform: " << maxSize;
		return false;
	}else{
		texAtlas.allocate(atlasPixelsLuminanceAlpha,false);
		texAtlas.setRGToRGBASwizzles(true);

		if(settings.antialiased && settings.fontSize>20){
			texAtlas.setTextureMinMagFilter(GL_LINEAR,GL_LINEAR);
		}else{
			texAtlas.setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
		}
		texAtlas.loadData(atlasPixelsLuminanceAlpha);
		bLoadedOk = true;
		return true;
	}
}

//-----------------------------------------------------------
bool ofTrueTypeFont::isLoaded() const{
	return bLoadedOk;
}

//-----------------------------------------------------------
bool ofTrueTypeFont::isAntiAliased() const{
	return settings.antialiased;
}

//-----------------------------------------------------------
bool ofTrueTypeFont::hasFullCharacterSet() const{
	return true;
}

//-----------------------------------------------------------
int ofTrueTypeFont::getSize() const{
	return settings.fontSize;
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
ofPath ofTrueTypeFont::getCharacterAsPoints(uint32_t character, bool vflip, bool filled) const{
	if( settings.contours == false ){
		ofLogError("ofxTrueTypeFont") << "getCharacterAsPoints(): contours not created, call loadFont() with makeContours set to true";
		return ofPath();
	}
	if (!isValidGlyph(character)){
		return ofPath();
	}

	if(vflip){
		if(filled){
			return charOutlines[indexForGlyph(character)];
		}else{
			return charOutlinesContour[indexForGlyph(character)];
		}
	}else{
		if(filled){
			return charOutlinesNonVFlipped[indexForGlyph(character)];
		}else{
			return charOutlinesNonVFlippedContour[indexForGlyph(character)];
		}
	}
}

//-----------------------------------------------------------
void ofTrueTypeFont::drawChar(uint32_t c, float x, float y, bool vFlipped) const{

	if (!isValidGlyph(c)){
		//ofLogError("ofTrueTypeFont") << "drawChar(): char " << c + NUM_CHARACTER_TO_START << " not allocated: line " << __LINE__ << " in " << __FILE__;
		return;
	}

	auto props = getGlyphProperties(c);

	float xmin		= props.xmin+x;
	float ymin		= props.ymin;
	float xmax		= props.xmax+x;
	float ymax		= props.ymax;

	if(!vFlipped){
	   ymin *= -1.0;
	   ymax *= -1.0;
	}

	ymin += y;
	ymax += y;

	ofIndexType firstIndex = stringQuads.getVertices().size();

	stringQuads.addVertex(glm::vec3(xmin,ymin,0.f));
	stringQuads.addVertex(glm::vec3(xmax,ymin,0.f));
	stringQuads.addVertex(glm::vec3(xmax,ymax,0.f));
	stringQuads.addVertex(glm::vec3(xmin,ymax,0.f));

	stringQuads.addTexCoord(glm::vec2(props.t1,props.v1));
	stringQuads.addTexCoord(glm::vec2(props.t2,props.v1));
	stringQuads.addTexCoord(glm::vec2(props.t2,props.v2));
	stringQuads.addTexCoord(glm::vec2(props.t1,props.v2));

	stringQuads.addIndex(firstIndex);
	stringQuads.addIndex(firstIndex+1);
	stringQuads.addIndex(firstIndex+2);
	stringQuads.addIndex(firstIndex+2);
	stringQuads.addIndex(firstIndex+3);
	stringQuads.addIndex(firstIndex);
	
	
}

//-----------------------------------------------------------
double ofTrueTypeFont::getKerning(uint32_t leftC, uint32_t rightC) const{
	if(FT_HAS_KERNING( face )){
		FT_Vector kerning;
		FT_Get_Kerning(face.get(), FT_Get_Char_Index(face.get(), leftC), FT_Get_Char_Index(face.get(), rightC), FT_KERNING_UNFITTED, &kerning);
		return int26p6_to_dbl(kerning.x);
	}else{
		return 0.0;
	}
}

void ofTrueTypeFont::iterateString(const string & str, float x, float y, bool vFlipped, std::function<void(uint32_t, glm::vec2)> f) const{
	glm::vec2 pos(x,y);

	float newLineDirection		= 1;
	if(!vFlipped){
		// this would align multiline texts to the last line when vflip is disabled
		//int lines = ofStringTimesInString(c,"\n");
		//Y = lines*lineHeight;
		newLineDirection = -1;
	}

	float directionX = settings.direction == OF_TTF_LEFT_TO_RIGHT?1:-1;

	uint32_t prevC = 0;
	for(auto c: ofUTF8Iterator(str)){
		try{
			if (c == '\n') {
				pos.y += lineHeight*newLineDirection;
				pos.x = x ; //reset X Pos back to zero
				prevC = 0;
			} else if (c == '\t') {
				if (settings.direction == OF_TTF_LEFT_TO_RIGHT){
					f(c, pos);
					pos.x += getGlyphProperties(' ').advance * spaceSize * TAB_WIDTH * directionX;
				} else{
					pos.x += getGlyphProperties(' ').advance * spaceSize * TAB_WIDTH * directionX;
					f(c, pos);
				}
				prevC = c;
			}else if(c == ' '){
				pos.x += getGlyphProperties(' ').advance * spaceSize * directionX;
				f(c, pos);
				prevC = c;
			}else if(isValidGlyph(c)) {
				const auto & props = getGlyphProperties(c);
				if(prevC>0){
					if(settings.direction == OF_TTF_LEFT_TO_RIGHT){
						pos.x += getKerning(prevC, c);
					}else{
						pos.x += getKerning(c, prevC);
					}
				}
				if(settings.direction == OF_TTF_LEFT_TO_RIGHT){
					f(c,pos);
					pos.x += props.advance  * directionX;
					pos.x += getGlyphProperties(' ').advance * (letterSpacing - 1.f) * directionX;
				}else{
					pos.x += props.advance  * directionX;
					pos.x += getGlyphProperties(' ').advance * (letterSpacing - 1.f) * directionX;
					f(c,pos);
				}
				prevC = c;
			}
		}catch(...){
			break;
		}
	}
}

//-----------------------------------------------------------
void ofTrueTypeFont::setDirection(ofTrueTypeFontDirection direction){
	settings.direction = direction;
}

//-----------------------------------------------------------
vector<ofPath> ofTrueTypeFont::getStringAsPoints(const string &  str, bool vflip, bool filled) const{
	vector<ofPath> shapes;

	if (!bLoadedOk){
		ofLogError("ofxTrueTypeFont") << "getStringAsPoints(): font not allocated: line " << __LINE__ << " in " << __FILE__;
		return shapes;
	};
	iterateString(str,0,0,vflip,[&](uint32_t c, glm::vec2 pos){
		shapes.push_back(getCharacterAsPoints(c,vflip,filled));
		shapes.back().translate(glm::vec3{pos, 0.f});
	});
	return shapes;

}

bool ofTrueTypeFont::isValidGlyph(uint32_t glyph) const{
	//return glyphIndexMap.find(glyph) != glyphIndexMap.end();
	return std::any_of(settings.ranges.begin(), settings.ranges.end(),
		[&](ofUnicode::range range){
			return glyph >= range.begin && glyph <= range.end;
	});
}

size_t ofTrueTypeFont::indexForGlyph(uint32_t glyph) const{
	return glyphIndexMap.find(glyph)->second;
}

const ofTrueTypeFont::glyphProps & ofTrueTypeFont::getGlyphProperties(uint32_t glyph) const{
	if(isValidGlyph(glyph)){
		return cps[indexForGlyph(glyph)];
	}else{
		return invalidProps;
	}
}

//-----------------------------------------------------------
void ofTrueTypeFont::drawCharAsShape(uint32_t c, float x, float y, bool vFlipped, bool filled) const{
	if(vFlipped){
		if(filled){
			charOutlines[indexForGlyph(c)].draw(x,y);
		}else{
			charOutlinesContour[indexForGlyph(c)].draw(x,y);
		}
	}else{
		if(filled){
			charOutlinesNonVFlipped[indexForGlyph(c)].draw(x,y);
		}else{
			charOutlinesNonVFlippedContour[indexForGlyph(c)].draw(x,y);
		}
	}
}

//-----------------------------------------------------------
float ofTrueTypeFont::stringWidth(const string& c) const{
//	return getStringBoundingBox(c, 0,0).width;
	int w = 0;
	iterateString( c, 0, 0, false, [&]( uint32_t c, glm::vec2 pos ){
		if ( c == '\t' ){
			w += getGlyphProperties(' ').advance * spaceSize * TAB_WIDTH;
		} else {
			w += getGlyphProperties( c ).advance;
		}
	});
	return w;
}

//-----------------------------------------------------------
ofRectangle ofTrueTypeFont::getStringBoundingBox(const string& c, float x, float y, bool vflip) const{

	if ( c.empty() ){
		return ofRectangle( x, y, 0.f, 0.f);
	}

	float minX = x;
	float minY = y;
	float maxY = y;
	// Calculate bounding box by iterating over glyph properties
	// Meaning of props can be deduced from illustration at top of:
	// https://www.freetype.org/freetype2/docs/tutorial/step2.html
	// 
	// We deliberately not generate a mesh and iterate over its
	// vertices, as this would not correctly return spacing for
	// blank characters.
	
	float w = 0;
	iterateString( c, x, y, vflip, [&]( uint32_t c, glm::vec2 pos ){
		auto props = getGlyphProperties( c );
		if ( c == '\t' ){
			w += props.advance * spaceSize * TAB_WIDTH;
		} else {
			w += props.advance;
		}

		minX = min( minX, pos.x );
		if ( vflip ){
			minY = min( minY, pos.y - ( props.ymax - props.ymin ) );
			maxY = max( maxY, pos.y - ( props.bearingY - props.height ) );
		} else{
			minY = min( minY, pos.y - ( props.ymax) );
			maxY = max( maxY, pos.y - ( props.ymin ) );
		}
	});

	float height = maxY - minY;

	return ofRectangle(minX, minY, w, height);
}

//-----------------------------------------------------------
float ofTrueTypeFont::stringHeight(const string& c) const{
	ofRectangle rect = getStringBoundingBox(c, 0,0);
	return rect.height;
}

//-----------------------------------------------------------
void ofTrueTypeFont::createStringMesh(const string& str, float x, float y, bool vflip) const{
	iterateString(str,x,y,vflip,[&](uint32_t c, glm::vec2 pos){
		drawChar(c, pos.x, pos.y, vflip);
	});
}

//-----------------------------------------------------------
const ofMesh & ofTrueTypeFont::getStringMesh(const string& c, float x, float y, bool vFlipped) const{
	stringQuads.clear();
	createStringMesh(c,x,y,vFlipped);
	return stringQuads;
}

//-----------------------------------------------------------
const ofTexture & ofTrueTypeFont::getFontTexture() const{
	return texAtlas;
}

//-----------------------------------------------------------
glm::vec2 ofTrueTypeFont::getFirstGlyphPosForTexture(const string & str, bool vflip) const{
	if(!str.empty()){
		try{
			auto c = *ofUTF8Iterator(str).begin();
			if(settings.direction == OF_TTF_LEFT_TO_RIGHT){
				if (c != '\n') {
					auto g = loadGlyph(c);
					return {-float(g.props.xmin), getLineHeight() + g.props.ymin + getDescenderHeight()};
				}
			}else{
				int width = 0;
				int lineWidth = 0;
				iterateString(str, 0, 0, vflip, [&](uint32_t c, ofVec2f){
					try{
						if (c != '\n') {
							auto g = loadGlyph(c);
							if(c == '\t')lineWidth += g.props.advance + getGlyphProperties(' ').advance * spaceSize * TAB_WIDTH;
							else if(c == ' ')lineWidth += g.props.advance + getGlyphProperties(' ').advance * spaceSize;
							else if(isValidGlyph(c))lineWidth += g.props.advance + getGlyphProperties(' ').advance * (letterSpacing - 1.f);
							width = max(width, lineWidth);
						}else{
							lineWidth = 0;
						}
					}catch(...){
					}
				});
				if (c != '\n') {
					auto g = loadGlyph(c);
					return {width - float(g.props.xmin), getLineHeight() + g.props.ymin + getDescenderHeight()};
				}else{
					return {float(width), 0.0};
				}
			}
		}catch(...){}
	}

	return {0.f, 0.f};
}

//-----------------------------------------------------------
ofTexture ofTrueTypeFont::getStringTexture(const string& str, bool vflip) const{
	vector<glyph> glyphs;
	vector<glm::vec2> glyphPositions;
	float height = 0;
	float width = 0;
	float lineWidth = 0;
	iterateString(str, 0, 0, vflip, [&](uint32_t c, ofVec2f pos){
		try{
			if (c != '\n') {
				auto g = loadGlyph(c);
				
				if (c == '\t'){
					auto temp = loadGlyph(' ');
					glyphs.push_back(temp);
				}else{
					glyphs.push_back(g);
				}
				 
				int x = pos.x + g.props.xmin;
				int y = pos.y;
				glyphPositions.emplace_back(x, y);
				
				if(c == '\t')lineWidth += g.props.advance + getGlyphProperties(' ').advance * spaceSize * TAB_WIDTH;
				else if(c == ' ')lineWidth += g.props.advance + getGlyphProperties(' ').advance * spaceSize;
				else if(isValidGlyph(c))lineWidth += g.props.advance + getGlyphProperties(' ').advance * (letterSpacing - 1.f);
				
				width = max(width, lineWidth);
				y += g.props.ymax;
				height = max(height, y + getLineHeight());
			}else{
				lineWidth = 0;
			}
		}catch(...){
		}
	});

	ofTexture tex;
	ofPixels totalPixels;
	totalPixels.allocate(width, height, OF_PIXELS_GRAY_ALPHA);
	//-------------------------------- clear data:
	totalPixels.set(0,255); // every luminance pixel = 255
	totalPixels.set(1,0);
	size_t i = 0;
	for(auto & g: glyphs){
		if(settings.direction == OF_TTF_LEFT_TO_RIGHT){
			g.pixels.blendInto(totalPixels, glyphPositions[i].x, glyphPositions[i].y + getLineHeight() + g.props.ymin + getDescenderHeight() );
		}else{
			g.pixels.blendInto(totalPixels, width-glyphPositions[i].x, glyphPositions[i].y + getLineHeight() + g.props.ymin + getDescenderHeight() );
		}
		i++;
		if(i==glyphPositions.size()){
			break;
		}
	}
	tex.allocate(totalPixels);
	return tex;
}

//-----------------------------------------------------------
void ofTrueTypeFont::drawString(const string &  c, float x, float y) const{

	if (!bLoadedOk){
		ofLogError("ofTrueTypeFont") << "drawString(): font not allocated";
		return;
	}

	ofGetCurrentRenderer()->drawString(*this,c,x,y);

}

//-----------------------------------------------------------
void ofTrueTypeFont::drawStringAsShapes(const string& str, float x, float y) const{
	if (!bLoadedOk){
		ofLogError("ofTrueTypeFont") << "drawStringAsShapes(): font not allocated: line " << __LINE__ << " in " << __FILE__;
		return;
	};

	//----------------------- error checking
	if (!settings.contours){
		ofLogError("ofTrueTypeFont") << "drawStringAsShapes(): contours not created for this font, call loadFont() with makeContours set to true";
		return;
	}

	iterateString(str,x,y,true,[&](uint32_t c, glm::vec2 pos){
		drawCharAsShape(c, pos.x, pos.y, ofIsVFlipped(), ofGetStyle().bFill);
	});
}

//-----------------------------------------------------------
std::size_t ofTrueTypeFont::getNumCharacters() const{
	return cps.size();
}
