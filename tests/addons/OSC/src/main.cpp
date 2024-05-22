#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"
#include "ofxOsc.h"

class ofApp: public ofxUnitTestsApp {
	void test_messages_constructors(){
		const char * chars = "/const_char";
		ofxOscMessage m_chars {chars};
		ofxTest(m_chars.getAddress() ==  "/const_char","ofxOscMessage{const char *}");

		const std::string cstr { "/const_string" };
		ofxOscMessage m_cstr{cstr};
		ofxTest(m_cstr.getAddress() ==  "/const_string","ofxOscMessage{const std::string}");

		std::string str { "/string" };
		ofxOscMessage m_str {str};
		ofxTest(m_str.getAddress() ==  "/string","ofxOscMessage{std::string");

		std::string & strref = str;
		ofxOscMessage m_strref {strref};
		ofxTest(m_strref.getAddress() ==  "/string","ofxOscMessage{std::string &}");

		const std::string & cstrref = cstr;
		ofxOscMessage m_cstrref {cstrref};
		ofxTest(m_cstrref.getAddress() ==  "/const_string","ofxOscMessage{const std::string &}");
		
		std::string mv { "/string_for_move" };
		ofxOscMessage m_mv {std::move(mv)};
		ofxTest(m_mv.getAddress() ==  "/string_for_move","ofxOscMessage{std::string &&}");
		
		std::unique_ptr<ofxOscReceiver> r = std::make_unique<ofxOscReceiver>();
		r = std::make_unique<ofxOscReceiver>();
		r = std::make_unique<ofxOscReceiver>();
		r = std::make_unique<ofxOscReceiver>();
		r = std::make_unique<ofxOscReceiver>();
		r->setup(2222);

		ofxTest(r.get()->getPort() == 2222, "ofxOscReceiver: port test");
		
	}

	void run(){
		test_messages_constructors();
	}
};

int main( ){
    ofInit();
    auto window = std::make_shared<ofAppNoWindow>();
    auto app = std::make_shared<ofApp>();
    ofRunApp(window, app);
    return ofRunMainLoop();
}
