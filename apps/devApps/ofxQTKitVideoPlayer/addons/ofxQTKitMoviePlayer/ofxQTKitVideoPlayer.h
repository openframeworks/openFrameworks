/*
 *  ofxQTKitMoviePlayer
 *
 **********************************************************
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * ----------------------
 * 
 * ofxQTKitVideoPlayer provides QTKit/CoreVideo accelerated movie playback
 * for openFrameworks on OS X
 * 
 * This class replaces almost all of the functionality of ofVideoPlayer on OS X
 * and uses the modern QTKit and CoreVideo libraries for playback
 *
 * adapted by James George
 * http://www.jamesgeorge.org
 *
 * Special Thanks to Marek Bereza for his initial QTKitVideoTexture
 * http://www.mrkbrz.com/
 *
 * Thanks to Anton Marini for help wrangling buffer contexts
 * http://vade.info/ 
 */

#ifndef OFX_QTKIT_VIDEO_PLAYER
#define OFX_QTKIT_VIDEO_PLAYER

#include "ofMain.h"

#ifdef __OBJC__
#import "QTKitMovieRenderer.h"
#endif

//different modes for the video player to run in
//this mode just uses the quicktime texture and is really fast, but offers no pixels-level access
#define OFXQTVIDEOPLAYER_MODE_TEXTURE_ONLY 0 
//this mode just renders pixels and can't be drawn directly to the screen
#define OFXQTVIDEOPLAYER_MODE_PIXELS_ONLY 1
//this mode renders pixels and textures, is a little bit slower than DRAW_ONLY, but faster than uploading your own texture
#define OFXQTVIDEOPLAYER_MODE_PIXELS_AND_TEXTURE 2

//JG since the ofVideoPlayer creates it's own texture, in order to get the fastest
//performance out of this class it needs to be used on it's own.  Is there a better way to structure this
//so that ofVideoPlayer can use our texture instead?

class ofxQTKitVideoPlayer : public ofBaseVideoPlayer
{
  public:	
	ofxQTKitVideoPlayer();
	virtual ~ofxQTKitVideoPlayer();
	bool				loadMovie(string path); //defaults to pixels only to mimmick base video player expectation
	bool				loadMovie(string path, int modex); //allows opening in certain optimized modes
	
	void 				closeMovie();
	void				clearMemory();
	
	void				idleMovie();

	
	void				play();
	void				stop();
	
	//should use an ofTexture, but this will have to do for now
	void				bind();
	void				unbind();
	
	bool 				isFrameNew(); //returns true if the frame has changed in this update cycle
	
	//gets regular openFrameworks compatible RGBA pixels
	unsigned char * 	getPixels();
	float 				getPosition();
	float				getPositionInSeconds();
	float 				getSpeed();
	float 				getDuration();
	bool				getIsMovieDone();
	int					getTotalNumFrames();
	int					getCurrentFrame();
	bool				getMovieLoops(); //JG: update to of standard loop state
	
	void				setPaused(bool bPause);
	void 				setPosition(float pct);
	void 				setVolume(int volume);
	void 				setLoops(bool bLoops); //JG: update to of standard loop state
	void 				setLoopState(int state);
	void   				setSpeed(float speed);
	void				setFrame(int frame);  // frame 0 = first frame...
	
	void 				draw(float x, float y, float w, float h);
	void 				draw(float x, float y);
	
	float				getWidth();
	float				getHeight();
		
	bool				isPaused();
	bool				isLoaded();
	bool				isPlaying();
	
	void				setType(int imageType); //default is OF_IMAGE_COLOR, supports OF_IMAGE_COLOR_ALPHA

	
  protected:
	
	int					width, height;
	int					nFrames;	
	float				currentSpeed;
	bool				bPaused;
	bool				bPlaying;
	int					type;
	
	//do lazy allocation and copy on these so it's faster if they aren't used
	unsigned char*	moviePixels;
	bool			bNewFrame;
	bool 			bHavePixelsChanged;	
	float			duration;
	
	//This #ifdef is so you can include this .h file in .cpp files
	//and avoid ugly casts in the .m file
	#ifdef __OBJC__
	QTKitMovieRenderer* moviePlayer;
	#else
	void* moviePlayer;
	#endif
	
};

#endif