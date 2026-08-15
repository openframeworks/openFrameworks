#pragma once

#include "ofMain.h"

// Shared by every platform target of glRenderSmokeTest (desktop, Emscripten,
// iOS, Android). Exercises the code paths touched by the OpenGL ES 3.0/3.1
// support work: indexed ofVboMesh draw + drawInstanced (drawElements /
// drawElementsInstanced), a tessellated ofPath (tess2 / ofIndexType), and an
// ofFbo round trip, on whatever GL/GLES context the host window created.
//
// No custom shader is used deliberately: the GLSL version/#define story
// differs per platform (desktop core profile vs GLES2 vs GLES3 "300 es" vs
// Emscripten) and isn't what this PR touches. Using the renderer's own
// default shader keeps the test focused on the index-type and tess2 paths
// while still exercising the ES-version #ifdef branches in
// ofGLProgrammableRenderer indirectly (every draw call goes through them).
class GLSmokeTestCore {
public:
	static constexpr int framesToRun = 12;

	void allocate() {
		release();

		indexedMesh.setMode(OF_PRIMITIVE_TRIANGLES);
		indexedMesh.addVertices({
			{-60.f, -60.f, 0.f}, {60.f, -60.f, 0.f},
			{60.f, 60.f, 0.f}, {-60.f, 60.f, 0.f}
		});
		indexedMesh.addColors({ofColor::red, ofColor::green, ofColor::blue, ofColor::yellow});
		indexedMesh.addIndices({0, 1, 2, 0, 2, 3});

		tessellatedPath.clear();
		tessellatedPath.setFilled(true);
		tessellatedPath.setFillColor(ofColor(255, 140, 20));
		tessellatedPath.setPolyWindingMode(OF_POLY_WINDING_ODD);
		for (int i = 0; i < 12; ++i) {
			const float radius = (i % 2 == 0) ? 55.f : 22.f;
			const float angle = ofDegToRad(i * 30.f - 90.f);
			const glm::vec2 point(std::cos(angle) * radius, std::sin(angle) * radius);
			if (i == 0) {
				tessellatedPath.moveTo(point);
			} else {
				tessellatedPath.lineTo(point);
			}
		}
		tessellatedPath.close();

		ofFbo::Settings settings;
		settings.width = 128;
		settings.height = 128;
		settings.internalformat = GL_RGBA;
		settings.useDepth = false;
		settings.numSamples = 0;
		fbo.allocate(settings);

		allocated = fbo.isAllocated();
		if (!allocated) {
			fail("ofFbo failed to allocate");
		}
	}

	void release() {
		fbo.clear();
		allocated = false;
	}

	void drawFrame() {
		if (!allocated) {
			fail("resources not allocated");
			return;
		}

		clearGLErrors();

		fbo.begin();
		ofClear(0, 0, 0, 255);
		ofSetColor(255);
		ofPushMatrix();
		ofTranslate(fbo.getWidth() * 0.5f, fbo.getHeight() * 0.5f);
		indexedMesh.draw();                          // ofGLProgrammableRenderer::draw(ofVboMesh) -> drawElements
		indexedMesh.drawInstanced(OF_MESH_FILL, 2);   // forces the drawElementsInstanced path (primCount > 1)
		ofPopMatrix();
		fbo.end();
		checkGLErrors("after mesh draw");

		ofSetColor(255);
		fbo.draw(0.f, 0.f, static_cast<float>(fbo.getWidth()), static_cast<float>(fbo.getHeight()));

		ofPushMatrix();
		ofTranslate(180.f, 160.f);
		tessellatedPath.draw();                       // tess2 triangulation -> ofIndexType-sized indices
		ofPopMatrix();
		checkGLErrors("after path draw");

		if (frame == framesToRun - 1) {
			verifyPixels();
		}
		++frame;
	}

	bool finished() const {
		return frame >= framesToRun;
	}

	bool passed() const {
		return allocated && glErrorCount == 0 && pixelCheckPassed && failureReason.empty();
	}

	std::string resultLine() const {
		if (passed()) {
			return "GL_SMOKE_TEST RESULT=PASS frames=" + ofToString(frame);
		}
		std::string reason = failureReason;
		if (reason.empty() && glErrorCount > 0) {
			reason = "glGetError seen " + ofToString(glErrorCount) + " time(s), last=" + ofToHex(lastGLError);
		}
		if (reason.empty() && !pixelCheckPassed) {
			reason = "FBO readback did not contain the expected non-background color";
		}
		return "GL_SMOKE_TEST RESULT=FAIL reason=" + reason;
	}

private:
	void fail(const std::string & reason) {
		if (failureReason.empty()) {
			failureReason = reason;
		}
		ofLogError("glRenderSmokeTest") << reason;
	}

	void clearGLErrors() {
		while (glGetError() != GL_NO_ERROR) {
		}
	}

	void checkGLErrors(const std::string & where) {
		GLenum error = glGetError();
		while (error != GL_NO_ERROR) {
			++glErrorCount;
			lastGLError = error;
			ofLogError("glRenderSmokeTest") << "glGetError=" << ofToHex(error) << " " << where;
			error = glGetError();
		}
	}

	void verifyPixels() {
		ofPixels pixels;
		fbo.readToPixels(pixels);
		if (!pixels.isAllocated()) {
			fail("FBO readToPixels produced no pixels");
			return;
		}
		const ofColor center = pixels.getColor(pixels.getWidth() / 2, pixels.getHeight() / 2);
		// Background is cleared to black; any of the mesh's red/green/blue/yellow
		// vertex colors interpolating in means the indexed draw actually reached
		// the GPU with correctly-sized indices instead of reading garbage/nothing.
		pixelCheckPassed = (center.r > 10 || center.g > 10 || center.b > 10);
		if (!pixelCheckPassed) {
			ofLogError("glRenderSmokeTest") << "center pixel=" << center << " (expected non-background)";
		}
	}

	ofVboMesh indexedMesh;
	ofPath tessellatedPath;
	ofFbo fbo;

	bool allocated = false;
	bool pixelCheckPassed = false;
	int frame = 0;
	int glErrorCount = 0;
	GLenum lastGLError = GL_NO_ERROR;
	std::string failureReason;
};
