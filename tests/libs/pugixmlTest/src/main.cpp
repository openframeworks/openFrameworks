#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"
#include <pugixml.hpp>

class ofApp: public ofxUnitTestsApp{
	void run(){
		const char* xml = "<settings><app version=\"12\"><addon>ofxGui</addon><addon>ofxOsc</addon></app></settings>";

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_string(xml);
		ofxTest(result, "parses well-formed xml");

		auto app = doc.child("settings").child("app");
		ofxTestEq(std::string(app.attribute("version").value()), std::string("12"), "reads an attribute");

		int addonCount = 0;
		for(auto addon : app.children("addon")) addonCount++;
		ofxTestEq(addonCount, 2, "iterates repeated child elements");
		ofxTestEq(std::string(app.child("addon").child_value()), std::string("ofxGui"), "reads element text");

		pugi::xml_document bad;
		pugi::xml_parse_result badResult = bad.load_string("<unclosed>");
		ofxTest(!badResult, "rejects malformed xml");
	}
};

int main(){
	ofInit();
	auto window = std::make_shared<ofAppNoWindow>();
	auto app = std::make_shared<ofApp>();
	ofRunApp(window, app);
	return ofRunMainLoop();
}
