// This include guard is not a pragma once on purpose
// so some IDEs are happy include this file back form the corresponding inl
#ifndef OF_MESH_H
#define OF_MESH_H

#include "ofConstants.h"
#include "ofGLUtils.h"

template<class V, class N, class C, class T>
class ofMeshFace_;

/// \brief Represents a set of vertices in 3D spaces with normals, colors,
/// and texture coordinates at those points.
///
/// Each of these different properties is stored in a vector.
/// Vertices are passed to your graphics card and your graphics card fill in
/// the spaces in between them in a processing usually called the rendering
/// pipeline. The rendering pipeline goes more or less like this:
///
/// 1. Say how you're going to connect all the points.
/// 2. Make some points.
/// 3. Say that you're done making points.
///
/// You may be thinking: I'll just make eight vertices and voila: a cube.
/// Not so quick. There's a hitch and that hitch is that the OpenGL renderer
/// has different ways of connecting the vertices that you pass to it and none
/// are as efficient as to only need eight vertices to create a cube.
///
/// You've probably seen a version of the following image somewhere before.
/// ![PRIMATIVES](3d/primitives_new-640x269.gif)
/// Generally you have to create your points to fit the drawing mode that
/// you've selected because of whats called winding.
/// A vertex gets connected to another vertex in the order that the mode does
/// its winding and this means that you might need multiple vertices in a given
/// location to create the shape you want. The cube, for example, requires
/// eighteen vertices, not the eight that you would expect.
/// If you note the order of vertices in the GL chart above you'll see that all
/// of them use their vertices slightly differently (in particular you should
/// make note of the GL_TRIANGLE_STRIP example). Drawing a shape requires that
/// you keep track of which drawing mode is being used and which order
/// your vertices are declared in.
///
/// If you're thinking: it would be nice if there were an abstraction layer
/// for this you're thinking right. Enter the mesh, which is really just
/// an abstraction of the vertex and drawing mode that we started with
/// but which has the added bonus of managing the draw order for you.
/// That may seem insignificant at first, but it provides some real benefits
/// when working with complex geometry.
///
/// A very typical usage is something like the following:
///
/// ~~~~{.cpp}
/// ofMesh mesh;
/// for (int y = 0; y < height; y++){
/// 	for (int x = 0; x<width; x++){
/// 		mesh.addVertex(glm::vec3(x,y,0)); // make a new vertex
/// 		mesh.addColor(ofFloatColor(0,0,0));  // add a color at that vertex
/// 	}
/// }
/// ~~~~
///
/// Now it's important to make sure that each vertex is correctly connected
/// with the other vertices around it. This is done using indices, which you
/// can set up like so:
/// ~~~~{.cpp}
/// for (int y = 0; y<height-1; y++){
/// 	for (int x=0; x<width-1; x++){
/// 		mesh.addIndex(x+y*width);				// 0
/// 		mesh.addIndex((x+1)+y*width);			// 1
/// 		mesh.addIndex(x+(y+1)*width);			// 10
///
/// 		mesh.addIndex((x+1)+y*width);			// 1
/// 		mesh.addIndex((x+1)+(y+1)*width);		// 11
/// 		mesh.addIndex(x+(y+1)*width);			// 10
/// 	}
/// }
/// ~~~~
template<class V, class N, class C, class T>
class ofMesh_{
public:
    using VertexType = V;
    using NormalType = N;
    using ColorType = C;
    using TexCoordType = T;
	/// \name Construction
	/// \{

	/// \brief This creates the mesh,
	/// using OF_PRIMITIVE_TRIANGLES without any initial vertices.
	ofMesh_();

	/// \brief This allows to you to use one of the other ofPrimitiveModes:
	/// OF_PRIMITIVE_TRIANGLES, OF_PRIMITIVE_TRIANGLE_STRIP,
	/// OF_PRIMITIVE_TRIANGLE_FAN, OF_PRIMITIVE_LINES, OF_PRIMITIVE_LINE_STRIP,
	/// OF_PRIMITIVE_LINE_LOOP, OF_PRIMITIVE_POINTS.
	/// See [ofGLUtils](../gl/ofGLUtils.htm) for more information on these types.
	ofMesh_(ofPrimitiveMode mode, const std::vector<V>& verts);

	virtual ~ofMesh_(){}

	void setFromTriangles( const std::vector<ofMeshFace_<V,N,C,T>>& tris, bool bUseFaceNormal=false );

	/// \}
	/// \name Mesh Mode
	/// \{

	/// \brief Allows you to set the ofPrimitiveMode.
	/// The available modes are OF_PRIMITIVE_TRIANGLES,
	/// OF_PRIMITIVE_TRIANGLE_STRIP, OF_PRIMITIVE_TRIANGLE_FAN,
	/// OF_PRIMITIVE_LINES, OF_PRIMITIVE_LINE_STRIP,
	/// OF_PRIMITIVE_LINE_LOOP, OF_PRIMITIVE_POINTS
	void setMode(ofPrimitiveMode mode);

	/// \\returns the primitive mode that the mesh is using.
	ofPrimitiveMode getMode() const;

	/// \}
	/// \name Primitive constructor helper methods
	/// \{
	static ofMesh_ plane(float width, float height, int columns=2, int rows=2,
		ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP);
	static ofMesh_ sphere(float radius, int res=12,
		ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP);
	static ofMesh_ icosahedron(float radius);
	static ofMesh_ icosphere(float radius, std::size_t iterations=2);
	///
	///	\brief A helper method that returns a cylinder made of triangles.
	/// The resolution settings for the radius, height, and cap are optional
	/// (they are set at a default of 12 segments around the radius, 6 segments
	/// in the height, and 2 on the cap). You have the option to cap the
	/// cylinder or not. The only valid modes are the default
	/// OF_PRIMITIVE_TRIANGLE_STRIP and OF_PRIMITIVE_TRIANGLES.
	///	~~~~{.cpp}
	///	ofMesh mesh;
	///	mesh = ofMesh::cylinder(100.0, 200.0);
	///	~~~~
	///
	///	![image of a simple cylinder](3d/cylinder.jpg)
	///
	static ofMesh_ cylinder(float radius, float height, int radiusSegments=12,
		int heightSegments=6, int numCapSegments=2, bool bCapped = true,
		ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP);

	/// \brief A helper method that returns a cone made of triangles.
	/// The resolution settings for the radius, height, and cap are optional
	/// (they are set at a default of 12 segments around the radius, 6 segments
	/// in the height, and 2 on the cap). The only valid modes are the default
	/// OF_PRIMITIVE_TRIANGLE_STRIP and OF_PRIMITIVE_TRIANGLES.
	/// ~~~~{.cpp}
	/// ofMesh mesh;
	/// mesh = ofMesh::cone(100.0, 200.0);
	/// ~~~~
	///
	/// ![image of a simple cone](3d/cone.jpg)
	static ofMesh_ cone(float radius, float height, int radiusSegments=12,
		int heightSegments=6, int capSegments=2,
		ofPrimitiveMode mode=OF_PRIMITIVE_TRIANGLE_STRIP);

	/// \brief A helper method that returns a box made of triangles.
	/// The resolution settings for the width and height are optional
	/// (they are both set at a default of 2 triangles per side).
	/// ~~~~{.cpp}
	/// ofMesh mesh;
	/// mesh = ofMesh::box(200.0, 200.0, 200.0);
	/// ~~~~
	///
	/// ![image of a simple box](3d/box.jpg)
	static ofMesh_ box(float width, float height, float depth, int resX=2,
		int resY=2, int resZ=2);

	/// \returns an ofMesh representing an XYZ coordinate system.
	static ofMesh_ axis(float size=1.0);

	/// \}
	/// \name Vertices
	/// \{

	/// \brief Add a new vertex at the end of the current list of vertices.
	/// It is important to remember that the order the vertices are added to
	/// the list determines how they link they form the polygons and strips
	/// (assuming you do not change their indeces). See the ofMesh class
	/// description for details.
	void addVertex(const V& v);

	/// \brief Add a vector of vertices to a mesh, allowing you to push out
	/// many at once rather than adding one at a time. The vector of vertices
	/// is added after the end of the current vertices list.
	void addVertices(const std::vector<V>& verts);

	/// \brief Add an array of vertices to the mesh.
	/// Because you are using a pointer to the array you also have to define
	/// the length of the array as an int (amt). The vertices are added at the
	/// end of the current vertices list.
	void addVertices(const V* verts, std::size_t amt);

	/// \brief Removes the vertex at the index in the vector.
	void removeVertex(ofIndexType index);

	/// \brief Removes the vertices at the startIndex in the vector and the endindex specified
	void removeVertices(ofIndexType startIndex, ofIndexType endIndex);

	void setVertex(ofIndexType index, const V& v);

	/// \brief Removes all the vertices.
	void clearVertices();

	/// \brief Removes all the vertices, colors, and indices from the mesh.
	void clear();


	/// \returns the size of the vertices vector for the mesh.
	/// This will tell you how many vertices are contained in the mesh.
	std::size_t getNumVertices() const;

	/// \returns a pointer to the vertices that the mesh contains.
	V* getVerticesPointer();

	/// \returns a pointer to the vertices that the mesh contains.
	const V* getVerticesPointer() const;

	/// \returns the vertex at the index.
	V getVertex(ofIndexType i) const;

	/// \returns the vector that contains all of the vertices of the mesh.
	std::vector<V> & getVertices();

	/// \returns the vector that contains all of the vertices of the mesh.
	const std::vector<V> & getVertices() const;

	/// \returns Whether the mesh has any vertices.
	bool hasVertices() const;

	/// \brief Add the vertices, normals, texture coordinates and indices of one mesh onto another mesh.
	/// Everything from the referenced mesh is simply added at the end
	/// of the current mesh's lists.
	void append(const ofMesh_ & mesh);

	void mergeDuplicateVertices();

	/// \returns a glm::vec3 defining the centroid of all the vetices in the mesh.
	V getCentroid() const;


	/// \}

	/// \name Normals
	/// \{

	/// \\returns the normal at the index in the normals vector.
	N getNormal(ofIndexType i) const;

	/// \brief Add a normal to the mesh as a 3D vector,
	/// typically perpendicular to the plane of the face. A normal is a vector
	/// that defines how a surface responds to lighting, i.e. how it is lit.
	/// The amount of light reflected by a surface is proportional to the angle
	/// between the light's direction and the normal. The smaller the angle the
	/// brighter the surface will look. See the normalsExample for advice on
	/// computing the normals.
	/// addNormal adds the 3D vector to the end of the list, so you need to
	/// make sure you add normals at the same index of the matching vertex.
	void addNormal(const N& n);

	/// \brief Add a vector of normals to a mesh,
	/// allowing you to push out many normals at once rather than
	/// adding one at a time. The vector of normals is added after the end of
	/// the current normals list.
	void addNormals(const std::vector<N>& norms);

	/// \brief Add an array of normals to the mesh.
	/// Because you are using a pointer to the array you also have to define
	/// the length of the array as an std::size_t (amt). The normals are added at the
	/// end of the current normals list.
	void addNormals(const N* norms, std::size_t amt);

	/// \brief Remove a normal.
	void removeNormal(ofIndexType index);

	/// \brief Remove normal's from index to end index from normals vector
	void removeNormals(ofIndexType startIndex, ofIndexType endIndex);

	/// \todo Documentation.
	void setNormal(ofIndexType index, const N& n);

	/// \brief Remove all the normals.
	void clearNormals();

	/// \brief This will tell you how many normals are contained in the mesh.
	/// \returns the size of the normals vector for the mesh.
	std::size_t getNumNormals() const;

	/// \returns a pointer to the normals that the mesh contains.
	N* getNormalsPointer();

	/// \returns a pointer to the normals that the mesh contains.
	const N* getNormalsPointer() const;

	/// Use this if you plan to change the normals as part of this call as it
	/// will force a reset of the cache.
	/// \returns the vector that contains all of the normals of the mesh,
	/// if it has any.
	std::vector<N> & getNormals();

	/// \returns the vector that contains all of the normals of the mesh, if
	/// it has any. (read only)
	const std::vector<N> & getNormals() const;

	/// /returnsWhether the mesh has any normals.
	bool hasNormals() const;

	/// \brief Enable mesh normals.
	/// Use disableNormals() to turn normals off.
	/// Normals are enabled by default when they are added to the mesh.
	virtual void enableNormals();
	/// \brief Disable mesh normals.
	/// Use enableNormals() to turn normals back on.
	virtual void disableNormals();
	virtual bool usingNormals() const;

	void smoothNormals( float angle );
        
        /// \brief Duplicates vertices and updates normals to get a low-poly look.
        void flatNormals();

	/// \}
	/// \name Faces
	/// \{

	/// \returns the vector that contains all of the faces of the mesh. This isn't currently implemented.
	ofMeshFace_<V,N,C,T> getFace(ofIndexType faceId) const;

	/// \brief Get normals for each face
	/// As a default it only calculates the normal for the face as a whole but
	/// by setting (perVertex = true) it will return the same normal value for
	/// each of the three vertices making up a face.
	/// \returns a vector containing the calculated normals of each face in the mesh.
	std::vector<N> getFaceNormals( bool perVetex=false) const;

	/// \returns the mesh as a vector of unique ofMeshFace_s
	/// a list of triangles that do not share vertices or indices
	const std::vector<ofMeshFace_<V,N,C,T>> & getUniqueFaces() const;

	/// \}
	/// \name Colors
	/// \{

	/// \brief Get the color at the index in the colors vector.
	/// \returns the color at the index in the colors vector.
	C getColor(ofIndexType i) const;

	/// \brief This adds a color to the mesh,
	/// the color will be associated with the vertex in the same position.
	void addColor(const C& c);

	/// \brief This adds colors using a reference to a vector of ofColors.
	/// For each color in the vector, this will put the colors at the corresponding vertex.
	void addColors(const std::vector<C>& cols);

	/// \brief This adds a pointer of colors to the ofMesh instance with the amount passed as the second parameter.
	void addColors(const C* cols, std::size_t amt);

	/// \brief Remove a color at the index in the colors vector.
	void removeColor(ofIndexType index);

	/// \brief Remove colors at the index to the end index of the colors vector
	void removeColors(ofIndexType startIndex, ofIndexType endIndex);

	/// \brief Set the color at the index in the colors vector.
	void setColor(ofIndexType index, const C& c);

	/// \brief Clear all the colors.
	void clearColors();

	/// \returns the size of the colors vector for the mesh.
	/// This will tell you how many colors are contained in the mesh.
	std::size_t getNumColors() const;

	/// Use this if you plan to change the colors as part of this call as it will force a reset of the cache.
	/// \returns a pointer that contains all of the colors of the mesh, if it has any.
	C* getColorsPointer();

	/// \returns a pointer that contains all of the colors of the mesh, if it has any. (read only)
	const C* getColorsPointer() const;

	/// Use this if you plan to change the colors as part of this call as it will force a reset of the cache.
	/// \returns the vector that contains all of the colors of the mesh, if it has any.
	std::vector<C> & getColors();

	/// \returns the vector that contains all of the colors of the mesh, if it has any. (read only)
	const std::vector<C> & getColors() const;

	/// /returns Whether the mesh has any colors.
	bool hasColors() const;

	/// \brief Enable mesh colors.
	/// Use disableColors() to turn colors off.
	/// Colors are enabled by default when they are added to the mesh.
	virtual void enableColors();

	/// \brief Disable mesh colors.
	/// Use enableColors() to turn colors back on.
	virtual void disableColors();
	virtual bool usingColors() const;

	/// \}


	/// \name Texture Coordinates
	/// \{

	/// \returns the Vec2f representing the texture coordinate.
	/// Because OF uses ARB textures these are in pixels rather than
	/// 0-1 normalized coordinates.
	T getTexCoord(ofIndexType i) const;

	/// \brief Add a Vec2f representing the texture coordinate.
	/// Because OF uses ARB textures these are in pixels rather than
	/// 0-1 normalized coordinates.
	void addTexCoord(const T& t);

	/// \brief Add a vector of texture coordinates to a mesh,
	/// allowing you to push out many at once rather than adding one at a time.
	/// The vector of texture coordinates is added after the end of the current
	/// texture coordinates list.
	void addTexCoords(const std::vector<T>& tCoords);

	/// \brief  Add an array of texture coordinates to the mesh.
	/// Because you are using a pointer to the array you also have to define
	/// the length of the array as an std::size_t (amt).
	/// The texture coordinates are added at the end of the current texture
	/// coordinates list.
	void addTexCoords(const T* tCoords, std::size_t amt);

	/// \brief  Remove a Vec2f representing the texture coordinate.
	void removeTexCoord(ofIndexType index);

	void removeTexCoords(ofIndexType startIndex, ofIndexType endIndex);
	
	void setTexCoord(ofIndexType index, const T& t);

	/// \brief  Clear all the texture coordinates.
	void clearTexCoords();

	/// \brief This will tell you how many texture coordinates are contained in the mesh.
	/// \returns the size of the texture coordinates vector for the mesh.
	std::size_t getNumTexCoords() const;

	/// \returns a pointer to the texture coords that the mesh contains.
	T* getTexCoordsPointer();

	/// \brief Get a pointer to the glm::vec2 texture coordinates that the mesh contains.
	const T* getTexCoordsPointer() const;

	/// \brief Get a vector representing the texture coordinates of the mesh
	/// Because OF uses ARB textures these are in pixels rather than 0-1 normalized coordinates.
	/// Use this if you plan to change the texture coordinates as part of this
	/// call as it will force a reset of the cache.
	/// \returns a vector of Vec2f representing the texture coordinates for the whole mesh.
	std::vector<T> & getTexCoords();

	/// Because OF uses ARB textures these are in pixels rather than 0-1 normalized coordinates.
	/// \returns a vector of Vec2f representing the texture coordinates for the whole mesh. (read only)
	const std::vector<T> & getTexCoords() const;

	/// /returns Whether the mesh has any textures assigned to it.
	bool hasTexCoords() const;

	/// \brief Enable mesh textures.
	/// Use disableTextures() to turn textures off.
	/// Textures are enabled by default when they are added to the mesh.
	virtual void enableTextures();

	/// \brief Disable mesh textures.
	/// Use enableTextures() to turn textures back on.
	virtual void disableTextures();
	virtual bool usingTextures() const;


	/// \}

	/// \name Indices
	/// \{

	/// \brief Allow you to set up the indices automatically when you add a vertex.
	void setupIndicesAuto();

	/// \brief Use this if you plan to change the indices as part of this call as it
	/// will force a reset of the cache.
	/// \returns the vector that contains all of the indices of the mesh, if it has any.
	std::vector<ofIndexType> & getIndices();


	/// \returns the index from the index vector. Each index represents the index of the vertex in the vertices vector. This determines the way that the vertices are connected into the polgoynon type set in the primitiveMode.
	ofIndexType getIndex(ofIndexType i) const;


	/// Add an index to the index vector. Each index represents the order of
	/// connection for  vertices. This determines the way that the vertices are
	/// connected according to the polygon type set in the primitiveMode.
	/// It important to note that a particular vertex might be used for several
	/// faces and so would be referenced several times in the index vector.
	/// ~~~~{.cpp}
	/// ofMesh mesh;
	/// mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	/// mesh.addVertex(glm::vec3(0,-200,0));
	/// mesh.addVertex(glm::vec3(200, 0, 0 ));
	/// mesh.addVertex(glm::vec3(-200, 0, 0 ));
	/// mesh.addVertex(glm::vec3(0, 200, 0 ));
	/// mesh.addIndex(0); //connect the first vertex we made, v0
	/// mesh.addIndex(1); //to v1
	/// mesh.addIndex(2); //to v2 to complete the face
	/// mesh.addIndex(1); //now start a new face beginning with v1
	/// mesh.addIndex(2); //that is connected to v2
	/// mesh.addIndex(3); //and we complete the face with v3
	/// ~~~~

	/// Will give you this shape:
	/// ![image of basic use of indices](3d/index.jpg)
	void addIndex(ofIndexType i);

	/// \brief This adds a vector of indices.
	void addIndices(const std::vector<ofIndexType>& inds);

	/// \brief This adds indices to the ofMesh by pointing to an array of indices.
	/// The "amt" defines the length of the array.
	void addIndices(const ofIndexType* inds, std::size_t amt);

	/// \brief Removes an index.
	void removeIndex(ofIndexType index);
	void removeIndices(ofIndexType startIndex, ofIndexType endIndex);

	/// \brief This sets the index at i.
	void setIndex(ofIndexType index, ofIndexType val);

	/// \brief Remove all the indices of the mesh.
	/// This means that your mesh will be a point cloud.
	void clearIndices();

	/// \brief This will tell you how many indices are contained in the mesh.
	/// \returns the size of the indices vector for the mesh.
	std::size_t getNumIndices() const;

	/// \returns a pointer to the indices that the mesh contains.
	ofIndexType* getIndexPointer();

	/// \returns a pointer to the indices that the mesh contains.
	const ofIndexType* getIndexPointer() const;


	/// \returns the vector that contains all of the indices of the mesh, if it has any. (read only)
	const std::vector<ofIndexType> & getIndices() const;

	/// /returns Whether the mesh has any indices assigned to it.
	bool hasIndices() const;

	/// \brief Adding a triangle means using three of the vertices that have already been added to create a triangle.
	/// This is an easy way to create triangles in the mesh. The indices refer to the index of the vertex in the vector of vertices.
	void addTriangle(ofIndexType index1, ofIndexType index2, ofIndexType index3);

	/// \brief Enable mesh indices.
	/// Use disableIndices() to turn indices off.
	/// Indices are enabled by default when they are added to the mesh.
	virtual void enableIndices();

	/// \brief Disable mesh indices.
	/// Use enableIndices() to turn indices back on.
	virtual void disableIndices();
	virtual bool usingIndices() const;

	void setColorForIndices( ofIndexType startIndex, ofIndexType endIndex, C color );

	/// The new mesh includes the mesh mode, colors, textures, and normals of the original mesh (assuming any were added).
	/// \returns a mesh made up of a range of indices from startIndex to the endIndex.
	ofMesh_<V,N,C,T> getMeshForIndices( ofIndexType startIndex, ofIndexType endIndex ) const;
	ofMesh_<V,N,C,T> getMeshForIndices( ofIndexType startIndex, ofIndexType endIndex, ofIndexType startVertIndex, ofIndexType endVertIndex ) const;


	/// \}
	/// \name Drawing
	/// \{

	/// \brief This allows you draw just the vertices, meaning that you'll have a point cloud.
	void drawVertices() const;

	/// \brief This draws the mesh as GL_LINES, meaning that you'll have a wireframe.
	void drawWireframe() const;

	/// \brief This draws the mesh as faces, meaning that you'll have a collection of faces.
	void drawFaces() const;

	/// \brief This draws the mesh using its primitive type, meaning that if
	/// you set them up to be triangles, this will draw the triangles.
	void draw() const;

	/// \brief This draws the mesh using a defined renderType,
	/// overriding the renderType defined with setMode().
	virtual void draw(ofPolyRenderMode renderType) const;

	/// \}
	/// \name Saving and loading
	/// \{

	/// \brief Loads a mesh from a file located at the provided path into the mesh.
	/// This will replace any existing data within the mesh.
	///
	/// It expects that the file will be in the [PLY Format](http://en.wikipedia.org/wiki/PLY_(file_format)).
	/// It will only load meshes saved in the PLY ASCII format; the binary format is not supported.
    void load(const std::filesystem::path& path);

	///  \brief Saves the mesh at the passed path in the [PLY Format](http://en.wikipedia.org/wiki/PLY_(file_format)).
	///
	///  There are two format options for PLY: a binary format and an ASCII format.
	///  By default, it will save using the ASCII format.
	///  Passing ``true`` into the ``useBinary`` parameter will save it in the binary format.
	///
	///  If you're planning on reloading the mesh into ofMesh, ofMesh currently only supports loading the ASCII format.
	///
	///  For more information, see the [PLY format specification](http://paulbourke.net/dataformats/ply/).
    void save(const std::filesystem::path& path, bool useBinary = false) const;

	/// \}

protected:

	/// \returns If the vertices of the mesh have changed, been added or removed.
	bool haveVertsChanged();

	/// \returns If the normals of the mesh have changed, been added or removed.
	bool haveNormalsChanged();

	/// \returns If the colors of the mesh have changed, been added or removed.
	bool haveColorsChanged();

	/// \returns If the texture coords of the mesh have changed, been added or removed.
	bool haveTexCoordsChanged();

	/// \returns If the indices of the mesh have changed, been added or removed.
	bool haveIndicesChanged();

private:

	std::vector<V> vertices;
	std::vector<C> colors;
	std::vector<N> normals;
	std::vector<T> texCoords;
	std::vector<ofIndexType> indices;

	// this variables are only caches and returned always as const
	// mutable allows to change them from const methods
	mutable std::vector<ofMeshFace_<V,N,C,T>> faces;
	mutable bool bFacesDirty;

	bool bVertsChanged, bColorsChanged, bNormalsChanged, bTexCoordsChanged,
		bIndicesChanged;
	ofPrimitiveMode mode;

	bool useColors;
	bool useTextures;
	bool useNormals;
	bool useIndices;

//	ofMaterial *mat;
};


/// \brief An ofMeshFace_ is a face on one of the ofPrimitive instances.
/// In the ofPrimitive a face consists of 3 points connected together.
///
/// You can get a vector of ofMeshFace_ instances from any ofPrimitive like so:
///
/// ~~~~{.cpp}
///
/// vector<ofMeshFace_> triangles = box.getMesh().getUniqueFaces();
///
/// ~~~~
///
/// They're very handy for manipulating individual vertices or doing strange/fun
/// things with intersection and boundary testing, among many many other things.
///
/// this is always a triangle
template<class V, class N, class C, class T>
class ofMeshFace_ {
public:
	ofMeshFace_();

	const N & getFaceNormal() const;

	void setVertex( ofIndexType index, const V& v );
	const V& getVertex( ofIndexType index ) const;

	void setNormal( ofIndexType index, const N& n );
	const N& getNormal( ofIndexType  index ) const;

	void setColor( ofIndexType index, const C& color );
	const C& getColor(ofIndexType  index) const;

	void setTexCoord( ofIndexType index, const T& tCoord );
	const T& getTexCoord( ofIndexType index ) const;

	void setHasColors( bool bColors );
	void setHasNormals( bool bNormals );
	void setHasTexcoords( bool bTexcoords );

	bool hasColors() const;
	bool hasNormals() const;
	bool hasTexcoords() const;

private:
	void calculateFaceNormal() const;
	bool bHasNormals, bHasColors, bHasTexcoords;

	// this variables are only caches and returned always as const
	// mutable allows to change them from const methods
	mutable bool bFaceNormalDirty;
	mutable N faceNormal;
	V vertices[3];
	N normals[3];
	C colors[3];
	T texCoords[3];
};

#include "ofMesh.inl"

using ofMesh = ofMesh_<ofDefaultVertexType, ofDefaultNormalType, ofDefaultColorType, ofDefaultTexCoordType>;
using ofMeshFace = ofMeshFace_<ofDefaultVertexType, ofDefaultNormalType, ofDefaultColorType, ofDefaultTexCoordType>;

#endif
