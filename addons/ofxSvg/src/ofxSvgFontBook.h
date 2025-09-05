#pragma once
#include "ofTrueTypeFont.h"
#include <map>
#include "ofxSvgCss.h"

/// @brief  This class is mostly for internal use. 
/// Used by the ofxSvgText elements for managing fonts
class ofxSvgFontBook {
public:
    class Font {
    public:
        std::string fontFamily;
        std::map< int, ofTrueTypeFont > sizes;
        std::map< int, ofTexture > textures;
        bool bold = false;
        bool italic = false;
		of::filesystem::path pathToFont;
    };

    static void setFontDirectory( std::string adir ) {
        mFontDirectory = adir;
    }
	
	static bool loadFont(const std::string& aFontFamily, int aFontSize, bool aBBold, bool aBItalic );
    static bool loadFont(const of::filesystem::path& aDirectory, ofxSvgCssClass& aCssClass );
    static bool loadFont(const of::filesystem::path& aDirectory, const std::string& aFontFamily, int aFontSize, bool aBBold, bool aBItalic );

    static std::string getFontKey( const std::string& aFontFamily, bool aBBold, bool aBItalic ) {
        auto fkey = aFontFamily;
        if( aBBold ) {fkey += "_bold";}
        if( aBItalic) {fkey += "_italic";}
        return fkey;
    }

    static bool hasFont(const std::string& aFontFamily, int aFontSize, bool aBBold, bool aBItalic ) {
        auto fkey = getFontKey(aFontFamily, aBBold, aBItalic);
        return hasFontForKey(fkey, aFontSize);
    }

    static bool hasFontForKey(const std::string& aFontKey, int aFontSize ) {
        if( fonts.count(aFontKey) > 0 ) {
            auto& font = fonts[aFontKey];
            if( font.sizes.count(aFontSize) > 0 ) {
                return true;
            }
        }
        return false;
    }

    static bool hasBookFont( const std::string& aFontKey ) {
        return ( fonts.count(aFontKey) > 0 );
    }

    static Font& getBookFont( const std::string& aFontKey ) {
        if( fonts.count(aFontKey) > 0 ) {
            return fonts[aFontKey];
        }
        return defaultBookFont;
    }

    static ofTrueTypeFont& getFontForKey( const std::string& aFontKey, int aFontSize );
    
    static ofTrueTypeFont defaultFont;

protected:
    static Font defaultBookFont;
    static std::string mFontDirectory;
    static std::map< std::string, Font > fonts;
    static bool _recursiveFontDirSearch(const std::string& afile, const std::string& aFontFamToLookFor,
										std::string& fontpath,
                                        const std::vector<std::string>& aAddNames,
                                        const std::vector<std::string>& aExcludeNames,
										int aNumRecursions);
};
