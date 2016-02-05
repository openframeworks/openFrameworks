#pragma once

#include <vector>
#include "ofPoint.h"
#include "ofRectangle.h"
#include "ofConstants.h"
#include "ofPath.h"
#include "ofTexture.h"
#include "ofMesh.h"

/// \file
/// The ofTrueTypeFont class provides an interface to load fonts into
/// openFrameworks. The fonts are converted to textures, and can be drawn on
/// screen. There are some options when you load the font - what size the
/// font is rendered at, whether or not it is anti-aliased, and whether the
/// font object will be the full character set or a subset (i.e., extended
/// ASCII, which can include diacritics like umlauts, or ASCII). The default
/// is anti-aliased, non-full character set. The library uses freetype, which
/// has certain patent problems in regards to true type hinting, especially
/// at small sizes, so non-anti-aliased type doesn't always render
/// beautifully. But we find it quite adequate, and at larger sizes it seems
/// to works well.


/// \cond INTERNAL

//--------------------------------------------------
typedef struct {
	int characterIndex;
	int glyph;
	int height;
	int width;
	int bearingX, bearingY;
	int xmin, xmax, ymin, ymax;
	int advance;
	float tW,tH;
	float t1,t2,v1,v2;
} charProps;


typedef ofPath ofTTFCharacter;

//--------------------------------------------------
#define NUM_CHARACTER_TO_START		32		// 0 - 32 are control characters, no graphics needed.


typedef struct FT_FaceRec_*  FT_Face;

/// \endcond

/// \name Fonts
/// \{
static const string OF_TTF_SANS = "sans-serif";
static const string OF_TTF_SERIF = "serif";
static const string OF_TTF_MONO = "monospace";
/// \}


void ofTrueTypeShutdown();



class ofTrueTypeFont{

public:


	/// \brief Construct a default ofTrueTypeFont.
	ofTrueTypeFont();

	/// \brief Destroy the ofTrueTypeFont.
	virtual ~ofTrueTypeFont();

    ofTrueTypeFont(const ofTrueTypeFont& mom);
    ofTrueTypeFont & operator=(const ofTrueTypeFont& mom);

    ofTrueTypeFont(ofTrueTypeFont&& mom);
    ofTrueTypeFont & operator=(ofTrueTypeFont&& mom);

	/// \name Load Font
	/// \{
				
	/// \brief Loads the font specified by filename, allows you to control size, aliasing, and other parameters.
	///
	/// loads a font, and allows you to set the following parameters: the filename, the size, if the font is anti-aliased,
	/// if it has a full character set, if you need it to have contours (for getStringPoints) and parameters that control 
	/// the simplification amount for those contours and the dpi of the font.
	/// 
	/// default (without dpi), non-full char set, anti aliased, 96 dpi
    ///
	/// \param filename The name of the font file to load.
    /// \param fontsize The size in pixels to load the font.
    /// \param _bAntiAliased true if the font should be anti-aliased.
    /// \param _bFullCharacterSet true if the full character set should be cached.
    /// \param makeContours true if the vector contours should be cached.
    /// \param simplifyAmt the amount to simplify the vector contours.  Larger number means more simplified.
    /// \param dpi the dots per inch used to specify rendering size.
	/// \returns true if the font was loaded correctly.
	bool load(const std::string& filename,
                  int fontsize,
                  bool _bAntiAliased=true,
                  bool _bFullCharacterSet=true,
                  bool makeContours=false,
                  float simplifyAmt=0.3f,
				  int dpi=0);

	OF_DEPRECATED_MSG("Use load instead",bool loadFont(string filename,
                  int fontsize,
                  bool _bAntiAliased=true,
                  bool _bFullCharacterSet=false,
                  bool makeContours=false,
                  float simplifyAmt=0.3f,
				  int dpi=0));
	
	/// \brief Has the font been loaded successfully?
	/// \returns true if the font was loaded.
	bool isLoaded() const;

	/// \}
	/// \name Font Settings
	/// \{
	
	/// \brief Set the default dpi for all typefaces.
	static void setGlobalDpi(int newDpi);
	
	/// \brief Is the font anti-aliased?
	/// \returns true if the font was set to be anti-aliased.
	bool isAntiAliased() const;

	/// \brief Does the font have a full character set?
	/// \returns true if the font was allocated with a full character set.
	bool hasFullCharacterSet() const;
	
	/// \brief Get the number of characters in the loaded character set.
	/// 
	/// If you allocate the font using different parameters, you can load in partial 
	/// and full character sets, this helps you know how many characters it can represent.
	///
	/// \returns Number of characters in loaded character set.
	int	getNumCharacters() const;

	/// \}
	/// \name Font Size
	/// \{

	/// \brief Returns the size of the font.
	/// \returns Size of font, set when font was loaded.
	int getSize() const;
	
	/// \brief Computes line height based on font size.
	/// \returns the current line height.
	float getLineHeight() const;

	/// \brief Sets line height for text drawn on screen. 
	///
	/// Note the line height is automatically computed based on the font size, when you load in the font.
	///
	/// \param height Line height for text drawn on screen.
	void setLineHeight(float height);

	/// \brief Get the ascender distance for this font.
	///
	/// The ascender is the vertical distance from the baseline to the highest "character" coordinate.
	/// The meaning of "character" coordinate depends on the font. Some fonts take accents into account,
	/// others do not, and still others define it simply to be the highest coordinate over all glyphs.
	///
	/// \returns the font ascender height in pixels.
	float getAscenderHeight() const;

	/// \brief Get the descender distance for this font.
	///
	/// The descender is the vertical distance from the baseline to the lowest "character" coordinate.
	/// The meaning of "character" coordinate depends on the font. Some fonts take accents into account,
	/// others do not, and still others define it simply to be the lowest coordinate over all glyphs.
	/// This value will be negative for descenders below the baseline (which is typical).
	///
	/// \returns the font descender height in pixels.
	float getDescenderHeight() const;

	/// \brief Get the global bounding box for this font.
	///
	/// The global bounding box is the rectangle inside of which all glyphs in the font can fit.
    /// Glyphs are drawn starting from (0,0) in the returned box (though note that the box can
    /// extend in any direction out from the origin).
    ///
	/// \returns the font descender height in pixels.
    const ofRectangle & getGlyphBBox() const;

	/// \brief Returns letter spacing of font object.
	///
	/// You can control this by the ofTrueTypeFont::setLetterSpacing() function. 1.0 = default spacing, 
	/// less then 1.0 would be tighter spacing, greater then 1.0 would be wider spacing.
	///
	/// \returns the letter spacing of font object.
	float getLetterSpacing() const;

	/// \brief Sets the letter spacing of the font object.
	/// 
	/// 1.0 = default spacing, less then 1.0 would be tighter spacing, greater then 1.0 would be wider spacing.
	/// \param spacing Spacing of font object. 
	void setLetterSpacing(float spacing);

	/// \brief Returns a variable that represents how wide spaces are.
	///
	/// It's a scalar for the width of the letter 'p', so 1.0 means that a space will be the size of the lower 
	/// case 'p' of that font. 2.0 means that it's 2 times the size of the lower case 'p', etc.
	///
	/// \returns the width of the space.
	float getSpaceSize() const;

	/// \brief Sets the size of the space ' ' character. 
	/// 
	/// This number, which defaults to 1.0, scales the width of the letter 'p' for the space.
	///
	/// \param size Scales the width of the letter 'p' for the space. 
	void setSpaceSize(float size);

	/// \brief Returns the string width.
	///
	/// This is essentially the width component of the ofTrueTypeFont::getStringBoundingBox() rectangle.
	///
	/// \param s The string to get the width of.
	/// \returns the string width. 
	float stringWidth(const std::string& s) const;

	/// \brief Returns the string height.
	///
	/// This is essentially the height component of the ofTrueTypeFont::getStringBoundingBox() rectangle.
	///
	/// \param s The string to get the height of.
	/// \returns the string height.
	float stringHeight(const std::string& s) const;

	/// \brief Returns the bounding box of a string as a rectangle.
	/// \param s The string to get bounding box of.
	/// \param x X position of returned rectangle.
	/// \param y Y position of returned rectangle.
	/// \returns the bounding box of a string as a rectangle.
	ofRectangle getStringBoundingBox(const std::string& s, float x, float y, bool vflip=true) const;

	/// \}
	/// \name Drawing
	/// \{

	/// \brief Draw a string s at position x,y
	/// \param s String to draw
	/// \param x X position of string
	/// \param y Y position of string
	void drawString(const std::string& s, float x, float y) const;

	/// \brief Draws the string as if it was geometrical shapes.
	/// 
	/// Uses the information contained in ofTTFContour and ofTTFCharacter.
	/// 
	/// \param x X position of shapes
	/// \param y Y position of shapes
	void drawStringAsShapes(const std::string& s, float x, float y) const;

	/// \todo Documentation.
	ofTTFCharacter getCharacterAsPoints(int character, bool vflip=true, bool filled=true) const;
	vector<ofTTFCharacter> getStringAsPoints(const std::string& str, bool vflip=true, bool filled=true) const;
	const ofMesh & getStringMesh(const std::string& s, float x, float y, bool vflip=true) const;
	const ofTexture & getFontTexture() const;

	/// \}
	
protected:
	/// \cond INTERNAL
	
	bool bLoadedOk;
	bool bAntiAliased;
	bool bFullCharacterSet;
	int nCharacters;
	
	vector <ofTTFCharacter> charOutlines;
	vector <ofTTFCharacter> charOutlinesNonVFlipped;
	vector <ofTTFCharacter> charOutlinesContour;
	vector <ofTTFCharacter> charOutlinesNonVFlippedContour;

	float lineHeight;
	float ascenderHeight;
	float descenderHeight;
	ofRectangle glyphBBox;
	float letterSpacing;
	float spaceSize;

	vector<charProps> cps; // properties for each character

	int fontSize;
	bool bMakeContours;
	float simplifyAmt;
	int dpi;


    int getKerning(int c, int prevC) const;
	void drawChar(int c, float x, float y, bool vFlipped) const;
	void drawCharAsShape(int c, float x, float y, bool vFlipped, bool filled) const;
	void createStringMesh(const std::string& s, float x, float y, bool vFlipped) const;
	
	std::string filename;

	ofTexture texAtlas;
	mutable ofMesh stringQuads;
	bool useKerning;

	/// \endcond

private:
#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
	friend void ofUnloadAllFontTextures();
	friend void ofReloadAllFontTextures();
#endif
    std::shared_ptr<FT_FaceRec_> face;
	void		unloadTextures();
	void		reloadTextures();
	static bool	initLibraries();
	static void finishLibraries();

	friend void ofExitCallback();
};


