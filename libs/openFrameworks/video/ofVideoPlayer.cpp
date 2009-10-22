#include "ofVideoPlayer.h"
#include "ofUtils.h"

#ifdef TARGET_LINUX
	#include <gst/video/video.h>
#endif

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
			ofLog(OF_LOG_ERROR,"NativePathNameToFSSpec failed %d", result);
			ofLog(OF_LOG_ERROR,"Error loading movie");
			return false;
		}

	#endif



	#ifdef TARGET_OSX
		FSRef 		fsref;
		result = FSPathMakeRef((const UInt8*)path, &fsref, &isdir);
		if (result) {
			ofLog(OF_LOG_ERROR,"FSPathMakeRef failed %d", result);
			ofLog(OF_LOG_ERROR,"Error loading movie");
			return false;
		}
		result = FSGetCatalogInfo(&fsref, kFSCatInfoNone, NULL, NULL, &theFSSpec, NULL);
		if (result) {
			ofLog(OF_LOG_ERROR,"FSGetCatalogInfo failed %d", result);
			ofLog(OF_LOG_ERROR,"Error loading movie");
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
			ofLog(OF_LOG_ERROR,"NewMovieFromFile failed %d", result);
			return false;
		}
	} else {
		ofLog(OF_LOG_ERROR,"OpenMovieFile failed %d", result);
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
	if ( ( err = MemError()) != noErr){ ofLog(OF_LOG_ERROR,"createMovieFromURL: error creating url handle"); return false;}

	BlockMoveData(url, *urlDataRef, strlen(url) + 1);

	err = NewMovieFromDataRef(&movie, newMovieActive,nil, urlDataRef, URLDataHandlerSubType);
	DisposeHandle(urlDataRef);

	if(err != noErr){
		ofLog(OF_LOG_ERROR,"createMovieFromURL: error loading url");
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

	#if defined(TARGET_OSX) && defined(BIG_ENDIAN)
		convertPixels(ofvp->offscreenGWorldPixels, ofvp->pixels, ofvp->width, ofvp->height);
	#endif

	ofvp->bHavePixelsChanged = true;
	if (ofvp->bUseTexture == true){
		ofvp->tex.loadData(ofvp->pixels, ofvp->width, ofvp->height, GL_RGB);
	}

	return noErr;
}

//--------------------------------------------------------------
#else
//--------------------------------------------------------------

#include "gst/app/gstappsink.h"

static bool plugin_registered = false;
static bool gst_inited = false;
//------------------------------------
void ofGstDataLock(ofGstVideoData * data){
	pthread_mutex_lock( &(data->buffer_mutex) );
}

//------------------------------------
void ofGstDataUnlock(ofGstVideoData * data){
	pthread_mutex_unlock( &(data->buffer_mutex) );
}



// called when the appsink notifies us that there is a new buffer ready for
// processing

static void
on_new_buffer_from_source (GstElement * elt, ofGstVideoData * data)
{
  guint size;
  GstBuffer *buffer;

  //get the buffer from appsink
  buffer = gst_app_sink_pull_buffer (GST_APP_SINK (elt));

  size = GST_BUFFER_SIZE (buffer);
  //ofLog(OF_LOG_VERBOSE,"new buffer of size %d", size);

  ofGstDataLock(data);
	  if(data->pixels){
		  memcpy (data->pixels, GST_BUFFER_DATA (buffer), size);
		  data->bHasPixelsChanged=true;
	  }
  ofGstDataUnlock(data);

  if( !data->nFrames && data->pipelineState==GST_STATE_PLAYING && data->speed==1 ){
	  data->nFrames=data->durationNanos/buffer->duration;
  }

  /// we don't need the appsink buffer anymore
  gst_buffer_unref (buffer);
}

static gboolean
appsink_plugin_init (GstPlugin * plugin)
{
  gst_element_register (plugin, "appsink", GST_RANK_NONE, GST_TYPE_APP_SINK);

  return TRUE;
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

        gstPipeline					= NULL;
		bIsFrameNew					= false;
		loopMode					= OF_LOOP_NONE;

		durationNanos				= 0;
		bIsMovieDone				= false;
		posChangingPaused			= 0;
		isStream					= false;

		gstData.durationNanos		= 0;
		gstData.nFrames				= 0;
		gstData.speed				= speed;
		gstData.bHasPixelsChanged	= false;
		bHavePixelsChanged			= false;

		pthread_mutex_init(&(gstData.buffer_mutex),NULL);
		pthread_mutex_init(&seek_mutex,NULL);

		if(!g_thread_supported()){
			g_thread_init(NULL);
		}
		if(!gst_inited){
			gst_init (NULL, NULL);
			gst_inited=true;
		}
		if(!plugin_registered){
			gst_plugin_register_static(GST_VERSION_MAJOR, GST_VERSION_MINOR,
			 			"appsink", "Element application sink",
			 			appsink_plugin_init, "0.1", "LGPL", "ofVideoPlayer", "openFrameworks",
			 			"http://openframeworks.cc/");
			plugin_registered=true;
		}
	//--------------------------------------------------------------
	#endif
	//--------------------------------------------------------------

}

//---------------------------------------------------------------------------
unsigned char * ofVideoPlayer::getPixels(){
	return pixels;
}

//------------------------------------
//for getting a reference to the texture
ofTexture & ofVideoPlayer::getTextureReference(){
	if(!tex.bAllocated() ){
		ofLog(OF_LOG_WARNING, "ofVideoPlayer - getTextureReference - texture is not allocated");
	}
	return tex;
}


//---------------------------------------------------------------------------
bool ofVideoPlayer::isFrameNew(){

	return bIsFrameNew;

}

//--------------------------------------------------------------------
void ofVideoPlayer::update(){
	idleMovie();
}

//---------------------------------------------------------------------------
void ofVideoPlayer::idleMovie(){


		//--------------------------------------------------------------
		#ifndef  TARGET_LINUX  // !linux = quicktime...
		//--------------------------------------------------------------
		if (bLoaded == true){
			#if defined(TARGET_WIN32) || defined(QT_USE_MOVIETASK)
				MoviesTask(moviePtr,0);
			#endif

		//--------------------------------------------------------------
		#else // linux.
		//--------------------------------------------------------------



		gstHandleMessage();
		if (bLoaded == true){
            ofGstDataLock(&gstData);

				bHavePixelsChanged = gstData.bHasPixelsChanged;
				if (bHavePixelsChanged){
					gstData.bHasPixelsChanged=false;
					bIsMovieDone = false;
					if(bUseTexture)
						tex.loadData(pixels, width, height, GL_RGB);
				}

			ofGstDataUnlock(&gstData);

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

	if(bLoaded){
		gst_element_set_state(GST_ELEMENT(gstPipeline), GST_STATE_NULL);
		//gst_object_unref(gstSink);
		gst_object_unref(gstPipeline);
	}

	//--------------------------------------
	#endif
    //--------------------------------------

    bLoaded = false;

}

//---------------------------------------------------------------------------
void ofVideoPlayer::close(){
	closeMovie();
}

//---------------------------------------------------------------------------
ofVideoPlayer::~ofVideoPlayer(){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------
		closeMovie();
		if(allocated)	delete[] pixels;
		if(allocated)	delete[] offscreenGWorldPixels;
		if ((offscreenGWorld)) DisposeGWorld((offscreenGWorld));

	 //--------------------------------------
	#else
	//--------------------------------------

		closeMovie();

		if (pixels != NULL){
			delete[] pixels;
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

	#if defined(TARGET_OSX) && defined(BIG_ENDIAN)
		QTNewGWorldFromPtr (&(offscreenGWorld), k32ARGBPixelFormat, &(movieRect), NULL, NULL, 0, (offscreenGWorldPixels), 4 * width);
	#else
		QTNewGWorldFromPtr (&(offscreenGWorld), k24RGBPixelFormat, &(movieRect), NULL, NULL, 0, (pixels), 3 * width);
	#endif

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

		#if defined(TARGET_OSX) && defined(BIG_ENDIAN)
			convertPixels(offscreenGWorldPixels, pixels, width, height);
		#endif

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
		bPaused 			= true;
		speed 				= 1.0f;
		bHavePixelsChanged 	= false;
		if( name.find( "://",0 ) == string::npos){
			name 			= "file://"+ofToDataPath(name,true);
			isStream		= false;
		}else{
			isStream		= true;
		}
		ofLog(OF_LOG_VERBOSE,"loading "+name);

		gstData.loop		= g_main_loop_new (NULL, FALSE);



		gstPipeline = gst_element_factory_make("playbin","player");
		g_object_set(G_OBJECT(gstPipeline), "uri", name.c_str(), NULL);

		// create the oF appsink for video rgb without sync to clock
		gstSink = gst_element_factory_make("appsink", NULL);
		GstCaps *caps = gst_caps_new_simple("video/x-raw-rgb", NULL);
		gst_app_sink_set_caps(GST_APP_SINK(gstSink), caps);
		gst_caps_unref(caps);
		gst_base_sink_set_sync(GST_BASE_SINK(gstSink), false);

		g_object_set (G_OBJECT(gstPipeline),"video-sink",gstSink,NULL);


		GstElement *audioSink = gst_element_factory_make("gconfaudiosink", NULL);
		g_object_set (G_OBJECT(gstPipeline),"audio-sink",audioSink,NULL);


		// pause the pipeline
		if(gst_element_set_state(GST_ELEMENT(gstPipeline), GST_STATE_PAUSED) ==
		   GST_STATE_CHANGE_FAILURE) {
			ofLog(OF_LOG_ERROR, "GStreamer: unable to set pipeline to paused\n");
			gst_object_unref(gstPipeline);
			return false;
		}

		gstData.pipeline=gstPipeline;

		// set the appsink to emit signals to get eos and errors
		g_object_set (G_OBJECT (gstSink), "emit-signals", TRUE, "sync", TRUE, NULL);
		g_signal_connect (gstSink, "new-buffer", G_CALLBACK (on_new_buffer_from_source), &gstData);

		if(!isStream){
			return allocate();
		}


		// unreference all elements so they get deleted on close
		gst_object_unref(audioSink);

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
		#if defined(TARGET_OSX) && defined(BIG_ENDIAN)
			convertPixels(offscreenGWorldPixels, pixels, width, height);
		#endif
		bHavePixelsChanged = true;
		if (bUseTexture == true){
			tex.loadData(pixels, width, height, GL_RGB);
		}

		bStarted = true;
		bPlaying = true;
	}

	//--------------------------------------
	#else
	//--------------------------------------

		bHavePixelsChanged = true;
		bStarted = true;
		bPlaying = true;
		setPaused(false);

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
	#else
	//--------------------------------------

		if (!bStarted){
		 	start();
		}else {
			bPlaying = true;
			setPaused(false);
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
	SetMovieActive (moviePtr, false);
	bStarted = false;

	//--------------------------------------
	#else
	//--------------------------------------

	setPaused(true);

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
	#else
	//--------------------------------------

	gdouble gvolume = CLAMP(volume,0,10);
	g_object_set(G_OBJECT(gstPipeline), "volume", gvolume, NULL);

	//--------------------------------------
	#endif
	//--------------------------------------

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

	//pct = CLAMP(pct, 0,1);// check between 0 and 1;
	GstFormat format = GST_FORMAT_TIME;
	GstSeekFlags flags = (GstSeekFlags) (GST_SEEK_FLAG_FLUSH|GST_SEEK_FLAG_ACCURATE);
	gint64 pos = (guint64)((double)pct*(double)durationNanos);
	if(bPaused){
	    seek_lock();
		gst_element_set_state (gstPipeline, GST_STATE_PLAYING);
		posChangingPaused=true;
		seek_unlock();
	}
	if(speed>0){
		if(!gst_element_seek(GST_ELEMENT(gstPipeline),speed, 	format,
				flags,
				GST_SEEK_TYPE_SET,
				pos,
				GST_SEEK_TYPE_SET,
				-1)) {
		ofLog(OF_LOG_WARNING,"GStreamer: unable to change speed");
		}
	}else{
		if(!gst_element_seek(GST_ELEMENT(gstPipeline),speed, 	format,
				flags,
				GST_SEEK_TYPE_SET,
				0,
				GST_SEEK_TYPE_SET,
				pos)) {
		ofLog(OF_LOG_WARNING,"GStreamer: unable to change speed");
		}
	}

	//--------------------------------------
	#endif
	//--------------------------------------


}

//---------------------------------------------------------------------------
void ofVideoPlayer::setFrame(int frame){

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
    #else
   //--------------------------------------

	   float pct = (float)frame / (float)gstData.nFrames;
	   setPosition(pct);


   //--------------------------------------
    #endif
   //--------------------------------------

}


//---------------------------------------------------------------------------
float ofVideoPlayer::getDuration(){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

		return (float) (GetMovieDuration (moviePtr) / (double) GetMovieTimeScale (moviePtr));

	//--------------------------------------
	#else
	//--------------------------------------

		return (float)durationNanos/(float)GST_SECOND;

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
		gint64 pos=0;
		GstFormat format=GST_FORMAT_TIME;
		if(!gst_element_query_position(GST_ELEMENT(gstPipeline),&format,&pos))
			ofLog(OF_LOG_ERROR,"GStreamer: cannot query position");
		return (float)pos/(float)durationNanos;

	//--------------------------------------
	#endif
	//--------------------------------------


}

//---------------------------------------------------------------------------
int ofVideoPlayer::getCurrentFrame(){

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

}


//---------------------------------------------------------------------------
bool ofVideoPlayer::getIsMovieDone(){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------
		bool bIsMovieDone = (bool)IsMovieDone(moviePtr);
		return bIsMovieDone;
	//--------------------------------------
	#else
	//--------------------------------------
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
	#else
	//--------------------------------------

		GstFormat format = GST_FORMAT_TIME;
		GstSeekFlags flags = (GstSeekFlags) (GST_SEEK_FLAG_FLUSH |GST_SEEK_FLAG_ACCURATE);
		gint64 pos;

		if(speed==0){
			gst_element_set_state (gstPipeline, GST_STATE_PAUSED);
			return;
		}

		if(!gst_element_query_position(GST_ELEMENT(gstPipeline),&format,&pos))
			ofLog(OF_LOG_ERROR,"GStreamer: cannot query position");

		if(!bPaused)
			gst_element_set_state (gstPipeline, GST_STATE_PLAYING);

		if(speed>0){
			if(!gst_element_seek(GST_ELEMENT(gstPipeline),speed, 	format,
					flags,
					GST_SEEK_TYPE_SET,
					pos,
					GST_SEEK_TYPE_SET,
					-1)) {
			ofLog(OF_LOG_WARNING,"GStreamer: unable to change speed");
			}
		}else{
			if(!gst_element_seek(GST_ELEMENT(gstPipeline),speed, 	format,
					flags,
					GST_SEEK_TYPE_SET,
					0,
					GST_SEEK_TYPE_SET,
					pos)) {
			ofLog(OF_LOG_WARNING,"GStreamer: unable to change speed");
			}
		}

		ofLog(OF_LOG_VERBOSE,"Gstreamer: speed change to %f", speed);

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
	#else
	//--------------------------------------

		//timeLastIdle = ofGetElapsedTimeMillis();
		if(bLoaded){
			if(bPaused)
				gst_element_set_state (gstPipeline, GST_STATE_PAUSED);
			else
				gst_element_set_state (gstPipeline, GST_STATE_PLAYING);
		}

	//--------------------------------------
	#endif
	//--------------------------------------

}

//------------------------------------
void ofVideoPlayer::setUseTexture(bool bUse){
	bUseTexture = bUse;
}

//we could cap these values - but it might be more useful
//to be able to set anchor points outside the image

//----------------------------------------------------------
void ofVideoPlayer::setAnchorPercent(float xPct, float yPct){
    if (bUseTexture)tex.setAnchorPercent(xPct, yPct);
}

//----------------------------------------------------------
void ofVideoPlayer::setAnchorPoint(int x, int y){
    if (bUseTexture)tex.setAnchorPoint(x, y);
}

//----------------------------------------------------------
void ofVideoPlayer::resetAnchor(){
   	if (bUseTexture)tex.resetAnchor();
}

//------------------------------------
void ofVideoPlayer::draw(float _x, float _y, float _w, float _h){
	if (bUseTexture){
		tex.draw(_x, _y, _w, _h);
	}
}

//------------------------------------
void ofVideoPlayer::draw(float _x, float _y){
	draw(_x, _y, (float)width, (float)height);
}

//------------------------------------
int ofVideoPlayer::getTotalNumFrames(){
	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------
	return nFrames;
	//--------------------------------------
	#else
	//--------------------------------------
	return gstData.nFrames;
	//--------------------------------------
	#endif
	//--------------------------------------

}

//----------------------------------------------------------
float ofVideoPlayer::getHeight(){
	return (float)height;
}

//----------------------------------------------------------
float ofVideoPlayer::getWidth(){
	return (float)width;
}


//--------------------------------------
#ifdef OF_VIDEO_PLAYER_GSTREAMER
//--------------------------------------

//----------------------------------------------------------
bool ofVideoPlayer::allocate(){
	// wait for paused state to query the duration
	GstState state = GST_STATE_PAUSED;
	gst_element_get_state(gstPipeline,&state,NULL,2*GST_SECOND);
	GstFormat format=GST_FORMAT_TIME;
	if(!gst_element_query_duration(gstPipeline,&format,&durationNanos))
		ofLog(OF_LOG_WARNING,"GStreamer: cannot query time duration");

	gstData.durationNanos = durationNanos;
	gstData.nFrames		  = 0;




	// query width, height, fps and do data allocation
	if (GstPad* pad = gst_element_get_static_pad(gstSink, "sink")) {
		if(gst_video_get_size(GST_PAD(pad), &width, &height) && bUseTexture){
			pixels=new unsigned char[width*height*3];
			gstData.pixels=pixels;
			memset(pixels,0,width*height*3);
			tex.allocate(width,height,GL_RGB);
			tex.loadData(pixels,width,height,GL_RGB);
			allocated = true;
		}else{
			ofLog(OF_LOG_ERROR,"GStreamer: cannot query width and height");
			return false;
		}

		/*GstCaps * caps = gst_pad_get_caps(pad);
		if(caps){
			int fps_n;
			int fps_d;

			if(gst_video_parse_caps_framerate (caps,&fps_n,&fps_d))
				ofLog(OF_LOG_VERBOSE,"fps_n:%d fps_d:%d",fps_n,fps_d);
			else
				ofLog(OF_LOG_WARNING,"Gstreamer: cannot get framerate, frame seek won't work");
		}else{
			ofLog(OF_LOG_WARNING,"Gstreamer: cannot get pad caps, frame seek won't work");
		}*/
        gst_object_unref(GST_OBJECT(pad));
    }else{
		ofLog(OF_LOG_ERROR,"GStreamer: cannot get sink pad");
		return false;
	}
	bLoaded = true;
	return bLoaded;
}


//----------------------------------------------------------
void ofVideoPlayer::gstHandleMessage()
{

	GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(gstPipeline));
	while(gst_bus_have_pending(bus)) {
		GstMessage* msg = gst_bus_pop(bus);

		ofLog(OF_LOG_VERBOSE,"GStreamer: Got %s message", GST_MESSAGE_TYPE_NAME(msg));

		switch (GST_MESSAGE_TYPE (msg)) {

			case GST_MESSAGE_BUFFERING:
				gint pctBuffered;
				gst_message_parse_buffering(msg,&pctBuffered);
				ofLog(OF_LOG_VERBOSE,"GStreamer: buffering %i\%", pctBuffered);
				if(isStream && !bLoaded){
					allocate();
				}
				if(pctBuffered<100){
					gst_element_set_state (gstPipeline, GST_STATE_PAUSED);
				}else if(!bPaused){
					gst_element_set_state (gstPipeline, GST_STATE_PLAYING);
				}
			break;

			case GST_MESSAGE_DURATION:{
				GstFormat format=GST_FORMAT_TIME;
				if(!gst_element_query_duration(gstPipeline,&format,&durationNanos))
					ofLog(OF_LOG_WARNING,"GStreamer: cannot query duration");
			}break;

			case GST_MESSAGE_STATE_CHANGED:
                GstState oldstate, newstate, pendstate;
				gst_message_parse_state_changed(msg, &oldstate, &newstate, &pendstate);
				gstData.pipelineState=newstate;
				seek_lock();
				if(posChangingPaused && newstate==GST_STATE_PLAYING){
                    gst_element_set_state (gstPipeline, GST_STATE_PAUSED);
                    posChangingPaused=false;
				}
				seek_unlock();


				ofLog(OF_LOG_VERBOSE,"GStreamer: state changed from %d to %d (%d)", oldstate, newstate, pendstate);
			break;

			case GST_MESSAGE_ASYNC_DONE:
				gstData.speed=speed;
				ofLog(OF_LOG_VERBOSE,"GStreamer: async done");
			break;

			case GST_MESSAGE_ERROR: {
				GError *err;
				gchar *debug;
				gst_message_parse_error(msg, &err, &debug);

				ofLog(OF_LOG_ERROR, "GStreamer Plugin: Embedded video playback halted; module %s reported: %s",
					  gst_element_get_name(GST_MESSAGE_SRC (msg)), err->message);

				g_error_free(err);
				g_free(debug);

				gst_element_set_state(GST_ELEMENT(gstPipeline), GST_STATE_NULL);

			}break;

			case GST_MESSAGE_EOS:
				ofLog(OF_LOG_VERBOSE,"GStreamer: end of the stream.");
				bIsMovieDone = true;

				switch(loopMode){

					case OF_LOOP_NORMAL:{
						GstFormat format = GST_FORMAT_TIME;
						GstSeekFlags flags = (GstSeekFlags) (GST_SEEK_FLAG_FLUSH |GST_SEEK_FLAG_KEY_UNIT);
						gint64 pos;
						gst_element_query_position(GST_ELEMENT(gstPipeline),&format,&pos);

						float loopSpeed;
						if(pos>0)
							loopSpeed=-speed;
						else
							loopSpeed=speed;
						if(!gst_element_seek(GST_ELEMENT(gstPipeline),
											speed,
											format,
											flags,
											GST_SEEK_TYPE_SET,
											0,
											GST_SEEK_TYPE_SET,
											durationNanos)) {
							ofLog(OF_LOG_WARNING,"GStreamer: unable to seek");
						}
					}break;

					case OF_LOOP_PALINDROME:{
						GstFormat format = GST_FORMAT_TIME;
						GstSeekFlags flags = (GstSeekFlags) (GST_SEEK_FLAG_FLUSH |GST_SEEK_FLAG_KEY_UNIT);
						gint64 pos;
						gst_element_query_position(GST_ELEMENT(gstPipeline),&format,&pos);
						float loopSpeed;
						if(pos>0)
							loopSpeed=-speed;
						else
							loopSpeed=speed;
						if(!gst_element_seek(GST_ELEMENT(gstPipeline),
											loopSpeed,
											GST_FORMAT_UNDEFINED,
											flags,
											GST_SEEK_TYPE_NONE,
											0,
											GST_SEEK_TYPE_NONE,
											0)) {
							ofLog(OF_LOG_WARNING,"GStreamer: unable to seek");
						}
					}break;
				}

			break;

			default:
				ofLog(OF_LOG_VERBOSE,"GStreamer: unhandled message");
			break;
		}
		//gst_message_unref(msg);
	}

	gst_object_unref(GST_OBJECT(bus));
}


void ofVideoPlayer::seek_lock(){
    pthread_mutex_lock( &seek_mutex );
}
void ofVideoPlayer::seek_unlock(){
    pthread_mutex_unlock( &seek_mutex );
}
//--------------------------------------
#endif
//--------------------------------------
