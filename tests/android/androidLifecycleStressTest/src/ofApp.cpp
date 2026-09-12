#include "ofApp.h"

namespace {
constexpr auto vertexShaderES3 = R"(
#version 300 es
uniform mat4 modelViewProjectionMatrix;
in vec4 position;
in vec4 color;
out vec4 vertexColor;
void main() {
	vertexColor = color;
	gl_Position = modelViewProjectionMatrix * position;
}
)";

constexpr auto fragmentShaderES3 = R"(
#version 300 es
precision highp float;
in vec4 vertexColor;
out vec4 outputColor;
void main() {
	outputColor = vertexColor;
}
)";

constexpr auto vertexShaderES2 = R"(
#version 100
uniform mat4 modelViewProjectionMatrix;
attribute vec4 position;
attribute vec4 color;
varying vec4 vertexColor;
void main() {
	vertexColor = color;
	gl_Position = modelViewProjectionMatrix * position;
}
)";

constexpr auto fragmentShaderES2 = R"(
#version 100
precision mediump float;
varying vec4 vertexColor;
void main() {
	gl_FragColor = vertexColor;
}
)";
}

void ofApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(60);
	ofBackground(12);

	indexedMesh.setMode(OF_PRIMITIVE_TRIANGLES);
	indexedMesh.addVertices({
		{-140.f, -100.f, 0.f}, {140.f, -100.f, 0.f},
		{140.f, 100.f, 0.f}, {-140.f, 100.f, 0.f}
	});
	indexedMesh.addColors({ofColor::red, ofColor::green, ofColor::blue, ofColor::yellow});
	indexedMesh.addIndices({0, 1, 2, 0, 2, 3});

	tessellatedPath.setFilled(true);
	tessellatedPath.setFillColor(ofColor(255, 120, 20, 210));
	tessellatedPath.setPolyWindingMode(OF_POLY_WINDING_ODD);
	for(int i = 0; i < 12; ++i) {
		const float radius = i % 2 == 0 ? 95.f : 38.f;
		const float angle = ofDegToRad(i * 30.f - 90.f);
		const glm::vec2 point(std::cos(angle) * radius, std::sin(angle) * radius);
		if(i == 0) tessellatedPath.moveTo(point);
		else tessellatedPath.lineTo(point);
	}
	tessellatedPath.close();

	allocateGLResources();
	logLifecycle("setup");
}

void ofApp::allocateGLResources() {
	releaseGLResources();

	ofFbo::Settings settings;
	settings.width = 256;
	settings.height = 256;
	settings.internalformat = GL_RGBA;
	settings.useDepth = true;
	settings.depthStencilAsTexture = false;
	settings.numSamples = 0;
	fbo.allocate(settings);

	int glesMajor = 2;
	if (ofGetGLRenderer()) {
		glesMajor = ofGetGLRenderer()->getGLVersionMajor();
	}
	usesShader = glesMajor >= 2;
	if (usesShader) {
		if (glesMajor >= 3) {
			shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShaderES3);
			shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderES3);
		} else {
			shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShaderES2);
			shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderES2);
		}
		shader.bindDefaults();
		resourcesAllocated = shader.linkProgram() && fbo.isAllocated();
	} else {
		resourcesAllocated = fbo.isAllocated();
	}
	ofLogNotice("androidLifecycleStressTest")
		<< "GL resources allocated=" << resourcesAllocated
		<< " gles=" << glesMajor
		<< " shader=" << usesShader
		<< " renderer=" << glGetString(GL_RENDERER)
		<< " version=" << glGetString(GL_VERSION);
}

void ofApp::releaseGLResources() {
	shader.unload();
	fbo.clear();
	resourcesAllocated = false;
}

void ofApp::update() {}

void ofApp::draw() {
	++renderedFrames;
	if(!resourcesAllocated) {
		ofSetColor(ofColor::red);
		ofDrawBitmapString("GL RESOURCES NOT ALLOCATED", 20, 30);
		return;
	}

	fbo.begin();
	ofClear(8, 16, 32, 255);
	ofPushMatrix();
	ofTranslate(fbo.getWidth() * 0.5f, fbo.getHeight() * 0.5f);
	ofRotateDeg(ofGetElapsedTimef() * 40.f);
	if (usesShader) {
		shader.begin();
	}
	indexedMesh.draw();
	if (usesShader) {
		shader.end();
	}
	ofPopMatrix();
	fbo.end();

	ofSetColor(255);
	fbo.draw(20, 50, 256, 256);
	ofPushMatrix();
	ofTranslate(ofGetWidth() * 0.68f, ofGetHeight() * 0.45f);
	ofRotateDeg(-ofGetElapsedTimef() * 25.f);
	tessellatedPath.draw();
	ofPopMatrix();

	ofSetColor(255);
	ofDrawBitmapStringHighlight(
		"GLES lifecycle stress\nframes: " + ofToString(renderedFrames) +
		"\npause/resume: " + ofToString(pauseCount) + "/" + ofToString(resumeCount) +
		"\nunload/reload GL: " + ofToString(unloadCount) + "/" + ofToString(reloadCount),
		20, ofGetHeight() - 100);

	const GLenum error = glGetError();
	if(error != GL_NO_ERROR) {
		ofLogError("androidLifecycleStressTest") << "glGetError=" << ofToHex(error)
			<< " frame=" << renderedFrames;
	}
	if(renderedFrames % 300 == 0) {
		ofLogNotice("androidLifecycleStressTest") << "heartbeat frame=" << renderedFrames;
	}
}

void ofApp::logLifecycle(const std::string & event) {
	ofLogNotice("androidLifecycleStressTest") << "lifecycle[" << ++lifecycleSequence
		<< "] " << event << " frame=" << renderedFrames;
}

void ofApp::pause() {
	++pauseCount;
	logLifecycle("pause");
}

void ofApp::resume() {
	++resumeCount;
	logLifecycle("resume");
}

void ofApp::stop() {
	logLifecycle("stop");
}

void ofApp::unloadGL() {
	++unloadCount;
	logLifecycle("unloadGL");
	releaseGLResources();
}

void ofApp::reloadGL() {
	++reloadCount;
	logLifecycle("reloadGL");
	allocateGLResources();
}

void ofApp::exit() {
	logLifecycle("exit");
	releaseGLResources();
}

bool ofApp::backPressed() {
	return false;
}
