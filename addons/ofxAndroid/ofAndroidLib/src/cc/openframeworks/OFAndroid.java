package cc.openframeworks;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Field;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.pm.ActivityInfo;
import android.hardware.SensorManager;
import android.opengl.GLSurfaceView;
import android.os.Environment;
import android.util.AttributeSet;
import android.util.Log;
import android.view.GestureDetector;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.View.OnClickListener;

public class OFAndroid {
	





	public OFAndroid(String packageName, Activity ofActivity){
		//Log.i("OF","external files dir: "+ ofActivity.getApplicationContext().getExternalFilesDir(null));
		OFAndroid.packageName = packageName;
        try {
        	
			// try to find if R.raw class exists will throw
        	// an exception if not
        	Class<?> raw = Class.forName(packageName+".R$raw");
			
        	// if it exists copy all the raw resources
        	// to a folder in the sdcard
	        Field[] files = raw.getDeclaredFields();
	        
	        String dataPath="";
    		try{
    			dataPath = Environment.getExternalStorageDirectory().getAbsolutePath();
    			dataPath += "/"+packageName;
    			Log.i("OF","creating app directory: " + dataPath);
				try{
					
					File dir = new File(dataPath);
					
					if(!dir.exists() && dir.mkdir()!=true) 
						throw new Exception();
				}catch(Exception e){
					Log.e("OF","error creating dir " + dataPath,e);
				}
				
    			for(int i=0; i<files.length; i++){
    	        	int fileId;
    	        	String fileName="";
    				
    				InputStream from=null;
    				File toFile=null;
    				FileOutputStream to=null;
    	        	try {
    					fileId = files[i].getInt(null);
    					String resName = ofActivity.getResources().getText(fileId).toString();
    					fileName = resName.substring(resName.lastIndexOf("/"));
    					
    					from = ofActivity.getResources().openRawResource(fileId);
    					//toFile = new File(Environment.getExternalStorageDirectory() + "/" + appName + "/" +fileName);
    					Log.i("OF","copying file " + fileName + " to " + dataPath);
    					toFile = new File(dataPath + "/" + fileName);
    					to = new FileOutputStream(toFile);
    					byte[] buffer = new byte[4096];
    					int bytesRead;
    					
    					while ((bytesRead = from.read(buffer)) != -1)
    					    to.write(buffer, 0, bytesRead); // write
    				} catch (Exception e) {
    					Log.e("OF","error copying file",e);
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
    		}catch(Exception e){
    			Log.e("OF","couldn't move app resources to data directory " + dataPath);
    			e.printStackTrace();
    		}
    		String app_name="";
			try {
				int app_name_id = Class.forName(packageName+".R$string").getField("app_name").getInt(null);
				app_name = ofActivity.getResources().getText(app_name_id).toString();
				Log.i("OF","app name: " + app_name);
			} catch (Exception e) {
				// TODO Auto-generated catch block
				Log.e("OF","error retrieving app name",e);
			} 
			OFAndroid.setAppDataDir(dataPath,app_name);
	        
        } catch (ClassNotFoundException e1) { }
        
        OFAndroid.ofActivity = ofActivity;

        gestureListener = new OFGestureListener(ofActivity);
        
        try {
        	Log.v("OF","trying to find class: "+packageName+".R$layout");
			Class<?> layout = Class.forName(packageName+".R$layout");
			View view = ofActivity.getLayoutInflater().inflate(layout.getField("main_layout").getInt(null),null);
			ofActivity.setContentView(view);
			
			Class<?> id = Class.forName(packageName+".R$id");
			mGLView = (OFGLSurfaceView)ofActivity.findViewById(id.getField("of_gl_surface").getInt(null));
			mGLView.setOnClickListener(gestureListener); 
			mGLView.setOnTouchListener(gestureListener.touchListener);
			
			
		} catch (Exception e) {
			Log.e("OF", "couldn't create view from layout falling back to GL only",e);
	        mGLView = new OFGLSurfaceView(ofActivity);
	        ofActivity.setContentView(mGLView);
	        
	        mGLView.setOnClickListener(gestureListener); 
	        mGLView.setOnTouchListener(gestureListener.touchListener);
		}
		android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_URGENT_AUDIO);
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
	
	static public boolean menuItemSelected(MenuItem item){
		try {
			Class<?> menu_ids = Class.forName(packageName+".R$id");
			Field[] fields = menu_ids.getFields();
			for(Field field: fields){
				Log.i("OF", "checking " + field.getName());
				if(item.getItemId() == field.getInt(null)){
					return onMenuItemSelected(field.getName());
				}
			}
		} catch (Exception e) {
			Log.w("OF","Trying to get menu items ", e);
		}
		return false;
	}

	// native methods to call OF c++ callbacks
    public static native void setAppDataDir(String data_dir,String app_name);
    public static native void init();
    public static native void onRestart();
    public static native void onPause();
    public static native void onResume();
    public static native void onStop();
    public static native void onDestroy();
    public static native void onSurfaceCreated();
    public static native void onSurfaceDestroyed();
    public static native void setup(int w, int h);
    public static native void resize(int w, int h);
    public static native void render();
    public static native void exit();
    
    public static native void onTouchDown(int id,float x,float y,float pressure);
    public static native void onTouchDoubleTap(int id,float x,float y,float pressure);
    public static native void onTouchUp(int id,float x,float y,float pressure);
    public static native void onTouchMoved(int id,float x,float y,float pressure);
    
    public static native void onKeyDown(int keyCode);
    public static native void onKeyUp(int keyCode);
    public static native boolean onBackPressed();
    
    public static native boolean onMenuItemSelected(String menu_id);
    

    // static methods to be called from OF c++ code
    public static void setFullscreen(boolean fs){
    	//ofActivity.requestWindowFeature(Window.FEATURE_NO_TITLE);
    	//ofActivity.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, 
        //                        WindowManager.LayoutParams.FLAG_FULLSCREEN);
	  /* if(fs)
	   {
	        ofActivity.getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
	        ofActivity.getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);
	    }
	    else
	    {
	    	ofActivity.getWindow().addFlags(WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);
	    	ofActivity.getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
	    }*/

	    //m_contentView.requestLayout();

    	//ofActivity.getWindow().setAttributes(attrs);
    }
    
    public static void setScreenOrientation(int orientation){
    	switch(orientation){
    	case 0:
    		ofActivity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
    		break;
    	case 90:
    		ofActivity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
    		break;
    	case 270:
    		ofActivity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
    		break;
    	case 180:
    		ofActivity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
    		break;
    	}
    }
    
    public static void pauseApp(){
		ofActivity.moveTaskToBack(true);
    }

	
	public static void setupAccelerometer(){
		if(accelerometer==null)
			accelerometer = new OFAndroidAccelerometer((SensorManager)ofActivity.getSystemService(Context.SENSOR_SERVICE));
	}
	
	public static void setupGPS(){
		if(gps==null)
			gps = new OFAndroidGPS(ofActivity);
		gps.startGPS();
	}
	
	public static void stopGPS(){
		if(gps==null)
			return;
		gps.stopGPS();
	}
	
	public static void alertBox(String msg){  
		new AlertDialog.Builder(ofActivity)  
			.setMessage(msg)  
			.setTitle("OF")  
			.setCancelable(false)  
			.setNeutralButton(android.R.string.ok,  
					new DialogInterface.OnClickListener() {  
				public void onClick(DialogInterface dialog, int whichButton){}  
		  	})  
		  	.show();    
	}
	
	public static Context getContext(){
		return ofActivity;
	}
    
    private OFGLSurfaceView mGLView;
    private static OFAndroidAccelerometer accelerometer;
    private static OFAndroidGPS gps;
    private static Activity ofActivity;
    private OFGestureListener gestureListener;
	private static String packageName;

    
	 
    static {
    	System.loadLibrary("OFAndroidApp"); 
    }



	public View getGLContentView() {
        return mGLView;
	}
	
    static public MenuItem getMenuItemByID(Menu menu, int id){
    	for(int i=0; i<menu.size(); i++){
    		if(menu.getItem(i).getItemId()==id) return menu.getItem(i);
    	}
    	return null;
    }
    
	
}

class OFGestureListener extends SimpleOnGestureListener implements OnClickListener {
	

	OFGestureListener(Activity activity){
		gestureDetector = new GestureDetector(activity,this);
        touchListener = new View.OnTouchListener() {
        	
            public boolean onTouch(View v, MotionEvent event) {
            	final int action = event.getAction();
            	final int pointerIndex = (action & MotionEvent.ACTION_POINTER_ID_MASK) 
                >> MotionEvent.ACTION_POINTER_ID_SHIFT;
                final int pointerId = event.getPointerId(pointerIndex);
                switch((action & MotionEvent.ACTION_MASK)){
                case MotionEvent.ACTION_MOVE:
	            	for(int i=0; i<event.getPointerCount(); i++){
	            		OFAndroid.onTouchMoved(event.getPointerId(i), event.getX(i), event.getY(i), event.getPressure(i));
	            	}
	            	break;
                case MotionEvent.ACTION_POINTER_UP:
                case MotionEvent.ACTION_UP:
                	OFAndroid.onTouchUp(pointerId, event.getX(pointerIndex), event.getY(pointerIndex), event.getPressure(pointerIndex));
                	break;
                case MotionEvent.ACTION_POINTER_DOWN:
                case MotionEvent.ACTION_DOWN:
                	OFAndroid.onTouchDown(pointerId, event.getX(pointerIndex), event.getY(pointerIndex), event.getPressure(pointerIndex));
                	break;
                case MotionEvent.ACTION_CANCEL:
                	//TODO: cancelled
                	break;
                }
                return gestureDetector.onTouchEvent(event);
            }
            
        };
	}
	
	public void onClick(View view) {
	}

    private GestureDetector gestureDetector;
    View.OnTouchListener touchListener;

	@Override
	public boolean onDoubleTap(MotionEvent event) {
		final int action = event.getAction();
		final int pointerIndex = (action & MotionEvent.ACTION_POINTER_ID_MASK) >> MotionEvent.ACTION_POINTER_ID_SHIFT;
        final int pointerId = event.getPointerId(pointerIndex);

        OFAndroid.onTouchDoubleTap(pointerId, event.getX(pointerIndex), event.getY(pointerIndex), event.getPressure(pointerIndex));

		return true;
		//return super.onDoubleTap(e);
	}
	
	@Override
	public boolean onDoubleTapEvent(MotionEvent event) {
		return super.onDoubleTapEvent(event);
	}

	@Override
	public boolean onSingleTapConfirmed(MotionEvent event) {
		return super.onSingleTapConfirmed(event);
	}

	@Override
	public boolean onDown(MotionEvent event) {
		return true;
	}

	@Override
	public boolean onFling(MotionEvent arg0, MotionEvent arg1, float arg2,float arg3) {
		return super.onFling(arg0, arg1, arg2, arg3);
	}

	@Override
	public void onLongPress(MotionEvent arg0) {
	}

	@Override
	public boolean onScroll(MotionEvent arg0, MotionEvent arg1, float arg2,	float arg3) {
		return super.onScroll(arg0, arg1, arg2, arg3);
	}

	@Override
	public void onShowPress(MotionEvent arg0) {
	}

	@Override
	public boolean onSingleTapUp(MotionEvent event) {
		return super.onSingleTapUp(event);
	}
}



class OFGLSurfaceView extends GLSurfaceView{
	public OFGLSurfaceView(Context context) {
        super(context);
        mRenderer = new OFAndroidWindow(getWidth(),getHeight());
        setRenderer(mRenderer);
    }
	
	public OFGLSurfaceView(Context context,AttributeSet attributes) {
        super(context,attributes);
        mRenderer = new OFAndroidWindow(getWidth(),getHeight());
        setRenderer(mRenderer);
    }

    @Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		super.surfaceDestroyed(holder);
    	OFAndroid.onSurfaceDestroyed();
	}


    OFAndroidWindow mRenderer;
}

class OFAndroidWindow implements GLSurfaceView.Renderer {
	
	public OFAndroidWindow(int w, int h){
		this.w = w;
		this.h = h;
	}
	
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
    	if(initialized){
    		OFAndroid.onSurfaceCreated();
    		return;
    	}
    	Log.i("OF","initializing app");
    	OFAndroid.init();
    	OFAndroid.setup(w,h);
    	initialized = true;
    	android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_LOWEST);
    }

    public void onSurfaceChanged(GL10 gl, int w, int h) {
    	OFAndroid.resize(w, h);
		this.w = w;
		this.h = h;
    }

    public void onDrawFrame(GL10 gl) {
    	OFAndroid.render();
    }

    static boolean initialized;
    int w,h;
}