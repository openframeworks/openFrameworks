#pragma once
#include <unordered_map>
#include "ofColor.h"
#include "ofLog.h"
#include "ofXml.h"
#include "ofxSvgUtils.h"

class ofxSvgCssClass {
public:
	
	/// \note: ofxSvgOptional is declared in ofxSvgUtils
	class Property {
	public:
		std::string srcString;
		ofxSvgOptional<float> fvalue;
		ofxSvgOptional<int> ivalue;
		ofxSvgOptional<std::string> svalue;
		ofxSvgOptional<ofColor> cvalue;
	};
	
	std::unordered_map<std::string, Property> properties;
	std::string name = "default";
	
	void clear();
	void scaleNumericalValues( float ascale );
	
	static std::string sRgbaToHexString(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	static std::string sRgbToHexString(unsigned char r, unsigned char g, unsigned char b);
	static bool sIsNone( const std::string& astr );
	static ofColor sGetColor(const std::string& astr);
	static float sGetFloat(const std::string& astr);
	
	bool addMissingClassProperties( const ofxSvgCssClass& aclass );
	bool setClassProperties( const ofxSvgCssClass& aclass );
	
	bool addProperties( std::string aPropertiesString );
	bool addProperty( std::string aPropString );
	bool addProperty( std::string aName, std::string avalue );
	bool addProperty( const std::string& aName, const Property& aprop );
	bool addProperty( const std::string& aName, const float& avalue );
	bool addProperty( const std::string& aName, const ofColor& acolor );
	
	bool removeProperty( std::string aPropString );
	
	bool setColor(const ofColor& acolor);
	
	bool setFillColor(const ofColor& acolor);
	bool setNoFill();
	bool isFilled();
	
	bool setStrokeColor(const ofColor& acolor);
	bool setStrokeWidth( const float& awidth );
	bool setNoStroke();
	bool hasStroke();
	
	bool setFontSize( int asize );
	int getFontSize(int adefault);
	
	bool setFontFamily( std::string aFontFamily );
	std::string getFontFamily( std::string aDefaultFontFamily );
	
	bool setFontBold( bool ab );
	bool isFontBold();
	
	bool setFontItalic( bool ab );
	bool isFontItalic();
	
	bool hasProperty( const std::string& akey );
	Property& getProperty( const std::string& akey );
	bool isNone(const std::string& akey);
	bool hasAndIsNone(const std::string& akey);
	
	std::string getValue(const std::string& akey, const std::string& adefault);
	int getIntValue(const std::string& akey, const int& adefault);
	float getFloatValue(const std::string& akey, const float& adefault);
	ofColor getColor(const std::string& akey);
	ofColor getColor(const std::string& akey, const ofColor& adefault);
	
	std::string toString(bool aBPrettyPrint=true);
	
protected:
	Property dummyProp;
};

class ofxSvgCssStyleSheet {
public:
	
	bool parse( std::string aCssString );
	void clear();
	
	void scaleNumericalValues( float ascale );
	
	ofxSvgCssClass& addClass( std::string aname );
	bool hasClass( const std::string& aname );
	ofxSvgCssClass& getClass( const std::string& aname );
	
	ofxSvgCssClass& getAddClass( ofxSvgCssClass& aclass );
	
	std::unordered_map<std::string, ofxSvgCssClass> classes;
	
	std::string toString(bool aBPrettyPrint=true);
	
protected:
	ofxSvgCssClass dummyClass;
};
