#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"
#include <ft2build.h>
#include FT_FREETYPE_H

class ofApp: public ofxUnitTestsApp{
	void run(){
		FT_Library library;
		FT_Error err = FT_Init_FreeType(&library);
		ofxTestEq(err, (FT_Error)0, "FT_Init_FreeType() succeeds");

		FT_Int major = 0, minor = 0, patch = 0;
		FT_Library_Version(library, &major, &minor, &patch);
		ofxTest(major > 0 || minor > 0, "FT_Library_Version() reports a sane version");

		FT_Error doneErr = FT_Done_FreeType(library);
		ofxTestEq(doneErr, (FT_Error)0, "FT_Done_FreeType() succeeds");
	}
};

int main(){
	ofInit();
	auto window = std::make_shared<ofAppNoWindow>();
	auto app = std::make_shared<ofApp>();
	ofRunApp(window, app);
	return ofRunMainLoop();
}
