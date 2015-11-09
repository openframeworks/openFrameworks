//
//  of3dGraphics.h
//  openFrameworksLib
//
//  Created by Nick Hardeman on 9/14/12.
//
//

#pragma once

#include "ofConstants.h"
#include "ofColor.h"
#include "ofPoint.h"
#include "ofMatrix4x4.h"
#include "ofRectangle.h"
#include "ofTypes.h"
#include "of3dPrimitives.h"


// Grid Plane - drawn from the center //
void ofSetPlaneResolution( int columns, int rows );
ofVec2f ofGetPlaneResolution();
void ofDrawPlane(float x, float y, float width, float height);
void ofDrawPlane(float x, float y, float z, float width, float height);
void ofDrawPlane(ofPoint& position, float width, float height);
void ofDrawPlane( float width, float height );

// UV Sphere
void ofSetSphereResolution(int res);
int ofGetSphereResolution();
void ofDrawSphere(float x, float y, float radius);
void ofDrawSphere(float x, float y, float z, float radius);
void ofDrawSphere(const ofPoint& position, float radius);
void ofDrawSphere(float radius);

// Deprecated Methods //
OF_DEPRECATED_MSG("Use ofDrawSphere instead.", void ofSphere(float x, float y, float radius) );
OF_DEPRECATED_MSG("Use ofDrawSphere instead.", void ofSphere(float x, float y, float z, float radius) );
OF_DEPRECATED_MSG("Use ofDrawSphere instead.", void ofSphere(const ofPoint& position, float radius) );
OF_DEPRECATED_MSG("Use ofDrawSphere instead.", void ofSphere(float radius) );


// Ico Sphere
void ofSetIcoSphereResolution( int res );
int ofGetIcoSphereResolution();
void ofDrawIcoSphere(float x, float y, float z, float radius);
void ofDrawIcoSphere(float x, float y, float radius);
void ofDrawIcoSphere(const ofPoint& position, float radius);
void ofDrawIcoSphere(float radius);

// Cylinder //
void ofSetCylinderResolution( int radiusSegments, int heightSegments, int capSegments=2 );
ofVec3f ofGetCylinderResolution();
void ofDrawCylinder(float x, float y, float radius, float height);
void ofDrawCylinder(float x, float y, float z, float radius, float height);
void ofDrawCylinder(const ofPoint& position, float radius, float height);
void ofDrawCylinder(float radius, float height);

// Cone

/// \brief Set the resolution of a polygonized cone. 
///
/// Allows you to set the polygonization resolution of any cones you subsequently draw with ofDrawCone(). 
///
/// \param radiusSegments The number of facets (subdivisions) around the cone's circular footprint. 
/// \param heightSegments The number of subdivisions from the cone's top to bottom. 
/// \param capSegments The number of annular (ring-shaped) subdivisions of the cone's endcap. 
void ofSetConeResolution( int radiusSegments, int heightSegments, int capSegments=2);

/// \brief Retrieve a data structure describing the resolution with which cones are polygonized. 
///
/// Allows you to fetch the resolution with which cones are polygonized.
/// Returns an ofVec3f containing the following data: 
/// Encoded as "x": radiusSegments, the number of facets (subdivisions) around the cone's circular footprint. 
/// Encoded as "y": heightSegments, the number of subdivisions from the cone's top to bottom. 
/// Encoded as "z": capSegments, the number of annular (ring-shaped) subdivisions of the cone's endcap. 
///
/// \returns An ofVec3f containing (radiusSegments, heightSegments, capSegments) for cone polygonization. 
ofVec3f ofGetConeResolution();

void ofDrawCone(float x, float y, float z, float radius, float height);
void ofDrawCone(float x, float y, float radius, float height);
void ofDrawCone(const ofPoint& position, float radius, float height);
void ofDrawCone(float radius, float height);

// Deprecated Methods //
OF_DEPRECATED_MSG("Use ofDrawCone instead.", void ofCone(float x, float y, float z, float radius, float height) );
OF_DEPRECATED_MSG("Use ofDrawCone instead.", void ofCone(float x, float y, float radius, float height) );
OF_DEPRECATED_MSG("Use ofDrawCone instead.", void ofCone(const ofPoint& position, float radius, float height) );
OF_DEPRECATED_MSG("Use ofDrawCone instead.", void ofCone(float radius, float height) );

// Box
void ofSetBoxResolution( int res );
void ofSetBoxResolution( int resWidth, int resHeight, int resDepth );
ofVec3f ofGetBoxResolution();

/// \brief Draws a rectangular box with the specified dimensions, starting from the specified coordinates. 
///
/// A box is a rectangular solid: an extruded rectangle. 
/// It is drawn starting from a 3D reference coordinate. 
/// It has a width (in x), a height (in y), and a depth (in z).
/// The box is drawn with the current color, e.g. set with ofSetColor().
/// The box is drawn filled by default; change this with ofFill();
///
/// \param x The x-coordinate of the box's origin. 
/// \param y The y-coordinate of the box's origin. 
/// \param z The z-coordinate of the box's origin. 
/// \param width The width of the box. 
/// \param height The height of the box.
/// \param depth The depth of the box.
void ofDrawBox( float x, float y, float z, float width, float height, float depth);

/// \brief Draws a cube with the specified size, starting from the specified coordinates. 
///
/// A cube is a rectangular solid bounded by six square faces of equal size.
/// It is also known as a regular hexahedron, a square parallelepiped, an equilateral cuboid 
/// and a right rhombohedron. It is a regular square prism in three orientations.
///
/// It is drawn starting from a 3D reference coordinate, with the specified size.
/// The cube is drawn with the current color, e.g. set with ofSetColor().
/// The cube is drawn filled by default; change this with ofFill();
///
/// \param x The x-coordinate of the cube's origin. 
/// \param y The y-coordinate of the cube's origin. 
/// \param z The z-coordinate of the cube's origin. 
/// \param size The size of the cube. 
void ofDrawBox(float x, float y, float z, float size);

/// \brief Draws a rectangular box with the specified dimensions, starting from the specified position. 
///
/// A box is a rectangular solid: an extruded rectangle. 
/// It is drawn starting from a 3D reference coordinate. 
/// It has a width (in x), a height (in y), and a depth (in z).
/// The box is drawn with the current color, e.g. set with ofSetColor().
/// The box is drawn filled by default; change this with ofFill();
///
/// \param position an ofPoint which contains the (x,y,z) coordinates for the box's reference corner. 
/// \param width The width of the box. 
/// \param height The height of the box.
/// \param depth The depth of the box.
void ofDrawBox(const ofPoint& position, float width, float height, float depth);

/// \brief Draws a cube with the specified size, starting from the specified position. 
///
/// A cube is drawn starting from a 3D reference position, with the specified size.
/// The cube is drawn with the current color, e.g. set with ofSetColor().
/// The cube is drawn filled by default; change this with ofFill();
///
/// \param position an ofPoint which contains the (x,y,z) coordinates for the cube's reference corner.  
/// \param size The size of the cube. 
void ofDrawBox(const ofPoint& position, float size);

/// \brief Draws a cube with the specified size, starting from the origin. 
///
/// The cube is drawn with the current color, e.g. set with ofSetColor().
/// The cube is drawn filled by default; change this with ofFill();
///
/// \param size The size of the cube. 
void ofDrawBox(float size);

/// \brief Draws a rectangular box with the specified dimensions, starting from the origin. 
///
/// A box is a rectangular solid: an extruded rectangle. 
/// It is drawn starting from the origin of the current reference frame. 
/// It has a width (in x), a height (in y), and a depth (in z).
/// The box is drawn with the current color, e.g. set with ofSetColor().
/// The box is drawn filled by default; change this with ofFill();
///
/// \param width The width of the box. 
/// \param height The height of the box.
/// \param depth The depth of the box.
void ofDrawBox( float width, float height, float depth );

// deprecated methods //
OF_DEPRECATED_MSG("Use ofDrawBox instead.", void ofBox( float x, float y, float z, float width, float height, float depth) );
OF_DEPRECATED_MSG("Use ofDrawBox instead.", void ofBox(float x, float y, float z, float size) );
OF_DEPRECATED_MSG("Use ofDrawBox instead.", void ofBox(const ofPoint& position, float width, float height, float depth) );
OF_DEPRECATED_MSG("Use ofDrawBox instead.", void ofBox(const ofPoint& position, float size) );
OF_DEPRECATED_MSG("Use ofDrawBox instead.", void ofBox(float size) );
OF_DEPRECATED_MSG("Use ofDrawBox instead.", void ofBox( float width, float height, float depth ) );

class of3dGraphics{
public:
	of3dGraphics(ofBaseRenderer * renderer);
	void setPlaneResolution( int columns, int rows );
	ofVec2f getPlaneResolution() const;
	void drawPlane(float x, float y, float width, float height) const;
	void drawPlane(float x, float y, float z, float width, float height) const;
	void drawPlane(ofPoint& position, float width, float height) const;
	void drawPlane( float width, float height ) const;

	// UV Sphere
	void setSphereResolution(int res);
	int getSphereResolution() const;
	void drawSphere(float x, float y, float radius) const;
	void drawSphere(float x, float y, float z, float radius) const;
	void drawSphere(const ofPoint& position, float radius) const;
	void drawSphere(float radius) const;

	// Ico Sphere
	void setIcoSphereResolution( int res );
	int getIcoSphereResolution() const;
	void drawIcoSphere(float x, float y, float z, float radius) const;
	void drawIcoSphere(float x, float y, float radius) const;
	void drawIcoSphere(const ofPoint& position, float radius) const;
	void drawIcoSphere(float radius) const;

	// Cylinder //
	void setCylinderResolution( int radiusSegments, int heightSegments, int capSegments=2 );
	ofVec3f getCylinderResolution() const;
	void drawCylinder(float x, float y, float radius, float height) const;
	void drawCylinder(float x, float y, float z, float radius, float height) const;
	void drawCylinder(const ofPoint& position, float radius, float height) const;
	void drawCylinder(float radius, float height) const;

	// Cone

	/// \brief Set the resolution of a polygonized cone.
	///
	/// Allows you to set the polygonization resolution of any cones you subsequently draw with ofDrawCone().
	///
	/// \param radiusSegments The number of facets (subdivisions) around the cone's circular footprint.
	/// \param heightSegments The number of subdivisions from the cone's top to bottom.
	/// \param capSegments The number of annular (ring-shaped) subdivisions of the cone's endcap.
	void setConeResolution( int radiusSegments, int heightSegments, int capSegments=2);

	/// \brief Retrieve a data structure describing the resolution with which cones are polygonized.
	///
	/// Allows you to fetch the resolution with which cones are polygonized.
	/// Returns an ofVec3f containing the following data:
	/// Encoded as "x": radiusSegments, the number of facets (subdivisions) around the cone's circular footprint.
	/// Encoded as "y": heightSegments, the number of subdivisions from the cone's top to bottom.
	/// Encoded as "z": capSegments, the number of annular (ring-shaped) subdivisions of the cone's endcap.
	///
	/// \returns An ofVec3f containing (radiusSegments, heightSegments, capSegments) for cone polygonization.
	ofVec3f getConeResolution() const;

	void drawCone(float x, float y, float z, float radius, float height) const;
	void drawCone(float x, float y, float radius, float height) const;
	void drawCone(const ofPoint& position, float radius, float height) const;
	void drawCone(float radius, float height) const;

	// Box
	void setBoxResolution( int res );
	void setBoxResolution( int resWidth, int resHeight, int resDepth );
	ofVec3f getBoxResolution() const;

	/// \brief Draws a rectangular box with the specified dimensions, starting from the specified coordinates.
	///
	/// A box is a rectangular solid: an extruded rectangle.
	/// It is drawn starting from a 3D reference coordinate.
	/// It has a width (in x), a height (in y), and a depth (in z).
	/// The box is drawn with the current color, e.g. set with ofSetColor().
	/// The box is drawn filled by default; change this with ofFill();
	///
	/// \param x The x-coordinate of the box's origin.
	/// \param y The y-coordinate of the box's origin.
	/// \param z The z-coordinate of the box's origin.
	/// \param width The width of the box.
	/// \param height The height of the box.
	/// \param depth The depth of the box.
	void drawBox( float x, float y, float z, float width, float height, float depth) const;

	/// \brief Draws a cube with the specified size, starting from the specified coordinates.
	///
	/// A cube is a rectangular solid bounded by six square faces of equal size.
	/// It is also known as a regular hexahedron, a square parallelepiped, an equilateral cuboid
	/// and a right rhombohedron. It is a regular square prism in three orientations.
	///
	/// It is drawn starting from a 3D reference coordinate, with the specified size.
	/// The cube is drawn with the current color, e.g. set with ofSetColor().
	/// The cube is drawn filled by default; change this with ofFill();
	///
	/// \param x The x-coordinate of the cube's origin.
	/// \param y The y-coordinate of the cube's origin.
	/// \param z The z-coordinate of the cube's origin.
	/// \param size The size of the cube.
	void drawBox(float x, float y, float z, float size) const;

	/// \brief Draws a rectangular box with the specified dimensions, starting from the specified position.
	///
	/// A box is a rectangular solid: an extruded rectangle.
	/// It is drawn starting from a 3D reference coordinate.
	/// It has a width (in x), a height (in y), and a depth (in z).
	/// The box is drawn with the current color, e.g. set with ofSetColor().
	/// The box is drawn filled by default; change this with ofFill();
	///
	/// \param position an ofPoint which contains the (x,y,z) coordinates for the box's reference corner.
	/// \param width The width of the box.
	/// \param height The height of the box.
	/// \param depth The depth of the box.
	void drawBox(const ofPoint& position, float width, float height, float depth) const;

	/// \brief Draws a cube with the specified size, starting from the specified position.
	///
	/// A cube is drawn starting from a 3D reference position, with the specified size.
	/// The cube is drawn with the current color, e.g. set with ofSetColor().
	/// The cube is drawn filled by default; change this with ofFill();
	///
	/// \param position an ofPoint which contains the (x,y,z) coordinates for the cube's reference corner.
	/// \param size The size of the cube.
	void drawBox(const ofPoint& position, float size) const;

	/// \brief Draws a cube with the specified size, starting from the origin.
	///
	/// The cube is drawn with the current color, e.g. set with ofSetColor().
	/// The cube is drawn filled by default; change this with ofFill();
	///
	/// \param size The size of the cube.
	void drawBox(float size) const;

	/// \brief Draws a rectangular box with the specified dimensions, starting from the origin.
	///
	/// A box is a rectangular solid: an extruded rectangle.
	/// It is drawn starting from the origin of the current reference frame.
	/// It has a width (in x), a height (in y), and a depth (in z).
	/// The box is drawn with the current color, e.g. set with ofSetColor().
	/// The box is drawn filled by default; change this with ofFill();
	///
	/// \param width The width of the box.
	/// \param height The height of the box.
	/// \param depth The depth of the box.
	void drawBox( float width, float height, float depth ) const;

	void drawAxis(float size) const;
	void drawGrid(float stepSize, size_t numberOfSteps, bool labels, bool x, bool y, bool z) const;
	void drawGridPlane(float stepSize, size_t numberOfSteps, bool labels) const;
	void drawArrow(const ofVec3f& start, const ofVec3f& end, float headSize) const;
	void drawRotationAxes(float radius, float stripWidth, int circleRes) const;

private:
	void renderCached3dPrimitive( const of3dPrimitive& model ) const;
	mutable ofBaseRenderer * renderer;
	ofPlanePrimitive plane;
	ofSpherePrimitive sphere;
	ofIcoSpherePrimitive icoSphere;
	ofBoxPrimitive box;
	ofConePrimitive cone;
	ofCylinderPrimitive cylinder;
	ofBoxPrimitive boxWireframe;
	of3dPrimitive axis;
};
