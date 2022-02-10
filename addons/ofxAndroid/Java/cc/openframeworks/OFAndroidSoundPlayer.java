package cc.openframeworks;

import static cc.openframeworks.OFAndroid.runOnMainThread;

import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.media.AudioAttributes;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.os.Build;
import android.provider.Settings;
import android.util.FloatMath;
import android.util.Log;
import androidx.annotation.Keep;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.Timer;
import java.util.TimerTask;

@Keep
public class OFAndroidSoundPlayer extends OFAndroidObject implements MediaPlayer.OnPreparedListener, MediaPlayer.OnErrorListener{

	final Boolean LOG_SOUND = false;

	OFAndroidSoundPlayer(){
		pan = 0.f;
		volume = leftVolume = rightVolume = 1;
		bIsLoaded = false;
		bIsPlaying = false;
		loop = false;
		speed = 1.0f;
		soundID = -1;
		streamID = -1;
		multiPlay = false;
		isPaused = false;
		bIsPrepared = false;
		fileName = null;
		contentType = AudioAttributes.CONTENT_TYPE_MUSIC;
		stream=true;
		pausePositionMS=0;
		bWasResume=false;
	}

	public void onDestroy() {
		stop();
		unloadSound();
		if (player != null) {
			if(player.isPlaying())
				player.stop();
			player.release(); }
		if(pool != null) pool.release(); pool = null;
		if(attributes != null) attributes = null;
	}

	protected void setContentType(int contentType){
		if(loop) contentType = AudioAttributes.CONTENT_TYPE_MUSIC;
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
			attributes = new AudioAttributes.Builder()
			   .setUsage(AudioAttributes.USAGE_GAME)
			   .setContentType(contentType)
			   .build();
		}

	}

	protected void createSoundPool(){
		if(pool == null) {
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
				pool = new SoundPool.Builder()
						.setAudioAttributes(attributes)
						.setMaxStreams(128)
						.build();
			}
		}
	}

	private AssetFileDescriptor getSeekableFileDescriptor(String fileName) {

		AssetManager assetManager = OFAndroid.assetManager;
		AssetFileDescriptor assetFileDescriptor = null;
		try {
			assetFileDescriptor = assetManager.openFd(fileName);
			return assetFileDescriptor;
		}
		catch (IOException e) {
			Log.d("OF", e.toString());
		}
		return assetFileDescriptor;
	}
	
	void loadSound(String fileName, boolean stream){

			setContentType(AudioAttributes.CONTENT_TYPE_SONIFICATION);
			if(stream){
				try {
					if(player!=null) {
						unloadSound();
					}
					bIsPrepared = false;
					player = new MediaPlayer();
					player.setOnErrorListener(this);
					player.setOnPreparedListener(this);
//					player.setAudioStreamType(AudioManager.STREAM_MUSIC);
					player.setAudioAttributes(attributes);

					assetManagerFileDescriptor = getSeekableFileDescriptor(fileName);
					bIsLoaded = false;
					if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.N && assetManagerFileDescriptor != null) {
						try {
							player.setDataSource(assetManagerFileDescriptor);
							bIsLoaded = true;
						} catch (Exception ex) {
							Log.e("OF","SoundPlayer Exception:" + fileName, ex);
						}
					}
//					if(bIsLoaded == false && Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP && assetManagerFileDescriptor != null) {
//						try {
//
//							FileInputStream in = new FileInputStream(assetManagerFileDescriptor.getFileDescriptor());
//							player.FileDescriptor(String.valueOf(in));
//							bIsLoaded = true;
//						} catch (Exception ex) {
//							Log.e("OF","SoundPlayer Exception:" + fileName, ex);
//						}
//					}
					if(!bIsLoaded) {
						try {
							player.setDataSource(OFAndroid.toDataPath(fileName));
						} catch (Exception ex) {
							Log.e("OF","SoundPlayer Exception:" + fileName, ex);
							return;
						}
					}

				} catch (Exception e) {
					Log.e("OF","SoundPlayer Exception: couldn't load " + fileName,e);
					return;
				}
				try {
					if (player != null) player.prepareAsync();
				}
				catch (Exception e) {
						Log.e("OF","SoundPlayer Exception: couldn't prepare " + fileName,e);
					return;
				}
			}else{
				try {
					createSoundPool();
					if(soundID!=-1) unloadSound();
					soundID = pool.load(fileName, 1);
				}
				catch (Exception e) {
					Log.e("OF","failed to create soundpool could not load " + fileName,e);
					return;
				}
			}
			setVolume(volume);
			bIsLoaded = true;
			this.fileName = fileName;
			this.stream = stream;

	}



	void unloadSound(){
		if(player!=null){
			try {

				if(bIsPlaying && bIsPrepared && player.isPlaying())
					player.pause();
			} catch (Exception e) {
				e.printStackTrace();
			}
			try {
				player.reset();
			} catch (Exception e) {
				e.printStackTrace();
			}
			try {
				player.release();
			} catch (Exception e) {
				e.printStackTrace();
			}

			player = null;
		}

		if(soundID!=-1){
			if(pool != null) pool.unload(soundID);
		}
		fileName = null;
		soundID = -1;
		streamID = -1;
		bIsLoaded = false;
		bIsPlaying = false;
	}
	
	void play(){
		if(stream){
			if(player==null) {
				Log.e("OF", "SoundPlayer play() player is null");
				return;
			}
			if(!bIsPrepared) {
				Log.e("OF", "SoundPlayer play() player is not prepared yet");
				return;
			}
			if(getIsPlaying() && getPosition() != 0) setPosition(0);
			try {
				player.start();
			} catch (IllegalStateException e) {
				e.printStackTrace();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}else{
			if(pool == null) return;
			if(!multiPlay){
				pool.stop(streamID);
			}
			streamID = pool.play(soundID,leftVolume,rightVolume,1,loop?-1:0,speed);
			bIsPlaying = true;
		}
	}
	private void startFadeIn(){
		final int FADE_DURATION = 500; //The duration of the fade
		//The amount of time between volume changes. The smaller this is, the smoother the fade
		final int FADE_INTERVAL = 250;
		final int MAX_VOLUME = 1; //The volume will increase from 0 to 1
		int numberOfSteps = FADE_DURATION/FADE_INTERVAL; //Calculate the number of fade steps
		//Calculate by how much the volume changes each step
		final float deltaVolume = MAX_VOLUME / (float)numberOfSteps;

		//Create a new Timer and Timer task to run the fading outside the main UI thread
		final Timer timer = new Timer(true);
		TimerTask timerTask = new TimerTask() {
			@Override
			public void run() {
				fadeInStep(deltaVolume); //Do a fade step
				//Cancel and Purge the Timer if the desired volume has been reached
				if(volume>=1f){
					timer.cancel();
					timer.purge();
				}
			}
		};
		timer.schedule(timerTask,FADE_INTERVAL,FADE_INTERVAL);
	}

	private void startFadeOut(){
		final int FADE_DURATION = 500; //The duration of the fade
		//The amount of time between volume changes. The smaller this is, the smoother the fade
		final int FADE_INTERVAL = 250;
		final float MAX_VOLUME = volume; //The volume will increase from 0 to 1
		int numberOfSteps = FADE_DURATION/FADE_INTERVAL; //Calculate the number of fade steps
		//Calculate by how much the volume changes each step
		final float deltaVolume = MAX_VOLUME / (float)numberOfSteps;

		//Create a new Timer and Timer task to run the fading outside the main UI thread
		final Timer timer = new Timer(true);
		TimerTask timerTask = new TimerTask() {
			@Override
			public void run() {
				fadeOutStep(deltaVolume); //Do a fade step
				//Cancel and Purge the Timer if the desired volume has been reached
				if(volume<=0.0f){
					timer.cancel();
					timer.purge();
					if (bIsLoaded && player.isPlaying()) {
						player.pause();
						player.seekTo(0);
					}
				}
			}
		};

		timer.schedule(timerTask,FADE_INTERVAL,FADE_INTERVAL);
	}

	private void fadeInStep(float deltaVolume){
		if (bIsLoaded && player.isPlaying()) {
			player.setVolume(volume, volume);
			volume += deltaVolume;
		}
	}

	private void fadeOutStep(float deltaVolume){
		if (bIsLoaded && player.isPlaying()) {
			player.setVolume(volume, volume);
			volume -= deltaVolume;
		}
	}



	void fadeOut(){
		if(stream){

			final long steps = 30;
			final double stepWidth = (double) 1 / steps;
			double mFadeOutCriteria = 1;
			if(player==null) return;
			try {
				if (bIsPlaying && bIsPrepared) {
					Log.i("OF", "SoundPlayer stop - pause() isPlaying");
//					player.pause();
//					player.seekTo(0);
					startFadeOut();
				} else {
					try {
//						if (bIsLoaded && player.isPlaying())
//							player.pause();
//						player.seekTo(0);
						startFadeOut();
					} catch(Exception ex) {
						Log.e("OF", "SoundPlayer stop - pause() !isPlaying excpetion:" + ex.getMessage());
					}
				}
			} catch (Exception e) {
				e.printStackTrace();
				return;
			}
		}else if(streamID!=-1){
			if(pool == null) return;
			pool.stop(streamID);
			bIsPlaying = false;
		}
	}

	void stop(){
		if(stream){
			if(player==null) return;
			try {
				if (bIsPlaying && bIsPrepared) {
					Log.i("OF", "SoundPlayer stop - pause() isPlaying");
					player.pause();
					player.seekTo(0);
				} else {
					try {
						if (bIsLoaded && bIsPrepared && player.isPlaying())
							player.pause();
						player.seekTo(0);
					} catch(Exception ex) {
						Log.e("OF", "SoundPlayer stop - pause() !isPlaying excpetion:" + ex.getMessage());
					}
				}
			} catch (Exception e) {
				e.printStackTrace();
				return;
			}
		}else if(streamID!=-1){
			if(pool == null) return;
			pool.stop(streamID);
			bIsPlaying = false;
		}
	}
	
	void setVolume(float vol){
		volume = vol;
		// calculates left/right volumes from pan-value (constant panning law) 
		// see: Curtis Roads: Computer Music Tutorial p 460
		// thanks to jasch
		if(Math.signum(pan) != 0) { // mastering this causes issues if not panning -
			float angle = pan * 0.7853981633974483f; // in radians from -45. to +45.
			float cosAngle = (float) Math.cos(angle);
			float sinAngle = (float) Math.sin(angle);
			leftVolume = (float) ((cosAngle - sinAngle) * 0.7071067811865475) * vol; // multiplied by sqrt(2)/2
			rightVolume = (float) ((cosAngle + sinAngle) * 0.7071067811865475) * vol; // multiplied by sqrt(2)/2
		}
		else {
			leftVolume = vol;
			rightVolume = vol;
		}
		if(stream){
			if(player!=null && bIsPrepared) player.setVolume(leftVolume, rightVolume);
		}else if(streamID!=-1){
			if(pool != null) pool.setVolume(streamID, leftVolume, rightVolume);
		}
	}
	
	float getVolume(){
		return volume;
	}
	
	void setPan(float vol){
		pan = vol;
		// in Android, panning is done by setting the volume on individual channels
		setVolume(volume);
	}
	
	void setSpeed(float spd){
		if(spd<0.5) spd = 0.5f;
		if(spd>2) spd=2;
		speed = spd;
		if(!stream && streamID!=-1) {
			if(pool != null) pool.setRate(streamID, spd);
		}
	}
	
	void setPaused(boolean bP){
		if(stream){
			if(player==null) return;
			if(bP) {
				try {
					if(bIsPrepared && bIsPlaying && player.isPlaying())
						player.pause();
					isPaused = true;
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
			else {
				try {
					if(bIsPrepared)
						player.start();
					isPaused = false;
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}else if(streamID!=-1){
			if(pool == null) return;
			if(bP){
				pool.pause(streamID);
				isPaused = true;
			}else{
				pool.resume(streamID);
				isPaused = false;
			}
			bIsPlaying = !bP;
		}
	}
	
	void setLoop(boolean bLp){
		if(stream){
			if(player!=null) {
				try {
					player.setLooping(bLp);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}else{
			if(streamID!=-1 && pool != null) pool.setLoop(streamID, -1);
		}
		loop = bLp;
	}
	
	void setMultiPlay(boolean bMp){
		multiPlay = bMp;
		if(fileName!=null && multiPlay && stream){
			if(LOG_SOUND) Log.w("OF", "multiplay only supported as no stream, reloading " + fileName + " as no stream");
			String currFileName = fileName;
			unloadSound();
			loadSound(currFileName, false);
		}
	}


	
	void setPosition(float pct){
		if(stream && getIsPlaying()) {
			if(getPositionMS() != pct) {
				try {
					player.seekTo((int) (player.getDuration() * pct)); // 0 = start, 1 = end;
					player.start();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
	}
	
	void setPositionMS(int ms){
		if(stream && getIsPlaying()) {
			if(getPosition() != ms) {
				try {
					player.seekTo(ms); // 0 = start, 1 = end;
					player.start();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
	}
	
	float getPosition(){
		if(stream && getIsPlaying()) {
			float position = 0;
			try {
				position = ((float) player.getCurrentPosition()) / (float) player.getDuration();
			} catch (Exception e) {
				e.printStackTrace();
			}
			return position;
		}
		else
			return 0;
	}
	
	int getPositionMS(){
		if(stream && getIsPlaying()){
			int position = 0;
			try {
				position = player.getCurrentPosition();
			} catch (Exception e) {
				e.printStackTrace();
			}
			return position;
		}
		else
			return 0;
	}
	
	public boolean getIsPlaying(){
		if(stream){
			if(!bIsLoaded || !bIsPrepared) return false;
			boolean isPlaying = false;
			try {
				isPlaying = player!=null && player.isPlaying();
			} catch (Exception e) {
				// NOTE: isPlaying() can potentially throw an exception and crash the application
				e.printStackTrace();
			}
			return isPlaying;
		}else{
			return bIsPlaying;
		}
	}

	public boolean getIsPaused(){
		if(stream){
			boolean paused = false;
			try {
				paused = player!=null && isPaused == true;
			} catch (Exception e) {
				// NOTE: isPlaying() cvan potentially throw an exception and crash the application
				e.printStackTrace();
			}
			return paused;
		}else{
			return isPaused;
		}
	}
	
	float getSpeed(){
		return speed;
	}
	
	float getPan(){
		return pan;
	}
	
	boolean isLoaded(){
		return bIsLoaded && (stream == true && player != null || stream == false);
	}

	@Override
	protected void appPause() {
		boolean wasPlaying = bIsPlaying;
		boolean currIsLoaded = bIsLoaded;
		String currFileName = fileName;
		if(bIsLoaded && bIsPlaying) {
			pausePositionMS = getPositionMS();
		} else {
			pausePositionMS = 0;
		}
		stop();
		if(!stream)
			unloadSound();
		bIsPlaying = wasPlaying;
		fileName = currFileName;
		bIsLoaded = currIsLoaded;
		if(pool != null) pool.autoPause();
	}

	@Override
	protected void appResume() {
		if(bIsLoaded && stream == false){
			bWasResume = true;
			loadSound(fileName, stream);
			if(bIsPrepared) {
				if (bIsPlaying) {
					//Log.i("OF","SoundPlayer appResume was playing " + fileName + " at " + pausePositionMS);
					play();
				}
				if (pausePositionMS != 0) {
					setPositionMS(pausePositionMS);
					pausePositionMS = 0;
				}
			}
		} else  {
			if (Build.VERSION.SDK_INT <= Build.VERSION_CODES.N) {
				Log.i("OF","SoundPlayer appResume - reloading stream for M" + fileName + " at " + pausePositionMS);

				if(bIsPlaying && bIsLoaded && bIsPrepared) {
					//Log.i("OF","SoundPlayer appResume was playing " + fileName + " at " + pausePositionMS);
					play();
				}
				if(pausePositionMS != 0 && bIsLoaded && bIsPrepared) {
					setPositionMS(pausePositionMS);
					pausePositionMS = 0;
				}
			} else {
				Log.i("OF","SoundPlayer appResume");
			}
		}
		if(pool != null) pool.autoResume();
	}

	@Override
	protected void appStop() {
		appPause();
	}

	@Override
	public void onPrepared(MediaPlayer mediaPlayer) {
		if(LOG_SOUND) Log.i("OF","onPrepared:" + fileName );
		bIsPrepared = true;
		if(bWasResume) {
			bWasResume = false;
			if (bIsPlaying) {
				Log.i("OF", "SoundPlayer appResume was playing " + fileName + " at " + pausePositionMS);
				play();
			}
			if (pausePositionMS != 0) {
				setPositionMS(pausePositionMS);
				pausePositionMS = 0;
			}
		}
	}

	@Override
	public boolean onError(MediaPlayer mp, int what, int extra) {
		// ... react appropriately ...
		// The MediaPlayer has moved to the Error state, must be reset!
		Log.w("OF", "onError The MediaPlayer has moved to the Error state, must be reset!");
		bIsPrepared = false;
		bWasResume = false;
		boolean forceResetIfError = true;
		if(forceResetIfError) {

			if (player != null) {
				if(player.isPlaying())
					player.stop();
				player.release();
				player = null;
			}
			if(fileName != null) {
				loadSound(fileName, stream);
				if (bIsPlaying) {
					//Log.i("OF","SoundPlayer appResume was playing " + fileName + " at " + pausePositionMS);
					play();
				}
				if (pausePositionMS != 0) {
					setPositionMS(pausePositionMS);
					pausePositionMS = 0;
				}
			}
		}
		return true;
	}



	private MediaPlayer player = null;
	private static SoundPool pool = null;
	AssetFileDescriptor assetManagerFileDescriptor = null;
	AudioAttributes attributes;
	private float pan ;
	private float volume;
	private boolean bIsLoaded;
	private boolean bIsPlaying;
	private boolean bWasResume;
	private boolean bIsPrepared;
	private boolean multiPlay;
	private String fileName;
	private int soundID, streamID;
	private float leftVolume, rightVolume;
	private boolean loop;
	private float speed;
	private boolean stream;
	int contentType;

	boolean isPaused;

	int pausePositionMS;


}
