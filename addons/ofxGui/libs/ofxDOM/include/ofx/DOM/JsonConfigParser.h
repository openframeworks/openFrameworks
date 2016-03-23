#pragma once

#include "ofMain.h"
#include "ofx/DOM/Types.h"
#include "ofx/DOM/Element.h"

namespace ofx {
namespace DOM {

class JsonConfigParser {
	public:

		template <typename AttributeType>
		static bool parse(const ofJson &config, ofParameter<AttributeType>& attribute){
			AttributeType res = attribute.get();
			parse<AttributeType>(config, attribute.getName(), res);
			if(attribute.get() == res){
				//no changes
				return false;
			}else{
				//changes
				attribute.set(res);
				return true;

			}

		}

		template <typename AttributeType>
		static bool parse(const ofJson &config, const string& name, AttributeType& attribute){
			if(!config.is_null()){
				return _parse(config, name, attribute);
			}
			return false;
		}

		static bool parse(const ofJson &config, Element* val);

		///\brief regex helper function copied from https://github.com/openframeworks/openFrameworks/issues/1110
		static vector <std::string> getMatchedStrings (std::string contents, string regex);

	protected:


		/// \todo this method is not called for arithmetic values (don't know why)
		/// that's why I copied the content to the default _parse function at the bottom.
		/// works but should be changed someday.
		template<typename AttributeType>
		static bool _parse(const ofJson &config, const string& name,
						   typename std::enable_if<std::is_arithmetic<AttributeType>::value, AttributeType>::type& val){

			if (config.find(name) != config.end()) {
				ofJson content = config[name];
				if(content.is_number()){
					val = content;
					return true;
				}else {
					ofLogError("JsonConfigParser::parse") << "Could not parse " << config << " to arithmetic value.";
				}
			}
			return false;
		}
		static bool _parse(const ofJson &config, const string& name, bool& val);
		static bool _parse(const ofJson &config, const string& name, std::string& val);
		static bool _parse(const ofJson &config, const string& name, ofColor& val);
		static bool _parse(const ofJson &config, const string& name, ofPoint& val);
		static bool _parse(const ofJson &config, const string& name, ofRectangle& val);
		static bool _parse(const ofJson &config, const string& name, LayoutFloat& val);

		///\brief Default parse method for non implemented attribute types
		template <typename AttributeType>
		static bool _parse(const ofJson &config, const string& name, AttributeType& attribute){

			if (config.find(name) != config.end()) {
				ofJson content = config[name];
				if(content.is_number()){
					attribute = content;
					return true;
				}else {
					ofLogError("JsonConfigParser::parse") << "Could not parse " << config << " to arithmetic value.";
				}
			}

			// TODO throw error when no parsing method found?
			return false;
		}

};

}} // namespace ofx::DOM
