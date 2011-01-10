#include "ofGStreamerPlayer.h"
#include "ofUtils.h"

#ifdef OF_VIDEO_PLAYER_GSTREAMER
	#include <gst/video/video.h>
#endif

//---------------------------------------------------------------------------
ofGStreamerPlayer::ofGStreamerPlayer (){
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
ofGStreamerPlayer::~ofGStreamerPlayer(){
	closeMovie();
}

//---------------------------------------------------------------------------
unsigned char * ofGStreamerPlayer::getPixels(){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_GSTREAMER
	//--------------------------------------
	
	return gstUtils.getPixels();
	
	//--------------------------------------
	#endif
	//--------------------------------------
}

//---------------------------------------------------------------------------
void ofGStreamerPlayer::idleMovie(){

	if (bLoaded == true){

		//--------------------------------------------------------------
		#ifdef OF_VIDEO_PLAYER_GSTREAMER // gstreamer.
		//--------------------------------------------------------------

			gstUtils.update();
			if(gstUtils.isFrameNew()){
				bHavePixelsChanged = true;
				width = gstUtils.getWidth();
				height = gstUtils.getHeight();
			}

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
bool ofGStreamerPlayer::isFrameNew(){
	return bIsFrameNew;
}


//---------------------------------------------------------------------------
void ofGStreamerPlayer::closeMovie(){

    //--------------------------------------
	#ifdef OF_VIDEO_PLAYER_GSTREAMER
	//--------------------------------------
		gstUtils.close();
	//--------------------------------------
	#endif
    //--------------------------------------

	clearMemory();
    bLoaded = false;

}

//---------------------------------------------------------------------------
bool ofGStreamerPlayer::loadMovie(string name){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_GSTREAMER
	//--------------------------------------

		if(gstUtils.loadMovie(name)){
			height = gstUtils.getHeight();
			width  = gstUtils.getWidth();
			bLoaded = true;
			ofLog(OF_LOG_VERBOSE,"ofGStreamerPlayer: movie loaded");
			return true;
		}else{
			ofLog(OF_LOG_ERROR,"ofGStreamerPlayer couldn't load movie");
			return false;
		}


	//--------------------------------------
	#endif
	//--------------------------------------

}

//--------------------------------------------------------
void ofGStreamerPlayer::play(){
	if( !isLoaded() ){
		ofLog(OF_LOG_ERROR, "ofGStreamerPlayer::play - movie not loaded!");
		return;
	}

	bPlaying = true;
	bPaused = false;
	
	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_GSTREAMER
	//--------------------------------------
		gstUtils.play();
	//--------------------------------------
	#endif
	//--------------------------------------

	//this is if we set the speed first but it only can be set when we are playing.
	setSpeed(speed);

}

//--------------------------------------------------------
void ofGStreamerPlayer::stop(){
	if( !isLoaded() ){
		ofLog(OF_LOG_ERROR, "ofGStreamerPlayer: movie not loaded!");
		return;
	}
	
	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_GSTREAMER
	//--------------------------------------

	gstUtils.setPaused(true);

	//--------------------------------------
	#endif
	//--------------------------------------


	bPlaying = false;
}

//--------------------------------------------------------
void ofGStreamerPlayer::setVolume(int volume){
	if( !isLoaded() ){
		ofLog(OF_LOG_ERROR, "ofGStreamerPlayer: movie not loaded!");
		return;
	}
	
	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_GSTREAMER
	//--------------------------------------

	gstUtils.setVolume(volume);

	//--------------------------------------
	#endif
	//--------------------------------------

}


//--------------------------------------------------------
void ofGStreamerPlayer::setLoopState(int state){
	
	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_GSTREAMER
	//--------------------------------------

		gstUtils.setLoopState(state);

	//--------------------------------------
	#endif
	//--------------------------------------

	//store the current loop state;
	currentLoopState = state;

}


//---------------------------------------------------------------------------
void ofGStreamerPlayer::setPosition(float pct){
	if( !isLoaded() ){
		ofLog(OF_LOG_ERROR, "ofGStreamerPlayer: movie not loaded!");
		return;
	}

 	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_GSTREAMER
	//--------------------------------------

		gstUtils.setPosition(pct);

	//--------------------------------------
	#endif
	//--------------------------------------


}

//---------------------------------------------------------------------------
void ofGStreamerPlayer::setFrame(int frame){
	if( !isLoaded() ){
		ofLog(OF_LOG_ERROR, "ofGStreamerPlayer: movie not loaded!");
		return;
	}

   //--------------------------------------
    #ifdef OF_VIDEO_PLAYER_GSTREAMER
   //--------------------------------------

	   gstUtils.setFrame(frame);


   //--------------------------------------
    #endif
   //--------------------------------------

}


//---------------------------------------------------------------------------
float ofGStreamerPlayer::getDuration(){
	if( !isLoaded() ){
		ofLog(OF_LOG_ERROR, "ofGStreamerPlayer: movie not loaded!");
		return 0.0;
	}
	
	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_GSTREAMER
	//--------------------------------------

		return gstUtils.getDuration();

	//--------------------------------------
	#endif
	//--------------------------------------

}

//---------------------------------------------------------------------------
float ofGStreamerPlayer::getPosition(){
	if( !isLoaded() ){
		ofLog(OF_LOG_ERROR, "ofGStreamerPlayer: movie not loaded!");
		return 0.0;
	}
	
	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_GSTREAMER
	//--------------------------------------

		return gstUtils.getPosition();

	//--------------------------------------
	#endif
	//--------------------------------------


}

//---------------------------------------------------------------------------
int ofGStreamerPlayer::getCurrentFrame(){
	if( !isLoaded() ){
		ofLog(OF_LOG_ERROR, "ofGStreamerPlayer: movie not loaded!");
		return 0;
	}

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_GSTREAMER
	//--------------------------------------

	return gstUtils.getCurrentFrame();

	//--------------------------------------
	#endif
	//--------------------------------------

}


//---------------------------------------------------------------------------
bool ofGStreamerPlayer::getIsMovieDone(){
	if( !isLoaded() ){
		ofLog(OF_LOG_ERROR, "ofGStreamerPlayer: movie not loaded!");
		return false;
	}

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_GSTREAMER
	//--------------------------------------
		
	return gstUtils.getIsMovieDone();
		
	//--------------------------------------
	#endif
	//--------------------------------------

}

//---------------------------------------------------------------------------
void ofGStreamerPlayer::firstFrame(){
	if( !isLoaded() ){
		ofLog(OF_LOG_ERROR, "ofGStreamerPlayer: movie not loaded!");
		return;
	}

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_GSTREAMER
	//--------------------------------------

	gstUtils.firstFrame();

	//--------------------------------------
	#endif
	//--------------------------------------

}

//---------------------------------------------------------------------------
void ofGStreamerPlayer::nextFrame(){
	if( !isLoaded() ){
		ofLog(OF_LOG_ERROR, "ofGStreamerPlayer: movie not loaded!");
		return;
	}

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_GSTREAMER
	//--------------------------------------

	gstUtils.nextFrame();

	//--------------------------------------
	#endif
	//--------------------------------------
}

//---------------------------------------------------------------------------
void ofGStreamerPlayer::previousFrame(){
	if( !isLoaded() ){
		ofLog(OF_LOG_ERROR, "ofGStreamerPlayer: movie not loaded!");
		return;
	}
	
	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_GSTREAMER
	//--------------------------------------
	
	gstUtils.previousFrame();
	
	//--------------------------------------
	#endif
	//--------------------------------------
}



//---------------------------------------------------------------------------
void ofGStreamerPlayer::setSpeed(float _speed){

	speed 				= _speed;

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_GSTREAMER
	//--------------------------------------

		gstUtils.setSpeed(_speed);

	//--------------------------------------
	#endif
	//--------------------------------------
}

//---------------------------------------------------------------------------
void ofGStreamerPlayer::setPaused(bool _bPause){

	bPaused = _bPause;

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_GSTREAMER
	//--------------------------------------

		gstUtils.setPaused(_bPause);

	//--------------------------------------
	#endif
	//--------------------------------------

}


//------------------------------------
int ofGStreamerPlayer::getTotalNumFrames(){
	
	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_GSTREAMER
	//--------------------------------------
	
	return gstUtils.getTotalNumFrames();
	
	//--------------------------------------
	#endif
	//--------------------------------------

}


//---------------------------------------------------------------------------
void ofGStreamerPlayer::clearMemory(){

}

//---------------------------------------------------------------------------
float ofGStreamerPlayer::getSpeed(){
	return speed;
}

//----------------------------------------------------------
float ofGStreamerPlayer::getWidth(){
	return (float)width;
}

//----------------------------------------------------------
float ofGStreamerPlayer::getHeight(){
	return (float)height;
}

//----------------------------------------------------------
bool ofGStreamerPlayer::isPaused(){
	return bPaused;
}

//----------------------------------------------------------
bool ofGStreamerPlayer::isLoaded(){
	return bLoaded;
}

//----------------------------------------------------------
bool ofGStreamerPlayer::isPlaying(){
	return bPlaying;
}

