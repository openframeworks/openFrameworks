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

const static string OF_TTF_SANS = "sans-serif";
const static string OF_TTF_SERIF = "serif";
const static string OF_TTF_MONO = "monospace";

class ofTrueTypeFont{

public:


	ofTrueTypeFont();
	virtual ~ofTrueTypeFont();
	
	//set the default dpi for all typefaces.
	static void setGlobalDpi(int newDpi);
			
	// 			-- default (without dpi), non-full char set, anti aliased, 96 dpi:
	/// \brief Loads the font specified by filename, allows you to control size, aliasing, and other parameters.
	///
	/// loads a font, and allows you to set the following parameters: the filename, the size, if the font is anti-aliased,
	/// if it has a full character set, if you need it to have contrours (for getStringPoints) and paramters that control 
	/// the simplification amount for those contours and the dpi of the font.
	/// 
	/// \returns Returns true if the font was loaded correctly.
	bool 		loadFont(string filename, int fontsize, bool _bAntiAliased=true, bool _bFullCharacterSet=false, bool makeContours=false, float simplifyAmt=0.3, int dpi=0);
	/// \brief Returns true if the font is loaded properly.
	/// \returns Returns true if the font is loaded properly.
	bool		isLoaded();
	/// \brief Returns true if the font was set to be anti-aliased.
	/// \returns Returns true if the font was set to be anti-aliased.
	bool		isAntiAliased();
	/// \brief Returns true if the font was allocated with a full character set.
	/// \returns Returns true if the font was allocated with a full character set.
	bool		hasFullCharacterSet();

	/// \brief Returns the size of the font.
	/// \returns Size of font, set when font was loaded.
        int             getSize();
        /// \brief Computes line height based on font size
        /// \returns Returns current line height
        float           getLineHeight();
        /// \brief Sets line height for text drawn on screen. 
        ///
        /// Note the line height is automatically computed based on the font size, when you load in the font.
	///
	/// \param height Line height for text drawn on screen
  	void 		setLineHeight(float height);
  	/// \brief Returns letter spacing of font object.
  	///
  	/// You can control this by the ofTrueTypeFont::setLetterSpacing() function. 1.0 = default spacing, 
  	/// less then 1.0 would be tighter spacing, greater then 1.0 would be wider spacing.
  	///
  	/// \returns Returns letter spacing of font object.
	float 		getLetterSpacing();
	/// \brief Sets the letter spacing of the font object.
	/// 
	/// 1.0 = default spacing, less then 1.0 would be tighter spacing, greater then 1.0 would be wider spacing.
	/// \param spacing Spacing of font object. 
	void 		setLetterSpacing(float spacing);
	/// \brief Returns a variable that represents how wide spaces are.
	///
	/// It's a scalar for the width of the letter 'p', so 1.0 means that a space will be the size of the lower 
	/// case 'p' of that font. 2.0 means that it's 2 times the size of the lower case 'p', etc.
	///
	/// \returns Returns a variable that represents how wide spaces are.
	float 		getSpaceSize();
	/// \brief Sets the size of the space ' ' character. 
	/// 
	/// This number, which defaults to 1.0, scales the width of the letter 'p' for the space.
	///
	/// \param size Scales the width of the letter 'p' for the space. 
	void 		setSpaceSize(float size);
	/// \brief Returns the string width.
	///
	/// This is essentially the width component of the ofTrueTypeFont::getStringBoundingBox() rectangle.
	///
	/// \param s The string to get the width of.
	/// \returns Returns the string width. 
	float 		stringWidth(string s);
	/// \brief Returns the string height.
	///
	/// This is essentially the height component of the ofTrueTypeFont::getStringBoundingBox() rectangle.
	///
	/// \param s The string to get the height of.
	/// \returns Returns the string height. 
	float 		stringHeight(string s);
	
	ofRectangle    getStringBoundingBox(string s, float x, float y);
	
	void 		drawString(string s, float x, float y);
	void		drawStringAsShapes(string s, float x, float y);
	
	//			get the num chars in the loaded char set
	int			getNumCharacters();	
	
	ofTTFCharacter getCharacterAsPoints(int character, bool vflip=ofIsVFlipped());
	vector<ofTTFCharacter> getStringAsPoints(string str, bool vflip=ofIsVFlipped());
	ofMesh & getStringMesh(string s, float x, float y);
	ofTexture & getFontTexture();

	void bind();
	void unbind();

	ofTextEncoding getEncoding() const;
	void setEncoding(ofTextEncoding encoding);

protected:
	bool			bLoadedOk;
	bool 			bAntiAliased;
	bool 			bFullCharacterSet;
	int 			nCharacters;
	
	vector <ofTTFCharacter> charOutlines;
	vector <ofTTFCharacter> charOutlinesNonVFlipped;

	float 			lineHeight;
	float			letterSpacing;
	float			spaceSize;

	vector<charProps> 	cps;			// properties for each character

	int				fontSize;
	bool			bMakeContours;
	float 			simplifyAmt;
	int 			dpi;

	void 			drawChar(int c, float x, float y);
	void			drawCharAsShape(int c, float x, float y);
	void			createStringMesh(string s, float x, float y);
	
	int				border;//, visibleBorder;
	string			filename;

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
	void		unloadTextures();
	void		reloadTextures();
	static bool	initLibraries();
	static void finishLibraries();

	friend void ofExitCallback();
};


