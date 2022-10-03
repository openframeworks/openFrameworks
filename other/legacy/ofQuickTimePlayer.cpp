#include "ofQuickTimePlayer.h"
#include "ofUtils.h"

#if !defined(TARGET_LINUX) && (!defined(MAC_OS_X_VERSION_10_12) || MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_12)
#ifdef  OF_VIDEO_PLAYER_QUICKTIME

bool  	createMovieFromPath(char * path, Movie &movie);
bool 	createMovieFromPath(char * path, Movie &movie){

	Boolean 	isdir			= false;
	OSErr 		result 			= 0;
	FSSpec 		theFSSpec;

	short 		actualResId 	= DoTheRightThing;

	#ifdef TARGET_WIN32
		result = NativePathNameToFSSpec (path, &theFSSpec, 0);
		if (result != noErr) {
			ofLogError("ofQuickTimePlayer") << "createMovieFromPath(): couldn't load movie, NativePathNameToFSSpec failed: OSErr " << result;
			return false;
		}

	#endif

	#ifdef TARGET_OSX
		FSRef 		fsref;
		result = FSPathMakeRef((const UInt8*)path, &fsref, &isdir);
		if (result) {
			ofLogError("ofQuickTimePlayer") << "createMovieFromPath(): couldn't load movie, FSPathMakeRef failed: OSErr " << result;
			return false;
		}
		result = FSGetCatalogInfo(&fsref, kFSCatInfoNone, nullptr, nullptr, &theFSSpec, nullptr);
		if (result) {
			ofLogError("ofQuickTimePlayer") << "createMovieFromPath(): couldn't load movie, FSGetCatalogInfo failed: OSErr ", result;
			return false;
		}
	#endif

	short movieResFile;
	result = OpenMovieFile (&theFSSpec, &movieResFile, fsRdPerm);
	if (result == noErr) {

		short   movieResID = 0;
		result = NewMovieFromFile(&movie, movieResFile, &movieResID, (unsigned char *) 0, newMovieActive, (Boolean *) 0);
		if (result == noErr){
			CloseMovieFile (movieResFile);
		} else {
			ofLogError("ofQuickTimePlayer") << "createMovieFromPath(): couldn't load movie, NewMovieFromFile failed: OSErr " << result;
			return false;
		}
	} else {
		ofLogError("ofQuickTimePlayer") << "createMovieFromPath(): couldn't load movie, OpenMovieFile failed: OSErr " << result;
		return false;
	}

	return true;
}

//--------------------------------------------------------------
bool createMovieFromURL(string urlIn,  Movie &movie){
	char * url = (char *)urlIn.c_str();
	Handle urlDataRef;

	OSErr err;

	urlDataRef = NewHandle(strlen(url) + 1);
	if ( ( err = MemError()) != noErr){
		ofLogError("ofQuickTimePlayer") << "createMovieFromURL(): couldn't create url handle from \"" << urlIn << "\": OSErr " << err;
		return false;
	}

	BlockMoveData(url, *urlDataRef, strlen(url) + 1);

	err = NewMovieFromDataRef(&movie, newMovieActive,nil, urlDataRef, URLDataHandlerSubType);
	DisposeHandle(urlDataRef);

	if(err != noErr){
		ofLogError("ofQuickTimePlayer") << "createMovieFromURL(): couldn't load url \"" << urlIn << "\": OSErr " << err;
		return false;
	}else{
		return true;
	}

	return false;
}


//--------------------------------------------------------------
OSErr 	DrawCompleteProc(Movie theMovie, long refCon);
OSErr 	DrawCompleteProc(Movie theMovie, long refCon){

	ofQuickTimePlayer * ofvp = (ofQuickTimePlayer *)refCon;

	#if defined(TARGET_OSX) && defined(__BIG_ENDIAN__)
		convertPixels(ofvp->offscreenGWorldPixels, ofvp->pixels.getPixels(), ofvp->width, ofvp->height);
	#endif

	ofvp->bHavePixelsChanged = true;
	return noErr;
}

//---------------------------------------------------------------------------
ofQuickTimePlayer::ofQuickTimePlayer (){

	//--------------------------------------------------------------
   #if defined(TARGET_WIN32) || defined(TARGET_OSX)
    //--------------------------------------------------------------
    	moviePtr	 				= nullptr;
    	allocated 					= false;
        offscreenGWorld				= nullptr;
	//--------------------------------------------------------------
	#endif
	//--------------------------------------------------------------

	bLoaded 					= false;
	width 						= 0;
	height						= 0;
	speed 						= 1;
	bStarted					= false;
	nFrames						= 0;
	bPaused						= true;
	currentLoopState			= OF_LOOP_NORMAL;
}


//---------------------------------------------------------------------------
ofQuickTimePlayer::~ofQuickTimePlayer(){

	closeMovie();
    clearMemory();
    
	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------
		if(allocated)	delete[] offscreenGWorldPixels;
		if ((offscreenGWorld)) DisposeGWorld((offscreenGWorld));
	//--------------------------------------
	#endif
	//--------------------------------------

}

//---------------------------------------------------------------------------
ofPixels& ofQuickTimePlayer::getPixels(){
	return pixels;
}

//---------------------------------------------------------------------------
const ofPixels& ofQuickTimePlayer::getPixels() const {
	return pixels;
}

//---------------------------------------------------------------------------
void ofQuickTimePlayer::update(){

	if (bLoaded == true){

		//--------------------------------------------------------------
		#ifdef OF_VIDEO_PLAYER_QUICKTIME
		//--------------------------------------------------------------
			
			// is this necessary on windows with quicktime?
			#ifdef TARGET_OSX 
				// call MoviesTask if we're not on the main thread
				if ( CFRunLoopGetCurrent() != CFRunLoopGetMain() )
				{
					//ofLog( OF_LOG_NOTICE, "not on the main loop, calling MoviesTask") ;
					MoviesTask(moviePtr,0);
				}
			#else
				// on windows we always call MoviesTask
				MoviesTask(moviePtr,0);
			#endif

		//--------------------------------------------------------------
		#endif
		//--------------------------------------------------------------
	}

	// ---------------------------------------------------
	// 		on all platforms,
	// 		do "new"ness ever time we idle...
	// 		before "isFrameNew" was clearning,
	// 		people had issues with that...
	// 		and it was badly named so now, newness happens
	// 		per-idle not per isNew call
	// ---------------------------------------------------
	
	if (bLoaded == true){

		bIsFrameNew = bHavePixelsChanged;
		if (bHavePixelsChanged == true) {
			bHavePixelsChanged = false;
		}
	}

}

//---------------------------------------------------------------------------
bool ofQuickTimePlayer::isFrameNew() const{
	return bIsFrameNew;
}
//---------------------------------------------------------------------------
void ofQuickTimePlayer::close(){
	closeMovie();
}

//---------------------------------------------------------------------------
void ofQuickTimePlayer::closeMovie(){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

	if (bLoaded == true){

	    DisposeMovie (moviePtr);
		DisposeMovieDrawingCompleteUPP(myDrawCompleteProc);

		moviePtr = nullptr;
        
    }

   	//--------------------------------------
	#endif
    //--------------------------------------

	bLoaded = false;

}


//--------------------------------------
#ifdef OF_VIDEO_PLAYER_QUICKTIME
//--------------------------------------

void ofQuickTimePlayer::createImgMemAndGWorld(){
	Rect movieRect;
	movieRect.top 			= 0;
	movieRect.left 			= 0;
	movieRect.bottom 		= height;
	movieRect.right 		= width;
	offscreenGWorldPixels = new unsigned char[4 * width * height + 32];
	allocated				= true;
	pixels.allocate(width,height,OF_IMAGE_COLOR);

	#if defined(TARGET_OSX) && defined(__BIG_ENDIAN__)
		QTNewGWorldFromPtr (&(offscreenGWorld), k32ARGBPixelFormat, &(movieRect), nullptr, nullptr, 0, (offscreenGWorldPixels), 4 * width);
	#else
		QTNewGWorldFromPtr (&(offscreenGWorld), k24RGBPixelFormat, &(movieRect), nullptr, nullptr, 0, (pixels.getPixels()), 3 * width);
	#endif

	LockPixels(GetGWorldPixMap(offscreenGWorld));

    // from : https://github.com/openframeworks/openFrameworks/issues/244
    // SetGWorld do not seems to be necessary for offscreen rendering of the movie
    // only SetMovieGWorld should be called
    // if both are called, the app will crash after a few ofVideoPlayer object have been deleted

	#ifndef TARGET_WIN32
        SetGWorld (offscreenGWorld, nullptr);
	#endif
	SetMovieGWorld (moviePtr, offscreenGWorld, nil);

}

//--------------------------------------
#endif
//--------------------------------------


//---------------------------------------------------------------------------
bool ofQuickTimePlayer::load(string name){


	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

		initializeQuicktime();			// init quicktime
		closeMovie();					// if we have a movie open, close it
		bLoaded 				= false;	// try to load now


    // from : https://github.com/openframeworks/openFrameworks/issues/244
    // http://developer.apple.com/library/mac/#documentation/QuickTime/RM/QTforWindows/QTforWindows/C-Chapter/3BuildingQuickTimeCa.html
    // Apple's documentation *seems* to state that a Gworld should have been set prior to calling NewMovieFromFile
    // So I set a dummy Gworld (1x1 pixel) before calling createMovieFromPath
    // it avoids crash at the creation of objet ofVideoPlayer after a previous ofVideoPlayer have been deleted

    #ifdef TARGET_WIN32
        if (width != 0 && height != 0){
            pixels.clear();
            delete [] offscreenGWorldPixels;
        }
        width = 1;
        height = 1;
        createImgMemAndGWorld();
    #endif


		if( name.substr(0, 7) == "http://" || name.substr(0,7) == "rtsp://" ){
			if(! createMovieFromURL(name, moviePtr) ) return false;
		}else{
			name 					= ofToDataPath(name);
			if( !createMovieFromPath((char *)name.c_str(), moviePtr) ) return false;
		}

		bool bDoWeAlreadyHaveAGworld = false;
		if (width != 0 && height != 0){
			bDoWeAlreadyHaveAGworld = true;
		}
		Rect 				movieRect;
		GetMovieBox(moviePtr, &(movieRect));
		if (bDoWeAlreadyHaveAGworld){
			// is the gworld the same size, then lets *not* de-allocate and reallocate:
			if (width == movieRect.right &&
				height == movieRect.bottom){
				SetMovieGWorld (moviePtr, offscreenGWorld, nil);
			} else {
				width 	= movieRect.right;
				height 	= movieRect.bottom;
				pixels.clear();
				delete [] offscreenGWorldPixels;
				if ((offscreenGWorld)) DisposeGWorld((offscreenGWorld));
				createImgMemAndGWorld();
			}
		} else {
			width	= movieRect.right;
			height 	= movieRect.bottom;
			createImgMemAndGWorld();
		}

		if (moviePtr == nullptr){
			return false;
		}

		//----------------- callback method
	    myDrawCompleteProc = NewMovieDrawingCompleteUPP (DrawCompleteProc);
		SetMovieDrawingCompleteProc (moviePtr, movieDrawingCallWhenChanged,  myDrawCompleteProc, (long)this);

		// ------------- get the total # of frames:
		nFrames				= 0;
		TimeValue			curMovieTime;
		curMovieTime		= 0;
		TimeValue			duration;

		//OSType whichMediaType	= VIDEO_TYPE; // mingw chokes on this
		OSType whichMediaType	= FOUR_CHAR_CODE('vide');

		short flags				= nextTimeMediaSample + nextTimeEdgeOK;

		while( curMovieTime >= 0 ) {
			nFrames++;
			GetMovieNextInterestingTime(moviePtr,flags,1,&whichMediaType,curMovieTime,0,&curMovieTime,&duration);
			flags = nextTimeMediaSample;
		}
		nFrames--; // there's an extra time step at the end of themovie




		// ------------- get some pixels in there ------
		GoToBeginningOfMovie(moviePtr);
		SetMovieActiveSegment(moviePtr, -1,-1);
		MoviesTask(moviePtr,0);

		#if defined(TARGET_OSX) && defined(__BIG_ENDIAN__)
			convertPixels(offscreenGWorldPixels, pixels.getPixels(), width, height);
		#endif

		bStarted 				= false;
		bLoaded 				= true;
		bPlaying 				= false;
		bHavePixelsChanged 		= false;
		speed 					= 1;

		return true;

	//--------------------------------------
	#endif
	//--------------------------------------



}



//--------------------------------------------------------
void ofQuickTimePlayer::start(){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

	if (bLoaded == true && bStarted == false){
		SetMovieActive(moviePtr, true);

		//------------------ set the movie rate to default
		//------------------ and preroll, so the first frames come correct

		TimeValue timeNow 	= 	GetMovieTime(moviePtr, 0);
		Fixed playRate 		=	GetMoviePreferredRate(moviePtr); 		//Not being used!

		PrerollMovie(moviePtr, timeNow, X2Fix(speed));
		SetMovieRate(moviePtr,  X2Fix(speed));
		setLoopState(currentLoopState);

		// get some pixels in there right away:
		MoviesTask(moviePtr,0);
		#if defined(TARGET_OSX) && defined(__BIG_ENDIAN__)
			convertPixels(offscreenGWorldPixels, pixels.getPixels(), width, height);
		#endif
		bHavePixelsChanged = true;

		bStarted = true;
		bPlaying = true;
	}

	//--------------------------------------
	#endif
	//--------------------------------------
}

//--------------------------------------------------------
void ofQuickTimePlayer::play(){
	if( !isLoaded() ){
		ofLogError("ofQuickTimePlayer") << "play(): movie not loaded";
		return;
	}

	bPlaying = true;
	bPaused = false;
	
	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

	if (!bStarted){
	 	start();
	}else {
		// ------------ lower level "startMovie"
		// ------------ use desired speed & time (-1,1,etc) to Preroll...
		TimeValue timeNow;
	   	timeNow = GetMovieTime(moviePtr, nil);
		PrerollMovie(moviePtr, timeNow, X2Fix(speed));
		SetMovieRate(moviePtr,  X2Fix(speed));
		MoviesTask(moviePtr, 0);
	}
	
	//--------------------------------------
	#endif
	//--------------------------------------

	//this is if we set the speed first but it only can be set when we are playing.
	setSpeed(speed);

}

//--------------------------------------------------------
void ofQuickTimePlayer::stop(){
	if( !isLoaded() ){
		ofLogError("ofQuickTimePlayer") << "stop(): movie not loaded";
		return;
	}
	
	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

	StopMovie (moviePtr);
	SetMovieActive (moviePtr, false);
	bStarted = false;

	//--------------------------------------
	#endif
	//--------------------------------------

	bPlaying = false;
}

//--------------------------------------------------------
void ofQuickTimePlayer::setVolume(float volume){
	if( !isLoaded() ){
		ofLogError("ofQuickTimePlayer") << "setVolume(): movie not loaded";
		return;
	}
	
	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

	SetMovieVolume(moviePtr, volume*255);

	//--------------------------------------
	#endif
	//--------------------------------------

}


//--------------------------------------------------------
void ofQuickTimePlayer::setLoopState(ofLoopType state){
	
	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------
		
		if( isLoaded() ){

			TimeBase myTimeBase;
			long myFlags = 0L;
			myTimeBase = GetMovieTimeBase(moviePtr);
			myFlags = GetTimeBaseFlags(myTimeBase);
			switch (state) {
				case OF_LOOP_NORMAL:
					myFlags |= loopTimeBase;
					myFlags &= ~palindromeLoopTimeBase;
					SetMoviePlayHints(moviePtr, hintsLoop, hintsLoop);
					SetMoviePlayHints(moviePtr, 0L, hintsPalindrome);
					break;
				case OF_LOOP_PALINDROME:
					myFlags |= loopTimeBase;
					myFlags |= palindromeLoopTimeBase;
					SetMoviePlayHints(moviePtr, hintsLoop, hintsLoop);
					SetMoviePlayHints(moviePtr, hintsPalindrome, hintsPalindrome);
					break;
				case OF_LOOP_NONE:
					default:
					myFlags &= ~loopTimeBase;
					myFlags &= ~palindromeLoopTimeBase;
					SetMoviePlayHints(moviePtr, 0L, hintsLoop |
					hintsPalindrome);
					break;
			}
			SetTimeBaseFlags(myTimeBase, myFlags);
			
		}

	//--------------------------------------
	#endif
	//--------------------------------------

	//store the current loop state;
	currentLoopState = state;

}

//---------------------------------------------------------------------------
ofLoopType ofQuickTimePlayer::getLoopState() const{
	return currentLoopState;
}

//---------------------------------------------------------------------------
void ofQuickTimePlayer::setPosition(float pct){
	if( !isLoaded() ){
		ofLogError("ofQuickTimePlayer") << "setPosition(): movie not loaded";
		return;
	}

 	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

	 	TimeRecord tr;
	 	tr.base 		= GetMovieTimeBase(moviePtr);
		long total 		= GetMovieDuration(moviePtr );
		long newPos 	= (long)((float)total * pct);
		SetMovieTimeValue(moviePtr, newPos);
		MoviesTask(moviePtr,0);

	//--------------------------------------
	#endif
	//--------------------------------------


}

//---------------------------------------------------------------------------
void ofQuickTimePlayer::setFrame(int frame){
	if( !isLoaded() ){
		ofLogError("ofQuickTimePlayer") << "setFrame(): movie not loaded";
		return;
	}
	
	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

	// frame 0 = first frame...

	// this is the simple way...
	//float durationPerFrame = getDuration() / getTotalNumFrames();

	// seems that freezing, doing this and unfreezing seems to work alot
	// better then just SetMovieTimeValue() ;

	if (!bPaused) SetMovieRate(moviePtr, X2Fix(0));

	// this is better with mpeg, etc:
	double frameRate = 0;
	double movieTimeScale = 0;
	MovieGetStaticFrameRate(moviePtr, &frameRate);
	movieTimeScale = GetMovieTimeScale(moviePtr);

	if (frameRate > 0){
		double frameDuration = 1 / frameRate;
		TimeValue t = (TimeValue)(frame * frameDuration * movieTimeScale);
		SetMovieTimeValue(moviePtr, t);
		MoviesTask(moviePtr, 0);
	}

   if (!bPaused) SetMovieRate(moviePtr, X2Fix(speed));

   //--------------------------------------
    #endif
   //--------------------------------------

}


//---------------------------------------------------------------------------
float ofQuickTimePlayer::getDuration() const{
	if( !isLoaded() ){
		ofLogError("ofQuickTimePlayer") << "getDuration(): movie not loaded";
		return 0.0;
	}
	
	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

		return (float) (GetMovieDuration (moviePtr) / (double) GetMovieTimeScale (moviePtr));

	//--------------------------------------
	#endif
	//--------------------------------------

}

//---------------------------------------------------------------------------
float ofQuickTimePlayer::getPosition() const{
	if( !isLoaded() ){
		ofLogError("ofQuickTimePlayer") << "getPosition(): movie not loaded";
		return 0.0;
	}
	
	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

		long total 		= GetMovieDuration(moviePtr);
		long current 	= GetMovieTime(moviePtr, nil);
		float pct 		= ((float)current/(float)total);
		return pct;

	//--------------------------------------
	#endif
	//--------------------------------------


}

//---------------------------------------------------------------------------
int ofQuickTimePlayer::getCurrentFrame() const{
	if( !isLoaded() ){
		ofLogError("ofQuickTimePlayer") << "getCurrentFrame(): movie not loaded";
		return 0;
	}
	
	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

	int frame = 0;

	// zach I think this may fail on variable length frames...
	float pos = getPosition();

	float  framePosInFloat = ((float)getTotalNumFrames() * pos);
	int    framePosInInt = (int)framePosInFloat;
	float  floatRemainder = (framePosInFloat - framePosInInt);
	if (floatRemainder > 0.5f) framePosInInt = framePosInInt + 1;
	//frame = (int)ceil((getTotalNumFrames() * getPosition()));
	frame = framePosInInt;

	return frame;

	//--------------------------------------
	#endif
	//--------------------------------------

}

//---------------------------------------------------------------------------
bool ofQuickTimePlayer::setPixelFormat(ofPixelFormat pixelFormat){
	//note as we only support RGB we are just confirming that this pixel format is supported
	if( pixelFormat == OF_PIXELS_RGB ){
		return true;
	}
	ofLogWarning("ofQuickTimePlayer") << "setPixelFormat(): requested pixel format " << pixelFormat << " not supported, expecting OF_PIXELS_RGB";
	return false;
}

//---------------------------------------------------------------------------
ofPixelFormat ofQuickTimePlayer::getPixelFormat() const{
	//note if you support more than one pixel format you will need to return a ofPixelFormat variable. 
	return OF_PIXELS_RGB;
}


//---------------------------------------------------------------------------
bool ofQuickTimePlayer::getIsMovieDone() const{
	if( !isLoaded() ){
		ofLogError("ofQuickTimePlayer") << "getIsMovieDone(): movie not loaded";
		return false;
	}
	
	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------
		
		bool bIsMovieDone = (bool)IsMovieDone(moviePtr);
		return bIsMovieDone;

	//--------------------------------------
	#endif
	//--------------------------------------

}

//---------------------------------------------------------------------------
void ofQuickTimePlayer::firstFrame(){
	if( !isLoaded() ){
		ofLogError("ofQuickTimePlayer") << "firstFrame(): movie not loaded";
		return;
	}
	
	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------
	
	setFrame(0);
		
	//--------------------------------------
	#endif
	//--------------------------------------

}

//---------------------------------------------------------------------------
void ofQuickTimePlayer::nextFrame(){
	if( !isLoaded() ){
		ofLogError("ofQuickTimePlayer") << "nextFrame(): movie not loaded";
		return;
	}
	
	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------
	
	setFrame(getCurrentFrame() + 1);
	
	//--------------------------------------
	#endif
	//--------------------------------------
}

//---------------------------------------------------------------------------
void ofQuickTimePlayer::previousFrame(){
	if( !isLoaded() ){
		ofLogError("ofQuickTimePlayer") << "previousFrame(): movie not loaded";
		return;
	}
	
	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------
	
	setFrame(getCurrentFrame() - 1);
	
	//--------------------------------------
	#endif
	//--------------------------------------
}



//---------------------------------------------------------------------------
void ofQuickTimePlayer::setSpeed(float _speed){

	speed 				= _speed;

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

		if (bPlaying == true){
			//setMovieRate actually plays, so let's call it only when we are playing
			SetMovieRate(moviePtr, X2Fix(speed));
		}

	//--------------------------------------
	#endif
	//--------------------------------------
}

//---------------------------------------------------------------------------
void ofQuickTimePlayer::setPaused(bool _bPause){

	bPaused = _bPause;

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

		// there might be a more "quicktime-ish" way (or smarter way)
		// to do this for now, to pause, just set the movie's speed to zero,
		// on un-pause, set the movie's speed to "speed"
		// (and hope that speed != 0...)
		if (bPlaying == true){
			if (bPaused == true) 	SetMovieRate(moviePtr, X2Fix(0));
			else 					SetMovieRate(moviePtr, X2Fix(speed));
		}
	
	//--------------------------------------
	#endif
	//--------------------------------------

}









//---------------------------------------------------------------------------
void ofQuickTimePlayer::clearMemory(){

	pixels.clear();

}

//---------------------------------------------------------------------------
float ofQuickTimePlayer::getSpeed() const{
	return speed;
}

//------------------------------------
int ofQuickTimePlayer::getTotalNumFrames() const{
	return nFrames;
}

//----------------------------------------------------------
float ofQuickTimePlayer::getWidth() const{
	return (float)width;
}

//----------------------------------------------------------
float ofQuickTimePlayer::getHeight() const{
	return (float)height;
}

//----------------------------------------------------------
bool ofQuickTimePlayer::isPaused() const{
	return bPaused;
}

//----------------------------------------------------------
bool ofQuickTimePlayer::isLoaded() const{
	return bLoaded;
}

//----------------------------------------------------------
bool ofQuickTimePlayer::isPlaying() const{
	return bPlaying;
}

#endif

#endif


