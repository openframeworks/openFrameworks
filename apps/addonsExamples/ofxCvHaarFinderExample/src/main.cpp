#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"
#include "ofAppNoWindow.h"
//#include "dummy_of.h"

void delayUs( unsigned long us )
{
	// sleep for the requested number of us
	struct timespec req = {0};
	req.tv_sec = us/(1000*1000);
	req.tv_nsec = (us%(1000*1000))*(1000);
	// avoid interruptions
	while (nanosleep( &req, &req ) == -1 && errno == EINTR )
		continue;
}


//========================================================================
int main( ){

#ifdef NO_WINDOW 
	ofAppNoWindow window;	

	//runDummyOf( new testApp() );

#else
	ofAppGlutWindow window;
#endif
	ofSetupOpenGL(&window, 640,640, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
