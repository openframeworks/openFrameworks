package cc.openframeworks;

import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.util.FloatMath;
import android.util.Log;

public class OFAndroidSoundPlayer extends OFAndroidObject{
	OFAndroidSoundPlayer(){
		pan = 0.5f;
		volume = leftVolume = rightVolume = 1;
		bIsLoaded = false;
		fileID = -1;
	}
	
	void loadSound(String fileName, boolean stream){
		try {
			if(!multiPlay){
				if(player!=null) unloadSound();
				player = new MediaPlayer();
				player.setDataSource(fileName);
				player.prepare();
			}else{
				if(pool!=null) unloadSound();
				pool = new SoundPool(8, AudioManager.STREAM_MUSIC, 0);
				fileID = pool.load(fileName, 1);
			}
			setVolume(volume);
			bIsLoaded = true;
			this.fileName = fileName;
		} catch (Exception e) {
			Log.e("OF","couldn't load " + fileName,e);
		} 
	}
	
	void unloadSound(){
		if(player!=null){
			player.release();
			player = null;
		}

		if(pool!=null){
			pool.release();
			pool = null;	
		}
		fileName = null;
		fileID = 0;
		bIsLoaded = false;
	}
	
	void play(){
		if(!multiPlay){
			if(player==null) return;
			if(getIsPlaying()) setPosition(0);	
			player.start();
		}else{
			if(pool==null) return;
			pool.play(fileID,leftVolume,rightVolume,1,loop?-1:0,speed);
		}
	}
	void stop(){
		if(!multiPlay){
			if(player==null) return;
			player.stop();
		}else{
			if(pool==null) return;
			pool.autoPause();
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
		if(!multiPlay && player!=null) player.setVolume(leftVolume, rightVolume);
	}
	
	float getVolume(){
		return volume;
	}
	
	void setPan(float vol){
		pan = vol;
		setVolume(volume);
	}
	
	void setSpeed(float spd){
		speed = spd;
	}
	
	void setPaused(boolean bP){
		if(!multiPlay){
			if(player==null) return;
			if(bP)
				player.pause();
			else
				player.start();
		}else{
			if(pool==null) return;
			if(bP){
				pool.autoPause();
			}else{
				pool.autoResume();
			}
		}
	}
	
	void setLoop(boolean bLp){
		if(!multiPlay && player!=null){
			player.setLooping(bLp);
		}
		loop = bLp;
	}
	
	void setMultiPlay(boolean bMp){
		if(multiPlay==bMp) return;
		multiPlay = bMp;
		if(fileName!=null && fileName!=null){
			String currFileName = fileName;
			unloadSound();
			loadSound(currFileName, !bMp);
		}
	}
	
	void setPosition(float pct){
		if(!multiPlay && player!=null) player.seekTo((int) (player.getDuration()*pct)); // 0 = start, 1 = end;
	}
	
	void setPositionMS(int ms){
		if(!multiPlay && player!=null) player.seekTo(ms); // 0 = start, 1 = end;
	}
	
	float getPosition(){
		if(!multiPlay && player!=null)
			return ((float)player.getCurrentPosition())/(float)player.getDuration();
		else
			return 0;
	}
	
	int getPositionMS(){
		if(!multiPlay && player!=null)
			return player.getCurrentPosition();
		else
			return 0;
	}
	
	boolean getIsPlaying(){
		if(!multiPlay)
			return player!=null && player.isPlaying();
		else
			return pool!=null;
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
	private SoundPool pool = new SoundPool(8, AudioManager.STREAM_MUSIC, 0);
	private float pan;
	private float volume;
	private boolean bIsLoaded;
	private boolean multiPlay;
	private String fileName;
	private int fileID;
	private float leftVolume, rightVolume;
	private boolean loop;
	private float speed;
}
