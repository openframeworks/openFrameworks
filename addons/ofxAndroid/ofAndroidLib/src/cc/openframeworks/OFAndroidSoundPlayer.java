package cc.openframeworks;

import android.media.AudioAttributes;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.util.FloatMath;
import android.util.Log;

public class OFAndroidSoundPlayer extends OFAndroidObject implements MediaPlayer.OnPreparedListener, MediaPlayer.OnErrorListener{
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


	}

	public void onDestroy() {
		stop();
		unloadSound();
		if (player != null) {player.stop(); player.release(); }
		if(pool != null) pool.release(); pool = null;
		if(attributes != null) attributes = null;
	}

	protected void setContentType(int contentType){
		if(loop) contentType = AudioAttributes.CONTENT_TYPE_MUSIC;
		 	attributes = new AudioAttributes.Builder()
				.setUsage(AudioAttributes.USAGE_GAME)
				.setContentType(contentType)
				.build();

	}

	protected void createSoundPool(){
		if(pool == null) {
			pool = new SoundPool.Builder()
					.setAudioAttributes(attributes)
					.setMaxStreams(128)
					.build();
		}
	}
	
	void loadSound(String fileName, boolean stream){
		try {
			setContentType(AudioAttributes.CONTENT_TYPE_SONIFICATION);
			if(stream){
				if(player!=null) unloadSound();
				player = new MediaPlayer();
				player.setOnErrorListener(this);
				player.setAudioAttributes(attributes);
				player.setDataSource(fileName);
				player.prepare();
			}else{
				createSoundPool();
				if(soundID!=-1) unloadSound();
				soundID = pool.load(fileName, 1);
			}
			setVolume(volume);
			bIsLoaded = true;
			this.fileName = fileName;
			this.stream = stream;
		} catch (Exception e) {
			Log.e("OF","couldn't load " + fileName,e);
		} 
	}
	
	void unloadSound(){
		if(player!=null){
			player.stop();
			player.reset();
			player.release();
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
			if(player==null) return;
			if(getIsPlaying() && getPosition() != 0) setPosition(0);
			player.start();
		}else{
			if(pool == null) return;
			if(!multiPlay){
				pool.stop(streamID);
			}
			streamID = pool.play(soundID,leftVolume,rightVolume,1,loop?-1:0,speed);
			bIsPlaying = true;
		}
	}
	void stop(){
		if(stream){
			if(player==null) return;
			if(player.isPlaying()) {
				//player.stop();
				player.pause();
				player.seekTo(0);
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
		float angle = pan * 0.7853981633974483f; // in radians from -45. to +45.
		float cosAngle = (float) Math.cos(angle);
		float sinAngle = (float) Math.sin(angle);
		leftVolume  = (float)((cosAngle - sinAngle) * 0.7071067811865475) * vol; // multiplied by sqrt(2)/2
		rightVolume = (float)((cosAngle + sinAngle) * 0.7071067811865475) * vol; // multiplied by sqrt(2)/2
		if(stream){
			if(player!=null) player.setVolume(leftVolume, rightVolume);
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
			if(bP)
				player.pause();
			else
				player.start();
		}else if(streamID!=-1){
			if(pool == null) return;
			if(bP){
				pool.pause(streamID);
			}else{
				pool.resume(streamID);
			}
			bIsPlaying = !bP;
		}
	}
	
	void setLoop(boolean bLp){
		if(stream){
			if(player!=null)
				player.setLooping(bLp);
		}else{
			if(streamID!=-1 && pool != null) pool.setLoop(streamID, -1);
		}
		loop = bLp;
	}
	
	void setMultiPlay(boolean bMp){
		multiPlay = bMp;
		if(fileName!=null && multiPlay && stream){
			Log.w("OF", "multiplay only supported as no stream, reloading " + fileName + " as no stream");
			String currFileName = fileName;
			unloadSound();
			loadSound(currFileName, false);
		}
	}
	
	void setPosition(float pct){
		if(stream && player!=null && player.isPlaying()) {
			if(getPositionMS() != pct)
				player.seekTo((int) (player.getDuration() * pct)); // 0 = start, 1 = end;
		}
	}
	
	void setPositionMS(int ms){
		if(stream && player!=null && player.isPlaying()) {
			if(getPosition() != ms)
				player.seekTo(ms); // 0 = start, 1 = end;
		}
	}
	
	float getPosition(){
		if(stream && player!=null && player.isPlaying())
			return ((float)player.getCurrentPosition())/(float)player.getDuration();
		else
			return 0;
	}
	
	int getPositionMS(){
		if(stream && player!=null && player.isPlaying())
			return player.getCurrentPosition();
		else
			return 0;
	}
	
	boolean getIsPlaying(){
		if(stream){
			return player!=null && player.isPlaying();
		}else{
			return bIsPlaying;
		}
	}
	
	float getSpeed(){
		return speed;
	}
	
	float getPan(){
		return pan;
	}
	
	boolean isLoaded(){
		return bIsLoaded;
	}

	@Override
	protected void appPause() {
		stop();
		String currFileName = fileName;
		boolean currIsLoaded = bIsLoaded; 
		unloadSound();
		fileName = currFileName;
		bIsLoaded = currIsLoaded;
		if(pool != null) pool.autoPause();
	}

	@Override
	protected void appResume() {
		if(bIsLoaded){
			loadSound(fileName, stream);
		}
		if(pool != null) pool.autoResume();
	}

	@Override
	protected void appStop() {
		appPause();
	}

	@Override
	public void onPrepared(MediaPlayer mediaPlayer) {

	}

	@Override
	public boolean onError(MediaPlayer mp, int what, int extra) {
		// ... react appropriately ...
		// The MediaPlayer has moved to the Error state, must be reset!
		Log.w("OF", "onError The MediaPlayer has moved to the Error state, must be reset!");
		return true;
	}



	private MediaPlayer player;
	private static SoundPool pool = null;
	AudioAttributes attributes;
	private float pan;
	private float volume;
	private boolean bIsLoaded;
	private boolean bIsPlaying;
	private boolean multiPlay;
	private String fileName;
	private int soundID, streamID;
	private float leftVolume, rightVolume;
	private boolean loop;
	private float speed;
	private boolean stream;
	int contentType;


}
