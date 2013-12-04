package cc.openframeworks;

import java.util.HashSet;
import java.util.Set;

import android.app.Activity;
import android.content.Intent;


public abstract class OFAndroidObject {

	public enum State{
		Created,
		Running,
		Paused,
		Stopped
	}
	
	protected State state;
	public static Set<OFAndroidObject> ofObjects = new HashSet<OFAndroidObject>();
	
	public OFAndroidObject(){
		state = State.Created;

		synchronized (ofObjects) {
			ofObjects.add(this);
		}
	}
	
    public void onPause(){
    	appPause();
    	state = State.Paused;
    }
    public void onResume(){
    	appResume();
    	state = State.Running;
    }
    public void onStop(){
    	appStop();
    	state = State.Stopped;
    }
    public void release(){
		synchronized (OFAndroidObject.ofObjects) {
			ofObjects.remove(this);
		}
    }
    
    public void onActivityResult(int requestCode, int resultCode,Intent intent){
    	
    }
    
    abstract protected void appPause();
    abstract protected void appResume();
    abstract protected void appStop();
    
    public static void setActivity(Activity activity){
    	OFAndroidObject.activity = activity;
    }
	protected static Activity activity;
}
