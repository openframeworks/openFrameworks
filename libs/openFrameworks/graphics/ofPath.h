#pragma once

#include "ofConstants.h"
#include "ofColor.h"
#include "ofPolyline.h"
#include "ofVboMesh.h"
#include "ofTessellator.h"

/// \class

/// \brief ofPath is a way to create a path or multiple paths consisting of
/// points. It allows you to combine multiple paths consisting of points into
/// a single vector data object that can be drawn to the screen, manipulated
/// point by point, or manipulated with it's child subpaths. It is better at
/// representing and manipulating complex shapes than the ofPolyline and more
/// easily represents multiple child lines or shapes as either ofSubPath or
/// ofPolyline instances. By default ofPath uses ofSubPath instances. Closing
/// the path automatically creates a new path:
///
/// ~~~~{.cpp}
/// for( int i = 0; i < 5; i++) {
///     // create a new ofSubPath
///     path.arc( i * 50 + 20, i * 50 + 20, i * 40 + 10, i * 40 + 10, 0, 360);
///     path.close();
/// }
/// ~~~~
///
/// To use ofPolyline instances, simply set the mode to POLYLINES
///
/// ~~~~{.cpp}
/// path.setMode(POLYLINES);
/// ~~~~
class ofPath{
public:
	/// \name Create and remove paths and sub paths
	/// \{

	/// \brief Create a new ofPath instance.
	ofPath();

	/// \brief Remove all subpaths from the ofPath instance
	void clear();

	/// \brief Create a new subpath, either an ofPolyline instance or an ofSubPath
	/// instance. All points added after a call to ofSubPath will be done in
	/// the newly created subpath. Calling close() automatically calls create
	/// newSubPath(), ensuring that the closed path doesn't have new points
	/// added to it.
	void newSubPath();

	/// \brief Close the current subpath and create a new subpath, either an
	/// ofPolyline or ofSubPath by calling newSubPath(), ensuring that the
	/// closed path doesn't have new points added to it.
	void close();

	/// \}
	/// \name Sub paths
	/// \{

	/// \brief Draw a straight line from the current drawing position to the
	/// location indicated by p.
	void lineTo(const glm::vec3 & p);
	void lineTo(const glm::vec2 & p);

	/// \brief Draw a straight line from the current drawing position to the
	/// location indicated by x,y.
	void lineTo(float x, float y);

	/// \brief Draw a straight line from the current drawing position to the
	/// location indicated by x,y,z.
	void lineTo(float x, float y, float z);


	/// \brief Move the drawing position to p. This means that a subsequent calls to,
	/// for instance, lineTo() or curveTo() will connect the location p to the new
	/// location.
	void moveTo(const glm::vec3 & p);
	void moveTo(const glm::vec2 & p);

	/// \brief Move the drawing position to x,y.z. This means that a subsequent
	/// calls to, for instance, lineTo() or curveTo() will connect the
	/// location x,y,z to the new location.
	void moveTo(float x, float y, float z=0);

	/// \brief Draws a curve to p from the current drawing position
	void curveTo(const glm::vec3 & p);
	void curveTo(const glm::vec2 & p);

	/// \brief Draws a curve to x,y from the current drawing position
	void curveTo(float x, float y);

	/// \brief Draws a curve to x,y,z from the current drawing position
	void curveTo(float x, float y, float z);

	/// \brief Create a cubic bezier line from the current drawing point with the 2
	/// control points indicated by glm::vec3 `cp1` and `cp2`, that ends at glm::vec3
	/// to.
	///
	/// ~~~~{.cpp}
	/// line.addVertex(glm::vec3(200, 400));
	/// line.bezierTo(100, 100, 800, 100, 700, 400);
	/// ~~~~
	/// ![polyline bezier](graphics/bezier.jpg)
	/// The control points are shown in red.
	void bezierTo(const glm::vec3 & cp1, const glm::vec3 & cp2, const glm::vec3 & p);
	void bezierTo(const glm::vec2 & cp1, const glm::vec2 & cp2, const glm::vec2 & p);

	/// \brief Create a cubic bezier line from the current drawing point with the 2
	/// control points indicated by the coordinates cx1, cy1 and cx2, cy2,
	/// that ends at the coordinates x, y.
	void bezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y);

	/// \brief Create a cubic bezier line in 3D space from the current drawing point
	/// with the 2 control points indicated by the coordinates cx1, cy1, cz1
	/// and cx2, cy2, cz2, that ends at the coordinates x, y, z.
	void bezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z);

	/// \brief Create a quadratic bezier line in 3D space from the current drawing
	/// point with the beginning indicated by the coordinates cx1, cy1, cz1,
	/// the control point at cx2, cy2, cz2, and that ends at the coordinates
	/// x, y, z.
	/// ![Curves](graphics/curves.jpg)
	void quadBezierTo(const glm::vec3 & cp1, const glm::vec3 & cp2, const glm::vec3 & p);
	void quadBezierTo(const glm::vec2 & cp1, const glm::vec2 & cp2, const glm::vec2 & p);

	/// \brief Creates a quadratic bezier line in 2D space from the current drawing
	/// point with the beginning indicated by the point p1, the control point
	/// at p2, and that ends at the point p3.
	void quadBezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y);

	/// \brief Creates a quadratic bezier line in 3D space from the current drawing
	/// point with the beginning indicated by the coordinates cx1, cy1, the
	/// control point at cx2, cy2, and that ends at the coordinates x, y.
	void quadBezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z);

	/// \brief Create an arc at centre, which has the radiusX, radiusY, and begins at
	/// angleBegin and ends at angleEnd. To draw a circle with a radius of 50 pixels
	/// at 100, 100:
	///
	/// \note angleBegin needs to be larger than angleEnd, i.e. 0,180 is ok,
	/// while 180,0 is not.
	void arc(const glm::vec3 & centre, float radiusX, float radiusY, float angleBegin, float angleEnd);
	void arc(const glm::vec3 & centre, float radiusX, float radiusY, float angleBegin, float angleEnd, bool clockwise);
	void arc(const glm::vec2 & centre, float radiusX, float radiusY, float angleBegin, float angleEnd);
	void arc(const glm::vec2 & centre, float radiusX, float radiusY, float angleBegin, float angleEnd, bool clockwise);

	/// \brief Create an arc at x,y, which has the radiusX, radiusY, and begins at
	/// angleBegin and ends at angleEnd. To draws a shape with a radius of 200 pixels
	/// at 300, 300:
	///
	/// ~~~~{.cpp}
	/// path.moveTo(300, 300);
	/// path.arc( 300, 300, 200, 200, 0, 271); // note 271, not 270 for precision
	/// ~~~~
	///
	/// ![ofPath arc](graphics/ofPath_arc.jpg)
	///
	/// \note angleBegin needs to be larger than angleEnd, i.e. 0, 180 is ok,
	/// while 180,0 is not.
	void arc(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd);

	/// \brief Create an arc at x,y,z, which has the radiusX, radiusY, and begins at
	/// angleBegin and ends at angleEnd.
	void arc(float x, float y, float z, float radiusX, float radiusY, float angleBegin, float angleEnd);

	void arcNegative(const glm::vec3 & centre, float radiusX, float radiusY, float angleBegin, float angleEnd);
	void arcNegative(const glm::vec2 & centre, float radiusX, float radiusY, float angleBegin, float angleEnd);
	void arcNegative(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd);
	void arcNegative(float x, float y, float z, float radiusX, float radiusY, float angleBegin, float angleEnd);

	void triangle(float x1,float y1,float x2,float y2,float x3, float y3);
	void triangle(float x1,float y1,float z1,float x2,float y2,float z2,float x3, float y3,float z3);
	void triangle(const glm::vec3 & p1, const glm::vec3 & p2, const glm::vec3 & p3);
	void triangle(const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3);

	void circle(float x, float y, float radius);
	void circle(float x, float y, float z, float radius);
	void circle(const glm::vec3 & p, float radius);
	void circle(const glm::vec2 & p, float radius);

	void ellipse(float x, float y, float width, float height);
	void ellipse(float x, float y, float z, float width, float height);
	void ellipse(const glm::vec3 & p, float width, float height);
	void ellipse(const glm::vec2 & p, float width, float height);

	void rectangle(const ofRectangle & r);
	void rectangle(const glm::vec3 & p,float w,float h);
	void rectangle(const glm::vec2 & p,float w,float h);
	void rectangle(float x,float y,float w,float h);
	void rectangle(float x,float y,float z,float w,float h);

	void rectRounded(const ofRectangle & b, float r);
	void rectRounded(const glm::vec3 & p, float w, float h, float r);
	void rectRounded(const glm::vec2 & p, float w, float h, float r);
	void rectRounded(float x, float y, float w, float h, float r);
	void rectRounded(const glm::vec3 & p, float w, float h, float topLeftRadius,
	                                                        float topRightRadius,
	                                                        float bottomRightRadius,
	                                                        float bottomLeftRadius);
	void rectRounded(const glm::vec2 & p, float w, float h, float topLeftRadius,
															float topRightRadius,
															float bottomRightRadius,
															float bottomLeftRadius);
	void rectRounded(const ofRectangle & b, float topLeftRadius,
	                                          float topRightRadius,
	                                          float bottomRightRadius,
	                                          float bottomLeftRadius);
	void rectRounded(float x, float y, float z, float w, float h, float topLeftRadius,
                                                      float topRightRadius,
                                                      float bottomRightRadius,
                                                      float bottomLeftRadius);
	/// \}
	/// \name Winding Mode
	/// \{

	/// \brief Set the way that the points in the sub paths are connected.
	///
	/// OpenGL can only render convex polygons which means that any shape that
	/// isn't convex, i.e. that has points which are concave, going inwards,
	/// need to be tessellated into triangles so that OpenGL can render them.
	/// If you're using filled shapes with your ofPath this is done
	/// automatically for you.
	///
	/// The possible options you can pass in are:
	///
	///     OF_POLY_WINDING_ODD
	///     OF_POLY_WINDING_NONZERO
	///     OF_POLY_WINDING_POSITIVE
	///     OF_POLY_WINDING_NEGATIVE
	///     OF_POLY_WINDING_ABS_GEQ_TWO
	void setPolyWindingMode(ofPolyWindingMode mode);

	/// \brief Get the poly winding mode currently in use.
	ofPolyWindingMode getWindingMode() const;

	/// \}
	/// \name Drawing Mode
	/// \{

	/// \brief Set whether the path should be drawn as wireframes or filled.
	void setFilled(bool hasFill); // default true

	/// \brief Set the stroke width of the line if the ofPath is to be drawn
	/// not in wireframe.
	void setStrokeWidth(float width); // default 0

	/// \brief Set the color of the path. This affects both the line if the
	/// path is drawn as wireframe and the fill if the path is drawn with
	/// fill. All subpaths are affected.
	void setColor( const ofColor& color );

	/// \brief Set the color of the path. This affects both the line if the path is
	/// drawn as wireframe and the fill if the path is drawn with fill. All
	/// subpaths are affected.
	void setHexColor( int hex );

	/// \brief Set the fill color of the path. This has no affect if the path is
	/// drawn as wireframe.
	void setFillColor(const ofColor & color);

	/// \brief Set the fill color of the path. This has no affect if the path is
	/// drawn as wireframe.
	void setFillHexColor( int hex );

	/// \brief Set the stroke color of the path. This has no affect if the path
	/// is drawn filled.
	void setStrokeColor(const ofColor & color);

	/// \brief Set the stroke color of the path. This has no affect if the path
	/// is drawn filled.
	void setStrokeHexColor( int hex );

	/// \brief Get whether the path is using a fill or not.
	///
	/// The default value is `true`
	bool isFilled() const;

	/// \brief Get the ofColor fill of the ofPath
	ofColor getFillColor() const;

	/// \brief Get the stroke color of the ofPath
	ofColor getStrokeColor() const;

	/// \brief Get the stroke width of the ofPath
	///
	/// The default value is `0
	float getStrokeWidth() const;

	bool hasOutline() const { return strokeWidth>0; }

	void setCurveResolution(int curveResolution);
	int getCurveResolution() const;

	void setCircleResolution(int circleResolution);
	int getCircleResolution() const;

	OF_DEPRECATED_MSG("Use setCircleResolution instead.", void setArcResolution(int res));
	OF_DEPRECATED_MSG("Use getCircleResolution instead.", int getArcResolution() const);

	void setUseShapeColor(bool useColor);
	bool getUseShapeColor() const;

	/// \}
	/// \name Drawing
	/// \{

	/// \brief Draws the path at 0,0. Calling draw() also calls tessellate()
	void draw() const;

	/// \brief Draws the path at x,y. Calling draw() also calls tessellate()
	void draw(float x, float y) const;

	/// \}
	/// \name Functions
	/// \{

	/// \brief Get an ofPolyline representing the outline of the ofPath.
	const std::vector<ofPolyline> & getOutline() const;

	void tessellate();

	const ofMesh & getTessellation() const;

	void simplify(float tolerance=0.3f);

	void translate(const glm::vec3 & p);

	void rotateDeg(float degrees, const glm::vec3& axis);
	void rotateRad(float radians, const glm::vec3& axis);
	OF_DEPRECATED_MSG("Use Deg/Rad versions.", void rotate(float degrees, const glm::vec3& axis ));

	void translate(const glm::vec2 & p);

	void rotateDeg(float degrees, const glm::vec2& axis);
	void rotateRad(float radians, const glm::vec2& axis);
	OF_DEPRECATED_MSG("Use Deg/Rad versions.", void rotate(float degrees, const glm::vec2& axis ));

	/// \brief Change the size of either the ofPolyline or ofSubPath instances that
	/// the ofPath contains. These changes are non-reversible, so for instance
	/// scaling by 0,0 zeros out all data.
	void scale(float x, float y);

	void append(const ofPath & path);

	/// \}
	/// \name Path Mode
	/// \{

	enum Mode{
		COMMANDS,
		POLYLINES
	};

	void setMode(Mode mode);
	Mode getMode() const;

	/// \}
	/// \name Path Commands
	/// \{

	struct Command{
		enum Type{
			moveTo,
			lineTo,
			curveTo,
			bezierTo,
			quadBezierTo,
			arc,
			arcNegative,
			close
		};

		/// for close
		Command(Type type);

		/// for lineTo and curveTo
		Command(Type type , const glm::vec3 & p);

		/// for bezierTo
		Command(Type type , const glm::vec3 & p, const glm::vec3 & cp1, const glm::vec3 & cp2);

		///for arc
		Command(Type type , const glm::vec3 & centre, float radiusX, float radiusY, float angleBegin, float angleEnd);


		Type type;
		glm::vec3 to;
		glm::vec3 cp1, cp2;
		float radiusX, radiusY, angleBegin, angleEnd;
	};

	std::vector<Command> & getCommands();
	const std::vector<Command> & getCommands() const;

	/// \}

private:

	ofPolyline & lastPolyline();
	void addCommand(const Command & command);
	void generatePolylinesFromCommands();

	// only needs to be called when path is modified externally
	void flagShapeChanged();
	bool hasChanged();

	// path description
	//vector<ofSubPath>		paths;
	std::vector<Command> 	commands;
	ofPolyWindingMode 	windingMode;
	ofColor 			fillColor;
	ofColor				strokeColor;
	float				strokeWidth;
	bool				bFill;
	bool				bUseShapeColor;

	// polyline / tessellation
	std::vector<ofPolyline>  polylines;
	std::vector<ofPolyline>  tessellatedContour; // if winding mode != ODD

#ifdef TARGET_OPENGLES
	ofMesh				cachedTessellation;
#else
	ofVboMesh			cachedTessellation;
#endif
#if defined(TARGET_EMSCRIPTEN)
	static ofTessellator tessellator;
#elif HAS_TLS
	static thread_local ofTessellator tessellator;
#else
	ofTessellator tessellator;
#endif
	bool				bHasChanged;
	int					prevCurveRes;
	int					curveResolution;
	int					circleResolution;
	bool 				bNeedsTessellation;
	bool				bNeedsPolylinesGeneration;

	Mode				mode;
};
