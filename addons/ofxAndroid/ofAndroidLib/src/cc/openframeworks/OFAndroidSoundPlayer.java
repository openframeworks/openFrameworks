package cc.openframeworks;

import android.media.MediaPlayer;
import android.util.Log;

public class OFAndroidSoundPlayer extends OFAndroidObject{
	OFAndroidSoundPlayer(){
		pan = 0.5f;
		volume = 1;
	}
	
	void loadSound(String fileName, boolean stream){
		try {
			player = new MediaPlayer();
			player.setDataSource(fileName);
			player.prepare();
		} catch (Exception e) {
			Log.e("OF","couldn't load " + fileName,e);
		} 
	}
	
	void unloadSound(){
		player.release();
	}
	
	void play(){
		if(getIsPlaying()) setPosition(0);
		player.start();
	}
	void stop(){
		player.stop();
	}
	
	void setVolume(float vol){
		volume = vol;
		float rightVol=1,leftVol=1;
		if(pan <= 0.5){
			leftVol = 1;
			rightVol = 1 + 2*(pan - 0.5f);  
		}
		if(pan >= 0.5){
			rightVol = 1;
			leftVol = 1 - 2*(pan - 0.5f);
		}
		player.setVolume(leftVol*vol, rightVol*vol);
	}
	
	void setPan(float vol){
		pan = vol;
		setVolume(volume);
	}
	
	void setSpeed(float spd){
	}
	
	void setPaused(boolean bP){
		player.pause();
	}
	
	void setLoop(boolean bLp){
		player.setLooping(bLp);
	}
	
	void setMultiPlay(boolean bMp){
		;
	}
	void setPosition(float pct){
		player.seekTo((int) (player.getDuration()*pct)); // 0 = start, 1 = end;
	}
	
	float getPosition(){
		return ((float)player.getCurrentPosition())/(float)player.getDuration();
	}
	boolean getIsPlaying(){
		return player.isPlaying();
	}
	float getSpeed(){
		return 1;
	}
	float getPan(){
		return pan;
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
	private float pan;
	private float volume;

}
