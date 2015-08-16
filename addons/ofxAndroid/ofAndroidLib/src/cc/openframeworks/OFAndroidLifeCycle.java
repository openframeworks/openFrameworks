package cc.openframeworks;

import java.util.Vector;
import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicBoolean;

import android.app.Activity;
import android.util.Log;

public class OFAndroidLifeCycle
{
	private static final int POP_AND_REMOVE_SELF = 1;
	private static final int POP = 2;
	private static final int PUSH = 3;
	
	private static Vector<State> m_statesStack = new Vector<State>();
	private static State m_currentState = null;
	private static Semaphore m_semaphor = new Semaphore(1, false);
	private static AtomicBoolean m_isWorkerDone = new AtomicBoolean(true);

	private static ILifeCycleCallback m_callback = null; 
	private static Activity m_activity = null;
	
	private static void pushState(State state)
	{
		int action = 0;
//        close
        try {
            m_semaphor.acquire();
            do {
                if (m_statesStack.isEmpty()) {
                    action = PUSH;
                    break;
                }
                State lastState = m_statesStack.lastElement();
                action = isDisableState(lastState, state);
                if (action == POP) {
                    m_statesStack.remove(lastState);
                }
            }
            while (action == POP);
            
            switch (action) {
                case POP_AND_REMOVE_SELF:
                    m_statesStack.remove(m_statesStack.size() - 1);
                    break;
                case PUSH:
                    m_statesStack.add(state);
                    break;

                default:
                    break;
            }
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
	
	private static int isDisableState(State lastInStack, State newState)
	{
		String errorMessage = "Illegal pushed state! Last state in stack "+ lastInStack.toString()+" new state: "+newState.toString();
		int  result = PUSH;
		switch (lastInStack) {
		case create:
			if(newState.equals(State.pause)||newState.equals(State.init)||newState.equals(State.create))
				throw new IllegalStateException(errorMessage);
			else if(newState.equals(State.exit))
				result = POP;
			else if(newState.equals(State.destroy))
				result = POP_AND_REMOVE_SELF;
			break;
		case resume:
			if(newState.equals(State.resume)||newState.equals(State.create)||newState.equals(State.init))
				throw new IllegalStateException(errorMessage);
			else if(newState.equals(State.destroy))
				result = POP;
			else if(newState.equals(State.pause))
				result = POP_AND_REMOVE_SELF;
			break;
		case pause:
			if(newState.equals(State.exit)||newState.equals(State.create)||newState.equals(State.init)||newState.equals(State.pause))
				throw new IllegalStateException(errorMessage);
			else if(newState.equals(State.resume))
				result = POP_AND_REMOVE_SELF;
			break;
		case destroy:
			if(newState.equals(State.destroy)||newState.equals(State.init)||newState.equals(State.resume)||newState.equals(State.pause))
				throw new IllegalStateException(errorMessage);
			else if(newState.equals(State.create))
				result = POP_AND_REMOVE_SELF;
			break;
		case exit:
			throw new IllegalStateException(errorMessage);

		default:
			break;
		}
		return result;
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
		case resume:
			if(!(m_currentState.equals(State.create)||m_currentState.equals(State.pause)))
				isLegal = false;
			break;
		case pause:
			if(!m_currentState.equals(State.resume))
				isLegal = false;
			break;
		case destroy:
			if(!(m_currentState.equals(State.pause)||m_currentState.equals(State.create)))
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
				// TODO Auto-generated method stub
				Runnable callbackFunction = null;
//close
                try {
                    m_semaphor.acquire();
                    while (!m_statesStack.isEmpty()) {
                        State next = m_statesStack.firstElement();
                        m_statesStack.removeElement(next);
//                    release
                        m_semaphor.release();
                        if (!isNextStateLegal(next))
                            throw new IllegalStateException("Illegal next state! when current state " + m_currentState.toString() + " next state: " + next.toString());

                        m_currentState = next;
                        switch (next) {
                            case init:
                                OFAndroidLifeCycleHelper.appInit(m_activity);
                                callbackFunction = new Runnable() {

                                    @Override
                                    public void run() {
                                        // TODO Auto-generated method stub
                                        m_callback.callbackInit();
                                    }
                                };
                                break;
                            case create:
                                OFAndroidLifeCycleHelper.onCreate();
                                callbackFunction = new Runnable() {

                                    @Override
                                    public void run() {
                                        // TODO Auto-generated method stub
                                        m_callback.callbackCreated();
                                    }
                                };
                                break;
                            case resume:
                                OFAndroidLifeCycleHelper.onResume();
                                callbackFunction = new Runnable() {

                                    @Override
                                    public void run() {
                                        // TODO Auto-generated method stub
                                        m_callback.callbackResumed();
                                    }
                                };
                                break;
                            case pause:
                                OFAndroidLifeCycleHelper.onPause();
                                callbackFunction = new Runnable() {

                                    @Override
                                    public void run() {
                                        // TODO Auto-generated method stub
                                        m_callback.callbackPaused();
                                    }
                                };
                                break;
                            case destroy:
                                OFAndroidLifeCycleHelper.onDestroy();
                                callbackFunction = new Runnable() {

                                    @Override
                                    public void run() {
                                        // TODO Auto-generated method stub
                                        m_callback.callbackDestroed();
                                    }
                                };
                                break;
                            case exit:
                                OFAndroidLifeCycleHelper.exit();
                                m_currentState = null;
                                break;

                            default:
                                break;
                        }
                        m_activity.runOnUiThread(callbackFunction);
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
	
//============================ Life Cycle Functions ===================//
	public static void init(ILifeCycleCallback callback, Activity activity)
	{
		OFAndroidLifeCycle.m_callback = callback;
		OFAndroidLifeCycle.m_activity = activity;
		if(m_currentState != null)
		{
			callback.callbackInit();
			return;
		}
		pushState(State.init);
	}
	
	public static void onCreate(OFActivity activity)
	{
		OFAndroidLifeCycle.m_activity = activity;
		OFAndroidLifeCycle.m_callback = activity;
		pushState(State.create);
	}
	
	public static void onResume()
	{
		pushState(State.resume);
	}
	
	public static void onPause()
	{
		pushState(State.pause);
	}
	
	public static void onDestroy()
	{
		pushState(State.destroy);
	}
	
	public static void exit()
	{
		pushState(State.exit);
	}
	
	public enum State 
	{
		init, create, resume, pause, destroy, exit;
	}

	public interface ILifeCycleCallback
	{
		public void callbackInit();
		
		public void callbackCreated();
		
		public void callbackResumed();
		
		public void callbackPaused();
		
		public void callbackDestroed();
	}
}
