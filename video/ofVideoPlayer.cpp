#include "ofVideoPlayer.h"
#include "ofUtils.h"

//--------------------------------------------------------------
#ifdef  OF_VIDEO_PLAYER_QUICKTIME
//--------------------------------------------------------------

bool  	createMovieFromPath(char * path, Movie &movie);
bool 	createMovieFromPath(char * path, Movie &movie){



	Boolean 	isdir			= false;
	OSErr 		result 			= 0;
	FSSpec 		theFSSpec;

	short 		actualResId 	= DoTheRightThing;

	#ifdef TARGET_WIN32
		result = NativePathNameToFSSpec (path, &theFSSpec, 0);
		if (result != noErr) {
			printf("NativePathNameToFSSpec failed %d\n", result);
			printf("ERROR LOADINGmovie \n");
			return false;
		}

	#endif



	#ifdef TARGET_OSX
		FSRef 		fsref;
		result = FSPathMakeRef((const UInt8*)path, &fsref, &isdir);
		if (result) { printf("FSPathMakeRef failed %d\n", result); printf("ERROR LOADINGmovie \n"); return false; }
		result = FSGetCatalogInfo(&fsref, kFSCatInfoNone, NULL, NULL, &theFSSpec, NULL);
		if (result) { printf("FSGetCatalogInfo failed %d\n", result); printf("ERROR LOADINGmovie \n"); return false; }
	#endif


	short movieResFile;
	result = OpenMovieFile (&theFSSpec, &movieResFile, fsRdPerm);
	if (result == noErr) {

		short   movieResID = 0;
		result = NewMovieFromFile(&movie, movieResFile, &movieResID, (unsigned char *) 0, newMovieActive, (Boolean *) 0);
		if (result == noErr){
			CloseMovieFile (movieResFile);
		} else {
			printf("NewMovieFromFile failed %d\n", result);
			return false;
		}
	} else {
		printf("OpenMovieFile failed %d\n", result);
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
	if ( ( err = MemError()) != noErr){ printf("createMovieFromURL: error creating url handle\n"); return false;}

	BlockMoveData(url, *urlDataRef, strlen(url) + 1);

	err = NewMovieFromDataRef(&movie, newMovieActive,nil, urlDataRef, URLDataHandlerSubType);
	DisposeHandle(urlDataRef);

	if(err != noErr){
		printf("createMovieFromURL: error loading url\n");
		return false;
	}else{
		return true;
	}

	return false;
}



//--------------------------------------------------------------
OSErr 	DrawCompleteProc(Movie theMovie, long refCon);
OSErr 	DrawCompleteProc(Movie theMovie, long refCon){

	ofVideoPlayer * ofvp = (ofVideoPlayer *)refCon;
	convertPixels(ofvp->offscreenGWorldPixels, ofvp->pixels, ofvp->width, ofvp->height);
	ofvp->bHavePixelsChanged = true;
	if (ofvp->bUseTexture == true){
		ofvp->tex.loadData(ofvp->pixels, ofvp->width, ofvp->height, GL_RGB);
	}

	return noErr;
}

//--------------------------------------------------------------
#endif
//--------------------------------------------------------------




//---------------------------------------------------------------------------
ofVideoPlayer::ofVideoPlayer (){

	bLoaded 					= false;
	width 						= 0;
	height						= 0;
	speed 						= 1;
	bUseTexture					= true;
	bStarted					= false;
	pixels						= NULL;
	nFrames						= 0;
	bPaused						= false;

	//--------------------------------------------------------------
    #ifndef  TARGET_LINUX  // !linux = quicktime...
    //--------------------------------------------------------------
    	moviePtr	 				= NULL;
    	allocated 					= false;
        offscreenGWorld				= NULL;
	//--------------------------------------------------------------
    #else
    //--------------------------------------------------------------

		fobsDecoder					= NULL;
	//--------------------------------------------------------------
	#endif
	//--------------------------------------------------------------

}

//---------------------------------------------------------------------------
unsigned char * ofVideoPlayer::getPixels(){
	return pixels;
}

//---------------------------------------------------------------------------
bool ofVideoPlayer::isFrameNew(){

	return bIsFrameNew;

}

//---------------------------------------------------------------------------
void ofVideoPlayer::idleMovie(){

    if (bLoaded == true){

		//--------------------------------------------------------------
		#ifndef  TARGET_LINUX  // !linux = quicktime...
		//--------------------------------------------------------------

			#ifdef TARGET_WIN32 || QT_USE_MOVIETASK
				MoviesTask(moviePtr,0);
			#endif

		//--------------------------------------------------------------
		#else // linux.
		//--------------------------------------------------------------

			diffTime = (float)ofGetElapsedTimeMillis() - (float)(timeLastIdle);
			timeLastIdle = (float)ofGetElapsedTimeMillis();
            float pctDone =  diffTime / durationMillis;   // given this much time, how much of the movie did we do?



            /* -----------------------------------------------------------
            // legacy code, on it's way out.
			in .h:
                //float				frameTime;    // legacy but we keep it in here for now, will remove soon

			in loadMovie:
                //not used anymore, but kept around for reference for now
                //istartTime 			= ofGetElapsedTimeMillis();
                //frameTime 			= ((1.0f / fobsDecoder->getFrameRate())*1000.0f);

			in idleMovie:
                // for the given time, for speed = 1, we did this many frames:
                this is another way of doing it, but I think it somehow drifts over a long time
                (more calculations = more chances for error?
                so we do it the other way (below)
                float amountDone = (float)(diffTime) / (float)frameTime;
                float pctDone = amountDone / (float)iTotalFrames;

                // my drift test
                static float adder = 0;
                static float adder2 = 0;
                adder += pctDone;
                adder2 += pctDone2;
                printf("%f %f   ----- %f \n", adder, adder2, ofGetElapsedTimef()/fobsDecoder->getDurationSeconds())
			----------------------------------------------------------- */

            if (bPaused != true) positionPct += (pctDone * speed); //speed;

			//------------------------------------- now, let's do different things if we are looping or not:
			if (loopMode == OF_LOOP_NONE){
			  if (positionPct > 1) positionPct = 1;
			  if (positionPct < 0) positionPct = 0;
			} else if (loopMode == OF_LOOP_PALINDROME) {
			  if (positionPct > 1) {
			      float diff = positionPct - 1.0f;
			      positionPct = 1 - diff;
			      speed *= -1;
			  }
			  if (positionPct < 0){
			      float diff = -positionPct;
			      positionPct = 0 + diff;
			      speed *= -1;
			  }
			} else {
			    while (positionPct < 0) positionPct += 1;
			    while (positionPct > 1) positionPct -= 1;
			}

			fobsDecoder->setFrame((int)(positionPct * iTotalFrames));

			int curFrameIndex =  fobsDecoder->getFrameIndex();
            bHavePixelsChanged = curFrameIndex != lastFrameIndex;
			if (curFrameIndex != lastFrameIndex){
			    unsigned char *rgb = fobsDecoder->getRGB();
			    memcpy(pixels, rgb, width*height*3);
			    tex.loadData(pixels, width, height, GL_RGB);
			}


			lastFrameIndex = curFrameIndex;


		//--------------------------------------------------------------
		#endif
		//--------------------------------------------------------------


		// ---------------------------------------------------
		// 		on all platforms,
		// 		do "new"ness ever time we idle...
		// 		before "isFrameNew" was clearning,
		// 		people had issues with that...
		// 		and it was badly named so now, newness happens
		// 		per-idle not per isNew call
		// ---------------------------------------------------

		bIsFrameNew = bHavePixelsChanged;
		if (bHavePixelsChanged == true) {
			bHavePixelsChanged = false;
		}
	}

}

//---------------------------------------------------------------------------
void ofVideoPlayer::closeMovie(){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

	if (bLoaded == true){

	    DisposeMovie (moviePtr);
	    #ifdef TARGET_WIN32
			DisposeMovieDrawingCompleteUPP (myDrawCompleteProc);
	    #endif
		moviePtr = NULL;
    }

    //--------------------------------------
	#else
	//--------------------------------------

	 fobsDecoder->close();

	//--------------------------------------
	#endif
    //--------------------------------------

    bLoaded = false;

}

//---------------------------------------------------------------------------
ofVideoPlayer::~ofVideoPlayer(){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------
		closeMovie();
		if(allocated)	delete(pixels);
		if(allocated)	delete(offscreenGWorldPixels);
		if ((offscreenGWorld)) DisposeGWorld((offscreenGWorld));

	 //--------------------------------------
	#else
	//--------------------------------------

		// [CHECK] anything else necessary for FOBS ? please check
		if (fobsDecoder != NULL){
			delete fobsDecoder;
		}

		if (pixels != NULL){
			delete pixels;
		}

	//--------------------------------------
	#endif
	//--------------------------------------

	tex.clear();

}




//--------------------------------------
#ifdef OF_VIDEO_PLAYER_QUICKTIME
//--------------------------------------

void ofVideoPlayer::createImgMemAndGWorld(){
	Rect movieRect;
	movieRect.top 			= 0;
	movieRect.left 			= 0;
	movieRect.bottom 		= height;
	movieRect.right 		= width;
	offscreenGWorldPixels 	= new unsigned char[4 * width * height + 32];
	pixels					= new unsigned char[width*height*3];
	QTNewGWorldFromPtr (&(offscreenGWorld), k32ARGBPixelFormat, &(movieRect), NULL, NULL, 0, (offscreenGWorldPixels), 4 * width);
	LockPixels(GetGWorldPixMap(offscreenGWorld));
	SetGWorld (offscreenGWorld, NULL);
	SetMovieGWorld (moviePtr, offscreenGWorld, nil);
	//------------------------------------ texture stuff:
	if (bUseTexture){
		// create the texture, set the pixels to black and
		// upload them to the texture (so at least we see nothing black the callback)
		tex.allocate(width,height,GL_RGB);
		memset(pixels, 0, width*height*3);
		tex.loadData(pixels, width, height, GL_RGB);
		allocated = true;
	}
}

//--------------------------------------
#endif
//--------------------------------------




//---------------------------------------------------------------------------
bool ofVideoPlayer::loadMovie(string name){


	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

		initializeQuicktime();			// init quicktime
		closeMovie();					// if we have a movie open, close it
		bLoaded 				= false;	// try to load now

		if( name.substr(0, 7) == "http://"){
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
				delete(pixels);
				delete(offscreenGWorldPixels);
				if ((offscreenGWorld)) DisposeGWorld((offscreenGWorld));
				createImgMemAndGWorld();
			}
		} else {
			width	= movieRect.right;
			height 	= movieRect.bottom;
			createImgMemAndGWorld();
		}

		if (moviePtr == NULL){
			return false;
		}

		//----------------- callback method
	    MovieDrawingCompleteUPP myDrawCompleteProc;
	    myDrawCompleteProc = NewMovieDrawingCompleteUPP (DrawCompleteProc);
		SetMovieDrawingCompleteProc (moviePtr, movieDrawingCallWhenChanged,  myDrawCompleteProc, (long)this);

		// ------------- get the total # of frames:
		nFrames				= 0;
		TimeValue			curMovieTime; 
		curMovieTime		= 0;
		TimeValue			duration; 
		OSType whichMediaType	= VIDEO_TYPE; 
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
		convertPixels(offscreenGWorldPixels, pixels, width, height);
		if (bUseTexture == true){
			tex.loadData(pixels, width, height, GL_RGB);
		}

		bStarted 				= false;
		bLoaded 				= true;
		bPlaying 				= false;
		bHavePixelsChanged 		= false;
		speed 					= 1;

		return true;

	//--------------------------------------
	#else
	//--------------------------------------

		bLoaded      		= false;
		bPaused 			= false;
		speed 				= 1.0f;
		bHavePixelsChanged 	= false;
		name 					= ofToDataPath(name);
		fobsDecoder 		= new omnividea::fobs::Decoder(name.c_str());
		omnividea::fobs::ReturnCode error = fobsDecoder->open();

		width 					= fobsDecoder->getWidth();
		height 					= fobsDecoder->getHeight();
		pixels					= new unsigned char[width*height*3];

		if (!fobsDecoder->isVideoPresent()){
			return false;
		}

		bLoaded = true;

		if (bUseTexture){
			// create the texture, set the pixels to black and
			// upload them to the texture (so at least we see nothing black the callback)
			tex.allocate(width,height,GL_RGB);
			memset(pixels, 0, width*height*3);
			tex.loadData(pixels, width, height, GL_RGB);
		}


		error = fobsDecoder->setFrame(0);

		if(error == omnividea::fobs::NoFrameError) {
			error = omnividea::fobs::OkCode;
			printf("NoFrameError\n");
		}

		if(omnividea::fobs::isOk(error)){
			// get some pixels in:
			unsigned char *rgb = fobsDecoder->getRGB();
			if(rgb == NULL) error = omnividea::fobs::GenericError;
			if(isOk(error))
			{
				memcpy(pixels, rgb, width*height*3);
				tex.loadData(pixels, width, height, GL_RGB);
			}
		}


		iTotalFrames		= (int)(fobsDecoder->getFrameRate()*fobsDecoder->getDurationSeconds());
		nFrames				= iTotalFrames;
		positionPct 		= 0;
		timeLastIdle 		= ofGetElapsedTimef();
        durationMillis      = fobsDecoder->getDurationSeconds() * 1000.0f;


		return true;


	//--------------------------------------
	#endif
	//--------------------------------------



}



//--------------------------------------------------------
void ofVideoPlayer::start(){

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
		setLoopState(OF_LOOP_NORMAL);

		// get some pixels in there right away:
		MoviesTask(moviePtr,0);
		convertPixels(offscreenGWorldPixels, pixels, width, height);
		bHavePixelsChanged = true;
		if (bUseTexture == true){
			tex.loadData(pixels, width, height, GL_RGB);
		}

		bStarted = true;
		bPlaying = true;
	}

	//--------------------------------------
	#endif
	//--------------------------------------
}

//--------------------------------------------------------
void ofVideoPlayer::play(){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

	if (!bStarted){
	 	start();
	}else {
		// ------------ lower level "startMovie"
		// ------------ use desired speed & time (-1,1,etc) to Preroll...
		bPlaying = true;
		TimeValue timeNow;
	   	timeNow = GetMovieTime(moviePtr, nil);
		PrerollMovie(moviePtr, timeNow, X2Fix(speed));
		SetMovieRate(moviePtr,  X2Fix(speed));
		MoviesTask(moviePtr, 0);
	}

	//--------------------------------------
	#endif
	//--------------------------------------


}

//--------------------------------------------------------
void ofVideoPlayer::stop(){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

	StopMovie (moviePtr);
	bPlaying = false;
	SetMovieActive (moviePtr, false);
	bStarted = false;

	//--------------------------------------
	#endif
	//--------------------------------------


}

//--------------------------------------------------------
void ofVideoPlayer::setVolume(int volume){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

	SetMovieVolume(moviePtr, volume);

	//--------------------------------------
	#endif
	//--------------------------------------

	// no volume currently for FOBS players :(

}

//--------------------------------------------------------
void ofVideoPlayer::setLoopState(int state){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

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

	//--------------------------------------
	#else
	//--------------------------------------

		loopMode = state;

	//--------------------------------------
	#endif
	//--------------------------------------

}


//---------------------------------------------------------------------------
void ofVideoPlayer::setPosition(float pct){


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
	#else
	//--------------------------------------

        pct = CLAMP(pct, 0,1);
        positionPct = pct;  // check between 0 and 1;

	//--------------------------------------
	#endif
	//--------------------------------------


}

//---------------------------------------------------------------------------
void ofVideoPlayer::setFrame(int frame){
	
#ifndef TARGET_LINUX
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
		TimeValue t = (frame * frameDuration * movieTimeScale); 
		SetMovieTimeValue(moviePtr, t);
		MoviesTask(moviePtr, 0); 
	}
	
   if (!bPaused) SetMovieRate(moviePtr, X2Fix(speed));
#else
   fobsDecoder->setFrame(frame);
#endif
	
}


//---------------------------------------------------------------------------
float ofVideoPlayer::getDuration(){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

		return GetMovieDuration (moviePtr) / (double) GetMovieTimeScale (moviePtr);

	//--------------------------------------
	#else
	//--------------------------------------

		return fobsDecoder->getDurationSeconds();

	//--------------------------------------
	#endif
	//--------------------------------------

}

//---------------------------------------------------------------------------
float ofVideoPlayer::getPosition(){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

		long total 		= GetMovieDuration(moviePtr);
		long current 	= GetMovieTime(moviePtr, nil);
		float pct 		= ((float)current/(float)total);
		return pct;

	//--------------------------------------
	#else
	//--------------------------------------

		return (float) fobsDecoder->getFrameTime() / getDuration();

	//--------------------------------------
	#endif
	//--------------------------------------


}

//---------------------------------------------------------------------------
int ofVideoPlayer::getCurrentFrame(){
	
	int frame = 0;
#ifndef TARGET_LINUX
	
	// zach I think this may fail on variable length frames...
	float pos = getPosition();
	
	
	float  framePosInFloat = (getTotalNumFrames() * getPosition());
	int    framePosInInt = (int)framePosInFloat;
	float  floatRemainder = (framePosInFloat - framePosInInt);
	if (floatRemainder > 0.5f) framePosInInt = framePosInInt + 1;
	//frame = (int)ceil((getTotalNumFrames() * getPosition()));
	frame = framePosInInt;
	
#else
	frame = fobsDecoder->getFrameIndex();
#endif
	return frame;
}

				
//---------------------------------------------------------------------------
bool ofVideoPlayer::getIsMovieDone(){
	
	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------
	
		bool bIsMovieDone = IsMovieDone(moviePtr);
		return bIsMovieDone;
	//--------------------------------------
	#endif
	//--------------------------------------
		
}

//---------------------------------------------------------------------------
void ofVideoPlayer::firstFrame(){
	setFrame(0);
}

//---------------------------------------------------------------------------
void ofVideoPlayer::nextFrame(){
	setFrame(getCurrentFrame() + 1);
}

//---------------------------------------------------------------------------
void ofVideoPlayer::previousFrame(){
	setFrame(getCurrentFrame() - 1);
}



//---------------------------------------------------------------------------
void ofVideoPlayer::setSpeed(float _speed){

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
float ofVideoPlayer::getSpeed(){
	return speed;
}

//---------------------------------------------------------------------------
void ofVideoPlayer::setPaused(bool _bPause){

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

//------------------------------------
void ofVideoPlayer::setUseTexture(bool bUse){
	bUseTexture = bUse;
}

//------------------------------------
void ofVideoPlayer::draw(float _x, float _y, float _w, float _h){
	if (bUseTexture){
		tex.draw(_x, _y, _w, _h);
	}
}

//------------------------------------
void ofVideoPlayer::draw(float _x, float _y){
	draw(_x, _y, width, height);
}



