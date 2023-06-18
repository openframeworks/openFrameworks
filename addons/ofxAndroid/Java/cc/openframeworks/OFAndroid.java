package cc.openframeworks;

import java.io.File;
import java.lang.reflect.Field;
import java.util.HashMap;
import java.util.Locale;
import java.util.Map;
import java.util.UUID;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.hardware.SensorManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiManager.MulticastLock;
import android.os.Build;
import android.os.Environment;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.provider.MediaStore;
import android.util.Log;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.ScaleGestureDetector;
import android.view.View;
import android.view.WindowManager.LayoutParams;
import android.widget.ArrayAdapter;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.Toast;
import java.util.concurrent.Semaphore;
import androidx.annotation.Keep;

@Keep
public class OFAndroid {

	public static String packageName;
	public static int eglVersion = 2;
	public static int samples = 8;
	public static int maxSamples = 8;
	public static int maximumFrameRate = 144;
	public static int targetFrameRate = 144;
	public static int width;
	public static int height;
	public static Locale locale;

	public static int lastInputID = -1;
	public static int lastDeviceID = -1;
	public static int lastInputVendorID = -1;
	public static int lastInputProductID = -1;
	public static String lastInputDescriptor = "";
	public static OFAndroidController.ControllerType lastControllerType = OFAndroidController.ControllerType.NONE;
	public static InputDevice lastInputDevice = null;
	public static AssetManager assetManager;

	public static boolean isDeviceSamsung = false;
	public static boolean isDeviceHTC = false;
	public static boolean isDeviceHuawei = false;
	public static boolean isDeviceAmazon = false;

	public static boolean isHTCDevice()
	{
		String manufacturer = android.os.Build.MANUFACTURER;
		return manufacturer.toLowerCase(Locale.ENGLISH).contains("htc");
	}

	public static boolean isSamsungDevice()
	{
		String manufacturer = android.os.Build.MANUFACTURER;
		return manufacturer.toLowerCase(Locale.ENGLISH).contains("samsung");
	}

	public static boolean isHuaweiDevice()
	{
		String manufacturer = android.os.Build.MANUFACTURER;
		return manufacturer.toLowerCase(Locale.ENGLISH).contains("huawei");
	}

	public static boolean isAmazonDevice()
	{
		String manufacturer = android.os.Build.MANUFACTURER;
		return manufacturer.toLowerCase(Locale.ENGLISH).contains("amazon");
	}

	// List based on http://bit.ly/NpkL4Q
	@SuppressLint("SdCardPath") // the following list is fall back 3
	private static final String[] mExternalStorageDirectories = new String[] {

			"/mnt/Removable/MicroSD",
			"/storage/removable/sdcard1", // !< Sony Xperia Z1
			"/Removable/MicroSD", // Asus ZenPad C
			"/removable/microsd",
			"/external_sd", // Samsung
			"/_ExternalSD", // some LGs
			"/storage/extSdCard", // later Samsung
			"/storage/extsdcard", // Main filesystem is case-sensitive; FAT isn't.
			"/mnt/extsd", // some Chinese tablets, e.g. Zeki
			"/storage/sdcard1", // If this exists it's more likely than sdcard0 to be removable.
			"/mnt/extSdCard",
			"/mnt/sdcard/external_sd",
			"/mnt/external_sd",
			"/storage/external_SD",
			"/storage/ext_sd", // HTC One Max
			"/mnt/sdcard/_ExternalSD",
			"/mnt/sdcard-ext",
			"/sdcard2", // HTC One M8s
			"/sdcard1", // Sony Xperia Z
			"/mnt/media_rw/sdcard1",   // 4.4.2 on CyanogenMod S3
			"/mnt/sdcard", // This can be built-in storage (non-removable).
			"/sdcard",
			"/storage/sdcard0",
			"/emmc",
			"/mnt/emmc",
			"/sdcard/sd",
			"/mnt/sdcard/bpemmctest",
			"/mnt/external1",
			"/data/sdext4",
			"/data/sdext3",
			"/data/sdext2",
			"/data/sdext",
			"/storage/microsd", //ASUS ZenFone 2
			// If we ever decide to support USB OTG storage, the following paths could be helpful:
			// An LG Nexus 5 apparently uses usb://1002/UsbStorage/ as a URI to access an SD
			// card over OTG cable. Other models, like Galaxy S5, use /storage/UsbDriveA
			//        "/mnt/usb_storage",
			//        "/mnt/UsbDriveA",
			//        "/mnt/UsbDrsdcard-extiveB",
	};

	private static String getPackageName(){
		return OFAndroidLifeCycle.getActivity().getPackageName();
	}

	public static boolean checkPermission(String permission){
		if (ContextCompat.checkSelfPermission(OFAndroidLifeCycle.getActivity(), permission)
				== PackageManager.PERMISSION_GRANTED) {
			return true;
		}else{
			return false;
		}
	}

	public static void requestPermission(String permission){
		if (ContextCompat.checkSelfPermission(OFAndroidLifeCycle.getActivity(), permission)
				!= PackageManager.PERMISSION_GRANTED) {

			ActivityCompat.requestPermissions(OFAndroidLifeCycle.getActivity(),
					new String[]{permission}, 0);
		}
	}

	public static void savePreferences() {
		Activity activity = OFAndroidLifeCycle.getActivity();
		if(activity != null && OFActivity.class.isInstance(activity))
			((OFActivity)activity).savePreferences();
	}

	private boolean isExternalStorageWritable() {
		return Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED);
	}

	public static Bitmap screenShot(View view) {
		Bitmap bitmap = Bitmap.createBitmap(view.getWidth(),view.getHeight(), Bitmap.Config.ARGB_8888);
		Canvas canvas = new Canvas(bitmap);
		view.draw(canvas);
		return bitmap;
	}

	@SuppressWarnings("unused")
	private String getPictureDirectoryPath() {
		return Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES).getPath();
	}


	@SuppressWarnings("unused")
	private void addScreenshotToGallery(String filepath) {
		Intent mediaScanIntent = new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE);
		File f = new File(filepath);
		Uri contentUri = Uri.fromFile(f);
		mediaScanIntent.setData(contentUri);
		OFAndroidLifeCycle.getActivity().sendBroadcast(mediaScanIntent);
	}

	public static Bitmap screenShot(OFGLSurfaceView view) {
		Bitmap bitmap = Bitmap.createBitmap(view.getWidth(),view.getHeight(), Bitmap.Config.ARGB_8888);
		Canvas canvas = new Canvas(bitmap);
		view.draw(canvas);
		return bitmap;
	}

	public static void share(Context context, Bitmap bitmap, String subject, String text){
		String pathofBmp=
				MediaStore.Images.Media.insertImage(context.getContentResolver(),
						bitmap,"title", null);
		Uri uri = Uri.parse(pathofBmp);
		Intent shareIntent = new Intent(Intent.ACTION_SEND);
		shareIntent.setType("image/*");
		shareIntent.putExtra(Intent.EXTRA_SUBJECT, subject);
		shareIntent.putExtra(Intent.EXTRA_TEXT, text);
		shareIntent.putExtra(Intent.EXTRA_STREAM, uri);
		context.startActivity(Intent.createChooser(shareIntent, subject));
	}


	// Checks if a volume containing external storage is available to at least read.
	private boolean isExternalStorageReadable() {
		return Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED) ||
				Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED_READ_ONLY);
	}

	public static String getDirectoryPictures() {
		return Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES).getPath();
	}

	public static String getRealExternalStorageDirectory(Context context) {



		// Standard way to get the external storage directory
		File externalPathFile = Environment.getExternalStorageDirectory();
		if (externalPathFile.exists()) {
			File externalPathFileInternal = new File(externalPathFile.getPath() + "/Android/data/"+packageName);
			if(externalPathFileInternal.exists() && externalPathFileInternal.canWrite())
				return externalPathFileInternal.getPath();
		}

		// Standard way to get the external storage directory
		String externalPath = context.getExternalFilesDir(null).getPath();
		if(externalPath.endsWith("/files")) externalPath = externalPath.replace("/files", "");
		File externalCardDir = new File(externalPath);
		if(externalCardDir.exists() && externalCardDir.canWrite()) {
			return externalCardDir.getPath();
		}

		// This checks if any of the directories from mExternalStorageDirectories exist, if it does, it uses that one instead
		for(int i = 0; i < mExternalStorageDirectories.length; i++)
		{
			//Log.i("OF", "Checking: " + mExternalStorageDirectories[i]);
			File SDCardDir = new File(mExternalStorageDirectories[i]);
	    	if(SDCardDir.exists() && SDCardDir.canWrite()) {
	    		externalPath = mExternalStorageDirectories[i];	// Found writable location
				break;
	    	}
		}

		Log.i("OF", "Using storage location: " + externalPath);
		return externalPath;
	}

	public static String getOldExternalStorageDirectory(String packageName)
	{
		// Standard way to get the external storage directory
		String externalPath = Environment.getExternalStorageDirectory().getPath();
		File SDCardDir = new File(externalPath);
    	if(SDCardDir.exists() && SDCardDir.canWrite()) {
    		return externalPath + "/Android/data/"+packageName;
    	}

		// This checks if any of the directories from mExternalStorageDirectories exist, if it does, it uses that one instead
		for(int i = 0; i < mExternalStorageDirectories.length; i++)
		{
			//Log.i("OF", "Checking: " + mExternalStorageDirectories[i]);
			SDCardDir = new File(mExternalStorageDirectories[i]);
	    	if(SDCardDir.exists() && SDCardDir.canWrite()) {
	    		externalPath = mExternalStorageDirectories[i] + "/Android/data/"+packageName;	// Found writable location
				break;
	    	}
		}

		Log.i("OF", "Using storage location: " + externalPath);
		return externalPath + "/Android/data/"+packageName;
	}

	public static void moveOldDataFrom(String fromFolder, String dst){

		// This checks if any of the directories from mExternalStorageDirectories exist, if it does, it uses that one instead
		for(int i = 0; i < mExternalStorageDirectories.length; i++)
		{
			//Log.i("OF", "Checking: " + mExternalStorageDirectories[i]);
			try {
				File SDCardDir = new File(mExternalStorageDirectories[i]);
				if (SDCardDir.exists()) { // sd cart mount location exists could have been used
					File appDataDir = new File(SDCardDir.getPath() + "/Android/data/" + packageName + fromFolder);
					if (appDataDir.exists() && appDataDir.canWrite()) {
						String externalPath = appDataDir.getPath();    // Found writable location
						File dstFile = new File(dst);
						if (appDataDir.equals(dstFile)) {
							Log.i("OF", "moveOldData  appDataDir same as new location:" + appDataDir.getPath() + " newLocation:" + dst);
							continue;
						}
						if (appDataDir.isDirectory() && appDataDir.listFiles().length > 1) {
							Log.i("OF", "moveOldData from:" + appDataDir.getPath() + " to:" + dst);
							moveOldData(appDataDir.getPath(), dst);
						}
					} else {
						Log.i("OF", "SDCardDir found however no app directory so skipping for" + SDCardDir.getPath());
					}
				}
			} catch(Exception ex) {

			}
		}
	}

	public static void moveOldData(String src, String dst){
		File srcFile = new File(src);
		File dstFile = new File(dst);

		if(srcFile.equals(dstFile)) return;

		if(srcFile.isDirectory() && srcFile.listFiles().length>1){
			Log.i("OF", "moveOldData  " + srcFile.getName());
			for(File f: srcFile.listFiles()){
				if(f.equals(dstFile)){
					Log.i("OF", "movingOldData " + f.getName() + " equals" + dstFile.getPath());
					moveOldData(f.getAbsolutePath(),dst+"/"+f.getName());
					continue;
				}
				Log.i("OF", "movingOldData " + f.getName());
				f.renameTo(new File(dst+"/"+f.getName()));
			}
		}
	}

	public static Context getContext(){
		return OFAndroidLifeCycle.getActivity();
	}
	
	public static String getAppDataDirectory(){
		return dataPath;
	}
	
	public static void initAppDataDirectory(Activity activity)
	{
		dataPath = getRealExternalStorageDirectory(activity);
	}
	

	static void runOnMainThread(Runnable runnable)
	{
		OFAndroidLifeCycle.getActivity().runOnUiThread(runnable);
	}
	
	static void runOnGLThread(Runnable runnable)
	{
		if(OFAndroidLifeCycle.getGLView() != null)
			OFAndroidLifeCycle.getGLView().queueEvent(runnable);
	}
	
	
	public static void reportPrecentage(float precent)
	{
		Activity activity = OFAndroidLifeCycle.getActivity();
		if(activity != null && OFActivity.class.isInstance(activity))
			((OFActivity)activity).onLoadPercent(precent);
	}
	
	public static void fatalErrorDialog(final Activity activity, final String msg){
		activity.runOnUiThread(new Runnable(){
			public void run() {
				new AlertDialog.Builder(activity)  
					.setMessage(msg)  
					.setTitle("")  
					.setCancelable(false)  
					.setNeutralButton(android.R.string.ok,  
							new DialogInterface.OnClickListener() {  
						public void onClick(DialogInterface dialog, int whichButton){
							activity.finish();
						}
	
				  	})  
				  	.show();
			}  
		});
	}
	
	static public void onUnpackingResourcesDone(){
		unpackingDone = true;
		Activity activity = OFAndroidLifeCycle.getActivity();
		if(activity != null && OFActivity.class.isInstance(activity))
			((OFActivity)activity).onUnpackingResourcesDone();
	}
	
	static public boolean menuItemSelected(int id){
		try {
			Class<?> menu_ids = Class.forName(getPackageName()+".R$id");
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
			Class<?> menu_ids = Class.forName(getPackageName()+".R$id");
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
		runOnMainThread(new Runnable(){
			public void run() {
				try {
					Class<?> menu_ids = Class.forName(getPackageName()+".R$id");
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
		runOnMainThread(new Runnable(){
			public void run() {
				try {
					Class<?> menu_ids = Class.forName(getPackageName()+".R$id");
					Field field = menu_ids.getField(id);
					CompoundButton checkbox = (CompoundButton) OFAndroidLifeCycle.getActivity().findViewById(field.getInt(null));
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
			string_ids = Class.forName(getPackageName()+".R$string");
			Field field = string_ids.getField(idStr);
			int id = field.getInt(null);
			return (String) OFAndroidLifeCycle.getActivity().getResources().getText(id);
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
			ConnectivityManager conMgr =  (ConnectivityManager)OFAndroidLifeCycle.getActivity().getSystemService(Context.CONNECTIVITY_SERVICE);
			return conMgr!=null && ( conMgr.getNetworkInfo(ConnectivityManager.TYPE_WIFI).getState() == NetworkInfo.State.CONNECTED  ) ;
		}catch(Exception e){
			Log.e("OF","error checking wifi connection",e);
			return false;
		}
	}
	
	static public boolean isMobileOnline(){
		try{
			ConnectivityManager conMgr =  (ConnectivityManager)OFAndroidLifeCycle.getActivity().getSystemService(Context.CONNECTIVITY_SERVICE);
			
			return conMgr!=null && ( conMgr.getNetworkInfo(ConnectivityManager.TYPE_MOBILE).getState() == NetworkInfo.State.CONNECTED  ) ;
		}catch(Exception e){
			Log.e("OF","error checking mobile connection",e);
			return false;
		}
	}

	static private BroadcastReceiver networkStateReceiver;
	
	public static void registerNetworkStateReceiver()
	{
		if(OFAndroid.networkStateReceiver!=null){
			OFAndroid.monitorNetworkState();
		}
	}
	
	public static void unregisterNetworkStateReceiver()
	{
		if(OFAndroid.networkStateReceiver!=null){
			try{
				OFAndroidLifeCycle.getActivity().unregisterReceiver(OFAndroid.networkStateReceiver);
			}catch(java.lang.IllegalArgumentException e){
				
			}
		}
	}

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
		OFAndroidLifeCycle.getActivity().registerReceiver(networkStateReceiver, filter);
		networkConnected(isOnline());
	}
	
	static public void launchBrowser(String url){

		if (!url.startsWith("http") && !url.startsWith("https")) { // fix crash due to missing http/https
			url = "https" + url; //force https if missing, it's 2021
		}
		try {
			Intent intent = new Intent();
			intent.setAction(Intent.ACTION_VIEW);
			intent.addCategory(Intent.CATEGORY_BROWSABLE);
			//intent.addFlags(FLAG_ACTIVITY_MULTIPLE_TASK);

			Log.v("OF", "launchBrowser: " + url);

			Intent.createChooser(intent, "Choose browser");
			intent.setData(Uri.parse(url));
			//OFAndroidLifeCycle.getActivity().startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse(url)));
			OFAndroidLifeCycle.getActivity().startActivity(intent);
		} catch (Exception ex) {
			Log.w("launchBrowser", "Intent to Launch Browser Failed. likely no browser for handling  Exception:" + ex.getMessage());
		}
	}

	
	static Map<Integer,ProgressDialog> progressDialogs = new HashMap<Integer, ProgressDialog>();
	static int lastProgressID=0;
	static public int progressBox(String msg){
		final String finmsg = msg;
		final int id = lastProgressID++;
		runOnMainThread(new Runnable(){
			public void run() {
				ProgressDialog d = new ProgressDialog(OFAndroidLifeCycle.getActivity());
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
			runOnMainThread(new Runnable(){
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

		synchronized (OFAndroidObject.ofObjects) {
			for(OFAndroidObject object : OFAndroidObject.ofObjects){
				object.onActivityResult(requestCode,resultCode,intent);
			}
		}
	}

	// native methods to call OF c++ callbacks
    public static native void setAppDataDir(String data_dir);
    public static native void init();
    public static native void onCreate();
    public static native void onRestart();
    public static native void onStart();
    public static native void onStop();
    public static native void onResume();
    public static native void onPause();
    public static native void onDestroy();
    public static native void onSurfaceCreated();
    public static native void onSurfaceDestroyed();
    public static native void setup(int w, int h);
    public static native void resize(int w, int h);
    public static native void render();
    public static native void exit();
    public static native void setAssetManager(AssetManager assetManager);
    public static native boolean isWindowReady();
    
    public static native void onTouchDown(int id,float x,float y,float pressure,float majoraxis,float minoraxis,float angle);
    public static native void onTouchDoubleTap(int id,float x,float y,float pressure);
    public static native void onTouchUp(int id,float x,float y,float pressure,float majoraxis,float minoraxis,float angle);
    public static native void onTouchMoved(int id,float x,float y,float pressure,float majoraxis,float minoraxis,float angle);
    public static native void onTouchCancelled(int id,float x,float y);


	public static native void onAxisMoved(int id, int deviceid, int productid, float x, float y);
    
    public static native void onSwipe(int id, int swipeDir);
    
    public static native boolean onScaleBegin(ScaleGestureDetector detector);
    public static native void onScaleEnd(ScaleGestureDetector detector);
    public static native boolean onScale(ScaleGestureDetector detector);
    
    public static native boolean onKeyDown(int keyCode, int unicode);
    public static native boolean onKeyUp(int keyCode, int unicode);
    public static native boolean onBackPressed();
    
    public static native boolean onMenuItemSelected(String menu_id);
    public static native boolean onMenuItemChecked(String menu_id, boolean checked);
    
    public static native void okPressed();
    public static native void cancelPressed();
    
    public static native void networkConnected(boolean conected);

	public static native void deviceOrientationChanged(int orientation);

	public static native void deviceHighestRefreshRate(int refreshRate);
	public static native void deviceRefreshRate(int refreshRate);

	public static native void setSampleSize(int samples);

	public static native void setMultiWindowMode(boolean multiWindow);

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
    
    public static int getOrientation()
    {
    	return orientation;
    }
    
    private static int orientation=-1;
    public static void setScreenOrientation(int orientation){
    	OFAndroid.orientation = orientation;
    	switch(orientation){
    	case 0:
    		OFAndroidLifeCycle.getActivity().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
    		break;
    	case 90:
    		OFAndroidLifeCycle.getActivity().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
    		break;
    	case 270:
			if (Build.VERSION.SDK_INT >= 9) {
				OFAndroidLifeCycle.getActivity().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE);
				break;
			}
			OFAndroidLifeCycle.getActivity().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
    		break;
    	case 180:
			if (Build.VERSION.SDK_INT >= 9) {
				OFAndroidLifeCycle.getActivity().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT);
				break;
			}
			OFAndroidLifeCycle.getActivity().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
    		break;
    	case -1:
    		OFAndroidLifeCycle.getActivity().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR);
    		break;
    	}
    }
    
    public static void pauseApp(){
    	OFAndroidLifeCycle.getActivity().moveTaskToBack(true);
    }

	
	public static void setupAccelerometer(){
		if(accelerometer==null)
			accelerometer = new OFAndroidAccelerometer((SensorManager)OFAndroidLifeCycle.getActivity().getSystemService(Context.SENSOR_SERVICE));
	}
	
	static MulticastLock mcLock;
	public static void enableMulticast(){
		WifiManager wifi = (WifiManager)OFAndroidLifeCycle.getActivity().getApplicationContext().getSystemService( Context.WIFI_SERVICE );
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
		runOnMainThread(new Runnable(){
			public void run() {
				new AlertDialog.Builder(OFAndroidLifeCycle.getActivity())  
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
		runOnMainThread(new Runnable(){
			public void run() {
				new AlertDialog.Builder(OFAndroidLifeCycle.getActivity())  
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
		runOnMainThread(new Runnable(){
			public void run() {
				Activity activity = OFAndroidLifeCycle.getActivity();
				final EditText input = new EditText(activity); 
					new AlertDialog.Builder(activity)  
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
		runOnMainThread(new Runnable(){
			public void run() {
				Activity activity = OFAndroidLifeCycle.getActivity();
				final ListView listView = new ListView(activity); 
				final ListAdapter adapter = new ArrayAdapter<String>(activity, android.R.layout.simple_list_item_1, alertList);
				listView.setAdapter(adapter);
				new AlertDialog.Builder(activity)   
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
		runOnMainThread(new Runnable(){
			public void run() {
				Toast toast = Toast.makeText(OFAndroidLifeCycle.getActivity(), alertMsg, Toast.LENGTH_SHORT);
	        	toast.show();  
			}  
		});
	}
	
	public static String toDataPath(String path){
		return dataPath + "/" + path;
	}
	
	static boolean sleepLocked=false;
	
	public static void lockScreenSleep(){
		if(!sleepLocked){
			runOnMainThread(new Runnable() {
				
				@Override
				public void run() {
					try{
						sleepLocked=true;
						OFAndroidLifeCycle.getActivity().getWindow().addFlags(LayoutParams.FLAG_KEEP_SCREEN_ON);
					}catch(Exception e){
						
					}
					
				}
			});
		}
	}
	
	public static void unlockScreenSleep(){
		if(sleepLocked){
			runOnMainThread(new Runnable() {
			
				@Override
				public void run() {
					try{
						sleepLocked=false;
						OFAndroidLifeCycle.getActivity().getWindow().clearFlags(LayoutParams.FLAG_KEEP_SCREEN_ON);
					}catch(Exception e){
						
					}
					
				}
			});
		}
	}

	public static void updateRefreshRates(){
			runOnMainThread(new Runnable() {
				@Override
				public void run() {
					try{
						OFAndroidLifeCycle.getActivity().DetermineDisplayConfiguration(true);
					}catch(Exception e){

					}
				}
			});
	}
	
	public static String getRandomUUID(){
		return UUID.randomUUID().toString();
	}

	
	public static boolean isApplicationSetup(){
		if(OFAndroidLifeCycle.getGLView() == null) return false;
		return OFAndroidLifeCycle.getGLView().isSetup();
	}
    
    private static OFAndroidAccelerometer accelerometer;
    private static OFAndroidGPS gps;
    public static OFGestureListener gestureListener;
	private static OFOrientationListener orientationListener;
	private static String dataPath;
	public static boolean unpackingDone = false;

	public static boolean wideGamut = false;
	public static boolean hdrScreen = false;

	public static void disableTouchEvents(){
		OFGLSurfaceView glView = OFAndroidLifeCycle.getGLView();
		if(glView != null) {
			glView.setOnClickListener(null);
			glView.setOnTouchListener(null);
			if(gestureListener != null) gestureListener.enableTouchEvents = false;
		}
		gestureListener = null;
	}
	
	public static void enableTouchEvents(){
		OFGLSurfaceView glView = OFAndroidLifeCycle.getGLView();
		if(glView != null) {
			if (gestureListener == null)
				gestureListener = new OFGestureListener(OFAndroidLifeCycle.getActivity());
			glView.setOnClickListener(gestureListener);
			gestureListener.enableTouchEvents = true;

		} else {
			Log.w( "OF", "enableTouchEvents GLView is null");
		}
	}

	public static void enableOrientationChangeEvents(){
		if(orientationListener == null)
			orientationListener = new OFOrientationListener(OFAndroidLifeCycle.getActivity());
		orientationListener.enable();
	}

	public static void disableOrientationChangeEvents(){
		if(orientationListener != null)
			orientationListener.disable();
	}
	
	public static void setupGL(int version, boolean preserveContextOnPause){
		final int finalversion = version;
		final boolean finalPreserveContextOnPause = preserveContextOnPause;
		final Semaphore mutex = new Semaphore( 0 );

		OFAndroid.eglVersion = finalversion;
		
		runOnMainThread(new Runnable() {
			@Override
			public void run() {
		        OFEGLConfigChooser.setGLESVersion(finalversion);
		        OFAndroidLifeCycle.glCreateSurface( finalPreserveContextOnPause );
		        mutex.release();
			}
		});
		
		try{
			mutex.acquire();
		} catch( Exception ex ){
			Log.w( "OF", "setupGL mutex acquire failed" );
		}
	}
	
	/**
	 * 
	 * @param keyCode
	 * @param keyCodeSource
	 * @return true to say we handled this, false to tell Android to handle it
	 */

	public static boolean keyDown(int keyCode, int keyCodeSource) {
		int unicodeChar = keyCodeSource;
		if(unicodeChar == 0 && keyCode < 714 && keyCode > 0) {
			unicodeChar = keyCode;
			if(OFActivity.LOG_INPUT) Log.i( "OF", "keyDown :" + keyCode + " unicodeChar:" + unicodeChar);
			return onKeyDown(keyCode, unicodeChar);
		} else {
			if(OFActivity.LOG_INPUT) Log.i( "OF", "keyDown :" + keyCode + " unicodeChar:" + unicodeChar);
			return onKeyDown(keyCode, unicodeChar);
		}
	}

	public static boolean keyUp(int keyCode, int keyCodeSource) {
		int unicodeChar = keyCodeSource;
		if(unicodeChar == 0 && keyCode < 714 && keyCode > 0) {
			unicodeChar = keyCode;
			if(OFActivity.LOG_INPUT) Log.i( "OF", "keyUp :" + keyCode + " unicodeChar:" + unicodeChar);
			return onKeyUp(keyCode, unicodeChar);
		} else {
			if(OFActivity.LOG_INPUT) Log.i( "OF", "keyUp :" + keyCode + " unicodeChar:" + unicodeChar);
			return onKeyUp(keyCode, unicodeChar);
		}
	}

	public static boolean keyDown(int keyCode, KeyEvent event) {
        if ((event.getKeyCode() == KeyEvent.KEYCODE_BACK  || event.getKeyCode() == KeyEvent.KEYCODE_MENU || event.getKeyCode() == KeyEvent.KEYCODE_BUTTON_MODE) && event.getRepeatCount() == 0) {
            if( onBackPressed() ) {
            	return true;
           	} else {
           		return false;
           	}
        }
		if((event.getKeyCode() == KeyEvent.KEYCODE_VOLUME_UP || (event.getKeyCode() == KeyEvent.KEYCODE_VOLUME_DOWN)) || (event.getKeyCode() == KeyEvent.KEYCODE_VOLUME_MUTE) || (event.getKeyCode() == KeyEvent.KEYCODE_FOCUS) || event.getKeyCode() == KeyEvent.KEYCODE_CAMERA || event.getKeyCode() == 287) return false;
		if((keyCode == KeyEvent.KEYCODE_VOLUME_UP || (keyCode == KeyEvent.KEYCODE_VOLUME_DOWN)) || (keyCode == KeyEvent.KEYCODE_VOLUME_MUTE) || (keyCode == KeyEvent.KEYCODE_FOCUS)  || keyCode == KeyEvent.KEYCODE_CAMERA || keyCode == 287) return false;


		int unicodeChar = event.getUnicodeChar();
		if(unicodeChar == 0 && keyCode < 714 && keyCode > 0) {
			unicodeChar = keyCode;
			if(OFActivity.LOG_INPUT) Log.i( "OF", "keyDown :" + keyCode + " unicodeChar:" + unicodeChar + " sourceID:" + event.getSource());
			return onKeyDown(keyCode, unicodeChar);
		} else {
			if(OFActivity.LOG_INPUT) Log.i( "OF", "keyDown :" + keyCode + " unicodeChar:" + unicodeChar + " sourceID:" + event.getSource());
			return onKeyDown(keyCode, unicodeChar);
		}

	}
	
	/**
	 * 
	 * @param keyCode
	 * @param event
	 * @return true to say we handled this, false to tell Android to handle it
	 */
	public static boolean keyUp(int keyCode, KeyEvent event) {
		if ((event.getKeyCode() == KeyEvent.KEYCODE_BACK  || event.getKeyCode() == KeyEvent.KEYCODE_MENU || event.getKeyCode() == KeyEvent.KEYCODE_BUTTON_MODE) && event.getRepeatCount() == 0) {
			return true; // handled event
		}
		if((event.getKeyCode() == KeyEvent.KEYCODE_VOLUME_UP || (event.getKeyCode() == KeyEvent.KEYCODE_VOLUME_DOWN)) || (event.getKeyCode() == KeyEvent.KEYCODE_VOLUME_MUTE) || (event.getKeyCode() == KeyEvent.KEYCODE_FOCUS)  || event.getKeyCode() == KeyEvent.KEYCODE_CAMERA || event.getKeyCode() == 287) return false;
		if((keyCode == KeyEvent.KEYCODE_VOLUME_UP || (keyCode == KeyEvent.KEYCODE_VOLUME_DOWN)) || (keyCode == KeyEvent.KEYCODE_VOLUME_MUTE) || (keyCode == KeyEvent.KEYCODE_FOCUS)  || keyCode == KeyEvent.KEYCODE_CAMERA || keyCode == 287) return false;


		int unicodeChar = event.getUnicodeChar();
		//toast("keyUp:" + keyCode);
		if(unicodeChar == 0 && keyCode < 714 && keyCode > 0) {
			unicodeChar = keyCode;
			if(OFActivity.LOG_INPUT) Log.i( "OF", "keyUp :" + keyCode + " unicodeChar:" + unicodeChar + " sourceID:" + event.getSource());
			return onKeyUp(keyCode, unicodeChar);
		} else {
			if(OFActivity.LOG_INPUT) Log.i( "OF", "keyUp :" + keyCode + " unicodeChar:" + unicodeChar + " sourceID:" + event.getSource());
			return onKeyUp(keyCode, unicodeChar);
		}
	}


}

