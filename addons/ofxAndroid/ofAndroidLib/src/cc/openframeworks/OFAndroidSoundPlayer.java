package cc.openframeworks;

import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.util.FloatMath;
import android.util.Log;

public class OFAndroidSoundPlayer extends OFAndroidObject{
	OFAndroidSoundPlayer(){
		pan = 0.f;
		volume = leftVolume = rightVolume = 1;
		bIsLoaded = false;
		bIsPlaying = false;
		loop = false;
		soundID = -1;
		streamID = -1;
	}
	
	void loadSound(String fileName, boolean stream){
		try {
			if(stream){
				if(player!=null) unloadSound();
				player = new MediaPlayer();
				player.setDataSource(fileName);
				player.prepare();
			}else{
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
			player.release();
			player = null;
		}

		if(soundID!=-1){
			pool.unload(soundID);
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
			if(getIsPlaying()) setPosition(0);	
			player.start();
		}else{
			if(!multiPlay)
				pool.stop(streamID);
			streamID = pool.play(soundID,leftVolume,rightVolume,1,loop?-1:0,speed);
			bIsPlaying = true;
		}
	}
	void stop(){
		if(stream){
			if(player==null) return;
			player.stop();
		}else if(streamID!=-1){
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
        float cosAngle = FloatMath.cos(angle);
        float sinAngle = FloatMath.sin(angle);
        leftVolume  = (float)((cosAngle - sinAngle) * 0.7071067811865475) * vol; // multiplied by sqrt(2)/2
        rightVolume = (float)((cosAngle + sinAngle) * 0.7071067811865475) * vol; // multiplied by sqrt(2)/2
		if(stream){
			if(player!=null) player.setVolume(leftVolume, rightVolume);
		}else if(streamID!=-1){
			pool.setVolume(streamID, leftVolume, rightVolume);
		}
	}
	
	float getVolume(){
		return volume;
	}
	
	void setPan(float vol){
		pan = vol;
		setVolume(volume);
	}
	
	void setSpeed(float spd){
		if(spd<0.5) spd = 0.5f;
		if(spd>2) spd=2;
		speed = spd;
		if(!stream && streamID!=-1) pool.setRate(streamID, spd);
	}
	
	void setPaused(boolean bP){
		if(stream){
			if(player==null) return;
			if(bP)
				player.pause();
			else
				player.start();
		}else if(streamID!=-1){
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
			if(streamID!=-1) pool.setLoop(streamID, -1);
		}
		loop = bLp;
	}
	
	void setMultiPlay(boolean bMp){
		if(multiPlay==bMp) return;
		multiPlay = bMp;
		if(fileName!=null){
			String currFileName = fileName;
			unloadSound();
			loadSound(currFileName, !bMp);
		}
	}
	
	void setPosition(float pct){
		if(stream && player!=null) player.seekTo((int) (player.getDuration()*pct)); // 0 = start, 1 = end;
	}
	
	void setPositionMS(int ms){
		if(stream && player!=null) player.seekTo(ms); // 0 = start, 1 = end;
	}
	
	float getPosition(){
		if(stream && player!=null)
			return ((float)player.getCurrentPosition())/(float)player.getDuration();
		else
			return 0;
	}
	
	int getPositionMS(){
		if(stream && player!=null)
			return player.getCurrentPosition();
		else
			return 0;
	}
	
	boolean getIsPlaying(){
		if(stream)
			return player!=null && player.isPlaying();
		else
			return bIsPlaying;
	}
	
	float getSpeed(){
		return speed;
	}
	
	float getPan(){
		return pan/2.f+1;
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
	}

	@Override
	protected void appResume() {
		if(bIsLoaded){
			loadSound(fileName, !multiPlay);
		}
	}

	@Override
	protected void appStop() {
		appPause();
	}
	
	
	private MediaPlayer player;
	private static SoundPool pool = new SoundPool(128, AudioManager.STREAM_MUSIC, 0);
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
}
