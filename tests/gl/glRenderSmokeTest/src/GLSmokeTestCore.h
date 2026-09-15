#pragma once

#include "ofMain.h"

// Shared by desktop / Emscripten / iOS. Indexed VBO + tess2 ofPath + FBO
// readback on ES1, ES2, ES3 and desktop GL. Instancing only when the context
// actually has it (GL 3.1+ / ES 3.0+); ES1/ES2/GL 2.1 skip that call.
class GLSmokeTestCore {
public:
	static constexpr int framesToRun = 12;

	void allocate() {
		release();
		usedInstancing = false;
		detectAPI();

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

		ofLogNotice("glRenderSmokeTest") << "api=" << apiLabel()
			<< " programmable=" << (ofIsGLProgrammableRenderer() ? 1 : 0)
			<< " instancing=" << (instancingAvailable ? 1 : 0)
			<< " ofIndexTypeBytes=" << sizeof(ofIndexType);
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
		indexedMesh.draw();
		if (instancingAvailable) {
			indexedMesh.drawInstanced(OF_MESH_FILL, 2);
			usedInstancing = true;
		}
		ofPopMatrix();
		fbo.end();
		checkGLErrors("after mesh draw");

		ofSetColor(255);
		fbo.draw(0.f, 0.f, static_cast<float>(fbo.getWidth()), static_cast<float>(fbo.getHeight()));

		ofPushMatrix();
		ofTranslate(180.f, 160.f);
		tessellatedPath.draw();
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
		const std::string suffix = " frames=" + ofToString(frame)
			+ " api=" + apiLabel()
			+ " instancing=" + ofToString(usedInstancing ? 1 : 0);
		if (passed()) {
			return "GL_SMOKE_TEST RESULT=PASS" + suffix;
		}
		std::string reason = failureReason;
		if (reason.empty() && glErrorCount > 0) {
			reason = "glGetError seen " + ofToString(glErrorCount) + " time(s), last=" + ofToHex(lastGLError);
		}
		if (reason.empty() && !pixelCheckPassed) {
			reason = "FBO readback did not contain the expected non-background color";
		}
		return "GL_SMOKE_TEST RESULT=FAIL reason=" + reason + suffix;
	}

private:
	void detectAPI() {
		glMajor = 0;
		glMinor = 0;
		instancingAvailable = false;
		auto renderer = ofGetGLRenderer();
		if (renderer) {
			glMajor = renderer->getGLVersionMajor();
			glMinor = renderer->getGLVersionMinor();
		}
#ifdef TARGET_OPENGLES
		instancingAvailable = glMajor >= 3;
#else
		instancingAvailable = ofIsGLProgrammableRenderer() && (glMajor > 3 || (glMajor == 3 && glMinor >= 1));
#endif
	}

	std::string apiLabel() const {
#ifdef TARGET_OPENGLES
		return "ES" + ofToString(glMajor) + "." + ofToString(glMinor);
#else
		return std::string(ofIsGLProgrammableRenderer() ? "GL" : "GL-FF") + ofToString(glMajor) + "." + ofToString(glMinor);
#endif
	}

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
	bool instancingAvailable = false;
	bool usedInstancing = false;
	int glMajor = 0;
	int glMinor = 0;
	int frame = 0;
	int glErrorCount = 0;
	GLenum lastGLError = GL_NO_ERROR;
	std::string failureReason;
};
