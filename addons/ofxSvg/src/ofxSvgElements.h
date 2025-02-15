//
//  ofxSvgElements.h
//
//  Created by Nick Hardeman on 7/31/15.
//

#pragma once
#include "ofNode.h"
#include "ofImage.h"
#include "ofPath.h"
#include <map>
#include "ofTrueTypeFont.h"

namespace ofx::svg {
enum SvgType {
	TYPE_ELEMENT = 0,
	TYPE_GROUP,
	TYPE_RECTANGLE,
	TYPE_IMAGE,
	TYPE_ELLIPSE,
	TYPE_CIRCLE,
	TYPE_PATH,
	TYPE_TEXT,
	TYPE_DOCUMENT,
	TYPE_TOTAL
};

class Element {
public:
	
	static std::string sGetTypeAsString(SvgType atype);
	static std::string sGetSvgXmlName(SvgType atype);
	
	virtual SvgType getType() {return TYPE_ELEMENT;}
	std::string getTypeAsString();
	
	std::string getName() { return name; }
	bool isGroup() {
		return (getType() == TYPE_GROUP);
	}
	
	void setVisible( bool ab ) { bVisible = ab; }
	bool isVisible() { return bVisible; }
	
	virtual std::string toString(int nlevel = 0);
	
	std::string name = "";
	float layer = -1.f;
	bool bVisible=true;
	bool bUseShapeColor = true;
	
	
	// will log messages to this module name
	static const std::string moduleName() { return "ofx::svg::Element"; }
		
	glm::vec2 pos = glm::vec2(0.f, 0.f);
	glm::vec2 scale = glm::vec2(1.0f, 1.0f);
	float rotation = 0.0f;
	
	virtual glm::mat4 getTransformMatrix();
	virtual ofNode getNodeTransform();
	
	virtual void draw() {}
	
	virtual void setUseShapeColor( bool ab ) {
		bUseShapeColor = ab;
	}
	
	virtual ofPolyline getFirstPolyline() {
		ofLogWarning(moduleName()) << __FUNCTION__ << " : Element " << getTypeAsString() << " does not have a path.";
		return ofPolyline();
	}
//protected:
	// used for saving to set the model position of the current mat4 //
	glm::vec2 mModelPos = glm::vec2(0.f, 0.f);
	glm::vec2 mModelRotationPoint = glm::vec2(0.f, 0.f);
	
};

class Path : public Element {
public:
	virtual SvgType getType() override {return TYPE_PATH;}
	
	virtual void setUseShapeColor( bool ab ) override {
		Element::setUseShapeColor(ab);
		path.setUseShapeColor(ab);
	}
	
	virtual void draw() override {
		if(isVisible()) path.draw();
	}
	
	bool isFilled() { return path.isFilled(); }
	bool hasStroke() { return path.hasOutline(); }
	float getStrokeWidth() { return path.getStrokeWidth(); }
	ofColor getFillColor() { return path.getFillColor(); }
	ofColor getStrokeColor() { return path.getStrokeColor(); }
	
	ofPolyline getFirstPolyline() override {
		if( path.getOutline().size() > 0 ) {
			return path.getOutline()[0];
		}
		ofLogWarning(moduleName()) << __FUNCTION__ << " : path does not have an outline.";
		return ofPolyline();
	}
	
	ofPath path;
};

class Rectangle : public Path {
public:
	virtual SvgType getType() override {return TYPE_RECTANGLE;}
	ofRectangle rectangle;
	
	float getWidth() { return rectangle.getWidth() * scale.x;}
	float getHeight() { return rectangle.getHeight() * scale.y;}
	
	float round = 0.f;
	
//	float getWidthScaled() { return rectangle.getWidth() * scale.x;}
//	float getHeightScaled() { return rectangle.getHeight() * scale.y;}
};

class Image : public Element {
public:
	virtual SvgType getType() override {return TYPE_IMAGE;}
	
	float getWidth() { return width * scale.x;}
	float getHeight() { return height * scale.y;}
	
	ofRectangle getRectangle();
//	float getWidthScaled() { return getWidth() * scale.x;}
//	float getHeightScaled() { return getHeight() * scale.y;}
	
	virtual void draw() override;
	glm::vec2 getAnchorPointForPercent( float ax, float ay );
	
	std::string getFilePath() { return filepath; }
	
	void setColor( ofColor aColor ) {
		color = aColor;
	}
	ofColor getColor() {
		return color;
	}
	
	ofColor color;
	ofImage img;
	bool bTryLoad = false;
	of::filesystem::path filepath;
	float width = 0.f;
	float height = 0.f;
};

class Circle : public Path {
public:
	virtual SvgType getType() override {return TYPE_CIRCLE;}
	float getRadius() {return radius;}
	float radius = 10.0;
};

class Ellipse : public Path {
public:
	virtual SvgType getType() override {return TYPE_ELLIPSE;}
	float radiusX, radiusY = 10.0f;
};


class Text : public Rectangle {
public:	
	class Font {
	public:
		std::string fontFamily;
		std::map< int, ofTrueTypeFont > sizes;
		std::map< int, ofTexture > textures;
	};
	
	static std::map< std::string, Font > fonts;
	
	class TextSpan {
	public:
		TextSpan() {
			text = "";
			fontSize = 12;
			lineHeight = 0;
		}
		
		std::string text;
		int fontSize = 12;
		std::string fontFamily;
		ofRectangle rect;
		ofColor color;
		float lineHeight = 12;
		ofTrueTypeFont& getFont();
		void draw( const std::string& astring, bool abCentered );
		void draw(const std::string &astring, const ofColor& acolor, bool abCentered );
	};
	
	static bool sortSpanOnFontFamily( const TextSpan& a, const TextSpan& b ) {
		return a.fontFamily < b.fontFamily;
	}
	
	static bool sortSpanOnFontSize( const TextSpan& a, const TextSpan& b ) {
		return a.fontSize < b.fontSize;
	}
	
//	Text() { type = OFX_SVG_TYPE_TEXT; fdirectory=""; bCentered=false; alpha=1.0; bOverrideColor=false; }
	virtual SvgType getType() override {return TYPE_TEXT;}
	
	ofTrueTypeFont& getFont();
	ofColor getColor();
	
	void create();
	void draw() override;
	void draw(const std::string &astring, bool abCentered );
	void draw(const std::string &astring, const ofColor& acolor, bool abCentered );
	
	void setFontDirectory( std::string aPath ) {
		fdirectory = aPath;
		//		cout << "Setting the font directory to " << fdirectory << endl;
	}
	
	void overrideColor( ofColor aColor ) {
		bOverrideColor = true;
		_overrideColor = aColor;
	}
	
	ofRectangle getRectangle();
	
	std::map< std::string, std::map<int, ofMesh> > meshes;
	std::vector< std::shared_ptr<TextSpan> > textSpans;
	
	std::string fdirectory;
	bool bCentered = false;
	float alpha = 1.;
	glm::vec2 ogPos = glm::vec2(0.f, 0.f);
	
protected:
	static ofTrueTypeFont defaultFont;
	bool _recursiveFontDirSearch(const std::string& afile, const std::string& aFontFamToLookFor, std::string& fontpath);
	ofFloatColor _overrideColor;
	bool bOverrideColor = false;
};

}













