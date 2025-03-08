package cc.openframeworks;

import android.app.Activity;
import android.content.SharedPreferences;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.graphics.Insets;
import android.graphics.Point;
import android.graphics.Rect;
import android.hardware.display.DisplayManager;
import android.os.Build;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowCompat;

import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowInsets;
import android.view.WindowManager;
import android.view.WindowMetrics;
import android.view.accessibility.AccessibilityEvent;

import java.util.ArrayList;

import static android.opengl.EGL14.EGL_NO_CONTEXT;

import static cc.openframeworks.OFAndroidObject.activity;

import com.getkeepsafe.relinker.ReLinker;

public abstract class OFActivity extends Activity implements DisplayManager.DisplayListener{

	private static final String TAG = "OF";
	private DisplayManager displayManager;
	private Display display;
	private Display presentationDisplay;
	public static final boolean LOG_INPUT = false;
	public static final boolean LOG_ENGINE = false;

	public float currentRefreshRate = 0;
	public float highestRefreshRate = 0;

	public static final String KEY_SAMPLES   = "samples";
	public static final String KEY_TARGET_SAMPLES   = "targetmsaa";
	public static final String KEY_HIGHEST   = "highest";
	public static final String KEY_TARGET_FPS   = "fpstarget";

	public boolean hasPaused = false;
	public boolean hasSetup = false;

	public void onGLSurfaceCreated(){
		Log.v("OF","onGLSurfaceCreated");
	}
	public void onLoadPercent(float percent){}
	public void onUnpackingResourcesDone(){}
	
	//gesture handler member
	private ViewGroup mOFGlSurfaceContainer;
	public ViewGroup getSurfaceContainer(){
		return mOFGlSurfaceContainer;
	}

	public static String packageName = null;

	public void initView(){
		if(packageName == null) {
			packageName = this.getPackageName();
		}
        try {
        	if(LOG_ENGINE) Log.v("OF","trying to find class: "+packageName+".R$layout");
			// Get the resource ID of the layout dynamically
			int layoutResID = getResources().getIdentifier("main_layout", "layout", packageName);

			// Check if the resource ID is valid
			if (layoutResID == 0) {
				Log.e("TAG", "Could not find main_layout.xml.");
				throw new Exception();
			}
			// Inflate the layout using its resource ID
			View view = getLayoutInflater().inflate(layoutResID, null);
			// Check if the view was successfully inflated
			if (view == null) {
				Log.e("TAG", "Could not find main_layout.xml.");
				throw new Exception();
			}
			if(view == null) {
				Log.e("OF", "Could not find main_layout.xml.");
				throw new Exception();
			}
			this.setContentView(view);
			// Find the ViewGroup by its ID
			int containerID = getResources().getIdentifier("of_gl_surface_container", "id", packageName);
			mOFGlSurfaceContainer = null;
			mOFGlSurfaceContainer = findViewById(containerID);
			if (mOFGlSurfaceContainer == null) {
				Log.e(TAG, "Could not find of_gl_surface_container in main_layout.xml. Copy main_layout.xml from the latest empty example to fix this warning.");
				throw new Exception();
			}
			
		} catch (Exception e) {
			Log.w(TAG, "couldn't create view from layout falling back to GL only",e);
			mOFGlSurfaceContainer = new ConstraintLayout(this);
	        this.setContentView(mOFGlSurfaceContainer);
		}

        if(mOFGlSurfaceContainer != null) {
			mOFGlSurfaceContainer.requestLayout();
		}
	}

	public void resetView(){
		String packageName = this.getPackageName();
		try {
			if(LOG_ENGINE) Log.v(TAG,"trying to find class: "+packageName+".R$layout");
			Class<?> layout = Class.forName(packageName+".R$layout");
			View view = this.getLayoutInflater().inflate(layout.getField("main_layout").getInt(null),null);
			if(view == null) {
				Log.e(TAG, "Could not find main_layout.xml.");
				throw new Exception();
			}
			this.setContentView(view);

			Class<?> id = Class.forName(packageName+".R$id");
			mOFGlSurfaceContainer = (ViewGroup)this.findViewById(id.getField("of_gl_surface_container").getInt(null));

			if(mOFGlSurfaceContainer == null) {
				Log.e(TAG, "Could not find of_gl_surface_container in main_layout.xml. Copy main_layout.xml from latest empty example to fix this warning.");
				throw new Exception();
			}

		} catch (Exception e) {
			Log.w(TAG, "couldn't create view from layout falling back to GL only",e);
			mOFGlSurfaceContainer = new ConstraintLayout(this);
			this.setContentView(mOFGlSurfaceContainer);
		}
	}

	@Override
	protected void onCreate(Bundle arg0) {
		super.onCreate(arg0);

		try {
			OFAndroid.packageName = getPackageName();
		} catch (Exception ex) {
			Log.e(TAG, "Failure to getPackageName" + ex.getMessage());
		}

		if(LOG_ENGINE) Log.i(TAG, "onCreate:" + OFAndroid.packageName);

		try {
			SharedPreferences settings = getPreferences();
			int samples =  OFAndroid.samples;
			int maxSamples =  OFAndroid.maxSamples;
			int maximumFrameRate =  OFAndroid.maximumFrameRate;
			int targetFrameRate =  OFAndroid.targetFrameRate;
			if(settings != null) {
				maxSamples = settings.getInt(KEY_SAMPLES, OFAndroid.maxSamples);
				samples = settings.getInt(KEY_TARGET_SAMPLES, OFAndroid.samples);
				maximumFrameRate = settings.getInt(KEY_HIGHEST, OFAndroid.maximumFrameRate);
				targetFrameRate = settings.getInt(KEY_TARGET_FPS, OFAndroid.targetFrameRate);
			}

			if (Build.VERSION.SDK_INT < Build.VERSION_CODES.N) {
				Log.i(TAG, "SDK < N (24) : MSAA off by default ");
				samples = 1; // force no MSAA old devices
				maxSamples = 1;
			}
			else if (Build.VERSION.SDK_INT < Build.VERSION_CODES.O && (samples >= 2 && maxSamples >= 2)) {
				Log.i(TAG, "SDK < O (26): AAx2 by default");
				maxSamples=2; // force 2xMSAA old devices
				samples=2; // force 2xMSAA old devices
			}

			OFAndroid.maximumFrameRate = maximumFrameRate; // Restrict Android FPS to 144 FPS - 144 to 240 Hz displays will default to rendering at 120 Hz
			OFAndroid.targetFrameRate = targetFrameRate;
			OFAndroid.samples = samples;
			OFAndroid.maxSamples = maxSamples;

		} catch (Exception ex) {
			Log.e(TAG, "onCreate: SharedPreferences:getPreferences Failed!" + ex.getMessage());
		}

		try {
			WindowCompat.setDecorFitsSystemWindows(getWindow(), false);  // https://developer.android.com/training/gestures/edge-to-edge#lay-out-in-full-screen
		}catch (Exception ex) {
			Log.e(TAG, "onCreate: setDecorFitsSystemWindows: Failed!" + ex.getMessage());
		}
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
			displayManager = getSystemService(DisplayManager.class);
			displayManager.registerDisplayListener(this, null);
		}
		try {
			Log.i(TAG, "onCreate: Device:" + Build.MANUFACTURER.toLowerCase());
			if(OFAndroid.isHTCDevice()) OFAndroid.isDeviceHTC = true;
			if(OFAndroid.isSamsungDevice()) OFAndroid.isDeviceSamsung = true;
			if(OFAndroid.isHuaweiDevice()) OFAndroid.isDeviceHuawei = true;
			if(OFAndroid.isAmazonDevice()) OFAndroid.isDeviceAmazon = true;
			Log.i(TAG, "onCreate: Device: isDeviceSamsung:" + OFAndroid.isDeviceSamsung + " isDeviceHuawei:" + OFAndroid.isDeviceHuawei + "  OFAndroid.isDeviceAmazon:" +  OFAndroid.isDeviceAmazon);
		} catch (Exception ex) {
			Log.e(TAG, "onCreate: Device: Failed!" + ex.getMessage());
		}

		OFAndroidLifeCycle.setActivity(this);
		if(OFAndroidLifeCycle.coreLibraryLoaded == false) {
			LoadCoreStatic();
		} else {
			//Setup();
		}

		clearViewGroup();
		initView();

	}

	SharedPreferences getPreferences() {
		try {
			if(getApplicationContext() != null) {
				SharedPreferences settings = getApplicationContext().getSharedPreferences("of_settings", 0);
				return settings;
			} else {
				Log.e(TAG, "getPreferences() getApplicationContext == null");
				return null;
			}
		} catch(Exception ex) {
			Log.e(TAG, ex.getMessage());
			return null;
		}
	}

	public void savePreferences() {
		if(OFAndroidLifeCycle.coreLibraryLoaded == false || OFAndroidLifeCycle.getGLView() == null) {
			Log.e(TAG, "savePreferences() environment not setup");
			return;
		}
		try {
			SharedPreferences settings = getApplicationContext().getSharedPreferences("of_settings", 0);
			SharedPreferences.Editor prefsEditor = settings.edit();
			prefsEditor.putInt(KEY_SAMPLES, OFAndroid.maxSamples);
			prefsEditor.putInt(KEY_HIGHEST, OFAndroid.maximumFrameRate);
			prefsEditor.putInt(KEY_TARGET_FPS, OFAndroid.targetFrameRate);
			prefsEditor.putInt(KEY_TARGET_SAMPLES, OFAndroid.samples);
			prefsEditor.apply();
			Log.e(TAG, "savePreferences() done!");
		} catch (Exception ex){
			Log.d(TAG, "savePreferences() -> Exception saving pref" + ex.getMessage());
		}
	}

	void clearViewGroup() {


		ViewGroup glContainer = getSurfaceContainer();
		if(glContainer != null ) {
			if (glContainer.getChildCount() > 0) {
				if (LOG_ENGINE) Log.i(TAG, "clearViewGroup:views:" + glContainer.getChildCount());
				runOnUiThread(new Runnable() {
					@Override
					public void run() {
						ViewGroup glContainer = getSurfaceContainer();

						if (glContainer.getChildCount() > 1) {
							for (int i = glContainer.getChildCount() - 1; i >= 0; i--) {
								View view = glContainer.getChildAt(i);
								if (view != null) {
									glContainer.removeViewAt(i);
									Log.w(OFAndroidLifeCycle.class.getSimpleName(), "SetFirstFrameDrawn::Removing Extra View at Index " + i);
								}
							}
						}
					}
				});
			}
		}
	}

	public void LoadCoreStatic() {
		if(OFAndroidLifeCycle.coreLibraryLoaded == false) {
			ReLinker.loadLibrary(this, "openFrameworksAndroid", new ReLinker.LoadListener() {
				@Override
				public void success() {
					OFAndroidLifeCycle.coreLibraryLoaded = true;
					Setup();
				}
				@Override
				public void failure(Throwable t) {
					Log.e(TAG, "Failure to Load Core Static Library: " + t.getMessage());
				}
			});
		}
	}

	public void Setup() {
		if(LOG_ENGINE) Log.i(TAG, "OFAndroid.Setup:" + hasSetup + " | OFAndroidLifeCycle.coreLibraryLoaded:" + OFAndroidLifeCycle.coreLibraryLoaded + "| OFAndroidLifeCycle.appLibraryLoaded :" + OFAndroidLifeCycle.appLibraryLoaded);

		if(!hasSetup &&
				OFAndroidLifeCycle.appLibraryLoaded &&
				OFAndroidLifeCycle.coreLibraryLoaded
		) {
			hasSetup = true;
			
			this.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					if(OFAndroid.assetManager == null)
						OFAndroid.assetManager = activity.getApplicationContext().getAssets();
					OFAndroid.setAssetManager(OFAndroid.assetManager);
					OFAndroidLifeCycle.init();
					OFAndroidLifeCycle.glCreate();

					DetermineDisplayConfiguration(true);
					DetermineDisplayDimensions();
				}
			});


		} else if(!hasSetup && !OFAndroidLifeCycle.coreLibraryLoaded && OFAndroidLifeCycle.appLibraryLoaded == true) {
			LoadCoreStatic();
		} else if(hasSetup) {
			if(LOG_ENGINE) Log.i(TAG, "OFAndroid.Setup:true | OFAndroidLifeCycle.coreLibraryLoaded:" + OFAndroidLifeCycle.coreLibraryLoaded + "| OFAndroidLifeCycle.appLibraryLoaded :" + OFAndroidLifeCycle.appLibraryLoaded);
		}
	}


	public void DetermineDisplayDimensions() {

		try {
			OFGLSurfaceView glView = OFAndroidLifeCycle.getGLView();
			OFAndroid.enableOrientationChangeEvents();
			if(glView != null) {
				DisplayMetrics displayMetrics = new DisplayMetrics();
				WindowManager windowManager = getWindowManager();
				if(windowManager != null && getWindowManager().getDefaultDisplay() != null) {
					getWindowManager().getDefaultDisplay().getRealMetrics(displayMetrics);
					int height = displayMetrics.heightPixels;
					int width = displayMetrics.widthPixels;
					int width_px = Resources.getSystem().getDisplayMetrics().widthPixels;
					int height_px = Resources.getSystem().getDisplayMetrics().heightPixels;
					int pixeldpi = Resources.getSystem().getDisplayMetrics().densityDpi;
					float density = Resources.getSystem().getDisplayMetrics().density;


					Point point = new Point();
					getWindowManager().getDefaultDisplay().getRealSize(point);

					double x = Math.pow(point.x/ displayMetrics.xdpi, 2);
					double y = Math.pow(point.y / displayMetrics.ydpi, 2);
					double screenInches = Math.sqrt(x + y);
					Log.d(TAG, "Screen inches : " + screenInches +" with realSize:" + point.x +" height:"  + point.y);

					//Log.i("OF", "DisplayMetrics: w/h:" +width + "x" + height + " barHeight:" + heightBar + "x barWidth:" + widthBar + " bar:" + bar + " widthBar:" + barWidth + " densityDPI:"  +pixeldpi);
					if(LOG_ENGINE) Log.i("OF", "DisplayRealMetrics: w/h:" + width_px + "x" + height_px + " pixeldpi:" + pixeldpi + " density:" + density);
					//if(hasSetup)
						glView.setWindowResize(width, height);
				} else {
					throw new Exception("Display window problem");
				}
			}
		} catch (Exception exception) {
			Log.w("OF", "Could not get Window for Display ", exception);
		}
	}


	public static int getScreenWidth(@NonNull Activity activity) {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
			WindowMetrics windowMetrics = activity.getWindowManager().getCurrentWindowMetrics();
			Rect bounds = windowMetrics.getBounds();
			Insets insets = windowMetrics.getWindowInsets().getInsetsIgnoringVisibility(
					WindowInsets.Type.systemBars()
			);

			if (activity.getResources().getConfiguration().orientation
					== Configuration.ORIENTATION_LANDSCAPE
					&& activity.getResources().getConfiguration().smallestScreenWidthDp < 600
			) { // landscape and phone
				int navigationBarSize = insets.right + insets.left;
				return bounds.width() - navigationBarSize;
			} else { // portrait or tablet
				return bounds.width();
			}
		} else {
			DisplayMetrics outMetrics = new DisplayMetrics();
			activity.getWindowManager().getDefaultDisplay().getMetrics(outMetrics);
			return outMetrics.widthPixels;
		}
	}

	public static int getScreenHeight(@NonNull Activity activity) {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
			WindowMetrics windowMetrics = activity.getWindowManager().getCurrentWindowMetrics();
			Rect bounds = windowMetrics.getBounds();
			Insets insets = windowMetrics.getWindowInsets().getInsetsIgnoringVisibility(
					WindowInsets.Type.systemBars()
			);

			if (activity.getResources().getConfiguration().orientation
					== Configuration.ORIENTATION_LANDSCAPE
					&& activity.getResources().getConfiguration().smallestScreenWidthDp < 600
			) { // landscape and phone
				return bounds.height();
			} else { // portrait or tablet
				int navigationBarSize = insets.bottom;
				return bounds.height() - navigationBarSize;
			}
		} else {
			DisplayMetrics outMetrics = new DisplayMetrics();
			activity.getWindowManager().getDefaultDisplay().getMetrics(outMetrics);
			return outMetrics.heightPixels;
		}
	}


	public void DetermineDisplayDimensionsConfigChange(Configuration config) {

		try {
			OFGLSurfaceView glView = OFAndroidLifeCycle.getGLView();
			OFAndroid.enableOrientationChangeEvents();
			//DisplayMetrics systemDisplayMetrics = Resources.getSystem().getDisplayMetrics();
			if(glView != null) {
				DisplayMetrics displayMetrics = new DisplayMetrics();
				WindowManager windowManager = getWindowManager();
				if(windowManager != null && getWindowManager().getDefaultDisplay() != null) {
					getWindowManager().getDefaultDisplay().getRealMetrics(displayMetrics);
					int height = displayMetrics.heightPixels;
					int width = displayMetrics.widthPixels;

					float density = displayMetrics.density;
					//int width = (int)(config.screenWidthDp * density);
					//int height = (int)(config.screenHeightDp * density);
					if(LOG_ENGINE) Log.i("OF", "DetermineDisplayDimensionsConfigChange: w/h:" + width + "x" + height + " density:" + density);

					glView.setWindowResize(getScreenWidth(this), getScreenHeight(this));
				} else {
					throw new Exception("Display window problem");
				}
			}
		} catch (Exception exception) {
			Log.w("OF", "Could not get Window for Display ", exception);
		}
	}

	public void DetermineDisplayConfiguration(boolean allowSetFrameRate) {
		if(LOG_ENGINE) Log.i("OF", "DetermineDisplayConfiguration RefreshRate :" +  currentRefreshRate);

		int modeID = 0;
		int highestModeID = 0;
		try {
			display = getWindowManager().getDefaultDisplay();
			if(display.isValid()) {
				if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
					boolean isWideColorGamut = display.isWideColorGamut();
					if(LOG_ENGINE) Log.i("OF", "Display WideColor Gamut Supported:" +  isWideColorGamut);
					OFAndroid.wideGamut = isWideColorGamut;
					boolean isHDR = display.isHdr();
					Log.i("OF", "Display is HDR Supported:" +  isHDR);
					OFAndroid.hdrScreen = isHDR;
				}
				currentRefreshRate = display.getRefreshRate();
				if(LOG_ENGINE) Log.i("OF", "Display Current RefreshRate :" +  currentRefreshRate);

				if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
					if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
						try {
							Display.Mode[] modes = display.getSupportedModes();
							Display.Mode currentMode = display.getMode();
							modeID = currentMode.getModeId();
							if(LOG_ENGINE) Log.i("OF", "Display Mode: CurrentMode:" + currentMode + " refreshRate: [" + currentMode.getRefreshRate() + "] mode PhysicalWidth:[" + currentMode.getPhysicalWidth() + "] mode PhysicalHeight:[" + currentMode.getPhysicalHeight() + "]");

							if (currentRefreshRate != currentMode.getRefreshRate()) {
								Log.e("OF", "Display Mode: Current Display Mode Refresh Rate not equal to current :" + currentMode + " refreshRate: [" + currentMode.getRefreshRate() + "] displayRefreshRate: " + currentRefreshRate);
							}
							currentRefreshRate = currentMode.getRefreshRate();
							for (Display.Mode mode : modes) {
								if(LOG_ENGINE) Log.i("OF", "Display Mode: Supported:" + mode + " refreshRate: [" + mode.getRefreshRate() + "] mode PhysicalWidth:[" + mode.getPhysicalWidth() + "] mode PhysicalHeight:[" + mode.getPhysicalHeight() + "]" + " <=Max:"+OFAndroid.maximumFrameRate + ":" + ( mode.getRefreshRate() <= OFAndroid.maximumFrameRate));
								if (mode.getRefreshRate() >= highestRefreshRate && mode.getRefreshRate() <= OFAndroid.maximumFrameRate ) {
									highestRefreshRate = mode.getRefreshRate();
									if (highestRefreshRate <= OFAndroid.maximumFrameRate) {
										highestModeID = mode.getModeId();
										Log.i("OF", "Display Mode Refresh Rate: Supported:" + mode + " refreshRate: [" + mode.getRefreshRate() + "]");
									}
								}
							}
						} catch (Exception ex) {
							Log.e("OF", "Display Mode Exception:", ex);
						}
					} else { // pre 23 Display Mode
						try {
							float[] refreshRates = display.getSupportedRefreshRates();
							for (float refreshRate : refreshRates) {
								if(LOG_ENGINE) Log.i("OF", "Display RefreshRate Supported:" + refreshRate + " <=Max:" + (refreshRate <= OFAndroid.maximumFrameRate));
								if (refreshRate >= highestRefreshRate && (refreshRate <= OFAndroid.maximumFrameRate)) {
									highestRefreshRate = refreshRate;
								}
							}
						} catch (Exception ex) {
							Log.e("OF", "Display Mode Exception:", ex);
						}
					}

					if(currentRefreshRate >= highestRefreshRate) {
						highestRefreshRate = currentRefreshRate;
						highestModeID = modeID;
					}

					if(allowSetFrameRate) {
						OFGLSurfaceView glView = OFAndroidLifeCycle.getGLView();
						if (glView != null) {

							if (highestRefreshRate > OFAndroid.maximumFrameRate) {
								highestRefreshRate = OFAndroid.maximumFrameRate; // allow capping
							}

							try {
								Window window = activity.getWindow();
								WindowManager.LayoutParams params = window.getAttributes();
								if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
									params.preferredDisplayModeId = highestModeID;
								}
								window.setAttributes(params);
							}catch(Exception ex) {
								Log.w("OF", "Setting ModeID:"+ ex.getMessage());
							}
							
							glView.setFrameRate(highestRefreshRate);
							currentRefreshRate = highestRefreshRate;
						}
					} else {
						highestRefreshRate = currentRefreshRate;
					}

				}
			} else {
				Log.w("OF", "Display is not valid yet");
			}

		} catch (Exception ex) {
			Log.w("OF", "Could not get Window for Display ", ex);
		}

		if(hasSetup) {
			OFAndroid.targetFrameRate = (int)currentRefreshRate;
			OFAndroid.deviceRefreshRate((int) currentRefreshRate);
			OFAndroid.deviceHighestRefreshRate((int) highestRefreshRate);
			OFAndroid.setSampleSize(OFAndroid.samples);
		}
	}


	@Override
	public void onDisplayAdded(int i) {
		checkForPresentationDisplays();
	}

	@Override
	public void onDisplayRemoved(int i) {
		checkForPresentationDisplays();
	}

	@Override
	public void onDisplayChanged(int i) {
		//if(hasPaused) return;
		Log.w("OF", "onDisplayChanged:" + i);
		checkForPresentationDisplays();
		try {
			display = getWindowManager().getDefaultDisplay();
			if (display != null && display.isValid()) {
				boolean allowFPSChange = i != 0;
				DetermineDisplayConfiguration(allowFPSChange);
				DetermineDisplayDimensions();
			}
		} catch(Exception exception) {
			Log.w("OF", "onDisplayChanged Display Could not get Window for Display ", exception);
		}
	}

	private void checkForPresentationDisplays() {
		if (displayManager!=null) {
			Display[] displays = displayManager.getDisplays(DisplayManager.DISPLAY_CATEGORY_PRESENTATION);
			if (displays.length>0) {
				presentationDisplay=displays[0];
				if(presentationDisplay != null) {
					DisplayMetrics metrics = new DisplayMetrics();
					presentationDisplay.getRealMetrics(metrics);
					int realHeight = metrics.heightPixels;
					int realWidth = metrics.heightPixels;
					if(LOG_ENGINE) Log.i("OF", "checkForPresentationDisplays found external display: isValid:" + presentationDisplay.isValid() + " name:" + presentationDisplay.getName() + " refreshRate:" + presentationDisplay.getRefreshRate() + " Resolution:" + realWidth + "x" + realHeight);
				}
			}
			else {
				presentationDisplay=null;
			}
		}
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {

		super.onConfigurationChanged(newConfig);
		try {
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
				if(LOG_ENGINE) Log.i("OF", "onConfigurationChanged() " + getRequestedOrientation() + " newConfig dpi:" + newConfig.densityDpi + " screenLayout:" + newConfig.screenLayout + " screenWidthDp:" + newConfig.screenWidthDp + " screenHeightDp:" + newConfig.screenHeightDp + " isScreenWideColorGamut:" + newConfig.isScreenWideColorGamut());
			} else {
				Log.i("OF", "onConfigurationChanged() " + getRequestedOrientation() + " newConfig dpi:" + newConfig.densityDpi + " screenLayout:" + newConfig.screenLayout + " screenWidthDp:" + newConfig.screenWidthDp + " screenHeightDp:" + newConfig.screenHeightDp + " isScreenWideColorGamut:" + false);
			}
			if(newConfig.locale != OFAndroid.locale) {
				if(LOG_ENGINE) Log.i("OF", "onConfigurationChanged() Locale has changed to:" + newConfig.locale.getDisplayLanguage());
			}
		} catch (Exception ex) {
			Log.e("OF", "Display Mode Exception:", ex);
		}
		int ofOrientation  = 0;
		switch (display.getRotation()) {
			case Surface.ROTATION_90:
				ofOrientation = 3;
				break;
			case Surface.ROTATION_180:
				ofOrientation = 2;
				break;
			case Surface.ROTATION_270:
				ofOrientation = 4;
				break;
			case Surface.ROTATION_0:
			default:
				ofOrientation = 1;
				break;
		}
		if (newConfig.orientation == Configuration.ORIENTATION_LANDSCAPE) {
			ofOrientation = 3;
		} else if (newConfig.orientation == Configuration.ORIENTATION_PORTRAIT){
			ofOrientation = 0;
		}

		if(hasSetup) {
			OFAndroid.deviceOrientationChanged(ofOrientation);
			DetermineDisplayConfiguration(false);
			DetermineDisplayDimensions();
			DetermineDisplayDimensionsConfigChange(newConfig);
		}

	}

	public boolean showNavigationBar(Resources resources) {
		int id = resources.getIdentifier("config_showNavigationBar", "bool", "android");
		return id > 0 && resources.getBoolean(id);
	}


	// These are handled in the View
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		return true;
	}
	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {

		return true;

	}
	@Override
	public boolean onGenericMotionEvent(MotionEvent event) {
		return true;
	}
	@Override
	public boolean onKeyMultiple(int keyCode, int count, KeyEvent event) {
		return true;
	}
	// -- View
	
	
	@Override
	protected void onStart() {
		if(LOG_ENGINE) Log.i("OF", "onStart");
		super.onStart();
		//OFAndroidLifeCycle.glStart();

		try {
			display = getWindowManager().getDefaultDisplay();
			if (display.isValid()) {
				DetermineDisplayConfiguration(true);
			}

		} catch(Exception exception) {
			Log.w("OF", "onStart Display Could not get Window for Display ", exception);
		}

	}
	
	@Override
	protected void onStop() {
		if(LOG_ENGINE) Log.i("OF", "onStop");
		hasPaused = true;

		OFGLSurfaceView glView = OFAndroidLifeCycle.getGLView();
		if(glView != null) {
			if (Build.VERSION.SDK_INT != Build.VERSION_CODES.P) {
			Log.i("OF", "onStop GLView setVisibility GONE");
				//glView.setVisibility(View.GONE);
			}
		}


		super.onStop();

	}

	public void OnBackPressed() {
		onBackPressed();
	}
	
	@Override
	protected void onRestart() {
		OFAndroidLifeCycle.setActivity(this);
		if(LOG_ENGINE) Log.i("OF", "onRestart");
		super.onRestart();
		OFAndroidLifeCycle.glRestart();
		DetermineDisplayConfiguration(true);
	}

	public void onForceRestart() {
		if(LOG_ENGINE) Log.i("OF", "onForceRestart");
		savePreferences();
		OFAndroid.setupGL(OFAndroid.eglVersion, true);
		DetermineDisplayConfiguration(true);
		DetermineDisplayDimensions();
		if(mOFGlSurfaceContainer == null) {
			Log.e("OF", "onResume mOFGlSurfaceContainer is null - glCreateSurface");
		}
		OFAndroidLifeCycle.glResume(mOFGlSurfaceContainer);
	}

	
	@Override
	protected void onResume() {
		if(LOG_ENGINE)Log.i("OF", "onResume");
		OFAndroidLifeCycle.setActivity(this);

		super.onResume();

		if(hasSetup == false) {
			Log.i("OF", "onResume hasSetup == false");
			return;
		}

		if(OFAndroidLifeCycle.isInit() == true && OFAndroidLifeCycle.firstFrameDrawn() == true &&
				android.opengl.EGL14.eglGetCurrentContext() == EGL_NO_CONTEXT) {
			hasPaused = true;
		}

		if(OFAndroidLifeCycle.firstFrameDrawn()) {
			Log.i("OF", "onResume firstFrameDrawn");
		}

		if( hasPaused == false) {
			Log.i("OF", "onResume hasPaused == false");
			return;
		}

		if(OFAndroid.assetManager == null)
			OFAndroid.assetManager = activity.getApplicationContext().getAssets();
		OFAndroid.setAssetManager(OFAndroid.assetManager);

		if(android.opengl.EGL14.eglGetCurrentContext() == EGL_NO_CONTEXT){
			Log.e("OF", "onResume eglGetCurrentContext = EGL_NO_CONTEXT BAD");
			OFAndroid.setupGL(OFAndroid.eglVersion, true);
		}
		if(OFAndroidLifeCycle.isInit() && mOFGlSurfaceContainer == null) {
			if(LOG_ENGINE) Log.i("OF", "onResume mOFGlSurfaceContainer is null");
		}
		//
		if(OFAndroidLifeCycle.isInit() && OFAndroidLifeCycle.getGLView() == null) {
			if(LOG_ENGINE) Log.i("OF", "onResume getGLView is null - glCreateSurface");
			//OFAndroidWindow.exit();
			OFAndroid.setupGL(OFAndroid.eglVersion, true);
		}

		if(OFAndroidLifeCycle.isInit() && OFAndroidLifeCycle.getGLView() != null && OFAndroidLifeCycle.getGLView().getDisplay() == null) {
			if(LOG_ENGINE) Log.i("OF", "onResume getGLView is null - glCreateSurface");
//			OFAndroidWindow.exit();
			OFAndroid.setupGL(OFAndroid.eglVersion, true);
		}

		DetermineDisplayConfiguration(true);
		DetermineDisplayDimensions();
		if(mOFGlSurfaceContainer == null) {
			Log.e("OF", "onResume mOFGlSurfaceContainer is null - glCreateSurface");
		}
		OFAndroidLifeCycle.glResume(mOFGlSurfaceContainer);

		hasPaused = false;

	}
	@Override
	protected void onPause() {
		if(LOG_ENGINE) Log.i("OF", "onPause");
		hasPaused = true;
		OFAndroidLifeCycle.glPause();

		super.onPause();
	}


	@Override
	public void onWindowFocusChanged(boolean hasFocus) {
		super.onWindowFocusChanged(hasFocus);
		OFGLSurfaceView glView = OFAndroidLifeCycle.getGLView();
		if (hasFocus && glView != null && glView.getVisibility() == View.GONE) {
			if(LOG_ENGINE) Log.i("OF", "onWindowFocusChanged GLView setVisibility VISIBLE");
			OFAndroidLifeCycle.glResume(mOFGlSurfaceContainer);
			glView.setVisibility(View.VISIBLE);
			DetermineDisplayConfiguration(true);
			DetermineDisplayDimensions();
		}
	}

	@Override
	protected void onDestroy() {
		if(LOG_ENGINE) Log.e("OF", "onDestroy");
		hasPaused = true;
		OFGLSurfaceView glView = OFAndroidLifeCycle.getGLView();
		if(glView != null) {
			//glView.setVisibility(View.GONE);
		}
		OFAndroidLifeCycle.glDestroy();
		if (displayManager!=null) {
			displayManager.unregisterDisplayListener(this);
		}
		OFAndroidLifeCycle.coreLibraryLoaded = false;
		OFAndroidLifeCycle.appLibraryLoaded = false;
		super.onDestroy();
	}
	
	

	@RequiresApi(28)
	private static class OnUnhandledKeyEventListenerWrapper implements View.OnUnhandledKeyEventListener {
		private ViewCompat.OnUnhandledKeyEventListenerCompat mCompatListener;

		OnUnhandledKeyEventListenerWrapper(ViewCompat.OnUnhandledKeyEventListenerCompat listener) {
			this.mCompatListener = listener;
		}

		public boolean onUnhandledKeyEvent(View v, KeyEvent event) {
			if(LOG_ENGINE) Log.i("OF", "OFActivity:onUnhandledKeyEvent" + " event:" + event.toString());
			if ((event.getKeyCode() == KeyEvent.KEYCODE_BACK  || event.getKeyCode() == KeyEvent.KEYCODE_MENU || event.getKeyCode() == KeyEvent.KEYCODE_BUTTON_MODE)) {
				return true;
			}
			return this.mCompatListener.onUnhandledKeyEvent(v, event);
		}
	}


	public ArrayList<Integer> getGameControllerIds() {
		ArrayList<Integer> gameControllerDeviceIds = new ArrayList<Integer>();
		int[] deviceIds = InputDevice.getDeviceIds();
		for (int deviceId : deviceIds) {
			InputDevice dev = InputDevice.getDevice(deviceId);
			int sources = dev.getSources();

			// Verify that the device has gamepad buttons, control sticks, or both.
			if (((sources & InputDevice.SOURCE_GAMEPAD) == InputDevice.SOURCE_GAMEPAD)
					|| ((sources & InputDevice.SOURCE_JOYSTICK)
					== InputDevice.SOURCE_JOYSTICK)) {
				// This device is a game controller. Store its device ID.
				if (!gameControllerDeviceIds.contains(deviceId)) {
					gameControllerDeviceIds.add(deviceId);
				}
			}
		}
		return gameControllerDeviceIds;
	}


	@Override
	public boolean dispatchGenericMotionEvent(MotionEvent ev) {
		if(LOG_ENGINE) Log.i("OF", "dispatchGenericMotionEvent" + " event:" + ev.toString());
		return OFAndroidController.genericMotionEvent(ev);
	}




	@Override
	public boolean dispatchKeyEvent (KeyEvent event){
		if(!hasSetup) return true;
		int deviceId = event.getDeviceId();
		if(LOG_INPUT) Log.i("OF", "dispatchKeyEvent" + " event:" + event.toString() + " deviceID:" + deviceId + " source:" + event.getSource());
		boolean returnValue = true;
		int sourceID = event.getSource();
		if ((event.getSource() & InputDevice.SOURCE_KEYBOARD) == InputDevice.SOURCE_KEYBOARD &&
				(event.getSource() & InputDevice.SOURCE_GAMEPAD) != InputDevice.SOURCE_GAMEPAD &&
				(event.getSource() & InputDevice.SOURCE_JOYSTICK) != InputDevice.SOURCE_JOYSTICK &&
				(event.getSource() & InputDevice.SOURCE_CLASS_JOYSTICK) != InputDevice.SOURCE_CLASS_JOYSTICK &&
				(event.getSource() & InputDevice.SOURCE_TOUCHPAD) != InputDevice.SOURCE_TOUCHPAD &&
				(event.getSource() & InputDevice.SOURCE_DPAD) != InputDevice.SOURCE_DPAD &&
				(event.getDevice() != null && event.getDevice().getVendorId() != OFAndroidController.VendorValve)){

			if (event.getAction() == KeyEvent.ACTION_DOWN && (event.getKeyCode() == KeyEvent.KEYCODE_BACK  || event.getKeyCode() == KeyEvent.KEYCODE_MENU || event.getKeyCode() == KeyEvent.KEYCODE_BUTTON_MODE) && event.getRepeatCount() == 0) {
				if( OFAndroid.onBackPressed() ) {
					returnValue = true;
				} else {
					Log.w("OF", "dispatchKeyEvent" + " event KEYCODE_BACK - Exiting");
					//super.onBackPressed();
					hasSetup = false;
					finishAffinity();
					System.exit(0);
					return false;
				}
			}

			if((event.getKeyCode() == KeyEvent.KEYCODE_VOLUME_UP || (event.getKeyCode() == KeyEvent.KEYCODE_VOLUME_DOWN)) || (event.getKeyCode() == KeyEvent.KEYCODE_VOLUME_MUTE) || (event.getKeyCode() == KeyEvent.KEYCODE_FOCUS)  || event.getKeyCode() == KeyEvent.KEYCODE_CAMERA || event.getKeyCode() == 287) {
				return false;
			}

			if(OFAndroid.lastInputID == -1 && event.getDevice() != null ||
					event.getDevice() != null && OFAndroid.lastInputID != event.getDeviceId() &&
							OFAndroid.lastInputVendorID != event.getDevice().getVendorId()) {
				OFAndroid.lastInputDevice = event.getDevice(); // not sure why this happens
				OFAndroid.lastInputDescriptor = event.getDevice().getDescriptor();
				OFAndroid.lastControllerType = OFAndroidController.getControllerType(event.getDevice());
				OFAndroid.lastInputVendorID = OFAndroid.lastInputDevice.getVendorId();
				OFAndroid.lastInputProductID = OFAndroid.lastInputDevice.getProductId();
				if(OFAndroid.lastInputVendorID == 0 && OFAndroid.lastInputProductID == 0 ) {
					OFAndroid.lastInputProductID = OFAndroidController.ProductID_PS5;
					OFAndroid.lastInputVendorID = OFAndroidController.VendorPS;
				}
			}
			if(LOG_INPUT && OFAndroid.lastInputDevice != null) {
				OFAndroid.toast("KB:" + OFAndroid.lastInputDevice.getName() + " keycode:" + event.getKeyCode() + "vendor: " + OFAndroid.lastInputDevice.getVendorId() + "PID:" + OFAndroid.lastInputProductID);
				Log.i("OF", "Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:" + OFAndroid.lastInputDevice.getVendorId() + "PID:" + OFAndroid.lastInputProductID);
			}

			if(event.getAction() == KeyEvent.ACTION_DOWN)
				returnValue = OFAndroid.keyDown(event.getKeyCode(), event);
			else if(event.getAction() == KeyEvent.ACTION_UP)
				returnValue = OFAndroid.keyUp(event.getKeyCode(), event);
			else if(event.getAction() == KeyEvent.ACTION_MULTIPLE)
				returnValue = OFAndroid.keyDown(event.getKeyCode(), event);
			else if(event.getAction() == KeyEvent.FLAG_CANCELED)
				returnValue = OFAndroid.keyUp(event.getKeyCode(), event);
			else
				returnValue = true;

		}
		else if ((event.getSource() & InputDevice.SOURCE_DPAD) == InputDevice.SOURCE_DPAD ||
				(event.getSource() & InputDevice.SOURCE_GAMEPAD) == InputDevice.SOURCE_GAMEPAD ||
				(event.getSource() & InputDevice.SOURCE_CLASS_JOYSTICK) == InputDevice.SOURCE_CLASS_JOYSTICK ||
				(event.getSource() & InputDevice.SOURCE_CLASS_BUTTON) == InputDevice.SOURCE_CLASS_BUTTON ||
				(event.getSource() & InputDevice.SOURCE_DPAD) > InputDevice.SOURCE_DPAD - 13 && (event.getSource() & InputDevice.SOURCE_DPAD)  < InputDevice.SOURCE_GAMEPAD  + 500
		) {
			int keyCode = event.getKeyCode();
			if(OFAndroid.lastInputID == -1 && event.getDevice() != null ||
					event.getDevice() != null && OFAndroid.lastDeviceID != event.getDeviceId() &&
							OFAndroid.lastInputVendorID != event.getDevice().getVendorId()) {
				OFAndroid.lastInputDevice = event.getDevice(); // not sure why this happens
				OFAndroid.lastInputID = event.getSource();
				OFAndroid.lastDeviceID = event.getDeviceId();
				OFAndroid.lastInputDescriptor = event.getDevice().getDescriptor();
				OFAndroid.lastControllerType = OFAndroidController.getControllerType(event.getDevice());
				OFAndroid.lastInputVendorID = OFAndroid.lastInputDevice.getVendorId();
				OFAndroid.lastInputProductID = OFAndroid.lastInputDevice.getProductId();
				if(OFAndroid.lastInputVendorID == 0 && OFAndroid.lastInputProductID == 0 ) {
					OFAndroid.lastInputProductID = OFAndroidController.ProductID_PS5;
					OFAndroid.lastInputVendorID = OFAndroidController.VendorPS;
				}
			}
			if(event.getDevice() == null && OFAndroid.lastInputDevice == null) {
				OFAndroid.lastInputDevice = OFAndroidController.getGameControllerForID(event.getDeviceId()); // fall back
				OFAndroid.lastControllerType = OFAndroidController.getControllerType(event.getDevice());
			}
			if (Build.VERSION.SDK_INT < Build.VERSION_CODES.S &&
					!OFAndroid.isDeviceAmazon && !OFAndroid.isHuaweiDevice() && !OFAndroid.isHTCDevice()
					||
					Build.VERSION.SDK_INT == Build.VERSION_CODES.S
					&& OFAndroid.isDeviceSamsung
					&& !OFAndroid.isDeviceAmazon
			) { // Android 12 fixes PS5 bug
				if(LOG_INPUT && OFAndroid.lastInputDevice != null) {
					OFAndroid.toast(OFAndroid.lastInputDevice.getName() + " keycode:" + keyCode + "vendor: " + OFAndroid.lastInputDevice.getVendorId() + "PID:" + OFAndroid.lastInputProductID);
					Log.i("OF", "Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:" + OFAndroid.lastInputDevice.getVendorId() + "PID:" + OFAndroid.lastInputProductID);

				}
				if (OFAndroid.lastInputDevice != null &&
						OFAndroid.lastInputProductID == OFAndroidController.ProductID_PS5 ||
						!(OFAndroid.lastInputProductID == OFAndroidController.ProductID_PS4_2) && !(OFAndroid.lastInputProductID == OFAndroidController.ProductID_PS4) &&
						(OFAndroid.lastInputVendorID == OFAndroidController.VendorPS &&
								(OFAndroid.lastInputDevice.getName().equals(OFAndroidController.PS5_Controller_NAME) || OFAndroid.lastInputDevice.getName().equals(OFAndroidController.PS5_Controller_NAME_GENERIC))
						)) {
					if (keyCode == 97) { // flips X to Square to fix HID issues for PS5
						keyCode = 96;
						if(LOG_INPUT && OFAndroid.lastInputDevice != null)
							Log.i("OF", "PS5 Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:" + OFAndroid.lastInputDevice.getVendorId() + " 97->96");
					} else if (keyCode == 96) {
						keyCode = 99;
						if(LOG_INPUT && OFAndroid.lastInputDevice != null)
							Log.i("OF", "PS5 Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:" + OFAndroid.lastInputDevice.getVendorId() + " 96->97");
					} else if (keyCode == 100) {
						if(LOG_INPUT && OFAndroid.lastInputDevice != null)
							Log.i("OF", "PS5 Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:" + OFAndroid.lastInputDevice.getVendorId() + " 100->102");
						keyCode = 102;
					} else if (keyCode == 102) {
						if(LOG_INPUT && OFAndroid.lastInputDevice != null)
							Log.i("OF", "PS5 Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:" + OFAndroid.lastInputDevice.getVendorId() + " 102->100");
						keyCode = 100;
					} else if (keyCode == 101) {
						keyCode = 103;
						if(LOG_INPUT && OFAndroid.lastInputDevice != null)
							Log.i("OF", "PS5 Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:" + OFAndroid.lastInputDevice.getVendorId() + " 101->103");
					} else if (keyCode == 103) {
						if(LOG_INPUT && OFAndroid.lastInputDevice != null)
							Log.i("OF", "PS5 Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:" + OFAndroid.lastInputDevice.getVendorId() + " 103->101");
						keyCode = 101;
					} else if (keyCode == 99) {
						if(LOG_INPUT && OFAndroid.lastInputDevice != null)
							Log.i("OF", "PS5 Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:" + OFAndroid.lastInputDevice.getVendorId() + " 99->100");
						keyCode = 100;
					}  else if (keyCode == 104) {
						if(LOG_INPUT && OFAndroid.lastInputDevice != null)
							Log.i("OF", "PS5 Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:" + OFAndroid.lastInputDevice.getVendorId() + " 99->100");
						keyCode = 109;
					} else if (keyCode == 105) {
						if(LOG_INPUT && OFAndroid.lastInputDevice != null)
							Log.i("OF", "PS5 Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:" + OFAndroid.lastInputDevice.getVendorId() + " 99->100");
						keyCode = 108;
					}
				} else {
					if(LOG_INPUT && OFAndroid.lastInputDevice != null) {
						OFAndroid.toast(OFAndroid.lastInputDevice.getName() + "| keycode:" + keyCode);
						Log.i("OF", "Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:" + OFAndroid.lastInputDevice.getVendorId() + "PID:" + OFAndroid.lastInputProductID);

					}
					if(LOG_INPUT && OFAndroid.lastInputDevice != null)
						Log.i("OF", "Not PS5 Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:" + OFAndroid.lastInputDevice.getVendorId());
					else
						Log.i("OF", "Not PS5 Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:" + OFAndroid.lastInputDevice.getVendorId());

				}
			} else if(OFAndroid.isHuaweiDevice() && !OFAndroid.isAmazonDevice()) {
				if(LOG_INPUT && OFAndroid.lastInputDevice != null)
					OFAndroid.toast(OFAndroid.lastInputDevice.getName() + " keycode:" + keyCode);

				if (OFAndroid.lastInputDevice != null &&
						OFAndroid.lastInputProductID == OFAndroidController.ProductID_PS5 ||
						!(OFAndroid.lastInputProductID == OFAndroidController.ProductID_PS4_2) && !(OFAndroid.lastInputProductID == OFAndroidController.ProductID_PS4) &&
						(OFAndroid.lastInputVendorID == OFAndroidController.VendorPS && (OFAndroid.lastInputDevice.getName().equals(OFAndroidController.PS5_Controller_NAME) || OFAndroid.lastInputDevice.getName().equals(OFAndroidController.PS5_Controller_NAME_GENERIC)
						))) {
					if (keyCode == 102) {
						if(LOG_INPUT && OFAndroid.lastInputDevice != null)
							Log.i("OF", "PS5 Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:" + OFAndroid.lastInputDevice.getVendorId() + " 102->100");
						keyCode = 100;
					} else if (keyCode == 101) {
						keyCode = 103;
						if(LOG_INPUT && OFAndroid.lastInputDevice != null)
							Log.i("OF", "PS5 Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:" + OFAndroid.lastInputDevice.getVendorId() + " 101->103");
					} else if (keyCode == 103) {
						if(LOG_INPUT && OFAndroid.lastInputDevice != null)
							Log.i("OF", "PS5 Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:" + OFAndroid.lastInputDevice.getVendorId() + " 103->101");
						keyCode = 101;
					}
				}
			} else if(OFAndroid.isAmazonDevice()) {
				if(LOG_INPUT && OFAndroid.lastInputDevice != null)
					OFAndroid.toast(OFAndroid.lastInputDevice.getName() + " keycode:" + keyCode);

				if(OFAndroid.lastInputProductID == OFAndroidController.ProductID_PS5 ||
						!(OFAndroid.lastInputProductID == OFAndroidController.ProductID_PS4_2) &&
								(OFAndroid.lastInputVendorID == OFAndroidController.VendorPS &&
										(OFAndroid.lastInputDevice.getName().equals(OFAndroidController.PS5_Controller_NAME) || OFAndroid.lastInputDevice.getName().equals(OFAndroidController.PS5_Controller_NAME_GENERIC))
								)) {
//					if (keyCode == 96) { // X on Amazon
//						if(LOG_INPUT && OFAndroid.lastInputDevice != null)
//							Log.i("OF", "AMAZON PS5 Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:"
//									+ OFAndroid.lastInputDevice.getVendorId() + " 96->97");
//						keyCode = 97; // to X
//					} else if (keyCode == 99) { // ▢ on Amazon
//						if(LOG_INPUT && OFAndroid.lastInputDevice != null)
//							Log.i("OF", "AMAZON PS5 Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:"
//									+ OFAndroid.lastInputDevice.getVendorId() + " 99->96");
//						keyCode = 96; // ▢
//					}
					if (keyCode == 100) { // △ on Amazon
						if(LOG_INPUT && OFAndroid.lastInputDevice != null)
							Log.i("OF", "PS5 Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:"
									+ OFAndroid.lastInputDevice.getVendorId() + " 100->99");
						keyCode = 99;
					}
//					else if (keyCode == 102) {
//						if(LOG_INPUT && OFAndroid.lastInputDevice != null)
//							Log.i("OF", "AMAZON PS5 Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:"
//									+ OFAndroid.lastInputDevice.getVendorId() + " 102->100");
//						keyCode = 100;
//					}
					else if (keyCode == 101) {
						keyCode = 103;
						if(LOG_INPUT && OFAndroid.lastInputDevice != null)
							Log.i("OF", "AMAZON PS5 Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:"
									+ OFAndroid.lastInputDevice.getVendorId() + " 101->103");
					}
//					else if (keyCode == 103) {
//						if(LOG_INPUT && OFAndroid.lastInputDevice != null)
//							Log.i("OF", "AMAZON PS5 Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:"
//									+ OFAndroid.lastInputDevice.getVendorId() + " 103->101");
//						keyCode = 101;
//					}
					else if (keyCode == 4) { // share
						if(LOG_INPUT && OFAndroid.lastInputDevice != null)
							Log.i("OF", "AMAZON PS5 Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:"
									+ OFAndroid.lastInputDevice.getVendorId() + " 4->109");
						keyCode = 109;
					}
				}
			}
			if(OFAndroid.lastInputID == -1 && event.getDevice() != null ||
					event.getDevice() != null && OFAndroid.lastDeviceID != event.getDeviceId() &&
							OFAndroid.lastInputVendorID != event.getDevice().getVendorId()) {
				OFAndroid.lastInputDevice = event.getDevice(); // not sure why this happens
				OFAndroid.lastInputID = event.getSource();
				OFAndroid.lastDeviceID = event.getDeviceId();
				OFAndroid.lastInputDescriptor = event.getDevice().getDescriptor();
				OFAndroid.lastControllerType = OFAndroidController.getControllerType(event.getDevice());
				OFAndroid.lastInputVendorID = OFAndroid.lastInputDevice.getVendorId();
				OFAndroid.lastInputProductID = OFAndroid.lastInputDevice.getProductId();
				if(OFAndroid.lastInputVendorID == 0 && OFAndroid.lastInputProductID == 0 ) {
					OFAndroid.lastInputProductID = OFAndroidController.ProductID_PS5;
					OFAndroid.lastInputVendorID = OFAndroidController.VendorPS;
				}
			}
			if (event.getAction() == KeyEvent.ACTION_DOWN && (event.getKeyCode() == KeyEvent.KEYCODE_BACK  || event.getKeyCode() == KeyEvent.KEYCODE_MENU || event.getKeyCode() == KeyEvent.KEYCODE_BUTTON_MODE || event.getKeyCode() == 0 ) && event.getRepeatCount() == 0) {
				if( OFAndroid.onBackPressed() ) {
					returnValue = true;
				} else {
					Log.w("OF", "dispatchKeyEvent" + " event KEYCODE_BACK - Exiting");
					//super.onBackPressed();
					hasSetup = false;
					finishAffinity();
					System.exit(0);
					return false;
				}
			}
			if(event.getAction() == KeyEvent.ACTION_DOWN)
				returnValue = OFAndroid.keyDown(keyCode+400, event);
			else if(event.getAction() == KeyEvent.ACTION_UP)
				returnValue = OFAndroid.keyUp(keyCode+400, event);
			else if(event.getAction() == KeyEvent.FLAG_CANCELED)
				returnValue = OFAndroid.keyUp(keyCode+400, event);
			else
				returnValue = true;



		}  else if ((event.getSource() & InputDevice.SOURCE_TRACKBALL) == InputDevice.SOURCE_TRACKBALL) {
			returnValue = true;
		} else {

			if(OFAndroid.lastInputID == -1 && event.getDevice() != null ||
					event.getDevice() != null &&  OFAndroid.lastDeviceID != event.getDeviceId() &&
					OFAndroid.lastInputVendorID != event.getDevice().getVendorId() ) {
				OFAndroid.lastInputDevice = event.getDevice(); // not sure why this happens
				OFAndroid.lastInputID = event.getSource();
				OFAndroid.lastDeviceID = event.getDeviceId();
				OFAndroid.lastInputDescriptor = event.getDevice().getDescriptor();
				OFAndroid.lastControllerType = OFAndroidController.getControllerType(event.getDevice());
				OFAndroid.lastInputVendorID = OFAndroid.lastInputDevice.getVendorId();
				OFAndroid.lastInputProductID = OFAndroid.lastInputDevice.getProductId();
				if(OFAndroid.lastInputVendorID == 0 && OFAndroid.lastInputProductID == 0 ) { // Fix for Samsung PS5
					OFAndroid.lastInputProductID = OFAndroidController.ProductID_PS5;
					OFAndroid.lastInputVendorID = OFAndroidController.VendorPS;
				}
			}
			if(LOG_INPUT && OFAndroid.lastInputDevice != null) {
				OFAndroid.toast("OTHER:" + OFAndroid.lastInputDevice.getName() + " keycode:" + event.getKeyCode() + "vendor: " + OFAndroid.lastInputDevice.getVendorId() + "PID:" + OFAndroid.lastInputProductID);
				Log.i("OF", "Controller: name:" + OFAndroid.lastInputDevice.getName() + " vendor:" + OFAndroid.lastInputDevice.getVendorId() + "PID:" + OFAndroid.lastInputProductID);
			}
			if(event.getAction() == KeyEvent.ACTION_DOWN && event.getKeyCode() != KeyEvent.KEYCODE_UNKNOWN)
				returnValue = OFAndroid.keyDown(event.getKeyCode(), event);
			else if(event.getAction() == KeyEvent.ACTION_UP && event.getKeyCode() != KeyEvent.KEYCODE_UNKNOWN)
				returnValue = OFAndroid.keyUp(event.getKeyCode(), event);
			else if(event.getAction() == KeyEvent.FLAG_CANCELED && event.getKeyCode() != KeyEvent.KEYCODE_UNKNOWN)
				returnValue = OFAndroid.keyUp(event.getKeyCode(), event);
			else
				returnValue = super.dispatchKeyEvent(event);
		}
		return returnValue;
	}

	@Override
	public boolean dispatchKeyShortcutEvent(KeyEvent event){
		if(LOG_INPUT) Log.i("OF", "dispatchKeyShortcutEvent" + " event:" + event.toString());
		return true;
	}

	@Override
	public boolean dispatchPopulateAccessibilityEvent(AccessibilityEvent event) {
		if(LOG_INPUT) Log.i("OF", "dispatchPopulateAccessibilityEvent" + " event:" + event.toString());
		return true;
	}

	@Override
	public boolean dispatchTouchEvent(MotionEvent ev) {
		if(LOG_INPUT) Log.i("OF", "dispatchTouchEvent" + " event:" + ev.toString());
		if(OFAndroid.gestureListener != null) {
			OFAndroid.gestureListener.onTouch(ev);
		}
		return true;
	}

	@Override
	public boolean dispatchTrackballEvent(MotionEvent ev) {
		if(LOG_INPUT) Log.i("OF", "dispatchTrackballEvent" + " event:" + ev.toString());
		if(OFAndroid.gestureListener != null) {
			OFAndroid.gestureListener.onTouch(ev);
		}
		return true;
	}
	
	@Override
	public boolean onNavigateUp(){
		if(LOG_INPUT) Log.i("OF", "onNavigateUp");
		return true;
	}

	@Override
	public void onMultiWindowModeChanged(boolean isInMultiWindowMode, Configuration newConfig) {
		if(LOG_ENGINE) Log.i("OF", "onMultiWindowModeChanged:isInMultiWindowMode:" + isInMultiWindowMode);
		OFAndroid.setMultiWindowMode(isInMultiWindowMode);

		//if(hasPaused) return;

		if(LOG_ENGINE) Log.i("OF", "onMultiWindowModeChanged() " + getRequestedOrientation() + " newConfig dpi:" + newConfig.densityDpi + " screenLayout:" + newConfig.screenLayout + " screenWidthDp:" + newConfig.screenWidthDp + " screenHeightDp:" + newConfig.screenHeightDp + " isScreenWideColorGamut:" + false);

		DetermineDisplayConfiguration(true);
		DetermineDisplayDimensions();
	}

	@Override
	public boolean onKeyShortcut(int keyCode, KeyEvent event) {
		if(LOG_INPUT) Log.i("OF", "onKeyShortcut:isInMultiWindowMode:" + keyCode);
		return true;
	}

	@Override
	public boolean isLaunchedFromBubble () {
		if(LOG_ENGINE) Log.i("OF", "isLaunchedFromBubble");
		DetermineDisplayConfiguration(true);
		DetermineDisplayDimensions();
		return true;
	}

	@Override
	public void onAttachedToWindow() {
		if(LOG_ENGINE) Log.i("OF", "onAttachedToWindow");
		super.onAttachedToWindow();
	}


}
