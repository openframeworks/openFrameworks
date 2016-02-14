//DirectShowVideo and ofDirectShowPlayer written by Theodore Watson, Jan 2014
//See the cpp file for the DirectShow implementation 
//To allow for QuickTime video playback install the K-Lite Mega Codec Pack 10.2

#pragma once 
#include "ofMain.h"

class DirectShowVideo; 

class ofDirectShowPlayer : public ofBaseVideoPlayer{
    public:
		ofDirectShowPlayer();
		ofDirectShowPlayer(const ofDirectShowPlayer&) = delete;
		ofDirectShowPlayer & operator=(const ofDirectShowPlayer&) = delete;
		ofDirectShowPlayer(ofDirectShowPlayer &&);
		ofDirectShowPlayer & operator=(ofDirectShowPlayer&&);

        bool                load(string path);
        void                update();

        void                close();
    
        void                play();
        void                stop();     
    
        bool                isFrameNew() const;

        const ofPixels &    getPixels() const;
        ofPixels &          getPixels();
    
        float               getWidth() const;
        float               getHeight() const;
    
        bool                isPaused() const;
        bool                isLoaded() const;
        bool                isPlaying() const;
    
        bool                setPixelFormat(ofPixelFormat pixelFormat);
        ofPixelFormat       getPixelFormat() const;

        float               getPosition() const;
        float               getSpeed() const;
        float               getDuration() const;
        bool                getIsMovieDone() const;
    
        void                setPaused(bool bPause);
        void                setPosition(float pct);
        void                setVolume(float volume); // 0..1
        void                setLoopState(ofLoopType state);
        void                setSpeed(float speed);
        void                setFrame(int frame);  // frame 0 = first frame...
    
        int                 getCurrentFrame() const;
        int                 getTotalNumFrames() const;
        ofLoopType          getLoopState() const;
    
        void                firstFrame();
        void                nextFrame();
        void                previousFrame();

    protected:
        std::shared_ptr<DirectShowVideo>   player;
		ofPixelFormat pixelFormat = OF_PIXELS_RGB;
};