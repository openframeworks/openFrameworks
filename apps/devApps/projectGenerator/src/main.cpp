#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"
#include "ofAppNoWindow.h"


//========================================================================
int main(  int argc, char *argv[]  ){

   
    cout << "In main." << endl;
    
    cout << "ofGetTargetPlatform=" << ofGetTargetPlatform() << endl;
    
#ifdef TARGET_LINUX
	if(argc==1){
		ofAppGlutWindow window;
		ofSetupOpenGL(&window, 1024,768, OF_WINDOW);
		ofRunApp( new ofApp());
	}else{
		ofAppNoWindow window;
		ofSetupOpenGL(&window, 1024,768, OF_WINDOW);
		ofApp * app = new ofApp;
		app->buildAllExamples = false;
		app->projectPath = "";
		for(int i=1;i<argc;i++){
			string arg = argv[i];
			if(arg.find("--")==0){
				arg = arg.substr(2);
				if(arg=="linux"){
					app->targetsToMake.push_back( OF_TARGET_LINUX );
				}else if(arg=="linux64"){
					app->targetsToMake.push_back( OF_TARGET_LINUX64 );
				}else if(arg=="linuxarmv6l"){
					app->targetsToMake.push_back( OF_TARGET_LINUXARMV6L );
				}else if(arg=="linuxarmv7l"){
					app->targetsToMake.push_back( OF_TARGET_LINUXARMV7L );
				}else if(arg=="win_cb"){
					app->targetsToMake.push_back( OF_TARGET_WINGCC );
				}else if(arg=="vs"){
					app->targetsToMake.push_back( OF_TARGET_WINVS );
				}else if(arg=="osx"){
					app->targetsToMake.push_back( OF_TARGET_OSX );
				}else if(arg=="ios"){
					app->targetsToMake.push_back( OF_TARGET_IPHONE );
				}else if(arg=="android"){
					ofLogError() << "platform not supported yet" << endl;
					std::exit(1);
				}else if(arg=="allplatforms"){
					app->targetsToMake.push_back( OF_TARGET_LINUX );
					app->targetsToMake.push_back( OF_TARGET_LINUX64 );
					app->targetsToMake.push_back( OF_TARGET_LINUXARMV6L );
					app->targetsToMake.push_back( OF_TARGET_LINUXARMV7L );
					app->targetsToMake.push_back( OF_TARGET_WINGCC );
					app->targetsToMake.push_back( OF_TARGET_WINVS );
					app->targetsToMake.push_back( OF_TARGET_OSX );
					app->targetsToMake.push_back( OF_TARGET_IPHONE );
				}else if(arg=="allexamples"){
					app->buildAllExamples = true;
				}else if(arg=="help"){
					cout << "OF Project Generator Usage:" << endl;
					cout << "projectGenerator [options] [pathToExample]" << endl;
					cout << "Options:" << endl;
					cout << "--osx: generate osx project files" << endl;
					cout << "--win_cb: generate windows codeblocks project files" << endl;
					cout << "--vs: generate windows visual studio project files" << endl;
					cout << "--linux: generate linux project files" << endl;
					cout << "--linux64: generate linux 64bits project files" << endl;
					cout << "--linuxarmv6l: generate linux armv6l project files" << endl;
					cout << "--linuxarmv7l: generate linux armv7l project files" << endl;
					cout << "--ios: generate iOS project files" << endl;
					cout << "--allplatforms: generate project files for all platforms" << endl;
					cout << "--allexamples: generate project files for all examples. [pathToExample] is not needed in this case." << endl;
					cout << endl;
					cout << "default: create project files for current platform for selected path" << endl;
					cout << "running over existing example updates project files for selected platforms" << endl;
					cout << "without parameters, shows gui" << endl;
					std::exit(0);
				}else{
					cout << "Error: Unknown argument --" << arg << ". Run \'projectGenerator help\' for details. Aborting." << endl;
					std::exit(1);
				}
			}else if(arg.find("-", 0, 1)==0){
				cout << "Error: Unknown argument " << arg << ". Run \'projectGenerator help\' for details. Aborting." << endl;
				std::exit(1);
			}else{
				app->projectPath = ofFilePath::removeTrailingSlash(ofFilePath::getPathForDirectory(ofFilePath::getAbsolutePath(arg,false)));
			}
		}

		if (!(app->buildAllExamples) && app->projectPath.empty()){
			cout << "Error: No target project given. Either run projectGenerator with the --allexamples option or supply a target project folder path. Run \'projectGenerator help\' for details." << endl;
			std::exit(1);
		}

		if(app->targetsToMake.empty())
			app->targetsToMake.push_back( ofGetTargetPlatform() );
		ofRunApp( app );
	}
#else 
    cout << "IN HERE!!" << endl;
    ofAppGlutWindow window;
    ofSetupOpenGL(&window, 1024,768, OF_WINDOW);
    ofApp * app = new ofApp;
    app->buildAllExamples = false;
    ofRunApp( app );
    
#endif
    
    
}
