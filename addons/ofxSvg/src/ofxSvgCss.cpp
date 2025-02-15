//
//  ofxSvg2Css.cp
//  Example
//
//  Created by Nick Hardeman on 8/22/24.
//

#include "ofxSvgCss.h"
#include "ofUtils.h"
#include "ofLog.h"
#include <regex>
#include <map>
#include <optional>

using namespace ofx::svg;

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
void CssClass::clear() {
	properties.clear();
	name = "default";
}

//--------------------------------------------------------------
std::string CssClass::sRgbaToHexString(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	std::stringstream ss;
	ss << std::hex << std::setfill('0') << std::uppercase;
	ss << std::setw(2) << static_cast<int>(r);
	ss << std::setw(2) << static_cast<int>(g);
	ss << std::setw(2) << static_cast<int>(b);
	ss << std::setw(2) << static_cast<int>(a);
	return "#"+ss.str();
}

//--------------------------------------------------------------
std::string CssClass::sRgbToHexString(unsigned char r, unsigned char g, unsigned char b) {
	std::stringstream ss;
	ss << std::hex << std::setfill('0') << std::uppercase;
	ss << std::setw(2) << static_cast<int>(r);
	ss << std::setw(2) << static_cast<int>(g);
	ss << std::setw(2) << static_cast<int>(b);
	return "#"+ss.str();
}

//--------------------------------------------------------------
bool CssClass::sIsNone( const std::string& astr ) {
	if( astr.empty() ) {
		return true;
	}
	if( ofToLower(astr) == "none" ) {
		return true;
	}
	return false;
}

//--------------------------------------------------------------
ofColor CssClass::sGetColor(const std::string& astr ) {
	bool bHasHash = false;
	std::string cstr = astr;
	if( ofIsStringInString(cstr, "#")) {
		ofStringReplace(cstr, "#", "");
		bHasHash = true;
	}
	cstr = ofToLower(cstr);
	
	if( bHasHash ) {
		ofColor tcolor(255);
		int hint = ofHexToInt(cstr);
		tcolor.setHex(hint);
		return tcolor;
	} else if( !astr.empty() ) {
		if( sCommonColors.count(cstr)) {
			return sCommonColors[cstr];
		}
	}
	return ofColor(255);
}

//--------------------------------------------------------------
float CssClass::sGetFloat(const std::string& astr) {
	if( astr.empty() ) return 0.f;
	
	bool bHasPix = false;
	std::string cstr = astr;
	if( ofIsStringInString(cstr, "px")) {
		bHasPix = true;
		ofStringReplace(cstr, "px", "");
	}
	return ofToFloat( cstr );
}

//--------------------------------------------------------------
bool CssClass::addProperties( std::string aPropertiesString ) {
	if( aPropertiesString.size() > 0 ) {
		auto propertiesStr = ofSplitString(aPropertiesString, ";", true, true);
		int pindex = 0;
		for( auto& propStr : propertiesStr ) {
//			std::cout << "    " << pindex << " - property: " << propStr << std::endl;
			addProperty(propStr);
			pindex++;
		}
		
//		for( auto& prop : properties ) {
//			ofLogNotice("ofx::svg2::CssClass") << " prop: " << prop.first << " : " << prop.second.srcString;
//		}
	}
	return properties.size() > 0;
}

//--------------------------------------------------------------
bool CssClass::addProperty( std::string aPropString ) {
	auto splitProps = ofSplitString(aPropString, ":", true );
	if( splitProps.size() == 2 ) {
		return addProperty(splitProps[0], splitProps[1]);
	}
	return false;
}

//--------------------------------------------------------------
bool CssClass::addProperty( std::string aName, std::string avalue ) {
	if( !aName.empty() && !avalue.empty() ) {
		Property newProp;
		newProp.srcString = avalue;
		ofStringReplace(newProp.srcString, ";", "");
//		if( ofIsStringInString(newProp.srcString, "px")) {
//			newProp.bInPixels = true;
//			ofStringReplace(newProp.srcString, "px", "");
//		}
//		if( ofIsStringInString(newProp.srcString, "#")) {
//			newProp.bHasHash = true;
//			ofStringReplace(newProp.srcString, "#", "");
//		}
		
		ofStringReplace(newProp.srcString, "'", "");
		properties[aName] = newProp;
//		if( newProp.bInPixels ) {
//			getFloatValue(aName, 1.f);
//		}
		return true;
	}
	return false;
}

//--------------------------------------------------
bool CssClass::addProperty( const std::string& aName, const Property& aprop ) {
	return addProperty(aName, aprop.srcString);
}

//--------------------------------------------------
bool CssClass::addProperty( const std::string& aName, const float& avalue ) {
	ofx::svg::CssClass::Property prop;
	prop.fvalue = avalue;
	prop.srcString = ofToString(avalue);
	return addProperty(aName, prop );
}

//--------------------------------------------------
bool CssClass::addProperty( const std::string& aName, const ofColor& acolor ) {
	ofx::svg::CssClass::Property prop;
	prop.cvalue = acolor;
	prop.srcString = sRgbToHexString(acolor.r, acolor.g, acolor.b);
//	ofLogNotice(" CssClass::addProperty") << prop.srcString << " color: " << acolor;
	return addProperty(aName, prop );
}

//--------------------------------------------------
bool CssClass::setFillColor(const ofColor& acolor) {
	return addProperty("fill", acolor);
}

//--------------------------------------------------
bool CssClass::setNoFill() {
	return addProperty("fill", "none" );
}

//--------------------------------------------------
bool CssClass::isFilled() {
	return !isNone("fill");
}

//--------------------------------------------------
bool CssClass::setStrokeColor(const ofColor& acolor) {
	return addProperty("stroke", acolor);
}

//--------------------------------------------------
bool CssClass::setStrokeWidth( const float& awidth ) {
	return addProperty("stroke-width", awidth);
}

//--------------------------------------------------
bool CssClass::setNoStroke() {
	return addProperty("stroke", "none" );
}

//--------------------------------------------------
bool CssClass::hasStroke() {
	return !isNone("stroke");
}

//--------------------------------------------------
bool CssClass::hasProperty( const std::string& akey ) {
	return (properties.count(akey) > 0);
}

//--------------------------------------------------
CssClass::Property& CssClass::getProperty( const std::string& akey ) {
	if( properties.count(akey) < 1 ) {
		return dummyProp;
	}
	return properties[akey];
}

//--------------------------------------------------
bool CssClass::isNone(const std::string& akey) {
	if( properties.count(akey) < 1 ) {
		return true;
	}
	return sIsNone( properties[akey].srcString );
}

//--------------------------------------------------
bool CssClass::hasAndIsNone(const std::string& akey) {
	if( hasProperty(akey)) {
		return sIsNone( properties[akey].srcString );
	}
	return false;
}

//--------------------------------------------------
std::string CssClass::getValue(const std::string& akey, const std::string& adefault) {
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
int CssClass::getIntValue(const std::string& akey, const int& adefault) {
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
float CssClass::getFloatValue(const std::string& akey, const float& adefault) {
	if( properties.count(akey) < 1 ) {
		return adefault;
	}
	auto& prop = properties[akey];
	if( !prop.fvalue.has_value() ) {
		prop.fvalue = sGetFloat(prop.srcString);
//		bool bHasPix = false;
//		std::string cstr = prop.srcString;
//		if( ofIsStringInString(cstr, "px")) {
//			bHasPix = true;
//			ofStringReplace(cstr, "px", "");
//		}
//		prop.fvalue = ofToFloat( cstr );
	}
	return prop.fvalue.value();
}

//--------------------------------------------------
ofColor CssClass::getColor(const std::string& akey) {
	if( properties.count(akey) < 1 ) {
		return ofColor(255);
	}
	auto& prop = properties[akey];
	if( !prop.cvalue.has_value() ) {
		prop.cvalue = sGetColor(prop.srcString);
	}
	return prop.cvalue.value();
}

//--------------------------------------------------
std::string CssClass::toString(bool aBPrettyPrint) {
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
bool CssStyleSheet::parse( std::string aCssString ) {
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
void CssStyleSheet::clear() {
	classes.clear();
}

//--------------------------------------------------
CssClass& CssStyleSheet::addClass(std::string aname) {
	if( hasClass(aname) ) {
//		ofLogWarning("ofx::svg2") << "CssStyleSheet already has class " << aname <<".";
		return classes[aname];
	}
	CssClass tclass;
	tclass.name = aname;
	classes[aname] = tclass;
	return classes[aname];
}

//--------------------------------------------------
bool CssStyleSheet::hasClass(const std::string& aname) {
	return classes.count(aname) > 0;
}

//--------------------------------------------------
CssClass& CssStyleSheet::getClass( const std::string& aname ) {
	if( hasClass(aname)) {
		return classes[aname];
	}
	ofLogWarning("ofx::svg2::CssStyleSheet") << "could not find class " << aname;
	return dummyClass;
}

//--------------------------------------------------
CssClass& CssStyleSheet::getAddClass( CssClass& aclass ) {
	
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
std::string CssStyleSheet::toString(bool aBPrettyPrint) {
	std::stringstream ss;
	for( auto& citer : classes ) {
		ss << std::endl;
		ss << "." << citer.first << " { ";
		ss << citer.second.toString(aBPrettyPrint);
		ss << "}" << std::endl;
	}
	
	return ss.str();
}
