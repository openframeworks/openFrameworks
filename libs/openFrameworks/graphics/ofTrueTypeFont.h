#pragma once


#include <vector>
#include "ofPoint.h"
#include "ofRectangle.h"
#include "ofConstants.h"
#include "ofPath.h"

//--------------------------------------------------
typedef struct {
	int value;
	int height;
	int width;
	int setWidth;
	int topExtent;
	int leftExtent;
	float tTex;
	float vTex;		//0-1 pct of bitmap...
	float xOff;
	float yOff;
} charProps;


typedef ofPath ofTTFCharacter;

//--------------------------------------------------
#define NUM_CHARACTER_TO_START		33		// 0 - 32 are control characters, no graphics needed.

class ofTrueTypeFont{

public:


	ofTrueTypeFont();
	virtual ~ofTrueTypeFont();
		
	// 			-- default, non-full char set, anti aliased:
	void 		loadFont(string filename, int fontsize, bool _bAntiAliased=true, bool _bFullCharacterSet=false, bool makeContours = false, bool simplifyAmt = 0.3);

	bool		bLoadedOk;
	bool 		bAntiAlised;
	bool 		bFullCharacterSet;

  	float 		getLineHeight();
  	void 		setLineHeight(float height);
	float 		getLetterSpacing();
	void 		setLetterSpacing(float spacing);
	float 		getSpaceSize();
	void 		setSpaceSize(float size);
	float 		stringWidth(string s);
	float 		stringHeight(string s);
	
	ofRectangle    getStringBoundingBox(string s, float x, float y);
	
	void 		drawString(string s, float x, float y);
	void		drawStringAsShapes(string s, float x, float y);
	
	int 		nCharacters;
	
	ofTTFCharacter getCharacterAsPoints(int character);

protected:
	vector <ofTTFCharacter> charOutlines;

	float 			lineHeight;
	float			letterSpacing;
	float			spaceSize;

	charProps 		* 	cps;			// properties for each character
	GLuint			*	texNames;		// textures for each character
	int				fontSize;
	bool			bMakeContours;

	void 			drawChar(int c, float x, float y);
	void			drawCharAsShape(int c, float x, float y);
	
	int 			ofNextPow2(int a);
	int				border, visibleBorder;
	string			filename;

private:
#ifdef TARGET_ANDROID
	friend void ofUnloadAllFontTextures();
	friend void ofReloadAllFontTextures();
#endif
	void		unloadTextures();
	void		reloadTextures();
};


