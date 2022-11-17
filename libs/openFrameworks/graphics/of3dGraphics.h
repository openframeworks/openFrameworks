//
//  of3dGraphics.h
//  openFrameworksLib
//
//  Created by Nick Hardeman on 9/14/12.
//
//

#pragma once

#include "of3dPrimitives.h"


/// \section Planes

/// \brief Set the resolution to use when drawing planes.
///
/// \param columns The number of columns to use when drawing planes with the
///        current renderer.
/// \param rows The number of rows to use when drawing planes with the current
///        renderer.
/// \sa ofDrawPlane()
void ofSetPlaneResolution( int columns, int rows );

/// \brief Get the current renderer's plane resolution as a vector.
/// \returns A vector whose x property describes column resolution and
///          y property describes row resolution.
/// \sa ofDrawPlane()
glm::vec2 ofGetPlaneResolution();

/// \brief Draw a plane with the current renderer.
///
/// Planes are drawn with their x, y, and z coordinates specifying the center of
/// the plane. This function draws the plane with a default origin z value of 0.
///
/// \param x The x coordinate of the plane's centroid.
/// \param y The y coordinate of the plane's centroid.
/// \param width The width of the plane.
/// \param height The height of the plane.
void ofDrawPlane(float x, float y, float width, float height);

/// \brief Draw a plane with the current renderer.
///
/// Planes are drawn with their x, y, and z coordinates specifying the center of
/// the plane.
///
/// \param x The x coordinate of the plane's centroid.
/// \param y The y coordinate of the plane's centroid.
/// \param z The z coordinate of the plane's centroid.
/// \param width The width of the plane.
/// \param height The height of the plane.
void ofDrawPlane(float x, float y, float z, float width, float height);

/// \brief Draw a plane with the current renderer.
///
/// Planes are drawn with their position coordinates specifying the center of
/// the plane.
///
/// \param position A 3D point specifying the plane's centroid.
/// \param width The width of the plane.
/// \param height The height of the plane.
void ofDrawPlane(const glm::vec3& position, float width, float height);

/// \brief Draw a plane with the current renderer at the origin.
/// \param width The width of the plane.
/// \param height The height of the plane.
void ofDrawPlane( float width, float height );

/// \section Spheres

/// \brief Set the sphere resolution for the current renderer.
///
/// The sphere resolution describes how many facets (cross-sections)
/// will be used when drawing spheres. A higher resolution will produce a
/// smoother looking sphere at the cost of using more vertices to do so.
///
/// \param res The resolution to use when drawing spheres.
/// \sa ofDrawSphere()
void ofSetSphereResolution(int res);

/// \brief Get the sphere resolution used by the current renderer.
/// \returns The sphere resolution used by the current renderer.
int ofGetSphereResolution();

/// \brief Draw a sphere with the current renderer.
///
/// Spheres are drawn with x, y, and z coordinates representing the center
/// of the sphere. This function draws the sphere with a default origin z value
/// of 0.
///
/// \param x The x coordinate of the sphere's centroid.
/// \param y The y coordinate of the sphere's centroid.
/// \param radius The radius of the sphere.
void ofDrawSphere(float x, float y, float radius);

/// \brief Draw a sphere with the current renderer.
///
/// Spheres are drawn with their x, y, and z coordinates representing the center
/// of the sphere.
///
/// \param x The x coordinate of the sphere's centroid.
/// \param y The y coordinate of the sphere's centroid.
/// \param z The z coordinate of the sphere's centroid.
/// \param radius The radius of the sphere.
void ofDrawSphere(float x, float y, float z, float radius);

/// \brief Draw a sphere with the current renderer.
///
/// Spheres are drawn with their position representing the center of the sphere.
///
/// \param position A 3D point specifying the sphere's centroid.
/// \param radius The radius of the sphere.
void ofDrawSphere(const glm::vec3& position, float radius);

/// \brief Draw a sphere with the current renderer at the coordinate system's
/// origin.
///
/// Spheres are drawn with their position representing the center of the sphere.
/// This function draws a sphere at the coordinate system origin of 0, 0, 0.
///
/// \param radius The radius of the sphere.
void ofDrawSphere(float radius);

OF_DEPRECATED_MSG("Use ofDrawSphere instead.", void ofSphere(float x, float y, float radius) );
OF_DEPRECATED_MSG("Use ofDrawSphere instead.", void ofSphere(float x, float y, float z, float radius) );
OF_DEPRECATED_MSG("Use ofDrawSphere instead.", void ofSphere(const glm::vec3& position, float radius) );
OF_DEPRECATED_MSG("Use ofDrawSphere instead.", void ofSphere(float radius) );

/// \brief Set the icosphere resolution for the current renderer.
///
/// A higher resolution will produce a smoother looking sphere at the cost of
/// using more vertices to do so.
///
/// \param res The resolution to use when drawing icospheres.
/// \sa ofDrawIcoSphere()
void ofSetIcoSphereResolution( int res );

/// \brief Get the icosphere resolution used by the current renderer.
///
/// \returns The icosphere resolution used by the current renderer.
int ofGetIcoSphereResolution();

/// \brief Draw an icosphere with the current renderer.
///
/// Icospheres are drawn with their x, y, and z coordinates representing the
/// center of the sphere.
///
/// \param x The x coordinate of the sphere's centroid.
/// \param y The y coordinate of the sphere's centroid.
/// \param z The z coordinate of the sphere's centroid.
/// \param radius The radius of the sphere.
void ofDrawIcoSphere(float x, float y, float z, float radius);

/// \brief Draw an icosphere with the current renderer.
///
/// Icospheres are drawn with their x, y, and z coordinates representing the
/// center of the sphere. This function draws the sphere with a default origin z
/// value of 0.
///
/// \param x The x coordinate of the sphere's centroid.
/// \param y The y coordinate of the sphere's centroid.
/// \param radius The radius of the sphere.
void ofDrawIcoSphere(float x, float y, float radius);

/// \brief Draw an icosphere with the current renderer.
///
/// Icospheres are drawn with their position coordinates representing the center
/// of the sphere.
///
/// \param position A 3D point specifying the sphere's centroid.
/// \param radius The radius of the sphere.
void ofDrawIcoSphere(const glm::vec3& position, float radius);

/// \brief Draw an icosphere with the current renderer at the origin.
///
/// Icospheres are drawn with their position coordinates representing the center
/// of the sphere. This function draws a sphere at the coordinate system origin
/// (0, 0, 0).
///
/// \param radius The radius of the sphere.
void ofDrawIcoSphere(float radius);

/// \section Cylinders

/// \brief Set the cylinder resolution for the current renderer.
/// \param radiusSegments The number of facets (subdivisions) around the
///                       cylinder's circular footprint. A larger number yields
///                       a higher resolution.
/// \param heightSegments The number of horizontal subdivisions from the
///        cylinder's top to bottom.
/// \param capSegments The number of annular (ring-shaped) subdivisions of
///        the cylinder's endcap. Defaults to 2.
void ofSetCylinderResolution( int radiusSegments, int heightSegments, int capSegments=2 );

/// \brief Get the current renderer's cylinder resolution as a 3D vector.
///
/// The resulting vector's x, y, and z properties correspond to the radius
/// segments, height segments, and cap segments of the current renderer's
/// cylinder resolution respectively.
///
/// \returns A 3D vector representing the current renderer's cylinder
/// resolution.
glm::vec3 ofGetCylinderResolution();

/// \brief Draw a cylinder with the current renderer.
///
/// Cyliners are drawn with x, y, and z coordinates representing the
/// centroid of the cylinder. This function doesn't specify a cylinder's z value
/// so it defaults to 0.
///
/// \param x The x coordinate to use when drawing this cylinder.
/// \param y The y coordinate to use when drawing this cylinder.
/// \param radius The radius to use when drawing this cylinder's circular
///        footprint.
/// \param height The height to use when drawing this cylinder.
void ofDrawCylinder(float x, float y, float radius, float height);

/// \brief Draw a cylinder with the current renderer.
///
/// Cylinders are drawn with x, y, and z coordinates representing the
/// centroid of the cylinder.
///
/// \param x The x coordinate to use when drawing this cylinder.
/// \param y The y coordinate to use when drawing this cylinder.
/// \param z The z coordinate to use when drawing this cylinder.
/// \param radius The radius to use when drawing this cylinder's circular
///        footprint.
/// \param height The height to use when drawing this cylinder.
void ofDrawCylinder(float x, float y, float z, float radius, float height);

/// \brief Draw a cylinder with the current renderer.
///
/// Cyliners are drawn with x, y, and z coordinates representing the
/// centroid of the cylinder.
///
/// \param position The 3D point to use as a position coordinate when
/// drawing this cylinder.
/// \param radius The radius to use when drawing this cylinder's circular
///        footprint.
/// \param height The height to use when drawing this cylinder.
void ofDrawCylinder(const glm::vec3& position, float radius, float height);

/// \brief Draw a cylinder at the origin using radius and height.
///
/// A cylinder drawn in this way will be positioned at the coordinate system
/// origin (0, 0, 0).
///
/// \param radius The radius to use when drawing this cylinder's circular
///        footprint.
void ofDrawCylinder(float radius, float height);

/// \section Cones

/// \brief Set the cone resolution for the current renderer.
///
/// Allows you to set the polygonization resolution of any cones you
/// subsequently draw with ofDrawCone().
///
/// \param radiusSegments The number of facets (subdivisions) around the cone's
///        circular footprint.
/// \param heightSegments The number of subdivisions from the cone's top to bottom. 
/// \param capSegments The number of annular (ring-shaped) subdivisions of the
///        cone's endcap. Defaults to 2.
void ofSetConeResolution( int radiusSegments, int heightSegments, int capSegments=2);

/// \brief Get the current cone resolution.
///
/// Allows you to fetch the resolution with which cones are polygonized.
/// Returns a glm::vec3 containing the following data:
/// Encoded as "x": radiusSegments, the number of facets (subdivisions) around
/// the cone's circular footprint.
/// Encoded as "y": heightSegments, the number of subdivisions from the cone's
/// top to bottom.
/// Encoded as "z": capSegments, the number of annular (ring-shaped)
/// subdivisions of the cone's endcap.
///
/// \returns A 3D vector containing radiusSegments, heightSegments, capSegments
/// for cone polygonization as x, y, and z properties respectively.
glm::vec3 ofGetConeResolution();

/// \brief Draw a cone with the current renderer.
///
/// Cones are drawn with x, y, and z coordinates representing the centroid
/// of the cone.
///
/// \param x The x coordinate to use when drawing this cone.
/// \param y The y coordinate to use when drawing this cone.
/// \param z The z coordinate to use when drawing this cone.
/// \param radius The radius to use when drawing this cone's circular
///        footprint.
/// \param height The height to use when drawing this cone.
void ofDrawCone(float x, float y, float z, float radius, float height);

/// \brief Draw a cone with the current renderer.
///
/// Cones are drawn with x, y, and z coordinates representing the centroid
/// of the cylinder. This function draws a cone with a z position property of 0.
///
/// \param x The x coordinate to use when drawing this cone.
/// \param y The y coordinate to use when drawing this cone.
/// \param radius The radius to use when drawing this cone's circular
///        footprint.
/// \param height The height to use when drawing this cone.
void ofDrawCone(float x, float y, float radius, float height);

/// \brief Draw a cone with the current renderer.
///
/// Cones are drawn with x, y, and z coordinates representing the centroid
/// of the cone.
///
/// \param position The 3D point representing this cone's position.
/// \param radius The radius to use when drawing this cone's circular
///        footprint.
/// \param height The height to use when drawing this cone.
void ofDrawCone(const glm::vec3& position, float radius, float height);

/// \brief Draw a cone at the origin using radius and height.
///
/// This cone will be drawn with its position at the coordinate system's
/// origin (0, 0, 0).
///
/// \param radius The radius to use when drawing this cone's circular
///        footprint.
/// \param height The height to use when drawing this cone.
void ofDrawCone(float radius, float height);

OF_DEPRECATED_MSG("Use ofDrawCone instead.", void ofCone(float x, float y, float z, float radius, float height) );
OF_DEPRECATED_MSG("Use ofDrawCone instead.", void ofCone(float x, float y, float radius, float height) );
OF_DEPRECATED_MSG("Use ofDrawCone instead.", void ofCone(const glm::vec3& position, float radius, float height) );
OF_DEPRECATED_MSG("Use ofDrawCone instead.", void ofCone(float radius, float height) );

/// \section Boxes

/// \brief Set the resolution the current renderer uses when drawing boxes.
///
/// \param res The number of subdivisions to use for box widths, heights, and
///            depths.
void ofSetBoxResolution( int res );

/// \brief Set the current box resolution.
/// \param resWidth The number of subdivisions to use when drawing box widths.
/// \param resHeight The number of subdivisions to use when drawing box heights.
/// \param resDepth The number of subdivisions to use when drawing box depths.
void ofSetBoxResolution( int resWidth, int resHeight, int resDepth );

/// \brief Get the current renderer's box resolution as a 3D vector.
///
/// The returned vector's x, y, and z properties represent the current
/// renderer's resolution width, resolution height, and resolution depth
/// respectively.
///
/// \returns This renderer's current box resolution as a 3D vector.
glm::vec3 ofGetBoxResolution();

/// \brief Draw a cube with the current renderer.
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

/// \brief Draw a cube with the current renderer.
///
/// A cube is a rectangular solid bounded by six square faces of equal size.
/// It is also known as a regular hexahedron, a square parallelepiped, an
/// equilateral cuboid and a right rhombohedron. It is a regular square prism in
/// three orientations.
///
/// It is drawn starting from a 3D reference coordinate, with the specified
/// size. The cube is drawn with the current color, e.g. set with ofSetColor().
/// The cube is drawn filled by default; change this with ofFill();
///
/// \param x The x-coordinate of the cube's origin. 
/// \param y The y-coordinate of the cube's origin. 
/// \param z The z-coordinate of the cube's origin. 
/// \param size The size of the cube. 
void ofDrawBox(float x, float y, float z, float size);

/// \brief Draws a cube with the current renderer.
///
/// A box is a rectangular solid: an extruded rectangle. 
/// It is drawn starting from a 3D reference coordinate. 
/// It has a width (in x), a height (in y), and a depth (in z).
/// The box is drawn with the current color, e.g. set with ofSetColor().
/// The box is drawn filled by default; change this with ofFill();
///
/// \param position a glm::vec3 which contains the (x,y,z) coordinates for the
///        box's reference corner.
/// \param width The width of the box. 
/// \param height The height of the box.
/// \param depth The depth of the box.
void ofDrawBox(const glm::vec3& position, float width, float height, float depth);

/// \brief Draws a cube with the current renderer.
///
/// A cube is drawn starting from a 3D reference position, with the specified
/// size.
/// The cube is drawn with the current color, e.g. set with ofSetColor().
/// The cube is drawn filled by default; change this with ofFill();
///
/// \param position a glm::vec3 which contains the (x,y,z) coordinates for the
///        cube's reference corner.
/// \param size The size of the cube. 
void ofDrawBox(const glm::vec3& position, float size);

/// \brief Draws a cube with the current renderer, starting from the origin.
///
/// The cube is drawn with the current color, e.g. set with ofSetColor().
/// The cube is drawn filled by default; change this with ofFill();
///
/// \param size The size of the cube. 
void ofDrawBox(float size);

/// \brief Draws a cube with the current renderer, starting from
/// the origin.
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
OF_DEPRECATED_MSG("Use ofDrawBox instead.", void ofBox(const glm::vec3& position, float width, float height, float depth) );
OF_DEPRECATED_MSG("Use ofDrawBox instead.", void ofBox(const glm::vec3& position, float size) );
OF_DEPRECATED_MSG("Use ofDrawBox instead.", void ofBox(float size) );
OF_DEPRECATED_MSG("Use ofDrawBox instead.", void ofBox( float width, float height, float depth ) );

class of3dGraphics{
public:
	/// \brief Construct a of3dGraphics object.
	///
	/// \param renderer A pointer to the base renderer.
	of3dGraphics(ofBaseRenderer * renderer);

	/// \section Planes

	/// \brief Set the plane resolution using \p column and \p rows.
	///
	/// \param columns The number of columns to use when drawing planes.
	/// \param rows The number of rows to use when drawing planes.
	void setPlaneResolution( int columns, int rows );
	
	/// \brief Get the current plane resolution as a 2D vector.
	///
	/// The resulting vector's x and y values corresponds to the current column
	/// and row resolutions respectively.
	///
	/// \returns A 2D vector representing the current plane resolution in
	/// columns and rows.
	glm::vec2 getPlaneResolution() const;

	/// \brief Draw a plane using x, y, width, and height.
	///
	/// Planes are drawn with their x, y, and z coordinates specifying the
	/// center of the plane. This function draws a plane with a z coordinate of
	/// 0.
	///
	/// \param x The x coordinate to use when drawing the plane.
	/// \param y The y coordinate to use when drawing the plane.
	/// \param width The width to use when drawing the plane.
	/// \param height The height to use when drawing the plane.
	void drawPlane(float x, float y, float width, float height) const;

	/// \brief Draw a plane using x, y, z, width, and height.
	///
	/// Planes are drawn with their x, y, and z coordinates specifying the
	/// center of the plane.
	///
	/// \param x The x coordinate to use when drawing the plane.
	/// \param y The y coordinate to use when drawing the plane.
	/// \param z The z coordinate to use when drawing the plane.
	/// \param width The width to use when drawing the plane.
	/// \param height The height to use when drawing the plane.
	void drawPlane(float x, float y, float z, float width, float height) const;

	/// \brief Draw a plane a position vector, width, and height.
	///
	/// Planes are drawn with their position coordinates specifying the center
	/// of the plane.
	///
	/// \param position A 3D point specifying the plane's centroid.
	/// \param width The width to use when drawing the plane.
	/// \param height The height to use when drawing the plane.
	void drawPlane(const glm::vec3& position, float width, float height) const;

	/// \brief Draw a plane at the coordinate system's origin.
	///
	/// This function draws a plane with a centroid at the coordinate system
	/// origin (0, 0, 0).
	///
	/// \param width The width of the plane.
	/// \param height The height of the plane.
	void drawPlane( float width, float height ) const;

	/// \section Spheres

	/// \brief Set the resolution to use when drawing spheres.
	///
	/// The sphere resolution describes how many facets (cross-sections)
	/// will be used when drawing spheres. A higher resolution will produce a
	/// smoother looking sphere at the cost of using more vertices to do so.
	///
	/// \param res The resolution to use when drawing spheres.
	/// \sa getSphereResolution()
	/// \sa drawSphere()
	void setSphereResolution(int res);

	/// \brief Get the current sphere resolution.
	///
	/// \returns The current sphere resolution.
	/// \sa setSphereResolution()
	int getSphereResolution() const;

	/// \brief Draw a sphere.
	///
	/// Spheres are drawn with x, y, and z coordinates representing the center
	/// of the sphere. This function draws the sphere with a default origin z
	/// value of 0.
	///
	/// \param x The x coordinate of the sphere's centroid.
	/// \param y The y coordinate of the sphere's centroid.
	/// \param radius The radius of the sphere.
	void drawSphere(float x, float y, float radius) const;

	/// \brief Draw a sphere.
	///
	/// Spheres are drawn with their x, y, and z coordinates representing the
	/// center of the sphere.
	///
	/// \param x The x coordinate of the sphere's centroid.
	void drawSphere(float x, float y, float z, float radius) const;

	/// \brief Draw a sphere.
	///
	/// Spheres are drawn with their position representing the center of the
	/// sphere.
	///
	/// \param position A 3D point specifying the sphere's centroid.
	/// \param radius The radius of the sphere.
	void drawSphere(const glm::vec3& position, float radius) const;

	/// \brief Draw a sphere at the coordinate system's origin.
	///
	/// Spheres are drawn with their position representing the center of the
	/// 0, 0, 0.
	///
	/// \param radius The radius of the sphere.
	void drawSphere(float radius) const;

	/// \brief Set the resolution to use when drawing icospheres.
	///
	/// A higher resolution will produce a smoother looking sphere at the cost
	/// of using more vertices to do so.
	///
	/// \param res The resolution to use when drawing icospheres.
	/// \sa getIcoSphereResolution()
	/// \sa drawIcoSphere()
	void setIcoSphereResolution( int res );

	/// \brief Get the current icosphere resolution.
	///
	/// \returns The current icosphere resolution.
	/// \sa setIcoSphereResolution()
	int getIcoSphereResolution() const;

	/// \brief Draw an icosphere.
	///
	/// Icospheres are drawn with their x, y, and z coordinates representing the
	/// center of the sphere.
	///
	/// \param x The x coordinate of the sphere's centroid.
	/// \param y The y coordinate of the sphere's centroid.
	/// \param z The z coordinate of the sphere's centroid.
	/// \param radius The radius of the sphere.
	void drawIcoSphere(float x, float y, float z, float radius) const;

	/// \brief Draw an icosphere.
	///
	/// Icospheres are drawn with their x, y, and z coordinates representing the
	/// center of the sphere. This function draws the sphere with a default
	/// origin z value of 0.
	///
	/// \param x The x coordinate of the sphere's centroid.
	/// \param y The y coordinate of the sphere's centroid.
	/// \param radius The radius of the sphere.
	void drawIcoSphere(float x, float y, float radius) const;

	/// \brief Draw an icosphere.
	///
	/// Icospheres are drawn with their position coordinates representing the
	/// center of the sphere.
	///
	/// \param position A 3D point specifying the sphere's centroid.
	/// \param radius The radius of the sphere.
	void drawIcoSphere(const glm::vec3& position, float radius) const;

	/// \brief Draw an icosphere at the coordinate system's origin.
	///
	/// Icospheres are drawn with their position coordinates representing the
	/// center of the sphere. This function draws a sphere at the coordinate
	/// system origin (0, 0, 0).
	///
	/// \param radius The radius of the sphere.
	void drawIcoSphere(float radius) const;

	/// \section Cylinders

	/// \brief Set the resolution to use when drawing cylinders.
	///
	/// \param radiusSegments The number of facets (subdivisions) around the
	///        cylinder's circular footprint. A larger number yields a higher
	///        resolution.
	/// \param heightSegments The number of horizontal subdivisions from the
	///        cylinder's top to bottom.
	/// \param capSegments The number of annular (ring-shaped) subdivisions of
	///        the cylinder's endcap. Defaults to 2.
	void setCylinderResolution( int radiusSegments, int heightSegments, int capSegments=2 );

	/// \brief Get the current cylinder resolution as a 3D vector.
	///
	/// The resulting vector's x, y, and z properties correspond to the radius
	/// segments, height segments, and cap segments of the current cylinder
	/// resolution respectively.
	///
	/// \returns A 3D vector representing the current cylinder resolution.
	glm::vec3 getCylinderResolution() const;

	/// \brief Draw a cylinder using x, y, radius, and height.
	///
	/// Cylinders are drawn with x, y, and z coordinates representing the
	/// centroid of the cylinder. This function doesn't specify a cylinder's z
	/// value so it defaults to 0.
	///
	/// \param x The x coordinate to use when drawing this cylinder.
	/// \param y The y coordinate to use when drawing this cylinder.
	/// \param radius The radius to use when drawing this cylinder's circular
	///        footprint.
	/// \param height The height to use when drawing this cylinder.
	void drawCylinder(float x, float y, float radius, float height) const;

	/// \brief Draw a cylinder using x, y, z, radius, and height.
	///
	/// Cylinders are drawn with x, y, and z coordinates representing the
	/// centroid of the cylinder.
	///
	/// \param x The x coordinate to use when drawing this cylinder.
	/// \param y The y coordinate to use when drawing this cylinder.
	/// \param z The z coordinate to use when drawing this cylinder.
	/// \param radius The radius to use when drawing this cylinder's circular
	///        footprint.
	/// \param height The height to use when drawing this cylinder.
	void drawCylinder(float x, float y, float z, float radius, float height) const;

	/// \brief Draw a cylinder using position, radius, and height.
	///
	/// Cyliners are drawn with x, y, and z coordinates representing the
	/// centroid of the cylinder.
	///
	/// \param position The 3D point to use as a position coordinate when
	/// drawing this cylinder.
	///
	/// \param radius The radius to use when drawing this cylinder's circular
	///        footprint.
	/// \param height The height to use when drawing this cylinder.
	void drawCylinder(const glm::vec3& position, float radius, float height) const;

	/// \brief Draw a cylinder at the origin using radius and height.
	///
	/// A cylinder drawn in this way will be positioned at the coordinate system
	/// origin (0, 0, 0).
	///
	/// \param radius The radius to use when drawing this cylinder's circular
	///        footprint.
	/// \param height The height to use when drawing this cylinder.
	void drawCylinder(float radius, float height) const;

	/// \section Cones

	/// \brief Set the resolution to use when drawing cones.
	///
	/// \param radiusSegments The number of facets (subdivisions) around the
	///        cone's circular footprint.
	/// \param heightSegments The number of subdivisions from the cone's top to
	///        bottom.
	/// \param capSegments The number of annular (ring-shaped) subdivisions of
	///        the cone's endcap.
	void setConeResolution( int radiusSegments, int heightSegments, int capSegments=2);

	/// \brief Get the cone resolution.
	///
	/// Allows you to fetch the resolution with which cones are polygonized.
	/// Returns a glm::vec3 containing the following data:
	/// Encoded as "x": radiusSegments, the number of facets (subdivisions)
	/// around the cone's circular footprint.
	/// Encoded as "y": heightSegments, the number of subdivisions from the
	/// cone's top to bottom.
	/// Encoded as "z": capSegments, the number of annular (ring-shaped)
	/// subdivisions of the cone's endcap.
	///
	/// \returns A 3D vector containing radiusSegments, heightSegments,
	/// capSegments for cone polygonization as x, y, and z properties
	/// respectively.
	glm::vec3 getConeResolution() const;

	/// \brief Draw a cone.
	///
	/// Cones are drawn with x, y, and z coordinates representing the centroid
	/// of the cone.
	///
	/// \param x The x coordinate to use when drawing this cone.
	/// \param y The y coordinate to use when drawing this cone.
	/// \param z The z coordinate to use when drawing this cone.
	/// \param radius The radius to use when drawing this cone's circular
	///        footprint.
	/// \param height The height to use when drawing this cone.
	void drawCone(float x, float y, float z, float radius, float height) const;

	/// \brief Draw a cone.
	///
	/// Cones are drawn with x, y, and z coordinates representing the centroid
	/// of the cylinder. This function draws a cone with a z position property
	/// of 0.
	///
	/// \param x The x coordinate to use when drawing this cone.
	/// \param y The y coordinate to use when drawing this cone.
	/// \param radius The radius to use when drawing this cone's circular
	///        footprint.
	/// \param height The height to use when drawing this cone.
	void drawCone(float x, float y, float radius, float height) const;

	/// \brief Draw a cone using x, y, z, radius, and height.
	///
	/// Cones are drawn with x, y, and z coordinates representing the centroid
	/// of the cone.
	///
	/// \param position The 3D point representing this cone's position.
	/// \param radius The radius to use when drawing this cone's circular
	///        footprint.
	/// \param height The height to use when drawing this cone.
	void drawCone(const glm::vec3& position, float radius, float height) const;

	/// \brief Draw a cone at the origin using radius and height.
	///
	/// This cone will be drawn with its position at the coordinate system's
	/// origin (0, 0, 0).
	///
	/// \param radius The radius to use when drawing this cone's circular
	///        footprint.
	/// \param height The height to use when drawing this cone.
	void drawCone(float radius, float height) const;

	/// \section Boxes

	/// \brief Set the resolution to use when drawing boxes.
	///
	/// \param res The number of subdivisions to use for box width, height,
	///        and depth.
	void setBoxResolution( int res );

	/// \brief Set the resolution to use when drawing boxes.
	///
	/// \param resWidth The number of subdivisions to use when drawing box
	///        widths.
	/// \param resHeight The number of subdivisions to use when drawing box
	///        heights.
	/// \param resDepth The number of subdivisions to use when drawing box
	void setBoxResolution( int resWidth, int resHeight, int resDepth );

	/// \brief Get the current box resolution as a 3D vector.
	///
	/// The returned vector's x, y, and z properties represent the current
	/// resolution width, resolution height, and resolution depth
	/// respectively.
	///
	/// returns a 3D vector representing the resolution in each dimension.
	glm::vec3 getBoxResolution() const;

	/// \brief Draw a cube.
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

	/// \brief Draw a cube.
	///
	/// A cube is a rectangular solid bounded by six square faces of equal size.
	/// It is also known as a regular hexahedron, a square parallelepiped, an
	/// equilateral cuboid and a right rhombohedron. It is a regular square
	/// prism in three orientations.
	///
	/// It is drawn starting from a 3D reference coordinate, with the specified
	/// size.
	///
	/// The cube is drawn with the current color, e.g. set with ofSetColor().
	/// The cube is drawn filled by default; change this with ofFill();
	///
	/// \param x The x-coordinate of the cube's origin.
	/// \param y The y-coordinate of the cube's origin.
	/// \param z The z-coordinate of the cube's origin.
	/// \param size The size of the cube.
	void drawBox(float x, float y, float z, float size) const;

	/// \brief Draw a cube.
	///
	/// A box is a rectangular solid: an extruded rectangle.
	/// It is drawn starting from a 3D reference coordinate.
	/// It has a width (in x), a height (in y), and a depth (in z).
	/// The box is drawn with the current color, e.g. set with ofSetColor().
	/// The box is drawn filled by default; change this with ofFill();
	///
	/// \param position a glm::vec3 which contains the (x,y,z) coordinates for the
	///        box's reference corner.
	/// \param width The width of the box.
	/// \param height The height of the box.
	/// \param depth The depth of the box.
	void drawBox(const glm::vec3& position, float width, float height, float depth) const;

	/// \brief Draw a cube.
	///
	/// A cube is drawn starting from a 3D reference position, with the specified
	/// size.
	///
	/// The cube is drawn with the current color, e.g. set with ofSetColor().
	/// The cube is drawn filled by default; change this with ofFill();
	///
	/// \param position a glm::vec3 which contains the (x,y,z) coordinates for the
	///        cube's reference corner.
	/// \param size The size of the cube.
	void drawBox(const glm::vec3& position, float size) const;

	/// \brief Draw a cube.
	///
	/// The cube is drawn with the current color, e.g. set with ofSetColor().
	/// The cube is drawn filled by default; change this with ofFill();
	///
	/// \param size The size of the cube.
	void drawBox(float size) const;

	/// \brief Draw a cube.
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

	/// \brief Draw the coordinate system's axes.
	///
	/// This draws a red, green, and blue lines for the x, y, and z axes
	/// respectively. This can be helpful when trying to orient other objects
	/// in respect to the coordinate system.
	///
	/// \param size The length to draw each axis line.
	void drawAxis(float size) const;

	/// \brief Draw the coordinate system's axes as a grid.
	/// \param stepSize The size of each row/column in each axis grid.
	/// \param numberOfSteps The number of rows/columns in each axis grid.
	/// \param labels True to draw the name and values of the axis as a bitmap
	/// string.
	/// \param x True to draw the x axis.
	/// \param y True to draw the y axis.
	/// \param z True to draw the z axis.
	void drawGrid(float stepSize, size_t numberOfSteps, bool labels, bool x, bool y, bool z) const;

	/// \brief Draw a coordinate system plane using the y and z axes.
	/// \param stepSize The size of each row/column on the axis grid.
	/// \param numberOfSteps The number of rows/columns on the axis grid.
	/// \param labels True to draw the names and values of the axes.
	void drawGridPlane(float stepSize, size_t numberOfSteps, bool labels) const;

	/// \brief Draw an arrow between two 3D points.
	/// \param start The 3D vector to use as the first point.
	/// \param end The 3D vector to use as the second point.
	/// \param headSize The size of the arrowhead.
	void drawArrow(const glm::vec3& start, const glm::vec3& end, float headSize) const;

	/// \brief Draw the coordinate system's rotation axes.
	///
	/// This draws red, green, and blue lines for the x, y, and z rotation
	/// axes respectively. This can be helpful when trying to orient other
	/// object's rotation in respect to the coordinate system.
	///
	/// \param radius The radius to draw the rotation axes with.
	/// \param stripWidth The width of each axis line.
	/// \param circleRes The circle resolution to use when drawing the axes.
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
