/*
 * ofMatrixStack.h
 *
 *  Created on: Apr 3, 2013
 *      Author: arturo
 */

#ifndef OFMATRIXSTACK_H_
#define OFMATRIXSTACK_H_

#include <stack>
#include "ofConstants.h"
#include "ofRectangle.h"
#include "ofGraphics.h"
#include "ofMatrix4x4.h"

class ofAppBaseWindow;
class ofFbo;

class ofMatrixStack {
public:
	ofMatrixStack(const ofAppBaseWindow & window);

	void setRenderSurface(const ofFbo & fbo);
	void setRenderSurface(const ofAppBaseWindow & window);

	void setOrientation(ofOrientation orientation, bool vFlip);
	ofOrientation getOrientation() const;

	void viewport(float x = 0, float y = 0, float width = -1, float height = -1, bool vflip=ofIsVFlipped());
	void nativeViewport(ofRectangle viewport);
	ofRectangle getCurrentViewport();
	ofRectangle getNativeViewport();

	const ofMatrix4x4 & getProjectionMatrix() const;
	const ofMatrix4x4 & getModelViewMatrix() const;
	const ofMatrix4x4 & getModelViewProjectionMatrix() const;
	const ofMatrix4x4 & getTextureMatrix() const;
	const ofMatrix4x4 & getCurrentMatrix() const;
	const ofMatrix4x4 & getProjectionMatrixNoOrientation() const;
	const ofMatrix4x4 & getOrientationMatrix() const;
	const ofMatrix4x4 & getOrientationMatrixInverse() const;

	ofMatrixMode getCurrentMatrixMode() const;

	ofHandednessType getHandedness() const;

	bool isVFlipped() const;
	bool customMatrixNeedsFlip() const;

	void pushView();
    void popView();

	void pushMatrix();
	void popMatrix();
	void translate(float x, float y, float z = 0);
	void scale(float xAmnt, float yAmnt, float zAmnt = 1);
	void rotate(float degrees, float vecX, float vecY, float vecZ);
	void matrixMode(ofMatrixMode mode);
	void loadIdentityMatrix (void);
	void loadMatrix (const float * m);
	void multMatrix (const float * m);

	void clearStacks();


private:
	bool vFlipped;
    ofOrientation orientation;
	ofRectangle currentViewport;
	ofHandednessType handedness;
	ofFbo * currentFbo;
	ofAppBaseWindow * currentWindow;

    ofMatrixMode currentMatrixMode;

	ofMatrix4x4	modelViewMatrix;
	ofMatrix4x4	projectionMatrix;
	ofMatrix4x4	textureMatrix;
	ofMatrix4x4 modelViewProjectionMatrix;
	ofMatrix4x4 orientedProjectionMatrix;
	ofMatrix4x4 orientationMatrix;
	ofMatrix4x4 orientationMatrixInverse;

	ofMatrix4x4 * currentMatrix;

	stack <ofRectangle> viewportHistory;
	stack <ofMatrix4x4> modelViewMatrixStack;
	stack <ofMatrix4x4> projectionMatrixStack;
	stack <ofMatrix4x4> textureMatrixStack;
	stack <pair<ofOrientation,bool> > orientationStack;

	int getRenderSurfaceWidth() const;
	int getRenderSurfaceHeight() const;
	bool doesHWOrientation() const;
	void updatedRelatedMatrices();

};

#endif /* OFMATRIXSTACK_H_ */
