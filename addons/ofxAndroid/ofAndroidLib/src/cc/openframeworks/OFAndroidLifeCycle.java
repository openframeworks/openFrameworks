package cc.openframeworks;

import java.util.ArrayList;
import java.util.Vector;
import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicBoolean;

import android.app.Activity;
import android.media.AudioManager;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;

public class OFAndroidLifeCycle
{
	
	static {
        
        Log.i("OF","static init");
        
        try {
            Log.i("OF","loading x86 library");
            System.loadLibrary("OFAndroidApp_x86");
        }
        catch(Throwable ex)	{
            Log.i("OF","failed x86 loading, trying neon detection",ex);
            
            try{
                System.loadLibrary("neondetection");
                if(OFAndroid.hasNeon()){
                    Log.i("OF","loading neon optimized library");
                    System.loadLibrary("OFAndroidApp_neon");
                }
                else{
                    Log.i("OF","loading not-neon optimized library");
                    System.loadLibrary("OFAndroidApp");
                }
            }catch(Throwable ex2){
                Log.i("OF","failed neon detection, loading not-neon library",ex2);
                System.loadLibrary("OFAndroidApp");
            }
        }
        Log.i("OF","initializing app");
    }
		
	private static Vector<State> m_statesStack = new Vector<State>();
	private static State m_currentState = null;
	private static Semaphore m_semaphor = new Semaphore(1, false);
	private static AtomicBoolean m_isWorkerDone = new AtomicBoolean(true);
	private static AtomicBoolean m_isInit = new AtomicBoolean(false);

	private static OFActivity m_activity = null;
	
	private static int m_countActivities = 0;
	private static ArrayList<Runnable> m_initializers = new ArrayList<Runnable>();
	
	private static OFGLSurfaceView mGLView = null;
	
	private static void pushState(State state)
	{
//        close
        try {
            m_semaphor.acquire();
            m_statesStack.add(state);
//        release
            m_semaphor.release();
            startWorkerThread();
        }
        catch (InterruptedException ex)
        {
            Log.e(OFAndroidLifeCycle.class.getSimpleName(), "pushState exception message: "+ex.getMessage(), ex);
            throw new RuntimeException("pushState state: "+ state +" exception message: "+ex.getMessage());
        }
	}
	
	private static boolean isNextStateLegal(State next)
	{
		boolean isLegal = true;
		
		switch(next)
		{
		case init:
			if(m_currentState != null)
				isLegal = false;
			break;
		case create:
			if(!(m_currentState.equals(State.init)||m_currentState.equals(State.destroy)))
				isLegal = false;
			break;
		case start:
			if(!(m_currentState.equals(State.create)||m_currentState.equals(State.stop)))
				isLegal = false;
			break;
		case stop:
			if(!(m_currentState.equals(State.start)||m_currentState.equals(State.pause)))
				isLegal = false;
			break;
		case resume:
			if(!(m_currentState.equals(State.pause)||m_currentState.equals(State.start)))
				isLegal = false;
			break;
		case pause:
			if(!m_currentState.equals(State.resume))
				isLegal = false;
			break;
		case destroy:
			if(!(m_currentState.equals(State.stop)||m_currentState.equals(State.create)))
				isLegal = false;
			break;
		case exit:
			if(!(m_currentState.equals(State.init)||m_currentState.equals(State.destroy)))
				isLegal = false;
			break;
		}
		return isLegal;
	}
	
	private static void startWorkerThread() throws IllegalStateException
	{
		synchronized (m_isWorkerDone) 
		{
			if(!m_isWorkerDone.get())
				return;
			m_isWorkerDone.set(false);
		}
		Thread worker = new Thread(new Runnable() {
			
			@Override
			public void run() 
			{
//close
                try {
                    m_semaphor.acquire();
                    while (!m_statesStack.isEmpty()) {
                        State next = m_statesStack.firstElement();
                        m_statesStack.removeElement(next);
//                    release
                        m_semaphor.release();
                        if (!isNextStateLegal(next))
                        {
                            throw new IllegalStateException("Illegal next state! when current state " + m_currentState.toString() + " next state: " + next.toString());
                        }
                        
                        m_currentState = next;
                        
                        switch (next) {
                            case init:
                                OFAndroidLifeCycleHelper.appInit(m_activity);
                                coreInitialized();
                                break;
                            case create:
                                OFAndroidLifeCycleHelper.onCreate();
                                break;
                            case start:
                                OFAndroidLifeCycleHelper.onStart();
                                break;
                            case stop:
                                OFAndroidLifeCycleHelper.onStop();
                                break;
                            case pause:
                                OFAndroidLifeCycleHelper.onPause();
                                break;
                            case resume:
                                OFAndroidLifeCycleHelper.onResume();
                                break;
                            case destroy:
                                OFAndroidLifeCycleHelper.onDestroy();
                                break;
                            case exit:
                                OFAndroidLifeCycleHelper.exit();
                                m_currentState = null;
                                break;

                            default:
                                break;
                        }
                        //close
                        m_semaphor.acquire();
                    }
                }
                catch (InterruptedException ex)
                {
                    Log.e(OFAndroidLifeCycle.class.getSimpleName(), "startWorkerThread: stack size: "+m_statesStack.size() + "exception message: "+ex.getMessage(), ex);
                    m_semaphor.release();
                }
//                release
                m_semaphor.release();
				synchronized (m_isWorkerDone) {
					m_isWorkerDone.set(true);
				}
			}
		});
		worker.start();
	}
	
	private static void coreInitialized()
    {
        synchronized (m_isInit) {
        	m_isInit.set(true);
		}
        if(m_activity != null)
        	m_activity.runOnUiThread(new Runnable() {
				
				@Override
				public void run() {
					for (Runnable init : m_initializers)
			        {
						init.run();
			        }
			        m_initializers.clear();
				}
			});
    }
	
	static OFActivity getActivity(){
		return OFAndroidLifeCycle.m_activity;
	}
	
	static OFGLSurfaceView getGLView(){
		return mGLView;
	}
	
	static void clearGLView(){
		mGLView = null;
	}
	
	public static void setActivity(OFActivity activity){
		m_activity = activity;
		activity.setVolumeControlStream(AudioManager.STREAM_MUSIC);
		OFAndroidObject.setActivity(activity);
	}
	
//============================ Life Cycle Functions ===================//
	
	public static void addPostInit(Runnable runnable)
	{
		if(isInit())
			runnable.run();
        else
        {
            m_initializers.add(runnable);
        }
	}
	
	public static void clearPostInit(){
		m_initializers.clear();
	}
	
	public static boolean isInit()
	{
		synchronized (m_isInit) {
			return m_isInit.get();
		}
	}
	
	public static void init()
	{
		if(m_currentState != null)
		{
			return;
		}
		Log.i("OF","OFAndroid init...");
		pushState(State.init);
	}
	
	static String TAG = "OF";
		
	public static void glCreate()
	{
		Log.d(TAG, "glCreate");
		if(m_countActivities == 0)
			pushState(State.create);
		m_countActivities++;
	}
	
	public static void glCreateSurface( boolean preserveContextOnPause )
	{
		if(mGLView == null)
		{
			Log.d(TAG, "Create surface");
			mGLView = new OFGLSurfaceView(m_activity);
			
			ViewGroup glContainer = getActivity().getSurfaceContainer();
			OFGLSurfaceView glView = getGLView();
			if( preserveContextOnPause )
				glView.setPreserveEGLContextOnPause( true );
			ViewGroup parent = (ViewGroup)glView.getParent();
			if( parent == null )
				glContainer.addView(glView, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
		}
	}
	
	public static void glStart()
	{
		Log.d(TAG, "glStart");
		OFGLSurfaceView glView = getGLView();
		if( glView != null )
			glView.onResume();
		
		pushState(State.start);
	}
	
	public static void glStop()
	{
		Log.d(TAG, "glStop");
		OFGLSurfaceView glView = getGLView();
		if( glView != null )
			glView.onPause();
		
		pushState(State.stop);
	}
	
	public static void glRestart()
	{
	}
	
	public static void glResume(ViewGroup glContainer)
	{
		Log.d(TAG, "glResume");
		pushState(State.resume);
	}
	
	public static void glPause()
	{
		Log.d(TAG, "glPause");
		pushState(State.pause);
	}
	
	public static void glDestroy()
	{
		Log.d(TAG, "glDestroy");
		m_countActivities--;
		if(m_countActivities == 0){
			Log.d(TAG, "glDestroy destroy ofApp");
			pushState(State.destroy);
		}
	}
	
	public static void exit()
	{
		pushState(State.exit);
	}
	
	public enum State 
	{
		init, create, start, stop, destroy, exit, pause, resume;
	}
}
