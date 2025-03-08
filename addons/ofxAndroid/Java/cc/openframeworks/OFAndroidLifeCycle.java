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
import android.widget.RelativeLayout;

import static android.opengl.EGL14.EGL_NO_CONTEXT;

public class OFAndroidLifeCycle
{
	public static Boolean coreLibraryLoaded = false;
	public static Boolean appLibraryLoaded = false;

	private static Vector<State> m_statesStack = new Vector<State>();
	private static State m_currentState = null;
	private static Semaphore m_semaphor = new Semaphore(1, false);
	private static AtomicBoolean m_isWorkerDone = new AtomicBoolean(true);
	private static AtomicBoolean m_isInit = new AtomicBoolean(false);
	private static AtomicBoolean m_isStateInit = new AtomicBoolean(false);
	private static AtomicBoolean m_isSurfaceCreated = new AtomicBoolean(false);
	private static AtomicBoolean m_isFirstFrameDrawn = new AtomicBoolean(false);
	private static AtomicBoolean m_hasSetup = new AtomicBoolean(false);
	private static OFActivity m_activity = null;
	
	private static int m_countActivities = 0;
	private static ArrayList<Runnable> m_initializers = new ArrayList<Runnable>();
	
	private static OFGLSurfaceView mGLView = null;
	private static OFGLSurfaceView pausedGLView = null;
	
	private static void pushState(State state)
	{
        try {
            m_semaphor.acquire();
            m_statesStack.add(state);
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
		if(next == null) return false;
		if(m_currentState == null) return true;
		
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
			if(!(m_currentState.equals(State.pause)|| m_currentState.equals(State.start)))
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
                try {
                    m_semaphor.acquire();
                    while (!m_statesStack.isEmpty()) {
                        State next = m_statesStack.firstElement();
                        m_statesStack.removeElement(next);
                        m_semaphor.release();
                        if (next != null && !isNextStateLegal(next))
                        {
                        	if(m_currentState != null)
								Log.e(OFAndroidLifeCycle.class.getSimpleName(), "Illegal next state! when current state " + m_currentState.toString() + " next state: " + next.toString());
                            else
								Log.e(OFAndroidLifeCycle.class.getSimpleName(), "Illegal next state! when current state null and next state: " + next.toString());

							//break;
							//throw new IllegalStateException("Illegal next state! when current state " + m_currentState.toString() + " next state: " + next.toString());
                        }
                        if(next != null) {
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
									synchronized (m_isSurfaceCreated) {
										m_isSurfaceCreated.set(true);
									}
									OFAndroidLifeCycleHelper.onStart();
									synchronized (m_isSurfaceCreated) {
										m_hasSetup.set(true);
									}
									break;
								case stop:
									OFAndroidLifeCycleHelper.onStop();
									break;
								case pause:
									OFAndroidLifeCycleHelper.onPause();
									break;
								case resume:
									synchronized (m_isSurfaceCreated) {
										if (m_hasSetup.get() == true)
											OFAndroidLifeCycleHelper.onResume();
										else
											Log.e(OFAndroidLifeCycle.class.getSimpleName(), "state:resume hasSetup == false");
									}
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
						}
                        m_semaphor.acquire();
                    }
                }
                catch (InterruptedException ex)
                {
                    Log.e(OFAndroidLifeCycle.class.getSimpleName(), "startWorkerThread: stack size: "+m_statesStack.size() + "exception message: "+ex.getMessage(), ex);
                    m_semaphor.release();
                }
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
        if(m_activity != null) {
			m_activity.runOnUiThread(new Runnable() {

				@Override
				public void run() {
					for (Runnable init : m_initializers) {
						init.run();
					}
					m_initializers.clear();
				}
			});
		}
    }

	public static void HasSetup() {
		ViewGroup glContainer = getActivity().getSurfaceContainer();
		if(glContainer != null && pausedGLView != null ) {
			if (glContainer.getChildCount() > 0) {

				m_activity.runOnUiThread(new Runnable() {

					@Override
					public void run() {
						ViewGroup glContainer = getActivity().getSurfaceContainer();

						//if(pausedGLView != null)
							//glContainer.removeView(pausedGLView);
					}
				});
			}
		}

				//			ViewGroup glContainer = getActivity().getSurfaceContainer();
				//glContainer.removeAllViews();
//				if(surfaceView != null && pausedGLView == surfaceView) {
//					Log.d(TAG, "SetFirstFrameDrawn removing pausedGLView");
//					glContainer.removeView(pausedGLView);
//					pausedGLView = null;
//					if (mGLView != null)
				//glContainer.removeView(pausedGLView);
//				if(mGLView != null){
//					glContainer.addView(mGLView, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
//				} else {
//					Log.d(TAG, "HasSetup removing pausedGLView");
//					pausedGLView = null;
//				}}
//					});
//			}
//				}
				//glContainer.removeAllViews();
			//}
//			if(mGLView != null) {
//				ViewGroup parent = (ViewGroup) mGLView.getParent();
//				if (parent == null)
//					glContainer.addView(mGLView, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
//			}
		//}
	}

	public static void SetFirstFrameDrawn()
	{
		synchronized (m_isFirstFrameDrawn) {
			m_isFirstFrameDrawn.set(true);
		}
		ViewGroup glContainer = getActivity().getSurfaceContainer();
		if(glContainer != null && pausedGLView != null ) {
			if (glContainer.getChildCount() > 0) {

				m_activity.runOnUiThread(new Runnable() {

					@Override
					public void run() {

						ViewGroup glContainer = getActivity().getSurfaceContainer();
						if(mGLView != null) {
							glContainer.bringChildToFront(mGLView);
						}
						if(pausedGLView != null)
							glContainer.removeView(pausedGLView);

						if (glContainer.getChildCount() > 1) {
							for(int i=glContainer.getChildCount()-1; i>=0;i--) {
								View view = glContainer.getChildAt(i);
								if(view != null && view != mGLView) {
									glContainer.removeViewAt(i);
									Log.w(OFAndroidLifeCycle.class.getSimpleName(), "SetFirstFrameDrawn::Removing Extra View at Index " + i);
								}
							}
						}
						if(isSurfaceValid() == false ) {
							// Exception whereFrame broken
							Log.w(OFAndroidLifeCycle.class.getSimpleName(), "SetFirstFrameDrawn::surface is not valid - Width:0 Height:0 - Recreate");
							glResume(glContainer);
						}
					}
				});
			}
		}
	}

	static boolean isSurfaceValid() {
		if(mGLView != null &&
				(mGLView.getHeight() >= 1 || mGLView.getWidth() >= 1)) {
			return true;
		} else {
			return false;
		}
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

	public static boolean firstFrameDrawn()
	{
		synchronized (m_isFirstFrameDrawn) {
			return m_isFirstFrameDrawn.get();
		}
	}

	public static void reset()
	{
		coreLibraryLoaded = false;
		appLibraryLoaded = false;
		m_statesStack.clear();
		m_initializers.clear();
		mGLView = null;
		pausedGLView = null;

		synchronized (m_isFirstFrameDrawn) {
			m_isFirstFrameDrawn.set(false);
		}
		synchronized (m_isSurfaceCreated) {
			m_isSurfaceCreated.set(false);
		}
		synchronized (m_isStateInit) {
			m_isStateInit.set(false);
		}
		synchronized (m_isFirstFrameDrawn) {
			m_isFirstFrameDrawn.set(false);
		}
	}

	public static boolean isSurfaceCreated()
	{
		synchronized (m_isSurfaceCreated) {
			return m_isSurfaceCreated.get();
		}
	}
	
	public static void init()
	{
		Log.i("OF","OFAndroid init...");

		synchronized (m_isStateInit) {
			if(m_isStateInit.get() == false) {
				m_isStateInit.set(true);
				pushState(State.init);

			}

		}
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
			if(glView != null) {
				if (preserveContextOnPause)
					glView.setPreserveEGLContextOnPause(true);
				ViewGroup parent = (ViewGroup) glView.getParent();

				if(glContainer != null) {
					if (glContainer.getChildCount() > 0) {
						View view = glContainer.getChildAt(0);
						if (pausedGLView == null)
							pausedGLView = (OFGLSurfaceView) view;
						if (pausedGLView != null) pausedGLView.setDoNotDraw();
						//glContainer.removeAllViews();
					}
					if (parent == null)
						glContainer.addView(glView, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));

					if (pausedGLView != null) {
						//glContainer.removeView(pausedGLView); // make ontop
						//glContainer.addView(pausedGLView, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
						//LayoutParams top = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT)
						//top.addRule(RelativeLayout.ALIGN_PARENT_TOP);
//					RelativeLayout relativePause = (RelativeLayout)pausedGLView;
//					pausedGLView.addRule();
						glContainer.bringChildToFront(pausedGLView);
					}
				} else {
					Log.e(TAG, "OFAndroidLifeCycle glCreateSurface glContainer is null");
				}
			}
		}
	}
	
	public static void glStart()
	{
		Log.d(TAG, "glStart");
		OFGLSurfaceView glView = getGLView();
		if( glView != null ) {
			if(glView.isSetup()) {
				glView.onResume();
			}
		}
		
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
		if(OFActivity.LOG_ENGINE) Log.d(TAG, "glRestart");
	}
	
	public static void glResume(ViewGroup glContainer)
	{
		if(OFActivity.LOG_ENGINE) Log.d(TAG, "glResume");
		OFGLSurfaceView glView = getGLView();
		boolean offScreenLayoutParamFix = false;
		if(glView.getWidth() <= 1|| glView.getHeight() <= 1) {
			offScreenLayoutParamFix = true;
			Log.d(TAG, "glResume:: offScreenLayoutParamFix required");
		}
		if( glView != null && glView.getRenderer() != null && offScreenLayoutParamFix == false) {

			glView.onResume();
		} else {
			if( glView != null ) {
				glView.onResume();
			}
			pausedGLView = mGLView;
			clearGLView(); // mGLView = null;
			Log.d(TAG, "glResume setGLESVersion");
			OFEGLConfigChooser.setGLESVersion(OFAndroid.eglVersion);
			glCreateSurface(true);
		}

//		if(mGLView != null) {
//			glContainer.removeView(mGLView);
//			glContainer.addView(mGLView, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
//		}
//		if(pausedGLView != null) {
//			glContainer.removeView(pausedGLView);
//			glContainer.addView(pausedGLView, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
//		}
		pushState(State.resume);
	}
	
	public static void glPause()
	{
		if(OFActivity.LOG_ENGINE) Log.d(TAG, "glPause");
		OFGLSurfaceView glView = getGLView();
		if( glView != null ) {
			glView.onPause();
		}
		OFAndroidLifeCycleHelper.onPause();
		pushState(State.pause);
	}
	
	public static void glDestroy()
	{
		if(OFActivity.LOG_ENGINE) Log.d(TAG, "glDestroy");
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
