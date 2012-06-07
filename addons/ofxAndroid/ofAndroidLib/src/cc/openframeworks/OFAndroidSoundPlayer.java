package cc.openframeworks;

import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.util.Log;

public class OFAndroidSoundPlayer extends OFAndroidObject{
	OFAndroidSoundPlayer(){
		pan = 0.5f;
		volume = leftVolume = rightVolume = 1;
		player = new MediaPlayer();
		bIsLoaded = false;
	}
	
	void loadSound(String fileName, boolean stream){
		try {
			if(!multiPlay){
				player.setDataSource(fileName);
				player.prepare();
				bIsLoaded = true;
			}else{
				fileID = pool.load(fileName, 1);
			}
			this.fileName = fileName;
		} catch (Exception e) {
			Log.e("OF","couldn't load " + fileName,e);
		} 
	}
	
	void unloadSound(){
		player.release();
		fileName = null;
		fileID = 0;
		bIsLoaded = false;
	}
	
	void play(){
		if(!multiPlay){
			if(getIsPlaying()) setPosition(0);	
			player.start();
		}else{
			pool.play(fileID,leftVolume,rightVolume,1,loop?-1:0,speed);
		}
	}
	void stop(){
		if(!multiPlay){
			player.stop();
		}else{
			pool.autoPause();
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
		if(!multiPlay) player.setVolume(leftVolume, rightVolume);
	}
	
	float getVolume(){
		return volume;
	}
	
	void setPan(float vol){
		pan = vol*2-1;
		setVolume(volume);
	}
	
	void setSpeed(float spd){
		speed = spd;
	}
	
	void setPaused(boolean bP){
		if(!multiPlay){
			if(bP)
				player.pause();
			else
				player.start();
		}else{
			if(bP){
				pool.autoPause();
			}else{
				pool.autoResume();
			}
		}
	}
	
	void setLoop(boolean bLp){
		if(!multiPlay){
			player.setLooping(bLp);
		}
		loop = bLp;
	}
	
	void setMultiPlay(boolean bMp){
		if(bMp){
			if(fileName!=null) fileID = pool.load(fileName, 1);
		}
		multiPlay = bMp;
	}
	
	void setPosition(float pct){
		if(!multiPlay) player.seekTo((int) (player.getDuration()*pct)); // 0 = start, 1 = end;
	}
	
	void setPositionMS(int ms){
		if(!multiPlay) player.seekTo(ms); // 0 = start, 1 = end;
	}
	
	float getPosition(){
		if(!multiPlay)
			return ((float)player.getCurrentPosition())/(float)player.getDuration();
		else
			return 0;
	}
	
	int getPositionMS(){
		if(!multiPlay)
			return player.getCurrentPosition();
		else
			return 0;
	}
	
	boolean getIsPlaying(){
		if(!multiPlay)
			return player.isPlaying();
		else
			return true;
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
	}

	@Override
	protected void appResume() {
		
	}

	@Override
	protected void appStop() {
		stop();
	}
	
	
	private MediaPlayer player;
	private static SoundPool pool = new SoundPool(100, AudioManager.STREAM_MUSIC, 0);
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
