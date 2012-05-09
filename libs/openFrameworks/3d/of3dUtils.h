#pragma once
#include "ofVectorMath.h"

void ofDrawAxis(float size);
void ofDrawGrid(float scale = 10.0f, float ticks = 8.0f, bool labels = false, bool x = true, bool y = true, bool z = true);
void ofDrawGridPlane(float scale, float ticks = 8.0f, bool labels = false);
void ofDrawArrow(const ofVec3f& start, const ofVec3f& end, float headSize = 0.05f);
void ofDrawRotationAxes(float radius, float stripWidth = 10, int circleRes = 60);