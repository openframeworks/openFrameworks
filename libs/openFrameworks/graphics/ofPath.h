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
	void lineTo(float x, float y, float z=0);

	// starts a new subpath with defaults for winding, stroke, fill...
	// the new subpath starts in p
	void moveTo(const ofPoint & p);
	void moveTo(float x, float y, float z=0);

	void curveTo(const ofPoint & p);
	void curveTo(float x, float y, float z=0);

	void bezierTo(const ofPoint & cp1, const ofPoint & cp2, const ofPoint & p);
	void bezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y);
	void bezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z);

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

	ofShape getShape(int curveResolution=16, bool tesselated=false);

	struct Command;
	const vector<Command> & getCommands() const;
	const vector<ofPath> & getSubPaths() const;

	struct Command{
		enum Type{
			lineTo,
			curveTo,
			bezier3DTo,
			bezier2DTo,
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
		{
			controlPoints.reserve(2);
			controlPoints.push_back(cp1);
			controlPoints.push_back(cp2);
		}

		///for arc
		Command(Type type , const ofPoint & centre, float radiusX, float radiusY, float angleBegin, float angleEnd)
		:type(type)
		,to(centre)
		{
			additionalParams.reserve(4);
			additionalParams.push_back(radiusX);
			additionalParams.push_back(radiusY);
			additionalParams.push_back(angleBegin);
			additionalParams.push_back(angleEnd);
		}
		const float & radiusX() const { return additionalParams[0]; }
		const float & radiusY()const { return additionalParams[1]; }
		const float & angleBegin()const { return additionalParams[2]; }
		const float & angleEnd()const { return additionalParams[3]; }

		const ofPoint & cp1()const { return controlPoints[0]; }
		const ofPoint & cp2()const { return controlPoints[1]; }

		float & radiusX() { return additionalParams[0]; }
		float & radiusY() { return additionalParams[1]; }
		float & angleBegin() { return additionalParams[2]; }
		float & angleEnd() { return additionalParams[3]; }

		ofPoint & cp1() { return controlPoints[0]; }
		ofPoint & cp2() { return controlPoints[1]; }

		Type type;
		ofPoint to;

		/// for arc radius/angles
		vector<float> additionalParams;

		/// for bezier control points
		vector<ofPoint> controlPoints;
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
};
