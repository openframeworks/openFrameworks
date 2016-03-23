#include "JsonConfigParser.h"

#include <regex>

namespace ofx {
namespace DOM {

bool JsonConfigParser::_parse(const ofJson &config, const string &name, bool& val){

	if (config.find(name) != config.end()) {
		ofJson content = config[name];
		if(content.is_boolean()){
			val = content;
			return true;
		}else {
			ofLogError("JsonConfigParser::parse") << "Could not parse " << config << " to boolean.";
		}
	}
	return false;
}

bool JsonConfigParser::_parse(const ofJson &config, const string &name, std::string& val){

	if (config.find(name) != config.end()) {
		ofJson content = config[name];
		if(content.is_string()){
			val = content;
			return true;
		}else {
			ofLogError("JsonConfigParser::parse") << "Could not parse " << config << " to string.";
		}
	}
	return false;
}

//source: https://gist.github.com/olmokramer/82ccce673f86db7cda5e
//complete check: (#(?:[\\da-f]{3}){1,2}|rgb\\((?:\d{1,3},\\s*){2}\\d{1,3}\\)|rgba\\((?:\\d{1,3},\\s*){3}\\d*\\.?\\d+\\)|hsl\\(\\d{1,3}(?:,\\s*\\d{1,3}%){2}\\)|hsla\\(\\d{1,3}(?:,\\s*\\d{1,3}%){2},\\s*\\d*\\.?\\d+\\))
bool JsonConfigParser::_parse(const ofJson &config, const string &name, ofColor& val){

	if (config.find(name) != config.end()) {
		ofJson content = config[name];
		std::string s_value = content;

		//look for colors in hex format
		vector<string> matches = JsonConfigParser::getMatchedStrings(s_value, "#(?:[\\da-f]{3}){1,2}");
		if(matches.size() > 0){
			int x;
			ofStringReplace(matches[0],"#","");
			std::stringstream ss;
			ss << std::hex << matches[0];
			ss >> x;
			val.set(ofColor::fromHex(x,255));
			return true;

		}

		//look for colors in rgba format
		matches.clear();
		matches = JsonConfigParser::getMatchedStrings(s_value, "rgba\\((?:\\d{1,3},\\s*){3}\\d*\\.?\\d+\\)");
		if(matches.size() > 0){
			vector<string> vals = ofSplitString(ofSplitString(ofSplitString(s_value, "rgba(")[1],")")[0],",");
			ofColor res(ofToFloat(vals[0]), ofToFloat(vals[1]), ofToFloat(vals[2]), ofToFloat(vals[3])*255);
			val.set(res);
			return true;
		}

		//loook for colors in rgb format
		matches.clear();
		matches = JsonConfigParser::getMatchedStrings(s_value, "rgb\\((?:\\d{1,3},\\s*){2}\\d{1,3}\\)");
		if(matches.size() > 0){
			vector<string> vals = ofSplitString(ofSplitString(ofSplitString(s_value, "rgb(")[1],")")[0],",");
			ofColor res(ofToFloat(vals[0]), ofToFloat(vals[1]), ofToFloat(vals[2]));
			val.set(res);
			return true;
		}

	}
	return false;

}

bool JsonConfigParser::_parse(const ofJson &config, const string &name, ofPoint& val){

	if (config.find("left") != config.end()) {
		ofJson left = config["left"];
		if(left.is_number()){
			val.x = left;
		}
	}
	if (config.find("top") != config.end()) {
		ofJson top = config["top"];
		if(top.is_number()){
			val.y = top;
		}
	}

	return true;
}


bool JsonConfigParser::_parse(const ofJson &config, const string &name, ofRectangle& val){

	if (config.find("left") != config.end()) {
		ofJson left = config["left"];
		if(left.is_number()){
			val.x = left;
		}
	}
	if (config.find("top") != config.end()) {
		ofJson top = config["top"];
		if(top.is_number()){
			val.y = top;
		}
	}
	if (config.find("width") != config.end()) {
		ofJson width = config["width"];
		if(width.is_number()){
			val.width = width;
		}
	}
	if (config.find("height") != config.end()) {
		ofJson height = config["height"];
		if(height.is_number()){
			val.height = height;
		}
	}
	return true;

}

bool JsonConfigParser::_parse(const ofJson &config, const string &name, LayoutFloat& val){

	if (config.find(name) != config.end()) {
		ofJson content = config[name];
		if(content == "none"){
			val = LayoutFloat::NONE;
			return true;
		}
		if(content == "left"){
			val = LayoutFloat::LEFT;
			return true;
		}
		if(content == "right"){
			val = LayoutFloat::RIGHT;
			return true;
		}
	}
	return false;
}

bool JsonConfigParser::parse(const ofJson &config, Element* val){

	if(!config.is_null()){
		if (config.find("left") != config.end()) {
			ofJson left = config["left"];
			if(left.is_number()){
				val->setPosition(left, val->getPosition().y);
			}
		}
		if (config.find("top") != config.end()) {
			ofJson top = config["top"];
			if(top.is_number()){
				val->setPosition(val->getPosition().x, top);
			}
		}
		if (config.find("width") != config.end()) {
			ofJson width = config["width"];
			if(width.is_number()){
				val->setWidth(width);
				val->setPercentalWidth(false);
			}else {
				if(ofSplitString(width, "%").size() > 0){
					vector<std::string> _val = JsonConfigParser::getMatchedStrings(width, "(?:\\b|-)([1-9]{1,2}[0]?|100)\\b");
					if(_val.size() > 0){
						float res = ofToFloat(_val[0])/100.;
						val->setPercentalWidth(true, res);
						return true;
					}
				}
			}
		}
		if (config.find("height") != config.end()) {
			ofJson height = config["height"];
			if(height.is_number()){
				val->setHeight(height);
				return true;
			}
		}
	}
	return false;

}


vector < string > JsonConfigParser::getMatchedStrings (string contents, string regex ){

	vector < string > results;
	std::regex regEx(regex);

	std::regex_iterator<std::string::iterator> rit (contents.begin(), contents.end(), regEx);
	std::regex_iterator<std::string::iterator> rend;

	while (rit!=rend) {
		results.push_back(rit->str());
		++rit;
	}

	return results;
}


}}
