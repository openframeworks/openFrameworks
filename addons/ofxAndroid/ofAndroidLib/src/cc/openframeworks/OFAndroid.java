package cc.openframeworks;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Field;
import java.util.HashMap;
import java.util.Map;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.pm.ActivityInfo;
import android.hardware.SensorManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.opengl.GLSurfaceView;
import android.os.Environment;
import android.os.PowerManager;
import android.util.AttributeSet;
import android.util.Log;
import android.view.GestureDetector;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.CompoundButton;
import android.widget.Toast;

public class OFAndroid {
	
	public OFAndroid(String packageName, Activity ofActivity){
		//Log.i("OF","external files dir: "+ ofActivity.getApplicationContext().getExternalFilesDir(null));
		OFAndroid.packageName = packageName;
		OFAndroidObject.setActivity(ofActivity);
        try {
        	
			// try to find if R.raw class exists will throw
        	// an exception if not
        	Class<?> raw = Class.forName(packageName+".R$raw");
			
        	// if it exists copy all the raw resources
        	// to a folder in the sdcard
	        Field[] files = raw.getDeclaredFields();
	        
	        dataPath="";
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
			enableTouchEvents();
			
			
		} catch (Exception e) {
			Log.e("OF", "couldn't create view from layout falling back to GL only",e);
	        mGLView = new OFGLSurfaceView(ofActivity);
	        ofActivity.setContentView(mGLView);
	        enableTouchEvents();
		}
		//android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_URGENT_AUDIO);
    }

	public void start(){
		Log.i("OF","onStart");
		enableTouchEvents();
	}
	
	public void restart(){
		Log.i("OF","onRestart");
		enableTouchEvents();
		onRestart();
        /*if(OFAndroidSoundStream.isInitialized() && OFAndroidSoundStream.wasStarted())
        	OFAndroidSoundStream.getInstance().start();*/
	}
	
	public void pause(){
		Log.i("OF","onPause");
		disableTouchEvents();
		mGLView.onPause();
		onPause();

		for(OFAndroidObject object : OFAndroidObject.ofObjects){
			object.onPause();
		}
		
		unlockScreenSleep();
	}
	
	public void resume(){
		Log.i("OF","onResume");
		enableTouchEvents();
        mGLView.onResume();
        onResume();

		for(OFAndroidObject object : OFAndroidObject.ofObjects){
			object.onResume();
		}
		
		if(wl!=null) lockScreenSleep();
	}
	
	public void stop(){
		Log.i("OF","onStop");
		disableTouchEvents();
		onStop();
		for(OFAndroidObject object : OFAndroidObject.ofObjects){
			object.onStop();
		}
		
		unlockScreenSleep();
		/*if(OFAndroidSoundStream.isInitialized()) 
			OFAndroidSoundStream.getInstance().stop();*/
	}
	
	public void destroy(){
		Log.i("OF","onDestroy");
		onDestroy();
	}
	
	static public boolean menuItemSelected(int id){
		try {
			Class<?> menu_ids = Class.forName(packageName+".R$id");
			Field[] fields = menu_ids.getFields();
			for(Field field: fields){
				Log.i("OF", "checking " + field.getName());
				if(id == field.getInt(null)){
					return onMenuItemSelected(field.getName());
				}
			}
		} catch (Exception e) {
			Log.w("OF","Trying to get menu items ", e);
		}
		return false;
	}
	
	static public boolean menuItemChecked(int id, boolean checked){
		try {
			Class<?> menu_ids = Class.forName(packageName+".R$id");
			Field[] fields = menu_ids.getFields();
			for(Field field: fields){
				if(id == field.getInt(null)){
					return onMenuItemChecked(field.getName(),checked);
				}
			}
		} catch (Exception e) {
			Log.w("OF","Trying to get menu items ", e);
		}
		return false;
	}
	
	static public void setMenuItemChecked(String idStr, boolean checked){
		final String id = idStr;
		ofActivity.runOnUiThread(new Runnable(){
			public void run() {
				try {
					Class<?> menu_ids = Class.forName(packageName+".R$id");
					Field field = menu_ids.getField(id);
					//ofActivity.getMenuInflater().
				} catch (Exception e) {
					Log.w("OF","Trying to get menu items ", e);
				}
			}
		});
	}
	
	static public void setViewItemChecked(String idStr, boolean checked){
		final String id = idStr;
		final boolean fchecked = checked;
		ofActivity.runOnUiThread(new Runnable(){
			public void run() {
				try {
					Class<?> menu_ids = Class.forName(packageName+".R$id");
					Field field = menu_ids.getField(id);
					CompoundButton checkbox = (CompoundButton) ofActivity.findViewById(field.getInt(null));
					checkbox.setChecked(fchecked);
				} catch (Exception e) {
					Log.w("OF","Trying to get menu items ", e);
				}
			}
		});
	}
	
	static public String getStringRes(String idStr){
		Class<?> string_ids;
		try {
			string_ids = Class.forName(packageName+".R$string");
			Field field = string_ids.getField(idStr);
			int id = field.getInt(null);
			return (String) ofActivity.getResources().getText(id);
		} catch (Exception e) {
			Log.e("OF","Couldn't get string resource",e);
		} 
		return "";
	}
	
	static public boolean isOnline(){
		try{
			ConnectivityManager conMgr =  (ConnectivityManager)ofActivity.getSystemService(Context.CONNECTIVITY_SERVICE);
			return conMgr!=null && ( conMgr.getNetworkInfo(ConnectivityManager.TYPE_MOBILE).getState() == NetworkInfo.State.CONNECTED 
				    ||  conMgr.getNetworkInfo(ConnectivityManager.TYPE_WIFI).getState() == NetworkInfo.State.CONNECTED  ) ;
		}catch(Exception e){
			return false;
		}
	}
	
	static public boolean isWifiOnline(){
		try{
			ConnectivityManager conMgr =  (ConnectivityManager)ofActivity.getSystemService(Context.CONNECTIVITY_SERVICE);
			return conMgr!=null && ( conMgr.getNetworkInfo(ConnectivityManager.TYPE_WIFI).getState() == NetworkInfo.State.CONNECTED  ) ;
		}catch(Exception e){
			return false;
		}
	}
	
	static public boolean isMobileOnline(){
		try{
			ConnectivityManager conMgr =  (ConnectivityManager)ofActivity.getSystemService(Context.CONNECTIVITY_SERVICE);
			
			return conMgr!=null && ( conMgr.getNetworkInfo(ConnectivityManager.TYPE_MOBILE).getState() == NetworkInfo.State.CONNECTED  ) ;
		}catch(Exception e){
			return false;
		}
	}
	
	
	static Map<Integer,ProgressDialog> progressDialogs = new HashMap<Integer, ProgressDialog>();
	static int lastProgressID=0;
	static public int progressBox(String msg){
		final String finmsg = msg;
		final int id = lastProgressID++;
		ofActivity.runOnUiThread(new Runnable(){
			public void run() {
				ProgressDialog d = new ProgressDialog(ofActivity);
				d.setMessage(finmsg);
				d.show();
				progressDialogs.put(id,d);
			}
		});
		return id;
	}
	
	static public void dismissProgressBox(int id){
		final ProgressDialog d = progressDialogs.get(id);
		if(d!=null){
			ofActivity.runOnUiThread(new Runnable(){
				public void run() {
					d.dismiss();
				}
			});
			progressDialogs.remove(id);
		}
	}
	
	static public void okCancelBox(String msg){
		final String alertMsg = msg;
		ofActivity.runOnUiThread(new Runnable(){
			public void run() {
				new AlertDialog.Builder(ofActivity)  
					.setMessage(alertMsg)  
					.setTitle("OF")  
					.setCancelable(false)  
					.setNeutralButton(android.R.string.ok,  
							new DialogInterface.OnClickListener() {  
						public void onClick(DialogInterface dialog, int whichButton){
							OFAndroid.okPressed();
						}
	
				  	})
				  	.setNegativeButton(android.R.string.cancel,

							new DialogInterface.OnClickListener() {  
						public void onClick(DialogInterface dialog, int whichButton){
							OFAndroid.cancelPressed();
						}
				  	})
				  	.show();    
				
			}  
		});
	}
	
	static public boolean checkSDCardMounted(){
		boolean canSaveExternal = false;

		String storageState = Environment.getExternalStorageState();

		if (Environment.MEDIA_MOUNTED.equals(storageState))
			canSaveExternal = true;
		else
			canSaveExternal = false;
		
		return canSaveExternal;
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
    public static native boolean onMenuItemChecked(String menu_id, boolean checked);
    
    public static native void okPressed();
    public static native void cancelPressed();
    

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
    	case -1:
    		ofActivity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR);
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
		final String alertMsg = msg;
		ofActivity.runOnUiThread(new Runnable(){
			public void run() {
				new AlertDialog.Builder(ofActivity)  
					.setMessage(alertMsg)  
					.setTitle("")  
					.setCancelable(false)  
					.setNeutralButton(android.R.string.ok,  
							new DialogInterface.OnClickListener() {  
						public void onClick(DialogInterface dialog, int whichButton){
							OFAndroid.okPressed();
						}
	
				  	})  
				  	.show();
			}  
		});
	}
	
	public static void toast(String msg){  
		if(msg=="") return;
		final String alertMsg = msg;
		ofActivity.runOnUiThread(new Runnable(){
			public void run() {
				Toast toast = Toast.makeText(ofActivity, alertMsg, Toast.LENGTH_SHORT);
	        	toast.show();  
			}  
		});
	}
	
	public static Context getContext(){
		return ofActivity;
	}
	
	public static String toDataPath(String path){
		return dataPath + "/" + path;
	}
	
	public static void lockScreenSleep(){
		if(wl==null){
			PowerManager pm = (PowerManager) ofActivity.getSystemService(Context.POWER_SERVICE);
	        wl = pm.newWakeLock(PowerManager.SCREEN_BRIGHT_WAKE_LOCK, "DoNotDimScreen");
		}
        wl.acquire();
        
	}
	
	public static void unlockScreenSleep(){
		if(wl==null) return;
		wl.release();
	}
	
    
    private OFGLSurfaceView mGLView;
    private static OFAndroidAccelerometer accelerometer;
    private static OFAndroidGPS gps;
    private static Activity ofActivity;
    private OFGestureListener gestureListener;
	private static String packageName;
	private static String dataPath;
	private static PowerManager.WakeLock wl;

    public static native boolean hasNeon();
	 
    static {
    	try{
    		System.loadLibrary("neondetection"); 
	    	if(hasNeon()){
	    		Log.i("OF","loading neon optimized library");
	    		System.loadLibrary("OFAndroidApp_neon");
	    	}else{
	    		Log.i("OF","loading not-neon optimized library");
	    		System.loadLibrary("OFAndroidApp");
	    	}
    	}catch(Throwable e){
    		Log.i("OF","failed neon detection, loading not-neon library",e);
    		System.loadLibrary("OFAndroidApp");
    	}
    }



	public View getGLContentView() {
        return mGLView;
	}
	
	public void disableTouchEvents(){
        mGLView.setOnClickListener(null); 
        mGLView.setOnTouchListener(null);
	}
	
	public void enableTouchEvents(){
        mGLView.setOnClickListener(gestureListener); 
        mGLView.setOnTouchListener(gestureListener.touchListener);
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
    	
    }

    public void onSurfaceChanged(GL10 gl, int w, int h) {
    	if(!setup){
        	Log.i("OF","initializing app");
        	OFAndroid.init();
        	OFAndroid.setup(w,h);
        	initialized = true;
        	setup = true;
        	android.os.Process.setThreadPriority(8);
        	
        	/*if(ETC1Util.isETC1Supported()) Log.i("OF","ETC supported");
        	else Log.i("OF","ETC not supported");*/
    	}
    	OFAndroid.resize(w, h);
		this.w = w;
		this.h = h;
    }

    public void onDrawFrame(GL10 gl) {
    	if(setup)
    		OFAndroid.render();
    }

    static boolean initialized;
    static boolean setup;
    int w,h;
}