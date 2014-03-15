#pragma once
#include "ofVectorMath.h"


/// \brief Draws x,y,z axes representing the current reference frame
///
/// This function draws a set of x,y,z axes, which can be helpful for understanding which way is up. 
/// There are no unit markings nor arrowheads. Axes are not drawn in the negative directions.  
/// Axes are drawn in red (+x), green (+y) and blue (+z), starting from the origin. 
//
/// \param size The size at which to draw the axes.
void ofDrawAxis(float size);
void ofDrawGrid(float scale = 10.0f, float ticks = 8.0f, bool labels = false, bool x = true, bool y = true, bool z = true);
void ofDrawGridPlane(float scale, float ticks = 8.0f, bool labels = false);
void ofDrawArrow(const ofVec3f& start, const ofVec3f& end, float headSize = 0.05f);
void ofDrawRotationAxes(float radius, float stripWidth = 10, int circleRes = 60);
