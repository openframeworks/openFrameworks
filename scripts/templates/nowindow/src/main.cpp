#include "ofApp.h"
#include "ofAppRunner.h"
#include "ofAppNoWindow.h"

int main(int argumentCount, char* argumentValues[]){
	// Command line arguments are presented as an array of null-terminated
	// c-strings passed in the `argumentValues` variable. The length of the
	// length of the string array is equal to the argument count.
	//
	// To create an openFrameworks application with without an OpenGL window,
	// we pass pointers to our ofAppNoWindow instance and our ofApp instance to
	// ofRunApp. Then we call the ofRunMainLoop() function to start the program.

	auto window = std::make_shared<ofAppNoWindow>();
	auto app = std::make_shared<ofApp>();

	ofRunApp(window,app);

	return ofRunMainLoop();
}
