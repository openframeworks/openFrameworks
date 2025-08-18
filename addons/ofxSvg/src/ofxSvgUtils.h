#pragma once
#include <string>
#include "ofPixels.h"

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
