package cc.openframeworks;

import android.annotation.SuppressLint;
import android.graphics.SurfaceTexture;
import android.graphics.SurfaceTexture.OnFrameAvailableListener;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnPreparedListener;
import android.media.MediaPlayer.OnVideoSizeChangedListener;
import android.util.Log;
import android.view.Surface;


public class OFAndroidVideoPlayer extends OFAndroidObject implements OnFrameAvailableListener {
	
	public OFAndroidVideoPlayer(){
		
		bIsLoaded = false;
		bIsPlaying = false;
		bIsPaused = true;
		bIsFrameNew = false;
		bAutoResume = false;
		
		movieCurrentTime = 0;
		
	}
	
	public static boolean supportsTextureRendering(){
		try {
			Class surfaceTextureClass = Class.forName("android.graphics.SurfaceTexture");
			return true;
		} catch (ClassNotFoundException e) {
			return false;
		}
	}
	
	public void setTextureReference(int texName) {
		textureName = texName;
		setTexture(textureName);
	}
	
	@SuppressLint("NewApi")
	public void setTexture(int texName) {
		surfaceTexture = new SurfaceTexture(texName);
		surfaceTexture.setOnFrameAvailableListener(this);
		surface = new Surface(surfaceTexture);
		mediaPlayer.setSurface(surface);
	}
	
	@SuppressLint("NewApi")
	public void clearTextures() {
		if(surface != null) {
			surface.release();
			surface = null;
		}
		// TODO Clearing surfaceTexture crashes appResume
		// so we have to check if it exists always before accesing its methods
		if(surfaceTexture != null) {
			surfaceTexture.setOnFrameAvailableListener(null);
			surfaceTexture.release();
			surfaceTexture = null;
		}
	}
	
	@SuppressLint("NewApi")
	public boolean update() {
		synchronized(this){
			if(bIsFrameNew) {
				movieCurrentTime = mediaPlayer.getCurrentPosition();
				if(surfaceTexture != null) surfaceTexture.updateTexImage();
				bIsFrameNew = false;
				return true;
			} else {
				return false;
			}
		}
	}
	
	@SuppressLint("NewApi")
	public void getTextureMatrix(float[] mtx) {
		if(surfaceTexture != null) surfaceTexture.getTransformMatrix(mtx);
	}
	
	@SuppressLint("NewApi")
	public void loadMovie(String fileName){
		try {
			if(mediaPlayer == null) {
				mediaPlayer = new MediaPlayer();
				mediaPlayer.setOnPreparedListener(new OnPreparedListener() {
					public void onPrepared(MediaPlayer mp) {
						bIsLoaded = true;
						if(bAutoResume) {
							setTexture(textureName);
							mediaPlayer.seekTo(movieResumeTime);
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
			}
			mediaPlayer.setDataSource(fileName);
			mediaPlayer.prepare();
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
		
		bIsPlaying = true;
		bIsPaused = false;
		
		mediaPlayer.start();
	}
	
	public void stop(){
		if(mediaPlayer==null) return;
		mediaPlayer.stop();
	}
	
	@SuppressLint("NewApi")
	void unloadMovie(){
		if(mediaPlayer!=null){
			mediaPlayer.setSurface(null);
			mediaPlayer.release();
			mediaPlayer = null;
		}
		clearTextures();
		fileName = null;
		bIsLoaded = false;
		bIsPlaying = false;
		bIsPaused = true;
	}
	
	int getWidth(){
		if(mediaPlayer==null) return 0;
		return mediaPlayer.getVideoWidth();
	}
	
	int getHeight(){
		if(mediaPlayer==null) return 0;
		return mediaPlayer.getVideoHeight();
	}
	
	
	
	@Override
	protected void appPause() {
		int currMovieTime = movieCurrentTime;
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
	private int textureName;
	private SurfaceTexture surfaceTexture;
	private Surface surface;
	private String fileName;
	private boolean bIsLoaded;
	private boolean bIsPlaying;
	private boolean bIsPaused;
	private boolean bIsFrameNew;
	
	private boolean bAutoResume;
	private int movieResumeTime;
	
	private int movieCurrentTime;
	
	
	
	
	
	
}