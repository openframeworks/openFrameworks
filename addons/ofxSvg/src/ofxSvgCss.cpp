#include "ofxSvgCss.h"
#include "ofUtils.h"
#include "ofLog.h"
#include <regex>
#include <map>
//#include <optional>


std::map<std::string, ofColor> sCommonColors = {
	{"white", ofColor(255, 255, 255)},
	{"black", ofColor(0, 0, 0)},
	{"red", ofColor(255, 0, 0)},
	{"green", ofColor(0, 255, 0)},
	{"blue", ofColor(0, 0, 255)},
	{"yellow", ofColor(255, 255, 0)},
	{"cyan", ofColor(0, 255, 255)},
	{"magenta", ofColor(255, 0, 255)},
	{"gray", ofColor(128, 128, 128)},
	{"orange", ofColor(255, 165, 0)},
	{"brown", ofColor(165, 42, 42)},
	{"pink", ofColor(255, 192, 203)},
	{"purple", ofColor(128, 0, 128)},
	{"lime", ofColor(0, 255, 0)},
	{"maroon", ofColor(128, 0, 0)},
	{"navy", ofColor(0, 0, 128)},
	{"olive", ofColor(128, 128, 0)},
	{"teal", ofColor(0, 128, 128)},
	{"violet", ofColor(238, 130, 238)},
	{"indigo", ofColor(75, 0, 130)},
	{"gold", ofColor(255, 215, 0)},
	{"silver", ofColor(192, 192, 192)},
	{"beige", ofColor(245, 245, 220)},
	{"lavender", ofColor(230, 230, 250)},
	{"turquoise", ofColor(64, 224, 208)},
	{"sky blue", ofColor(135, 206, 235)},
	{"mint", ofColor(189, 252, 201)},
	{"coral", ofColor(255, 127, 80)},
	{"salmon", ofColor(250, 128, 114)},
	{"khaki", ofColor(240, 230, 140)},
	{"ivory", ofColor(255, 255, 240)},
	{"peach", ofColor(255, 218, 185)},
	{"aquamarine", ofColor(127, 255, 212)},
	{"chartreuse", ofColor(127, 255, 0)},
	{"plum", ofColor(221, 160, 221)},
	{"chocolate", ofColor(210, 105, 30)},
	{"orchid", ofColor(218, 112, 214)},
	{"tan", ofColor(210, 180, 140)},
	{"slate gray", ofColor(112, 128, 144)},
	{"periwinkle", ofColor(204, 204, 255)},
	{"sea green", ofColor(46, 139, 87)},
	{"mauve", ofColor(224, 176, 255)},
	{"rose", ofColor(255, 0, 127)},
	{"rust", ofColor(183, 65, 14)},
	{"amber", ofColor(255, 191, 0)},
	{"crimson", ofColor(220, 20, 60)},
	{"sand", ofColor(194, 178, 128)},
	{"jade", ofColor(0, 168, 107)},
	{"denim", ofColor(21, 96, 189)},
	{"copper", ofColor(184, 115, 51)}
};

//--------------------------------------------------------------
void ofxSvgCssClass::clear() {
	properties.clear();
	name = "default";
}

//--------------------------------------------------------------
void ofxSvgCssClass::scaleNumericalValues( float ascale ) {
	for( auto& piter : properties ) {
		if(piter.second.fvalue.has_value()) {
			piter.second.fvalue = piter.second.fvalue.value() * ascale;
		}
		if( piter.second.ivalue.has_value() ) {
			piter.second.ivalue = piter.second.ivalue.value() * ascale;
		}
	}
}

//--------------------------------------------------------------
std::string ofxSvgCssClass::sRgbaToHexString(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	std::stringstream ss;
	ss << std::hex << std::setfill('0') << std::uppercase;
	ss << std::setw(2) << static_cast<int>(r);
	ss << std::setw(2) << static_cast<int>(g);
	ss << std::setw(2) << static_cast<int>(b);
	ss << std::setw(2) << static_cast<int>(a);
	return "#"+ss.str();
}

//--------------------------------------------------------------
std::string ofxSvgCssClass::sRgbToHexString(unsigned char r, unsigned char g, unsigned char b) {
	std::stringstream ss;
	ss << std::hex << std::setfill('0') << std::uppercase;
	ss << std::setw(2) << static_cast<int>(r);
	ss << std::setw(2) << static_cast<int>(g);
	ss << std::setw(2) << static_cast<int>(b);
	return "#"+ss.str();
}

//--------------------------------------------------------------
bool ofxSvgCssClass::sIsNone( const std::string& astr ) {
	if( astr.empty() ) {
		return true;
	}
	if( ofToLower(astr) == "none" ) {
		return true;
	}
	return false;
}

//--------------------------------------------------------------
ofColor ofxSvgCssClass::sGetColor(const std::string& astr ) {
	bool bHasHash = false;
	std::string cstr = astr;
	if( ofIsStringInString(cstr, "#")) {
		ofStringReplace(cstr, "#", "");
		bHasHash = true;
	}
	cstr = ofToLower(cstr);
	// if this is only 3 chars, it is short hand and needs to be expanded to 6 characters
	if( cstr.size() == 3 ) {
		std::string fullHex;
		for( char c : cstr ) {
			fullHex += c;
			fullHex += c;
		}
		cstr = fullHex;
	}
	
	if( bHasHash ) {
		ofColor tcolor(255);
		if( cstr.size() == 8 ) { 
			ofLogVerbose("SvgCss") << "going to try to get the hex from: " << cstr;
			unsigned long hexValue = std::stoul(cstr, nullptr, 16);
			// Manually extract components from the 8-digit hex value (RRGGBBAA)
			int r = (hexValue >> 24) & 0xFF; // Extract the red component
			int g = (hexValue >> 16) & 0xFF; // Extract the green component
			int b = (hexValue >> 8) & 0xFF;  // Extract the blue component
			int a = hexValue & 0xFF;         // Extract the alpha component
			
			// Create the color using the extracted components
			tcolor.set(r, g, b, a);
		} else {
			int hint = ofHexToInt(cstr);
			tcolor.setHex(hint);
		}
//		ofLogNotice("ofxSvgCssClass") << "color: " << cstr << " ofColor: " << tcolor;
		return tcolor;
	} else if( !astr.empty() ) {
		if( sCommonColors.count(cstr)) {
			return sCommonColors[cstr];
		}
	}
	return ofColor(255);
}

//--------------------------------------------------------------
float ofxSvgCssClass::sGetFloat(const std::string& astr) {
	if( astr.empty() ) return 0.f;
	
//	bool bHasPix = false;
	std::string cstr = astr;
	if( ofIsStringInString(cstr, "px")) {
//		bHasPix = true;
		ofStringReplace(cstr, "px", "");
	}
	return ofToFloat( cstr );
}

//--------------------------------------------------------------
bool ofxSvgCssClass::addMissingClassProperties( const ofxSvgCssClass& aclass ) {
	for( auto& propI : aclass.properties ) {
		if( properties.count(propI.first) < 1 ) {
			properties[propI.first] = propI.second;
		}
	}
	return properties.size() > 0;
}

//--------------------------------------------------------------
bool ofxSvgCssClass::setClassProperties( const ofxSvgCssClass& aclass ) {
	for( auto& propI : aclass.properties ) {
		properties[propI.first] = propI.second;
	}
	return properties.size() > 0;
}

//--------------------------------------------------------------
bool ofxSvgCssClass::addProperties( std::string aPropertiesString ) {
	if( aPropertiesString.size() > 0 ) {
		auto propertiesStr = ofSplitString(aPropertiesString, ";", true, true);
//		int pindex = 0;
		for( auto& propStr : propertiesStr ) {
//			std::cout << "    " << pindex << " - property: " << propStr << std::endl;
			addProperty(propStr);
//			pindex++;
		}
	}
	return properties.size() > 0;
}

//--------------------------------------------------------------
bool ofxSvgCssClass::addProperty( std::string aPropString ) {
	auto splitProps = ofSplitString(aPropString, ":", true );
	if( splitProps.size() == 2 ) {
		return addProperty(splitProps[0], splitProps[1]);
	}
	return false;
}

//--------------------------------------------------------------
bool ofxSvgCssClass::addProperty( std::string aName, std::string avalue ) {
	if( !aName.empty() && !avalue.empty() ) {
		Property newProp;
		newProp.srcString = avalue;
		ofStringReplace(newProp.srcString, ";", "");
		ofStringReplace(newProp.srcString, "'", "");
		properties[aName] = newProp;
		return true;
	}
	return false;
}

//--------------------------------------------------
bool ofxSvgCssClass::addProperty( const std::string& aName, const Property& aprop ) {
	return addProperty(aName, aprop.srcString);
}

//--------------------------------------------------
bool ofxSvgCssClass::addProperty( const std::string& aName, const float& avalue ) {
	ofxSvgCssClass::Property prop;
	prop.fvalue = avalue;
	prop.srcString = ofToString(avalue);
	return addProperty(aName, prop );
}

//--------------------------------------------------
bool ofxSvgCssClass::addProperty( const std::string& aName, const ofColor& acolor ) {
	ofxSvgCssClass::Property prop;
	prop.cvalue = acolor;
	prop.srcString = sRgbToHexString(acolor.r, acolor.g, acolor.b);
//	ofLogNotice(" CssClass::addProperty") << prop.srcString << " color: " << acolor;
	return addProperty(aName, prop );
}

//--------------------------------------------------
bool ofxSvgCssClass::removeProperty( std::string aPropString ) {
	bool bHas = hasProperty(aPropString);
	properties.erase(aPropString);
	return bHas;
}

//--------------------------------------------------
bool ofxSvgCssClass::setColor(const ofColor& acolor) {
	return addProperty("color", acolor);
}

//--------------------------------------------------
bool ofxSvgCssClass::setFillColor(const ofColor& acolor) {
	return addProperty("fill", acolor);
}

//--------------------------------------------------
bool ofxSvgCssClass::setNoFill() {
	return addProperty("fill", "none" );
}

//--------------------------------------------------
bool ofxSvgCssClass::isFilled() {
	return !isNone("fill");
}

//--------------------------------------------------
bool ofxSvgCssClass::setStrokeColor(const ofColor& acolor) {
	return addProperty("stroke", acolor);
}

//--------------------------------------------------
bool ofxSvgCssClass::setStrokeWidth( const float& awidth ) {
	return addProperty("stroke-width", awidth);
}

//--------------------------------------------------
bool ofxSvgCssClass::setNoStroke() {
//	return addProperty("stroke", "none" );
	bool bstroke = removeProperty("stroke");
	bool bstrokeW = removeProperty("stroke-width");
	return bstroke || bstrokeW;
}

//--------------------------------------------------
bool ofxSvgCssClass::hasStroke() {
	return !isNone("stroke");
}

//--------------------------------------------------
bool ofxSvgCssClass::setFontSize( int asize ) {
	return addProperty("font-size", asize);
}

//--------------------------------------------------
int ofxSvgCssClass::getFontSize(int adefault) {
	return getIntValue("font-size", adefault);
}

//--------------------------------------------------
bool ofxSvgCssClass::setFontFamily( std::string aFontFamily ) {
	return addProperty("font-family", aFontFamily);
}

//--------------------------------------------------
std::string ofxSvgCssClass::getFontFamily( std::string aDefaultFontFamily ) {
	return getValue("font-family", aDefaultFontFamily);
}

//--------------------------------------------------
bool ofxSvgCssClass::setFontBold( bool ab ) {
	return addProperty("font-weight", ab ? "bold" : "regular" );
}

//--------------------------------------------------
bool ofxSvgCssClass::isFontBold() {
	bool bold = false;
	if( ofIsStringInString(getValue("font-weight", "" ), "bold")) {
		bold = true;
	}
	return bold;
}

//--------------------------------------------------
bool ofxSvgCssClass::setFontItalic(bool ab) {
	return addProperty("font-style", ab ? "italic" : "regular" );
}

//--------------------------------------------------
bool ofxSvgCssClass::isFontItalic() {
	bool italic = false;
	if( ofIsStringInString(getValue("font-style", "" ), "italic")) {
		italic = true;
	}
	return italic;
}

//--------------------------------------------------
bool ofxSvgCssClass::hasProperty( const std::string& akey ) {
	return (properties.count(akey) > 0);
}

//--------------------------------------------------
ofxSvgCssClass::Property& ofxSvgCssClass::getProperty( const std::string& akey ) {
	if( properties.count(akey) < 1 ) {
		return dummyProp;
	}
	return properties[akey];
}

//--------------------------------------------------
bool ofxSvgCssClass::isNone(const std::string& akey) {
	if( properties.count(akey) < 1 ) {
		return true;
	}
	return sIsNone( properties[akey].srcString );
}

//--------------------------------------------------
bool ofxSvgCssClass::hasAndIsNone(const std::string& akey) {
	if( hasProperty(akey)) {
		return sIsNone( properties[akey].srcString );
	}
	return false;
}

//--------------------------------------------------
std::string ofxSvgCssClass::getValue(const std::string& akey, const std::string& adefault) {
	if( properties.count(akey) < 1 ) {
		return adefault;
	}
	auto& prop = properties[akey];
	if(!prop.svalue.has_value()) {
		prop.svalue = prop.srcString;
	}
	return prop.svalue.value();
}

//--------------------------------------------------
int ofxSvgCssClass::getIntValue(const std::string& akey, const int& adefault) {
	if( properties.count(akey) < 1 ) {
		return adefault;
	}
	auto& prop = properties[akey];
	if(!prop.ivalue.has_value()) {
		prop.ivalue = ofToInt(prop.srcString);
	}
	return prop.ivalue.value();
}

//--------------------------------------------------
float ofxSvgCssClass::getFloatValue(const std::string& akey, const float& adefault) {
	if( properties.count(akey) < 1 ) {
		return adefault;
	}
	auto& prop = properties[akey];
	if( !prop.fvalue.has_value() ) {
		prop.fvalue = sGetFloat(prop.srcString);
	}
	return prop.fvalue.value();
}

//--------------------------------------------------
ofColor ofxSvgCssClass::getColor(const std::string& akey) {
	return getColor(akey, ofColor(0));
}

//--------------------------------------------------
ofColor ofxSvgCssClass::getColor(const std::string& akey, const ofColor& adefault) {
	if( properties.count(akey) < 1 ) {
		return adefault;
	}
	auto& prop = properties[akey];
	if( !prop.cvalue.has_value() ) {
		prop.cvalue = sGetColor(prop.srcString);
	}
	return prop.cvalue.value();
}

//--------------------------------------------------
std::string ofxSvgCssClass::toString(bool aBPrettyPrint) {
	std::stringstream ss;
	for( auto& piter : properties ) {
		if(aBPrettyPrint) {
			ss << std::endl << "    ";
		}
		ss << piter.first << ":" << piter.second.srcString << ";";
	}
	return ss.str();
}

//--------------------------------------------------
bool ofxSvgCssStyleSheet::parse( std::string aCssString ) {
	if( aCssString.empty() ) {
		return false;
	}
	
	classes.clear();
	
	ofStringReplace(aCssString, "<![CDATA[", "");
	ofStringReplace(aCssString, "]]>", "");
	
	// Regular expression to match class names (e.g., .cls-1, .cls-2, etc.)
	std::regex class_regex(R"((\.[\w\-]+(?:,\s*\.[\w\-]+)*))");
	// Regular expression to match properties within curly braces (e.g., fill: none; stroke-miterlimit: 10;)
	std::regex property_regex(R"(\{([^}]+)\})");
	
	std::smatch class_match;
	std::smatch property_match;
	
	// Search for each class rule block
	auto search_start = aCssString.cbegin();
	while (std::regex_search(search_start, aCssString.cend(), class_match, class_regex)) {
		std::string class_list = class_match[1];  // Extract the class list (e.g., .cls-1, .cls-2)
		
		// Move search start forward to find the corresponding property block
		std::string::const_iterator prop_search_start = class_match.suffix().first;
		
		// Find the corresponding properties block
		if (std::regex_search(prop_search_start, aCssString.cend(), property_match, property_regex)) {
			std::string properties_block = property_match[1];  // Extract properties
			std::map<std::string, std::string> properties;
			
			// Split properties into key-value pairs
			std::regex property_pair_regex(R"(([\w\-]+)\s*:\s*([^;]+);?)");
			std::smatch property_pair_match;
			
			auto prop_search_start = properties_block.cbegin();
			while (std::regex_search(prop_search_start, properties_block.cend(), property_pair_match, property_pair_regex)) {
				std::string key = property_pair_match[1];  // e.g., "fill"
				std::string value = property_pair_match[2];  // e.g., "none"
				properties[key] = value;  // Add the property to the map
				prop_search_start = property_pair_match.suffix().first;  // Continue searching for more properties
			}
			
			// Process the list of classes (comma-separated classes)
			std::regex individual_class_regex(R"(\.[\w\-]+)");
			auto class_search_start = class_list.cbegin();
			std::smatch individual_class_match;
			while (std::regex_search(class_search_start, class_list.cend(), individual_class_match, individual_class_regex)) {
				std::string class_name = individual_class_match[0];  // Extract the individual class (e.g., .cls-1)
				
				if( class_name.size() > 0 && class_name[0] == '.' ) {
					class_name = class_name.substr(1, std::string::npos);
				}
				
				// Merge properties for the class (with priority for the latest properties)
				for (const auto& prop : properties) {
					auto& svgCssClass = addClass(class_name);
					svgCssClass.addProperty(prop.first, prop.second);
//					cssClasses[class_name][prop.first] = prop.second;
				}
				
				class_search_start = individual_class_match.suffix().first;  // Move to the next class
			}
		}
		
		search_start = property_match.suffix().first;  // Move to the next block
	}
	return classes.size() > 0;
}

//--------------------------------------------------
void ofxSvgCssStyleSheet::clear() {
	classes.clear();
}

//--------------------------------------------------
void ofxSvgCssStyleSheet::scaleNumericalValues( float ascale ) {
	for( auto& tclass : classes ) {
		tclass.second.scaleNumericalValues(ascale);
	}
}

//--------------------------------------------------
ofxSvgCssClass& ofxSvgCssStyleSheet::addClass(std::string aname) {
	if( hasClass(aname) ) {
		return classes[aname];
	}
	ofxSvgCssClass tclass;
	tclass.name = aname;
	classes[aname] = tclass;
	return classes[aname];
}

//--------------------------------------------------
bool ofxSvgCssStyleSheet::hasClass(const std::string& aname) {
	return classes.count(aname) > 0;
}

//--------------------------------------------------
ofxSvgCssClass& ofxSvgCssStyleSheet::getClass( const std::string& aname ) {
	if( hasClass(aname)) {
		return classes[aname];
	}
	ofLogWarning("ofxSvgCssStyleSheet") << "could not find class " << aname;
	return dummyClass;
}

//--------------------------------------------------
ofxSvgCssClass& ofxSvgCssStyleSheet::getAddClass( ofxSvgCssClass& aclass ) {
	
	for( auto& tclass : classes ) {
		bool bFoundAll = true;
		
		// check for the same number of properties
		if( tclass.second.properties.size() != aclass.properties.size() ) {
			continue;
		}
		
		for( auto& aprop : aclass.properties ) {
			bool bFound = false;
			for( auto& tprop : tclass.second.properties ) {
				if( tprop.first == aprop.first && tprop.second.srcString == aprop.second.srcString) {
					bFound = true;
					break;
				}
			}
			if( !bFound ) {
				bFoundAll = false;
			}
		}
		
		if( bFoundAll ) {
			// we found another class that has all the things
//			ofLogNotice("CssStyleSheet::getAddClass") << "found matching class: " << tclass.second.name;
			return classes[tclass.second.name];
		}
	}
	
	// check if name already exists
	std::string className = aclass.name;
	int ccounter = 0;
	bool bKeepGoing = true;
	while( bKeepGoing ) {
		bool bFound = false;
		for( auto& tclass : classes ) {
			if( className == tclass.first ) {
				bFound = true;
				break;
			}
		}
		if( bFound ) {
			ccounter ++;
			className = aclass.name + ofToString(ccounter);
		} else {
			bKeepGoing = false;
		}
	}
	
	aclass.name = className;
	classes[className] = aclass;
	return classes[className];
}

//--------------------------------------------------
std::string ofxSvgCssStyleSheet::toString(bool aBPrettyPrint) {
	std::stringstream ss;
	for( auto& citer : classes ) {
		ss << std::endl;
		ss << "." << citer.first << " { ";
		ss << citer.second.toString(aBPrettyPrint);
		ss << "}" << std::endl;
	}
	
	return ss.str();
}
