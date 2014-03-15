#pragma once
#include "ofVectorMath.h"


/// \brief Draws x,y,z axes representing the current reference frame.
///
/// This function draws a set of x,y,z axes, which can be helpful for understanding which way is up. 
/// There are no unit markings nor arrowheads. Axes are not drawn in the negative directions.  
/// Axes are drawn in red (+x), green (+y) and blue (+z), starting from the origin. 
//
/// \param size The size at which to draw the axes.
void ofDrawAxis(float size);

/// \brief Draws grid planes representing the current reference frame, with subdivisions.
///
/// This function draws a set of double-ended grid planes centered at the origin. 
/// Parameters allow each of the grid planes (yz,xz, and xy) to be turned on or off. 
/// Optionally, the function can include numeric labels for grid subdivisions. 
/// (Numeric labels are drawn using ofDrawBitmapString internally.)
/// The yz plane (at x=0) is drawn in red.
/// The xz plane (at y=0) is drawn in green.
/// The xy plane (at z=0) is drawn in blue.
///
/// \param scale The extent of each grid plane, on each side of the origin. A scale of 10 will produce a grid which is 20 units wide (10 on the positive side of the origin, 10 on the negative side).
/// \param ticks The number of ticks (i.e. subdivisions) on each side of the origin. 
/// \param labels Whether or not labels are drawn. 
/// \param x Whether or not the yz plane (at x=0) is drawn.
/// \param y Whether or not the xz plane (at y=0) is drawn.
/// \param z Whether or not the xy plane (at z=0) is drawn.
void ofDrawGrid(float scale = 10.0f, float ticks = 8.0f, bool labels = false, bool x = true, bool y = true, bool z = true);

void ofDrawGridPlane(float scale, float ticks = 8.0f, bool labels = false);
void ofDrawArrow(const ofVec3f& start, const ofVec3f& end, float headSize = 0.05f);
void ofDrawRotationAxes(float radius, float stripWidth = 10, int circleRes = 60);
