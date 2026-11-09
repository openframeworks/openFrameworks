// AI Token Backburn - what your AI coding tools have used, and what's left.
// Copyright (c) 2026 Daniel Rosser. MIT License.

#include "City.h"
#include "Fonts.h"

#include <algorithm>
#include <random>

using namespace backburn;

namespace {

// the city's blocks: towers stand on a grid, the streets between are where the traces run
const float BLOCK = 160;
const int BLOCKS = 7;   // per side, centred on the origin

float ease(float t) {
	t = ofClamp(t, 0, 1);
	return t < 0.5f ? 4 * t * t * t : 1 - std::pow(-2 * t + 2, 3) / 2;
}

// lines on the ground in the streets: runs with right-angle turns, ending in pads, like a circuit board.
// Returns the run's points, so energy pulses can travel it in draw().
std::vector<glm::vec3> trace(ofVboMesh & mesh, ofVboMesh & pads, std::mt19937 & rng, glm::vec3 p) {
	std::uniform_int_distribution<int> dir(0, 3), len(1, 4);
	glm::vec3 d(1, 0, 0);
	std::vector<glm::vec3> points { p };
	for (int segment = 0; segment < 6; segment++) {
		switch (dir(rng)) {
		case 0: d = { 1, 0, 0 }; break;
		case 1: d = { -1, 0, 0 }; break;
		case 2: d = { 0, 0, 1 }; break;
		default: d = { 0, 0, -1 }; break;
		}
		p += d * (BLOCK * 0.25f * len(rng));
		points.push_back(p);
	}
	for (size_t i = 0; i + 1 < points.size(); i++) {
		mesh.addVertex(points[i]);
		mesh.addVertex(points[i + 1]);
	}
	// a pad at the end: a small square
	const float s = 5;
	for (auto [a, b] : { std::pair<glm::vec2, glm::vec2>{ { -s, -s }, { s, -s } }, { { s, -s }, { s, s } }, { { s, s }, { -s, s } }, { { -s, s }, { -s, -s } } }) {
		pads.addVertex(p + glm::vec3(a.x, 0, a.y));
		pads.addVertex(p + glm::vec3(b.x, 0, b.y));
	}
	return points;
}

// a point this far along a trace run (clamped to its ends); pulses ride these
glm::vec3 alongPath(const std::vector<glm::vec3> & path, float at) {
	if (path.empty()) return glm::vec3(0);
	if (at <= 0) return path.front();
	for (size_t i = 0; i + 1 < path.size(); i++) {
		const float seg = glm::distance(path[i], path[i + 1]);
		if (at <= seg) return glm::mix(path[i], path[i + 1], seg > 0 ? at / seg : 0.0f);
		at -= seg;
	}
	return path.back();
}

float pathLength(const std::vector<glm::vec3> & path) {
	float total = 0;
	for (size_t i = 0; i + 1 < path.size(); i++) total += glm::distance(path[i], path[i + 1]);
	return total;
}

// one quad of a frosted tower, flat-shaded by its facing so the box reads as a solid
void addQuad(ofMesh & mesh, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, const ofFloatColor & colour) {
	const auto base = mesh.getNumVertices();
	for (const glm::vec3 & v : { a, b, c, d }) {
		mesh.addVertex(v);
		mesh.addColor(colour);
	}
	mesh.addIndex(base);
	mesh.addIndex(base + 1);
	mesh.addIndex(base + 2);
	mesh.addIndex(base);
	mesh.addIndex(base + 2);
	mesh.addIndex(base + 3);
}

// a solid frosted-glass box: milky faces, each side a different brightness (lit from the front-top),
// the top brightest, so towers occlude what's behind them instead of adding up like ghosts
void frostedBox(const glm::vec3 & centre, float w, float h, float d, const ofFloatColor & body, float alpha) {
	const float x = w / 2, y = h / 2, z = d / 2;
	auto face = [&](float shade, float a) {
		ofFloatColor c = body;
		c.r = std::min(1.0f, c.r * shade);
		c.g = std::min(1.0f, c.g * shade);
		c.b = std::min(1.0f, c.b * shade);
		c.a = a;
		return c;
	};
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	addQuad(mesh, centre + glm::vec3(-x, -y, z), centre + glm::vec3(x, -y, z), centre + glm::vec3(x, y, z), centre + glm::vec3(-x, y, z), face(1.0f, alpha));          // front
	addQuad(mesh, centre + glm::vec3(x, -y, -z), centre + glm::vec3(-x, -y, -z), centre + glm::vec3(-x, y, -z), centre + glm::vec3(x, y, -z), face(0.45f, alpha));   // back
	addQuad(mesh, centre + glm::vec3(x, -y, z), centre + glm::vec3(x, -y, -z), centre + glm::vec3(x, y, -z), centre + glm::vec3(x, y, z), face(0.72f, alpha));       // right
	addQuad(mesh, centre + glm::vec3(-x, -y, -z), centre + glm::vec3(-x, -y, z), centre + glm::vec3(-x, y, z), centre + glm::vec3(-x, y, -z), face(0.58f, alpha));  // left
	addQuad(mesh, centre + glm::vec3(-x, y, z), centre + glm::vec3(x, y, z), centre + glm::vec3(x, y, -z), centre + glm::vec3(-x, y, -z), face(1.25f, alpha * 0.9f)); // top
	mesh.draw();
}

} // namespace

void City::setup() {
	// faces render at 512x1024 (see setReport), so the fonts load at twice the old size to stay sharp.
	// Families come from Fonts.h (per platform); when none is on this machine the face stays unloaded
	// and drawFace falls back to no text rather than failing to start.
	const AppFonts & families = appFonts();
	font.load(firstFontFile(families.mono), 22, true, true);
	labelFont.load(firstFontFile(families.wide), 44, true, true);
	buildTraces();
	// filler towers on most blocks; the sources take the middle row when the report arrives
	std::mt19937 rng(5);
	std::uniform_real_distribution<float> unit(0, 1);
	for (int x = -BLOCKS / 2; x <= BLOCKS / 2; x++)
		for (int z = -BLOCKS / 2; z <= BLOCKS / 2; z++) {
			if (z == 0 || unit(rng) < 0.35f) continue;   // the middle row is kept for the sources
			Tower t;
			t.base = { x * BLOCK + (unit(rng) - 0.5f) * 30, 0, z * BLOCK + (unit(rng) - 0.5f) * 30 };
			t.width = 30 + unit(rng) * 40;
			t.depth = 30 + unit(rng) * 40;
			t.height = t.target = 40 + unit(rng) * 160;
			towers.push_back(t);
		}
	eye = toEye = { 0, 900, 900 };
	look = toLook = { 0, 0, 0 };
	camera.setNearClip(2);
	camera.setFarClip(6000);
	camera.setFov(55);
}

void City::buildTraces() {
	traces.setMode(OF_PRIMITIVE_LINES);
	pads.setMode(OF_PRIMITIVE_LINES);
	std::mt19937 rng(9);
	std::uniform_real_distribution<float> unit(0, 1);
	const float half = BLOCK * BLOCKS / 2;
	for (int i = 0; i < 160; i++) {
		// start in a street: halfway between blocks on one axis
		glm::vec3 p((std::floor(unit(rng) * BLOCKS) - BLOCKS / 2 + 0.5f) * BLOCK, 0.5f, (unit(rng) - 0.5f) * 2 * half);
		if (unit(rng) < 0.5f) std::swap(p.x, p.z);
		paths.push_back(trace(traces, pads, rng, p));
	}
	// messages on the board: a few dozen pulses, each riding a random trace, at its own speed
	std::uniform_int_distribution<int> pick(0, int(paths.size()) - 1);
	for (int i = 0; i < 42; i++) {
		Pulse pulse;
		pulse.path = pick(rng);
		pulse.at = unit(rng) * pathLength(paths[pulse.path]);
		pulse.speed = 50 + unit(rng) * 130;
		pulse.length = 16 + unit(rng) * 30;
		pulses.push_back(pulse);
	}
}

void City::setReport(const Report & report, const std::map<std::string, std::vector<std::string>> & lines) {
	// the sources stand along the middle row, Claude first; height by share of the use
	double most = 0;
	for (const auto & p : report.providers) most = std::max(most, p.total.units());
	int i = 0;
	const int count = int(report.providers.size());
	for (const auto & p : report.providers) {
		auto it = std::find_if(towers.begin(), towers.end(), [&](const Tower & t) { return t.provider == p.id; });
		if (it == towers.end()) {
			Tower t;
			t.provider = p.id;
			t.width = t.depth = 70;
			t.height = 20;
			t.face.allocate(512, 1024, GL_RGBA);   // 2x the old 256x512: the words stay readable up close
			towers.push_back(t);
			it = towers.end() - 1;
		}
		it->label = ofToUpper(p.name);
		it->base = { (i - (count - 1) / 2.0f) * BLOCK * 1.1f, 0, 0 };
		// the busiest stands tallest; the others in proportion, never stubs
		const double share = most > 0 ? p.total.units() / most : 0;
		it->target = 140 + 320 * float(std::sqrt(share));
		auto l = lines.find(p.id);
		it->lines = l == lines.end() ? std::vector<std::string>() : l->second;
		i++;
	}
	faceRefresh = 0;
}

void City::startIntro(float seconds) {
	introAt = 0;
	introTime = 0;
	introLength = seconds;
	eye = { 0, 1600, 1400 };
	look = { 0, 0, 200 };
}

glm::vec3 City::viewFor(const std::string & provider, glm::vec3 & lookAt) const {
	for (const auto & t : towers)
		if (!provider.empty() && t.provider == provider) {
			// in front of it and a little above, looking up its face
			lookAt = t.base + glm::vec3(0, t.height * 0.55f, 0);
			return t.base + glm::vec3(BLOCK * 0.35f, t.height * 0.5f + 40, BLOCK * 1.5f);
		}
	// the whole row of sources, from up the street
	lookAt = { 0, 120, 0 };
	return { -BLOCK * 0.6f, 260, BLOCK * 3.2f };
}

void City::flyTo(const std::string & provider) {
	if (provider == focus && flight >= 1) return;
	focus = provider;
	fromEye = eye;
	fromLook = look;
	toEye = viewFor(provider, toLook);
	flight = 0;
}

void City::update(float dt, bool animate) {
	if (!animate) dt = 0;
	time += dt;
	// board energy: each pulse runs its trace, then jumps to a random run like a routed message
	for (auto & pulse : pulses) {
		if (paths.empty()) break;
		pulse.at += dt * pulse.speed;
		if (pulse.at - pulse.length > pathLength(paths[pulse.path])) {
			static std::mt19937 rng(23);
			std::uniform_int_distribution<int> pick(0, int(paths.size()) - 1);
			pulse.path = pick(rng);
			pulse.at = 0;
		}
	}
	for (auto & t : towers) {
		t.height += (t.target - t.height) * std::min(1.0f, dt * 2.0f);
		t.scroll += dt * 14;
	}
	if (introAt >= 0 && introTime < introLength) {
		// the dive: high over the board, down between the blocks, round to the sources
		introTime += animate ? dt : introLength;
		const float k = ease(introTime / introLength);
		glm::vec3 endLook, end = viewFor(focus, endLook);
		const glm::vec3 high(BLOCK * 2.5f, 1700, BLOCK * 3.5f), mid(-BLOCK * 1.8f, 220, BLOCK * 2.0f);
		// a curve through high -> mid -> end
		const glm::vec3 a = glm::mix(high, mid, k), b = glm::mix(mid, end, k);
		eye = glm::mix(a, b, k);
		look = glm::mix(glm::vec3(0, 0, 0), endLook, k);
		fromEye = eye;
		fromLook = look;
		toEye = end;
		toLook = endLook;
		flight = 1;
	} else if (flight < 1) {
		flight = std::min(1.0f, flight + (animate ? dt / flightLength : 1));
		const float k = ease(flight);
		// arc up over the towers between, then down
		eye = glm::mix(fromEye, toEye, k) + glm::vec3(0, std::sin(k * PI) * 160, 0);
		look = glm::mix(fromLook, toLook, k);
	} else {
		// at rest: drift a little, so the city breathes
		eye = toEye + glm::vec3(std::sin(time * 0.15f) * 30, std::sin(time * 0.11f) * 12, 0);
		look = toLook;
	}
	faceRefresh -= dt;
}

// a source's face: its lines, scrolling up, in the theme's text colour
void City::drawFace(Tower & t) {
	if (!t.face.isAllocated()) return;
	t.face.begin();
	ofClear(0, 0, 0, 0);
	ofSetColor(colours_.text);
	labelFont.drawString(t.label, 24, 80);
	const float lineH = 30;
	const int n = int(t.lines.size());
	if (n) {
		const float span = n * lineH;
		for (int i = 0; i < n; i++) {
			float y = 140 + std::fmod(i * lineH - t.scroll * 2 + span * 100, span);
			if (y > 1024) continue;
			ofSetColor(colours_.text, 140 + 115 * (i % 3 == 0));
			font.drawString(t.lines[i], 20, y);
		}
	}
	t.face.end();
}

void City::drawFaces() {
	// a few times a second; scrolling needs no more
	if (faceRefresh > 0) return;
	for (auto & t : towers)
		if (!t.provider.empty()) drawFace(t);
	faceRefresh = 0.08f;
}

void City::draw(float w, float h) {
	camera.setPosition(eye);
	camera.lookAt(look);
	camera.begin(ofRectangle(0, 0, w, h));
	ofPushStyle();
	ofEnableDepthTest();
	ofEnableBlendMode(OF_BLENDMODE_ADD);

	// the board: traces and their pads, glowing
	ofSetLineWidth(2.0f);
	ofSetColor(colours_.trace, 235);
	traces.draw();
	ofSetColor(colours_.pad, 255);
	pads.draw();
	// energy running the board like messages: a bright head with a fading tail on a random trace.
	// Additive here (and the glow pass over the frame) is what blooms.
	for (const auto & pulse : pulses) {
		if (paths.empty()) break;
		const auto & path = paths[pulse.path];
		// follow the trace's bends, not a straight cut across them: sampled every few units
		auto raised = [&](float at) { return alongPath(path, at) + glm::vec3(0, 1.2f, 0); };
		for (int pass = 0; pass < 2; pass++) {
			ofSetColor(colours_.edge, pass == 0 ? 90 : 255);
			ofSetLineWidth(pass == 0 ? 5.0f : 2.0f);
			glm::vec3 prev = raised(pulse.at - pulse.length);
			for (float d = pulse.at - pulse.length + 4; d < pulse.at; d += 4) {
				const glm::vec3 next = raised(d);
				ofDrawLine(prev, next);
				prev = next;
			}
			ofDrawLine(prev, raised(pulse.at));
		}
		ofSetColor(colours_.edge, 255);
		ofDrawSphere(raised(pulse.at), 3.2f);
	}
	ofSetLineWidth(2.0f);

	// towers, far to near so the frosted outer glass blends right over the solid cores
	std::vector<Tower *> order;
	for (auto & t : towers) order.push_back(&t);
	std::sort(order.begin(), order.end(), [&](const Tower * a, const Tower * b) {
		return glm::distance2(a->base, eye) > glm::distance2(b->base, eye);
	});
	// frosted glass is a solid, not a glow: normal blending (it hides what's behind), depth written
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	for (Tower * t : order) {
		const bool source = !t->provider.empty();
		const bool focused = source && t->provider == focus;
		const glm::vec3 c = t->base + glm::vec3(0, t->height / 2, 0);
		// like the film stills: pale glass you can read through, over a darker solid core that gives it body
		ofFloatColor core = colours_.tower;
		core.lerp(ofFloatColor(1, 1, 1), source ? 0.16f : 0.08f);
		// a solid core inside, then the frosted outer skin over it (white: the mesh carries the colour)
		ofSetColor(255);
		glDepthMask(GL_TRUE);
		frostedBox(c, t->width * 0.82f, t->height, t->depth * 0.82f, core, source ? (focused ? 0.88f : 0.80f) : 0.50f);
		glDepthMask(GL_FALSE);
		frostedBox(c, t->width, t->height, t->depth, colours_.tower, source ? (focused ? 0.34f : 0.27f) : 0.17f);
		// light pooled at its foot, where the tower meets the board
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		ofSetColor(colours_.edge, source ? 60 : 20);
		ofPushMatrix();
		ofTranslate(t->base + glm::vec3(0, 0.8f, 0));
		ofRotateXDeg(90);
		ofDrawRectangle(-t->width / 2, -t->depth / 2, t->width, t->depth);
		ofPopMatrix();
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		// bright edges, as if light catches the glass rim
		ofBoxPrimitive box(t->width, t->height, t->depth, 1, 1, 1);
		box.setPosition(c);
		ofSetColor(colours_.edge, source ? 255 : 120);
		box.drawWireframe();
		// its text, only on the faces turned to the camera (the others would read backwards through the glass).
		// Mixed, like the stills: even sides are solid black data panels (opaque backing behind the words),
		// odd sides stay transparent frosted glass with the words floating in it.
		if (source && t->face.isAllocated()) {
			for (int side = 0; side < 4; side++) {
				const float angle = side * HALF_PI;
				const glm::vec3 normal(std::sin(angle), 0, std::cos(angle));
				const float half = (side % 2 ? t->width : t->depth) / 2;
				if (glm::dot(normal, eye - (c + normal * half)) <= 0) continue;
				const float fw = (side % 2 ? t->depth : t->width), fh = t->height;
				ofPushMatrix();
				ofTranslate(c);
				ofRotateYDeg(side * 90.0f);
				ofTranslate(0, 0, half + 0.5f);
				if (side % 2 == 0) {
					ofSetColor(0, 0, 0, 215);
					ofDrawRectangle(-fw / 2, -fh / 2, fw, fh);
				}
				ofSetColor(255, focused ? 255 : 210);
				// y is up here; the face's first line (its name) belongs at the top
				t->face.getTexture().draw(-fw / 2, -fh / 2, fw, fh);
				ofPopMatrix();
			}
		}
		// a glow ring where it meets the board
		ofSetColor(colours_.edge, source ? 170 : 50);
		ofNoFill();
		ofPushMatrix();
		ofTranslate(t->base + glm::vec3(0, 1, 0));
		ofRotateXDeg(90);
		ofDrawRectangle(-t->width / 2 - 6, -t->depth / 2 - 6, t->width + 12, t->depth + 12);
		ofPopMatrix();
		ofFill();
	}
	glDepthMask(GL_TRUE);
	ofDisableDepthTest();
	ofPopStyle();
	camera.end();
}
