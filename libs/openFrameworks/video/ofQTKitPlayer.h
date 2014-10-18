// Copyright (c) 2012 openFrameworks team
// openFrameworks is released under the MIT License. See libs/_Licence.txt

#pragma once

#ifdef OF_VIDEO_PLAYER_QTKIT

#include "ofMain.h"

#ifdef __OBJC__
 #import "ofQTKitMovieRenderer.h"
#endif

// ofQTKitDecodeMode allows for highly efficient decoding of video
// pixel data.  Options are available for direct-to-screen/texture rendering
// as well as more traditional CPU/RAM pixel-based rendering.

enum ofQTKitDecodeMode {
	OF_QTKIT_DECODE_PIXELS_ONLY = 0,
    // OF_QTKIT_DECODE_PIXELS_ONLY: This mode decodes pixels only and can't
    // be drawn directly to the screen without an an additional step.
    
	OF_QTKIT_DECODE_TEXTURE_ONLY,
    // OF_QTKIT_DECODE_TEXTURE_ONLY: This is the fastest mode, but doesn't
    // allow direct pixel access because video rendering skips the CPU and RAM.
    // Pixels ARE available to texture / fragment shaders though.
    
	OF_QTKIT_DECODE_PIXELS_AND_TEXTURE
    // OF_QTKIT_DECODE_PIXELS_AND_TEXTURE: This mode combines both
    // OF_QTKIT_DECODE_PIXELS_ONLY AND OF_QTKIT_DECODE_TEXTURE_ONLY modes.
    // It is slightly slower OF_QTKIT_DECODE_TEXTURE_ONLY, but faster than
    // managing your own ofTexture.
};


class ofQTKitPlayer  : public ofBaseVideoPlayer {
	public:

		ofQTKitPlayer();
		virtual ~ofQTKitPlayer();

		bool                load(string path); //default mode is PIXELS_ONLY
		bool                load(string path, ofQTKitDecodeMode mode);

		void                closeMovie();
		void                close();

		void                idleMovie();
		void                update();
		void                play();
		void                stop();

		bool                isFrameNew() const; //returns true if the frame has changed in this update cycle

		// Returns openFrameworks compatible RGBA pixels.
        // Be aware of your current render mode.
    
        ofPixels&     getPixels();
        const ofPixels& getPixels() const;


		// Returns openFrameworks compatible ofTexture pointer.
        // if decodeMode == OF_QTKIT_DECODE_PIXELS_ONLY,
        // the returned pointer will be NULL.
		ofTexture * getTexturePtr();

		float               getPosition() const;
		float               getPositionInSeconds() const;
		float               getSpeed() const;
		ofLoopType          getLoopState() const;
		float               getDuration() const;
		bool                getIsMovieDone() const;
		int                 getTotalNumFrames() const;
		int                 getCurrentFrame() const;

		void                setPaused(bool bPaused);
		void                setPosition(float pct);
		void                setVolume(float volume);
		void                setBalance(float balance);
		void                setLoopState(ofLoopType state);
		void                setSpeed(float speed);
		void                setFrame(int frame); // frame 0 = first frame...
		
        // ofQTKitPlayer only supports OF_PIXELS_RGB and OF_PIXELS_RGBA.
		bool                setPixelFormat(ofPixelFormat pixelFormat);
		ofPixelFormat       getPixelFormat() const;
		
		ofQTKitDecodeMode   getDecodeMode() const;

        // Enabling synchronous seeing ensures that any call to
        // setFrame(), setPosition() or jump(), will result in pixels
        // from the desired frame are loaded correctly.
    
        // Disabling synchronous seeking will result in a speed boost
        // during playback, but won't guarantee that frames are available
        // when seeking frames manually (i.e. asynchronously).
    
        void                setSynchronousSeeking(bool synchronous);
		bool                getSynchronousSeeking() const;

		void                draw(float x, float y, float w, float h);
		void                draw(float x, float y);

		float               getWidth() const;
		float               getHeight() const;

		bool                isPaused() const;
		bool                isLoaded() const;
		bool                isPlaying() const;


		void                firstFrame();
		void                nextFrame();
		void                previousFrame();

	protected:
    
        ofLoopType currentLoopState;

        bool bPaused;
        bool bNewFrame;
		bool bHavePixelsChanged;
		
        float duration;
		float speed;
		
        ofQTKitDecodeMode decodeMode;
	    
        string moviePath;
		
        bool bSynchronousSeek;
		
        // updateTexture() pulls texture data from the movie QTKit
        // renderer into our internal ofTexture.
		void updateTexture();
		void reallocatePixels();
		
		ofTexture tex;
		
        ofPixels pixels;
		ofPixelFormat pixelFormat;
		
        // This #ifdef is so you can include this .h file in .cpp files
		// and avoid ugly casts in the .m file
		#ifdef __OBJC__
			QTKitMovieRenderer * moviePlayer;
		#else
			void * moviePlayer;
		#endif

};

#endif
