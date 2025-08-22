#pragma once
#include <unordered_map>
#include "ofColor.h"
#include "ofLog.h"
#include "ofXml.h"

class ofxSvgCssClass {
public:
	
	// adding this Optional class since std::optional is not a part of all std:: distributions at the moment, looking at you gcc < 10 nh
	template<typename T>
	class Optional {
	public:
		Optional() : hasValue(false) {}  // Default constructor, no value
		Optional(const T& value) : hasValue(true), data(value) {}  // Construct with a value
		Optional(T&& value) : hasValue(true), data(std::move(value)) {}  // Move constructor
		
		// Copy and move constructors
		Optional(const Optional& other) = default;
		Optional(Optional&& other) noexcept = default;
		
		// Assignment operators
		Optional& operator=(const Optional& other) = default;
		Optional& operator=(Optional&& other) noexcept = default;
		
		// Destructor
		~Optional() = default;
		
		// Check if there's a value
		bool has_value() const { return hasValue; }
		
		// Accessors for the value
		T& value() {
//			if (!hasValue) throw std::runtime_error("No value present");
			if (!hasValue) {
				ofLogError("ofxSvgCssClass") << "No value present";
			}
			return data;
		}
		
		const T& value() const {
//			if (!hasValue) throw std::runtime_error("No value present");
			if (!hasValue) {
				ofLogError("ofxSvgCssClass") << "No value present";
			}
			return data;
		}
		
		// Reset to an empty state
		void reset() { hasValue = false; }
		
	private:
		bool hasValue;
		T data;
	};
	
	class Property {
	public:
		std::string srcString;
		Optional<float> fvalue;
		Optional<int> ivalue;
		Optional<std::string> svalue;
		Optional<ofColor> cvalue;
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
	
	bool isFontBold();
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
