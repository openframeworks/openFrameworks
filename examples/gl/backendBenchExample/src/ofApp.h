#pragma once

#include "ofMain.h"
#include <string>
#include <vector>

#if defined(TARGET_OF_IOS)
#include "ofxiOSApp.h"
#endif

void benchConfigure(int argc, char ** argv);

#if defined(TARGET_OF_IOS)
class ofApp : public ofxiOSApp {
#else
class ofApp : public ofBaseApp {
#endif
public:
	enum class BenchMode {
		Mesh,
		Immediate,
		Batch
	};

	void setup() override;
	void update() override;
	void draw() override;

private:
	struct Trial {
		int count = 0;
		BenchMode mode = BenchMode::Mesh;
	};

	struct LogoPoint {
		float x = 0.f;
		float y = 0.f;
		bool mark = false;
		unsigned char band = 0;
	};

	void startTrial();
	void buildLogo();
	void drawLogo();
	std::string backendLabel() const;

	std::vector<Trial> trials_;
	std::vector<LogoPoint> logo_;
	int trial_ = 0;
	float seconds_ = 10.f;
	bool measuring_ = false;
	bool started_ = false;
	double phaseStart_ = 0.0;
	int frames_ = 0;
	float minDt_ = 1.e9f;
	ofMesh mesh_;
};
