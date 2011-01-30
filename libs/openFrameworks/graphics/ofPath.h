#pragma once

#include "ofConstants.h"
#include "ofPoint.h"
#include "ofColor.h"
#include "ofShape.h"

class ofPath{
public:

	ofPath();

	void clear();

	ofPath & newSubPath();

	void lineTo(const ofPoint & p);
	void lineTo(float x, float y);
	void lineTo(float x, float y, float z);

	// starts a new subpath with defaults for winding, stroke, fill...
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

	void arc(const ofPoint & centre, float radiusX, float radiusY, float angleBegin, float angleEnd);
	void arc(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd);
	void arc(float x, float y, float z, float radiusX, float radiusY, float angleBegin, float angleEnd);

	// closes current path, next command starts a new one with defaults for winding, stroke, fill...
	void close();

	void setPolyWindingMode(ofPolyWindingMode mode);
	void setColor( const ofColor& color ) { setFillColor( color ); setStrokeColor( color ); }
	void setHexColor( int hex ) { setColor( ofColor().fromHex( hex ) ); };
	void setFilled(bool hasFill); // default false
	void setFillColor(const ofColor & color);
	void setFillHexColor( int hex ) { setFillColor( ofColor().fromHex( hex ) ); };
	void setStrokeColor(const ofColor & color);
	void setStrokeHexColor( int hex ) { setStrokeColor( ofColor().fromHex( hex ) ); };
	void setStrokeWidth(float width); // default 1

	ofPolyWindingMode getWindingMode() const;
	bool isFilled() const; // default false
	ofColor getFillColor() const;
	ofColor getStrokeColor() const;
	float getStrokeWidth() const; // default 1
	bool isClosed() const;

	void updateShape();
	void draw(float x=0, float y=0);

	ofShape & getShape(int curveResolution=-1);



	struct Command;

	// only needs to be called when path is modified externally
	void markedChanged();
	vector<Command> & getCommands();
	vector<ofPath> & getSubPaths();

	const vector<Command> & getCommands() const;
	const vector<ofPath> & getSubPaths() const;

	void addCommand(const Command & command);

	struct Command{
		enum Type{
			line2DTo,
			line3DTo,
			curve2DTo,
			curve3DTo,
			bezier3DTo,
			bezier2DTo,
			quadBezier3DTo,
			quadBezier2DTo,
			arc2D,
			arc3D,
		};

		/// for lineTo and curveTo
		Command(Type type , const ofPoint & p)
		:type(type)
		,to(p)
		{}

		/// for bezierTo
		Command(Type type , const ofPoint & p, const ofPoint & cp1, const ofPoint & cp2)
		:type(type)
		,to(p)
		,cp1(cp1)
		,cp2(cp2)
		{
		}

		///for arc
		Command(Type type , const ofPoint & centre, float radiusX, float radiusY, float angleBegin, float angleEnd)
		:type(type)
		,to(centre)
		,radiusX(radiusX)
		,radiusY(radiusY)
		,angleBegin(angleBegin)
		,angleEnd(angleEnd)
		{
		}

		Type type;
		ofPoint to;
		ofPoint cp1, cp2;
		float radiusX, radiusY, angleBegin, angleEnd;
	};


private:

	ofPath & lastPath();

	vector<ofPath>		subPaths;
	vector<Command> 	commands;
	ofPolyWindingMode 	windingMode;
	ofColor 			fillColor;
	ofColor				strokeColor;
	float				strokeWidth;
	bool				bFill;
	bool				bClosed;
	ofShape				cachedShape;
	bool				hasChanged;
	ofBaseRenderer * 	renderer;
	int					prevCurveRes;
};
