package cc.openframeworks;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Field;
import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.content.pm.ActivityInfo;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.graphics.PixelFormat;
import android.hardware.SensorManager;
import android.media.AudioManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiManager.MulticastLock;
import android.opengl.GLSurfaceView;
import android.os.Environment;
import android.util.AttributeSet;
import android.util.Log;
import android.view.GestureDetector;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.WindowManager.LayoutParams;
import android.widget.ArrayAdapter;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.Toast;

public class OFAndroid {
	
	// List based on http://bit.ly/NpkL4Q
	private static final String[] mExternalStorageDirectories = new String[] { 
			"/mnt/sdcard-ext", 
			"/mnt/sdcard/external_sd", 
			"/sdcard/sd", 
			"/mnt/external_sd", 
			"/emmc",  
			"/mnt/sdcard/bpemmctest", 
			"/mnt/sdcard/_ExternalSD",  
			"/mnt/Removable/MicroSD", 
			"/Removable/MicroSD" };
	
	public static String getRealExternalStorageDirectory()
	{				
		// Standard way to get the external storage directory
		String externalPath = Environment.getExternalStorageDirectory().getAbsolutePath();	
		File SDCardDir = new File(externalPath);		
    	if(SDCardDir.exists() && SDCardDir.canWrite()) {		
    		return externalPath;
    	}
		
		// This checks if any of the directories from mExternalStorageDirectories exist, if it does, it uses that one instead
		for(int i = 0; i < mExternalStorageDirectories.length; i++)
		{
			//Log.i("OF", "Checking: " + mExternalStorageDirectories[i]);	
			SDCardDir = new File(mExternalStorageDirectories[i]);		
	    	if(SDCardDir.exists() && SDCardDir.canWrite()) {				
	    		externalPath = mExternalStorageDirectories[i];	// Found writable location
				break;
	    	}	    	
		}
		
		Log.i("OF", "Using storage location: " + externalPath);
		return externalPath;		
	}
	
	public static String getAppDataDirectory(){
		return dataPath;
	}
	
	public OFAndroid(String appPackageName, OFActivity activity){
		OFAndroid.ofActivity = activity;
		ofActivity.setVolumeControlStream(AudioManager.STREAM_MUSIC);
		//Log.i("OF","external files dir: "+ ofActivity.getApplicationContext().getExternalFilesDir(null));
		OFAndroid.packageName = appPackageName;
		OFAndroidObject.setActivity(ofActivity);
		unpackingDone = false;
		
		new Thread(new Runnable(){
			@Override
			public void run() {
		        try {
		        	
					// try to find if R.raw class exists will throw
		        	// an exception if not
		        	Class<?> raw = Class.forName(packageName+".R$raw");
					
		        	// if it exists copy all the raw resources
		        	// to a folder in the sdcard
			        Field[] files = raw.getDeclaredFields();
			        
			        boolean copydata = false;
		
			        SharedPreferences preferences = ofActivity.getPreferences(Context.MODE_PRIVATE);
			        long lastInstalled = preferences.getLong("installed", 0);
			        
			        PackageManager pm = ofActivity.getPackageManager();
			        ApplicationInfo appInfo = pm.getApplicationInfo(packageName, 0);
			        String appFile = appInfo.sourceDir;
			        long installed = new File(appFile).lastModified();
			        if(installed>lastInstalled){
			        	Editor editor = preferences.edit();
			        	editor.putLong("installed", installed);
			        	editor.commit();
			        	copydata = true;
			        }
			        
			        ofActivity.onLoadPercent(.05f);
		
			        dataPath="";
		    		try{
						// Set data path
						//dataPath = Environment.getExternalStorageDirectory().getAbsolutePath();
						dataPath = getRealExternalStorageDirectory();
						dataPath += "/Android/data/"+packageName;

						// Check if old data path exists and copy content to new data path location
						String oldDataPath = getRealExternalStorageDirectory() + "/" + packageName;								
						File oldDataDir = new File(oldDataPath);				
						if(oldDataDir.exists()) 
						{	
							Log.d("OF", "Found old data in: " + oldDataPath);

							File newDatadir = new File(dataPath);
							if (oldDataDir.renameTo(newDatadir))
								Log.d("OF", "Moved data to new storage location: " + dataPath);
							else
								Log.e("OF", "Could not move data to new storage location: " + dataPath);				
						}

		    			Log.i("OF","creating app directory: " + dataPath);
						try{
							
							File dir = new File(dataPath);
							
							if(!dir.exists() && dir.mkdir()!=true) 
								throw new Exception();
						}catch(Exception e){
							Log.e("OF","error creating dir " + dataPath,e);
						}
				        ofActivity.onLoadPercent(.10f);
						
						if(copydata){
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

						        ofActivity.onLoadPercent(.10f+.30f/files.length*i);
			    	        }
						}else{
					        ofActivity.onLoadPercent(.40f);
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
			        
		        } catch (ClassNotFoundException e1) { 
		        	
		        } catch (NameNotFoundException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}		
		        OFAndroid.onUnpackingResourcesDone();
		        ofActivity.onLoadPercent(.80f);
			}
			//android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_URGENT_AUDIO);
		}).start();
		
		
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

		if(networkStateReceiver!=null){
			try{
				ofActivity.unregisterReceiver(networkStateReceiver);
			}catch(java.lang.IllegalArgumentException e){
				
			}
		}
	}
	
	public void resume(){
		Log.i("OF","onResume");
		enableTouchEvents();
        mGLView.onResume();
		
		for(OFAndroidObject object : OFAndroidObject.ofObjects){
			object.onResume();
		}
		
        onResume();
        
        if(OFAndroid.orientation!=-1) OFAndroid.setScreenOrientation(OFAndroid.orientation);
		
		if(networkStateReceiver!=null){
			monitorNetworkState();
		}
	}
	
	public void stop(){
		Log.i("OF","onStop");
		disableTouchEvents();
		onStop();
		for(OFAndroidObject object : OFAndroidObject.ofObjects){
			object.onStop();
		}
		
		unlockScreenSleep();

		if(networkStateReceiver!=null){
			try{
				ofActivity.unregisterReceiver(networkStateReceiver);
			}catch(java.lang.IllegalArgumentException e){
				
			}
		}
		/*if(OFAndroidSoundStream.isInitialized()) 
			OFAndroidSoundStream.getInstance().stop();*/
	}
	
	public void destroy(){
		Log.i("OF","onDestroy");
		onDestroy();
	}
	
	static public void onUnpackingResourcesDone(){
		unpackingDone = true;
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
			return isWifiOnline() || isMobileOnline();
		}catch(Exception e){
			Log.e("OF","error checking connection",e);
			return false;
		}
	}
	
	static public boolean isWifiOnline(){
		try{
			ConnectivityManager conMgr =  (ConnectivityManager)ofActivity.getSystemService(Context.CONNECTIVITY_SERVICE);
			return conMgr!=null && ( conMgr.getNetworkInfo(ConnectivityManager.TYPE_WIFI).getState() == NetworkInfo.State.CONNECTED  ) ;
		}catch(Exception e){
			Log.e("OF","error checking wifi connection",e);
			return false;
		}
	}
	
	static public boolean isMobileOnline(){
		try{
			ConnectivityManager conMgr =  (ConnectivityManager)ofActivity.getSystemService(Context.CONNECTIVITY_SERVICE);
			
			return conMgr!=null && ( conMgr.getNetworkInfo(ConnectivityManager.TYPE_MOBILE).getState() == NetworkInfo.State.CONNECTED  ) ;
		}catch(Exception e){
			Log.e("OF","error checking mobile connection",e);
			return false;
		}
	}

	static private BroadcastReceiver networkStateReceiver;

	static public void monitorNetworkState(){
		networkStateReceiver = new BroadcastReceiver() {

		    @Override
		    public void onReceive(Context context, Intent intent) {
		    	String action = intent.getAction();
		    	if(!action.equals(ConnectivityManager.CONNECTIVITY_ACTION)) return;
		    	
	    		boolean noConnectivity =
	                    intent.getBooleanExtra(ConnectivityManager.EXTRA_NO_CONNECTIVITY, false);

	            if (noConnectivity) {
	                networkConnected(false);
	            } else {
	            	networkConnected(true);
	            }
		        Log.w("Network Listener", "Network Type Changed");
		    }
		};
		IntentFilter filter = new IntentFilter(ConnectivityManager.CONNECTIVITY_ACTION);        
		ofActivity.registerReceiver(networkStateReceiver, filter);
		networkConnected(isOnline());
	}
	
	static public void launchBrowser(String url){
		ofActivity.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse(url)));
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
				d.setCancelable(false);
				d.show();
				progressDialogs.put(id,d);
			}
		});
		return id;
	}
	
	static public void dismissProgressBox(int id){
		final int dId = id;
		final ProgressDialog d = progressDialogs.get(id);
		if(d!=null){
			ofActivity.runOnUiThread(new Runnable(){
				public void run() {
					d.dismiss();
					progressDialogs.remove(dId);
				}
			});
		}
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
	
	public static void onActivityResult(int requestCode, int resultCode,Intent intent){

		for(OFAndroidObject object : OFAndroidObject.ofObjects){
			object.onActivityResult(requestCode,resultCode,intent);
		}
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
    public static native void onTouchCancelled(int id,float x,float y);
    
    public static native void onSwipe(int id, int swipeDir);
    
    public static native void onKeyDown(int keyCode);
    public static native void onKeyUp(int keyCode);
    public static native boolean onBackPressed();
    
    public static native boolean onMenuItemSelected(String menu_id);
    public static native boolean onMenuItemChecked(String menu_id, boolean checked);
    
    public static native void okPressed();
    public static native void cancelPressed();
    
    public static native void networkConnected(boolean conected);
    

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
    
    
    private static int orientation=-1;
    public static void setScreenOrientation(int orientation){
    	OFAndroid.orientation = orientation;
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
	
	static MulticastLock mcLock;
	public static void enableMulticast(){
		WifiManager wifi = (WifiManager)ofActivity.getSystemService( Context.WIFI_SERVICE );
		if(wifi != null)
		{
		    mcLock = wifi.createMulticastLock("mylock");
		    mcLock.acquire();
		}
	}
	
	public static void disableMulticast(){
		if(mcLock!=null){
			mcLock.release();
		}
	}
	
	public static void alertBox(String msg){  
		final String alertMsg = msg;
		/*try{
			Looper.prepare();
		}catch(Exception e){
			
		}
		final Handler handler = new Handler() {
	        @Override
	        public void handleMessage(Message mesg) {
	            throw new RuntimeException();
	        } 
	    };*/
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
							//handler.sendMessage(handler.obtainMessage());
						}
	
				  	})  
				  	.show();
			}  
		});
		// loop till a runtime exception is triggered.
	    /*try { Looper.loop(); }
	    catch(RuntimeException e2) {}*/
	    //alert.dismiss();
	}
	
	
	
	static public void positiveNegativeBox(String msg,final int positiveButton, final int negativeButton){
		final String alertMsg = msg; 
		/*try{
			Looper.prepare();
		}catch(Exception e){
			
		}
		final Handler handler = new Handler() {
	        @Override
	        public void handleMessage(Message mesg) {
	            throw new RuntimeException();
	        } 
	    };*/
		ofActivity.runOnUiThread(new Runnable(){
			public void run() {
				new AlertDialog.Builder(ofActivity)  
					.setMessage(alertMsg)  
					.setTitle("OF")  
					.setCancelable(false)  
					.setNeutralButton(positiveButton,  
							new DialogInterface.OnClickListener() {  
						public void onClick(DialogInterface dialog, int whichButton){
							OFAndroid.okPressed();
							//handler.sendMessage(handler.obtainMessage());
						}
	
				  	})
				  	.setNegativeButton(negativeButton,

							new DialogInterface.OnClickListener() {  
						public void onClick(DialogInterface dialog, int whichButton){
							OFAndroid.cancelPressed();
							//handler.sendMessage(handler.obtainMessage());
						}
				  	})
				  	.show();    
				
			}  
		});
		// loop till a runtime exception is triggered.
	    /*try { Looper.loop(); }
	    catch(RuntimeException e2) {}*/
	}
	
	public static void okCancelBox(String msg){
		positiveNegativeBox(msg,android.R.string.ok,android.R.string.cancel);
	}
	
	public static void yesNoBox(String msg){
		positiveNegativeBox(msg,android.R.string.yes,android.R.string.no);
	}

    private static String textBoxResult="";
	public static void alertTextBox(String question, String text){  
		final String alertQuestion = question;
		final String alertMsg = text;
		/*try{
			Looper.prepare();
		}catch(Exception e){
			
		}
		final Handler handler = new Handler() {
	        @Override
	        public void handleMessage(Message mesg) {
	            throw new RuntimeException();
	        } 
	    };*/
	    textBoxResult=text;
		ofActivity.runOnUiThread(new Runnable(){
			public void run() {
				final EditText input = new EditText(ofActivity); 
					new AlertDialog.Builder(ofActivity)  
					.setMessage(alertMsg)  
					.setTitle(alertQuestion)  
					.setCancelable(false)  
					.setNeutralButton(android.R.string.ok,  
							new DialogInterface.OnClickListener() {  
						public void onClick(DialogInterface dialog, int whichButton){
							textBoxResult = input.getText().toString();
							OFAndroid.okPressed();
							//handler.sendMessage(handler.obtainMessage());
						}
	
				  	})  
				  	.setNegativeButton(android.R.string.cancel,
							new DialogInterface.OnClickListener() {  
						public void onClick(DialogInterface dialog, int whichButton){
							OFAndroid.cancelPressed();
							//handler.sendMessage(handler.obtainMessage());
						}
				  	})
				  	.setView(input)
				  	.show();  
			}  
		});
		
		// loop till a runtime exception is triggered.
	    /*try { Looper.loop(); }
	    catch(RuntimeException e2) {}*/
	    //alert.dismiss();
	}
	
	public static String getLastTextBoxResult(){
		return textBoxResult;
	}
	
	private static boolean alertListResult;
	public static boolean alertListBox(String title, String[] list){  
		final String[] alertList = list;
		final String alertTitle = title;
		/*Looper.prepare();
		final Handler handler = new Handler() {
	        @Override
	        public void handleMessage(Message mesg) {
	            throw new RuntimeException();
	        } 
	    };*/
	    alertListResult=false;
		ofActivity.runOnUiThread(new Runnable(){
			public void run() {
				final ListView listView = new ListView(ofActivity); 
				final ListAdapter adapter = new ArrayAdapter<String>(ofActivity, android.R.layout.simple_list_item_1, alertList);
				listView.setAdapter(adapter);
				new AlertDialog.Builder(ofActivity)   
					.setTitle(alertTitle)  
					.setCancelable(false)  
					.setNeutralButton(android.R.string.ok,  
							new DialogInterface.OnClickListener() {  
						public void onClick(DialogInterface dialog, int whichButton){
							alertListResult = true;
							//OFAndroid.okPressed();
							//handler.sendMessage(handler.obtainMessage());
						}
	
				  	})  
				  	/*.setNegativeButton(android.R.string.cancel,
							new DialogInterface.OnClickListener() {  
						public void onClick(DialogInterface dialog, int whichButton){
							alertListResult = false;
							OFAndroid.cancelPressed();
							//handler.sendMessage(handler.obtainMessage());
						}
				  	})*/
				  	.setView(listView)
				  	.show();  
			}  
		});
		
		// loop till a runtime exception is triggered.
	    //try { Looper.loop(); }
	    //catch(RuntimeException e2) {}
	    
	    return alertListResult;
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
	
	static boolean sleepLocked=false;
	
	public static void lockScreenSleep(){
		if(!sleepLocked){
			ofActivity.runOnUiThread(new Runnable() {
				
				@Override
				public void run() {
					try{
						sleepLocked=true;
						ofActivity.getWindow().addFlags(LayoutParams.FLAG_KEEP_SCREEN_ON);
					}catch(Exception e){
						
					}
					
				}
			});
		}
	}
	
	public static void unlockScreenSleep(){
		if(sleepLocked){
			ofActivity.runOnUiThread(new Runnable() {
			
				@Override
				public void run() {
					try{
						sleepLocked=false;
				        ofActivity.getWindow().clearFlags(LayoutParams.FLAG_KEEP_SCREEN_ON);
					}catch(Exception e){
						
					}
					
				}
			});
		}
	}
	
	public static String getRandomUUID(){
		return UUID.randomUUID().toString();
	}
	
    
    private static OFGLSurfaceView mGLView;
    private static OFAndroidAccelerometer accelerometer;
    private static OFAndroidGPS gps;
    private static OFActivity ofActivity;
    private OFGestureListener gestureListener;
	private static String packageName;
	private static String dataPath;
	public static boolean unpackingDone;

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



	public static SurfaceView getGLContentView() {
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
	
	/**
	 * 
	 * @param keyCode
	 * @param event
	 * @return true to say we handled this, false to tell Android to handle it
	 */
	public static boolean keyDown(int keyCode, KeyEvent event) {
        if ((keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0)) {
            if( onBackPressed() ) {
            	return true;
           	} else {
           		// let the Android system handle the back button
           		return false;
           	}
        }
		
        if (KeyEvent.isModifierKey(keyCode)) {
        	/* Android sends a shift keycode (for instance),
        	   then the key that goes with the shift. We don't need the first
        	   keycode, that info is in event.getMetaState() anyway */
        	return false;
        }
        else
        {
        	int unicodeChar = event.getUnicodeChar();
        	onKeyDown(unicodeChar);

        	// return false to let Android handle certain keys
    		// like the back and menu keys
        	return false;
        }
	}
	
	/**
	 * 
	 * @param keyCode
	 * @param event
	 * @return true to say we handled this, false to tell Android to handle it
	 */
	public static boolean keyUp(int keyCode, KeyEvent event) {
        if (KeyEvent.isModifierKey(keyCode)) {
        	/* Android sends a shift keycode (for instance),
        	   then the key that goes with the shift. We don't need the first
        	   keycode, that info is in event.getMetaState() anyway */
        	return false;
        }
        else
        {
    		int unicodeChar = event.getUnicodeChar();
    		onKeyUp(unicodeChar);
    		
    		// return false to let Android handle certain keys
    		// like the back and menu keys
        	return false;
        }
	}
}

class OFGestureListener extends SimpleOnGestureListener implements OnClickListener {
	

	OFGestureListener(Activity activity){
		gestureDetector = new GestureDetector(activity,this);
        touchListener = new View.OnTouchListener() {
        	
            public boolean onTouch(View v, MotionEvent event) {
            	final int action = event.getAction();
            	final int pointerIndex = (action & MotionEvent.ACTION_POINTER_INDEX_MASK) 
                >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
                final int pointerId = event.getPointerId(pointerIndex);
                switch((action & MotionEvent.ACTION_MASK)){
                case MotionEvent.ACTION_MOVE:
                {
            		for(int i=0; i<event.getHistorySize(); i++)
            		{            			
		                for(int j=0; j<event.getPointerCount(); j++){	                			
	                		OFAndroid.onTouchMoved(event.getPointerId(j), event.getHistoricalX(j, i), event.getHistoricalY(j, i), event.getHistoricalPressure(j, i));
	                	}            			
                	}
	            	for(int i=0; i<event.getPointerCount(); i++){
	            		OFAndroid.onTouchMoved(event.getPointerId(i), event.getX(i), event.getY(i), event.getPressure(i));
	            	}
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
                	OFAndroid.onTouchCancelled(pointerId,event.getX(),event.getY());
                	break;
                }
                return gestureDetector.onTouchEvent(event);
            }
            
        };
	}
	
	public void onClick(View view) {
	}

	@Override
	public boolean onDoubleTap(MotionEvent event) {
		final int action = event.getAction();
		final int pointerIndex = (action & MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
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
	public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
		/*boolean res = super.onFling(e1, e2, velocityX, velocityY);
		return res;*/
		
		final float xDistance = Math.abs(e1.getX() - e2.getX());
		final float yDistance = Math.abs(e1.getY() - e2.getY());

		if(xDistance > OFGestureListener.swipe_Max_Distance || yDistance > OFGestureListener.swipe_Max_Distance)
			return false;

		velocityX = Math.abs(velocityX);
		velocityY = Math.abs(velocityY);
        boolean result = false;

        if(velocityX > OFGestureListener.swipe_Min_Velocity && xDistance > OFGestureListener.swipe_Min_Distance){
        	if(e1.getX() > e2.getX()) // right to left
        		OFAndroid.onSwipe(e1.getPointerId(0),SWIPE_LEFT);
        	else
        		OFAndroid.onSwipe(e1.getPointerId(0),SWIPE_RIGHT);
   
        	result = true;
        }else if(velocityY > OFGestureListener.swipe_Min_Velocity && yDistance > OFGestureListener.swipe_Min_Distance){
        	if(e1.getY() > e2.getY()) // bottom to up 
        		OFAndroid.onSwipe(e1.getPointerId(0),SWIPE_UP);
        	else
        		OFAndroid.onSwipe(e1.getPointerId(0),SWIPE_DOWN);
   
        	result = true;
        }

        return result;
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

    private GestureDetector gestureDetector;
    View.OnTouchListener touchListener;
    public static int swipe_Min_Distance = 100;
    public static int swipe_Max_Distance = 350;
    public static int swipe_Min_Velocity = 100;
    public final static int SWIPE_UP    = 1;
    public final static int SWIPE_DOWN  = 2;
    public final static int SWIPE_LEFT  = 3;
    public final static int SWIPE_RIGHT = 4;
}

/*class OFEGLConfigChooser implements GLSurfaceView.EGLConfigChooser{
	static int DEPTH_SIZE=16,STENCIL_SIZE=0,EGL_GLES2_BIT=1;
	
	private static final int[] EGLCONFIG_ATTRIBUTES_8888 = new int[] {
		EGL10.EGL_RED_SIZE, 8,
		EGL10.EGL_GREEN_SIZE, 8,
		EGL10.EGL_BLUE_SIZE, 8,
		EGL10.EGL_ALPHA_SIZE, 8,
		EGL10.EGL_DEPTH_SIZE, OFEGLConfigChooser.DEPTH_SIZE,
		EGL10.EGL_STENCIL_SIZE, OFEGLConfigChooser.STENCIL_SIZE,
		EGL10.EGL_RENDERABLE_TYPE, OFEGLConfigChooser.EGL_GLES2_BIT,
		EGL10.EGL_NONE
	};
	
	private static final int[] EGLCONFIG_ATTRIBUTES_FALLBACK = new int[] {
		EGL10.EGL_RED_SIZE, 5,
		EGL10.EGL_GREEN_SIZE, 6,
		EGL10.EGL_BLUE_SIZE, 5,
		EGL10.EGL_ALPHA_SIZE, 0,
		EGL10.EGL_DEPTH_SIZE, 0,
		EGL10.EGL_STENCIL_SIZE, 0,
		EGL10.EGL_RENDERABLE_TYPE, 1,
		EGL10.EGL_NONE
	};
	
	private boolean is24bit=false;
	
	boolean is24Bit(){
		return is24bit;
	}
	
	@Override
	public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {
		int[] buffer = new int[1];
		int[] attributes;
		if(egl.eglChooseConfig(display,EGLCONFIG_ATTRIBUTES_8888,null,0,buffer)==false){
			egl.eglChooseConfig(display,EGLCONFIG_ATTRIBUTES_FALLBACK,null,0,buffer);
			Log.w("OF", "couldn't create requested EGL config, defaulting to RGB565 fallback");
			attributes = EGLCONFIG_ATTRIBUTES_FALLBACK;
			is24bit = false;
		}else{
			attributes = EGLCONFIG_ATTRIBUTES_8888;
			is24bit = true;
		}
		int configCount = buffer[0];
		final EGLConfig[] eglConfigs = new EGLConfig[configCount];
		egl.eglChooseConfig(display,attributes,eglConfigs,configCount,buffer);
		return findEGLConfig(egl, display, eglConfigs, attributes);
	}	

	private int getConfigAttrib(final EGL10 pEGL, final EGLDisplay pEGLDisplay, final EGLConfig pEGLConfig, final int pAttribute, final int pDefaultValue) {
		int[] buffer = new int[1];
		if(pEGL.eglGetConfigAttrib(pEGLDisplay, pEGLConfig, pAttribute, buffer)) {
			return buffer[0];
		} else {
			return pDefaultValue;
		}
	}
	
	private boolean match(int redSize, int greenSize, int blueSize, int alphaSize, int depthSize, int stencilSize, int [] attributes){
		return redSize==attributes[0] && greenSize==attributes[1] && blueSize==attributes[2] && alphaSize==attributes[3] &&
				depthSize==attributes[4] && stencilSize==attributes[5];
	}
	
	private EGLConfig findEGLConfig(final EGL10 pEGL, final EGLDisplay pEGLDisplay, final EGLConfig[] pEGLConfigs, int[] attributes) {
		for(int i = 0; i < pEGLConfigs.length; ++i) {
			final EGLConfig config = pEGLConfigs[i];
			if(config != null) {
				final int redSize = this.getConfigAttrib(pEGL, pEGLDisplay, config, EGL10.EGL_RED_SIZE, 0);
				final int greenSize = this.getConfigAttrib(pEGL, pEGLDisplay, config, EGL10.EGL_GREEN_SIZE, 0);
				final int blueSize = this.getConfigAttrib(pEGL, pEGLDisplay, config, EGL10.EGL_BLUE_SIZE, 0);
				final int alphaSize = this.getConfigAttrib(pEGL, pEGLDisplay, config, EGL10.EGL_ALPHA_SIZE, 0);
				final int depthSize = this.getConfigAttrib(pEGL, pEGLDisplay, config, EGL10.EGL_DEPTH_SIZE, 0);
				final int stencilSize = this.getConfigAttrib(pEGL, pEGLDisplay, config, EGL10.EGL_STENCIL_SIZE, 0);
				
				if(match(redSize, greenSize, blueSize, alphaSize, depthSize, stencilSize,attributes)) {
					return config;
				}
			}
		}
		return pEGLConfigs[0];
	}
}*/

class OFEGLConfigChooser implements GLSurfaceView.EGLConfigChooser {
	
    public OFEGLConfigChooser(int r, int g, int b, int a, int depth, int stencil) {
        mRedSize = r;
        mGreenSize = g;
        mBlueSize = b;
        mAlphaSize = a;
        mDepthSize = depth;
        mStencilSize = stencil;
    }

    /* This EGL config specification is used to specify 1.x rendering.
     * We use a minimum size of 4 bits for red/green/blue, but will
     * perform actual matching in chooseConfig() below.
     */
    private static boolean DEBUG = false;
    private static int EGL_OPENGL_ES_BIT = 1;
    private static int[] s_configAttribs2 =
    {
        EGL10.EGL_RED_SIZE, 4,
        EGL10.EGL_GREEN_SIZE, 4,
        EGL10.EGL_BLUE_SIZE, 4,
        EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
        EGL10.EGL_NONE
    };

    public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {

        /* Get the number of minimally matching EGL configurations
         */
        int[] num_config = new int[1];
        egl.eglChooseConfig(display, s_configAttribs2, null, 0, num_config);

        int numConfigs = num_config[0];

        if (numConfigs <= 0) {
            throw new IllegalArgumentException("No configs match configSpec");
        }

        /* Allocate then read the array of minimally matching EGL configs
         */
        EGLConfig[] configs = new EGLConfig[numConfigs];
        egl.eglChooseConfig(display, s_configAttribs2, configs, numConfigs, num_config);

        if (DEBUG) {
             printConfigs(egl, display, configs);
        }
        /* Now return the "best" one
         */
        return chooseConfig(egl, display, configs);
    }

    public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display,
            EGLConfig[] configs) {
        for(EGLConfig config : configs) {
            int d = findConfigAttrib(egl, display, config,
                    EGL10.EGL_DEPTH_SIZE, 0);
            int s = findConfigAttrib(egl, display, config,
                    EGL10.EGL_STENCIL_SIZE, 0);

            // We need at least mDepthSize and mStencilSize bits
            if (d < mDepthSize || s < mStencilSize)
                continue;

            // We want an *exact* match for red/green/blue/alpha
            int r = findConfigAttrib(egl, display, config,
                    EGL10.EGL_RED_SIZE, 0);
            int g = findConfigAttrib(egl, display, config,
                        EGL10.EGL_GREEN_SIZE, 0);
            int b = findConfigAttrib(egl, display, config,
                        EGL10.EGL_BLUE_SIZE, 0);
            int a = findConfigAttrib(egl, display, config,
                    EGL10.EGL_ALPHA_SIZE, 0);

            if (r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize)
                return config;
        }
        return null;
    }

    private int findConfigAttrib(EGL10 egl, EGLDisplay display,
            EGLConfig config, int attribute, int defaultValue) {

        if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) {
            return mValue[0];
        }
        return defaultValue;
    }

    private void printConfigs(EGL10 egl, EGLDisplay display,
        EGLConfig[] configs) {
        int numConfigs = configs.length;
        Log.w("OF", String.format("%d configurations", numConfigs));
        for (int i = 0; i < numConfigs; i++) {
            Log.w("OF", String.format("Configuration %d:\n", i));
            printConfig(egl, display, configs[i]);
        }
    }

    private void printConfig(EGL10 egl, EGLDisplay display,
            EGLConfig config) {
        int[] attributes = {
                EGL10.EGL_BUFFER_SIZE,
                EGL10.EGL_ALPHA_SIZE,
                EGL10.EGL_BLUE_SIZE,
                EGL10.EGL_GREEN_SIZE,
                EGL10.EGL_RED_SIZE,
                EGL10.EGL_DEPTH_SIZE,
                EGL10.EGL_STENCIL_SIZE,
                EGL10.EGL_CONFIG_CAVEAT,
                EGL10.EGL_CONFIG_ID,
                EGL10.EGL_LEVEL,
                EGL10.EGL_MAX_PBUFFER_HEIGHT,
                EGL10.EGL_MAX_PBUFFER_PIXELS,
                EGL10.EGL_MAX_PBUFFER_WIDTH,
                EGL10.EGL_NATIVE_RENDERABLE,
                EGL10.EGL_NATIVE_VISUAL_ID,
                EGL10.EGL_NATIVE_VISUAL_TYPE,
                0x3030, // EGL10.EGL_PRESERVED_RESOURCES,
                EGL10.EGL_SAMPLES,
                EGL10.EGL_SAMPLE_BUFFERS,
                EGL10.EGL_SURFACE_TYPE,
                EGL10.EGL_TRANSPARENT_TYPE,
                EGL10.EGL_TRANSPARENT_RED_VALUE,
                EGL10.EGL_TRANSPARENT_GREEN_VALUE,
                EGL10.EGL_TRANSPARENT_BLUE_VALUE,
                0x3039, // EGL10.EGL_BIND_TO_TEXTURE_RGB,
                0x303A, // EGL10.EGL_BIND_TO_TEXTURE_RGBA,
                0x303B, // EGL10.EGL_MIN_SWAP_INTERVAL,
                0x303C, // EGL10.EGL_MAX_SWAP_INTERVAL,
                EGL10.EGL_LUMINANCE_SIZE,
                EGL10.EGL_ALPHA_MASK_SIZE,
                EGL10.EGL_COLOR_BUFFER_TYPE,
                EGL10.EGL_RENDERABLE_TYPE,
                0x3042 // EGL10.EGL_CONFORMANT
        };
        String[] names = {
                "EGL_BUFFER_SIZE",
                "EGL_ALPHA_SIZE",
                "EGL_BLUE_SIZE",
                "EGL_GREEN_SIZE",
                "EGL_RED_SIZE",
                "EGL_DEPTH_SIZE",
                "EGL_STENCIL_SIZE",
                "EGL_CONFIG_CAVEAT",
                "EGL_CONFIG_ID",
                "EGL_LEVEL",
                "EGL_MAX_PBUFFER_HEIGHT",
                "EGL_MAX_PBUFFER_PIXELS",
                "EGL_MAX_PBUFFER_WIDTH",
                "EGL_NATIVE_RENDERABLE",
                "EGL_NATIVE_VISUAL_ID",
                "EGL_NATIVE_VISUAL_TYPE",
                "EGL_PRESERVED_RESOURCES",
                "EGL_SAMPLES",
                "EGL_SAMPLE_BUFFERS",
                "EGL_SURFACE_TYPE",
                "EGL_TRANSPARENT_TYPE",
                "EGL_TRANSPARENT_RED_VALUE",
                "EGL_TRANSPARENT_GREEN_VALUE",
                "EGL_TRANSPARENT_BLUE_VALUE",
                "EGL_BIND_TO_TEXTURE_RGB",
                "EGL_BIND_TO_TEXTURE_RGBA",
                "EGL_MIN_SWAP_INTERVAL",
                "EGL_MAX_SWAP_INTERVAL",
                "EGL_LUMINANCE_SIZE",
                "EGL_ALPHA_MASK_SIZE",
                "EGL_COLOR_BUFFER_TYPE",
                "EGL_RENDERABLE_TYPE",
                "EGL_CONFORMANT"
        };
        int[] value = new int[1];
        for (int i = 0; i < attributes.length; i++) {
            int attribute = attributes[i];
            String name = names[i];
            if ( egl.eglGetConfigAttrib(display, config, attribute, value)) {
                Log.w("OF", String.format("  %s: %d\n", name, value[0]));
            } else {
                // Log.w(TAG, String.format("  %s: failed\n", name));
                while (egl.eglGetError() != EGL10.EGL_SUCCESS);
            }
        }
    }

    // Subclasses can adjust these values:
    protected int mRedSize;
    protected int mGreenSize;
    protected int mBlueSize;
    protected int mAlphaSize;
    protected int mDepthSize;
    protected int mStencilSize;
    private int[] mValue = new int[1];
}

class OFGLSurfaceView extends GLSurfaceView{
	public OFGLSurfaceView(Context context) {
        super(context);
        mRenderer = new OFAndroidWindow(getWidth(),getHeight());
        getHolder().setFormat( PixelFormat.RGBA_8888 );
        OFEGLConfigChooser configChooser = new OFEGLConfigChooser(8,8,8,8,16,0);
        setEGLConfigChooser(configChooser);
        setRenderer(mRenderer);
    }
	
	public OFGLSurfaceView(Context context,AttributeSet attributes) {
        super(context,attributes);
        mRenderer = new OFAndroidWindow(getWidth(),getHeight());
        getHolder().setFormat( PixelFormat.RGBA_8888 );
        OFEGLConfigChooser configChooser = new OFEGLConfigChooser(8,8,8,8,16,0);
        setEGLConfigChooser(configChooser);
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
    		try{
    			((OFActivity)OFAndroid.getContext()).onGLSurfaceCreated();
    		}catch(Exception e){
    			Log.e("OF","couldn call onGLSurfaceCreated",e);
    		}
    		return;
    	}
    	
    }

    public void onSurfaceChanged(GL10 gl, int w, int h) {
		this.w = w;
		this.h = h;
    	if(!setup && OFAndroid.unpackingDone){
        	setup();
    	}
    	OFGestureListener.swipe_Min_Distance = (int)(Math.max(w, h)*.04);
    	OFGestureListener.swipe_Max_Distance = (int)(Math.max(w, h)*.6);
    	OFAndroid.resize(w, h);
    }
    
    private void setup(){
    	Log.i("OF","initializing app");
    	OFAndroid.init();
    	OFAndroid.setup(w,h);
    	initialized = true;
    	setup = true;
    	android.os.Process.setThreadPriority(8);
    	OFGestureListener.swipe_Min_Distance = (int)(Math.max(w, h)*.04);
    	OFGestureListener.swipe_Max_Distance = (int)(Math.max(w, h)*.6);
		try{
			((OFActivity)OFAndroid.getContext()).onGLSurfaceCreated();
		}catch(Exception e){
			Log.e("OF","couldn call onGLSurfaceCreated",e);
		}
    	
    	/*if(ETC1Util.isETC1Supported()) Log.i("OF","ETC supported");
    	else Log.i("OF","ETC not supported");*/
    }

    public void onDrawFrame(GL10 gl) {
    	if(setup && OFAndroid.unpackingDone)
    		OFAndroid.render();
    	else if(!setup && OFAndroid.unpackingDone)
    		setup();
    }

    static boolean initialized;
    static boolean setup;
    int w,h;
}