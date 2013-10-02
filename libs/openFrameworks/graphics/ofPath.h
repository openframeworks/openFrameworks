#pragma once

#include "ofConstants.h"
#include "ofPoint.h"
#include "ofColor.h"
#include "ofPolyline.h"
#include "ofBaseTypes.h"
#include "ofVboMesh.h"
#include "ofTessellator.h"


class ofPath{
public:

	ofPath();

	void clear();

	// creates a new subpath
	void newSubPath();
	// closes current path, next command starts a new one
	void close();

	void lineTo(const ofPoint & p);
	void lineTo(float x, float y);
	void lineTo(float x, float y, float z);

	// starts a new subpath with current parameters for winding, stroke, fill...
	// the new subpath starts in p
	void moveTo(const ofPoint & p);
	void moveTo(float x, float y, float z=0);

	void curveTo(const ofPoint & p);
	void curveTo(float x, float y);
	void curveTo(float x, float y, float z);

	void bezierTo(const ofPoint & cp1, const ofPoint & cp2, const ofPoint & p);
	void bezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y);
	void bezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z);

	void quadBezierTo(const ofPoint & cp1, const ofPoint & cp2, const ofPoint & p);
	void quadBezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y);
	void quadBezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z);

    void arc(const ofPoint & centre, float radiusX, float radiusY, float angleBegin, float angleEnd, bool clockwise);

	void arc(const ofPoint & centre, float radiusX, float radiusY, float angleBegin, float angleEnd);
	void arc(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd);
	void arc(float x, float y, float z, float radiusX, float radiusY, float angleBegin, float angleEnd);

	void arcNegative(const ofPoint & centre, float radiusX, float radiusY, float angleBegin, float angleEnd);
	void arcNegative(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd);
	void arcNegative(float x, float y, float z, float radiusX, float radiusY, float angleBegin, float angleEnd);

	void triangle(float x1,float y1,float x2,float y2,float x3, float y3);
	void triangle(float x1,float y1,float z1,float x2,float y2,float z2,float x3, float y3,float z3);
	void triangle(const ofPoint & p1, const ofPoint & p2, const ofPoint & p3);

	void circle(float x, float y, float radius);
	void circle(float x, float y, float z, float radius);
	void circle(const ofPoint & p, float radius);

	void ellipse(float x, float y, float width, float height);
	void ellipse(float x, float y, float z, float width, float height);
	void ellipse(const ofPoint & p, float width, float height);

	void rectangle(const ofRectangle & r);
	void rectangle(const ofPoint & p,float w,float h);
	void rectangle(float x,float y,float w,float h);
	void rectangle(float x,float y,float z,float w,float h);

	void rectRounded(const ofRectangle & b, float r);
	void rectRounded(const ofPoint & p, float w, float h, float r);
	void rectRounded(float x, float y, float w, float h, float r);
	void rectRounded(const ofPoint & p, float w, float h, float topLeftRadius,
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

	void setPolyWindingMode(ofPolyWindingMode mode);
	void setFilled(bool hasFill); // default true
	void setStrokeWidth(float width); // default 0
	void setColor( const ofColor& color );
	void setHexColor( int hex );
	void setFillColor(const ofColor & color);
	void setFillHexColor( int hex );
	void setStrokeColor(const ofColor & color);
	void setStrokeHexColor( int hex );


	ofPolyWindingMode getWindingMode() const;
	bool isFilled() const; // default true
	ofColor getFillColor() const;
	ofColor getStrokeColor() const;
	float getStrokeWidth() const; // default 0
	bool hasOutline() const { return strokeWidth>0; }

	void draw(float x, float y);
	void draw();

	vector<ofPolyline> & getOutline();
	ofMesh & getTessellation();

	void simplify(float tolerance=0.3);

	// only needs to be called when path is modified externally
	void flagShapeChanged();
	bool hasChanged();

	enum Mode{
		COMMANDS,
		POLYLINES
	};

	void setMode(Mode mode);
	Mode getMode();

	void setCurveResolution(int curveResolution);
	int getCurveResolution() const;

    void setCircleResolution(int circleResolution);
    int getCircleResolution() const;
    
	OF_DEPRECATED_MSG("Use setCircleResolution instead.", void setArcResolution(int res));
    OF_DEPRECATED_MSG("Use getCircleResolution instead.", int getArcResolution() const);

	void setUseShapeColor(bool useColor);
	bool getUseShapeColor() const;
	
	void tessellate();

	void translate(const ofPoint & p);
	void rotate(float az, const ofVec3f& axis );
	void scale(float x, float y);

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
		Command(Type type , const ofPoint & p);

		/// for bezierTo
		Command(Type type , const ofPoint & p, const ofPoint & cp1, const ofPoint & cp2);

		///for arc
		Command(Type type , const ofPoint & centre, float radiusX, float radiusY, float angleBegin, float angleEnd);


		Type type;
		ofPoint to;
		ofPoint cp1, cp2;
		float radiusX, radiusY, angleBegin, angleEnd;
	};

	vector<Command> & getCommands();
	const vector<Command> & getCommands() const;

private:

	ofPolyline & lastPolyline();
	void addCommand(const Command & command);
	void generatePolylinesFromCommands();

	// path description
	//vector<ofSubPath>		paths;
	vector<Command> 	commands;
	ofPolyWindingMode 	windingMode;
	ofColor 			fillColor;
	ofColor				strokeColor;
	float				strokeWidth;
	bool				bFill;
	bool				bUseShapeColor;

	// polyline / tessellation
	vector<ofPolyline>  polylines;
	vector<ofPolyline>  tessellatedContour; // if winding mode != ODD

#ifdef TARGET_OPENGLES
	ofMesh				cachedTessellation;
#else
	ofVboMesh			cachedTessellation;
#endif
	bool				cachedTessellationValid;

	static ofTessellator tessellator;

	bool				bHasChanged;
	int					prevCurveRes;
	int					curveResolution;
	int					circleResolution;
	bool 				bNeedsTessellation;
	bool				bNeedsPolylinesGeneration;

	Mode				mode;
};


