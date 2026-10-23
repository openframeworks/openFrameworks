/***********************************************************************
 
 Copyright (C) 2011 by Zach Gage
 Copyright (C) 2015-2022 Dan Rosser - Updated for Various Apps and Games - Used in Super Hexagon

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 ************************************************************************/ 

/*
 ofxShapeBatchRenderer was written primarily for use on embedded devices as
 a deferred renderer for of shape calls.
 */

#ifndef OFX_SHAPE_BATCH_RENDERER_H
#define OFX_SHAPE_BATCH_RENDERER_H

#include "ofMain.h"

#define DEFAULT_LAYER 0
#define USING_VBO
// USE_DRAW_INSTANCED calls ofVboMesh::drawInstanced with numObjects*shapeMode.
// That repeats the already-batched mesh. The bench measures one draw().
// #define USE_DRAW_INSTANCED

enum SHAPEMODE_t {SBR_LINE=2, SBR_TRIANGLE=3}; //line or triangle based drawing.... essentially ofNoFill(), or ofFill();

//class -------------------------------------------
class ofxShapeBatchRenderer
{
public:
	ofxShapeBatchRenderer(SHAPEMODE_t mode, int _numObjectsPerLayer, int _numLayers);
	~ofxShapeBatchRenderer();
	
	//raw --
	//this can only be used if you're in triangle mode
	bool addTriangle(ofPoint a, ofPoint b, ofPoint c, int layer=DEFAULT_LAYER);
	bool addTriangle(ofPoint a, int aR, int aG, int aB, int aA, ofPoint b, int bR, int bG, int bB, int bA, ofPoint c, int cR, int cG, int cB, int cA, int layer=DEFAULT_LAYER);
	
	//this can only be used if you're in line mode
	bool addLine(ofPoint a, ofPoint b, int layer=DEFAULT_LAYER);
	bool addLine(ofPoint a, int aR, int aG, int aB, int aA, ofPoint b, int bR, int bG, int bB, int bA, int layer=DEFAULT_LAYER);

	
	//handled --
	bool addCircle(float x, float y, float z, float radius, int layer=DEFAULT_LAYER);
	bool addElipse(float x, float y, float z, float w, float h, int layer=DEFAULT_LAYER);
	
	bool addRect(float x, float y, float z, float w, float h, int layer=DEFAULT_LAYER);
	bool addCenteredRect(float x, float y, float z, float w, float h, int layer=DEFAULT_LAYER);
	
	//functional --
	void draw();
	void clear();
	
	//features --
	void setColor(int r, int g, int b, int a=255);
	void setCircleResolution(int resolution);
	void setLayerLineWidth(int layer, int lineWidth); //only relavent in SBR_LINE mode
	void setSafeMode(bool safe);
    
    void setupLayer(int layer);
		
protected:
	double * verts;
	unsigned char * colors;
		
	int * numObjects;
    int * numObjectsLast;
	int * layerLineWidth;
	
	int numLayers;
	int numObjectsPerLayer;
	
	bool bSetupCircle;
	
	bool safeMode;
				   
	SHAPEMODE_t shapeMode;
	
	int red;
	int green;
	int blue;
	int alpha;
	
	int	numCirclePts;
	float circlePts[OF_MAX_CIRCLE_PTS*2];

	#if defined(USING_VBO)
	vector<ofVboMesh*> meshes;
	#endif
};

#endif
