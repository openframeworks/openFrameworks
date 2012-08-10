// Copyright (c) 2012 openFrameworks team
// openFrameworks is released under the MIT License. See libs/_Licence.txt
#pragma once

#include "ofMain.h"

#ifdef __OBJC__
 #import "QTKitMovieRenderer.h"
#endif

//different modes for the video player to run in
//this mode just uses the quicktime texture and is really fast, but offers no pixels-level access
enum ofQTKitDecodeMode {
	OF_QTKIT_DECODE_PIXELS_ONLY = 0, //this mode just renders pixels and can't be drawn directly to the screen
	OF_QTKIT_DECODE_TEXTURE_ONLY, //This is the fastest mode, but doesn't allow any pixel access as the video rendering skips the CPU. You can still have fun with shaders though!
	OF_QTKIT_DECODE_PIXELS_AND_TEXTURE //this mode renders pixels and textures, is a little bit slower than TEXTURE_ONLY, but faster than managing your own ofTexture
};


class ofQTKitPlayer  : public ofBaseVideoPlayer {
	public:

		ofQTKitPlayer();
		virtual ~ofQTKitPlayer();

		bool                loadMovie(string path); //default mode is PIXELS_ONLY
		bool                loadMovie(string path, ofQTKitDecodeMode mode);

		void                closeMovie();
		void                close();

		void                idleMovie();
		void                update();
		void                play();
		void                stop();

		/// deprecated but left for backwards compatability -- use getTexture()->bind() now moving forward
		void                bind();
		void                unbind();

		bool                isFrameNew(); //returns true if the frame has changed in this update cycle

		//gets regular openFrameworks compatible RGBA pixels
		unsigned char * getPixels();
		ofPixelsRef         getPixelsRef();

		//returns an ofTexture will be NULL if OFXQTVIDEOPLAYER_MODE_PIXELS_ONLY
		ofTexture * getTexture();

		float               getPosition();
		float               getPositionInSeconds();
		float               getSpeed();
		bool                getMovieLoopState();
		float               getDuration();
		bool                getIsMovieDone();
		int                 getTotalNumFrames();
		int                 getCurrentFrame();

		void                setPaused(bool bPaused);
		void                setPosition(float pct);
		void                setVolume(int volume);
		void                setBalance(float balance);
		void                setLoopState(bool loops);
		void                setLoopState(int ofLoopState);
		void                setSpeed(float speed);
		void                setFrame(int frame); // frame 0 = first frame...
		void                setPixelFormat(ofPixelFormat pixelFormat);

		ofQTKitDecodeMode   getDecodeMode();

		//Enabling synchronous scrubbing ensures that any call to setFrame, setPosition or jump to position
		//followed by an update will result in the pixels from the desired frame being loaded into the video player
		//Turn off synchronous scrubbing will give a speed boost, but won't guarentuee instantly updating frames
		void                setSynchronousScrubbing(bool synchronous);
		bool                getSynchronousScrubbing();

		void                draw(ofRectangle drawRect);
		void                draw(float x, float y, float w, float h);
		void                draw(float x, float y);

		float               getWidth();
		float               getHeight();

		bool                isPaused();
		bool                isLoaded();
		bool                isPlaying();


		void                firstFrame();
		void                nextFrame();
		void                previousFrame();

	protected:
		bool bNewFrame;
		bool bHavePixelsChanged;
		float duration;
		float speed;
		ofQTKitDecodeMode decodeMode;

		//pulls texture data from the movie renderer into our ofTexture
		void updateTexture();
		void reallocatePixels();
		//do lazy allocation and copy on these so it's faster if they aren't used
		ofTexture tex;
		ofPixels pixels;
		ofPixelFormat pixelFormat;
		//This #ifdef is so you can include this .h file in .cpp files
		//and avoid ugly casts in the .m file
		#ifdef __OBJC__
			QTKitMovieRenderer * moviePlayer;
		#else
			void * moviePlayer;
		#endif

};
