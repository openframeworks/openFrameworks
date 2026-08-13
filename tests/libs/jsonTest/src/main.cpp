#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"
#include "json.hpp"

class ofApp: public ofxUnitTestsApp{
	void run(){
		using nlohmann::json;

		auto parsed = json::parse(R"({"name":"openFrameworks","version":12,"addons":["ofxGui","ofxOsc"]})");
		ofxTestEq(parsed["name"].get<std::string>(), std::string("openFrameworks"), "parses string field");
		ofxTestEq(parsed["version"].get<int>(), 12, "parses int field");
		ofxTestEq(parsed["addons"].size(), (size_t)2, "parses array field");
		ofxTestEq(parsed["addons"][0].get<std::string>(), std::string("ofxGui"), "array element 0");

		json built;
		built["ok"] = true;
		built["count"] = 3;
		ofxTestEq(built.dump(), std::string("{\"count\":3,\"ok\":true}"), "serializes back to json");

		ofxTest(!json::accept(std::string("{not valid json")), "rejects malformed json");
	}
};

int main(){
	ofInit();
	auto window = std::make_shared<ofAppNoWindow>();
	auto app = std::make_shared<ofApp>();
	ofRunApp(window, app);
	return ofRunMainLoop();
}
