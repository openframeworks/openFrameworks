package cc.openframeworks;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Field;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.opengl.GLSurfaceView;
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;
import android.content.Context;
import android.view.MotionEvent;

public class App extends Activity{
	@Override
    public void onCreate(Bundle savedInstanceState)
    { 
        super.onCreate(savedInstanceState);
        try {
			// try to find if R class exists will throw
        	// an exception if not
        	Class.forName("cc.openframeworks.R");
			
        	// if it exists copy all the raw resources
        	// to a folder in the sdcard
	        Field[] files = R.raw.class.getDeclaredFields();

    		try{
    			new File("/sdcard/" + getResources().getText(R.string.app_name)).mkdir();
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
					fileName = files[i].getName();
					
					from = getResources().openRawResource(fileId);
					toFile = new File("/sdcard/" + getResources().getText(R.string.app_name) + "/" +fileName);
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
        mGLView = new OFGLSurfaceView(this);
        setContentView(mGLView);
    }

    @Override
    protected void onPause() {
        super.onPause();
        mGLView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mGLView.onResume();
    }
    
    private GLSurfaceView mGLView;
	 
	 static {
		 System.loadLibrary("OFAndroidApp"); 
	 }
}

class OFGLSurfaceView extends GLSurfaceView {
    public OFGLSurfaceView(Context context) {
        super(context);
        mRenderer = new OFAndroidWindow(context.getResources().getText(R.string.app_name).toString());
        setRenderer(mRenderer);
    }

    public boolean onTouchEvent(final MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_DOWN) {
            for(int i=0;i<event.getPointerCount();i++){
            	onTouchDown(event.getPointerId(i), event.getX(i), event.getY(i), event.getPressure(i));
            }
        }

        if (event.getAction() == MotionEvent.ACTION_UP) {
            for(int i=0;i<event.getPointerCount();i++){
            	onTouchUp(event.getPointerId(i), event.getX(i), event.getY(i), event.getPressure(i));
            }
        }
        
        if (event.getAction() == MotionEvent.ACTION_MOVE) {
            for(int i=0;i<event.getPointerCount();i++){
            	onTouchMoved(event.getPointerId(i), event.getX(i), event.getY(i), event.getPressure(i));
            }
        }
        return true;
    }

    OFAndroidWindow mRenderer;
    
    private static native void onTouchDown(int id,float x,float y,float pressure);
    private static native void onTouchUp(int id,float x,float y,float pressure);
    private static native void onTouchMoved(int id,float x,float y,float pressure);
}

class OFAndroidWindow implements GLSurfaceView.Renderer {
	public OFAndroidWindow(String app_name){
		this.app_name = app_name;
	}
	
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
    	setAppName(app_name);
    	init();
        setup();
        
    }

    public void onSurfaceChanged(GL10 gl, int w, int h) {
        //gl.glViewport(0, 0, w, h);
        resize(w, h);
    }

    public void onDrawFrame(GL10 gl) {
        render();
    }

    private String app_name;
    private static native void setAppName(String app_name);
    private static native void init();
    private static native void setup();
    private static native void resize(int w, int h);
    private static native void render();
    private static native void exit();
}
