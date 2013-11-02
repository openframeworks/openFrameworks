package cc.openframeworks;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Field;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;
import java.util.UUID;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

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
import android.hardware.SensorManager;
import android.media.AudioManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiManager.MulticastLock;
import android.os.Environment;
import android.os.StatFs;
import android.util.Log;
import android.view.KeyEvent;
import android.view.SurfaceView;
import android.view.View;
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
		Log.i("OF","OFAndorid init...");
		OFAndroid.ofActivity = activity;
		ofActivity.setVolumeControlStream(AudioManager.STREAM_MUSIC);
		//Log.i("OF","external files dir: "+ ofActivity.getApplicationContext().getExternalFilesDir(null));
		OFAndroid.packageName = appPackageName;
		OFAndroidObject.setActivity(ofActivity);
		unpackingDone = false;
		
		new Thread(new Runnable(){
			@Override
			public void run() {
				Class<?> raw = null;
		        boolean copydata = false;
		        Field[] files = null;
		        try {
		        	
					// try to find if R.raw class exists will throw
		        	// an exception if not
		        	raw = Class.forName(packageName+".R$raw");
		        	// if it exists copy all the raw resources
		        	// to a folder in the sdcard
			        files = raw.getDeclaredFields(); 
		
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
				} catch (NameNotFoundException e1) {
					copydata = false;
		        } catch (ClassNotFoundException e1) { 
		        } 
	        	
		        
		        ofActivity.onLoadPercent(.05f);
	
		        dataPath="";
	    		try{
	    			Log.i("OF", "sd mounted: " + checkSDCardMounted());
					dataPath = getRealExternalStorageDirectory();
					dataPath += "/Android/data/"+packageName;

	    			Log.i("OF","creating app directory: " + dataPath);
					try{
						File dir = new File(dataPath);
						if(!dir.exists() && dir.mkdirs()!=true){
							if(copydata){
								fatalErrorDialog("Error while copying resources to sdcard:\nCouldn't create directory " + dataPath);
								return;
							}else{
								throw new Exception();
							}
						}
					}catch(Exception e){
						fatalErrorDialog("Error while copying resources to sdcard:\nCouldn't create directory " + dataPath + "\n"+e.getMessage());
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
		    					Log.i("OF","copying file " + fileName + " to " + dataPath);
		    					toFile = new File(dataPath + "/" + fileName);
		    					to = new FileOutputStream(toFile);
		    					byte[] buffer = new byte[4096];
		    					int bytesRead;
		    					
		    					while ((bytesRead = from.read(buffer)) != -1)
		    					    to.write(buffer, 0, bytesRead); // write
		    	        	}catch (Exception e) {
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
	    			Log.e("OF","couldn't move app resources to data directory " + dataPath,e);
	    		}
	    		String app_name="";
				try {
					int app_name_id = Class.forName(packageName+".R$string").getField("app_name").getInt(null);
					app_name = ofActivity.getResources().getText(app_name_id).toString();
					Log.i("OF","app name: " + app_name);
					
					if(copydata){
						StatFs stat = new StatFs(getRealExternalStorageDirectory());
						double sdAvailSize = (double)stat.getAvailableBlocks()
				                   * (double)stat.getBlockSize();
						
						try{
							ZipFile resourceszip = new ZipFile(dataPath+"/"+app_name+"resources.zip");
							Enumeration entries=resourceszip.entries();
							int totalZipSize = 0;
							while(entries.hasMoreElements()){
								ZipEntry zipentry = (ZipEntry)entries.nextElement();
								totalZipSize+=zipentry.getSize();
							}
							resourceszip.close();
							Log.i("OF","size of uncompressed resources: " + totalZipSize + " avaliable space:" + sdAvailSize);
							if(totalZipSize>=sdAvailSize){
								final int mbsize = totalZipSize/1024/1024;
								fatalErrorDialog("Error while copying resources to sdcard:\nNot enough space available.("+mbsize+"Mb)\nMake more space by deleting some file in your sdcard");
							}else{
								OFAndroid.setAppDataDir(dataPath,app_name);
						        OFAndroid.onUnpackingResourcesDone();
						        ofActivity.onLoadPercent(.80f);
							}
						}catch(Exception e){
							fatalErrorDialog("Error while copying resources to sdcard:\nCheck that you have enough space available.\n");
						}
					}else{
						OFAndroid.setAppDataDir(dataPath,app_name);
				        OFAndroid.onUnpackingResourcesDone();
				        ofActivity.onLoadPercent(.80f);
					}
				} catch (Exception e) {
					Log.e("OF","error retrieving app name",e);
				} 	
			}
		}).start();
		

    	OFAndroid.init();
    	
    }

	private void fatalErrorDialog(final String msg){
		ofActivity.runOnUiThread(new Runnable(){
			public void run() {
				new AlertDialog.Builder(ofActivity)  
					.setMessage(msg)  
					.setTitle("")  
					.setCancelable(false)  
					.setNeutralButton(android.R.string.ok,  
							new DialogInterface.OnClickListener() {  
						public void onClick(DialogInterface dialog, int whichButton){
							ofActivity.finish();
						}
	
				  	})  
				  	.show();
			}  
		});
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

		if(networkStateReceiver!=null){
			try{
				ofActivity.unregisterReceiver(networkStateReceiver);
			}catch(java.lang.IllegalArgumentException e){
				
			}
		}

		sleepLocked=false;
	}
	
	public void resume(){
		Log.i("OF","onResume");
		enableTouchEvents();
        mGLView.onResume();
		
		for(OFAndroidObject object : OFAndroidObject.ofObjects){
			object.onResume();
		}
		
        if(mGLView.isSetup()) onResume();
        
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
		

		if(networkStateReceiver!=null){
			try{
				ofActivity.unregisterReceiver(networkStateReceiver);
			}catch(java.lang.IllegalArgumentException e){
				
			}
		}
		
		sleepLocked=false;
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
    private static OFGestureListener gestureListener;
	private static String packageName;
	private static String dataPath;
	public static boolean unpackingDone;

    public static native boolean hasNeon();
	 
    static {
    	try{
    		Log.i("OF","static init");
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
    	Log.i("OF","initializing app");
    }



	public static SurfaceView getGLContentView() {
        return mGLView;
	}
	
	public static void disableTouchEvents(){
        mGLView.setOnClickListener(null); 
        mGLView.setOnTouchListener(null);
	}
	
	public static void enableTouchEvents(){
        mGLView.setOnClickListener(gestureListener); 
        mGLView.setOnTouchListener(gestureListener.touchListener);
	}
	
	public static void setupGL(int version){		
		gestureListener = new OFGestureListener(ofActivity);
        OFEGLConfigChooser.setGLESVersion(version);
        
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

