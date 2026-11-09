// AI Token Backburn - what your AI coding tools have used, and what's left.
// Copyright (c) 2026 Daniel Rosser. MIT License.

#pragma once

#include "Usage.h"

#include "ofMain.h"

#include <map>
#include <string>
#include <vector>

// The city: a circuit-board floor of glowing traces, and translucent towers rising out of it. Each AI source is a
// tower, as tall as its share of the use, with its numbers, task titles and Backburn's log scrolling up its faces;
// dimmer towers fill the blocks between. The camera dives in from above at the start and flies to a source's
// tower when its tab opens.
class City {
public:
	struct Colours {
		ofFloatColor tower, edge, trace, pad, text;
	};

	void setup();
	// the sources' towers follow the report; lines: what scrolls up a source's faces, by provider id
	void setReport(const backburn::Report & report, const std::map<std::string, std::vector<std::string>> & lines);
	void setColours(const Colours & colours) { colours_ = colours; }
	// start the dive from above (the intro); seconds: how long until the camera settles
	void startIntro(float seconds);
	void skipIntro() { if (introAt >= 0) introTime = introLength; }
	// fly to a source's tower ("" for the overview of the whole city)
	void flyTo(const std::string & provider);
	void update(float dt, bool animate);
	// the sources' faces into their own buffers; before draw(), and outside any other buffer (a buffer begun inside
	// another doesn't put the outer one back on every GL path, and the city went blank on those frames)
	void drawFaces();
	// into the current framebuffer, w x h, with its own depth
	void draw(float w, float h);

	bool introDone() const { return introAt < 0 || introTime >= introLength; }
	// 0..1 through the intro, for fading the interface in at the end
	float introProgress() const { return introAt < 0 ? 1.0f : std::min(1.0f, introTime / introLength); }
	// a tower-to-tower flight is underway (not the intro dive)
	bool flying() const { return introDone() && flight < 1; }

private:
	struct Tower {
		std::string provider;   // "" for a filler tower
		std::string label;
		glm::vec3 base;         // centre of its foot
		float width = 40, depth = 40, height = 120;
		float target = 120;     // heights ease to this as the report changes
		ofFbo face;             // the text scrolling up its sides
		std::vector<std::string> lines;
		float scroll = 0;
	};
	void buildTraces();
	void drawFace(Tower & t);
	glm::vec3 viewFor(const std::string & provider, glm::vec3 & look) const;

	// board traces as paths, so energy can run along them like messages
	struct Pulse {
		int path = 0;
		float at = 0;        // distance along the path
		float speed = 60;    // units a second
		float length = 26;   // the bright tail behind the head
	};
	std::vector<std::vector<glm::vec3>> paths;
	std::vector<Pulse> pulses;

	std::vector<Tower> towers;
	ofVboMesh traces, pads;
	ofTrueTypeFont font, labelFont;
	ofCamera camera;
	Colours colours_ = { ofFloatColor(0.35f, 1.0f, 0.85f), ofFloatColor(0.75f, 1.0f, 0.95f), ofFloatColor(0.55f, 0.35f, 1.0f),
		                 ofFloatColor(0.75f, 0.55f, 1.0f), ofFloatColor(0.55f, 1.0f, 0.85f) };
	float time = 0;
	// the dive
	float introAt = -1, introTime = 0, introLength = 6;
	// flying between towers
	glm::vec3 fromEye, fromLook, toEye, toLook, eye, look;
	float flight = 1, flightLength = 1.6f;
	std::string focus;
	float faceRefresh = 0;
};
