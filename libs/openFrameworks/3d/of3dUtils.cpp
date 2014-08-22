#include "of3dUtils.h"
#include "ofGraphics.h"
#include "of3dGraphics.h"


ofVboMesh & cachedAxesVbo(){
    static ofVboMesh * axis = new ofVboMesh(ofMesh::axis());
    return *axis;
}

/** @brief Draws x,y,z axes representing the current reference frame
 *  @detail Axes are drawn in red (+x), green (+y) and blue (+z)
 *	@param size size at which to draw the axes
 **/
void ofDrawAxis(float size) {
	if (ofGetGLProgrammableRenderer()){
		ofPushMatrix();
		ofScale(size, size,size);
		cachedAxesVbo().draw();
		ofPopMatrix();
	} else {
		ofPushStyle();
		ofSetLineWidth(3);
		
		// draw x axis
		ofSetColor(ofColor::red);
		ofLine(0, 0, 0, size, 0, 0);
		
		// draw y axis
		ofSetColor(ofColor::green);
		ofLine(0, 0, 0, 0, size, 0);
		
		// draw z axis
		ofSetColor(ofColor::blue);
		ofLine(0, 0, 0, 0, 0, size);
		ofPopStyle();
	}
}

//--------------------------------------------------------------
void ofDrawGrid(float scale, float ticks, bool labels, bool x, bool y, bool z) {
	
	ofColor c(255,0,0);
	
	ofPushStyle();
	
	if (x) {
		c.setHsb(0.0f, 200.0f, 255.0f);
		ofSetColor(c);
		ofDrawGridPlane(scale, ticks, labels);
	}
	if (y) {
		c.setHsb(255.0f / 3.0f, 200.0f, 255.0f);
		ofSetColor(c);
		ofPushMatrix();
		ofRotate(90, 0, 0, -1);
		ofDrawGridPlane(scale, ticks, labels);
		ofPopMatrix();
	}
	if (z) {
		c.setHsb(255.0f * 2.0f / 3.0f, 200.0f, 255.0f);
		ofSetColor(c);
		ofPushMatrix();
		ofRotate(90, 0, 1, 0);
		ofDrawGridPlane(scale, ticks, labels);
		ofPopMatrix();
	}
	
	if (labels) {
		ofPushStyle();
		ofSetColor(255, 255, 255);
		float labelPos = scale * (1.0f + 0.5f / ticks);
		ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
		ofDrawBitmapString("x", labelPos, 0, 0);
		ofDrawBitmapString("y", 0, labelPos, 0);
		ofDrawBitmapString("z", 0, 0, labelPos);
		ofPopStyle();
	}
	ofPopStyle();
}


//--------------------------------------------------------------
void ofDrawGridPlane(float scale, float ticks, bool labels) {
	
	float minor = scale / ticks;
	float major =  minor * 2.0f;
	
	ofPushStyle();
	for (int iDimension=0; iDimension<2; iDimension++)
	{
		for (float yz=-scale; yz<=scale; yz+= minor)
		{
			//major major
			if (fabs(yz) == scale || yz == 0)
				ofSetLineWidth(2);
			
			//major
			else if (yz / major == floor(yz / major) )
				ofSetLineWidth(1.5);
			
			//minor
			else
				ofSetLineWidth(1);
			if (iDimension==0)
				ofLine(0, yz, -scale, 0, yz, scale);
			else
				ofLine(0, -scale, yz, 0, scale, yz);
		}
	}
	ofPopStyle();
	
	if (labels) {
		//draw numbers on axes
		ofPushStyle();
		ofSetColor(255, 255, 255);
		
		float accuracy = ceil(-log(scale/ticks)/log(10.0f));
		
		ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
		for (float yz = -scale; yz<=scale; yz+=minor)
		{
			ofDrawBitmapString(ofToString(yz, accuracy), 0, yz, 0);
			ofDrawBitmapString(ofToString(yz, accuracy), 0, 0, yz);		
		}
		ofPopStyle();
	}
	
}

//--------------------------------------------------------------
void ofDrawArrow(const ofVec3f& start, const ofVec3f& end, float headSize) {
	
	//draw line
	ofLine(start, end);
	
	//draw cone
	ofMatrix4x4 mat;
	mat.makeRotationMatrix( ofVec3f(0,1,0), start - end );
	ofPushMatrix();
	ofTranslate(end);
	ofMultMatrix(mat.getPtr());
	ofTranslate(0, headSize*0.5 ,0);
    ofDrawCone(headSize, headSize*2.);
	ofPopMatrix();
}
//--------------------------------------------------------------
void ofDrawRotationAxes(float radius, float stripWidth, int circleRes){
	
	ofMesh axisXMesh;
	axisXMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	
	ofMesh axisYMesh;
	axisYMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	
	ofMesh axisZMesh;
	axisZMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	
	for (int j = 0; j<=circleRes; j++) {
		float x = cos(TWO_PI * j/circleRes);
		float y = sin(TWO_PI * j/circleRes);
		axisXMesh.addColor(ofFloatColor(ofFloatColor::red));
		axisXMesh.addVertex(ofVec3f(x*radius, y*radius, -stripWidth));
		axisXMesh.addColor(ofFloatColor(ofFloatColor::red));
		axisXMesh.addVertex(ofVec3f(x*radius, y*radius,  stripWidth));
		
		axisYMesh.addColor(ofFloatColor(ofFloatColor::blue));
		axisYMesh.addVertex(ofVec3f(x*radius, -stripWidth, y*radius));
		axisYMesh.addColor(ofFloatColor(ofFloatColor::blue));
		axisYMesh.addVertex(ofVec3f(x*radius,  stripWidth, y*radius));
		
		axisZMesh.addColor(ofFloatColor(ofFloatColor::green));
		axisZMesh.addVertex(ofVec3f(-stripWidth, x*radius, y*radius));
		axisZMesh.addColor(ofFloatColor(ofFloatColor::green));
		axisZMesh.addVertex(ofVec3f( stripWidth, x*radius, y*radius));
	}
	
	ofPushStyle();
	ofEnableDepthTest();
	axisXMesh.draw();
	axisYMesh.draw();
	axisZMesh.draw();
	ofDrawAxis(radius);
	ofPopStyle();
	
}

