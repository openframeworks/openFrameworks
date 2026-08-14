#include "ofxSvgFontBook.h"

using std::string;

std::string ofxSvgFontBook::mFontDirectory;
ofTrueTypeFont ofxSvgFontBook::defaultFont;
std::map< string, ofxSvgFontBook::Font > ofxSvgFontBook::fonts;

ofxSvgFontBook::Font ofxSvgFontBook::defaultBookFont;



// Convert UTF-8 string to Unicode codepoints
std::vector<uint32_t> ofxSvgFontBook::_utf8ToCodepoints(const std::string& str) {
	std::vector<uint32_t> codepoints;
	size_t i = 0;
	while (i < str.size()) {
		uint32_t cp = 0;
		unsigned char c = str[i];
		if (c < 0x80) {
			cp = c; i++;
		} else if (c < 0xE0) {
			cp = (c & 0x1F) << 6 | (str[i+1] & 0x3F); i += 2;
		} else if (c < 0xF0) {
			cp = (c & 0x0F) << 12 | (str[i+1] & 0x3F) << 6 | (str[i+2] & 0x3F); i += 3;
		} else {
			cp = (c & 0x07) << 18 | (str[i+1] & 0x3F) << 12 | (str[i+2] & 0x3F) << 6 | (str[i+3] & 0x3F); i += 4;
		}
		codepoints.push_back(cp);
	}
	return codepoints;
}

ofxSvgFontBook::TextLanguage ofxSvgFontBook::detectLanguage(const std::string& utf8Text) {
	auto codepoints = _utf8ToCodepoints(utf8Text);
	// TODO: Add more language support.
	int japaneseScore = 0;
	int koreanScore   = 0;
	int chineseScore  = 0;
	
	for (uint32_t cp : codepoints) {
		// Hiragana — exclusively Japanese
		if (cp >= 0x3040 && cp <= 0x309F) japaneseScore += 3;
		// Katakana — exclusively Japanese
		else if (cp >= 0x30A0 && cp <= 0x30FF) japaneseScore += 3;
		// Hangul — exclusively Korean
		else if (cp >= 0xAC00 && cp <= 0xD7AF) koreanScore += 3;
		else if (cp >= 0x1100 && cp <= 0x11FF) koreanScore += 3;
		// CJK Unified Ideographs — shared, but count for Chinese
		// Japanese kanji will still trip this, so it's a weak signal
		else if (cp >= 0x4E00 && cp <= 0x9FFF) chineseScore += 1;
		// CJK Extension A/B
		else if (cp >= 0x3400 && cp <= 0x4DBF) chineseScore += 1;
		else if (cp >= 0x20000 && cp <= 0x2A6DF) chineseScore += 1;
		// Bopomofo — exclusively Traditional Chinese
		else if (cp >= 0x02EA && cp <= 0x02EB) chineseScore += 3;
		else if (cp >= 0x3100 && cp <= 0x312F) chineseScore += 3;
	}
	
	if (japaneseScore == 0 && koreanScore == 0 && chineseScore == 0) {
		return TextLanguage::ENGLISH;
	}
	
	// Japanese and Korean have exclusive characters so trust high scores
	if (japaneseScore > koreanScore && japaneseScore > chineseScore) return TextLanguage::JAPANESE;
	if (koreanScore > japaneseScore && koreanScore > chineseScore)   return TextLanguage::KOREAN;
	if (chineseScore > 0)                                             return TextLanguage::CHINESE;
	
	return TextLanguage::OTHER;
}

//--------------------------------------------------------------
bool ofxSvgFontBook::loadFont(const std::string& aFontFamily, int aFontSize, bool aBBold, bool aBItalic ) {
    return loadFont(mFontDirectory, aFontFamily, aFontSize, aBBold, aBItalic );
}

//--------------------------------------------------------------
bool ofxSvgFontBook::loadFont(const of::filesystem::path& aDirectory, const std::string& aFontFamily, int aFontSize, bool aBBold, bool aBItalic ) {
	ofxSvgCssClass css;
	css.setFontFamily(aFontFamily);
	css.setFontSize(aFontSize);
	css.setFontBold(aBBold);
	css.setFontItalic(aBItalic);
	return loadFont( aDirectory, css );
}

//--------------------------------------------------------------
bool ofxSvgFontBook::loadFont(const of::filesystem::path& aDirectory, ofxSvgCssClass& aCssClass ) {
	// create a ttf settings temp
	ofTrueTypeFontSettings fsettings(aDirectory,aCssClass.getFontSize(12));
	fsettings.contours = false;
	return loadFont( aDirectory, aCssClass, fsettings );
}

//--------------------------------------------------------------
bool ofxSvgFontBook::loadFont( ofxSvgCssClass& aCssClass, ofxSvgFontBook::TextLanguage alanguage ) {
	// create a ttf settings temp
	ofTrueTypeFontSettings fsettings(mFontDirectory,aCssClass.getFontSize(12));
	fsettings.contours = false;
	
//	auto detectedLang = ofxSvgFontBook::detectLanguage(tempStr);
	if( alanguage == ofxSvgFontBook::TextLanguage::JAPANESE ) {
		fsettings.addRanges(ofAlphabet::Japanese);
	} else if( alanguage == ofxSvgFontBook::TextLanguage::CHINESE ) {
		fsettings.addRanges(ofAlphabet::Chinese);
	} else if( alanguage == ofxSvgFontBook::TextLanguage::KOREAN ) {
		fsettings.addRanges(ofAlphabet::Korean);
	}
	
	return loadFont( mFontDirectory, aCssClass, fsettings );
}

//--------------------------------------------------------------
bool ofxSvgFontBook::loadFont(const of::filesystem::path& aDirectory, ofxSvgCssClass& aCssClass, ofTrueTypeFontSettings aFontSettings ) {
	auto fontFamily = aCssClass.getFontFamily("Arial");
	auto fontSize = aCssClass.getFontSize(12);
	bool bBold = aCssClass.isFontBold();
	bool bItalic = aCssClass.isFontItalic();
	
	ofLogVerbose("ofxFontBook") << "checking font: " << fontFamily << " bold: " << bBold << " italic: " << bItalic << " fkey: ";
	auto fkey = ofxSvgFontBook::getFontKey(fontFamily, bBold, bItalic );
	
	ofLogVerbose("ofxFontBook") << "checking font: " << fontFamily << " bold: " << bBold << " italic: " << bItalic;
	
	if( fonts.count(fkey) == 0 ) {
		Font tafont;
		tafont.fontFamily = fontFamily;
		tafont.bold = bBold;
		tafont.italic = bItalic;
		fonts[fkey] = tafont;
	}
	bool bFontLoadOk = true;

	Font& tfont = fonts[ fkey ];
	if (tfont.sizes.count(fontSize) == 0) {
		bool bHasFontDirectory = false;
		
		std::vector<std::string> fontsDirsToSearch;
		
		if( !mFontDirectory.empty() ) {
			if( ofFile::doesFileExist(mFontDirectory)) {
				fontsDirsToSearch.push_back(ofFilePath::removeTrailingSlash(mFontDirectory) );
			}
		}
		
		
		if( !aDirectory.empty() ) {
			if( ofDirectory::doesDirectoryExist(ofFilePath::removeTrailingSlash(aDirectory))) {
				fontsDirsToSearch.push_back(ofFilePath::removeTrailingSlash(aDirectory) );
			}
		}
		
		
		
		if( fontsDirsToSearch.size() < 1 ) {
			fontsDirsToSearch.push_back( ofToDataPath("", true) );
		}
		
		bHasFontDirectory = fontsDirsToSearch.size() > 0;
		
		std::vector<std::string> fontNamesToSearch = {fontFamily};
		// sometimes there are fallback fonts included with a comma separator
		if( ofIsStringInString(fontFamily, ",")) {
			std::vector<std::string> splitNames = ofSplitString(fontFamily, ",", true, true);
			for( auto& sname : splitNames ) {
				// remove spaces
				ofStringReplace(sname, " ", "" );
			}
//			fontNamesToSearch.insert(fontNamesToSearch.end(), splitNames.begin(), splitNames.end());
			fontNamesToSearch = splitNames;
		}
		


//      string _filename, int _fontSize, bool _bAntiAliased, bool _bFullCharacterSet, bool _makeContours, float _simplifyAmt, int _dpi
		// first let's see if the fonts are provided. Some system fonts are .dfont that have several of the faces
		// in them, but OF isn't setup to parse them, so we need each bold, regular, italic, etc to be a .ttf font //
		string tfontPath = tfont.fontFamily;
		if (bHasFontDirectory) {
			std::stringstream fs;
			bool bf = true;
			for( auto& fname : fontNamesToSearch ) {
				if( !bf ) {
					fs << ", ";
				}
				fs << fname;
				bf = false;
			}
			
			string tNewFontPath = "";

			std::vector<std::string> subStrs;
			std::vector<std::string> excludeStrs;
			if( bBold ) {
				subStrs.push_back("bold");
			} else {
				excludeStrs.push_back("bold");
			}
			if( bItalic ) {
				subStrs.push_back("italic");
			} else {
				excludeStrs.push_back("italic");
			}
			
			bool bMightHaveFoundTheFont = false;
			ofLogVerbose("ofxSvgFontBook") << "trying to load font: " << tfont.fontFamily << " bold: " << bBold << " italic: " << bItalic;
//			bool bFoundTheFont = _recursiveFontDirSearch(fontsDirectory, tfont.fontFamily, tNewFontPath, subStrs, excludeStrs, 0);
			for( auto& fontDir : fontsDirsToSearch ) {
				ofLogVerbose("ofxSvgFontBook") << __FUNCTION__ << " : " << fs.str() << " : starting off searching directory : " << fontDir;
				for( auto& fontFam : fontNamesToSearch ) {
					bool bFoundTheFont = _recursiveFontDirSearch(fontDir, fontFam, tNewFontPath, subStrs, excludeStrs, 0);
					if (bFoundTheFont) {
						tfontPath = tNewFontPath;
						bMightHaveFoundTheFont = true;
						ofLogVerbose("ofxSvgFontBook") << "Found the font at " << tfontPath;
						break;
					}
				}
			}
			
			if( !bMightHaveFoundTheFont ) {
				// search the system level //
				#if defined(TARGET_OSX)
				std::filesystem::path fontPath = std::filesystem::path(getenv("HOME")) / "Library" / "Fonts";
				if( ofDirectory::doesDirectoryExist(fontPath) ) {
//					fontsDirectory = ofToDataPath(fontPath, true);
					
					for( auto& fontFam : fontNamesToSearch ) {
						bool bFoundTheFont = _recursiveFontDirSearch(ofToDataPath(fontPath, true), fontFam, tNewFontPath, subStrs, excludeStrs, 0);
						if (bFoundTheFont) {
							tfontPath = tNewFontPath;
							bMightHaveFoundTheFont = true;
							ofLogVerbose("ofxSvgFontBook") << "Found the font at " << tfontPath;
							break;
						}
					}
					
				}
				#endif
			}
			
			if( !bMightHaveFoundTheFont ) {
				// search the system level //
				#if defined(TARGET_OSX)
				std::filesystem::path fontPath = "/Library/Fonts";
				if( ofDirectory::doesDirectoryExist(fontPath) ) {
//					fontsDirectory = ofToDataPath(fontPath, true);
					
					for( auto& fontFam : fontNamesToSearch ) {
						bool bFoundTheFont = _recursiveFontDirSearch(ofToDataPath(fontPath, true), fontFam, tNewFontPath, subStrs, excludeStrs, 0);
						if (bFoundTheFont) {
							tfontPath = tNewFontPath;
							bMightHaveFoundTheFont = true;
							ofLogVerbose("ofxSvgFontBook") << "Found the font at " << tfontPath;
							break;
						}
					}
					
				}
				#endif
			}
		}
		
		

		ofLogVerbose("ofxSvgFontBook") << __FUNCTION__ << " : Trying to load font from: " << tfontPath << " adirectory: " << aDirectory;

		if (tfontPath == "") {
			bFontLoadOk = false;
		} else {
			// load(const std::string& _filename, int _fontSize, bool _bAntiAliased, bool _bFullCharacterSet, bool _makeContours, float _simplifyAmt, int _dpi)
//			bFontLoadOk = tfont.sizes[fontSize].load(tfontPath, fontSize, true, true, false, 0.5, 72);
			
			ofTrueTypeFontSettings fsettings = aFontSettings;
			fsettings.fontName = tfontPath;
			fsettings.fontSize = fontSize;
			fsettings.dpi = aFontSettings.dpi == 0 ? 72 : aFontSettings.dpi;
			fsettings.addRange(ofUnicode::Latin1Supplement);
			fsettings.addRange(ofUnicode::Latin);
			fsettings.addRange(ofUnicode::GeneralPunctuation);
			bFontLoadOk = tfont.sizes[fontSize].load(fsettings);
			
			ofLogVerbose("ofxSvgFontBook") << __FUNCTION__ << " : loaded font ("<<bFontLoadOk<<") from: " << tfontPath;
			if( bFontLoadOk && tfont.pathToFont.empty() ) {
				tfont.pathToFont = tfontPath;
			}
		}
		if(bFontLoadOk) {
//                    tfont.sizes[ vIt->first ].setSpaceSize( 0.57 );
//                    tfont.sizes[ vIt->first ]       = datFontTho;
			tfont.textures[ fontSize ] = tfont.sizes[ fontSize ].getFontTexture();
		} else {
			ofLogError("ofxSvgFontBook") << __FUNCTION__ << " : error loading font family: " << tfont.fontFamily << " size: " << fontSize;
			tfont.sizes.erase(fontSize);
		}
	}
	return bFontLoadOk;
}

//--------------------------------------------------------------
bool ofxSvgFontBook::_recursiveFontDirSearch(const string& afile, const string& aFontFamToLookFor, string& fontpath,
											 const std::vector<std::string>& aAddNames,
											 const std::vector<std::string>& aExcludeNames,
											 int aNumRecursions) {
	if (fontpath != "") {
		return true;
	}
	int numRecursions = aNumRecursions+1;
	if( numRecursions > 20 ) {
		ofLogVerbose("ofxSvgFontBook") << __FUNCTION__ << " too many recursions, aborting.";
		return false;
	}
	ofFile tfFile( afile, ofFile::Reference );
	if (tfFile.isDirectory()) {
		ofLogVerbose("ofxSvgFontBook") << __FUNCTION__ << " : searching in directory : " << afile << " | " << ofGetFrameNum();
		ofDirectory tdir;
		tdir.listDir(afile);
		tdir.sort();
		for (std::size_t i = 0; i < tdir.size(); i++) {
			bool bFontFound = _recursiveFontDirSearch(tdir.getPath(i), aFontFamToLookFor, fontpath, aAddNames, aExcludeNames, numRecursions);
            if( bFontFound ) {
                return true;
            }
		}
		tdir.close();
	} else {
		if ( tfFile.getExtension() == "ttf" || tfFile.getExtension() == "otf" || tfFile.getExtension() == "ttc") {
			auto tfbase = ofToLower(tfFile.getBaseName());
			auto fontFamLower = ofToLower(aFontFamToLookFor);
			
			if( aAddNames.size() > 0 || aExcludeNames.size() > 0 ) {
				
				if(ofIsStringInString(tfbase, fontFamLower)) {
					bool bAllFound = true;
					for( auto& subName : aAddNames ) {
						if( !ofIsStringInString(tfbase, ofToLower(subName))) {
//							ofLogNotice("ofxSvgFontBook") << __FUNCTION__ << " add name not found: " << subName << " font: " << fontFamLower;
							bAllFound = false;
							break;
						}
					}
					if(aExcludeNames.size() > 0 ) {
						for( auto& subName : aExcludeNames ) {
							if( ofIsStringInString(tfbase, ofToLower(subName))) {
//								ofLogNotice("ofxSvgFontBook") << __FUNCTION__ << " exclude name not found: " << subName << " font: " << fontFamLower;
								bAllFound = false;
								break;
							}
						}
					}
					
//					ofLogNotice("ofxSvgFontBook") << __FUNCTION__ << " checking font fam: " << fontFamLower << " file: " << tfbase << " allfound: " << bAllFound;
					
					if(bAllFound) {
						fontpath = tfFile.getAbsolutePath();
						return true;
					}
				}
			} else {
				if (ofToLower( tfFile.getBaseName() ) == fontFamLower) {
					ofLogVerbose("ofxSvgFontBook") << __FUNCTION__ << " : found font file for " << aFontFamToLookFor;
					fontpath = tfFile.getAbsolutePath();
					return true;
				}
				string tAltFileName = ofToLower(tfFile.getBaseName());
				ofStringReplace(tAltFileName, " ", "-");
				if (tAltFileName == fontFamLower) {
					ofLogVerbose("ofxSvgFontBook") << __FUNCTION__ << " : found font file for " << aFontFamToLookFor;
					fontpath = tfFile.getAbsolutePath();
					return true;
				}
			}
		}
	}
    return false;
}

//--------------------------------------------------------------
ofTrueTypeFont& ofxSvgFontBook::getFontForKey( const std::string& aFontKey, int aFontSize ) {
    if( fonts.count(aFontKey) > 0 ) {
        if( fonts[aFontKey].sizes.count(aFontSize) > 0 ) {
            return fonts[aFontKey].sizes[aFontSize];
        }
    }
    return defaultFont;
}
