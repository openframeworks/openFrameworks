#include "ofxSvgFontBook.h"

using std::string;

std::string ofxSvgFontBook::mFontDirectory;
ofTrueTypeFont ofxSvgFontBook::defaultFont;
std::map< string, ofxSvgFontBook::Font > ofxSvgFontBook::fonts;

ofxSvgFontBook::Font ofxSvgFontBook::defaultBookFont;

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
	//    cout << "checking directory: " << fdirectory+"/fonts/" << endl;
		std::string fontsDirectory = "";// = ofToDataPath("", true);
		if( !aDirectory.empty() ) {
			fontsDirectory = aDirectory.string();
		}
		
		if( !ofFile::doesFileExist(fontsDirectory)) {
			if( ofFile::doesFileExist(mFontDirectory)) {
				fontsDirectory = mFontDirectory;
			}
		}
		
//		if( !ofFile::doesFileExist(fontsDirectory)) {
////			fs::path fontPath = fs::path(getenv("HOME")) / "Library" / "Fonts";
//			#if defined(TARGET_OSX)
//			std::filesystem::path fontPath = std::filesystem::path(getenv("HOME")) / "Library" / "Fonts";
//			if( ofDirectory::doesDirectoryExist(fontPath) ) {
//				fontsDirectory = ofToDataPath(fontPath, true);
//			}
//			#endif
//		}
		if( !ofFile::doesFileExist( fontsDirectory )) {
			fontsDirectory = ofToDataPath("", true);
		}
		
		if( ofFile::doesFileExist( fontsDirectory )) {
			bHasFontDirectory = true;
		}
		
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
		


//                string _filename, int _fontSize, bool _bAntiAliased, bool _bFullCharacterSet, bool _makeContours, float _simplifyAmt, int _dpi
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
			
			ofLogVerbose("ofxSvgFontBook") << __FUNCTION__ << " : " << fs.str() << " : starting off searching directory : " << fontsDirectory;
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
			for( auto& fontFam : fontNamesToSearch ) {
				bool bFoundTheFont = _recursiveFontDirSearch(fontsDirectory, fontFam, tNewFontPath, subStrs, excludeStrs, 0);
				if (bFoundTheFont) {
					tfontPath = tNewFontPath;
					bMightHaveFoundTheFont = true;
					ofLogVerbose("ofxSvgFontBook") << "Found the font at " << tfontPath;
					break;
				}
			}
			
			if( !bMightHaveFoundTheFont ) {
				// search the system level //
				#if defined(TARGET_OSX)
				std::filesystem::path fontPath = std::filesystem::path(getenv("HOME")) / "Library" / "Fonts";
				if( ofDirectory::doesDirectoryExist(fontPath) ) {
					fontsDirectory = ofToDataPath(fontPath, true);
					
					for( auto& fontFam : fontNamesToSearch ) {
						bool bFoundTheFont = _recursiveFontDirSearch(fontsDirectory, fontFam, tNewFontPath, subStrs, excludeStrs, 0);
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
					fontsDirectory = ofToDataPath(fontPath, true);
					
					for( auto& fontFam : fontNamesToSearch ) {
						bool bFoundTheFont = _recursiveFontDirSearch(fontsDirectory, fontFam, tNewFontPath, subStrs, excludeStrs, 0);
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

			/*ofDirectory tfDir;
			tfDir.listDir( fontsDirectory );
			for( int ff = 0; ff < tfDir.size(); ff++ ) {
				ofFile tfFile = tfDir.getFile(ff);
				if( tfFile.getExtension() == "ttf" || tfFile.getExtension() == "otf" ) {
					cout << ff << " - font family: " << tfont.fontFamily << " file name: " << tfFile.getBaseName() << endl;
					if( ofToLower(tfFile.getBaseName()) == ofToLower(tfont.fontFamily) ) {
						ofLogNotice(" >> ofxSvgText found font file for " ) << tfont.fontFamily;
						tfontPath = tfFile.getAbsolutePath();
						break;
					}
				}
			}*/
		}
		
		

		ofLogVerbose("ofxSvgFontBook") << __FUNCTION__ << " : Trying to load font from: " << tfontPath;

		if (tfontPath == "") {
			bFontLoadOk = false;
		} else {
			// load(const std::string& _filename, int _fontSize, bool _bAntiAliased, bool _bFullCharacterSet, bool _makeContours, float _simplifyAmt, int _dpi)
			bFontLoadOk = tfont.sizes[fontSize].load(tfontPath, fontSize, true, true, false, 0.5, 72);
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
		if ( tfFile.getExtension() == "ttf" || tfFile.getExtension() == "otf") {
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
