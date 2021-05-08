package cc.openframeworks;

import java.io.File;
import java.io.FileNotFoundException;
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
import android.content.res.AssetManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.StatFs;
import android.util.Log;

public class OFAndroidLifeCycleHelper 
{
	private static final String TAG = OFAndroidLifeCycleHelper.class.getSimpleName();
	private static boolean appInitFlag = false;
	private static boolean started;

	public static void appInit(Activity activity)
	{
		if(appInitFlag)
			return;
		appInitFlag = true;
		
		copyAssetsToDataPath(activity);
		
		OFAndroid.init();
	}

	private static void copyAssetsToDataPath(Activity activity) {
		OFAndroid.packageName = activity.getPackageName();

		Log.i(TAG,"starting resources extractor");
		boolean copydata = false;
		String[] files = new String[0];
		AssetManager am = activity.getApplicationContext().getAssets();

		try {
			SharedPreferences preferences = activity.getPreferences(Context.MODE_PRIVATE);

			long lastInstalled = preferences.getLong("installed", 0);

			PackageManager pm = activity.getPackageManager();

			ApplicationInfo appInfo = pm.getApplicationInfo(OFAndroid.packageName, 0);

			String appFile = appInfo.sourceDir;
			long installed = new File(appFile).lastModified();
			if(installed>lastInstalled){
				Editor editor = preferences.edit();
				editor.putLong("installed", installed);
				editor.apply();
				copydata = true;
			}
			files = am.list("");

		} catch (NameNotFoundException e1) {
			copydata = false;
		} catch (IOException e) {
			e.printStackTrace();
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
				if(!dir.isDirectory()){
					copydata = true;
					if(!dir.mkdirs()){
						OFAndroid.fatalErrorDialog(activity, "Error while copying resources to sdcard:\nCouldn't create directory " + dataPath);
						Log.e(TAG,"error creating dir " + dataPath);
						return;
					}
				}
			} catch(Exception e){
				OFAndroid.fatalErrorDialog(activity, "Error while copying resources to sdcard:\nCouldn't create directory " + dataPath + "\n"+e.getMessage());
				Log.e(TAG,"error creating dir " + dataPath,e);
			}
			OFAndroid.moveOldData(OFAndroid.getOldExternalStorageDirectory(OFAndroid.packageName), dataPath);
			OFAndroid.setAppDataDir(dataPath);
			OFAndroid.reportPrecentage(.10f);
		} catch(Exception e){
			Log.e(TAG,"couldn't move app resources to data directory " + dataPath,e);
		}


		try {
			if(copydata){
				for (String file : files) {
					try {
						copyAssetFolder(am, file, dataPath+"/"+file);
					} catch (Exception e) {
						Log.e("OF", "error copying file", e);
					}

				}

			}else{
				OFAndroid.reportPrecentage(.80f);
			}
		} catch (Exception e) {
			Log.e(TAG,"error retrieving app name",e);
		}
	}

	private static void copyAssetFolder(AssetManager am, String src, String dest) throws IOException {

		Log.i("Copy ",src);
		InputStream srcIS = null;
		File destfh;

		// this is the only way we can tell if this is a file or a
		// folder - we have to open the asset, and if the open fails,
		// it's a folder...
		boolean isDir = false;
		try {
			srcIS = am.open(src);
		} catch (FileNotFoundException e) {
			isDir = true;
		}

		// either way, we'll use the dest as a File
		destfh = new File(dest);

		// and now, depending on ..
		if(isDir) {

			// If the directory doesn't yet exist, create it
			if( !destfh.exists() ){
				destfh.mkdir();
			}

			// list the assets in the directory...
			String assets[] = am.list(src);

			// and copy them all using same.
			for(String asset : assets) {
				copyAssetFolder(am, src + "/" + asset, dest + "/" + asset);
			}

		} else {
			int count, buffer_len = 2048;
			byte[] data = new byte[buffer_len];

			// copy the file from the assets subsystem to the filesystem
			FileOutputStream destOS = new FileOutputStream(destfh);

			//copy the file content in bytes
			while( (count = srcIS.read(data, 0, buffer_len)) != -1) {
				destOS.write(data, 0, count);
			}

			// and close the two files
			srcIS.close();
			destOS.close();
		}
	}


	
	public static void onCreate()
	{
		OFAndroid.onCreate();
		OFAndroid.onUnpackingResourcesDone();
		
		OFAndroid.runOnMainThread(new Runnable() {
		   @Override
		   public void run() {
				OFAndroid.enableTouchEvents();
				OFAndroid.enableOrientationChangeEvents();
			}
		});
	}
	
	public static void onResume(){
		OFAndroid.onResume();
	}
	
	public static void onPause(){
		OFAndroid.onPause();
	}
	
	public static void onStart(){
		Log.i(TAG,"onStart");
		
		final OFGLSurfaceView glView = OFAndroidLifeCycle.getGLView();
		if(started) return;
		started = true;
		
		OFAndroid.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				OFAndroid.enableTouchEvents();
				OFAndroid.enableOrientationChangeEvents();
				synchronized (OFAndroidObject.ofObjects) {
					for(OFAndroidObject object : OFAndroidObject.ofObjects){
						object.onResume();
					}
					
				}
			}
		});
		
        if(glView.isSetup()){
        	Log.i(TAG,"resume view and native");
        	OFAndroid.onStart();
        }
        
        if(OFAndroid.getOrientation()!=-1) OFAndroid.setScreenOrientation(OFAndroid.getOrientation());
		
		OFAndroid.registerNetworkStateReceiver();
	}
	
	public static void onStop(){
		Log.i(TAG,"onStop");
		
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
			}
		});
		
		OFAndroid.onStop();
		OFAndroid.unregisterNetworkStateReceiver();

		OFAndroid.sleepLocked=false;
		started = false;
	}

	public static void onDestroy(){
		started = false;		
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
		
		OFAndroid.onDestroy();
		OFAndroidWindow.exit();
	}
	
	public static void exit()
	{
		OFAndroid.exit();
	}
}
