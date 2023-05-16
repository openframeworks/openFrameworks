#pragma once

#include "ofGraphicsConstants.h"
#include "ofConstants.h"
#include "glm/fwd.hpp"
#include "ofColor.h"

class ofRectangle;
class ofPath;
class of3dPrimitive;
class ofLight;
class ofMaterial;
class ofBaseMaterial;
class ofCamera;
class ofTrueTypeFont;
class ofNode;
class of3dGraphics;
class ofVbo;
class ofVboMesh;
class ofSoundBuffer;
class ofFbo;
class ofBaseVideoDraws;

template<typename T>
class ofPixels_;
typedef ofPixels_<unsigned char> ofPixels;
typedef ofPixels_<float> ofFloatPixels;
typedef ofPixels_<unsigned short> ofShortPixels;
typedef ofPixels& ofPixelsRef;

template<typename T>
class ofImage_;
typedef ofImage_<unsigned char> ofImage;
typedef ofImage_<float> ofFloatImage;
typedef ofImage_<unsigned short> ofShortImage;

template<class T>
class ofPolyline_;
using ofPolyline = ofPolyline_<ofDefaultVertexType>;

template<class V, class N, class C, class T>
class ofMesh_;
using ofMesh = ofMesh_<ofDefaultVertexType, ofDefaultNormalType, ofDefaultColorType, ofDefaultTexCoordType>;

enum ofFboMode: short;
enum ofLoopType: short;
enum ofOrientation: short;

/// \brief Contains general information about the style of ofGraphics
/// elements such as color, line width and others.
class ofStyle{
	public:
		/// \brief Create a default ofStyle.
		ofStyle(){
			bFill				= true;
			blendingMode		= OF_BLENDMODE_ALPHA;
			smoothing			= false;
			circleResolution	= 20;
			sphereResolution	= 20;
			curveResolution		= 20;
			lineWidth			= 1.0;
			polyMode			= OF_POLY_WINDING_ODD;
			rectMode			= OF_RECTMODE_CORNER;
			#ifdef TARGET_OPENGLES
				drawBitmapMode		= OF_BITMAPMODE_SIMPLE;
			#else
				drawBitmapMode		= OF_BITMAPMODE_MODEL_BILLBOARD;
			#endif
			bgColor.set(60, 60, 60);
			//depthTest = false;
		}

		/// \brief Destroy the ofStyle.
		virtual ~ofStyle(){}

		/// \brief The color used when rendering.
		///
		/// This style depends on the state of the ofStyle::bFill.
		ofColor color;

		/// \brief The background color used when rendering.
		ofColor bgColor;

		/// \brief The current rendering mode for polygons.
		///
		/// ofPolyWindingMode determines how ovelapping parts of the same
		/// polygon are rendered.
		///
		/// \sa ofPolyWindingMode
		ofPolyWindingMode polyMode;

		/// \brief Determine how some shapes are anchored (e.g. ofDrawEllipse).
		///
		/// The rectMode applies to several shapes, including ofDrawRectangle
		/// and ofDrawEllipse.  If the current rectMode is `OF_RECTMODE_CENTER`,
		/// the rendered shape will be drawn with x, y in the center of the
		/// shape. If the current rectMode is `OF_RECTMODE_CORNER`, the
		/// rendered shape will be drawn with the x, y in the upper-left-hand
		/// corner of the shape.
		ofRectMode rectMode;

		/// \brief True if the renderer should fill a rendered shape.
		///
		/// If true, the renderer will fill the rendered shape.  If false, the
		/// renderer will draw the outline of the rendered shape.
		bool bFill;

		/// \brief The render mode for bitmaps (e.g. ofDrawBitmapString).
		///
		/// \sa ofDrawBitmapString(const T& textString, float x, float y)
		ofDrawBitmapMode drawBitmapMode;

		/// \brief The blending mode.
		ofBlendMode blendingMode;

		/// \brief True if anti-aliasing is enabled.
		bool smoothing;

		// \brief The resolution of rendered circles and arcs (e.g. ofCircle).
		int circleResolution;

		// \brief The resolution of rendered spheres (e.g. ofSphere).
		int sphereResolution;

		// \brief The resolution of rendered curves.
		int curveResolution;

		/// \brief The width of rendered lines.
		/// \warning This is not currently implemented in modern OF renderers.
		float lineWidth;

		//bool depthTest; removed since it'll break old projects setting depth test through glEnable
};

//----------------------------------------------------------
// ofBaseDraws
//----------------------------------------------------------

class ofBaseDraws{
public:
	/// \brief Destroy the abstract object.
	virtual ~ofBaseDraws(){}

	/// \brief Draw at a position at the native size.
	///
	/// Native size is determined by getWidth() and getHeight().
	///
	/// \param x Draw position on the x axis.
	/// \param y Draw position on the y axis.
	virtual void draw(float x, float y) const {
		draw(x, y, getWidth(), getHeight());
	}

	/// \brief Draw at a position with the specified size.
	///
	/// \param x Draw position on the x axis.
	/// \param y Draw position on the y axis.
	/// \param w Draw width.
	/// \param h Draw height.
	virtual void draw(float x, float y, float w, float h) const = 0;

	/// \brief Draw at a position at the native size.
	///
	/// Native size is determined by getWidth() and getHeight().
	///
	/// \param point Draw position.
	virtual void draw(const glm::vec2 & point) const;

	/// \brief Draw at a position and size specified by a rectangle.
	///
	/// \param rect Draw position and size.
	virtual void draw(const ofRectangle & rect) const;

	/// \brief Draw at a position.
	///
	/// \param point Draw position.
	/// \param w Draw width.
	/// \param h Draw height.
	virtual void draw(const glm::vec2 & point, float w, float h) const;

	/// \brief Get the height.
	/// \returns the height.
	virtual float getHeight() const = 0;

	/// \brief Get the width.
	/// \returns the width.
	virtual float getWidth() const = 0;

	/// \brief Set the anchor point the item is drawn around as a percentage.
	///
	/// This can be useful if you want to rotate an image around a particular
	/// point.
	///
	/// \param xPct Horizontal position as a percentage (0 - 1).
	/// \param yPct Vertical position as a percentage (0 - 1).
	virtual void setAnchorPercent(float xPct, float yPct){};

	/// \brief Set the anchor point the item is drawn around in pixels.
	///
	/// This can be useful if you want to rotate an image around a particular
	/// point.
	///
	/// \param x Horizontal texture position in pixels.
	/// \param y Vertical texture position in pixels.
	virtual void setAnchorPoint(float x, float y){};

	/// \brief Reset the anchor point to (0, 0).
	virtual void resetAnchor(){};

};

/// \brief An abstract class representing an object that can be updated.
class ofBaseUpdates{
public:
	/// \brief Destroy the ofBaseUpdates.
	virtual ~ofBaseUpdates(){}

	/// \brief Update the object's state.
	virtual void update()=0;
};

/// \brief An abstract class representing an object that has pixels.
///
/// This empty class primarily exists to allow templated subclasses of different
/// types to be stored as raw or shared pointers in collections such as
/// std::vector.
///
/// Example:
/// \code{.cpp}
///
/// std::vector<ofAbstractHasPixels> pixelProviders;
///
/// ofPixels pixels;
/// ofFloatPixels floatPixels;
/// ofShortPixels shortPixels;
///
/// // ...
///
/// pixelProviders.push_back(&pixels);
/// pixelProviders.push_back(&floatPixels);
/// pixelProviders.push_back(&shortPixels);
///
/// \endcode
class ofAbstractHasPixels{
public:
	/// \brief Destroy the ofAbstractHasPixels.
	virtual ~ofAbstractHasPixels(){}
};


/// \brief A base class represeting an object that has pixels.
/// \tparam T The pixel data type.
template<typename T>
class ofBaseHasPixels_: public ofAbstractHasPixels{
public:
	/// \brief Destroy the ofAbstractHasPixels.
	virtual ~ofBaseHasPixels_(){}

	/// \brief Get a reference to the underlying ofPixels.
	/// \returns a reference the underlying ofPixels.
	virtual ofPixels_<T> & getPixels()=0;

	/// \brief Get a const reference to the underlying ofPixels.
	/// \returns a const reference the underlying ofPixels.
	virtual const ofPixels_<T> & getPixels() const=0;
};

/// \brief A typedef for an unsigned char ofBaseHasPixels_.
typedef ofBaseHasPixels_<unsigned char> ofBaseHasPixels;

/// \brief A typedef for an float ofBaseHasPixels_.
typedef ofBaseHasPixels_<float> ofBaseHasFloatPixels;

/// \brief A typedef for an unsigned short ofBaseHasPixels_.
typedef ofBaseHasPixels_<unsigned short> ofBaseHasShortPixels;


/// \brief The base renderer interface.
class ofBaseRenderer{
public:
	virtual ~ofBaseRenderer(){}

	/// \brief Get the string representation of the renderer type.
	///
	/// For example, this method may return "GL", "ProgrammableGL", or another
	/// type depending on the renderer being used.
	///
	/// \returns The string representation of the renderer type.
	virtual const std::string & getType()=0;

	/// \brief Starts using this renderer as the rendering surface.
	virtual void startRender() = 0;
	/// \brief Stop using this renderer as the rendering surface.
	virtual void finishRender() = 0;

	/// \brief Draw a polyline with this renderer.
	/// \param poly The polyline to draw with this renderer.
	virtual void draw(const ofPolyline & poly) const=0;
	/// \brief Draw a path with this renderer.
	/// \param shape The path to draw with this renderer.
	virtual void draw(const ofPath & shape) const=0;
	/// \brief Draw a path with this renderer at \p x and \p y.
	/// \param shape The path to draw with this renderer.
	/// \param x The x coordinate to use to draw \p shape.
	/// \param y The y coordinate to use to draw \p shape.
	virtual void draw(const ofPath & shape, float x, float y) const{
		const_cast<ofBaseRenderer*>(this)->pushMatrix();
		const_cast<ofBaseRenderer*>(this)->translate(x,y);
		draw(shape);
		const_cast<ofBaseRenderer*>(this)->popMatrix();
	}
	/// \brief Draw a \p mesh with this renderer using the \p renderType.
	///
	/// \p renderType defines how the \p mesh will be rendered and may be:
	/// 	OF_MESH_POINTS,
	///		OF_MESH_WIREFRAME,
	///		OF_MESH_FILL
	///
	/// \param mesh The mesh to draw with this renderer.
	/// \param renderType The render mode to use to draw \p mesh with this
	/// renderer.
	/// \sa ofPolyRenderMode
	virtual void draw(const ofMesh & mesh, ofPolyRenderMode renderType) const;

	/// \brief Draw a mesh with this renderer.
	///
	/// \p renderType defines how the \p mesh will be rendered and may be:
	/// 	OF_MESH_POINTS,
	///		OF_MESH_WIREFRAME,
	///		OF_MESH_FILL
	///
	/// \param vertexData The mesh to draw with this renderer.
	/// \param renderType The render mode to use to draw \p vertexData to
	/// this renderer.
	/// \param useColors True to use per-vertex coloring to draw the
	/// \p vertexData.
	/// \param useTextures True to use texture coordinates to draw the \p
	/// vertexData.
	/// \param useNormals True to use normals to draw the \p vertexData.
	/// \sa ofPolyRenderMode
	virtual void draw(const ofMesh & vertexData, ofPolyRenderMode renderType, bool useColors, bool useTextures, bool useNormals) const=0;

	/// \brief Draw a \p model with this renderer using the \p renderType.
	///
	/// \p renderType defines how the \p model will be rendered and may be:
	/// 	OF_MESH_POINTS,
	///		OF_MESH_WIREFRAME,
	///		OF_MESH_FILL
	///
	/// \param model The model to draw with this renderer.
	/// \param renderType The render mode to use when drawing the \p model
	/// with this renderer.
	/// \sa ofPolyRenderMode
	virtual void draw(const of3dPrimitive& model, ofPolyRenderMode renderType) const=0;

	/// \brief Draw a node with this renderer using ofNode::customDraw().
	/// \param model The node to draw with this renderer.
	/// \sa ofNode::customDraw()
	virtual void draw(const ofNode& model) const=0;

	/// \brief Draw an \p image with this renderer.
	/// \param image The image to draw with this renderer.
	/// \param x The x coordinate to use when drawing \p image with this
	/// renderer.
	/// \param y The y coordinate to use to draw \p image with this renderer.
	/// \param z The z coordinate to use to drawing \p image with this renderer.
	/// \param w The width to use to draw the \p image with this renderer.
	/// \param h The height to use to draw the \p image with this renderer.
	/// \param sx The subsection x axis offset within the image texture.
	/// \param sy The subsection y axis offset within the image texture.
	/// \param sw The subsection width offset within the image texture.
	/// \param sh The subsection height offset within the image texture.
	virtual void draw(const ofImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const=0;

	/// \brief Draw an \p image with this renderer.
	/// \param image The image to draw with this renderer.
	/// \param x The x coordinate to use to draw \p image with this renderer.
	/// \param y The y coordinate to use to draw \p image with this renderer.
	/// \param z The z coordinate to use to draw \p image with this renderer.
	/// \param w The width to use to draw \p image with this renderer.
	/// \param h The height to use to draw \p image with this renderer.
	/// \param sx The subsection x axis offset within the image texture.
	/// \param sy The subsection y axis offset within the image texture.
	/// \param sw The subsection width offset within the image texture.
	/// \param sh The subsection height offset within the image texture.
	virtual void draw(const ofFloatImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const=0;

	/// \brief Draw an \p image with this renderer.
	/// \param image The image to draw with this renderer.
	/// \param x The x coordinate to use to draw \p image with this renderer.
	/// \param y The y coordinate to use to draw \p image with this renderer.
	/// \param z The z coordinate to use to draw \p image with this renderer.
	/// \param w The width to use when drawing the image with this renderer.
	/// \param h The height to use when drawing the image with this renderer.
	/// \param sx The subsection x axis offset within the image texture.
	/// \param sy The subsection y axis offset within the image texture.
	/// \param sw The subsection width offset within the image texture.
	/// \param sh The subsection height offset within the image texture.
	virtual void draw(const ofShortImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const=0;

	/// \brief Draw a \p video with this renderer.
	/// \param video The video with draw with this renderer.
	/// \param x The x coordinate to use to draw \p video with this renderer.
	/// \param y The y coordinate to use to draw \p video with this renderer.
	/// \param w The width to use to draw the video with this renderer.
	/// \param h The height to use to draw the video with this renderer.
	virtual void draw(const ofBaseVideoDraws & video, float x, float y, float w, float h) const=0;

	//--------------------------------------------
	// transformations
	/// \brief Push the current viewport into the renderer's viewport stack.
	///
	/// pushViewport() save the current viewport to the renderer's viewport
	/// history stack allowing new viewport operations to effect only the new
	/// viewport created by this method. Should be paired with popView.
	///
	/// \sa viewport()
	virtual void pushView()=0;

	/// \brief Pop the current viewport from the renderer's view stack.
	///
	/// popView() restores the renderer's viewport to the state it was last
	/// saved with with a call to pushView().
	///
	/// \sa viewport()
	virtual void popView()=0;

	/// \brief Set this renderer's viewport using a rectangle.
	///
	/// This method uses the ofGetWidth() and ofGetHeight() to set the width and
	/// height of the viewport. It also automatically creates near and far
	/// clipping planes based on this width and height.
	virtual void viewport(ofRectangle viewport)=0;

	/// \brief Set this renderer's viewport manually using x, y, width, and
	/// height.
	/// \param x The x coordinate of the viewport. Defaults to 0.
	/// \param y The y coordinate of the viewport. Defaults to 0.
	/// \param w The width of the viewport. Defaults to -1 setting its width
	/// according to the rendering surface's width.
	/// \param h The height of the viewport. Defaults to -1 setting its height
	/// according to the rendering surface's height.
	virtual void viewport(float x = 0, float y = 0, float width = -1, float height = -1, bool vflip=true)=0;

	/// \brief Setup the renderer to use a perspective matrix.
	/// \param width The width of the desired perspective matrix. Defaults to -1
	/// setting its width according to the rendering surface's width.
	/// \param height The height of the desired perspective matrix. Defaults to
	/// Defaults to -1 setting its width according to the rendering surface's
	/// width.
	/// \param fov The field of view to use with the perspective matrix.
	/// Defaults to 60.
	/// \param nearDist The near clipping distance to use with the perspective
	/// matrix. Setting this value to 0 uses the default near distance. Defaults
	/// to 0.
	/// \param farDist The far clipping distance to use with the perspective
	/// matrix. Setting this value to 0 uses the default near distance. Defaults
	/// to 0.
	virtual void setupScreenPerspective(float width = -1, float height = -1, float fov = 60, float nearDist = 0, float farDist = 0)=0;

	/// \brief Setup the renderer to use an orthographic matrix.
	/// \param width The width of the desired orthographic matrix. Defaults to
	/// -1 setting its width according to the rendering surface's width.
	/// \param height The height of the desired orthographic matrix. Defaults to
	/// -1 setting its height according to the rendering surface's height.
	/// \param nearDist The near clipping distance to use with the orthographic
	/// matrix. Setting this value to 0 uses the defualt near distance. Defaults
	/// to 0.
	/// \param farDist The far clipping distance to use with the orthographic
	/// matrix. Setting this value to 0 uses the defualt near distance. Defaults
	/// to 0.
	virtual void setupScreenOrtho(float width = -1, float height = -1, float nearDist = -1, float farDist = 1)=0;

	/// \brief set this renderer's orientation.
	///
	/// Possible orientation values include:
	/// 	OF_ORIENTATION_DEFAULT
	///		OF_ORIENTATION_180
	///		OF_ORIENTATION_90_LEFT
	///		OF_ORIENTATION_90_RIGHT
	///		OF_ORIENTATION_UNKNOWN
	///
	/// \param orientation The orientation to use with this renderer.
	/// \param vFlip True if the orientation should be vertically flipped.
	/// \sa ofOrientation
	virtual void setOrientation(ofOrientation orientation, bool vFlip)=0;

	/// \brief Get this renderer's current viewport.
	///
	/// Unlike getNativeViewport(), this method gets this renderer's current
	/// viewport with orientation and vertical flipping applied.
	///
	/// \returns This renderer's viewport as a rectangle.
	/// \sa getNativeViewport()
	virtual ofRectangle getCurrentViewport() const=0;

	/// \brief Get this renderer's current native viewport.
	///
	/// Unlike getViewport(), this method gets this renderer's current viewport
	/// without orientation and vertical flipping applied.
	///
	/// \returns This renderer's native viewport as a rectangle.
	/// \sa getViewport()
	virtual ofRectangle getNativeViewport() const=0;

	/// \brief Get the renderer's current viewport width.
	/// \returns The renderer's current viewport width.
	virtual int getViewportWidth() const=0;

	/// \brief Get the renderer's current viewport width.
	/// \returns The renderer's current viewport width.
	virtual int getViewportHeight() const=0;

	/// \brief Returns true if the renderer's current viewport is vertically
	/// flipped.
	/// \returns True if the renderer's current viewport is vertically flipped.
	virtual bool isVFlipped() const=0;

	/// \brief Set the coordinate handidness of this renderer.
	///
	/// Possible handednesses include OF_LEFT_HANDED and OF_RIGHT_HANDED.
	/// In a left handed coordinate system positive x, y and z axes point right,
	/// up and forward, respectively and positive rotation is clockwise about
	/// the axis of rotation. In a right handed coordinate system the positive x
	/// and y axes point right and up, and the negative z axis points forward
	/// and positive rotation is counterclockwise about the axis of rotation.
	///
	/// \param handedness The handedness to set this renderer to use.
	/// \sa http://seanmiddleditch.com/matrices-handedness-pre-and-post-multiplication-row-vs-column-major-and-notations/
	/// \sa https://www.evl.uic.edu/ralph/508S98/coordinates.html
	virtual void setCoordHandedness(ofHandednessType handedness)=0;

	/// \brief Get the coordinate handidness of this renderer.
	///
	/// Possible handednesses include OF_LEFT_HANDED and OF_RIGHT_HANDED.
	/// In a left handed coordinate system positive x, y and z axes point right,
	/// up and forward, respectively and positive rotation is clockwise about
	/// the axis of rotation. In a right handed coordinate system the positive x
	/// and y axes point right and up, and the negative z axis points forward
	/// and positive rotation is counterclockwise about the axis of rotation.
	///
	/// \returns The handedness this renderer is using.
	/// \sa http://seanmiddleditch.com/matrices-handedness-pre-and-post-multiplication-row-vs-column-major-and-notations/
	/// \sa https://www.evl.uic.edu/ralph/508S98/coordinates.html
	virtual ofHandednessType getCoordHandedness() const=0;

	//our openGL wrappers

	/// \brief Pushes this renderer's matrix stack down by one.
	///
	/// pushMatrix() saves the renderer's current matrix allowing new
	/// transformations and scales to effect only the new matrix created by
	/// this method. Should be paired with a call to popMatrix().
	///
	virtual void pushMatrix()=0;
	/// \brief Pops this renderer's current matrix stack.
	///
	/// popMatrix() restores the renderer's matrix to the state it was last
	/// saved with with a call to pushMatrix().
	///
	virtual void popMatrix()=0;

	/// \brief Get this renderer's current matrix for particular a matrix mode.
	///
	/// Possible matrix modes include:
	///		OF_MATRIX_MODELVIEW
	///		OF_MATRIX_PROJECTION
	///		OF_MATRIX_TEXTURE
	///
	/// \param matrixMode_ The matrix mode to get the current matrix of.
	/// \returns The current matrix specified by \p matrixMode_
	/// \sa ofMatrixMode
	virtual glm::mat4 getCurrentMatrix(ofMatrixMode matrixMode_) const=0;

	/// \brief Get this renderer's current orientation matrix.
	/// \returns This renderer's current orientation matrix.
	virtual glm::mat4 getCurrentOrientationMatrix() const=0;

	/// \brief Translate this renderer's current matrix by x, y, and z.
	/// \param x The x coordinate to translate this renderer's current matrix
	/// by.
	/// \param y The y coordinate to translate this renderer's current matrix
	/// by.
	/// \param z The z coordinate to translate this renderer's current matrix
	/// by. Defaults to 0.
	virtual void translate(float x, float y, float z = 0)=0;

	/// \brief Translate this renderer's current matrix by a point.
	/// \param p The 3D point to translate this renderer's current matrix by.
	virtual void translate(const glm::vec3 & p)=0;

	/// \brief Scale this renderer's current matrix by xAmnt, yAmnt, and zAmnt.
	/// \param xAmnt The amount to scale this renderer's current matrix's x
	/// axis by.
	/// \param yAmnt The amount to scale this renderer's current matrix's y
	/// axis by.
	/// \param zAmnt The amount to scale this renderer's current matrix's z
	/// axis by. Defaults to 1.
	virtual void scale(float xAmnt, float yAmnt, float zAmnt = 1)=0;

	/// \brief Rotate this renderer's current matrix by \p degrees about a euler.
	/// \param degrees Degrees to rotate about vecX, vecY, and vecZ.
	/// \param vecX The x axis to rotate about.
	/// \param vecY The y axis to rotate about.
	/// \param vecZ The z axis to rotate about.
	virtual void rotateDeg(float degrees, float vecX, float vecY, float vecZ);

	/// \brief Rotate this renderer's current matrix by \p degrees about the x
	/// axis.
	/// \param degrees Degrees to rotate about the x axis.
	virtual void rotateXDeg(float degrees);

	/// \brief Rotate this renderer's current matrix by \p degrees about the y
	/// axis.
	/// \param degrees Degrees to rotate about the y axis.
	virtual void rotateYDeg(float degrees);

	/// \brief Rotate this renderer's current matrix by \p degrees about the z
	/// axis.
	/// \param degrees Degrees to rotate about the z axis.
	virtual void rotateZDeg(float degrees);

	/// \brief Rotate this renderer's current matrix by \p degrees about the z
	/// axis.
	///
	/// This method is an alias of rotateZ().
	///
	/// \param degrees Degrees to rotate about the z axis.
	/// \sa rotateZ()
	virtual void rotateDeg(float degrees);

	/// \brief Rotate this renderer's current matrix by \p degrees about a euler.
	/// \param degrees Degrees to rotate about vecX, vecY, and vecZ.
	/// \param vecX The x axis to rotate about.
	/// \param vecY The y axis to rotate about.
	/// \param vecZ The z axis to rotate about.
	virtual void rotateRad(float degrees, float vecX, float vecY, float vecZ)=0;

	/// \brief Rotate this renderer's current matrix by \p degrees about the x
	/// axis.
	/// \param degrees Degrees to rotate about the x axis.
	virtual void rotateXRad(float degrees)=0;

	/// \brief Rotate this renderer's current matrix by \p degrees about the y
	/// axis.
	/// \param degrees Degrees to rotate about the y axis.
	virtual void rotateYRad(float degrees)=0;

	/// \brief Rotate this renderer's current matrix by \p degrees about the z
	/// axis.
	/// \param degrees Degrees to rotate about the z axis.
	virtual void rotateZRad(float degrees)=0;

	/// \brief Rotate this renderer's current matrix by \p degrees about the z
	/// axis.
	///
	/// This method is an alias of rotateZ().
	///
	/// \param degrees Degrees to rotate about the z axis.
	/// \sa rotateZ()
	virtual void rotateRad(float degrees)=0;

	/// \brief Sets this renderer's current matrix mode.
	///
	/// The possible matrix modes include:
	///		OF_MATRIX_MODELVIEW
	///		OF_MATRIX_PROJECTION
	///		OF_MATRIX_TEXTURE
	///
	/// \param mode The matrix mode this renderer's matrix should use.
	virtual void matrixMode(ofMatrixMode mode)=0;

	/// \brief Load this renderer's identity matrix.
	///
	/// This identity matrix is an mat4 matrix with 1s on the main
	/// diagonal and 0s elsewhere.
	/// [
	///		[1, 0, 0, 0],
	///		[0, 1, 0, 0],
	///		[0, 0, 1, 0],
	///		[0, 0, 0, 1]
	///	]
	///
	/// Matrix multiplications using this matrix as the multiplier will yield no
	/// change in the multiplicand matrix.
	virtual void loadIdentityMatrix (void)=0;

	/// \brief Load a matrix as this renderer's current matrix.
	/// \param m The matrix to load into this renderer.
	virtual void loadMatrix (const glm::mat4 & m)=0;

	/// \brief Load m as this renderer's current matrix.
	///
	/// \p m can be passed to loadMatrix() in this way from raw data
	///
	/// \param m Float pointer to an 4x4 matrix.
	virtual void loadMatrix (const float *m)=0;

	/// \brief Multiply this renderer's current matrix by \p m.
	/// \param m The matrix to multiply this renderer's current matrix by.
	virtual void multMatrix (const glm::mat4 & m)=0;

	/// \brief Multiply this renderer's current matrix by \p m.
	///
	/// \p m can be passed to loadMatrix() in this way with raw data
	///
	/// \param m Float pointer to an mat4 to multiply this renderer's
	/// current matrix by.
	virtual void multMatrix (const float *m)=0;

	/// \brief Load \p m into this renderer's matrix stack as a view matrix.
	/// \param m The view matrix to load into this renderer's matrix stack.
	virtual void loadViewMatrix(const glm::mat4 & m)=0;

	/// \brief Multiply this renderer's view matrix by \p m.
	/// \param m The matrix to multiply this renderer's view matrix by.
	virtual void multViewMatrix(const glm::mat4 & m)=0;

	/// \brief Get this renderer's current view matrix.
	/// \returns This renderer's current view matrix.
	virtual glm::mat4 getCurrentViewMatrix() const=0;

	/// \brief Get this renderer's current normal matrix.
	/// \returns This renderer's current normal matrix.
	virtual glm::mat4 getCurrentNormalMatrix() const=0;


	/// \brief Bind \p camera's matrices to this renderer's matrix stack.
	///
	/// Bind's the \p camera's modelview and projection matrices to this
	/// renderer's matrix stack using \p viewport. Should be followed with a
	/// call to unbind().
	///
	/// \param camera The camera to bind to this renderer.
	/// \param viewport The viewport to use when binding \p camera to this
	/// renderer.
	virtual void bind(const ofCamera & camera, const ofRectangle & viewport)=0;
	/// \brief Unbind the camera from this renderer.
	/// \param camera The camera to unbind from this renderer.
	virtual void unbind(const ofCamera & camera)=0;


	/// \brief setup the default graphics settings for this renderer.
	virtual void setupGraphicDefaults()=0;
	/// \brief setup the default screen settings for this renderer.
	virtual void setupScreen()=0;

	// drawing modes
	/// \brief Set this renderer's rect mode.
	///
	/// Possible rect modes include OF_RECTMODE_CORNER and OF_RECTMODE_CENTER.
	///
	/// \param mode The rect mode to request this renderer to use.
	/// \sa ofRectMode
	virtual void setRectMode(ofRectMode mode)=0;
	/// \brief Get this renderer's current rect mode.
	///
	/// Possible rect modes include OF_RECTMODE_CORNER and OF_RECTMODE_CENTER.
	///
	/// \returns The renderer's current rect mode.
	/// \sa ofRectMode
	virtual ofRectMode getRectMode()=0;
	/// \brief set this renderer's fill flag.
	///
	/// Possible fill flags include OF_OUTLINE and OF_FILLED.
	///
	/// \param fill The fill flag to request this renderer to use.
	/// \sa ofFillFlag
	virtual void setFillMode(ofFillFlag fill)=0;
	/// \brief Get this renderer's current fill flag.
	///
	/// Possible fill flags include OF_OUTLINE and OF_FILLED.
	///
	/// \returns The fill flag this render is currently using.
	/// \sa ofFillFlag
	virtual ofFillFlag getFillMode()=0;
	/// \brief Set the line width this renderer should use when drawing lines.
	/// \param lineWidth The line width to request this renderer to use.
	virtual void setLineWidth(float lineWidth)=0;
	/// \brief Enable/disable depth testing with this renderer.
	///
	/// When depth testing is enabled the order shapes are drawn with the
	/// renderer is dependent on their distance from the camera rather than the
	/// order their drawing methods were called. This should be enabled when
	/// expecting normal behavior when drawing 3D scenes.
	///
	/// \param depthTest True to enable depth testing.
	virtual void setDepthTest(bool depthTest)=0;
	/// \brief Set this renderer's current blend mode.
	///
	/// Possible blend modes include:
	/// \brief Rotate this renderer's current matrix by \p degrees about the z
	/// axis.
	///
	/// This method is an alias of rotateZ().
	///
	/// \param degrees Degrees to rotate about the z axis.
	/// \sa rotateZ()
	///		OF_BLENDMODE_DISABLED
	///		OF_BLENDMODE_ALPHA
	///		OF_BLENDMODE_ADD
	///		OF_BLENDMODE_SUBTRACT
	///		OF_BLENDMODE_MULTIPLY
	///		OF_BLENDMODE_SCREEN
	///
	/// \param blendMode The blend mode to request this renderer to use.
	/// \sa ofBlendMode
	/// \sa https://helpx.adobe.com/photoshop/using/blending-modes.html
	virtual void setBlendMode(ofBlendMode blendMode)=0;
	/// \brief Enable/disable line smoothing for this renderer if it's supported.
	/// \param smooth True to enable line smoothing for this renderer if it's
	/// supported.
	virtual void setLineSmoothing(bool smooth)=0;
	/// \brief Set the resolution to use when drawing ellipses with this
	/// renderer.
	/// \param res The number of points to use when drawing circles and ellipses
	/// with this renderer.
	virtual void setCircleResolution(int res)=0;
	/// \brief Enable this renderer to use anti-aliasing if it is supported.
	virtual void enableAntiAliasing()=0;
	/// \brief Disable this renderer from using anti-aliasing.
	virtual void disableAntiAliasing()=0;

	/// \brief Set the global color this renderer will use when drawing.
	///
	/// The renderer will continue using a color set by setColor() until another
	/// call to setColor() changes the drawing color.
	///
	/// \param r The red value between 0 and 255 to use when drawing.
	/// \param g The green value between 0 and 255 to use when drawing.
	/// \param b The blue value between 0 and 255 to use when drawing.
	virtual void setColor(int r, int g, int b)=0;
	/// \brief Set the global color this renderer will use when drawing.
	///
	/// The renderer will continue using a color set by setColor() until another
	/// call to setColor() changes the drawing color.
	///
	/// \param r The red value between 0 and 255 to use when drawing.
	/// \param g The green value between 0 and 255 to use when drawing.
	/// \param b The blue value between 0 and 255 to use when drawing.
	/// \param a The alpha value between 0 and 255 to use when drawing.
	virtual void setColor(int r, int g, int b, int a)=0;
	/// \brief Set the global color this renderer will use when drawing.
	///
	/// The renderer will continue using a color set by setColor() until another
	/// call to setColor() changes the drawing color.
	///
	/// \param color The color to use when drawing.
	virtual void setColor(const ofColor & color)=0;
	/// \brief Set the global color this renderer will use when drawing.
	///
	/// The renderer will continue using a color set by setColor() until another
	/// call to setColor() changes the drawing color.
	///
	/// \param color The color to use when drawing.
	/// \param _a The alpha value between 0 and 255 to use when drawing.
	virtual void setColor(const ofColor & color, int _a)=0;
	/// \brief Set the global color this renderer will use when drawing.
	///
	/// The renderer will continue using a color set by setColor() until another
	/// call to setColor() changes the drawing color.
	///
	/// \param gray The grayscale value to use when drawing.
	virtual void setColor(int gray)=0;
	/// \brief Set the global color this renderer will use when drawing.
	///
	/// The renderer will continue using a color set by setColor() until another
	/// call to setColor() changes the drawing color.
	///
	/// \param hexColor The hexidecimal representation of the color to use when
	/// drawing.
	virtual void setHexColor( int hexColor )=0;

	/// \brief Set this renderer's bitmap text mode.
	///
	/// Possible bitmap texture modes include:
	///		OF_BITMAPMODE_SIMPLE
	///		OF_BITMAPMODE_SCREEN
	///		OF_BITMAPMODE_VIEWPORT
	///		OF_BITMAPMODE_MODEL
	///		OF_BITMAPMODE_MODEL_BILLBOARD
	///
	/// \param mode The bitmap mode to request this renderer to use.
	/// \sa ofDrawBitmapMode
	virtual void setBitmapTextMode(ofDrawBitmapMode mode)=0;

	/// \brief Get this renderer's current background color.
	/// \returns This renderer's current background color.
	virtual ofColor getBackgroundColor()=0;
	/// \brief Set this renderer's background color.
	/// \param c The color to request this renderer to use.
	virtual void setBackgroundColor(const ofColor & c)=0;

	/// \brief Immediately paint a background color to the screen.
	///
	/// If automatic background drawing is enabled (which it is by default) this
	/// method called from ofApp::setup() will also repaint the background with
	/// this color each frame.
	///
	/// \param c The color to paint the background with.
	virtual void background(const ofColor & c)=0;
	/// \brief Immediately paint a grayscale background color to the screen.
	///
	/// If automatic background drawing is enabled (which it is by default) this
	/// method called from ofApp::setup() will also repaint the background with
	/// this color each frame.
	///
	/// \param brightness The grayscale value between 0 and 255 to paint the
	/// background with.
	virtual void background(float brightness)=0;
	/// \brief Immediately paint a grayscale background color to the screen.
	///
	/// If automatic background drawing is enabled (which it is by default) this
	/// method called from ofApp::setup() will also repaint the background with
	/// this color each frame.
	///
	/// \param hexColor The 24-bit hex representation of the color to paint the
	/// background with.
	/// \param _a The alpha value between 0 and 255 to apply to \p hexColor when
	/// when painting the background.
	virtual void background(int hexColor, float _a=255.0f)=0;
	/// \brief Immediately paint a background color to the screen.
	///
	/// \param r The red value between 0 and 255 to use for the background.
	/// \param g The green value between 0 and 255 to use for the background.
	/// \param b The blue value between 0 and 255 to use for the background.
	/// \param a The alpha value between 0 and 255 to use for the background.
	virtual void background(int r, int g, int b, int a=255)=0;

	/// \brief Enable/disable automatic redrawing of the background each frame.
	/// \param bManual False to disable automatic background redrawing.
	virtual void setBackgroundAuto(bool bManual)=0;
	/// \brief Get the current auto redraw background setting for this renderer.
	/// \returns True if this renderer is set to redraw the background each
	/// frame.
	virtual bool getBackgroundAuto()=0;

	/// \brief Clear this renderer's current color and bit depths.
	///
	/// clear() will clear the screen entirely.
	///
	virtual void clear()=0;
	/// \brief Clear this renderer's color and bit depths and replace them.
	///
	/// clear() will clear the screen entirely.
	///
	/// \param r The red value between 0 and 255 to use when clearing the
	/// screen.
	/// \param g The green value between 0 and 255 to use when clearing the
	/// screen.
	/// \param b The blue value between 0 and 255 use when clearing the screen.
	/// \param a The alpha value between 0 and 255 use when clearing the screen.
	/// Defaults to 0.
	virtual void clear(float r, float g, float b, float a=0)=0;
	/// \brief Clear this renderer's color and bit depths replacing them.
	///
	/// clear() will clear the screen entirely.
	///
	/// \param brightness The grayscale value between 0 and 255 to use when
	/// clearing the screen.
	/// \param a The alpha value between 0 and 255 to use when clearing the
	/// screen. Defaults to 0.
	virtual void clear(float brightness, float a=0)=0;
	/// \brief Restore the alpha color to its full opacity value.
	virtual void clearAlpha()=0;

	/// \brief Draw a line between two 3D points.
	/// \param x1 The x coordinate of the first point.
	/// \param y1 The y coordinate of the first point.
	/// \param z1 The z coordinate of the first point.
	/// \param x2 The x coordinate of the second point.
	/// \param y2 The y coordinate of the second point.
	/// \param z2 The z coordinate of the second point.
	virtual void drawLine(float x1, float y1, float z1, float x2, float y2, float z2) const=0;
	/// \brief Draw a rectangle using a 3D point and a width and height.
	/// \param x The x coordinate of the rectangle.
	/// \param y The y coordinate of the rectangle.
	/// \param z The z coordinate of the rectangle.
	/// \param w The width of the rectangle.
	/// \param h The height of the rectangle.
	/// \sa ofRectMode
	virtual void drawRectangle(float x, float y, float z, float w, float h) const=0;
	/// \brief Draw a triangle using three 3D points.
	/// \param x1 The x coordinate of the first point.
	/// \param y1 The y coordinate of the first point.
	/// \param z1 The z coordinate of the first point.
	/// \param x2 The x coordinate of the second point.
	/// \param y2 The y coordinate of the second point.
	/// \param z2 The z coordinate of the second point.
	/// \param x3 The x coordinate of the third point.
	/// \param y3 The y coordinate of the third point.
	/// \param z3 The z coordinate of the third point.
	virtual void drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) const=0;
	/// \brief Draw a circle using a 3D point and a radius.
	/// \param x The x coordinate of the center of the circle.
	/// \param y The y coordinate of the center of the circle.
	/// \param z The z coordinate of the center of the circle.
	/// \param radius The length of the radius of the circle.
	virtual void drawCircle(float x, float y, float z, float radius) const=0;
	/// \brief Draw an ellipse using a 3D point, width, and height.
	/// \param x The x coordinate of the center of the circle.
	/// \param y The y coordinate of the center of the circle.
	/// \param z The z coordinate of the center of the circle.
	/// \param width The width of the circle.
	/// \param height The height of the circle.
	virtual void drawEllipse(float x, float y, float z, float width, float height) const=0;
	/// \brief Draw text with this renderer using the current bitmap text mode.
	///
	/// When using the OF_BITMAPMODE_SIMPLE bitmap text strings are drawn with
	/// their origin at the bottom left corner of the text. However,
	/// setBitmapTextMode() can transform this default behavior with modes like:
	///		OF_BITMAPMODE_SIMPLE
	///		OF_BITMAPMODE_SCREEN
	///		OF_BITMAPMODE_VIEWPORT
	///		OF_BITMAPMODE_MODEL
	///		OF_BITMAPMODE_MODEL_BILLBOARD
	///
	/// \param text The text to draw with the renderer.
	/// \param x The x position for the bottom of \p text.
	/// \param y The y position for the left alignment of \p text.
	/// \param z The z position of the text.
	virtual void drawString(std::string text, float x, float y, float z) const=0;
	/// \brief Draw text with this renderer using an ofTrueType font.
	/// \param font The font to use when drawing \p text.
	/// \param text The text to draw with the renderer.
	/// \param x The x position for the bottom of \p text.
	/// \param y The y position for the left alignment of \p text.
	virtual void drawString(const ofTrueTypeFont & font, std::string text, float x, float y) const=0;


	// returns true an ofPath to draw with, this allows to keep
	// immediate mode rendering working in multiwindow with multiple
	// contexts without reimplementing the logic on every renderer
	/// \brief Get a reference to the path used internally by this renderer.
	/// \returns A reference to the path used internally by this renderer.
	virtual ofPath & getPath()=0;

	/// \brief Get this renderer's current style object.
	/// \returns This renderer's current style object.
	virtual ofStyle getStyle() const=0;
	/// \brief Set this renderer's current style object.
	/// \param style The style object to set this renderer to use.
	virtual void setStyle(const ofStyle & style) = 0;
	/// \brief Push this renderer's current style into its internal style stack.
	///
	/// This creates a new style object used by this renderer internally until
	/// popStyle() is called.
	///
	virtual void pushStyle()=0;
	/// \brief Pop this renderer's current style from its internal style stack.
	///
	/// This restores the style that was last saved with pushStyle().
	///
	virtual void popStyle()=0;

	/// \brief Set the resolution used when drawing curves with this renderer.
	/// \param resolution The resolution to request this renderer to use when
	/// drawing curves.
	virtual void setCurveResolution(int resolution)=0;

	/// \brief Set this renderer's poly winding mode.
	///
	/// Possible poly winding modes include:
	///		OF_POLY_WINDING_ODD
	///		OF_POLY_WINDING_NONZERO
	///		OF_POLY_WINDING_POSITIVE
	///		OF_POLY_WINDING_NEGATIVE
	///		OF_POLY_WINDING_ABS_GEQ_TWO
	///
	/// \param mode The poly winding mode to request this renderer to use.
	/// \sa ofPolyWindingMode
	virtual void setPolyMode(ofPolyWindingMode mode)=0;

	/// \brief Get a const reference of this renderer's 3D graphics object.
	/// \returns The 3D graphics object currently being used by this renderer.
	virtual const of3dGraphics & get3dGraphics() const=0;
	/// \brief Get a reference with this renderer's 3D graphics object.
	/// \returns the 3D graphics object currently being used by this renderer.
	virtual of3dGraphics & get3dGraphics()=0;


	/// \brief Set this renderer's plane resolution using \p column and \p rows.
	/// \param columns The number of columns to use when drawing planes with
	/// this renderer.
	/// \param rows The number of rows to use when drawing planes with this
	/// renderer.
	virtual void setPlaneResolution( int columns, int rows );
	/// \brief Get this renderer's current plane resolution as a 2D vector.
	///
	/// The resulting vector's x and y values corresponds to the current column
	/// and row resolutions of this renderer's plane resolution respectively.
	///
	/// \returns A 2D vector representing this renderer's plane resolution in
	/// columns and rows.
	virtual glm::vec2 getPlaneResolution() const;
	/// \brief Draw a plane with the renderer using x, y, width, and height.
	///
	/// The number of rows and columns this plane will have is dependent on this
	/// renderer's current plane resolution set with setPlaneResolution().
	///
	/// \param x The x coordinate to use when drawing the plane with this
	/// renderer.
	/// \param y The y coordinate to use when drawing the plane with this
	/// renderer.
	/// \param width The width to use when drawing the plane with this renderer.
	/// \param height The height to use when drawing the plane with this
	/// renderer.
	virtual void drawPlane(float x, float y, float width, float height) const;
	/// \brief Draw a plane with the renderer using x, y, z, width, and height.
	///
	/// The number of rows and columns this plane will have is dependent on this
	/// renderer's current plane resolution set with setPlaneResolution().
	///
	/// \param x The x coordinate to use when drawing the plane with this
	/// renderer.
	/// \param y The y coordinate to use when drawing the plane with this
	/// renderer.
	/// \param z The z coordinate to use when drawing the plane with this
	/// renderer.
	/// \param width The width to use when drawing the plane with this renderer.
	/// \param height The height to use when drawing the plane with this
	/// renderer.
	virtual void drawPlane(float x, float y, float z, float width, float height) const;
	/// \brief Draw a plane with the renderer using a 3D point, width, and height.
	///
	/// The number of rows and columns this plane will have is dependent on this
	/// renderer's current plane resolution set with setPlaneResolution().
	///
	/// \param position A 3D point to use as the position to draw the plane with
	/// this renderer.
	/// \param width The width to use when drawing the plane with this renderer.
	/// \param height The height to use when drawing the plane with this
	/// renderer.
	virtual void drawPlane(const glm::vec3& position, float width, float height) const;
	/// \brief Draw a plane with the renderer at the origin.
	///
	/// The number of rows and columns this plane will have is dependent on this
	/// renderer's current plane resolution set with setPlaneResolution().
	///
	/// \param width The width of the plane to use when drawing the plane with
	/// this renderer.
	/// \param height The height to use when drawing the plane with this
	/// renderer.
	virtual void drawPlane( float width, float height ) const;

	/// UV Sphere
	/// \brief Set the point resolution to use when drawing a sphere with this
	/// renderer.
	/// \param res The desired sphere resolution to use with this renderer.
	virtual void setSphereResolution(int res);
	/// \brief Get this renderer's current sphere resolution.
	/// \returns This renderer's current sphere resolution.
	virtual int getSphereResolution() const;
	/// \brief Draw a sphere with this renderer using x, y, and radius.
	///
	/// Spheres are drawn with x, y, and z coordinates representing the center
	/// of the sphere.
	///
	/// \param x The x coordinate to use when drawing this sphere.
	/// \param y The y coordinate to use when drawing this sphere.
	/// \param radius The radius to use when drawing this sphere.
	virtual void drawSphere(float x, float y, float radius) const;
	/// \brief Draw a sphere with this renderer using x, y, z, and radius.
	///
	/// Spheres are drawn with x, y, and z coordinates representing the center
	/// of the sphere.
	///
	/// \param x The x coordinate to use when drawing this sphere.
	/// \param y The y coordinate to use when drawing this sphere.
	/// \param z The z coordinate to use when drawing this sphere.
	/// \param radius The radius to use when drawing this sphere.
	virtual void drawSphere(float x, float y, float z, float radius) const;
	/// \brief Draw a sphere with this renderer using a position point and
	/// radius.
	///
	/// Spheres are drawn with x, y, and z coordinates representing the center
	/// of the sphere.
	///
	/// \param position The 3D position point to use when drawing the sphere.
	/// \param radius The radius to use when drawing this sphere.
	virtual void drawSphere(const glm::vec3& position, float radius) const;
	/// \brief Draw a sphere with the renderer at the defualt origin using
	/// radius.
	/// \param radius The radius to use when drawing the sphere with this
	/// renderer.
	virtual void drawSphere(float radius) const;

	// Ico Sphere
	/// \brief Set the point resolution to use when drawing an icosphere with
	/// this renderer.
	/// \param res The desired icosphere resolution to use with this renderer.
	virtual void setIcoSphereResolution( int res );
	/// \brief Get this renderer's current icosphere resolution.
	/// \returns This renderer's current icosphere resolution.
	virtual int getIcoSphereResolution() const;
	/// \brief Draw an icosphere with this renderer using x, y, and radius.
	///
	/// Spheres are drawn with x, y, and z coordinates representing the center
	/// of the icosphere.
	///
	/// \param x The x coordinate to use when drawing this icosphere.
	/// \param y The y coordinate to use when drawing this icosphere.
	/// \param z The z coordinate to use when drawing this icosphere.
	/// \param radius The radius to use when drawing this icosphere.
	virtual void drawIcoSphere(float x, float y, float z, float radius) const;
	/// \brief Draw an icosphere with this renderer using x, y, and radius.
	///
	/// Spheres are drawn with x, y, and z coordinates representing the center
	/// of the icosphere.
	///
	/// \param x The x coordinate to use when drawing this icosphere.
	/// \param y The y coordinate to use when drawing this icosphere.
	/// \param radius The radius to use when drawing this icosphere.
	virtual void drawIcoSphere(float x, float y, float radius) const;
	/// \brief Draw an icosphere with this renderer using x, y, and radius.
	///
	/// Icospheres are drawn with x, y, and z coordinates representing the
	/// center of the icosphere.
	///
	/// \param position The 3D position point to use when drawing the icosphere.
	/// \param radius The radius to use when drawing this icosphere.
	virtual void drawIcoSphere(const glm::vec3& position, float radius) const;
	/// \brief Draw an icosphere with the renderer at the origin using radius.
	/// \param radius The radius to use when drawing the icosphere with this
	/// renderer.
	virtual void drawIcoSphere(float radius) const;

	/// \brief Set this renderer's cylinder resolution.
	/// \param radiusSegments The number of facets (subdivisions) around the
	/// icosphere's circular footprint. A larger number yields a higher
	/// resolution.
	/// \param heightSegments The number of subdivisions from the cylinder's top
	/// to bottom.
	/// \param capSegments The number of annular (ring-shaped) subdivisions of
	/// the cylinder's endcap. Defaults to 2.
	virtual void setCylinderResolution( int radiusSegments, int heightSegments, int capSegments=2 );
	/// \brief Get this renderer's cylinder resolution as a 3D vector.
	///
	/// The resulting vector's x, y, and z properties correspond to the radius
	/// segments, height segments, and cap segments of this renderer's cylinder
	/// resolution respectively.
	///
	/// \returns A 3D vector representing this renderer's current cylinder
	/// resolution.
	virtual glm::vec3 getCylinderResolution() const;
	/// \brief Draw a cylinder with this renderer using x, y, radius, and
	/// height.
	///
	/// Cyliners are drawn with x, y, and z coordinates representing the
	/// centroid of the cylinder. Radius is the radius of the cylinder's
	/// circular footprint and height representing the height of the cylinder.
	///
	/// \param x The x coordinate to use when drawing this cylinder.
	/// \param y The y coordinate to use when drawing this cylinder.
	/// \param radius The radius to use when drawing this cylinder's circular
	/// footprint.
	/// \param height The height to use when drawing this cylinder.
	virtual void drawCylinder(float x, float y, float radius, float height) const;
	/// \brief Draw a cylinder with this renderer using x, y, z, radius, and
	/// height.
	///
	/// Cyliners are drawn with x, y, and z coordinates representing the
	/// centroid of the cylinder. Radius is the radius of the cylinder's
	/// circular footprint and height representing the height of the cylinder.
	///
	/// \param x The x coordinate to use when drawing this cylinder.
	/// \param y The y coordinate to use when drawing this cylinder.
	/// \param z The z coordinate to use when drawing this cylinder.
	/// \param radius The radius to use when drawing this cylinder's circular
	/// footprint.
	/// \param height The height to use when drawing this cylinder.
	virtual void drawCylinder(float x, float y, float z, float radius, float height) const;
	/// \brief Draw a cylinder with this renderer using position, radius, and
	/// height.
	///
	/// Cyliners are drawn with x, y, and z coordinates representing the
	/// centroid of the cylinder. Radius is the radius of the cylinder's
	/// circular footprint and height representing the height of the cylinder.
	///
	/// \param position The 3D point to use as a position coordinate when
	/// drawing this cylinder.
	///
	/// \param radius The radius to use when drawing this cylinder's circular
	/// footprint.
	/// \param height The height to use when drawing this cylinder.
	virtual void drawCylinder(const glm::vec3& position, float radius, float height) const;
	/// \brief Draw a cylinder at the origin using radius and height.
	///
	/// A cylinder drawn in this way will be positioned at the origin. Radius is
	/// the radius of the cylinder's circular footprint and height representing
	/// the height of the cylinder.
	///
	/// \param radius The radius to use when drawing this cylinder's circular
	/// footprint.
	/// \param height The height to use when drawing this cylinder.
	virtual void drawCylinder(float radius, float height) const;

	/// \brief Set the resolution of a polygonized cone.
	///
	/// Allows you to set the polygonization resolution of any cones you
	/// subsequently draw with ofDrawCone().
	///
	/// \param radiusSegments The number of facets (subdivisions) around the
	/// cone's circular footprint.
	/// \param heightSegments The number of subdivisions from the cone's top to
	/// bottom.
	/// \param capSegments The number of annular (ring-shaped) subdivisions of
	/// the cone's endcap.
	virtual void setConeResolution( int radiusSegments, int heightSegments, int capSegments=2);

	/// \brief Get this renderer's cone resolution as a 3D vector.
	///
	/// The resulting vector's x, y, and z properties correspond to the radius
	/// segments, height segments, and cap segments of this renderer's cone
	/// resolution respectively.
	///
	/// \returns A 3D vector representing this renderer's current cone
	/// resolution.
	virtual glm::vec3 getConeResolution() const;

	/// \brief Draw a cone with this renderer using x, y, z, radius, and
	/// height.
	///
	/// Cones are drawn with x, y, and z coordinates representing the centroid
	/// of the cone. Radius is the radius of the cone's circular
	/// footprint and height representing the height of the cone.
	///
	/// \param x The x coordinate to use when drawing this cone.
	/// \param y The y coordinate to use when drawing this cone.
	/// \param z The z coordinate to use when drawing this cone.
	/// \param radius The radius to use when drawing this cone's circular
	/// footprint.
	/// \param height The height to use when drawing this cone.
	virtual void drawCone(float x, float y, float z, float radius, float height) const;
	/// \brief Draw a cone with this renderer using x, y, z, radius, and
	/// height.
	///
	/// Cones are drawn with x, y, and z coordinates representing the centroid
	/// of the cylinder. Radius is the radius of the cone's circular
	/// footprint and height representing the height of the cone's.
	///
	/// \param x The x coordinate to use when drawing this cone.
	/// \param y The y coordinate to use when drawing this cone.
	/// \param radius The radius to use when drawing this cone's circular
	/// footprint.
	/// \param height The height to use when drawing this cone.
	virtual void drawCone(float x, float y, float radius, float height) const;
	/// \brief Draw a cone with this renderer using x, y, z, radius, and
	/// height.
	///
	/// Cones are drawn with x, y, and z coordinates representing the centroid
	/// of the cone. \p Radius is the radius of the cone's circular
	/// footprint and \p height representing the height of the cone's.
	///
	/// \param position The 3D point representing this cone's position.
	/// \param radius The radius to use when drawing this cone's circular
	/// footprint.
	/// \param height The height to use when drawing this cone.
	virtual void drawCone(const glm::vec3& position, float radius, float height) const;
	/// \brief Draw a cone at the origin using radius and height.
	///
	/// This cone will be drawn with its position at the coordinate system's
	/// origin. \p Radius is the radius of the cone's circular footprint and
	/// \p height representing the height of the cone.
	///
	/// \param radius The radius to use when drawing this cone's circular
	/// footprint.
	/// \param height The height to use when drawing this cone.
	virtual void drawCone(float radius, float height) const;

	// Box
	/// \brief Set the resolution this renderer uses when drawing boxes.
	/// \param res The resolution to use for box widths, heights, and depths.
	virtual void setBoxResolution( int res );
	/// \brief Set the resolution this renderer uses when drawing boxes.
	/// \param resWidth The width resolution this renderer uses when drawing
	/// boxes.
	/// \param resHeight The height resolution this renderer uses when drawing
	/// boxes.
	/// \param resDepth The depth resolution this renderer uses when drawing
	/// boxes.
	virtual void setBoxResolution( int resWidth, int resHeight, int resDepth );
	/// \brief Get this renderer's current box resolution as a 3D vector.
	///
	/// The returned vector's x, y, and z properties represent this renderer's
	/// current resolution width, resolution height, and resolution depth
	/// respectively.
	///
	/// \returns This renderer's current box resolution as a 3D vector.
	virtual glm::vec3 getBoxResolution() const;

	/// \brief Draws a rectangular box using x, y, z, width, height, and depth.
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
	virtual void drawBox( float x, float y, float z, float width, float height, float depth) const;

	/// \brief Draws a cube using x, y, z, and size.
	/// coordinates.
	///
	/// A cube is a rectangular solid bounded by six square faces of equal size.
	/// It is also known as a regular hexahedron, a square parallelepiped, an
	/// equilateral cuboid and a right rhombohedron. It is a regular square
	/// prism in three orientations.
	///
	/// It is drawn starting from a 3D reference coordinate, with the specified
	/// size. The cube is drawn with the current color, e.g. set with
	/// ofSetColor(). The cube is drawn filled by default; change this with
	/// ofFill().
	///
	/// \param x The x-coordinate of the cube's origin.
	/// \param y The y-coordinate of the cube's origin.
	/// \param z The z-coordinate of the cube's origin.
	/// \param size The size of the cube.
	virtual void drawBox(float x, float y, float z, float size) const;

	/// \brief Draws a rectangular box using position, width, height and depth.
	///
	/// A box is a rectangular solid: an extruded rectangle.
	/// It is drawn starting from a 3D reference coordinate.
	/// It has a width (in x), a height (in y), and a depth (in z).
	/// The box is drawn with the current color, e.g. set with ofSetColor().
	/// The box is drawn filled by default; change this with ofFill();
	///
	/// \param position an glm::vec3 which contains the (x,y,z) coordinates for the box's reference corner.
	/// \param width The width of the box.
	/// \param height The height of the box.
	/// \param depth The depth of the box.
	virtual void drawBox(const glm::vec3& position, float width, float height, float depth) const;

	/// \brief Draws a cube with the specified size, starting from the specified
	/// position.
	///
	/// A cube is drawn starting from a 3D reference position, with the
	/// specified size. The cube is drawn with the current color, e.g. set with
	/// ofSetColor(). The cube is drawn filled by default; change this with
	/// ofFill().
	///
	/// \param position an glm::vec3 which contains the (x,y,z) coordinates for
	/// the cube's reference corner.
	/// \param size The size of the cube.
	virtual void drawBox(const glm::vec3& position, float size) const;

	/// \brief Draws a cube with the specified size at the origin.
	///
	/// The cube is drawn with the current color, e.g. set with ofSetColor().
	/// The cube is drawn filled by default; change this with ofFill();
	///
	/// \param size The size of the cube.
	virtual void drawBox(float size) const;

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
	virtual void drawBox( float width, float height, float depth ) const;
	/// \brief Draw the coordinate system's axes with the renderer.
	///
	/// This draws a red, green, and blue lines for the x, y, and z axes
	/// respectively. This can be helpful when trying to orient other objects
	/// in respect to the coordinate system.
	///
	/// \param size The length to draw each axis line.
	virtual void drawAxis(float size) const;
	/// \brief Draw the coordinate system's axes as a grid with the renderer.
	/// \param stepSize The size of each row/column in each axis grid.
	/// \param numberOfSteps The number of rows/columns in each axis grid.
	/// \param labels True to draw the name and values of the axis as a bitmap
	/// string.
	/// \param x True to draw the x axis.
	/// \param y True to draw the y axis.
	/// \param z True to draw the z axis.
	virtual void drawGrid(float stepSize, size_t numberOfSteps, bool labels, bool x, bool y, bool z) const;
	/// \brief Draw a coordinate system plane using the y and z axes.
	/// \param stepSize The size of each row/column on the axis grid.
	/// \param numberOfSteps The number of rows/columns on the axis grid.
	/// \param labels True to draw the names and values of the axes.
	virtual void drawGridPlane(float stepSize, size_t numberOfSteps, bool labels) const;
	/// \brief Draw an arrow between two 3D points.
	/// \brief start The 3D vector to use as the first point.
	/// \brief end The 3D vector to use as the second point.
	/// \float headSize The size of the arrowhead.
	virtual void drawArrow(const glm::vec3& start, const glm::vec3& end, float headSize) const;
	/// \brief Draw the coordinate system's axes with the renderer.
	///
	/// This draws red, green, and blue lines for the x, y, and z rotation
	/// axes respectively. This can be helpful when trying to orient other
	/// object's rotation in respect to the coordinate system.
	///
	/// \param radius The radius to draw the rotation axes with.
	/// \param stripWidth The width of each axis line.
	/// \param circleRes The circle resolution to use when drawing the axes.
	virtual void drawRotationAxes(float radius, float stripWidth, int circleRes) const;
};
