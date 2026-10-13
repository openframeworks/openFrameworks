#include "ofApp.h"

#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace {

int gFishPreset = 2;
int gView = 0;
bool gPretty = false;
bool gNoVsync = false;
int gShotFrame = 0;
int gFishOverride = -1;
std::chrono::steady_clock::time_point gTimedStart;
bool gTimed = false;

void reportShot(ofApp & app) {
	const double sec = gTimed
		? std::chrono::duration<double>(std::chrono::steady_clock::now() - gTimedStart).count()
		: 0.0;
	const double fps = sec > 0.0 ? static_cast<double>(app.frameCount) / sec : 0.0;
	std::ostringstream line;
	line << std::fixed
		 << "SHOT /tmp/aquarium-shot.png fish=" << app.fishTotal
		 << " view=" << ofApp::kViews[app.viewIndex].name
		 << " frames=" << app.frameCount
		 << " sec=" << std::setprecision(4) << sec
		 << " avg_fps=" << std::setprecision(3) << fps
		 << " px=" << ofGetWidth() << "x" << ofGetHeight();
	std::cout << line.str() << std::endl;
	ofLogNotice("aquarium") << line.str();
#if defined(TARGET_OF_IOS)
	if (const char * home = std::getenv("HOME")) {
		std::ofstream out(std::string(home) + "/Documents/aquarium-result.txt", std::ios::trunc);
		out << line.str() << std::endl;
	}
	std::cout.flush();
	std::_Exit(0);
#else
	ofExit(0);
#endif
}

float unitRand() {
	return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
}

bool raySphere(const float * p1, const float * p2, const float * center, float radius, float * hit) {
	const float eps = 0.0001f;
	float dp[3] = {p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2]};
	const float a = aq::dot3(dp, dp);
	const float b = 2.f * (dp[0] * (p1[0] - center[0]) + dp[1] * (p1[1] - center[1]) + dp[2] * (p1[2] - center[2]));
	const float c = aq::dot3(center, center) + aq::dot3(p1, p1)
		- 2.f * (center[0] * p1[0] + center[1] * p1[1] + center[2] * p1[2])
		- radius * radius;
	const float disc = b * b - 4.f * a * c;
	if (std::fabs(a) < eps || disc < 0.f) {
		return false;
	}
	const float sq = std::sqrt(disc);
	const float mu1 = (-b + sq) / (2.f * a);
	const float mu2 = (-b - sq) / (2.f * a);
	const float m = std::max(mu1, mu2);
	hit[0] = p1[0] + m * dp[0];
	hit[1] = p1[1] + m * dp[1];
	hit[2] = p1[2] + m * dp[2];
	return true;
}

bool refractVec(const float * i, const float * n, float eta, float * out) {
	const float dotNI = aq::dot3(n, i);
	const float k = 1.f - eta * eta * (1.f - dotNI * dotNI);
	if (k < 0.f) {
		return false;
	}
	const float scale = eta * dotNI + std::sqrt(k);
	out[0] = eta * i[0] - scale * n[0];
	out[1] = eta * i[1] - scale * n[1];
	out[2] = eta * i[2] - scale * n[2];
	return true;
}

} // namespace

const int ofApp::kFishPresets[10] = {1, 100, 500, 1000, 5000, 10000, 15000, 20000, 25000, 30000};

const ofApp::View ofApp::kViews[6] = {
	{"Inside 1", 63.3f, 91.6f, 7.5f, 13.2f, 0.0258f, 82.699f, {0.218f, 0.502f, 0.706f}, 16.5f, 1.5f, 0.738f, {0.338f, 0.81f, 1.f}},
	{"Outside 1", 17.1f, 69.2f, 59.1f, 124.4f, 0.0258f, 56.923f, {0.218f, 0.246f, 0.394f}, 27.1f, 1.46f, 0.53f, {0.382f, 0.602f, 1.f}},
	{"Inside original", 0.f, 88.f, 38.f, 69.f, 0.0258f, 64.f, {0.218f, 0.246f, 0.394f}, 16.5f, 1.5f, 0.738f, {0.338f, 0.81f, 1.f}},
	{"Outside original", 72.f, 73.f, 3.9f, 120.f, 0.0258f, 74.f, {0.218f, 0.246f, 0.394f}, 27.1f, 1.46f, 0.53f, {0.382f, 0.602f, 1.f}},
	{"Center", 24.f, 73.f, 24.f, 0.f, 0.06f, 60.f, {0.22f, 0.25f, 0.39f}, 14.5f, 1.3f, 0.53f, {0.54f, 0.86f, 1.f}},
	{"Outside LG", 20.f, 127.f, 39.9f, 124.f, 0.06f, 24.f, {0.22f, 0.25f, 0.39f}, 27.1f, 1.2f, 0.53f, {0.382f, 0.602f, 1.f}},
};

void aquariumConfigure(int argc, char ** argv) {
	for (int i = 1; i < argc; ++i) {
		const std::string arg = argv[i];
		if (arg == "--pretty") {
			gPretty = true;
		} else if (arg == "--novsync") {
			gNoVsync = true;
		} else if (arg == "--view" && i + 1 < argc) {
			gView = std::atoi(argv[++i]);
		} else if (arg == "--fish" && i + 1 < argc) {
			gFishOverride = std::atoi(argv[++i]);
		} else if (arg == "--shot" && i + 1 < argc) {
			gShotFrame = std::atoi(argv[++i]);
		}
	}
}

bool ofApp::loadShader(ofShader & shader, const std::string & vert, const std::string & frag) {
	auto sourceFor = [](const std::string & path, bool fragment) {
		std::string source = ofBufferFromFile(path).getText();
#if defined(TARGET_OPENGLES)
		const std::string from = "#version 150";
		const auto pos = source.find(from);
		if (pos != std::string::npos) {
			source.replace(pos, from.size(), "#version 300 es");
		}
		if (fragment) {
			const auto line = source.find('\n');
			if (line != std::string::npos) {
				source.insert(line + 1,
					"precision highp float;\nprecision highp int;\n"
					"precision highp sampler2D;\nprecision highp samplerCube;\n");
			}
		}
#endif
		return source;
	};
	const std::string vertSource = sourceFor(vert, false);
	const std::string fragSource = sourceFor(frag, true);
	if (vertSource.empty() || !shader.setupShaderFromSource(GL_VERTEX_SHADER, vertSource)) {
		ofLogError("aquarium") << "vert " << vert;
		return false;
	}
	if (fragSource.empty() || !shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragSource)) {
		ofLogError("aquarium") << "frag " << frag;
		return false;
	}
	if (ofIsGLProgrammableRenderer()) {
		shader.bindDefaults();
		shader.bindAttribute(4, "tangent");
		shader.bindAttribute(5, "binormal");
	}
	return shader.linkProgram();
}

const ofTexture & ofApp::textureNamed(const std::string & name) {
	auto it = images.find(name);
	if (it == images.end() || !it->second.isAllocated()) {
		return white;
	}
	return it->second.getTexture();
}

bool ofApp::loadAqm(const std::string & path, std::vector<Model> & models) {
	std::ifstream in(path, std::ios::binary);
	if (!in) {
		ofLogError("aquarium") << "missing " << path;
		return false;
	}
	std::vector<unsigned char> bytes((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	if (bytes.size() < 8 || std::memcmp(bytes.data(), "AQM1", 4) != 0) {
		ofLogError("aquarium") << "bad mesh " << path;
		return false;
	}
	const unsigned char * p = bytes.data() + 4;
	const unsigned char * end = bytes.data() + bytes.size();
	auto need = [&](size_t n) {
		if (p + n > end) {
			p = end;
			return false;
		}
		return true;
	};
	auto u32 = [&]() {
		std::uint32_t v = 0;
		if (need(4)) {
			std::memcpy(&v, p, 4);
			p += 4;
		}
		return v;
	};
	auto str = [&]() {
		const std::uint32_t n = u32();
		std::string s;
		if (need(n)) {
			s.assign(reinterpret_cast<const char *>(p), n);
			p += n;
		}
		return s;
	};
	const std::uint32_t nmodels = u32();
	const int hasNormal = 1, hasUv = 2, hasTan = 4, hasBin = 8;
	for (std::uint32_t m = 0; m < nmodels; ++m) {
		Model model;
		const std::uint32_t ntex = u32();
		for (std::uint32_t t = 0; t < ntex; ++t) {
			const std::string key = str();
			const std::string file = str();
			if (key == "diffuse") {
				model.diffuse = file;
			} else if (key == "normalMap") {
				model.normalMap = file;
			} else if (key == "reflectionMap") {
				model.reflectionMap = file;
			}
		}
		const std::uint32_t nverts = u32();
		const std::uint32_t nidx = u32();
		const std::uint32_t flags = u32();
		if (!need(sizeof(float) * nverts * 3)) {
			return false;
		}
		std::vector<float> pos(nverts * 3);
		std::memcpy(pos.data(), p, pos.size() * sizeof(float));
		p += pos.size() * sizeof(float);
		float maxZ = -1e30f;
		for (std::uint32_t v = 0; v < nverts; ++v) {
			maxZ = std::max(maxZ, pos[v * 3 + 2]);
		}
		std::vector<float> nor, uv, tan, bin;
		auto takeF = [&](std::vector<float> & dst, int comps) {
			dst.resize(nverts * comps);
			if (!need(dst.size() * sizeof(float))) {
				return;
			}
			std::memcpy(dst.data(), p, dst.size() * sizeof(float));
			p += dst.size() * sizeof(float);
		};
		if (flags & hasNormal) {
			takeF(nor, 3);
		}
		if (flags & hasUv) {
			takeF(uv, 2);
		}
		if (flags & hasTan) {
			takeF(tan, 3);
		}
		if (flags & hasBin) {
			takeF(bin, 3);
		}
		if (nor.empty()) {
			nor.assign(nverts * 3, 0.f);
			for (std::uint32_t v = 0; v < nverts; ++v) {
				nor[v * 3 + 1] = 1.f;
			}
		}
		if (uv.empty()) {
			uv.assign(nverts * 2, 0.f);
		}
		if (tan.empty() || bin.empty()) {
			tan.resize(nverts * 3);
			bin.resize(nverts * 3);
			for (std::uint32_t v = 0; v < nverts; ++v) {
				float nrm[3] = {nor[v * 3], nor[v * 3 + 1], nor[v * 3 + 2]};
				float helper[3] = {0.f, 1.f, 0.f};
				if (std::fabs(nrm[1]) > 0.9f) {
					helper[0] = 1.f;
					helper[1] = 0.f;
				}
				float t[3], b[3];
				aq::cross3(t, helper, nrm);
				aq::normalize3(t, t);
				aq::cross3(b, nrm, t);
				aq::normalize3(b, b);
				tan[v * 3] = t[0];
				tan[v * 3 + 1] = t[1];
				tan[v * 3 + 2] = t[2];
				bin[v * 3] = b[0];
				bin[v * 3 + 1] = b[1];
				bin[v * 3 + 2] = b[2];
			}
		}
		std::vector<ofIndexType> idx(nidx);
		if (!need(sizeof(std::uint32_t) * nidx)) {
			return false;
		}
		for (std::uint32_t i = 0; i < nidx; ++i) {
			std::uint32_t v = 0;
			std::memcpy(&v, p, 4);
			p += 4;
			idx[i] = static_cast<ofIndexType>(v);
		}
#if OF_USE_DAWN
		model.mesh.pos = std::move(pos);
		model.mesh.nor = std::move(nor);
		model.mesh.uv = std::move(uv);
		model.mesh.tan = std::move(tan);
		model.mesh.bin = std::move(bin);
		model.mesh.indices.resize(nidx);
		for (std::uint32_t i = 0; i < nidx; ++i) {
			model.mesh.indices[i] = static_cast<std::uint32_t>(idx[i]);
		}
#else
		model.mesh.vbo.setVertexData(pos.data(), 3, static_cast<int>(nverts), GL_STATIC_DRAW);
		model.mesh.vbo.setNormalData(nor.data(), static_cast<int>(nverts), GL_STATIC_DRAW);
		model.mesh.vbo.setTexCoordData(uv.data(), static_cast<int>(nverts), GL_STATIC_DRAW);
		model.mesh.vbo.setAttributeData(4, tan.data(), 3, static_cast<int>(nverts), GL_STATIC_DRAW);
		model.mesh.vbo.setAttributeData(5, bin.data(), 3, static_cast<int>(nverts), GL_STATIC_DRAW);
		model.mesh.vbo.setIndexData(idx.data(), static_cast<int>(nidx), GL_STATIC_DRAW);
#endif
		model.mesh.indexCount = static_cast<int>(nidx);
		model.mesh.maxZ = maxZ;
		models.push_back(std::move(model));
	}
	return p <= end;
}

void ofApp::applyView(int index) {
	viewIndex = (index % 6 + 6) % 6;
	const View & v = kViews[viewIndex];
	targetHeight = v.targetHeight;
	targetRadius = v.targetRadius;
	eyeHeight = v.eyeHeight;
	eyeRadius = v.eyeRadius;
	eyeSpeed = v.eyeSpeed;
	fov = v.fov;
	std::memcpy(ambient, v.ambient, sizeof(ambient));
	fogPower = v.fogPower;
	fogMult = v.fogMult;
	fogOffset = v.fogOffset;
	std::memcpy(fogRgb, v.fogRgb, sizeof(fogRgb));
}

void ofApp::distributeFish(int totalFish) {
	fishTotal = std::max(0, totalFish);
	int numLeft = fishTotal;
	const char * types[] = {"Big", "Medium", "Small"};
	for (auto & kind : fish) {
		kind.num = 0;
	}
	for (const char * type : types) {
		for (auto & kind : fish) {
			if (std::strncmp(kind.name, type, std::strlen(type)) != 0) {
				continue;
			}
			int numType = numLeft;
			if (std::strcmp(type, "Big") == 0) {
				numType = std::min(numLeft, fishTotal < 100 ? 1 : 2);
			} else if (std::strcmp(type, "Medium") == 0) {
				const int cap = fishTotal < 1000 ? (fishTotal / 10) : (fishTotal < 10000 ? 80 : 160);
				numType = std::min(numLeft, cap);
			}
			numLeft -= numType;
			kind.num = numType;
			kind.inst.assign(numType, FishKind::Inst{});
		}
	}
}

void ofApp::resetPseudo() {
	randomSeed = 0;
}

float ofApp::pseudo() {
	randomSeed = 134775813u * randomSeed + 1u;
	return static_cast<float>(static_cast<double>(randomSeed) / 4294967296.0);
}

void ofApp::initRay(Ray & ray) {
	const float r = unitRand();
	ray.duration = 1.f + unitRand() * 1.f;
	ray.timer = ray.duration;
	ray.rot = r * 1.f;
	ray.x = (r - 0.5f) * 20.f;
}

void ofApp::spawnBubbles() {
	const float radius = unitRand() * 50.f;
	const float angle = unitRand() * static_cast<float>(PI) * 2.f;
	const float ox = std::sin(angle) * radius;
	const float oz = std::cos(angle) * radius;
	for (int i = 0; i < 100; ++i) {
		Bubble b;
		b.x = ox + (unitRand() * 2.f - 1.f) * 0.1f;
		b.y = -2.f + (unitRand() * 2.f - 1.f) * 2.f;
		b.z = oz + (unitRand() * 2.f - 1.f) * 0.1f;
		b.vx = (unitRand() * 2.f - 1.f) * 0.05f;
		b.vy = 0.f;
		b.vz = (unitRand() * 2.f - 1.f) * 0.05f;
		b.ay = 0.05f + unitRand() * 0.02f;
		b.size0 = 0.01f + unitRand() * 0.01f;
		b.size1 = 0.4f + unitRand() * 0.2f;
		b.age = 0;
		b.life = 40.f;
		bubbles.push_back(b);
	}
}

void ofApp::setup() {
#if defined(TARGET_OF_IOS)
	// ofxiOSAppDelegate points the data root at the bundle root. This example
	// keeps scene.json under Documents/data, which the simulator can read.
	if (const char * home = std::getenv("HOME")) {
		ofSetDataPathRoot(std::string(home) + "/Documents/data/");
	}
#endif
	ofDisableArbTex();
	ofSetWindowTitle("Aquarium");
	ofSetVerticalSync(!gNoVsync);
	ofSetFrameRate(0);
	ofBackground(0, 204, 255);

#if !OF_USE_DAWN
	ofPixels px;
	px.allocate(1, 1, OF_PIXELS_RGBA);
	px.setColor(ofColor(255, 255, 255, 255));
	white.allocate(px);
	white.loadData(px);
#endif

	if (!ofFile::doesFileExist("scene.json")) {
		ofLogError("aquarium") << "missing bin/data/scene.json — run scripts/fetch_assets.py";
		return;
	}
	const ofJson doc = ofLoadJson("scene.json");
	for (const auto & item : doc["scenes"]) {
		Scene scene;
		scene.name = item.value("name", "");
		scene.group = item.value("group", "base");
		scene.program = item.value("program", "auto");
		scene.fog = item.value("fog", true);
		scene.blend = item.value("blend", false);
		if (!loadAqm(ofToDataPath(item.value("file", "")), scene.models)) {
			ofLogError("aquarium") << "failed " << scene.name;
			continue;
		}
		sceneByName[scene.name] = static_cast<int>(scenes.size());
		scenes.push_back(std::move(scene));
	}
	for (const auto & item : doc["objects"]) {
		const std::string name = item.value("name", "");
		auto found = sceneByName.find(name);
		if (found == sceneByName.end()) {
			continue;
		}
		Object obj;
		obj.scene = found->second;
		const auto & w = item["worldMatrix"];
		for (int i = 0; i < 16 && i < static_cast<int>(w.size()); ++i) {
			obj.world.m[i] = w[i].get<float>();
		}
		objects.push_back(obj);
	}

	ofDirectory assets("assets");
	assets.listDir();
	for (const ofFile & image : assets.getFiles()) {
		const std::string name = image.getFileName();
		const std::string ext = ofToLower(image.getExtension());
		if (ext != "png" && ext != "jpg" && ext != "jpeg") {
			continue;
		}
		ofImage loaded;
#if OF_USE_DAWN
		loaded.setUseTexture(false);
#endif
		if (!loaded.load(image.path())) {
			ofLogWarning("aquarium") << "texture " << name;
			continue;
		}
#if !OF_USE_DAWN
		ofTexture & tex = loaded.getTexture();
		tex.generateMipmap();
		tex.setTextureWrap(GL_REPEAT, GL_REPEAT);
		tex.setTextureMinMagFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
#endif
		images.emplace(name, std::move(loaded));
	}

#if !OF_USE_DAWN
	const char * faces[6] = {
		"GlobeOuter_EM_positive_x.jpg",
		"GlobeOuter_EM_negative_x.jpg",
		"GlobeOuter_EM_positive_y.jpg",
		"GlobeOuter_EM_negative_y.jpg",
		"GlobeOuter_EM_positive_z.jpg",
		"GlobeOuter_EM_negative_z.jpg"
	};
	glGenTextures(1, &cube);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cube);
	for (int i = 0; i < 6; ++i) {
		ofImage face;
		face.setUseTexture(false);
		if (!face.load(std::string("assets/") + faces[i])) {
			ofLogError("aquarium") << "sky face " << faces[i];
			continue;
		}
		const ofPixels & pix = face.getPixels();
		const GLenum format = pix.getNumChannels() == 4 ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, pix.getWidth(), pix.getHeight(), 0, format, GL_UNSIGNED_BYTE, pix.getData());
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
#endif

#if !OF_USE_DAWN
	auto quad = [](std::vector<float> & pos, std::vector<float> & uv, float angle, bool ray) {
		const float c = std::cos(angle);
		const float s = std::sin(angle);
		float corner[4][3];
		float coord[4][2] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
		if (ray) {
			corner[0][0] = -0.5f; corner[0][1] = 0; corner[0][2] = 0;
			corner[1][0] = 0.5f; corner[1][1] = 0; corner[1][2] = 0;
			corner[2][0] = 0.5f; corner[2][1] = 1; corner[2][2] = 0;
			corner[3][0] = -0.5f; corner[3][1] = 1; corner[3][2] = 0;
		} else {
			corner[0][0] = -0.5f; corner[0][1] = 0; corner[0][2] = 0;
			corner[1][0] = 0.5f; corner[1][1] = 0; corner[1][2] = 0;
			corner[2][0] = 0.5f; corner[2][1] = 0; corner[2][2] = 1;
			corner[3][0] = -0.5f; corner[3][1] = 0; corner[3][2] = 1;
		}
		const int order[6] = {0, 1, 2, 0, 2, 3};
		for (int index : order) {
			const float x = corner[index][0];
			const float y = corner[index][1];
			const float z = corner[index][2];
			pos.push_back(c * x - s * y);
			pos.push_back(s * x + c * y);
			pos.push_back(z);
			uv.push_back(coord[index][0]);
			uv.push_back(coord[index][1]);
		}
	};
	std::vector<float> lp, lu, rp, ru;
	const float turn = 120.f * static_cast<float>(PI) / 180.f;
	quad(lp, lu, 0, false);
	quad(lp, lu, turn, false);
	quad(lp, lu, -turn, false);
	quad(rp, ru, 0, true);
	laserVbo.setVertexData(lp.data(), 3, static_cast<int>(lp.size() / 3), GL_STATIC_DRAW);
	laserVbo.setTexCoordData(lu.data(), static_cast<int>(lu.size() / 2), GL_STATIC_DRAW);
	laserCount = static_cast<int>(lp.size() / 3);
	rayVbo.setVertexData(rp.data(), 3, static_cast<int>(rp.size() / 3), GL_STATIC_DRAW);
	rayVbo.setTexCoordData(ru.data(), static_cast<int>(ru.size() / 2), GL_STATIC_DRAW);
	rayCount = static_cast<int>(rp.size() / 3);

	const bool shadersOk =
		loadShader(litShader, "shaders_gl3/static.vert", "shaders_gl3/lit.frag") &&
		loadShader(innerShader, "shaders_gl3/static.vert", "shaders_gl3/inner.frag") &&
		loadShader(outerShader, "shaders_gl3/static.vert", "shaders_gl3/outer.frag") &&
		loadShader(seaweedShader, "shaders_gl3/seaweed.vert", "shaders_gl3/seaweed.frag") &&
		loadShader(fishShader, "shaders_gl3/fish.vert", "shaders_gl3/lit.frag") &&
		loadShader(texShader, "shaders_gl3/tex.vert", "shaders_gl3/tex.frag") &&
		loadShader(bubbleShader, "shaders_gl3/bubble.vert", "shaders_gl3/bubble.frag");
	if (!shadersOk) {
		ofLogError("aquarium") << "shader link failed";
		return;
	}
#endif

	auto add = [&](const char * name, float speed, float speedRange, float radius, float radiusRange, float tail, float heightOffset, float heightRange, float length, float wave, float bend) {
		FishKind kind;
		kind.name = name;
		kind.speed = speed;
		kind.speedRange = speedRange;
		kind.radius = radius;
		kind.radiusRange = radiusRange;
		kind.tailSpeed = tail;
		kind.heightOffset = heightOffset;
		kind.heightRange = heightRange;
		kind.length = length;
		kind.wave = wave;
		kind.bend = bend;
		fish.push_back(kind);
	};
	add("SmallFishA", 1.f, 1.5f, 30.f, 25.f, 10.f, 0.f, 16.f, 10.f, 1.f, 2.f);
	add("MediumFishA", 1.f, 2.f, 10.f, 20.f, 1.f, 0.f, 16.f, 10.f, -2.f, 2.f);
	add("MediumFishB", 0.5f, 4.f, 10.f, 20.f, 3.f, -8.f, 5.f, 10.f, -2.f, 2.f);
	add("BigFishA", 0.5f, 0.5f, 50.f, 3.f, 1.5f, 0.f, 16.f, 10.f, -1.f, 0.5f);
	add("BigFishB", 0.5f, 0.5f, 45.f, 3.f, 1.f, 0.f, 16.f, 10.f, -0.7f, 0.3f);
	fish[3].lasers = true;
	fish[3].laserRot = 0.04f;
	fish[3].laserOff[1] = 0.1f;
	fish[3].laserOff[2] = 9.f;
	fish[3].laserScale[0] = fish[3].laserScale[1] = 0.3f;
	fish[3].laserScale[2] = 1000.f;
	fish[4].lasers = true;
	fish[4].laserRot = 0.04f;
	fish[4].laserOff[1] = -0.3f;
	fish[4].laserOff[2] = 9.f;
	fish[4].laserScale[0] = fish[4].laserScale[1] = 0.3f;
	fish[4].laserScale[2] = 1000.f;
	for (auto & kind : fish) {
		auto found = sceneByName.find(kind.name);
		if (found == sceneByName.end() || scenes[found->second].models.empty()) {
			ofLogError("aquarium") << "missing fish " << kind.name;
			continue;
		}
		kind.scene = found->second;
		kind.maxZ = scenes[kind.scene].models[0].mesh.maxZ;
	}

	rays.resize(5);
	for (auto & ray : rays) {
		initRay(ray);
	}
	applyView(gView);
	if (gPretty) {
		optNormal = true;
		optReflect = true;
		prettyDone = true;
	}
	if (gFishOverride >= 0) {
		distributeFish(gFishOverride);
	} else {
		fishSetting = std::max(0, std::min(9, gFishPreset));
		distributeFish(kFishPresets[fishSetting]);
	}
#if OF_USE_DAWN
	if (!aquariumDawnPrepare(*this)) {
		ofLogError("aquarium") << "dawn setup failed";
		return;
	}
#endif
	ready = true;
#if OF_USE_DAWN
	std::cout << "aquarium dawn=WebGPU retained-buffers one-draw-per-fish" << std::endl;
#else
	const char * glVersion = reinterpret_cast<const char *>(glGetString(GL_VERSION));
	const char * glRenderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
	std::cout << "aquarium gl=" << (glVersion ? glVersion : "?")
			  << " renderer=" << (glRenderer ? glRenderer : "?") << std::endl;
	ofLogNotice("aquarium") << "gl=" << (glVersion ? glVersion : "?")
							<< " renderer=" << (glRenderer ? glRenderer : "?");
#endif
	gTimedStart = std::chrono::steady_clock::now();
	gTimed = true;
	std::cout << "aquarium scenes=" << scenes.size()
			  << " objects=" << objects.size()
			  << " fish=" << fishTotal
			  << " view=" << kViews[viewIndex].name
			  << " px=" << ofGetWidth() << "x" << ofGetHeight()
			  << std::endl;
	ofLogNotice("aquarium") << "scenes=" << scenes.size()
							<< " objects=" << objects.size()
							<< " fish=" << fishTotal
							<< " view=" << kViews[viewIndex].name
							<< " px=" << ofGetWidth() << "x" << ofGetHeight();
}

void ofApp::update() {
	if (!ready) {
		return;
	}
	const float now = ofGetElapsedTimef();
	const float dt = then == 0.f ? 0.f : now - then;
	then = now;
	clock += dt * globalsSpeed;
	eyeClock += dt * eyeSpeed;
	if (optRays) {
		for (auto & ray : rays) {
			ray.timer -= dt * globalsSpeed;
			if (ray.timer < 0.f) {
				initRay(ray);
			}
		}
	}
	if (optBubbles) {
		bubbleTimer -= dt * globalsSpeed;
		if (bubbleTimer < 0.f) {
			bubbleTimer = 2.f + unitRand() * 8.f;
			spawnBubbles();
		}
		for (auto & bubble : bubbles) {
			bubble.age += dt * globalsSpeed;
		}
		bubbles.erase(std::remove_if(bubbles.begin(), bubbles.end(), [](const Bubble & b) {
			return b.age > b.life;
		}), bubbles.end());
	}
	if (!prettyDone && ofGetFrameRate() > 40.f) {
		prettyDone = true;
		optNormal = true;
		optReflect = true;
		std::cout << "aquarium pretty on (normal maps + reflection)" << std::endl;
	}
}

void ofApp::setMat(const ofShader & shader, const char * name, const aq::Mat4 & m) {
	glm::mat4 matrix;
	std::memcpy(glm::value_ptr(matrix), m.m, sizeof(m.m));
	shader.setUniformMatrix4f(name, matrix);
}

void ofApp::updateCamera() {
	const float aspect = ofGetHeight() > 0 ? static_cast<float>(ofGetWidth()) / static_cast<float>(ofGetHeight()) : 1.f;
	const float near = 1.f;
	const float far = 25000.f;
	const float top = std::tan(fov * static_cast<float>(PI) / 180.f * 0.5f) * near;
	const float bottom = -top;
	const float left = aspect * bottom;
	const float right = aspect * top;
	aq::frustum(projection, left, right, bottom, top, near, far);

	eye[0] = std::sin(eyeClock) * eyeRadius;
	eye[1] = eyeHeight;
	eye[2] = std::cos(eyeClock) * eyeRadius;
	float target[3] = {
		std::sin(eyeClock + static_cast<float>(PI)) * targetRadius,
		targetHeight,
		std::cos(eyeClock + static_cast<float>(PI)) * targetRadius
	};
	const float up[3] = {0.f, 1.f, 0.f};
	aq::cameraLookAt(viewInverse, eye, target, up);
	aq::inverse(view, viewInverse);
	aq::mul(viewProjection, view, projection);

	float xAxis[3], yAxis[3];
	aq::axis(xAxis, viewInverse, 0);
	aq::axis(yAxis, viewInverse, 1);
	light[0] = eye[0] + xAxis[0] * 20.f + yAxis[0] * 30.f;
	light[1] = eye[1] + xAxis[1] * 20.f + yAxis[1] * 30.f;
	light[2] = eye[2] + xAxis[2] * 20.f + yAxis[2] * 30.f;

	aq::Mat4 translationOnly;
	aq::translation(translationOnly, view.m[12], view.m[13], view.m[14]);
	aq::mul(viewProjectionNoRot, translationOnly, projection);
}

void ofApp::setShared(const ofShader & shader) {
	setMat(shader, "viewProjection", viewProjection);
	setMat(shader, "viewInverse", viewInverse);
	shader.setUniform3f("lightWorldPos", light[0], light[1], light[2]);
	shader.setUniform4f("lightColor", 1.f, 1.f, 1.f, 1.f);
	shader.setUniform4f("ambient", ambient[0], ambient[1], ambient[2], 1.f);
	shader.setUniform4f("fogColor", fogRgb[0], fogRgb[1], fogRgb[2], 1.f);
	shader.setUniform1f("fogPower", fogPower);
	shader.setUniform1f("fogMult", fogMult);
	shader.setUniform1f("fogOffset", fogOffset);
}

void ofApp::bindModelTextures(const ofShader & shader, const Model & model) {
	shader.setUniformTexture("diffuse", textureNamed(model.diffuse), 0);
	shader.setUniformTexture("normalMap", textureNamed(model.normalMap), 1);
	shader.setUniformTexture("reflectionMap", textureNamed(model.reflectionMap), 2);
	shader.setUniformTexture("skybox", GL_TEXTURE_CUBE_MAP, static_cast<GLint>(cube), 3);
}

void ofApp::drawGroup(const std::string & group, float specularFactor) {
	int index = 0;
	for (const Object & obj : objects) {
		const Scene & scene = scenes[obj.scene];
		if (scene.group != group) {
			continue;
		}
		ofShader * shader = &litShader;
		if (scene.program == "inner") {
			shader = &innerShader;
		} else if (scene.program == "outer") {
			shader = &outerShader;
		} else if (scene.program == "seaweed") {
			shader = &seaweedShader;
		}
		if (scene.blend) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		} else {
			glDisable(GL_BLEND);
		}
		shader->begin();
		setShared(*shader);
		shader->setUniform1f("uFog", (optFog && scene.fog) ? 1.f : 0.f);
		shader->setUniform1f("shininess", 50.f);
		shader->setUniform1f("specularFactor", specularFactor);
		shader->setUniform4f("specular", 1.f, 1.f, 1.f, 1.f);
		shader->setUniform1f("time", clock + static_cast<float>(index));
		if (scene.program == "inner") {
			shader->setUniform1f("refractionFudge", refractionFudge);
			shader->setUniform1f("eta", eta);
			shader->setUniform1f("tankColorFudge", tankColorFudge);
		}
		aq::Mat4 inv;
		aq::Mat4 invT;
		aq::inverse(inv, obj.world);
		aq::transpose(invT, inv);
		setMat(*shader, "world", obj.world);
		setMat(*shader, "worldInverseTranspose", invT);
		for (const Model & model : scene.models) {
			float useNormal = 0.f;
			float useReflect = 0.f;
			if (scene.program != "diffuse" && scene.program != "seaweed" && optNormal && !model.normalMap.empty()) {
				useNormal = 1.f;
			}
			if ((scene.program == "auto") && optReflect && !model.reflectionMap.empty()) {
				useReflect = 1.f;
			}
			shader->setUniform1f("uNormal", useNormal);
			shader->setUniform1f("uReflect", useReflect);
			shader->setUniform1f("uNormalBias", 0.f);
			bindModelTextures(*shader, model);
			if (model.mesh.indexCount > 0) {
				model.mesh.vbo.drawElements(GL_TRIANGLES, model.mesh.indexCount);
			}
		}
		shader->end();
		++index;
	}
}

void ofApp::drawFish() {
	resetPseudo();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	fishShader.begin();
	setShared(fishShader);
	fishShader.setUniform1f("uFog", optFog ? 1.f : 0.f);
	fishShader.setUniform1f("shininess", 5.f);
	fishShader.setUniform1f("specularFactor", 0.3f);
	fishShader.setUniform4f("specular", 1.f, 1.f, 1.f, 1.f);
	for (auto & kind : fish) {
		if (kind.num <= 0 || kind.scene < 0) {
			continue;
		}
		const Model & model = scenes[kind.scene].models[0];
		const float useNormal = (optNormal && !model.normalMap.empty()) ? 1.f : 0.f;
		const float useReflect = (optReflect && !model.reflectionMap.empty()) ? 1.f : 0.f;
		fishShader.setUniform1f("uNormal", useNormal);
		fishShader.setUniform1f("uReflect", useReflect);
		fishShader.setUniform1f("uNormalBias", 1.f);
		fishShader.setUniform1f("fishLength", kind.length);
		fishShader.setUniform1f("fishWaveLength", kind.wave);
		fishShader.setUniform1f("fishBendAmount", kind.bend);
		bindModelTextures(fishShader, model);
		const float baseClock = clock * fishSpeed;
		const float tail = kind.tailSpeed * fishTailSpeed;
		const float height = fishHeight + kind.heightOffset;
		const float heightRange = fishHeightRange * kind.heightRange;
		for (int i = 0; i < kind.num; ++i) {
			const float fishClock = baseClock + static_cast<float>(i) * fishOffset;
			const float speed = kind.speed + pseudo() * kind.speedRange;
			const float scale = (1.f + pseudo()) ;
			const float xRadius = kind.radius + pseudo() * kind.radiusRange;
			const float yRadius = 2.f + pseudo() * heightRange;
			const float zRadius = kind.radius + pseudo() * kind.radiusRange;
			const float speedClock = fishClock * speed;
			const float xClock = speedClock * fishXClock;
			const float yClock = speedClock * fishYClock;
			const float zClock = speedClock * fishZClock;
			const float px = std::sin(xClock) * xRadius;
			const float py = std::sin(yClock) * yRadius + height;
			const float pz = std::cos(zClock) * zRadius;
			const float nx = std::sin(xClock - 0.04f) * xRadius;
			const float ny = std::sin(yClock - 0.01f) * yRadius + height;
			const float nz = std::cos(zClock - 0.04f) * zRadius;
			const float time = std::fmod((clock + static_cast<float>(i)) * tail * speed, static_cast<float>(PI) * 2.f);
			fishShader.setUniform3f("worldPosition", px, py, pz);
			fishShader.setUniform3f("nextPosition", nx, ny, nz);
			fishShader.setUniform1f("scale", scale);
			fishShader.setUniform1f("time", time);
			model.mesh.vbo.drawElements(GL_TRIANGLES, model.mesh.indexCount);
			if (optLasers && kind.lasers) {
				FishKind::Inst & inst = kind.inst[static_cast<size_t>(i)];
				inst.position[0] = px;
				inst.position[1] = py;
				inst.position[2] = pz;
				inst.target[0] = nx;
				inst.target[1] = ny;
				inst.target[2] = nz;
				inst.scale = scale;
				inst.time = time;
				inst.outside = false;
			}
		}
	}
	fishShader.end();
}

void ofApp::drawLasers(bool outside) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);
	texShader.begin();
	setMat(texShader, "viewProjection", viewProjection);
	texShader.setUniformTexture("colorMap", textureNamed("beam.png"), 0);
	const float c = 0.5f + static_cast<float>(frameCount % 2) + 0.5f;
	texShader.setUniform4f("colorMult", c, c * 0.1f, c * 0.1f, c);
	const float up[3] = {0.f, 1.f, 0.f};
	const float center[3] = {0.f, 36.f, 0.f};
	for (auto & kind : fish) {
		if (!kind.lasers) {
			continue;
		}
		for (int i = 0; i < kind.num; ++i) {
			FishKind::Inst & inst = kind.inst[static_cast<size_t>(i)];
			aq::Mat4 world;
			if (!outside) {
				const float mult = kind.maxZ / kind.length;
				const float s = std::sin(inst.time + mult * kind.wave);
				const float offset = mult * mult * s * kind.bend;
				float off[3] = {offset, kind.laserOff[1], kind.laserOff[2]};
				aq::Mat4 scale;
				aq::Mat4 look;
				aq::Mat4 spin;
				aq::Mat4 shift;
				aq::Mat4 spun;
				aq::scaling(scale, 1.f, 1.f, 1.f);
				aq::cameraLookAt(look, inst.position, inst.target, up);
				aq::mul(world, scale, look);
				aq::rotationY(spin, s * kind.laserRot);
				aq::translation(shift, off[0], off[1], off[2]);
				aq::mul(spun, spin, shift);
				aq::Mat4 placed;
				aq::mul(placed, spun, world);
				world = placed;
				float dir[3] = {world.m[8], world.m[9], world.m[10]};
				aq::normalize3(dir, dir);
				float p1[3] = {world.m[12], world.m[13], world.m[14]};
				float p2[3] = {p1[0] + dir[0] * 1000.f, p1[1] + dir[1] * 1000.f, p1[2] + dir[2] * 1000.f};
				float hit[3];
				if (!raySphere(p1, p2, center, 74.f, hit)) {
					continue;
				}
				const float dx = hit[0] - p1[0];
				const float dy = hit[1] - p1[1];
				const float dz = hit[2] - p1[2];
				const float len = std::sqrt(dx * dx + dy * dy + dz * dz);
				aq::Mat4 beamScale;
				aq::Mat4 beam;
				aq::scaling(beamScale, kind.laserScale[0], kind.laserScale[1], len);
				aq::mul(beam, beamScale, world);
				setMat(texShader, "world", beam);
				laserVbo.draw(GL_TRIANGLES, 0, laserCount);
				float normal[3];
				aq::normalize3(normal, hit);
				float neg[3] = {-dir[0], -dir[1], -dir[2]};
				float bent[3];
				if (refractVec(neg, normal, 1.2f, bent)) {
					inst.outside = true;
					inst.outPos[0] = hit[0];
					inst.outPos[1] = hit[1];
					inst.outPos[2] = hit[2];
					inst.outTarget[0] = hit[0] + bent[0];
					inst.outTarget[1] = hit[1] + bent[1];
					inst.outTarget[2] = hit[2] + bent[2];
				}
			} else if (inst.outside) {
				aq::Mat4 scale;
				aq::Mat4 look;
				aq::scaling(scale, 0.5f, 0.5f, 200.f);
				aq::cameraLookAt(look, inst.outPos, inst.outTarget, up);
				aq::mul(world, scale, look);
				setMat(texShader, "world", world);
				laserVbo.draw(GL_TRIANGLES, 0, laserCount);
			}
		}
	}
	texShader.end();
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
}

void ofApp::drawBubbles() {
	if (bubbles.empty()) {
		return;
	}
	float right[3];
	float upv[3];
	aq::axis(right, viewInverse, 0);
	aq::axis(upv, viewInverse, 1);
	std::vector<float> pos;
	std::vector<float> uv;
	std::vector<float> col;
	pos.reserve(bubbles.size() * 18);
	uv.reserve(bubbles.size() * 12);
	col.reserve(bubbles.size() * 24);
	const float quad[6][2] = {{-1, -1}, {1, -1}, {1, 1}, {-1, -1}, {1, 1}, {-1, 1}};
	const float quv[6][2] = {{0, 0}, {1, 0}, {1, 1}, {0, 0}, {1, 1}, {0, 1}};
	for (const Bubble & bubble : bubbles) {
		const float t = bubble.age;
		const float cx = bubble.x + bubble.vx * t;
		const float cy = bubble.y + bubble.vy * t + 0.5f * bubble.ay * t * t;
		const float cz = bubble.z + bubble.vz * t;
		const float u = std::min(1.f, t / bubble.life);
		const float size = bubble.size0 + (bubble.size1 - bubble.size0) * u;
		const float alpha = u < 0.8f ? 1.f : (1.f - u) / 0.2f;
		const float half = size * 0.5f;
		for (int v = 0; v < 6; ++v) {
			pos.push_back(cx + right[0] * quad[v][0] * half + upv[0] * quad[v][1] * half);
			pos.push_back(cy + right[1] * quad[v][0] * half + upv[1] * quad[v][1] * half);
			pos.push_back(cz + right[2] * quad[v][0] * half + upv[2] * quad[v][1] * half);
			uv.push_back(quv[v][0]);
			uv.push_back(quv[v][1]);
			col.push_back(0.7f);
			col.push_back(0.8f);
			col.push_back(1.f);
			col.push_back(alpha);
		}
	}
	const int count = static_cast<int>(pos.size() / 3);
	bubbleVbo.setVertexData(pos.data(), 3, count, GL_STREAM_DRAW);
	bubbleVbo.setTexCoordData(uv.data(), count, GL_STREAM_DRAW);
	bubbleVbo.setColorData(col.data(), count, GL_STREAM_DRAW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	bubbleShader.begin();
	setMat(bubbleShader, "viewProjection", viewProjection);
	bubbleShader.setUniformTexture("colorMap", textureNamed("bubble.png"), 0);
	bubbleVbo.draw(GL_TRIANGLES, 0, count);
	bubbleShader.end();
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
}

void ofApp::drawRays() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	texShader.begin();
	setMat(texShader, "viewProjection", viewProjectionNoRot);
	texShader.setUniformTexture("colorMap", textureNamed("LightRay.png"), 0);
	for (const Ray & ray : rays) {
		const float lerp = ray.duration > 0.f ? ray.timer / ray.duration : 0.f;
		const float y = std::max(70.f, std::min(120.f, 50.f + eyeHeight));
		aq::Mat4 spin;
		aq::Mat4 shift;
		aq::Mat4 placed;
		aq::Mat4 scale;
		aq::Mat4 world;
		aq::rotationZ(spin, ray.rot + lerp * 0.2f);
		aq::translation(shift, ray.x, y, 0.f);
		aq::mul(placed, spin, shift);
		aq::scaling(scale, 10.f, -100.f, 10.f);
		aq::mul(world, scale, placed);
		const float alpha = std::sin(lerp * static_cast<float>(PI));
		texShader.setUniform4f("colorMult", 1.f, 1.f, 1.f, alpha);
		setMat(texShader, "world", world);
		rayVbo.draw(GL_TRIANGLES, 0, rayCount);
	}
	texShader.end();
	glDepthMask(GL_TRUE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
}

void ofApp::draw() {
	if (!ready) {
		return;
	}
#if OF_USE_DAWN
	updateCamera();
	aquariumDawnDraw(*this);
	++frameCount;
	if (gShotFrame > 0 && frameCount >= gShotFrame) {
		reportShot(*this);
	}
	return;
#endif
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glDisable(GL_BLEND);

	updateCamera();
	drawGroup("base", 1.f);
	drawFish();
	if (optTank) {
		drawGroup("inner", 1.f);
	}
	drawGroup("seaweed", 1.f);
	if (optLasers) {
		drawLasers(false);
	}
	if (optMuseum) {
		drawGroup("outside", 0.f);
	}
	if (optBubbles) {
		drawBubbles();
	}
	if (optRays) {
		drawRays();
	}
	if (optTank) {
		drawGroup("outer", 1.f);
	}
	if (optLasers) {
		drawLasers(true);
	}

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	++frameCount;
	if (gShotFrame > 0 && frameCount >= gShotFrame) {
#if !defined(TARGET_OF_IOS)
		ofSaveScreen("/tmp/aquarium-shot.png");
#endif
		reportShot(*this);
		return;
	}

	ofSetColor(255);
	std::string hud = "fish " + ofToString(fishTotal) + "   view " + kViews[viewIndex].name;
	hud += std::string("   ") + (optNormal ? "normal " : "") + (optReflect ? "reflect " : "") + (optFog ? "fog" : "");
	hud += "    fps " + ofToString(ofGetFrameRate(), 1);
	hud += "\n1-9,0 fish   space view   n r f t m b y l";
	ofDrawBitmapStringHighlight(hud, 16, 24);
}

void ofApp::keyPressed(int key) {
	if (!ready) {
		return;
	}
	if (key == ' ') {
		applyView(viewIndex + 1);
	} else if (key >= '1' && key <= '9') {
		fishSetting = key - '1';
		distributeFish(kFishPresets[fishSetting]);
	} else if (key == '0') {
		fishSetting = 9;
		distributeFish(kFishPresets[9]);
	} else if (key == 'n' || key == 'N') {
		optNormal = !optNormal;
		prettyDone = true;
	} else if (key == 'r' || key == 'R') {
		optReflect = !optReflect;
		prettyDone = true;
	} else if (key == 'f' || key == 'F') {
		optFog = !optFog;
	} else if (key == 't' || key == 'T') {
		optTank = !optTank;
	} else if (key == 'm' || key == 'M') {
		optMuseum = !optMuseum;
	} else if (key == 'b' || key == 'B') {
		optBubbles = !optBubbles;
	} else if (key == 'y' || key == 'Y') {
		optRays = !optRays;
	} else if (key == 'l' || key == 'L') {
		optLasers = !optLasers;
	}
}
