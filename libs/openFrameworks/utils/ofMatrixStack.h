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
#include "glm/mat4x4.hpp"
#include "ofGraphicsConstants.h"

class ofBaseDraws;
class ofAppBaseWindow;
class ofFbo;
enum ofOrientation: short;

class ofMatrixStack {
public:
	ofMatrixStack(const ofAppBaseWindow * window);

	void setRenderSurface(const ofBaseDraws & fbo);
	void setRenderSurfaceNoMatrixFlip(const ofBaseDraws & fbo);
	void setRenderSurface(const ofAppBaseWindow & window);

	void setOrientation(ofOrientation orientation, bool vFlip);
	ofOrientation getOrientation() const;

	void viewport(float x, float y, float width, float height, bool vflip);
	void nativeViewport(ofRectangle viewport);
	ofRectangle getCurrentViewport() const;
	ofRectangle getNativeViewport() const;
	ofRectangle getFullSurfaceViewport() const;

	const glm::mat4 & getProjectionMatrix() const;
	const glm::mat4 & getViewInverse() const;
	const glm::mat4 & getViewMatrix() const;
	const glm::mat4 & getModelMatrix() const;
	const glm::mat4 & getModelViewMatrix() const;
	const glm::mat4 & getModelViewProjectionMatrix() const;
	const glm::mat4 & getTextureMatrix() const;
	const glm::mat4 & getCurrentMatrix() const;
	const glm::mat4 & getProjectionMatrixNoOrientation() const;
	const glm::mat4 & getOrientationMatrix() const;
	const glm::mat4 & getOrientationMatrixInverse() const;

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
	void rotateRad(float radians, float vecX, float vecY, float vecZ);
	void matrixMode(ofMatrixMode mode);
	void loadIdentityMatrix (void);
	
	void loadMatrix (const glm::mat4 & m);
	void multMatrix (const glm::mat4 & m);

	void loadViewMatrix(const glm::mat4 & matrix);
	void multViewMatrix(const glm::mat4 & matrix);

	void clearStacks();
	bool doesHardwareOrientation() const;

private:
	bool vFlipped;
    ofOrientation orientation;
	ofRectangle currentViewport;
	ofHandednessType handedness;
	ofBaseDraws * currentRenderSurface;
	ofAppBaseWindow * currentWindow;

    ofMatrixMode currentMatrixMode;

	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 viewInverse;
	glm::mat4 modelViewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 textureMatrix;
	glm::mat4 modelViewProjectionMatrix;
	glm::mat4 orientedProjectionMatrix;
	glm::mat4 orientationMatrix;
	glm::mat4 orientationMatrixInverse;

	glm::mat4 * currentMatrix;

	std::stack <ofRectangle> viewportHistory;
	std::stack <glm::mat4> viewMatrixStack;
	std::stack <glm::mat4> modelViewMatrixStack;
	std::stack <glm::mat4> projectionMatrixStack;
	std::stack <glm::mat4> textureMatrixStack;
	std::stack <std::pair<ofOrientation,bool> > orientationStack;
	bool flipRenderSurfaceMatrix;

	int getRenderSurfaceWidth() const;
	int getRenderSurfaceHeight() const;
	bool doesHWOrientation() const;
	inline void updatedRelatedMatrices();

};

#endif /* OFMATRIXSTACK_H_ */
