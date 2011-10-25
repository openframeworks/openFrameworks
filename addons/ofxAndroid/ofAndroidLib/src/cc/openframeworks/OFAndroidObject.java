package cc.openframeworks;

import java.util.HashSet;
import java.util.Set;

import android.app.Activity;


public abstract class OFAndroidObject {

	public enum State{
		Created,
		Running,
		Paused,
		Stopped
	}
	
	protected State state;
	public static Set<OFAndroidObject> ofObjects = new HashSet<OFAndroidObject>();
	
	OFAndroidObject(){
		state = State.Created;
		ofObjects.add(this);
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
    	ofObjects.remove(this);
    }
    
    abstract protected void appPause();
    abstract protected void appResume();
    abstract protected void appStop();
    
    public static void setActivity(Activity activity){
    	OFAndroidObject.activity = activity;
    }
	protected static Activity activity;
}
