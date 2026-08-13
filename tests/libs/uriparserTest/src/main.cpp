#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"
#include <uriparser/Uri.h>
#include <string>

class ofApp: public ofxUnitTestsApp{
	void run(){
		const std::string url = "https://openframeworks.cc/download/?version=0.12.1#top";

		UriUriA uri;
		const char* errorPos;
		int rc = uriParseSingleUriA(&uri, url.c_str(), &errorPos);
		ofxTestEq(rc, 0, "parses a well-formed url");

		auto rangeToString = [](const UriTextRangeA& r) -> std::string {
			if(!r.first || !r.afterLast) return "";
			return std::string(r.first, r.afterLast - r.first);
		};

		ofxTestEq(rangeToString(uri.scheme), std::string("https"), "extracts scheme");
		ofxTestEq(rangeToString(uri.hostText), std::string("openframeworks.cc"), "extracts host");
		ofxTestEq(rangeToString(uri.fragment), std::string("top"), "extracts fragment");

		uriFreeUriMembersA(&uri);

		UriUriA badUri;
		const char* badErrorPos;
		int badRc = uriParseSingleUriA(&badUri, "not a uri : : :", &badErrorPos);
		ofxTest(badRc != 0, "rejects malformed uri");
		uriFreeUriMembersA(&badUri);
	}
};

int main(){
	ofInit();
	auto window = std::make_shared<ofAppNoWindow>();
	auto app = std::make_shared<ofApp>();
	ofRunApp(window, app);
	return ofRunMainLoop();
}
