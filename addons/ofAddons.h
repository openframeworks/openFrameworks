//#warning ______________________________________________________________________________
//#warning !!! ofAddons.h is deprecated - from OF 007 and on this file will NOT EXIST !!!
//#warning To see the new usage for using addons in OF please see www.openframeworks.cc/006-Important-Notes
//#warning ______________________________________________________________________________

// these #define are kept around to make sure that old code doesn't break, but from 0.07 we will be using a non #define system for addons.
// see:  http://www.openframeworks.cc/006-Important-Notes

//-------------------------------------------- openCV
#ifdef OF_ADDON_USING_OFXOPENCV
	#include "ofxCvMain.h"
#endif

//-------------------------------------------- dir list
#ifdef OF_ADDON_USING_OFXDIRLIST
	#include "ofxDirList.h"
#endif

//-------------------------------------------- vector math
#ifdef OF_ADDON_USING_OFXVECTORMATH
	#include "ofxVectorMath.h"
#endif

//-------------------------------------------- network
#ifdef OF_ADDON_USING_OFXNETWORK
	#include "ofxNetwork.h"
#endif

//-------------------------------------------- vector graphics
#ifdef OF_ADDON_USING_OFXVECTORGRAPHICS
	#include "ofxVectorGraphics.h"
#endif

//-------------------------------------------- OSC
#ifdef OF_ADDON_USING_OFXOSC
	#include "ofxOsc.h"
#endif

//-------------------------------------------- ofthread
#ifdef OF_ADDON_USING_OFXTHREAD
	#include "ofxThread.h"
#endif

//-------------------------------------------- ofXML
#ifdef OF_ADDON_USING_OFXXMLSETTINGS
	#include "ofxXmlSettings.h"
#endif
