#pragma once
#include <string>
#include "ofPixels.h"

// adding this Optional class since std::optional is not a part of all std:: distributions at the moment, looking at you gcc < 10 nh
// and not included in older versions of OF on Windows, ie. 12.0.
template<typename T>
class ofxSvgOptional {
public:
	ofxSvgOptional() : hasValue(false) {}  // Default constructor, no value
	ofxSvgOptional(const T& value) : hasValue(true), data(value) {}  // Construct with a value
	ofxSvgOptional(T&& value) : hasValue(true), data(std::move(value)) {}  // Move constructor
	
	// Copy and move constructors
	ofxSvgOptional(const ofxSvgOptional& other) = default;
	ofxSvgOptional(ofxSvgOptional&& other) noexcept = default;
	
	// Assignment operators
	ofxSvgOptional& operator=(const ofxSvgOptional& other) = default;
	ofxSvgOptional& operator=(ofxSvgOptional&& other) noexcept = default;
	
	// Destructor
	~ofxSvgOptional() = default;
	
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


class ofxSvgUtils {
public:
    
//	Adapted from code by René Nyffenegger.
//	base64 encoding and decoding with C++.
//	More information at
//	https://renenyffenegger.ch/notes/development/Base64/Encoding-and-decoding-base-64-with-cpp
	static std::string base64_encode(unsigned char const* bytes_to_encode, size_t in_len, bool url);
	static std::string base64_decode(std::string const& encoded_string, bool remove_linebreaks);
	
	static std::string base64_encode( const ofPixels& apixels );
	static ofPixels base64_decode(std::string const& encoded_string );
	
	
	
};
