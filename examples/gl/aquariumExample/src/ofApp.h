#pragma once

#include "ofMain.h"
#include "aquariumMath.h"

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#if defined(TARGET_OF_IOS)
#include "ofxiOSApp.h"
#endif

void aquariumConfigure(int argc, char ** argv);

class ofApp;
bool aquariumDawnPrepare(ofApp & app);
void aquariumDawnDraw(ofApp & app);

// WebGL Aquarium (webglsamples.org) in openFrameworks.
// Copyright 2009, Google Inc. Portions of the scene logic are BSD-3-Clause.
#if defined(TARGET_OF_IOS)
class ofApp : public ofxiOSApp {
#else
class ofApp : public ofBaseApp {
#endif
public:
	void setup() override;
	void update() override;
	void draw() override;
	void keyPressed(int key) override;

	struct Mesh {
		ofVbo vbo;
		int indexCount = 0;
		float maxZ = 0;
#if OF_USE_DAWN
		std::vector<float> pos;
		std::vector<float> nor;
		std::vector<float> uv;
		std::vector<float> tan;
		std::vector<float> bin;
		std::vector<std::uint32_t> indices;
#endif
	};
	struct Model {
		Mesh mesh;
		std::string diffuse;
		std::string normalMap;
		std::string reflectionMap;
	};
	struct Scene {
		std::string name;
		std::string group;
		std::string program;
		bool fog = true;
		bool blend = false;
		std::vector<Model> models;
	};
	struct Object {
		int scene = -1;
		aq::Mat4 world;
	};
	struct FishKind {
		const char * name = "";
		float speed = 0;
		float speedRange = 0;
		float radius = 0;
		float radiusRange = 0;
		float tailSpeed = 0;
		float heightOffset = 0;
		float heightRange = 0;
		float length = 10;
		float wave = 1;
		float bend = 1;
		bool lasers = false;
		float laserRot = 0;
		float laserOff[3] = {0, 0, 0};
		float laserScale[3] = {1, 1, 1};
		int num = 0;
		int scene = -1;
		float maxZ = 1;
		struct Inst {
			float position[3] = {};
			float target[3] = {};
			float scale = 1;
			float time = 0;
			bool outside = false;
			float outPos[3] = {};
			float outTarget[3] = {};
		};
		std::vector<Inst> inst;
	};
	struct Ray {
		float duration = 1;
		float timer = 1;
		float rot = 0;
		float x = 0;
	};
	struct Bubble {
		float x = 0, y = 0, z = 0;
		float vx = 0, vy = 0, vz = 0;
		float ay = 0;
		float size0 = 0.01f;
		float size1 = 0.4f;
		float age = 0;
		float life = 40;
	};
	struct View {
		const char * name;
		float targetHeight, targetRadius, eyeHeight, eyeRadius, eyeSpeed, fov;
		float ambient[3];
		float fogPower, fogMult, fogOffset;
		float fogRgb[3];
	};

	bool loadShader(ofShader & shader, const std::string & vert, const std::string & frag);
	bool loadAqm(const std::string & path, std::vector<Model> & models);
	const ofTexture & textureNamed(const std::string & name);
	void applyView(int index);
	void distributeFish(int totalFish);
	void updateCamera();
	void setMat(const ofShader & shader, const char * name, const aq::Mat4 & m);
	void setShared(const ofShader & shader);
	void bindModelTextures(const ofShader & shader, const Model & model);
	void drawGroup(const std::string & group, float specularFactor);
	void drawFish();
	void drawLasers(bool outside);
	void drawRays();
	void drawBubbles();
	void initRay(Ray & ray);
	void spawnBubbles();
	float pseudo();
	void resetPseudo();

	bool ready = false;
	std::vector<Scene> scenes;
	std::vector<Object> objects;
	std::unordered_map<std::string, int> sceneByName;
	std::unordered_map<std::string, ofImage> images;
	ofTexture white;
	GLuint cube = 0;

	ofShader litShader;
	ofShader innerShader;
	ofShader outerShader;
	ofShader seaweedShader;
	ofShader fishShader;
	ofShader texShader;
	ofShader bubbleShader;

	ofVbo laserVbo;
	int laserCount = 0;
	ofVbo rayVbo;
	int rayCount = 0;
	ofVbo bubbleVbo;

	std::vector<FishKind> fish;
	std::vector<Ray> rays;
	std::vector<Bubble> bubbles;
	float bubbleTimer = 0;

	aq::Mat4 projection;
	aq::Mat4 view;
	aq::Mat4 viewInverse;
	aq::Mat4 viewProjection;
	aq::Mat4 viewProjectionNoRot;
	float eye[3] = {};
	float light[3] = {};

	int viewIndex = 0;
	int fishSetting = 2;
	int fishTotal = 500;
	float targetHeight = 0, targetRadius = 88, eyeHeight = 38, eyeRadius = 69, eyeSpeed = 0.0258f, fov = 64;
	float ambient[3] = {0.218f, 0.246f, 0.394f};
	float fogPower = 16.5f, fogMult = 1.5f, fogOffset = 0.738f;
	float fogRgb[3] = {0.338f, 0.81f, 1.f};
	float fishHeightRange = 1, fishHeight = 25, fishSpeed = 0.124f, fishOffset = 0.52f;
	float fishXClock = 1, fishYClock = 0.556f, fishZClock = 1, fishTailSpeed = 1;
	float globalsSpeed = 1;
	float refractionFudge = 3, eta = 1, tankColorFudge = 0.8f;

	bool optNormal = false;
	bool optReflect = false;
	bool optFog = true;
	bool optTank = true;
	bool optMuseum = true;
	bool optBubbles = true;
	bool optRays = true;
	bool optLasers = false;
	bool prettyDone = false;

	float clock = 0;
	float eyeClock = 0;
	float then = 0;
	int frameCount = 0;
	std::uint32_t randomSeed = 0;

	static const int kFishPresets[10];
	static const View kViews[6];
};
