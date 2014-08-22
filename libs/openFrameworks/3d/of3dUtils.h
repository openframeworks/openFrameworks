#pragma once

#include "ofVectorMath.h"

/// \brief Draws x,y,z axes representing the current reference frame.
///
/// This function draws a set of x,y,z axes, which can be helpful for
/// understanding which way is up. There are no unit markings nor arrowheads.
/// Axes are not drawn in the negative directions. Axes are drawn in red (+x),
/// green (+y) and blue (+z), starting from the origin. 
//
/// \param size The size at which to draw the axes.
void ofDrawAxis(float size);

/// \brief Draws grid planes representing the current reference frame.
///
/// This function draws a set of three double-ended grid planes centered at the
/// origin. Parameters allow each of the grid planes (yz,xz, and xy) to be
/// turned on or off. Optionally, the function can include white numeric labels
/// for grid subdivisions. (Numeric labels are drawn using ofDrawBitmapString
/// internally.)
/// 
/// The yz plane (at x=0) is drawn in red.
/// The xz plane (at y=0) is drawn in green.
/// The xy plane (at z=0) is drawn in blue.
///
/// \param scale The extent of each grid plane, on each side of the origin. A scale of 10 will produce a set of grid which are 20 units wide (10 on the positive side of the origin, 10 on the negative side).
/// \param ticks The number of ticks (i.e. subdivisions) on each side of the origin. 
/// \param labels Whether or not labels are drawn. 
/// \param x Whether or not the yz plane (at x=0) is drawn.
/// \param y Whether or not the xz plane (at y=0) is drawn.
/// \param z Whether or not the xy plane (at z=0) is drawn.
void ofDrawGrid(float scale = 10.0f, float ticks = 8.0f, bool labels = false, bool x = true, bool y = true, bool z = true);

/// \brief Draws the YZ grid plane representing the current reference frame.
///
/// This function draws the YZ grid plane, at x=0, in a double-ended manner
/// centered at the origin. Optionally, the function can include white numeric
/// labels for grid subdivisions. (Numeric labels are drawn using
/// ofDrawBitmapString internally.) The grid is drawn with the current color
/// (i.e. set with ofSetColor). 
///
/// \param scale The extent of each grid plane, on each side of the origin. A scale of 10 will produce a grid which is 20 units wide (10 on the positive side of the origin, 10 on the negative side).
/// \param ticks The number of ticks (i.e. subdivisions) on each side of the origin. 
/// \param labels Whether or not labels are drawn. 
void ofDrawGridPlane(float scale, float ticks = 8.0f, bool labels = false);

/// \brief Draws an arrow with the current color. 
///
/// This function draws an arrow from a start (tail) point to an end (head)
/// point, with a conical arrowhead at the end point. The arrow is drawn in the
/// current color (e.g. set with ofSetColor). 
///
/// Example: 
///
///     ofSetColor(0,0,0);
///     ofVec3f arrowTailPoint (0,0,0);
///     ofVec3f arrowHeadPoint (100, 150,0);
///     ofDrawArrow(arrowTailPoint, arrowHeadPoint, 20.0);
///
/// \param start The start point (tail) of the arrow, expressed as an ofVec3f.
/// \param end The end point (head) of the arrow, expressed as an ofVec3f. 
/// \param headSize The size of the arrowhead. 
void ofDrawArrow(const ofVec3f& start, const ofVec3f& end, float headSize = 0.05f);

/// \brief This function renders a set of 3 axis-aligned circular bands, centered at the origin.
///
/// \param radius The radius of the circular bands. 
/// \param stripWidth The width of the circular bands. The default is 10 units. 
/// \param circleRes The resolution of the circular bands. The default is 60 segments.
void ofDrawRotationAxes(float radius, float stripWidth = 10, int circleRes = 60);
