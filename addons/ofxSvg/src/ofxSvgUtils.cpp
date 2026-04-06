#include "ofxSvgUtils.h"
#include <stdexcept>
#include "ofImage.h"

/*----------------------------------------------------------------------*/
/*
 
 base64 encoding and decoding with C++.
 More information at
 https://renenyffenegger.ch/notes/development/Base64/Encoding-and-decoding-base-64-with-cpp
 
 Version: 2.rc.09 (release candidate)
 
 Copyright (C) 2004-2017, 2020-2022 René Nyffenegger
 
 This source code is provided 'as-is', without any express or implied
 warranty. In no event will the author be held liable for any damages
 arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
 1. The origin of this source code must not be misrepresented; you must not
 claim that you wrote the original source code. If you use this source code
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.
 
 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original source code.
 
 3. This notice may not be removed or altered from any source distribution.
 
 René Nyffenegger rene.nyffenegger@adp-gmbh.ch
 
 */


/*
 https://github.com/ReneNyffenegger/cpp-base64/blob/master/base64.cpp
 This code has been adapted to work in only a .cpp file.
 // certain functions have been ommitted.
 */

static const char* base64_chars[2] = {
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789"
	"+/",
	
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789"
	"-_"};


static unsigned int pos_of_char(const unsigned char chr) {
	//
	// Return the position of chr within base64_encode()
	//
	
	if      (chr >= 'A' && chr <= 'Z') return chr - 'A';
	else if (chr >= 'a' && chr <= 'z') return chr - 'a' + ('Z' - 'A')               + 1;
	else if (chr >= '0' && chr <= '9') return chr - '0' + ('Z' - 'A') + ('z' - 'a') + 2;
	else if (chr == '+' || chr == '-') return 62; // Be liberal with input and accept both url ('-') and non-url ('+') base 64 characters (
	else if (chr == '/' || chr == '_') return 63; // Ditto for '/' and '_'
	else
		//
		// 2020-10-23: Throw std::exception rather than const char*
		//(Pablo Martin-Gomez, https://github.com/Bouska)
		//
		throw std::runtime_error("Input is not valid base64-encoded data.");
}


std::string ofxSvgUtils::base64_encode(unsigned char const* bytes_to_encode, size_t in_len, bool url) {
	
	size_t len_encoded = (in_len +2) / 3 * 4;
	
	unsigned char trailing_char = url ? '.' : '=';
	
	//
	// Choose set of base64 characters. They differ
	// for the last two positions, depending on the url
	// parameter.
	// A bool (as is the parameter url) is guaranteed
	// to evaluate to either 0 or 1 in C++ therefore,
	// the correct character set is chosen by subscripting
	// base64_chars with url.
	//
	const char* base64_chars_ = base64_chars[url];
	
	std::string ret;
	ret.reserve(len_encoded);
	
	unsigned int pos = 0;
	
	while (pos < in_len) {
		ret.push_back(base64_chars_[(bytes_to_encode[pos + 0] & 0xfc) >> 2]);
		
		if (pos+1 < in_len) {
			ret.push_back(base64_chars_[((bytes_to_encode[pos + 0] & 0x03) << 4) + ((bytes_to_encode[pos + 1] & 0xf0) >> 4)]);
			
			if (pos+2 < in_len) {
				ret.push_back(base64_chars_[((bytes_to_encode[pos + 1] & 0x0f) << 2) + ((bytes_to_encode[pos + 2] & 0xc0) >> 6)]);
				ret.push_back(base64_chars_[  bytes_to_encode[pos + 2] & 0x3f]);
			}
			else {
				ret.push_back(base64_chars_[(bytes_to_encode[pos + 1] & 0x0f) << 2]);
				ret.push_back(trailing_char);
			}
		}
		else {
			
			ret.push_back(base64_chars_[(bytes_to_encode[pos + 0] & 0x03) << 4]);
			ret.push_back(trailing_char);
			ret.push_back(trailing_char);
		}
		
		pos += 3;
	}
	
	
	return ret;
}

//template <typename String> // nh removed to only work with string
std::string ofxSvgUtils::base64_decode(std::string const& encoded_string, bool remove_linebreaks) {
	//
	// decode(…) is templated so that it can be used with String = const std::string&
	// or std::string_view (requires at least C++17)
	//
	
	if (encoded_string.empty()) return std::string();
	
	if (remove_linebreaks) {
		
		std::string copy(encoded_string);
		
		copy.erase(std::remove(copy.begin(), copy.end(), '\n'), copy.end());
		
		return base64_decode(copy, false); // nh modified to work without .cpp and .h
									//		return base64_decode(copy, false);
	}
	
	size_t length_of_string = encoded_string.length();
	size_t pos = 0;
	
	//
	// The approximate length (bytes) of the decoded string might be one or
	// two bytes smaller, depending on the amount of trailing equal signs
	// in the encoded string. This approximation is needed to reserve
	// enough space in the string to be returned.
	//
	size_t approx_length_of_decoded_string = length_of_string / 4 * 3;
	std::string ret;
	ret.reserve(approx_length_of_decoded_string);
	
	while (pos < length_of_string) {
		//
		// Iterate over encoded input string in chunks. The size of all
		// chunks except the last one is 4 bytes.
		//
		// The last chunk might be padded with equal signs or dots
		// in order to make it 4 bytes in size as well, but this
		// is not required as per RFC 2045.
		//
		// All chunks except the last one produce three output bytes.
		//
		// The last chunk produces at least one and up to three bytes.
		//
		
		size_t pos_of_char_1 = pos_of_char(encoded_string.at(pos+1) );
		
		//
		// Emit the first output byte that is produced in each chunk:
		//
		ret.push_back(static_cast<std::string::value_type>( ( (pos_of_char(encoded_string.at(pos+0)) ) << 2 ) + ( (pos_of_char_1 & 0x30 ) >> 4)));
		
		if ( ( pos + 2 < length_of_string  )       &&  // Check for data that is not padded with equal signs (which is allowed by RFC 2045)
			encoded_string.at(pos+2) != '='     &&
			encoded_string.at(pos+2) != '.'         // accept URL-safe base 64 strings, too, so check for '.' also.
			)
		{
			//
			// Emit a chunk's second byte (which might not be produced in the last chunk).
			//
			unsigned int pos_of_char_2 = pos_of_char(encoded_string.at(pos+2) );
			ret.push_back(static_cast<std::string::value_type>( (( pos_of_char_1 & 0x0f) << 4) + (( pos_of_char_2 & 0x3c) >> 2)));
			
			if ( ( pos + 3 < length_of_string )     &&
				encoded_string.at(pos+3) != '='  &&
				encoded_string.at(pos+3) != '.'
				)
			{
				//
				// Emit a chunk's third byte (which might not be produced in the last chunk).
				//
				ret.push_back(static_cast<std::string::value_type>( ( (pos_of_char_2 & 0x03 ) << 6 ) + pos_of_char(encoded_string.at(pos+3))   ));
			}
		}
		
		pos += 4;
	}
	
	return ret;
}

/* end base64 encoding / decoding */
/*----------------------------------------------------------------------*/


//---------------------------------------------------------------
std::string ofxSvgUtils::base64_encode( const ofPixels& apixels ) {
	ofBuffer tbuffer;
	ofSaveImage(apixels, tbuffer);
	auto buffStr = tbuffer.getText();
	const unsigned char* data = reinterpret_cast<const unsigned char*>(buffStr.data());
	return ofxSvgUtils::base64_encode( data, tbuffer.size(), false );
}

//---------------------------------------------------------------
ofPixels ofxSvgUtils::base64_decode(std::string const& encoded_string ) {
	ofBuffer tbuffer;
	
	ofPixels rpix;
	
	std::string estring = encoded_string;
	ofStringReplace(estring, "data:image/png;base64,", "" );
	
	if( estring.empty() ) {
		return rpix;
	}
	
	try {
		std::string decoded_string = base64_decode(estring, false );
		
		if( decoded_string.size() < 1 ) {
			return rpix;
		}
		tbuffer.set(decoded_string);
		
		bool bok = ofLoadImage(rpix, tbuffer);
		ofLogNotice("ofxSvgUtils::base64_decode") << "pixels ok: " << bok << " pixels: " << rpix.getWidth() << " x " << rpix.getHeight();
		
	} catch (const std::runtime_error& e) {
		if (std::string(e.what()) == "Input is not valid base64-encoded data.") {
			ofLogError("ofxSvgUtils::base64_decode") << "Caught specific base64 error: " << e.what() << "\n";
		} else {
			ofLogError("ofxSvgUtils::base64_decode") << "Caught runtime_error: " << e.what() << "\n";
		}
	}
	catch (...) {
		ofLogError("ofxSvgUtils::base64_decode") << "Caught unknown exception.\n";
	}
	
	
	
//	bool bok = ofLoadImage(rpix, tbuffer);
//	ofLogNotice("ofxSvgUtils::base64_decode") << "pixels ok: " << bok << " pixels: " << rpix.getWidth() << " x " << rpix.getHeight();
	return rpix;
}

