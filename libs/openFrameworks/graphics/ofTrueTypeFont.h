#ifndef _OF_TTF_FONT_H_
#define _OF_TTF_FONT_H_


#include "ofConstants.h"
#include "ofGraphics.h"
#include "ofUtils.h"
#include "ofTypes.h"

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

//For drawStringAsShapes and getCharacterAsPoints only
//Make this number smaller to create TTF shapes with more pts = slower but accurate
//Make this number larger to create TTF shapes with less pts  = faster but not as accurate
//Our default is 0.3 which removes segments that are less than 0.3 of a pixel in length
#define TTF_SHAPE_SIMPLIFICATION_AMNT (0.3)

class ofTTFContour{
	public:
		vector <ofPoint>pts;
};


class ofTTFCharacter{
	public:
		vector <ofTTFContour> contours;
};

//--------------------------------------------------
#define NUM_CHARACTER_TO_START		33		// 0 - 32 are control characters, no graphics needed.

class ofTrueTypeFont{

public:


	ofTrueTypeFont();
	virtual ~ofTrueTypeFont();
		
	// 			-- default, non-full char set, anti aliased:
	void 		loadFont(string filename, int fontsize);
	void 		loadFont(string filename, int fontsize, bool _bAntiAliased, bool _bFullCharacterSet, bool makeContours = false);

	bool		bLoadedOk;
	bool 		bAntiAlised;
	bool 		bFullCharacterSet;

  	float 		getLineHeight();
  	void 		setLineHeight(float height);
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
	charProps 		* 	cps;			// properties for each character
	GLuint			*	texNames;		// textures for each character
	int				fontSize;
	bool			bMakeContours;

	void 			drawChar(int c, float x, float y);
	void			drawCharAsShape(int c, float x, float y);
	
	int 			ofNextPow2(int a);
	int				border, visibleBorder;


};


#endif

