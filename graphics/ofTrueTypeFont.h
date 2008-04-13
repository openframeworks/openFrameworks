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

/*
i tried to solve this problem by replacing
cps[i].height = face->glyph->bitmap_top;
with
cps[i].height = face->glyph->bitmap.rows;
in ofTrueTypeFont::loadFont

that solved the bouding-box problem but broke the drawString function of course.
so i think in charProps the member height should be renamed to top and
a new member height should be set to bitmap_rows
stringHeight can then calculate the right bouding box. a stringTop function could also be introduced. 
*/

//--------------------------------------------------
#define NUM_CHARACTER_TO_START		33		// 0 - 32 are control characters, no graphics needed.




class ofTrueTypeFont{

public:


	ofTrueTypeFont();
	~ofTrueTypeFont();
	
	// 			-- default, non-full char set, anti aliased:
	void 		loadFont(string filename, int fontsize);
	void 		loadFont(string filename, int fontsize, bool _bAntiAliased, bool _bFullCharacterSet);

	bool		bLoadedOk;
	bool 		bAntiAlised;
	bool 		bFullCharacterSet;

  	float 		getLineHeight();
  	void 		setLineHeight(float height);
	float 		stringWidth(string s);
	float 		stringHeight(string s);
	
	ofRectangle    getStringBoundingBox(string s, float x, float y);
	
	void 		drawString(string s, float x, float y);
	int 		nCharacters;

protected:

	float 			lineHeight;
	charProps 		* 	cps;			// properties for each character
	GLuint			*	texNames;		// textures for each character
	int				fontSize;

	void 			drawChar(int c, float x, float y);
	int 			ofNextPow2(int a);
	int				border, visibleBorder;


};


#endif

