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

const static string OF_TTF_SANS = "sans-serif";
const static string OF_TTF_SERIF = "serif";
const static string OF_TTF_MONO = "monospace";

typedef struct FT_FaceRec_*  FT_Face;

class ofTrueTypeFont{

public:


	ofTrueTypeFont();
	virtual ~ofTrueTypeFont();
	
	//set the default dpi for all typefaces.
	static void setGlobalDpi(int newDpi);
			
	// 			-- default (without dpi), non-full char set, anti aliased, 96 dpi:
	bool 		loadFont(string filename, int fontsize, bool _bAntiAliased=true, bool _bFullCharacterSet=false, bool makeContours=false, float simplifyAmt=0.3, int dpi=0);
	
	bool		isLoaded();
	bool		isAntiAliased();
	bool		hasFullCharacterSet();

    int         getSize();
    float       getLineHeight();
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

	vector<charProps> 	cps;			// properties for each characterIndex

	int				fontSize;
	bool			bMakeContours;
	float 			simplifyAmt;
	int 			dpi;

    int             getKerning(int c, int prevC);
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
	FT_Face		face;
	void		unloadTextures();
	void		reloadTextures();
	static bool	initLibraries();
	static void finishLibraries();

	friend void ofExitCallback();
};


