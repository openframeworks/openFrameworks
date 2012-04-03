#include "ofAppNoWindow.h"
#include "ofBaseApp.h"
#include "ofMain.h"



#if defined TARGET_OSX || defined TARGET_LINUX
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <termios.h>

struct termios orig_termios;
struct sigaction act_open;

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}


void set_conio_terminal_mode()
{
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
	// setup new_termios for raw keyboard input
    cfmakeraw(&new_termios);
	// handle "\n" properly
	new_termios.c_oflag |= OPOST;
	//new_termios.c_oflag |= ONLCR;
	// set the new_termios
    tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit()
{
	return 0;
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

int getch()
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}

#endif


//----------------------------------------------------------
ofAppNoWindow::ofAppNoWindow(){
	timeNow				= 0;
	timeThen			= 0;
	fps					= 60; //give a realistic starting value - win32 issues
	nFramesForFPS		= 0;
	nFrameCount			= 0;
	bFrameRateSet		= false;
	millisForFrame		= 0;
	prevMillis			= 0;
	diffMillis			= 0;
	frameRate			= 0;
	lastFrameTime		= 0;
}

//------------------------------------------------------------
void ofAppNoWindow::runAppViaInfiniteLoop(ofBaseApp * appPtr){
	static ofEventArgs voidEventArgs;

	ofAppPtr = appPtr;

	if(ofAppPtr){
		ofAppPtr->setup();
		ofAppPtr->update();
	}

	#if defined TARGET_OSX || defined TARGET_LINUX
	// for keyboard
	//set_conio_terminal_mode();
	#endif

	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents().setup, voidEventArgs );
		ofNotifyEvent( ofEvents().update, voidEventArgs );
	#endif

    ofLogNotice()<<	"***\n***\n*** ofAppNoWindow running a headerless openFrameworks app\n"
			"***\n*** keyboard input works here\n"
			"***\n*** press Esc or Ctrl-C to quit\n"
			"***\n";
	while (true)
	{
	    if (nFrameCount != 0 && bFrameRateSet == true){
            diffMillis = ofGetElapsedTimeMillis() - prevMillis;
            if (diffMillis > millisForFrame){
                ; // we do nothing, we are already slower than target frame
            } else {
                int waitMillis = millisForFrame - diffMillis;
                #ifdef TARGET_WIN32
                    Sleep(waitMillis);         //windows sleep in milliseconds
                #else
                    usleep(waitMillis * 1000);   //mac sleep in microseconds - cooler :)
                #endif
            }
        }
        prevMillis = ofGetElapsedTimeMillis(); // you have to measure here


        /// listen for escape
        #ifdef TARGET_WIN32
        if (GetAsyncKeyState(VK_ESCAPE))
            OF_EXIT_APP(0);
        #endif

		#if defined TARGET_OSX || defined TARGET_LINUX
		while ( kbhit() )
		{
			int key = getch();
			if ( key == 27 )
			{
				OF_EXIT_APP(0);
			}
			else if ( key == /* ctrl-c */ 3 )
			{
				ofLogNotice()<<	"Ctrl-C pressed\n";
				OF_EXIT_APP(0);
			}
			else
			{
				ofAppPtr->keyPressed( key );
			}
		}
		#endif


        /// update
        if (ofAppPtr)
            ofAppPtr->update();

		#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents().update, voidEventArgs);
		#endif



        // -------------- fps calculation:
		timeNow = ofGetElapsedTimef();
		double diff = timeNow-timeThen;
		if( diff  > 0.00001 ){
			fps			= 1.0 / diff;
			frameRate	*= 0.9f;
			frameRate	+= 0.1f*fps;
		}
		lastFrameTime	= diff;
		timeThen		= timeNow;
		// --------------

		nFrameCount++;		// increase the overall frame count

	}
}

//------------------------------------------------------------
void ofAppNoWindow::exitApp(){

//  -- This already exists in ofExitCallback

//	static ofEventArgs voidEventArgs;
//
//	if(ofAppPtr)ofAppPtr->exit();
//
//	#ifdef OF_USING_POCO
//		ofNotifyEvent( ofEvents.exit, voidEventArgs );
//	#endif

	ofLog(OF_LOG_VERBOSE,"No Window OF app is being terminated!");


#if defined TARGET_OSX || defined TARGET_LINUX
    // this doesn't exist on windows and gives linking errors, so commented out.
	reset_terminal_mode();
#endif

	OF_EXIT_APP(0);
}

//------------------------------------------------------------
float ofAppNoWindow::getFrameRate(){
	return frameRate;
}

//------------------------------------------------------------
int ofAppNoWindow::getFrameNum(){
	return nFrameCount;
}

//------------------------------------------------------------
void ofAppNoWindow::setFrameRate(float targetRate){
	// given this FPS, what is the amount of millis per frame
	// that should elapse?

	// --- > f / s

	if (targetRate == 0){
		bFrameRateSet = false;
		return;
	}

	bFrameRateSet 			= true;
	float durationOfFrame 	= 1.0f / (float)targetRate;
	millisForFrame 			= (int)(1000.0f * durationOfFrame);

	frameRate				= targetRate;

}
