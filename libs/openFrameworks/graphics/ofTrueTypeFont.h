#pragma once

#include <vector>
#include "ofPoint.h"
#include "ofRectangle.h"
#include "ofConstants.h"
#include "ofPath.h"
#include "ofTexture.h"
#include "ofMesh.h"

//--------------------------------------------------
typedef struct {
	int character;
	int height;
	int width;
	int setWidth;
	int topExtent;
	int leftExtent;
	float tW,tH;
	float x1,x2,y1,y2;
	float t1,t2,v1,v2;
} charProps;


typedef ofPath ofTTFCharacter;

//--------------------------------------------------
#define NUM_CHARACTER_TO_START		33		// 0 - 32 are control characters, no graphics needed.

/// \todo?
const static string OF_TTF_SANS = "sans-serif";
const static string OF_TTF_SERIF = "serif";
const static string OF_TTF_MONO = "monospace";

/// \todo
class ofTrueTypeFont{

public:


	/// \todo
	ofTrueTypeFont();

	/// \todo
	virtual ~ofTrueTypeFont();
	
	// set the default dpi for all typefaces.
	/// \todo
	static void setGlobalDpi(int newDpi);
			
	/// \brief Loads the font specified by filename, allows you to control size, aliasing, and other parameters.
	///
	/// loads a font, and allows you to set the following parameters: the filename, the size, if the font is anti-aliased,
	/// if it has a full character set, if you need it to have contours (for getStringPoints) and paramters that control 
	/// the simplification amount for those contours and the dpi of the font.
	/// 
	/// default (without dpi), non-full char set, anti aliased, 96 dpi
    ///
	/// \param filename The name of the font file to load.
    /// \param fontsize The size in pixels to load the font.
    /// \param _bAntiAliased true iff the font should be anti-aliased.
    /// \param _bFullCharacterSet true iff the full character set should be cached.
    /// \param makeControus true iff the vector contours should be cached.
    /// \param simplifyAmt the amount to simplify the vector contours.  Larger number means more simplified.
    /// \param dpi the dots per inch used to specify rendering size.
	/// \returns true if the font was loaded correctly.
	bool loadFont(string filename,
                  int fontsize,
                  bool _bAntiAliased=true,
                  bool _bFullCharacterSet=false,
                  bool makeContours=false,
                  float simplifyAmt=0.3,
                  int dpi=0);
	
	/// \brief Has the font been loaded successfully?
	/// \returns true if the font was loaded.
	bool isLoaded();
	
	/// \brief Is the font anit-aliased?
	/// \returns true if the font was set to be anti-aliased.
	bool isAntiAliased();

	/// \brief Does the font have a full character set?
	/// \returns true if the font was allocated with a full character set.
	bool hasFullCharacterSet();

	/// \brief Returns the size of the font.
	/// \returns Size of font, set when font was loaded.
	int getSize();
	
	/// \brief Computes line height based on font size.
	/// \returns Returns current line height.
	float getLineHeight();

	/// \brief Sets line height for text drawn on screen. 
	///
	/// Note the line height is automatically computed based on the font size, when you load in the font.
	///
	/// \param height Line height for text drawn on screen.
	void setLineHeight(float height);

	/// \brief Returns letter spacing of font object.
	///
	/// You can control this by the ofTrueTypeFont::setLetterSpacing() function. 1.0 = default spacing, 
	/// less then 1.0 would be tighter spacing, greater then 1.0 would be wider spacing.
	///
	/// \returns Returns letter spacing of font object.
	float getLetterSpacing();

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
	/// \returns Returns a variable that represents how wide spaces are.
	float getSpaceSize();

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
	/// \returns Returns the string width. 
	float stringWidth(string s);

	/// \brief Returns the string height.
	///
	/// This is essentially the height component of the ofTrueTypeFont::getStringBoundingBox() rectangle.
	///
	/// \param s The string to get the height of.
	/// \returns Returns the string height. 
	float stringHeight(string s);

	/// \brief Returns the bounding box of a string as a rectangle.
	/// \param s The string to get bounding box of.
	/// \param x X position of returned rectangle.
	/// \param y Y position of returned rectangle.
	/// \returns Returns the bounding box of a string as a rectangle.
	ofRectangle getStringBoundingBox(string s, float x, float y);

	/// \brief Draw a string s at position x,y
	/// \param s String to draw
	/// \param x X position of string
	/// \param y Y position of string
	void drawString(string s, float x, float y);

	/// \brief Draws the string as if it was geometrical shapes.
	/// 
	/// Uses the information contained in ofTTFContour and ofTTFCharacter.
	/// 
	/// \param x X position of shapes
	/// \param y Y position of shapes
	void drawStringAsShapes(string s, float x, float y);

	/// \brief Get the num chars in the loaded character set.
	/// 
	/// If you allocate the font using different paramters, you can load in partial 
	/// and full character sets, this helps you know how many characters it can represent.
	///
	/// \returns Number of characters in loaded character set.
	int	getNumCharacters();	
	
	/// \todo
	ofTTFCharacter getCharacterAsPoints(int character, bool vflip=ofIsVFlipped());
	vector<ofTTFCharacter> getStringAsPoints(string str, bool vflip=ofIsVFlipped());
	ofMesh & getStringMesh(string s, float x, float y);
	ofTexture & getFontTexture();

	/// \todo
	void bind();
	void unbind();

	/// \brief Get the current font endcoding.
	/// 
	/// This is set by ofTrueTypeFont::setEncoding() to either OF_ENCODING_UTF8 or 
	/// OF_ENCODING_ISO_8859_15. OF_ENCODING_ISO_8859_15 is for an 8-bit single-byte
	/// coded graphic character sets, like ASCII while OF_ENCODING_UTF8 is a variable-width 
	/// encoding that can represent every character in the Unicode character set.
	///
	/// \returns encoding used by the font object.
	ofTextEncoding getEncoding() const;
	
	/// \brief Sets the current font encoding.
	///
	/// Can be set to either OF_ENCODING_UTF8 or OF_ENCODING_ISO_8859_15. OF_ENCODING_ISO_8859_15 
	/// is for an 8-bit single-byte coded graphic character sets, like ASCII while OF_ENCODING_UTF8 
	/// is a variable-width encoding that can represent every character in the Unicode character set. 
	/// This function is useful if you are trying to draw unicode strings.
	///
	/// \param encoding The encoding used by the font object, either OF_ENCODING_UTF8 or 
	/// \param OF_ENCODING_ISO_8859_15
	void setEncoding(ofTextEncoding encoding);

protected:
	bool bLoadedOk;
	bool bAntiAliased;
	bool bFullCharacterSet;
	int nCharacters;
	
	vector <ofTTFCharacter> charOutlines;
	vector <ofTTFCharacter> charOutlinesNonVFlipped;

	float lineHeight;
	float letterSpacing;
	float spaceSize;

	vector<charProps> cps; // properties for each character

	int fontSize;
	bool bMakeContours;
	float simplifyAmt;
	int dpi;

	void drawChar(int c, float x, float y);
	void drawCharAsShape(int c, float x, float y);
	void createStringMesh(string s, float x, float y);
	
	int border;
	string filename;

	ofTexture texAtlas;
	bool binded;
	ofMesh stringQuads;

private:
#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
	friend void ofUnloadAllFontTextures();
	friend void ofReloadAllFontTextures();
#endif

	GLint blend_src, blend_dst;
	GLboolean blend_enabled;
	GLboolean texture_2d_enabled;

	ofTextEncoding encoding;
	void unloadTextures();
	void reloadTextures();
	static bool	initLibraries();
	static void finishLibraries();

	friend void ofExitCallback();
};


