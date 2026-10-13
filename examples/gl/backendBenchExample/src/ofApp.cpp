#include "ofApp.h"

#include "../vendor/ofxShapeBatchRenderer/ofxShapeBatchRenderer.cpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>

#if defined(TARGET_OF_IOS) && !defined(TARGET_OF_TVOS) && !OF_USE_DAWN && !OF_USE_ANGLE
#include "ofxiOS.h"
#endif

namespace {

int gCount = 0;
std::string gMode = "both";
float gSeconds = 10.f;
std::unique_ptr<ofxShapeBatchRenderer> gBatch;

bool wantMode(ofApp::BenchMode mode) {
	if(gMode == "mesh") {
		return mode == ofApp::BenchMode::Mesh;
	}
	if(gMode == "immediate") {
		return mode == ofApp::BenchMode::Immediate;
	}
	if(gMode == "batch") {
		return mode == ofApp::BenchMode::Batch;
	}
	return true;
}

const char * modeName(ofApp::BenchMode mode) {
	switch(mode) {
	case ofApp::BenchMode::Mesh: return "mesh";
	case ofApp::BenchMode::Immediate: return "immediate";
	case ofApp::BenchMode::Batch: return "batch";
	}
	return "batch";
}

// 256px openFrameworks mark (examples/3d/ofBoxExample/bin/data/of.png).
// Stripes are lines of constant x+y, period 15, white on residues 4..9.
// The circle and the F sit on top of the stripes as solid white.
bool logoMark(int x, int y) {
	const float dx = (x + 0.5f) - 75.246f;
	const float dy = (y + 0.5f) - 138.411f;
	if(dx * dx + dy * dy <= 60.708f * 60.708f) {
		return true;
	}
	if(x >= 147 && x <= 199 && y >= 72 && y <= 196) {
		return true;
	}
	if(x >= 207 && y >= 72 && y <= 111 && (x + y) <= 324) {
		return true;
	}
	if(x >= 207 && y >= 128 && y <= 156 && (x + y) <= 369) {
		return true;
	}
	return false;
}

bool stripeWhite(int x, int y) {
	int s = (x + y) % 15;
	if(s < 0) {
		s += 15;
	}
	return s >= 4 && s <= 9;
}

void hsv(float h, float & r, float & g, float & b) {
	h = std::fmod(h, 1.f);
	if(h < 0.f) {
		h += 1.f;
	}
	const float x = h * 6.f;
	const int sector = static_cast<int>(x);
	const float f = x - static_cast<float>(sector);
	switch(sector % 6) {
	case 0: r = 1.f; g = f; b = 0.f; break;
	case 1: r = 1.f - f; g = 1.f; b = 0.f; break;
	case 2: r = 0.f; g = 1.f; b = f; break;
	case 3: r = 0.f; g = 1.f - f; b = 1.f; break;
	case 4: r = f; g = 0.f; b = 1.f; break;
	default: r = 1.f; g = 0.f; b = 1.f - f; break;
	}
}

}

void benchConfigure(int argc, char ** argv) {
	for(int i = 1; i < argc; i++) {
		const std::string arg = argv[i];
		if(arg == "--count" && i + 1 < argc) {
			gCount = std::atoi(argv[++i]);
		} else if(arg == "--mode" && i + 1 < argc) {
			gMode = argv[++i];
		} else if(arg == "--seconds" && i + 1 < argc) {
			gSeconds = static_cast<float>(std::atof(argv[++i]));
		}
	}
}

std::string ofApp::backendLabel() const {
	auto renderer = ofGetCurrentRenderer();
	if(!renderer) {
		return "none";
	}
#if OF_USE_DAWN
	if(renderer->getType() == "WebGPU") {
		return "dawn";
	}
#else
	const GLubyte * name = glGetString(GL_RENDERER);
	if(name) {
		return reinterpret_cast<const char *>(name);
	}
#endif
	return renderer->getType();
}

void stopBench() {
#if defined(TARGET_OF_IOS)
	std::exit(0);
#else
	ofExit(0);
#endif
}

void ofApp::setup() {
	ofSetVerticalSync(false);
	ofSetFrameRate(0);
#if defined(TARGET_OF_IOS) && !defined(TARGET_OF_TVOS) && !OF_USE_DAWN && !OF_USE_ANGLE
	if(ofxiOSGLKViewController * controller = ofxiOSGetGLKViewController()) {
		[controller setPreferredFPS:120];
	}
#endif
	ofSetBackgroundColor(0, 0, 0);
	ofSetBackgroundAuto(true);
	seconds_ = gSeconds > 0.f ? gSeconds : 10.f;

	const ofApp::BenchMode modes[] = {
		ofApp::BenchMode::Mesh,
		ofApp::BenchMode::Immediate,
		ofApp::BenchMode::Batch
	};
	if(gCount > 0) {
		for(ofApp::BenchMode mode : modes) {
			if(wantMode(mode)) {
				trials_.push_back({ gCount, mode });
			}
		}
	} else {
		const int counts[] = { 1000, 5000, 20000, 50000 };
		for(int count : counts) {
			for(ofApp::BenchMode mode : modes) {
				if(wantMode(mode)) {
					trials_.push_back({ count, mode });
				}
			}
		}
	}
	mesh_.setMode(OF_PRIMITIVE_TRIANGLES);
	mesh_.enableColors();
	buildLogo();
	startTrial();
}

void ofApp::buildLogo() {
	logo_.clear();
	// Finer than the source so 50k items do not stack on the same point.
	const int cells = 400;
	const float step = 256.f / static_cast<float>(cells);
	logo_.reserve(static_cast<std::size_t>(cells * cells) / 2);
	for(int iy = 0; iy < cells; iy++) {
		for(int ix = 0; ix < cells; ix++) {
			const float fx = (static_cast<float>(ix) + 0.5f) * step;
			const float fy = (static_cast<float>(iy) + 0.5f) * step;
			const int x = static_cast<int>(fx);
			const int y = static_cast<int>(fy);
			if(x < 1 || y < 1 || x > 254 || y > 254) {
				continue;
			}
			const bool mark = logoMark(x, y);
			const bool stripe = stripeWhite(x, y);
			if(!mark && !stripe) {
				continue;
			}
			LogoPoint point;
			point.x = fx;
			point.y = fy;
			point.mark = mark;
			int band = (x + y) / 15;
			if(band < 0) {
				band = 0;
			}
			point.band = static_cast<unsigned char>(band);
			logo_.push_back(point);
		}
	}
}

void ofApp::startTrial() {
	measuring_ = false;
	frames_ = 0;
	minDt_ = 1.e9f;
	if(trial_ < static_cast<int>(trials_.size())) {
		const Trial & trial = trials_[trial_];
		mesh_.getVertices().resize(static_cast<size_t>(trial.count) * 3);
		mesh_.getColors().resize(static_cast<size_t>(trial.count) * 3);
		if(trial.mode == BenchMode::Batch) {
			gBatch.reset(new ofxShapeBatchRenderer(SBR_TRIANGLE, trial.count, 1));
		}
	}
}

void ofApp::drawLogo() {
	const Trial & trial = trials_[trial_];
	const int n = trial.count;
	if(logo_.empty() || n <= 0) {
		return;
	}
	const float t = static_cast<float>(ofGetElapsedTimef());
	const float side = std::min(static_cast<float>(ofGetWidth()), static_cast<float>(ofGetHeight())) * 0.92f;
	const float left = (static_cast<float>(ofGetWidth()) - side) * 0.5f;
	const float top = (static_cast<float>(ofGetHeight()) - side) * 0.5f;
	const float scale = side / 256.f;
	// Long axis of each triangle follows the stripe direction (1, -1).
	const float cs = 0.70710678f;
	const float sn = -0.70710678f;
	const float lx[3] = { 14.f, -8.f, -8.f };
	const float ly[3] = { 0.f, 5.f, -5.f };
	const std::size_t count = logo_.size();

	auto place = [&](int i, float & x, float & y, float & r, float & g, float & b) {
		const LogoPoint & point = logo_[(static_cast<std::size_t>(static_cast<uint32_t>(i) * 2654435761u)) % count];
		const float slide = std::sin(t * 0.6f + static_cast<float>(point.band) * 0.35f) * 0.8f;
		x = left + (point.x + slide * cs) * scale;
		y = top + (point.y + slide * sn) * scale;
		if(point.mark) {
			r = g = b = 1.f;
		} else {
			hsv(static_cast<float>(point.band) * 0.045f + t * 0.02f, r, g, b);
		}
	};

	if(trial.mode == BenchMode::Mesh) {
		auto & verts = mesh_.getVertices();
		auto & colors = mesh_.getColors();
		for(int i = 0; i < n; i++) {
			float x, y, r, g, b;
			place(i, x, y, r, g, b);
			const ofFloatColor color(r, g, b, 1.f);
			for(int k = 0; k < 3; k++) {
				verts[static_cast<size_t>(i) * 3 + k] = {
					x + lx[k] * cs - ly[k] * sn,
					y + lx[k] * sn + ly[k] * cs,
					0.f
				};
				colors[static_cast<size_t>(i) * 3 + k] = color;
			}
		}
		mesh_.draw(OF_MESH_FILL);
	} else if(trial.mode == BenchMode::Batch && gBatch) {
		gBatch->clear();
		for(int i = 0; i < n; i++) {
			float x, y, r, g, b;
			place(i, x, y, r, g, b);
			const int ir = static_cast<int>(r * 255.f + 0.5f);
			const int ig = static_cast<int>(g * 255.f + 0.5f);
			const int ib = static_cast<int>(b * 255.f + 0.5f);
			gBatch->setColor(ir, ig, ib, 255);
			gBatch->addTriangle(
				ofPoint(x + lx[0] * cs - ly[0] * sn, y + lx[0] * sn + ly[0] * cs, 0.f),
				ofPoint(x + lx[1] * cs - ly[1] * sn, y + lx[1] * sn + ly[1] * cs, 0.f),
				ofPoint(x + lx[2] * cs - ly[2] * sn, y + lx[2] * sn + ly[2] * cs, 0.f));
		}
		gBatch->draw();
	} else {
		for(int i = 0; i < n; i++) {
			float x, y, r, g, b;
			place(i, x, y, r, g, b);
			ofSetColor(ofFloatColor(r, g, b, 1.f));
			ofDrawTriangle(
				x + lx[0] * cs - ly[0] * sn, y + lx[0] * sn + ly[0] * cs,
				x + lx[1] * cs - ly[1] * sn, y + lx[1] * sn + ly[1] * cs,
				x + lx[2] * cs - ly[2] * sn, y + lx[2] * sn + ly[2] * cs);
		}
	}
}

void ofApp::update() {
}

void ofApp::draw() {
	if(trials_.empty() || trial_ >= static_cast<int>(trials_.size())) {
		stopBench();
		return;
	}
	if(!started_) {
		started_ = true;
		phaseStart_ = ofGetElapsedTimef();
		std::cout << "RENDERER backend=" << backendLabel()
				  << " px=" << ofGetWidth() << "x" << ofGetHeight()
				  << " seconds=" << seconds_ << std::endl;
	}

	drawLogo();

	const double now = ofGetElapsedTimef();
	if(!measuring_) {
		if(now - phaseStart_ >= 1.0) {
			measuring_ = true;
			phaseStart_ = now;
			frames_ = 0;
			minDt_ = 1.e9f;
		}
		return;
	}

	frames_++;
	const float dt = ofGetLastFrameTime();
	if(dt > 0.f && dt < minDt_) {
		minDt_ = dt;
	}
	if(now - phaseStart_ < static_cast<double>(seconds_)) {
		return;
	}

	const double sec = now - phaseStart_;
	const double avg = sec > 0.0 ? static_cast<double>(frames_) / sec : 0.0;
	const double minFps = minDt_ < 1.e8f && minDt_ > 0.f ? 1.0 / static_cast<double>(minDt_) : 0.0;
	const Trial & trial = trials_[trial_];
	std::ostringstream line;
	line << "BENCH backend=" << backendLabel()
		 << " mode=" << modeName(trial.mode)
		 << " n=" << trial.count
		 << " sec=" << sec
		 << " frames=" << frames_
		 << " avg_fps=" << avg
		 << " min_fps=" << minFps
		 << " px=" << ofGetWidth() << "x" << ofGetHeight();
	std::cout << line.str() << std::endl;
	ofLogNotice("backendBench") << line.str();

	trial_++;
	if(trial_ >= static_cast<int>(trials_.size())) {
		stopBench();
		return;
	}
	phaseStart_ = ofGetElapsedTimef();
	startTrial();
}
