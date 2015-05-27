#pragma once

#include "CreEPS.hpp"
#include "ofMain.h"

#include <vector>

	//to do:
	//background color?
	//ofNextContour
	//shape -winding mode

using namespace ns_creeps;

class ofxVectorGraphics{

		public:
			ofxVectorGraphics();

			//----------------------------------------------------------
			//only call these two functions when you are ready to capture your graphics to disk!!!
			void beginEPS(string fileName, int x = 0, int y = 0, int w = ofGetWidth(), int h = ofGetHeight() );
			void endEPS();

			//----------------------------------------------------------
			void enableDraw();
			void disableDraw();

			//----------------------------------------------------------
			void enableCenterRect();
			void disableCenterRect();

			//----------------------------------------------------------
			void fill();
			void noFill();

			//----------------------------------------------------------
			void setLineWidth(float lineWidth);

			//----------------------------------------------------------
			void setColor(int _r, int _g, int _b);
			void setColor(int hexColor);
			void changeColor(float r, float g, float b);

			//----------------------------------------------------------
			void rect(float x1,float y1, float w, float h);
			void triangle(float x1,float y1, float x2, float y2, float x3, float y3);
			void circle(float x1,float y1, float radius);
			void ellipse(float x1,float y1, float w, float h);
			void line(float x1,float y1, float x2, float y2);
			void arc(float x, float y, float radius, float offsetAngleDegrees, float internalAngleDegrees);
			void bezier(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
			void curve(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

			//----------------------------------------------------------
			void beginShape();
			void polyVertex(float x, float y);
			void bezierVertex(float x1, float y1, float x2, float y2, float x3, float y3);
			void curveVertex(float x, float y);
			void endShape(bool bClose = false);

			//the creeps object is left public
			//if people want to use more than the features
			//ofxVectorGraphics implements
			CreEPS creeps;

		protected:
			void clearAllVertices();

			bool bFill;
			bool bDraw;
			bool bCenter;
			bool bRecord;
			bool bShouldClose;
			bool bFirstPoint;
			int  whichShapeMode;

			vector<double *>curvePts;
};
