package cc.openframeworks;

import static cc.openframeworks.OFAndroidObject.activity;

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
import androidx.annotation.Keep;

@Keep
public class OFAndroidLifeCycleHelper 
{
	private static final String TAG = OFAndroidLifeCycleHelper.class.getSimpleName();
	private static boolean appInitFlag = false;
	private static boolean started;

	private static boolean copyAssets = true;
	private static boolean copyOldAssets = true;
	private static String copyOldAssetsFolder = "";

	public static void appInit(Activity activity)
	{
		if(appInitFlag)
			return;
		appInitFlag = true;


		copyAssetsToDataPath(activity);
		
		OFAndroid.init();
	}

	public static void setCopyAssets(boolean toCopyAssets) {
		copyAssets = toCopyAssets;
	}

	public static void setCopyOldAssets(boolean toCopyOldAssets, String path) {
		copyOldAssets = toCopyOldAssets;
		copyOldAssetsFolder = path;
	}

	private static void copyAssetsToDataPath(Activity activity) {
		OFAndroid.packageName = activity.getPackageName();

		Log.i(TAG,"starting resources extractor");
		boolean copydata = false;
		String[] files = new String[0];
		if(OFAndroid.assetManager == null)
			OFAndroid.assetManager = activity.getApplicationContext().getAssets();
		boolean restored = false;
		try {
			SharedPreferences preferences = activity.getPreferences(Context.MODE_PRIVATE);

			long lastInstalled = preferences.getLong("installed", 0);
			restored = preferences.getBoolean("restored", false);

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
			files = OFAndroid.assetManager.list("");

		} catch (NameNotFoundException e1) {
			copydata = false;
		} catch (IOException e) {
			e.printStackTrace();
		}

		if(copyAssets == false) {
			copydata = copyAssets;
		}


		OFAndroid.reportPrecentage(.05f);

		String dataPath="";
		try{
			Log.i(TAG, "sd mounted: " + OFAndroid.checkSDCardMounted());
			OFAndroid.initAppDataDirectory(activity);
			dataPath = OFAndroid.getAppDataDirectory();
			Log.i(TAG,"creating app directory: " + dataPath);
			OFAndroid.setAppDataDir(dataPath);
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
			if(copyOldAssets && !restored) {
				OFAndroid.moveOldDataFrom(copyOldAssetsFolder, dataPath);
				try {
					SharedPreferences preferences = activity.getPreferences(Context.MODE_PRIVATE);
					Editor editor = preferences.edit();
					editor.putBoolean("restored", true);
					editor.apply();
				} catch (Exception ex) {
					Log.e(TAG,"Exception saving preferences:" + ex.getMessage());
				}
				OFAndroid.reportPrecentage(.10f);
			} else {
				OFAndroid.reportPrecentage(.10f);
			}

			OFAndroid.reportPrecentage(.10f);
		} catch(Exception e){
			Log.e(TAG,"couldn't move app resources to data directory " + dataPath,e);
		}


		try {
			if(copydata){
				for (String file : files) {
					try {
						copyAssetFolder( OFAndroid.assetManager, file, dataPath+"/"+file);
					} catch (Exception e) {
						Log.e("OF", "error copying file", e);
					}

				}

			}else{
				OFAndroid.reportPrecentage(.70f);
			}
		} catch (Exception e) {
			Log.e(TAG,"error retrieving app name",e);
		}
	}

	private static boolean DoNotCopyFile(String file) {
		if(file == null || file.startsWith("license") || file.startsWith("device_features") )
			return false;
		return true;
	}

	private static void copyAssetFolder(AssetManager am, String src, String dest) throws IOException {

		if(OFActivity.LOG_ENGINE) Log.i("Copy ",src);
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

			data = null;
		}
		destfh = null;
	}


	
	public static void onCreate()
	{
		if(OFActivity.LOG_ENGINE) Log.i(TAG,"onCreate");
		if(OFAndroid.assetManager == null)
			OFAndroid.assetManager = activity.getApplicationContext().getAssets();
		OFAndroid.setAssetManager(OFAndroid.assetManager);
		OFAndroid.onCreate();
		OFAndroid.onUnpackingResourcesDone();
		OFGLSurfaceView glView = OFAndroidLifeCycle.getGLView();
		if(OFAndroidLifeCycle.isSurfaceCreated() == false && glView == null){
			Log.i(TAG,"onCreate glView is null or not setup");

			OFAndroid.setupGL(OFAndroid.eglVersion, true);
			if(OFAndroidLifeCycle.coreLibraryLoaded && OFAndroidLifeCycle.appLibraryLoaded) {
				OFAndroid.onStart();
			}
		}
		
		OFAndroid.runOnMainThread(new Runnable() {
		   @Override
		   public void run() {
				OFAndroid.enableTouchEvents();
				OFAndroid.enableOrientationChangeEvents();
			}
		});
	}

	public static void onForceRestart() {
		if(OFActivity.LOG_ENGINE) Log.i(TAG,"onForceRestart");
		OFAndroid.setupGL(OFAndroid.eglVersion, true);
		if(OFAndroidLifeCycle.coreLibraryLoaded && OFAndroidLifeCycle.appLibraryLoaded) {
			OFAndroid.onStart();
		}
	}
	
	public static void onResume(){
		if(OFActivity.LOG_ENGINE) Log.i(TAG,"onResume");

		OFAndroid.enableOrientationChangeEvents();
		if(OFAndroidLifeCycle.coreLibraryLoaded && OFAndroidLifeCycle.appLibraryLoaded) {
			OFAndroid.onResume();
		}
		OFGLSurfaceView glView = OFAndroidLifeCycle.getGLView();
		if(OFAndroidLifeCycle.isSurfaceCreated() == true && glView == null){
			Log.w(TAG,"onResume glView is null or not setup");
			OFAndroid.setupGL(OFAndroid.eglVersion, true);
			if(OFAndroidLifeCycle.coreLibraryLoaded && OFAndroidLifeCycle.appLibraryLoaded) {
				OFAndroid.onStart();
			}
		}
		else if( glView.getRenderer() == null){
			Log.w(TAG,"onResume glView is null or not setup");
			OFAndroid.setupGL(OFAndroid.eglVersion, true);
			if(OFAndroidLifeCycle.coreLibraryLoaded && OFAndroidLifeCycle.appLibraryLoaded) {
				OFAndroid.onStart();
			}
		}
		else if( glView != null && glView.getDisplay() == null) {
			Log.w(TAG,"onResume glView has a null display");
			OFAndroid.setupGL(OFAndroid.eglVersion, true);
			if(OFAndroidLifeCycle.coreLibraryLoaded && OFAndroidLifeCycle.appLibraryLoaded) {
				OFAndroid.onStart();
			}
		}
		OFAndroid.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				OFAndroid.enableTouchEvents();
				OFAndroid.registerNetworkStateReceiver();
			}
		});
		if(OFAndroidLifeCycle.isSurfaceCreated() == true && OFAndroidLifeCycle.isInit()) {
			synchronized (OFAndroidObject.ofObjects) {
				for (OFAndroidObject object : OFAndroidObject.ofObjects) {
					if(object != null) {
						object.onResume();
						object.appResume();
					}
				}
			}
			OFAndroid.runOnMainThread(new Runnable() {
				@Override
				public void run() {
					if (OFAndroid.getOrientation() != -1)
						OFAndroid.setScreenOrientation(OFAndroid.getOrientation());
			}
			});
		} else {
			Log.e(TAG,"onResume not ready yet");
		}
	}
	
	public static void onPause(){
		Log.i(TAG,"onPause");
		if(OFAndroidLifeCycle.coreLibraryLoaded && OFAndroidLifeCycle.appLibraryLoaded) {
			OFAndroid.onPause();
		}
		OFAndroid.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				OFAndroid.disableTouchEvents();
				OFAndroid.unregisterNetworkStateReceiver();
				synchronized (OFAndroidObject.ofObjects) {
					for(OFAndroidObject object : OFAndroidObject.ofObjects){
						if(OFAndroidLifeCycle.coreLibraryLoaded && OFAndroidLifeCycle.appLibraryLoaded) {
							if (object != null) {
								object.onPause();
							}
						}
					}
				}

			}
		});

	}
	
	public static void onStart(){
		if(OFActivity.LOG_ENGINE) Log.i(TAG,"onStart");
		final OFGLSurfaceView glView = OFAndroidLifeCycle.getGLView();
		if(started) {
			if(glView == null || glView != null && !glView.isSetup()){
				Log.i(TAG,"resume view and native");
				OFAndroid.onStart();
			}
			else
				return;
		}
		started = true;
		OFAndroid.onStart();
		OFAndroid.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				OFAndroid.registerNetworkStateReceiver();
				OFAndroid.enableTouchEvents();
				OFAndroid.enableOrientationChangeEvents();
				synchronized (OFAndroidObject.ofObjects) {
					for(OFAndroidObject object : OFAndroidObject.ofObjects){
						if(OFAndroidLifeCycle.coreLibraryLoaded && OFAndroidLifeCycle.appLibraryLoaded) {
							if (object != null) {
								object.onResume();
								object.appResume();
							}
						}
					}
					
				}
			}
		});

//        if(OFAndroid.getOrientation()!=-1) {
//			Log.i(TAG,"setScreenOrientation " + OFAndroid.getOrientation());
//			OFAndroid.setScreenOrientation(OFAndroid.getOrientation());
//		} else {
//			Log.i(TAG,"not setScreenOrientation " + OFAndroid.getOrientation());
//		}


	}
	
	public static void onStop(){
		if(OFActivity.LOG_ENGINE) Log.i(TAG,"onStop");
		OFAndroid.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				OFAndroid.disableTouchEvents();
				OFAndroid.unregisterNetworkStateReceiver();
				synchronized (OFAndroidObject.ofObjects) {
					for(OFAndroidObject object : OFAndroidObject.ofObjects){
						if(OFAndroidLifeCycle.coreLibraryLoaded && OFAndroidLifeCycle.appLibraryLoaded) {
							if (object != null) {
								object.onPause();
							}
						}
					}
				}
			}
		});

		if(OFAndroidLifeCycle.coreLibraryLoaded && OFAndroidLifeCycle.appLibraryLoaded) {
			OFAndroid.onStop();
		}

		OFAndroid.sleepLocked=false;
		started = false;
	}

	public static void onDestroy(){
		started = false;
		if(OFActivity.LOG_ENGINE)  Log.i(TAG,"onDestroy");
		OFAndroid.runOnMainThread(new Runnable() {
			
			@Override
			public void run() {
				OFAndroid.disableTouchEvents();
				OFAndroid.disableOrientationChangeEvents();
				OFAndroid.unregisterNetworkStateReceiver();
				synchronized (OFAndroidObject.ofObjects) {
					for(OFAndroidObject object : OFAndroidObject.ofObjects){
						if(OFAndroidLifeCycle.coreLibraryLoaded && OFAndroidLifeCycle.appLibraryLoaded) {
							if (object != null) {
								object.onStop();
								object.onDestroy();
							}
						}
					}
				}
			}
		});
		OFAndroid.sleepLocked=false;
		if(OFAndroidLifeCycle.coreLibraryLoaded && OFAndroidLifeCycle.appLibraryLoaded) {
			OFAndroid.onStop();
			OFAndroid.onDestroy();
		}

		//OFAndroidWindow.exit();
		OFAndroidLifeCycle.coreLibraryLoaded = false;
	}
	
	public static void exit()
	{
		if(OFActivity.LOG_ENGINE) Log.i(TAG,"exit");

		if(OFAndroidLifeCycle.coreLibraryLoaded && OFAndroidLifeCycle.appLibraryLoaded) {
			OFAndroid.exit();
		}
	}
}
