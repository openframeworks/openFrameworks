#pragma once

#include "ofConstants.h"
#include "ofPoint.h"
#include "ofColor.h"
#include "ofPolyline.h"
#include "ofBaseTypes.h"
#include "ofMesh.h"
#include "ofTessellator.h"

class ofSubPath;

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

	vector<ofSubPath> & getSubPaths();
	const vector<ofSubPath> & getSubPaths() const;

	vector<ofPolyline> & getOutline();
	ofMesh & getTessellation();

	void simplify(float tolerance=0.3);

	// only needs to be called when path is modified externally
	void flagShapeChanged();

	enum Mode{
		PATHS,
		POLYLINES
	};

	void setMode(Mode mode);

	void setCurveResolution(int curveResolution);
	int getCurveResolution();

	void setArcResolution(int res);
	int getArcResolution();

	void setUseShapeColor(bool useColor);
	bool getUseShapeColor();
	
	void tessellate();

	void translate(const ofPoint & p);
	void rotate(float az, const ofVec3f& axis );
	void scale(float x, float y);

private:

	ofSubPath & lastPath();
	ofPolyline & lastPolyline();

	void generatePolylinesFromPaths();

	// path description
	vector<ofSubPath>		paths;
	ofPolyWindingMode 	windingMode;
	ofColor 			fillColor;
	ofColor				strokeColor;
	float				strokeWidth;
	bool				bFill;
	bool				bUseShapeColor;

	// polyline / tessellation
	vector<ofPolyline>  polylines;
	vector<ofPolyline>  tessellatedContour; // if winding mode != ODD

	ofMesh				cachedTessellation;
	bool				cachedTessellationValid;

	static ofTessellator tessellator;

	bool				hasChanged;
	int					prevCurveRes;
	int					curveResolution;
	int					arcResolution;
	bool 				bNeedsTessellation;

	Mode				mode;
};


class ofSubPath{
public:
	struct Command;

	ofSubPath();
	vector<Command> & getCommands();
	const vector<Command> & getCommands() const;
	void addCommand(const Command & command);
	bool hasChanged();
	void close();
	bool isClosed();
	int size();

	struct Command{
		enum Type{
			lineTo,
			curveTo,
			bezierTo,
			quadBezierTo,
			arc,
            arcNegative,
		};

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

private:
	vector<Command> commands;
	bool			bClosed;
	bool			bHasChanged;
};

