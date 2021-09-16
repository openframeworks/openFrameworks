package cc.openframeworks;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.graphics.Point;
import android.hardware.display.DisplayManager;
import android.os.Build;
import android.os.Bundle;

import androidx.annotation.RequiresApi;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowCompat;

import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.InputDevice;
import android.view.InputEvent;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.FrameLayout;

import java.util.ArrayList;

import static android.opengl.EGL14.EGL_NO_CONTEXT;

import com.getkeepsafe.relinker.ReLinker;

public abstract class OFActivity extends Activity implements DisplayManager.DisplayListener{

	private static final String TAG = "OF";
	private DisplayManager displayManager;
	private Display display;
	private Display presentationDisplay;

	public float currentRefreshRate;
	public float highestRefreshRate;

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


	public void initView(){
		String packageName = this.getPackageName();
        try {
        	Log.v("OF","trying to find class: "+packageName+".R$layout");
			Class<?> layout = Class.forName(packageName+".R$layout");
			View view = this.getLayoutInflater().inflate(layout.getField("main_layout").getInt(null),null);
			if(view == null) {
				Log.w("OF", "Could not find main_layout.xml.");
				throw new Exception();
			}
			this.setContentView(view);
			
			Class<?> id = Class.forName(packageName+".R$id");
			mOFGlSurfaceContainer = (ViewGroup)this.findViewById(id.getField("of_gl_surface_container").getInt(null));
			
			if(mOFGlSurfaceContainer == null) {
				Log.w(TAG, "Could not find of_gl_surface_container in main_layout.xml. Copy main_layout.xml from latest empty example to fix this warning.");
				throw new Exception();
			}
			
		} catch (Exception e) {
			Log.w(TAG, "couldn't create view from layout falling back to GL only",e);
			mOFGlSurfaceContainer = new FrameLayout(this);
	        this.setContentView(mOFGlSurfaceContainer);
		}
	}

	public void resetView(){
		String packageName = this.getPackageName();
		try {
			Log.v(TAG,"trying to find class: "+packageName+".R$layout");
			Class<?> layout = Class.forName(packageName+".R$layout");
			View view = this.getLayoutInflater().inflate(layout.getField("main_layout").getInt(null),null);
			if(view == null) {
				Log.w(TAG, "Could not find main_layout.xml.");
				throw new Exception();
			}
			this.setContentView(view);

			Class<?> id = Class.forName(packageName+".R$id");
			mOFGlSurfaceContainer = (ViewGroup)this.findViewById(id.getField("of_gl_surface_container").getInt(null));

			if(mOFGlSurfaceContainer == null) {
				Log.w(TAG, "Could not find of_gl_surface_container in main_layout.xml. Copy main_layout.xml from latest empty example to fix this warning.");
				throw new Exception();
			}

		} catch (Exception e) {
			Log.w(TAG, "couldn't create view from layout falling back to GL only",e);
			mOFGlSurfaceContainer = new FrameLayout(this);
			this.setContentView(mOFGlSurfaceContainer);
		}
	}


	private boolean create_first = true;
	
	@Override
	protected void onCreate(Bundle arg0) {
		super.onCreate(arg0);
		try {
			OFAndroid.packageName = getPackageName();
		} catch (Exception ex) {
			Log.e(TAG, "Failure to getPackageName" + ex.getMessage());
		}

		WindowCompat.setDecorFitsSystemWindows(getWindow(), false);  // https://developer.android.com/training/gestures/edge-to-edge#lay-out-in-full-screen

		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
			displayManager = getSystemService(DisplayManager.class);
			displayManager.registerDisplayListener(this, null);

		}
		OFAndroidLifeCycle.setActivity(this);
		if(OFAndroidLifeCycle.coreLibraryLoaded == false) {
			LoadCoreStatic();
		} else {
			//Setup();
		}

		initView();
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
		Log.i(TAG, "OFAndroid.Setup:" + hasSetup + " | OFAndroidLifeCycle.coreLibraryLoaded:" + OFAndroidLifeCycle.coreLibraryLoaded + "| OFAndroidLifeCycle.appLibraryLoaded :" + OFAndroidLifeCycle.appLibraryLoaded);

		if(hasSetup == false &&
				OFAndroidLifeCycle.appLibraryLoaded == true &&
				OFAndroidLifeCycle.coreLibraryLoaded == true
		) {
			hasSetup = true;
			
			this.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					OFAndroidLifeCycle.init();
					OFAndroidLifeCycle.glCreate();
					DetermineDisplayConfiguration();
					DetermineDisplayDimensions();
				}
			});

		} else if(hasSetup == false && OFAndroidLifeCycle.coreLibraryLoaded == false && OFAndroidLifeCycle.appLibraryLoaded == true) {
			LoadCoreStatic();
		} else if(hasSetup == true) {
			Log.i(TAG, "OFAndroid.Setup:true | OFAndroidLifeCycle.coreLibraryLoaded:" + OFAndroidLifeCycle.coreLibraryLoaded + "| OFAndroidLifeCycle.appLibraryLoaded :" + OFAndroidLifeCycle.appLibraryLoaded);
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



					Point point = new Point();
					getWindowManager().getDefaultDisplay().getRealSize(point);

					double x = Math.pow(point.x/ displayMetrics.xdpi, 2);
					double y = Math.pow(point.y / displayMetrics.ydpi, 2);
					double screenInches = Math.sqrt(x + y);
					Log.d(TAG, "Screen inches : " + screenInches +" with realSize:" + point.x +" height:"  + point.y);


					//Log.i("OF", "DisplayMetrics: w/h:" +width + "x" + height + " barHeight:" + heightBar + "x barWidth:" + widthBar + " bar:" + bar + " widthBar:" + barWidth + " densityDPI:"  +pixeldpi);
					Log.i("OF", "DisplayRealMetrics: w/h:" + width_px + "x" + height_px + " pixeldpi:" + pixeldpi);
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

	public void DetermineDisplayConfiguration() {
		try {
			display = getWindowManager().getDefaultDisplay();
			if(display.isValid()) {
				if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {
					boolean isWideColorGamut = display.isWideColorGamut();
					Log.i("OF", "Display WideColor Gamut Supported:" +  isWideColorGamut);
					OFAndroid.wideGamut = isWideColorGamut;
					boolean isHDR = display.isHdr();
					Log.i("OF", "Display is HDR Supported:" +  isHDR);
					OFAndroid.hdrScreen = isHDR;
				}
				currentRefreshRate = display.getRefreshRate();
				Log.i("OF", "Display Current RefreshRate :" +  currentRefreshRate);

				if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.LOLLIPOP) {
					if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.M) {
						try {
							Display.Mode[] modes = display.getSupportedModes();
							Display.Mode currentMode = display.getMode();
							Log.i("OF", "Display Mode: CurrentMode:" + currentMode + " refreshRate: [" + currentMode.getRefreshRate() + "] mode PhysicalWidth:[" + currentMode.getPhysicalWidth() + "] mode PhysicalHeight:[" + currentMode.getPhysicalHeight() + "]");

							if (currentRefreshRate != currentMode.getRefreshRate()) {
								Log.e("OF", "Display Mode: Current Display Mode Refresh Rate not equal to current :" + currentMode + " refreshRate: [" + currentMode.getRefreshRate() + "] displayRefreshRate: " + currentRefreshRate);
							}
							currentRefreshRate = currentMode.getRefreshRate();
							for (Display.Mode mode : modes) {
								Log.i("OF", "Display Mode: Supported:" + mode + " refreshRate: [" + mode.getRefreshRate() + "] mode PhysicalWidth:[" + mode.getPhysicalWidth() + "] mode PhysicalHeight:[" + mode.getPhysicalHeight() + "]");
								if (mode.getRefreshRate() >= highestRefreshRate) {
									highestRefreshRate = mode.getRefreshRate();
								}
							}
						} catch (Exception ex) {
							Log.e("OF", "Display Mode Exception:", ex);
						}
					} else { // pre 23 Display Mode
						try {
							float[] refreshRates = display.getSupportedRefreshRates();
							for (float refreshRate : refreshRates) {
								Log.i("OF", "Display RefreshRate Supported:" + refreshRate);
								if (refreshRate >= highestRefreshRate) {
									highestRefreshRate = refreshRate;
								}
							}
						} catch (Exception ex) {
							Log.e("OF", "Display Mode Exception:", ex);
						}
					}

					OFGLSurfaceView glView = OFAndroidLifeCycle.getGLView();
					if(glView != null) {
						glView.setFrameRate(highestRefreshRate);
						currentRefreshRate = highestRefreshRate;
					}

//					if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {
//						boolean isWideColorGamut = display.isWideColorGamut();
//						Log.i("OF", "Display WideColor Gamut Supported:" +  isWideColorGamut);
//						OFAndroid.wideGamut = isWideColorGamut;
//
//						boolean isHDR = display.isHdr();
//						Log.i("OF", "Display is HDR Supported:" +  isHDR);
//						OFAndroid.hdrScreen = isHDR;
//					}
//					Display.Mode[] supportedModes = new Display.Mode[0];
//					if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.M) {
//						supportedModes = display.getSupportedModes();
//					}
//					for(Display.Mode mode : supportedModes){
//						Log.i("OF", "Could not get Window fo:" +  mode);
//					}
				}
			} else {
				Log.w("OF", "Display is not valid yet");
			}

		} catch (Exception ex) {
			Log.w("OF", "Could not get Window for Display ", ex);
		}
//		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O_MR1) {
//			if(getWindow() != null && getWindow().isWideColorGamut()) {
//				OFAndroid.wideGamut = true;
//			}
//			OFAndroid.hdrScreen = false;
//		}


		if(hasSetup) {
			OFAndroid.deviceRefreshRate((int) currentRefreshRate);
			OFAndroid.deviceHighestRefreshRate((int) highestRefreshRate);
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
		checkForPresentationDisplays();
		if(hasPaused) return;
		try {
			display = getWindowManager().getDefaultDisplay();
			if (display.isValid()) {
				DetermineDisplayConfiguration();
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
					Log.i("OF", "checkForPresentationDisplays found external display: isValid:" + presentationDisplay.isValid() + " name:" + presentationDisplay.getName() + " refreshRate:" + presentationDisplay.getRefreshRate() + " Resolution:" + realWidth + "x" + realHeight);
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

		if(hasPaused) return;

		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
			Log.i("OF", "onConfigurationChanged() " + getRequestedOrientation() + " newConfig dpi:" + newConfig.densityDpi + " screenLayout:" + newConfig.screenLayout + " screenWidthDp:" + newConfig.screenWidthDp + " screenHeightDp:" + newConfig.screenHeightDp + " isScreenWideColorGamut:" + newConfig.isScreenWideColorGamut());
		} else {
			Log.i("OF", "onConfigurationChanged() " + getRequestedOrientation() + " newConfig dpi:" + newConfig.densityDpi + " screenLayout:" + newConfig.screenLayout + " screenWidthDp:" + newConfig.screenWidthDp + " screenHeightDp:" + newConfig.screenHeightDp + " isScreenWideColorGamut:" + false);
		}

		if (newConfig.orientation== Configuration.ORIENTATION_PORTRAIT)
		{

		}
		else  if (newConfig.orientation == Configuration.ORIENTATION_LANDSCAPE)
		{

		}

		DetermineDisplayConfiguration();
		DetermineDisplayDimensions();

	}

	public boolean showNavigationBar(Resources resources) {
		int id = resources.getIdentifier("config_showNavigationBar", "bool", "android");
		return id > 0 && resources.getBoolean(id);
	}

	private int getNavigationBarHeight() {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {
			DisplayMetrics metrics = new DisplayMetrics();
			getWindowManager().getDefaultDisplay().getMetrics(metrics);
			int usableHeight = metrics.heightPixels;
			getWindowManager().getDefaultDisplay().getRealMetrics(metrics);
			int realHeight = metrics.heightPixels;
			if (realHeight > usableHeight)
				return realHeight - usableHeight;
			else
				return 0;
		}
		return 0;
	}

	private int getNavigationBarWidth() {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {
			DisplayMetrics metrics = new DisplayMetrics();
			getWindowManager().getDefaultDisplay().getMetrics(metrics);
			int usableWidth = metrics.widthPixels;
			getWindowManager().getDefaultDisplay().getRealMetrics(metrics);
			int realWidth= metrics.widthPixels;
			if (realWidth > usableWidth)
				return realWidth - usableWidth;
			else
				return 0;
		}
		return 0;
	}
	
	@Override
	protected void onStart() {
		Log.i("OF", "onStart");
		super.onStart();
		//OFAndroidLifeCycle.glStart();

		try {
			display = getWindowManager().getDefaultDisplay();
			if (display.isValid()) {
				DetermineDisplayConfiguration();
			}

		} catch(Exception exception) {
			Log.w("OF", "onStart Display Could not get Window for Display ", exception);
		}

	}
	
	@Override
	protected void onStop() {
		Log.i("OF", "onStop");
		hasPaused = true;
		Log.w("OF", "not calling OFAndroidLifeCycle.glStop()");
//		OFAndroidLifeCycle.glStop();
//		OFAndroidLifeCycle.reset();
//		mOFGlSurfaceContainer = null;

		super.onStop();

	}
	
	@Override
	protected void onRestart() {
		OFAndroidLifeCycle.setActivity(this);
		Log.i("OF", "onRestart");
		super.onRestart();
		OFAndroidLifeCycle.glRestart();
	}

	public void onForceRestart() {
		Log.i("OF", "onForceRestart");
		OFAndroid.setupGL(OFAndroid.eglVersion, true);
		DetermineDisplayConfiguration();
		DetermineDisplayDimensions();
		if(mOFGlSurfaceContainer == null) {
			Log.e("OF", "onResume mOFGlSurfaceContainer is null - glCreateSurface");
		}
		OFAndroidLifeCycle.glResume(mOFGlSurfaceContainer);
	}

	
	@Override
	protected void onResume() {
		Log.i("OF", "onResume");
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



		if(android.opengl.EGL14.eglGetCurrentContext() == EGL_NO_CONTEXT){
			Log.e("OF", "onResume eglGetCurrentContext = EGL_NO_CONTEXT BAD");
			//OFAndroidWindow.exit();
			//OFAndroidWindow.surfaceHasBeenDestroyed();
			OFAndroid.setupGL(OFAndroid.eglVersion, true);
		}
		if(OFAndroidLifeCycle.isInit() && mOFGlSurfaceContainer == null) {
			Log.i("OF", "onResume mOFGlSurfaceContainer is null");
		}
		//
		if(OFAndroidLifeCycle.isInit() && OFAndroidLifeCycle.getGLView() == null) {
			Log.i("OF", "onResume getGLView is null - glCreateSurface");
			//OFAndroidWindow.exit();
			OFAndroid.setupGL(OFAndroid.eglVersion, true);
		}

		if(OFAndroidLifeCycle.isInit() && OFAndroidLifeCycle.getGLView() != null && OFAndroidLifeCycle.getGLView().getDisplay() == null) {
			Log.i("OF", "onResume getGLView is null - glCreateSurface");
//			OFAndroidWindow.exit();
			OFAndroid.setupGL(OFAndroid.eglVersion, true);
		}

		DetermineDisplayConfiguration();
		DetermineDisplayDimensions();
		if(mOFGlSurfaceContainer == null) {
			Log.e("OF", "onResume mOFGlSurfaceContainer is null - glCreateSurface");
		}
		OFAndroidLifeCycle.glResume(mOFGlSurfaceContainer);

		hasPaused = false;

	}
	@Override
	protected void onPause() {
		Log.i("OF", "onPause");
		hasPaused = true;
		OFAndroidLifeCycle.glPause();
		super.onPause();
	}
	@Override
	protected void onDestroy() {
		Log.e("OF", "onDestroy");
		hasPaused = true;
		OFAndroidLifeCycle.glDestroy();
		if (displayManager!=null) {
			displayManager.unregisterDisplayListener(this);
		}

		super.onDestroy();
	}
	
	@Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
		Log.i("OF", "onKeyDown" + keyCode + " event:" + event.toString());
		if(!hasSetup) return false;
		if ((event.getSource() & InputDevice.SOURCE_GAMEPAD)
				== InputDevice.SOURCE_GAMEPAD) {
			int deviceId = event.getDeviceId();
			OFAndroid.keyDown(keyCode+400, event);
			return true;
		}
		else if (OFAndroid.keyDown(keyCode, event)) {
		    return true;
		} else {
		    return super.onKeyDown(keyCode, event);
		}
    }
    
    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
		if(!hasSetup) return false;
		Log.i("OF", "onKeyUp" + keyCode + " event:" + event.toString());

		if ((event.getSource() & InputDevice.SOURCE_GAMEPAD)
				== InputDevice.SOURCE_GAMEPAD) {
			int deviceId = event.getDeviceId();
			return OFAndroid.keyUp(keyCode+400, event);
		}
		else if (OFAndroid.keyUp(keyCode, event)) {
		    return true;
		} else {
		    return super.onKeyUp(keyCode, event);
		}
    }

//	@Override
//	public boolean onGenericMotionEvent(MotionEvent event) {
//		if(!hasSetup) return true;
//		return genericMotionEvent(event);
//	}

	final static int DPAD_UP       = 0;
	final static int DPAD_LEFT     = 1;
	final static int DPAD_RIGHT    = 2;
	final static int DPAD_DOWN     = 3;
	final static int DPAD_CENTER   = 4;
	int directionPressed = -1;
	int keyCode = -1;
	int lastKeyCode = -1;
	int deviceCode = -1;

	boolean DPAD_UP_DOWN  = false;
	boolean DPAD_LEFT_DOWN     = false;
	boolean DPAD_RIGHT_DOWN    = false;
	boolean DPAD_DOWN_DOWN     = false;
	boolean DPAD_CENTER_DOWN   = false;


	public boolean genericMotionEvent (MotionEvent event){
		Log.i("OF", "dispatchGenericMotionEvent" + " event:" + event.toString() + " deviceId:" + event.getDeviceId());

		if(deviceCode == -1)
			deviceCode = event.getDeviceId();
		
		//if(event.getDeviceId() != deviceCode) return true;
		try {
			if ((event.getSource() & InputDevice.SOURCE_JOYSTICK) ==
					InputDevice.SOURCE_JOYSTICK &&
					event.getAction() == MotionEvent.ACTION_MOVE) {
				if (event.getAction() == MotionEvent.ACTION_MOVE) {
					// process the joystick movement...
					final int historySize = event.getHistorySize();

					// Process the movements starting from the
					// earliest historical position in the batch
					for (int i = 0; i < historySize; i++) {
						// Process the event at historical position i
						processJoystickInput(event, i);
					}
					//processJoystickInput(event, -1);
					return true;
				}

			}
			if ((event.getSource() & InputDevice.SOURCE_DPAD) == InputDevice.SOURCE_DPAD ||
					(event.getSource() & InputDevice.SOURCE_GAMEPAD) == InputDevice.SOURCE_GAMEPAD ||
					(event.getSource() & InputDevice.SOURCE_JOYSTICK) == InputDevice.SOURCE_JOYSTICK ||
					(event.getSource() & InputDevice.SOURCE_CLASS_BUTTON) == InputDevice.SOURCE_CLASS_BUTTON ||
					(event.getSource() & InputDevice.SOURCE_DPAD) > InputDevice.SOURCE_DPAD - 13 && (event.getSource() & InputDevice.SOURCE_DPAD) < InputDevice.SOURCE_GAMEPAD + 500
			) {
				// If the input event is a MotionEvent, check its hat axis values.
				directionPressed = -1;

				if ((InputEvent) event instanceof KeyEvent) {    // If the input event is a KeyEvent, check its key code.

					// Use the key code to find the D-pad direction.
					KeyEvent keyEvent = (KeyEvent) (InputEvent) event;
					boolean isKeyDown = event.getActionMasked() == MotionEvent.ACTION_MOVE || event.getActionMasked() == MotionEvent.ACTION_BUTTON_PRESS;
					if (keyEvent.getKeyCode() == KeyEvent.KEYCODE_DPAD_LEFT) {
						directionPressed = DPAD_LEFT;
						keyCode = KeyEvent.KEYCODE_DPAD_LEFT;
						DPAD_LEFT_DOWN = isKeyDown;
					} else if (keyEvent.getKeyCode() == KeyEvent.KEYCODE_DPAD_RIGHT) {
						directionPressed = DPAD_RIGHT;
						keyCode = KeyEvent.KEYCODE_DPAD_RIGHT;
						DPAD_RIGHT_DOWN = isKeyDown;
					} else if (keyEvent.getKeyCode() == KeyEvent.KEYCODE_DPAD_UP) {
						directionPressed = DPAD_UP;
						keyCode = KeyEvent.KEYCODE_DPAD_UP;
						DPAD_UP_DOWN = isKeyDown;
					} else if (keyEvent.getKeyCode() == KeyEvent.KEYCODE_DPAD_DOWN) {
						directionPressed = DPAD_DOWN;
						keyCode = KeyEvent.KEYCODE_DPAD_DOWN;
						DPAD_DOWN_DOWN = isKeyDown;
					} else if (keyEvent.getKeyCode() == KeyEvent.KEYCODE_DPAD_CENTER) {
						directionPressed = DPAD_CENTER;
						keyCode = KeyEvent.KEYCODE_DPAD_CENTER;
						DPAD_CENTER_DOWN = isKeyDown;
					}
					if (event.getActionMasked() == MotionEvent.ACTION_MOVE || event.getActionMasked() == MotionEvent.ACTION_BUTTON_PRESS) {
						return OFAndroid.keyDown(keyCode + 400, keyCode);
					}
					else if (event.getActionMasked() == MotionEvent.ACTION_BUTTON_RELEASE || event.getActionMasked() == MotionEvent.ACTION_CANCEL) {
						return OFAndroid.keyUp(keyCode + 400, keyCode);
					}
					else {
						return true;
					}
				} else if (event instanceof MotionEvent) {

					// Use the hat axis value to find the D-pad direction
					MotionEvent motionEvent = (MotionEvent) event;
					Log.i("OF", "onGenericMotionEvent motionEvent:" + motionEvent);
					float xaxis = motionEvent.getAxisValue(MotionEvent.AXIS_HAT_X);
					float yaxis = motionEvent.getAxisValue(MotionEvent.AXIS_HAT_Y);
					boolean isKeyDown = true;
					// Check if the AXIS_HAT_X value is -1 or 1, and set the D-pad
					// LEFT and RIGHT direction accordingly.
					if (Float.compare(xaxis, -1.0f) == 0) {
						directionPressed = DPAD_LEFT;
						keyCode = KeyEvent.KEYCODE_DPAD_LEFT;
					} else if (Float.compare(xaxis, 1.0f) == 0) {
						directionPressed = DPAD_RIGHT;
						keyCode = KeyEvent.KEYCODE_DPAD_RIGHT;
					}
					// Check if the AXIS_HAT_Y value is -1 or 1, and set the D-pad
					// UP and DOWN direction accordingly.
					else if (Float.compare(yaxis, -1.0f) == 0) {
						directionPressed = DPAD_UP;
						keyCode = KeyEvent.KEYCODE_DPAD_UP;
					} else if (Float.compare(yaxis, 1.0f) == 0) {
						directionPressed = DPAD_DOWN;
						keyCode = KeyEvent.KEYCODE_DPAD_DOWN;
					} else if(Float.compare(xaxis, 0.0f) == 0 &&  Float.compare(yaxis, 0.0f) == 0) {
						boolean value = true;
						if(keyCode != -10)
							value = OFAndroid.keyUp(keyCode + 400, keyCode);
						keyCode = -1; lastKeyCode = -1;
						return value;
					}
					if (event.getActionMasked() == MotionEvent.ACTION_MOVE || event.getActionMasked() == MotionEvent.ACTION_BUTTON_PRESS) {
						return OFAndroid.keyDown(keyCode + 400, keyCode);
					}
					else if (event.getActionMasked() == MotionEvent.ACTION_BUTTON_RELEASE || event.getActionMasked() == MotionEvent.ACTION_CANCEL) {
						boolean value = OFAndroid.keyUp(keyCode + 400, keyCode);
						keyCode = -1;
						return value;
					}
					else
						return true;

				}
			} else if (event.isFromSource(InputDevice.SOURCE_CLASS_POINTER)) { // PS4 Touch Pad
				switch (event.getAction()) {
					case MotionEvent.ACTION_HOVER_MOVE:
						// process the mouse hover movement...
						return true;
					case MotionEvent.ACTION_SCROLL:
						// process the scroll wheel movement...
						return true;
				}
			}
		} catch(Exception exeception) {
			Log.i("OF", "onGenericMotionEvent Exception:" + exeception.getMessage());
		}
		return super.onGenericMotionEvent(event);
	}

	private static float getCenteredAxis(MotionEvent event,
										 InputDevice device, int axis, int historyPos) {
		final InputDevice.MotionRange range =
				device.getMotionRange(axis, event.getSource());

		// A joystick at rest does not always report an absolute position of
		// (0,0). Use the getFlat() method to determine the range of values
		// bounding the joystick axis center.
		if (range != null) {
			final float flat = range.getFlat();
			final float value =
					historyPos < 0 ? event.getAxisValue(axis):
							event.getHistoricalAxisValue(axis, historyPos);

			// Ignore axis values that are within the 'flat' region of the
			// joystick axis center.
			if (Math.abs(value) > flat) {
				return value;
			}
		}
		return 0;
	}

	private void processJoystickInput(MotionEvent event,
									  int historyPos) {

		InputDevice inputDevice = event.getDevice();

		// Calculate the horizontal distance to move by
		// using the input value from one of these physical controls:
		// the left control stick, hat axis, or the right control stick.
		float x = getCenteredAxis(event, inputDevice,
				MotionEvent.AXIS_X, historyPos);
		if (x == 0) {
			x = getCenteredAxis(event, inputDevice,
					MotionEvent.AXIS_HAT_X, historyPos);
		}
		if (x == 0) {
			x = getCenteredAxis(event, inputDevice,
					MotionEvent.AXIS_Z, historyPos);
		}

		// Calculate the vertical distance to move by
		// using the input value from one of these physical controls:
		// the left control stick, hat switch, or the right control stick.
		float y = getCenteredAxis(event, inputDevice,
				MotionEvent.AXIS_Y, historyPos);
		if (y == 0) {
			y = getCenteredAxis(event, inputDevice,
					MotionEvent.AXIS_HAT_Y, historyPos);
		}
		if (y == 0) {
			y = getCenteredAxis(event, inputDevice,
					MotionEvent.AXIS_RZ, historyPos);
		}
		Log.i("OF", "processJoystickInput: " + " x:" + x + " y::" + y);
	}

//	@Override
//	public boolean dispatchKeyEvent (KeyEvent event){
//		if(!hasSetup) return false;
//		int deviceId = event.getDeviceId();
//		//Log.i("OF", "dispatchKeyEvent" + " event:" + event.toString() + " deviceID:" + deviceId);
//		boolean returnValue = false;
//
//		if ((event.getSource() & InputDevice.SOURCE_DPAD) == InputDevice.SOURCE_DPAD ||
//				(event.getSource() & InputDevice.SOURCE_GAMEPAD) == InputDevice.SOURCE_GAMEPAD ||
//				(event.getSource() & InputDevice.SOURCE_CLASS_JOYSTICK) == InputDevice.SOURCE_CLASS_JOYSTICK ||
//				(event.getSource() & InputDevice.SOURCE_CLASS_BUTTON) == InputDevice.SOURCE_CLASS_BUTTON ||
//				(event.getSource() & InputDevice.SOURCE_DPAD) > InputDevice.SOURCE_DPAD - 13 && (event.getSource() & InputDevice.SOURCE_DPAD)  < InputDevice.SOURCE_GAMEPAD  + 500
//		) {
//			if(event.getAction() == KeyEvent.ACTION_DOWN)
//				returnValue = OFAndroid.keyDown(event.getKeyCode()+400, event);
//			else if(event.getAction() == KeyEvent.ACTION_UP)
//				returnValue = OFAndroid.keyUp(event.getKeyCode()+400, event);
//			else
//				returnValue = true;
//		}  else if ((event.getSource() & InputDevice.SOURCE_TRACKBALL) == InputDevice.SOURCE_TRACKBALL) {
//			returnValue = true;
//		} else {
//			if(event.getAction() == KeyEvent.ACTION_DOWN)
//				returnValue = OFAndroid.keyDown(event.getKeyCode(), event);
//			else if(event.getAction() == KeyEvent.ACTION_UP)
//				returnValue = OFAndroid.keyUp(event.getKeyCode(), event);
//			else
//				returnValue = super.dispatchKeyEvent(event);
//		}
//		Log.i("OF", "dispatchKeyEvent:return:" + returnValue + " event:" + event.toString() + " deviceID:" + deviceId);
//
//		return false;
//	}

	@RequiresApi(28)
	private static class OnUnhandledKeyEventListenerWrapper implements View.OnUnhandledKeyEventListener {
		private ViewCompat.OnUnhandledKeyEventListenerCompat mCompatListener;

		OnUnhandledKeyEventListenerWrapper(ViewCompat.OnUnhandledKeyEventListenerCompat listener) {
			this.mCompatListener = listener;
		}

		public boolean onUnhandledKeyEvent(View v, KeyEvent event) {
			Log.i("OF", "OFActivity:onUnhandledKeyEvent" + " event:" + event.toString());
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
}
