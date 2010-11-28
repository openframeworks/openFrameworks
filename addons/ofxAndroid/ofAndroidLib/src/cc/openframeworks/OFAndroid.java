package cc.openframeworks;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Field;
import java.util.Iterator;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.Context;
import android.hardware.SensorManager;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

public class OFAndroid {
	
	public OFAndroid(String appName, String packageName, Activity ofActivity){

        try {
        	
			// try to find if R.raw class exists will throw
        	// an exception if not
        	Class<?> raw = Class.forName(packageName+".R$raw");
			
        	// if it exists copy all the raw resources
        	// to a folder in the sdcard
	        Field[] files = raw.getDeclaredFields();

    		try{
    			new File("/sdcard/" + appName).mkdir();
    		}catch(Exception e){
    			
    		}
    		
	        for(int i=0; i<files.length; i++){
	        	int fileId;
	        	String fileName;
				
				InputStream from=null;
				File toFile=null;
				FileOutputStream to=null;
	        	try {
					fileId = files[i].getInt(null);
					String resName = ofActivity.getResources().getText(fileId).toString();
					fileName = resName.substring(resName.lastIndexOf("/"));
					
					from = ofActivity.getResources().openRawResource(fileId);
					toFile = new File("/sdcard/" + appName + "/" +fileName);
					to = new FileOutputStream(toFile);
					byte[] buffer = new byte[4096];
					int bytesRead;
					
					while ((bytesRead = from.read(buffer)) != -1)
					    to.write(buffer, 0, bytesRead); // write
				} catch (Exception e) {
					e.printStackTrace();
				} finally {
					if (from != null)
					  try {
					    from.close();
					  } catch (IOException e) { }
					  
			        if (to != null)
			          try {
			            to.close();
			          } catch (IOException e) { }
				}
	        }
        } catch (ClassNotFoundException e1) { }
        
        mGLView = new OFGLSurfaceView(ofActivity, appName);
        ofActivity.setContentView(mGLView);
        
        accelerometer = new OFAndroidAccelerometer((SensorManager)ofActivity.getSystemService(Context.SENSOR_SERVICE));
    }

	public void start(){
		Log.i("OF","onStart");
	}
	
	public void restart(){
		Log.i("OF","onRestart");
		onRestart();
        /*if(OFAndroidSoundStream.isInitialized() && OFAndroidSoundStream.wasStarted())
        	OFAndroidSoundStream.getInstance().start();*/
	}
	
	public void pause(){
		Log.i("OF","onPause");
		mGLView.onPause();
		onPause();

		for(OFAndroidObject object : OFAndroidObject.ofObjects){
			object.onPause();
		}
		
	}
	
	public void resume(){
		Log.i("OF","onResume");
        mGLView.onResume();
        onResume();

		for(OFAndroidObject object : OFAndroidObject.ofObjects){
			object.onResume();
		}
	}
	
	public void stop(){
		Log.i("OF","onStop");
		onStop();
		for(OFAndroidObject object : OFAndroidObject.ofObjects){
			object.onStop();
		}
		/*if(OFAndroidSoundStream.isInitialized()) 
			OFAndroidSoundStream.getInstance().stop();*/
	}
	
	public void destroy(){
		Log.i("OF","onDestroy");
		onDestroy();
	}
    
    private GLSurfaceView mGLView;
    private OFAndroidAccelerometer accelerometer;
	 
    static {
    	System.loadLibrary("OFAndroidApp"); 
    }


    public static native void setAppName(String app_name);
    public static native void init();
    public static native void onRestart();
    public static native void onPause();
    public static native void onResume();
    public static native void onStop();
    public static native void onDestroy();
    public static native void onSurfaceCreated();
    public static native void onSurfaceDestroyed();
    public static native void setup();
    public static native void resize(int w, int h);
    public static native void render();
    public static native void exit();
    
    public static native void onTouchDown(int id,float x,float y,float pressure);
    public static native void onTouchUp(int id,float x,float y,float pressure);
    public static native void onTouchMoved(int id,float x,float y,float pressure);
    
}

class OFGLSurfaceView extends GLSurfaceView {
	public OFGLSurfaceView(Context context, String appName) {
        super(context);
        mRenderer = new OFAndroidWindow(appName);
        setRenderer(mRenderer);
    }

    public boolean onTouchEvent(final MotionEvent event) {
    	final int action = event.getAction();
        switch (action & MotionEvent.ACTION_MASK) {
        case MotionEvent.ACTION_DOWN: {
        	OFAndroid.onTouchDown(event.getPointerId(0),event.getX(),event.getY(),event.getPressure());
            break;
        }
            
        case MotionEvent.ACTION_MOVE: {
        	for(int i=0; i<event.getPointerCount(); i++){
        		OFAndroid.onTouchMoved(event.getPointerId(i), event.getX(i), event.getY(i), event.getPressure(i));
        	}
            break;
        }
            
        case MotionEvent.ACTION_UP: {
        	OFAndroid.onTouchUp(event.getPointerId(0), event.getX(), event.getY(), event.getPressure());
            break;
        }
            
        case MotionEvent.ACTION_CANCEL: {
            break;
        }
        
        case MotionEvent.ACTION_POINTER_UP: {
            // Extract the index of the pointer that left the touch sensor
            final int pointerIndex = (action & MotionEvent.ACTION_POINTER_ID_MASK) 
                    >> MotionEvent.ACTION_POINTER_ID_SHIFT;
            final int pointerId = event.getPointerId(pointerIndex);

            OFAndroid.onTouchUp(pointerId, event.getX(pointerIndex), event.getY(pointerIndex), event.getPressure(pointerIndex));
            break;
        }
        case MotionEvent.ACTION_POINTER_DOWN: {
            // Extract the index of the pointer that left the touch sensor
            final int pointerIndex = (action & MotionEvent.ACTION_POINTER_ID_MASK) 
                    >> MotionEvent.ACTION_POINTER_ID_SHIFT;
            final int pointerId = event.getPointerId(pointerIndex);

            OFAndroid.onTouchDown(pointerId, event.getX(pointerIndex), event.getY(pointerIndex), event.getPressure(pointerIndex));
            break;
        }
        }
        
        return true;
    }

    @Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		super.surfaceDestroyed(holder);
    	OFAndroid.onSurfaceDestroyed();
	}

    OFAndroidWindow mRenderer;
}

class OFAndroidWindow implements GLSurfaceView.Renderer {
	public OFAndroidWindow(String app_name){
		this.app_name = app_name;
	}
	
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
    	OFAndroid.onSurfaceCreated();
    	if(initialized) return;
    	Log.i("OF","initializing app");
    	OFAndroid.setAppName(app_name);
    	OFAndroid.init();
    	OFAndroid.setup();
    	initialized = true;
    }

    public void onSurfaceChanged(GL10 gl, int w, int h) {
        //gl.glViewport(0, 0, w, h);
    	OFAndroid.resize(w, h);
    }

    public void onDrawFrame(GL10 gl) {
    	OFAndroid.render();
    }

    private String app_name;
    static boolean initialized;
}