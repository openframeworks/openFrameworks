#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"
#include "ofAppNoWindow.h"


//========================================================================
int main(  int argc, char *argv[]  ){

   
#ifdef TARGET_LINUX
	if(argc==1){
		ofAppGlutWindow window;
		ofSetupOpenGL(&window, 1024,768, OF_WINDOW);
		ofRunApp( new testApp());
	}else{
		ofAppNoWindow window;
		ofSetupOpenGL(&window, 1024,768, OF_WINDOW);
		testApp * app = new testApp;
		app->projectPath = ofFilePath::getAbsolutePath(argv[argc-1],false);
		cout << argv[0] << endl;
		ofRunApp( app );
	}
#else 
    ofAppGlutWindow window;
    ofSetupOpenGL(&window, 1024,768, OF_WINDOW);
    ofRunApp( new testApp());
    
#endif
    
    
}
