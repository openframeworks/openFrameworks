#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxFbo.h"
#include "ofxShader.h"


#define kFBOWidth			320
#define kFBOHeight			240


// used for screen layout
#define kPreviewWidth		320
#define kPreviewHeight		240


// auto calculated
#define kPreviewX(i)	((kPreviewWidth + 10) * i)
#define kPreviewY(i)	((kPreviewHeight + 60) * i)

#define kLabelX(i)		(kPreviewX(i) + 15)
#define kLabelY(i)		(kPreviewY(i) + kPreviewHeight + 15)


// DEMO MODES
// 1 - simple use, render to single texture in fbo, render that to screen with shader
// 2 - simple MRT, render same thing simultaneously to multiple textures in fbo
// 3 - GLSL MRT, render different things simultaneously to multiple textures in fbo
// 4 - pingpong use, render to single texture in fbo, then pingpong between two textures to blur


class testApp : public ofBaseApp{
	
public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);

	
	int				demoMode;
	bool			useMSAA;
	ofxFbo			fbo;
	ofxShader		shader;
	ofVideoGrabber	vidGrabber;
	

	void drawScene(float x, float y, float w, float h);
	
	void setupDemo(int i);
	
	inline void demo1_setup();
	inline void demo1_draw();
	
	inline void demo2_setup();
	inline void demo2_draw();

	inline void demo3_setup();
	inline void demo3_draw();

	inline void demo4_setup();
	inline void demo4_draw();
};

#endif

