package cc.openframeworks;

import android.media.MediaPlayer;
import android.util.Log;

public class OFAndroidSoundPlayer extends OFAndroidObject{
	OFAndroidSoundPlayer(){
		pan = 0.5f;
		volume = 1;
		player = new MediaPlayer();
	}
	
	void loadSound(String fileName, boolean stream){
		try {
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
        // calculates left/right volumes from pan-value (constant panning law) 
        // see: Curtis Roads: Computer Music Tutorial p 460
		// thanks to jasch
        float angle = pan * 0.7853981633974483f; // in radians from -45. to +45.
        float cosAngle = (float) Math.cos(angle);
        float sinAngle = (float) Math.sin(angle);
        float leftVol  = (float)((cosAngle - sinAngle) * 0.7071067811865475); // multiplied by sqrt(2)/2
        float rightVol = (float)((cosAngle + sinAngle) * 0.7071067811865475); // multiplied by sqrt(2)/2
		player.setVolume(leftVol*vol, rightVol*vol);
	}
	
	void setPan(float vol){
		pan = vol*2-1;
		setVolume(volume);
	}
	
	void setSpeed(float spd){
	}
	
	void setPaused(boolean bP){
		if(bP)
			player.pause();
		else
			player.start();
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
	
	void setPositionMS(int ms){
		player.seekTo(ms); // 0 = start, 1 = end;
	}
	
	float getPosition(){
		return ((float)player.getCurrentPosition())/(float)player.getDuration();
	}
	
	int getPositionMS(){
		return player.getCurrentPosition();
	}
	
	boolean getIsPlaying(){
		return player.isPlaying();
	}
	
	float getSpeed(){
		return 1;
	}
	
	float getPan(){
		return pan/2.f+1;
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
