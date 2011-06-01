package cc.openframeworks;

import java.util.HashSet;
import java.util.Set;


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
    	pause();
    	state = State.Paused;
    }
    public void onResume(){
    	resume();
    	state = State.Running;
    }
    public void onStop(){
    	stop();
    	state = State.Stopped;
    }
    public void release(){
    	ofObjects.remove(this);
    }
    
    abstract protected void pause();
    abstract protected void resume();
    abstract protected void stop();
}
