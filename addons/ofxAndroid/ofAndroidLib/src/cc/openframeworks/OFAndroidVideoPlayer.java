package cc.openframeworks;

import android.annotation.TargetApi;
import android.graphics.SurfaceTexture;
import android.graphics.SurfaceTexture.OnFrameAvailableListener;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnPreparedListener;
import android.media.MediaPlayer.OnVideoSizeChangedListener;
import android.media.MediaPlayer.OnCompletionListener;
import android.os.Build;
import android.util.FloatMath;
import android.util.Log;
import android.view.Surface;


@TargetApi(Build.VERSION_CODES.ICE_CREAM_SANDWICH)
public class OFAndroidVideoPlayer extends OFAndroidObject implements OnFrameAvailableListener {
	
	public OFAndroidVideoPlayer(){
		
		bIsLoaded = false;
		bIsPlaying = false;
		bIsPaused = true;
		bIsFrameNew = false;
		bAutoResume = false;
		bIsMoviedone = false;
		bIsLooping = false;
		
		// TODO Get movie FPS to implement Frame methods
		// movieFPS = 16;
		// nFrames = 0;
		
		pan = 0.f;
		volume = leftVolume = rightVolume = 1;
		
	}
	
	public void setTexture(int texName) {
		surfaceTexture = new SurfaceTexture(texName);
		surfaceTexture.setOnFrameAvailableListener(this);
		surface = new Surface(surfaceTexture);
		mediaPlayer.setSurface(surface);
	}
	
	public void clearTextures() {
		if(surface != null) {
			surface.release();
			surface = null;
		}
		if(surfaceTexture != null) {
			surfaceTexture.setOnFrameAvailableListener(null);
			surfaceTexture.release();
			surfaceTexture = null;
		}
	}
	
	public boolean update() {
		synchronized(this){
			if(bIsFrameNew) {
				if(surfaceTexture != null) surfaceTexture.updateTexImage();
				bIsFrameNew = false;
				bIsMoviedone = false;
				return true;
			} else {
				return false;
			}
		}
	}
	
	public void getTextureMatrix(float[] mtx) {
		if(surfaceTexture != null) surfaceTexture.getTransformMatrix(mtx);
	}
	
	public void loadMovie(String fileName){
		try {
			if(mediaPlayer == null) {
				mediaPlayer = new MediaPlayer();
				mediaPlayer.reset();
				mediaPlayer.setLooping(bIsLooping);
				mediaPlayer.setOnPreparedListener(new OnPreparedListener() {
					public void onPrepared(MediaPlayer mp) {
						bIsLoaded = true;
						if(bAutoResume) {
							setPositionMS(movieResumeTime);
							bAutoResume = false;
							play();
						}
					}
				});
				mediaPlayer.setOnVideoSizeChangedListener(new OnVideoSizeChangedListener() {
					public void onVideoSizeChanged(MediaPlayer mp, int width,
							int height) {
						// TODO Auto-generated method stub
					}
				});
				mediaPlayer.setOnCompletionListener(new OnCompletionListener() {
					public void onCompletion(MediaPlayer mp) {
						bIsMoviedone = true;
					}
				});
			} else {
				mediaPlayer.reset();
			}
			mediaPlayer.setDataSource(fileName);
			mediaPlayer.prepare();
			bIsLoaded = true;
			//setVolume(volume);
			this.fileName = fileName;
		} catch (Exception e) {
			Log.e("OF","couldn't load " + fileName,e);
		} 		
	}
	
	public void play(){

		if(mediaPlayer==null) return;
		
		if(!bIsLoaded){
			Log.e("OF","ofxAndroidVideo::play - movie not loaded!");
			return;
		}
		
		mediaPlayer.start();
		
		bIsPlaying = true;
		bIsPaused = false;
		
	}
	
	public void stop(){
		if(mediaPlayer==null) return;
		mediaPlayer.stop();
		bIsPlaying = false;
	}
	
	void unloadMovie(){
		if(mediaPlayer!=null){
			mediaPlayer.setSurface(null);
			mediaPlayer.reset();
			mediaPlayer.release();
			mediaPlayer = null;
		}
		clearTextures();
		
		fileName = null;
		bIsLoaded = false;
		bIsMoviedone = false;
		bIsPlaying = false;
		bIsPaused = true;
		
		//nFrames = 0;
		
	}
	
	void setVolume(float vol){
		volume = vol;
		// calculates left/right volumes from pan-value (constant panning law) 
		// see: Curtis Roads: Computer Music Tutorial p 460
		// thanks to jasch
		float angle = pan * 0.7853981633974483f; // in radians from -45. to +45.
		float cosAngle = (float) Math.cos(angle);
		float sinAngle = (float) Math.sin(angle);
		leftVolume  = (float)((cosAngle - sinAngle) * 0.7071067811865475) * vol; // multiplied by sqrt(2)/2
		rightVolume = (float)((cosAngle + sinAngle) * 0.7071067811865475) * vol; // multiplied by sqrt(2)/2
		if(mediaPlayer!=null)mediaPlayer.setVolume(leftVolume, rightVolume);
	}
	
	float getVolume(){
		return volume;
	}
	
	void setPaused(boolean bP){
		if(mediaPlayer==null) return;
		if(bP) {
			if(bIsPlaying) {
				mediaPlayer.pause();
				bIsPlaying = !bP;
				bIsPaused = bP;
			}
		} else {
			if(bIsPaused) {
				mediaPlayer.start();
				bIsPlaying = !bP;
				bIsPaused = bP;
			}
		}
	}
	
	void setLoopState(boolean bL){
		if(mediaPlayer==null) return;
		mediaPlayer.setLooping(bL);
		bIsLooping = bL;
	}
	
	boolean getLoopState(){
		if(mediaPlayer==null) return false;
		return mediaPlayer.isLooping();
	}
	
	int getWidth(){
		if(mediaPlayer==null) return 0;
		return mediaPlayer.getVideoWidth();
	}
	
	int getHeight(){
		if(mediaPlayer==null) return 0;
		return mediaPlayer.getVideoHeight();
	}
	
	/* Needs movie frameRate to work
	int	getCurrentFrame(){
		
		float  framePosInFloat = ((float)getTotalNumFrames() * (float)getPosition());
		int    framePosInInt = (int)framePosInFloat;
		float  floatRemainder = (framePosInFloat - framePosInInt);
		if (floatRemainder > 0.5) framePosInInt = framePosInInt + 1;

		return framePosInInt;

	}
	*/
	
	/* Needs movie frameRate to work
	int	getTotalNumFrames(){
		return nFrames;
	}
	*/
		
	public boolean isLoaded(){
		return bIsLoaded;
	}
	
	public boolean isPaused(){
		return bIsPaused;
	}
	
	public boolean isPlaying(){
		return bIsPlaying;
	}
	
	boolean isMovieDone(){
		return bIsMoviedone;
	}
	
	
	void setPosition(float pct){
		if(mediaPlayer!=null) mediaPlayer.seekTo((int) (mediaPlayer.getDuration()*pct)); // 0 = start, 1 = end;
	}
	
	void setPositionMS(int ms){
		if(mediaPlayer!=null) mediaPlayer.seekTo(ms); // 0 = start, 1 = end;
	}
	
	float getPosition(){
		if(mediaPlayer!=null)
			return ((float)mediaPlayer.getCurrentPosition())/(float)mediaPlayer.getDuration();
		else
			return 0;
	}
	
	int getPositionMS(){
		if(mediaPlayer!=null)
			return mediaPlayer.getCurrentPosition();
		else
			return 0;
	}
	
	float getDuration(){
		if(mediaPlayer!=null)
			return (float)mediaPlayer.getDuration();
		else
			return 0;
	}
	
	int getDurationMS(){
		if(mediaPlayer!=null)
			return mediaPlayer.getDuration();
		else
			return 0;
	}
	

	
	@Override
	protected void appPause() {
		int currMovieTime = getPositionMS();
		stop();
		String currFileName = fileName;
		boolean currIsLoaded = bIsLoaded; 
		boolean currIsPlaying = bIsPlaying;
		unloadMovie();
		fileName = currFileName;
		bIsLoaded = currIsLoaded;	
		bIsPlaying = currIsPlaying;
		
		bAutoResume = true;
		movieResumeTime = currMovieTime;
	
	}

	@Override
	protected void appResume() {
		if(bIsLoaded){
			loadMovie(fileName);
		}
	}

	@Override
	protected void appStop() {
		appPause();
	}
	
	@Override
	public void onFrameAvailable(SurfaceTexture arg0) {
		synchronized(this){
			bIsFrameNew = true;
		}
	}
	
	
	
	private MediaPlayer mediaPlayer;
	
	private SurfaceTexture surfaceTexture;
	private Surface surface;
	private String fileName;
	private float pan;
	private float volume;
	private float leftVolume, rightVolume;
	private boolean bIsLoaded;
	private boolean bIsPlaying;
	private boolean bIsPaused;
	private boolean bIsMoviedone;
	private boolean bIsFrameNew;
	private boolean bIsLooping;
	
	//private int movieFPS;
	//private int nFrames;
	
	private boolean bAutoResume;
	private int movieResumeTime;
	
	
}
