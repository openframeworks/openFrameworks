package cc.openframeworks.android;

import androidx.annotation.Keep;
import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowCompat;
import androidx.core.view.WindowInsetsCompat;
import androidx.core.view.WindowInsetsControllerCompat;

import android.app.Activity;
import android.app.KeyguardManager;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.WindowManager;

import cc.openframeworks.OFAndroid;
import cc.openframeworks.OFAndroidLifeCycle;

import com.getkeepsafe.relinker.ReLinker;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import java.io.File;
import java.io.IOException;

import cc.openframeworks.OFAndroid;
import cc.openframeworks.OFAndroidLifeCycle;
import cc.openframeworks.OFAndroidLifeCycleHelper;


public class OFActivity extends cc.openframeworks.OFActivity {

    public static String appName = "ofApp"; // modify this to target appName (ofApp etc)
    private static final String LOG_TAG = appName + "::OFActivity";
	private static String dataPath="";
	private static SharedPreferences settings;
	private boolean hasCreated = false;
	private int frameRate = 0;

	boolean keep = false;

	private ReLinker.Logger logcatLogger = new ReLinker.Logger() {
		@Override
		public void log(String message) {
			Log.d("ReLinker", message);
		}
	};

	// Extremely important
    public OFActivity() {
        OFAndroidLifeCycle.coreLibraryLoaded = true;

        OFAndroid.maxSamples = 4;
        OFAndroid.maximumFrameRate = 144;

		OFActivity thisActivity = this;
		ReLinker.Logger logcatLogger = new ReLinker.Logger() {
			@Override
			public void log(String message) {
				Log.d("ReLinker", message);
			}
		};
		ReLinker.log(logcatLogger)
                .force()
                .recursively()
                .loadLibrary(this, appName, new ReLinker.LoadListener() {
                    @Override
                    public void success() {
                        Log.i(LOG_TAG, "loadLibrary success");
                        OFAndroidLifeCycle.appLibraryLoaded = true;
                        Setup(); // very important - this will in turn call main
                    }

                    @Override
                    public void failure(Throwable t) {
                        /* Boo */
                        Log.i(LOG_TAG, "loadLibrary failure" + t.getMessage());
                    }
                });
    }

	private static final int BUFFER_SIZE = 5192;


    @Override
    public void onCreate(Bundle savedInstanceState) {
        Log.i(LOG_TAG, "onCreate");
        super.onCreate(savedInstanceState);

		setDataPath(this);
		OFAndroidLifeCycleHelper.setCopyAssets(true);
		///OFAndroidLifeCycleHelper.setCopyOldAssets(true, ""); // old location was in error in a data folder. must move to keep in sync


		setFullscreen();
        hideSystemBars();
		if (!isTaskRoot()) {
			Log.w(LOG_TAG, "isNotTask Root Detected. Closing new Activity. Due to launching resumed app from App Drawer / Play Games Link. Will continue normal activity");
			finish();
			return;
		} else{
			Log.w(LOG_TAG, "isNotTask Root not Detected. Will continue with splash activity");
		}
		getWindow().setTitle(getString(R.string.APP_NAME));
		OFAndroid.packageName = getPackageName();
		OFAndroidLifeCycle.reset(); // Reset STATIC

		settings = getSharedPreferences("OFActivity_settings", 0);
		final String packageName = getPackageName();
		setDataPath(this);

		boolean isAssetsCopied = false; //settings.getBoolean("isAssetsCopied", false);


		if(!isAssetsCopied)
		{
			Log.i(LOG_TAG, "No assets found, copying from app");
			if(copyAssetsToDataPath(this)){ // copy all assets
				File settingsFile = null;
				settingsFile = new File(dataPath);
				boolean assetsAvailable = settingsFile != null && settingsFile.exists();
				assetsAvailable = settingsFile.exists(); // lets double check
				if(assetsAvailable) {
					Log.i(LOG_TAG, "Asset assets");
					SharedPreferences.Editor editor = settings.edit();
					editor.putBoolean("isAssetsCopied", true);
					editor.commit();
					//StartoFApp();
				} else{
					Log.e(LOG_TAG, "Assets have not been copied correctly :(");
				}
			} else {
				Log.e(LOG_TAG, "Assets have not been copied correctly :(");
			}
		} else {
			//StartoFApp();
		}

		hasCreated = true;
    }

	private void StartoFApp() {
		Log.i(LOG_TAG, "StartoFApp");
		Intent intent = new Intent();
		//intent.setClassName(SplashScreenActivity.this, "cc.openframeworks.android.OFActivity");
		//startActivity(intent);
		finish();
	}

	private static boolean copyAssetsToDataPath(Activity activity) {
		OFAndroid.packageName = activity.getPackageName();
		Log.i(LOG_TAG,"starting resources extractor deploying to " + dataPath);
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
			//if(installed>lastInstalled){
				SharedPreferences.Editor editor = preferences.edit();
				editor.putLong("installed", installed);
				editor.apply();
				copydata = true;
			//}
			files = am.list("");
		} catch (PackageManager.NameNotFoundException e1) {
			copydata = false;
		} catch (IOException e) {
			e.printStackTrace();
		}
		try {
			if(copydata){
				for (String file : files) {
					try {
						if(DoCopyFile(file)) {
							String unzipLocation = dataPath;
							// Unzip assets.zip
							// boolean succeed = false; //unpackZip(dataPath, "data.zip", dataPath);
							//unzipFromAssets(activity, file, dataPath);
							Log.i(LOG_TAG, "DoCopyFile: "+ file);
							copyAssetFolder(am, file, dataPath+"/"+file);
							// Store defaults
						} else {
							Log.i(LOG_TAG, "Dont CopyFile: "+ file);
						}
					} catch (Exception e) {
						Log.e(LOG_TAG, "error copying file", e);
					}
				}
				return true;
			}else{
				return true;
			}
		} catch (Exception e) {
			Log.e(LOG_TAG,"error retrieving app name",e);
		}
		return false;
	}

	private static boolean DoCopyFile(String file) {
		return file != null;
	}

	private static void copyAssetFolder(AssetManager am, String src, String dest) throws IOException {
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
		destfh = new File(dest);
		if(isDir) {
			Log.i("Copy ",src);
			if( !destfh.exists() ){
				destfh.mkdir();
			}
			String assets[] = am.list(src);
			for(String asset : assets) {
				copyAssetFolder(am, src + "/" + asset, dest + "/" + asset);
			}

		} else {
			if(!destfh.exists()) {
				Log.i("Copy ", src);
				int count, buffer_len = 4096;
				byte[] data = new byte[buffer_len];
				// copy the file from the assets subsystem to the filesystem
				FileOutputStream destOS = new FileOutputStream(destfh);
				while ((count = srcIS.read(data, 0, buffer_len)) != -1) {
					destOS.write(data, 0, count);
				}
				// and close the two files
				srcIS.close();
				destOS.close();
				data = null;
			} else {
				Log.i("Skipping Copy ", src);
			}
		}
		destfh = null;
	}

	public static void unzipFromAssets(Context context, String zipFile, String destination) {
		try {
			if (destination == null || destination.length() == 0)
				destination = context.getFilesDir().getAbsolutePath();
			InputStream stream = context.getAssets().open(zipFile);
			unzip(stream, destination);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public static void unzip(InputStream stream, String destination) {
		dirChecker(destination, "");
		byte[] buffer = new byte[BUFFER_SIZE];
		try {
			ZipInputStream zin = new ZipInputStream(stream);
			ZipEntry ze = null;
			while ((ze = zin.getNextEntry()) != null) {
				Log.v(LOG_TAG, "Unzipping " + ze.getName());
				if (ze.isDirectory()) {
					dirChecker(destination, ze.getName());
				} else {
					File f = new File(destination, ze.getName());
					if (!f.exists()) {
						boolean success = f.createNewFile();
						if (!success) {
							Log.w(LOG_TAG, "Failed to create file " + f.getName());
							continue;
						}
						FileOutputStream fout = new FileOutputStream(f);
						int count;
						while ((count = zin.read(buffer)) != -1) {
							fout.write(buffer, 0, count);
						}
						zin.closeEntry();
						fout.close();
					}
				}

			}
			zin.close();
		} catch (Exception e) {
			Log.e(LOG_TAG, "unzip", e);
		}

	}

	private static boolean dirChecker(String destination, String dir) {
		File f = new File(destination, dir);

		if (!f.isDirectory()) {
			boolean success = f.mkdirs();
			if (!success) {
				Log.w(LOG_TAG, "Failed to create folder " + f.getName());
			}
			return success;
		}
		return false;
	}

    // Menus
    // http://developer.android.com/guide/topics/ui/menus.html
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Create settings menu options from here, one by one or infalting an xml
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // This passes the menu option string to OF
        // you can add additional behavior from java modifying this method
        // but keep the call to OFAndroid so OF is notified of menu events
        if(OFAndroid.menuItemSelected(item.getItemId())){

            return true;
        }
        return super.onOptionsItemSelected(item);
    }


    @Override
    public boolean onPrepareOptionsMenu (Menu menu){
        // This method is called every time the menu is opened
        //  you can add or remove menu options from here
        return  super.onPrepareOptionsMenu(menu);
    }

    public void onRestore() {

        if (!OFAndroidLifeCycle.appLibraryLoaded) return;
    }

    private void hideSystemBars() {
        WindowInsetsControllerCompat windowInsetsController =
                ViewCompat.getWindowInsetsController(getWindow().getDecorView());
        if (windowInsetsController == null) {
            return;
        }
        // Configure the behavior of the hidden system bars
        windowInsetsController.setSystemBarsBehavior(
                WindowInsetsControllerCompat.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE
        );
        // Hide both the status bar and the navigation bar
        windowInsetsController.hide(WindowInsetsCompat.Type.systemBars());
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);

        if (hasFocus) {
            if (Build.VERSION.SDK_INT < Build.VERSION_CODES.KITKAT) {

                setFullscreen();
            } else {
                setFullscreen();
            }
        }	else {

        }
    }

    private void handleException(Exception e, String details) {
        Log.e(LOG_TAG, "Exception:", e);

    }
	@Override
	public void onDetachedFromWindow() {
		super.onDetachedFromWindow();
	}

	@Override
	public void onPause() {
		super.onPause();
	}

	@Override
	public void onStart() {
		super.onStart();
		if(!keep){
			KeepScreen();
		}
	}

	@Override
	public void onStop() {
		Log.d(LOG_TAG, "onStop");
		keep=false;
		super.onStop();
	}

	@Override
	public void onRestart() {
		Log.d(LOG_TAG, "onRestart");
		super.onRestart();
	}

	@Override
	public void onResume() {
		Log.d(LOG_TAG, "onResume");
		super.onResume();
		final SharedPreferences settings = getSharedPreferences("settings", 0);
		boolean isFirstLaunch = settings.getBoolean("is_first_launch", true);
		if(isFirstLaunch) {
			SharedPreferences.Editor editor = settings.edit();
			editor.putBoolean("is_first_launch", false);
			editor.commit();
		}
		if(!keep){
			KeepScreen();
		}
	}

	private void KeepScreen() {
		this.runOnUiThread(new Runnable() {

			public void run() {
				DisableDeviceToTurnOffScreenWhenIdle();
				keep= true;
			}
		});
	};

	public void DisableDeviceToTurnOffScreenWhenIdle() {
		Log.i(LOG_TAG, "DisableDeviceToTurnOffScreenWhenIdle FLAG_KEEP_SCREEN_ON");
		Activity thisActivity = this; // box
		runOnUiThread(new Runnable() {
			public void run() {
				if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O_MR1) {
					try {
						setShowWhenLocked(false);
						KeyguardManager keyguardManager = (KeyguardManager) getSystemService(Context.KEYGUARD_SERVICE);
						keyguardManager.requestDismissKeyguard(thisActivity, null);
					} catch (Exception ex) {
						Log.e(LOG_TAG, "Enable setTurnScreenOn and setShowWhenLocked is not present on device!");
					}
				}
				try {
					OFAndroid.lockScreenSleep();
					Log.i(LOG_TAG, "Window addFlags: FLAG_TURN_SCREEN_ON");

				} catch (Exception ex) {
					Log.e(LOG_TAG, "AllowDeviceToTurnOffScreenWhenIdle " + ex.getMessage());
				}
			}
		});
	}

	public void AllowDeviceToTurnOffScreenWhenIdle() {
		Log.i(LOG_TAG, "AllowDeviceToTurnOffScreenWhenIdle clearFlags FLAG_KEEP_SCREEN_ON");
		Activity thisActivity = this; // box
		runOnUiThread(new Runnable() {
			public void run() {
				if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O_MR1) {
					try {
						setShowWhenLocked(true);
					} catch (Exception ex) {
						Log.e(LOG_TAG, "Enable setTurnScreenOn and setShowWhenLocked is not present on device!");
					}
				}
				try {
					OFAndroid.unlockScreenSleep();
					Log.i(LOG_TAG, "Window Clear Flags: FLAG_TURN_SCREEN_ON");
				} catch (Exception ex) {
					Log.e(LOG_TAG, "AllowDeviceToTurnOffScreenWhenIdle " + ex.getMessage());
				}
			}
		});
	}

    public void setFullscreen() {
        try {
            if (Build.VERSION.SDK_INT < Build.VERSION_CODES.KITKAT) {
                // No sticky immersive mode for devices pre-kitkat
                getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                        WindowManager.LayoutParams.FLAG_FULLSCREEN);
            } else {
                View decorView = getWindow().getDecorView();
//				int uiOptions = View.SYSTEM_UI_FLAG_FULLSCREEN;
                int uiOptions = View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
                decorView.setSystemUiVisibility(uiOptions);
            }
            WindowCompat.setDecorFitsSystemWindows(getWindow(), false);

        } catch (Exception ex) {
            handleException(ex, "setFullscreen exception");
        }
        try {
            View decorView = getWindow().getDecorView();
            int[] locations = new int[2];
            decorView.getLocationInWindow(locations);
            int[] locations2 = new int[2];
            decorView.getLocationOnScreen(locations2);

        } catch (Exception ex) {
            handleException(ex, "setFullscreen exception");
        }

        WindowInsetsControllerCompat windowInsetsController =
                ViewCompat.getWindowInsetsController(getWindow().getDecorView());
        if (windowInsetsController == null) {
            return;
        }
        // Hide both the status bar and the navigation bar
        windowInsetsController.hide(WindowInsetsCompat.Type.systemBars());
        windowInsetsController.hide(WindowInsetsCompat.Type.navigationBars());
        windowInsetsController.hide(WindowInsetsCompat.Type.statusBars());
    }

	private static void setDataPath(Activity activity) {
		dataPath="";
		try{
			Log.i(LOG_TAG, "sd mounted: " + OFAndroid.checkSDCardMounted());
			OFAndroid.initAppDataDirectory(activity);
			dataPath = OFAndroid.getAppDataDirectory();

			Log.i(LOG_TAG,"creating app directory: " + dataPath);
			try{
				File dir = new File(dataPath);
				if(!dir.isDirectory()){
					if(!dir.mkdirs()){
						Log.e(LOG_TAG,"error creating dir " + dataPath);
						return;
					}
				}
			} catch(Exception e){
				Log.e(LOG_TAG,"error creating dir " + dataPath,e);
			}


		} catch(Exception e){
			Log.e(LOG_TAG,"couldn't move app resources to data directory " + dataPath,e);
		}
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {

		if(LOG_ENGINE) Log.i(LOG_TAG, "onConfigurationChanged() " + getRequestedOrientation() + " newConfig dpi:" + newConfig.densityDpi + " screenLayout:" + newConfig.screenLayout + " screenWidthDp:" + newConfig.screenWidthDp + " screenHeightDp:" + newConfig.screenHeightDp + " hasPaused:" + hasPaused);

		super.onConfigurationChanged(newConfig);


		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
			if(LOG_ENGINE) Log.i(LOG_TAG, "onConfigurationChanged() " + getRequestedOrientation() + " newConfig dpi:" + newConfig.densityDpi + " screenLayout:" + newConfig.screenLayout + " screenWidthDp:" + newConfig.screenWidthDp + " screenHeightDp:" + newConfig.screenHeightDp + " isScreenWideColorGamut:" + newConfig.isScreenWideColorGamut());
		} else {
			if(LOG_ENGINE) Log.i(LOG_TAG, "onConfigurationChanged() " + getRequestedOrientation() + " newConfig dpi:" + newConfig.densityDpi + " screenLayout:" + newConfig.screenLayout + " screenWidthDp:" + newConfig.screenWidthDp + " screenHeightDp:" + newConfig.screenHeightDp + " isScreenWideColorGamut:" + false);
		}
	}

	SharedPreferences getPreferences() {
		try {
			if(getApplicationContext() != null) {
				SharedPreferences settings = getApplicationContext().getSharedPreferences("settings", 0);
				return settings;
			} else {
				Log.e(LOG_TAG, "getPreferences() getApplicationContext == null");
				return null;
			}
		} catch(Exception ex) {
			Log.e(LOG_TAG, ex.getMessage());
			return null;
		}
	}

	SharedPreferences getSavePreferences() {
		try {
			if(getApplicationContext() != null) {
				SharedPreferences scores = getApplicationContext().getSharedPreferences("settings", 0);
				return scores;
			} else {
				Log.e(LOG_TAG, "getSavePreferences() getApplicationContext == null");
				return null;
			}
		} catch(Exception ex) {
			Log.e(LOG_TAG, ex.getMessage());
			return null;
		}
	}


}
