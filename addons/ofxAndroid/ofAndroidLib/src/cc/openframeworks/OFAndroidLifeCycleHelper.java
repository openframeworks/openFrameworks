package cc.openframeworks;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Field;
import java.util.Locale;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.StatFs;
import android.util.Log;

public class OFAndroidLifeCycleHelper 
{
	private static final String TAG = OFAndroidLifeCycleHelper.class.getSimpleName();
	private static boolean appInitFlag = false;
	private static boolean resumed;
	
	public static void appInit(Activity activity)
	{
		if(appInitFlag)
			return;
		appInitFlag = true;
		
		String packageName = activity.getPackageName();
		
		Log.i(TAG,"starting resources extractor");
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

	        SharedPreferences preferences = activity.getPreferences(Context.MODE_PRIVATE);
	        long lastInstalled = preferences.getLong("installed", 0);
	        
	        PackageManager pm = activity.getPackageManager();

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
    	
        
        OFAndroid.reportPrecentage(.05f);

        String dataPath="";
		try{
			Log.i(TAG, "sd mounted: " + OFAndroid.checkSDCardMounted());
			OFAndroid.initAppDataDirectory(activity);
			dataPath = OFAndroid.getAppDataDirectory();

			Log.i(TAG,"creating app directory: " + dataPath);
			try{
				File dir = new File(dataPath);
				if(!(dir.mkdirs() || dir.isDirectory())){
					if(copydata){
						OFAndroid.fatalErrorDialog(activity, "Error while copying resources to sdcard:\nCouldn't create directory " + dataPath);
						Log.e(TAG,"error creating dir " + dataPath);
						return;
					}else{
						throw new Exception();
					}
				}
			}catch(Exception e){
				OFAndroid.fatalErrorDialog(activity, "Error while copying resources to sdcard:\nCouldn't create directory " + dataPath + "\n"+e.getMessage());
				Log.e(TAG,"error creating dir " + dataPath,e);
			}
			OFAndroid.moveOldData(OFAndroid.getOldExternalStorageDirectory(packageName), dataPath);
			OFAndroid.setAppDataDir(dataPath);
	        OFAndroid.reportPrecentage(.10f);
		}catch(Exception e){
			Log.e(TAG,"couldn't move app resources to data directory " + dataPath,e);
		}
		
		
		String app_name="";
		try {
			int app_name_id = Class.forName(packageName+".R$string").getField("app_name").getInt(null);
			app_name = activity.getResources().getText(app_name_id).toString().toLowerCase(Locale.US);
			Log.i(TAG,"app name: " + app_name);
			
			if(copydata){
				StatFs stat = new StatFs(dataPath);
				double sdAvailSize = (double)stat.getAvailableBlocks()
		                   * (double)stat.getBlockSize();
				for(int i=0; i<files.length; i++){
    	        	int fileId;
    	        	String fileName="";
    				
    				InputStream from=null;
    				FileOutputStream to=null;
    	        	try {
    					fileId = files[i].getInt(null);
    					String resName = activity.getResources().getText(fileId).toString();
    					fileName = resName.substring(resName.lastIndexOf("/"));
    					Log.i(TAG,"checking " + fileName);
    					if(fileName.equals("/ofdataresources.zip")){
    						
	    					from = activity.getResources().openRawResource(fileId);
							try{
								ZipInputStream resourceszip = new ZipInputStream(from);
								int totalZipSize = 0;
								ZipEntry entry;
								File outdir = new File(dataPath);
								while ((entry = resourceszip.getNextEntry()) != null){
									totalZipSize+=entry.getSize();
								}
								resourceszip.close();
								Log.i(TAG,"size of uncompressed resources: " + totalZipSize + " avaliable space:" + sdAvailSize);
								if(totalZipSize>=sdAvailSize){
									final int mbsize = totalZipSize/1024/1024;
									OFAndroid.fatalErrorDialog(activity, "Error while copying resources to sdcard:\nNot enough space available.("+mbsize+"Mb)\nMake more space by deleting some file in your sdcard");
								}else{
									from = activity.getResources().openRawResource(fileId);
									resourceszip = new ZipInputStream(from);
									

									while ((entry = resourceszip.getNextEntry()) != null){
										String name = entry.getName();
								        if( entry.isDirectory() )
								        {
								        	OFZipUtil.mkdirs(outdir,name);
								          continue;
								        }
								        String dir = OFZipUtil.dirpart(name);
								        if( dir != null )
								        	OFZipUtil.mkdirs(outdir,dir);

								        OFZipUtil.extractFile(resourceszip, outdir, name);
								        OFAndroid.reportPrecentage((float)(.10+i*.01));
									}

									resourceszip.close();
							        OFAndroid.reportPrecentage(.80f);
								}
							}catch(Exception e){
								OFAndroid.fatalErrorDialog(activity, "Error while copying resources to sdcard:\nCheck that you have enough space available.\n");
							}
    					}else{
							Log.i(TAG, "Resources file not found");
						}
    	        	}catch (Exception e) {
    					Log.e(TAG,"error copying file",e);
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
			}else{
		        OFAndroid.reportPrecentage(.80f);
			}
		} catch (Exception e) {
			Log.e(TAG,"error retrieving app name",e);
		} 	
		OFAndroid.init();

	}
	
	public static void onCreate()
	{
		OFAndroid.onCreate();
		OFAndroid.onUnpackingResourcesDone();
	}
	
	public static void onStart(){
		Log.i(TAG,"onStart");
		OFAndroid.runOnMainThread(new Runnable() {
			
			@Override
			public void run() {
				OFAndroid.enableTouchEvents();
				OFAndroid.enableOrientationChangeEvents();
			}
		});
	}
	
	public static void onRestart(){
		Log.i(TAG,"onRestart");
		OFAndroid.runOnMainThread(new Runnable() {
			
			@Override
			public void run() {
				OFAndroid.enableTouchEvents();
				OFAndroid.enableOrientationChangeEvents();
			}
		});
		OFAndroid.onRestart();
        /*if(OFAndroidSoundStream.isInitialized() && OFAndroidSoundStream.wasStarted())
        	OFAndroidSoundStream.getInstance().start();*/
	}
	
	public static void onResume(){
		final OFGLSurfaceView glView = OFAndroidLifeCycle.getGLView();
		if(resumed) return;
		Log.i(TAG,"onResume");
		resumed = true;
		OFAndroid.runOnMainThread(new Runnable() {
			
			@Override
			public void run() {
				OFAndroid.enableTouchEvents();
				OFAndroid.enableOrientationChangeEvents();
				glView.onResume();
				synchronized (OFAndroidObject.ofObjects) {
					for(OFAndroidObject object : OFAndroidObject.ofObjects){
						object.onResume();
					}
					
				}
			}
		});
		
        if(glView.isSetup()){
        	Log.i(TAG,"resume view and native");
        	OFAndroid.onResume();
        }
        
        if(OFAndroid.getOrientation()!=-1) OFAndroid.setScreenOrientation(OFAndroid.getOrientation());
		
		OFAndroid.registerNetworkStateReceiver();
	}
	
	public static void onPause(){
		Log.i(TAG,"onPause");
		OFAndroid.runOnMainThread(new Runnable() {
			
			@Override
			public void run() {
				OFAndroid.disableTouchEvents();
				OFAndroid.disableOrientationChangeEvents();

				synchronized (OFAndroidObject.ofObjects) {
					for(OFAndroidObject object : OFAndroidObject.ofObjects){
						object.onPause();
					}
				}
				OFAndroidLifeCycle.getGLView().onPause();
			}
		});
		
		OFAndroid.onPause();
		OFAndroid.unregisterNetworkStateReceiver();

		OFAndroid.sleepLocked=false;
		resumed = false;
	}
	
	public static void onStop(){
		resumed = false;
		Log.i(TAG,"onStop");
		
		OFAndroid.runOnMainThread(new Runnable() {
			
			@Override
			public void run() {
				OFAndroid.disableTouchEvents();
				OFAndroid.disableOrientationChangeEvents();

				synchronized (OFAndroidObject.ofObjects) {
					for(OFAndroidObject object : OFAndroidObject.ofObjects){
						object.onStop();
					}
				}
			}
		});
		OFAndroid.onStop();
		OFAndroid.unregisterNetworkStateReceiver();
		
		OFAndroid.sleepLocked=false;
	}
	
	public static void onDestroy(){
		Log.i(TAG,"onDestroy");
		OFAndroid.onDestroy();
		OFAndroidWindow.exit();
	}
	
	public static void exit()
	{
		OFAndroid.exit();
	}
}
