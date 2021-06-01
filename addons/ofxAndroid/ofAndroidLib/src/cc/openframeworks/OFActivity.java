package cc.openframeworks;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.hardware.display.DisplayManager;
import android.os.Build;
import android.os.Bundle;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import java.util.ArrayList;

public abstract class OFActivity extends Activity implements DisplayManager.DisplayListener{

	private DisplayManager displayManager;
	private Display display;
	private Display presentationDisplay;

	public float currentRefreshRate;
	public float highestRefreshRate;

	public void onGLSurfaceCreated(){}
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
				Log.w("OF", "Could not find of_gl_surface_container in main_layout.xml. Copy main_layout.xml from latest empty example to fix this warning.");
				throw new Exception();
			}
			
		} catch (Exception e) {
			Log.w("OF", "couldn't create view from layout falling back to GL only",e);
			mOFGlSurfaceContainer = new FrameLayout(this);
	        this.setContentView(mOFGlSurfaceContainer);
		}
	}

	public void resetView(){
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
				Log.w("OF", "Could not find of_gl_surface_container in main_layout.xml. Copy main_layout.xml from latest empty example to fix this warning.");
				throw new Exception();
			}

		} catch (Exception e) {
			Log.w("OF", "couldn't create view from layout falling back to GL only",e);
			mOFGlSurfaceContainer = new FrameLayout(this);
			this.setContentView(mOFGlSurfaceContainer);
		}
	}


	private boolean create_first = true;
	
	@Override
	protected void onCreate(Bundle arg0) {
		super.onCreate(arg0);

		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
			displayManager = getSystemService(DisplayManager.class);
			displayManager.registerDisplayListener(this, null);

		}
		try {
			display = getWindowManager().getDefaultDisplay();
			if(display.isValid()) {
				currentRefreshRate = display.getRefreshRate();
				Log.i("OF", "Display Current RefreshRate :" +  currentRefreshRate);

				if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.LOLLIPOP) {
					float[] refreshRates = display.getSupportedRefreshRates();
					highestRefreshRate = currentRefreshRate;
					for(float refreshRate : refreshRates){
						Log.i("OF", "Display RefreshRate Supported:" +  refreshRate);
						if(refreshRate > highestRefreshRate) {
							highestRefreshRate = refreshRate;
						}
					}


					if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {
						boolean isWideColorGamut = display.isWideColorGamut();
						Log.i("OF", "Display WideColor Gamut Supported:" +  isWideColorGamut);
						OFAndroid.wideGamut = isWideColorGamut;

						boolean isHDR = display.isHdr();
						Log.i("OF", "Display is HDR Supported:" +  isHDR);
						OFAndroid.hdrScreen = isHDR;
					}
					Display.Mode[] supportedModes = new Display.Mode[0];
					if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.M) {
						supportedModes = display.getSupportedModes();
					}
					for(Display.Mode mode : supportedModes){
						Log.i("OF", "Could not get Window fo:" +  mode);
					}


				}
			} else {
				Log.w("OF", "Display is not valid yet");
			}
		} catch (Exception exception) {
			Log.w("OF", "Could not get Window for Display ", exception);
		}


		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O_MR1) {
			if(getWindow() != null && getWindow().isWideColorGamut()) {
				OFAndroid.wideGamut = true;
			}
//			if(getWindow() != null && ) {
//				OFAndroid. = true;
//			}
			OFAndroid.hdrScreen = false;
		}



		OFAndroidLifeCycle.setActivity(this);
		OFAndroidLifeCycle.init();
		OFAndroidLifeCycle.glCreate();

		OFAndroid.deviceRefreshRate((int)currentRefreshRate);
		OFAndroid.deviceHighestRefreshRate((int)highestRefreshRate);

		//create gesture listener
		//register the two events
		initView();
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
		try {
			display = getWindowManager().getDefaultDisplay();
			if (display.isValid()) {

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
					float[] refreshRates = display.getSupportedRefreshRates();
					highestRefreshRate = currentRefreshRate;
					for (float refreshRate : refreshRates) {
						Log.i("OF", "Display RefreshRate Supported:" + refreshRate);
						if (refreshRate > highestRefreshRate) {
							highestRefreshRate = refreshRate;
						}
					}
				}

				OFGLSurfaceView glView = OFAndroidLifeCycle.getGLView();
				if(glView != null) {

					DisplayMetrics displayMetrics = new DisplayMetrics();
					getWindowManager().getDefaultDisplay().getRealMetrics(displayMetrics);
					int height = displayMetrics.heightPixels;
					int width = displayMetrics.widthPixels;
					int bar = getNavigationBarHeight();
					if(bar > 256) bar = 0;
					int barWidth = getNavigationBarHeight();
					if(barWidth > 256) barWidth = 0; // fixes bug in android display metrics
					int heightBar = displayMetrics.heightPixels + bar;
					int widthBar = displayMetrics.widthPixels + barWidth;
					int width_px = Resources.getSystem().getDisplayMetrics().widthPixels;
					int height_px = Resources.getSystem().getDisplayMetrics().heightPixels;
					int pixeldpi = Resources.getSystem().getDisplayMetrics().densityDpi;
					Log.i("OF", "DisplayMetrics: w/h:" +width + "x" + height + " barHeight:" + heightBar + "x barWidth:" + widthBar + " bar:" + bar + " widthBar:" + barWidth + " densityDPI:"  +pixeldpi);
					Log.i("OF", "DisplayRealMetrics: w/h:" +width_px + "x" + height_px + " pixeldpi:" + pixeldpi);


					glView.setWindowResize(widthBar, heightBar);
				}
				OFAndroid.deviceRefreshRate((int)currentRefreshRate);
				OFAndroid.deviceHighestRefreshRate((int)highestRefreshRate);
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

		OFGLSurfaceView glView = OFAndroidLifeCycle.getGLView();
		if(glView != null) {

			DisplayMetrics displayMetrics = new DisplayMetrics();
			getWindowManager().getDefaultDisplay().getRealMetrics(displayMetrics);
			int height = displayMetrics.heightPixels;
			int width = displayMetrics.widthPixels;
			int bar = getNavigationBarHeight();
			int barWidth = getNavigationBarHeight();
			int heightBar = displayMetrics.heightPixels + bar;
			int widthBar = displayMetrics.widthPixels + barWidth;

			int width_px = Resources.getSystem().getDisplayMetrics().widthPixels;
			int height_px = Resources.getSystem().getDisplayMetrics().heightPixels;
			int pixeldpi = Resources.getSystem().getDisplayMetrics().densityDpi;

			Log.i("OF", "DisplayMetrics: w/h:" +width + "x" + height + " barHeight:" + heightBar + "x barWidth:" + widthBar + " bar:" + bar + " widthBar:" + barWidth + " densityDPI:"  +pixeldpi);
			Log.i("OF", "DisplayRealMetrics: w/h:" +width_px + "x" + height_px + " pixeldpi:" + pixeldpi);

			glView.setWindowResize(widthBar, heightBar);
		}

		//super.initView();
	}

	public boolean showNavigationBar(Resources resources) {
		int id = resources.getIdentifier("config_showNavigationBar", "bool", "android");
		return id > 0 && resources.getBoolean(id);
	}

	private int getNavigationBarHeight() {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {
			DisplayMetrics metrics = new DisplayMetrics();
			getWindowManager().getDefaultDisplay().getRealMetrics(metrics);
			int usableHeight = metrics.heightPixels;
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
		OFAndroidLifeCycle.glStart();

		OFAndroid.deviceRefreshRate((int)currentRefreshRate);
		OFAndroid.deviceHighestRefreshRate((int)highestRefreshRate);
	}
	
	@Override
	protected void onStop() {
		Log.i("OF", "onStop");
		super.onStop();
		OFAndroidLifeCycle.glStop();
	}
	
	@Override
	protected void onRestart() {
		Log.i("OF", "onRestart");
		super.onRestart();
		OFAndroidLifeCycle.glRestart();
	}
	
	@Override
	protected void onResume() {
		Log.i("OF", "onResume");
		super.onResume();
		OFAndroidLifeCycle.setActivity(this);
		if(OFAndroidLifeCycle.isInit() && mOFGlSurfaceContainer == null) {
			Log.i("OF", "onResume mOFGlSurfaceContainer is null");
		}
		//resetView();
		if(OFAndroidLifeCycle.isInit() && OFAndroidLifeCycle.getGLView() == null) {
			Log.i("OF", "onResume getGLView is null - glCreateSurface");
			OFAndroid.setupGL(OFAndroid.eglVersion, true);
		}

		OFGLSurfaceView glView = OFAndroidLifeCycle.getGLView();
		if(glView != null) {

			DisplayMetrics displayMetrics = new DisplayMetrics();
			getWindowManager().getDefaultDisplay().getRealMetrics(displayMetrics);
			int height = displayMetrics.heightPixels;
			int width = displayMetrics.widthPixels;
			int bar = getNavigationBarHeight();
			int barWidth = getNavigationBarHeight();
			int heightBar = displayMetrics.heightPixels + bar;
			int widthBar = displayMetrics.widthPixels + barWidth;

			int width_px = Resources.getSystem().getDisplayMetrics().widthPixels;
			int height_px = Resources.getSystem().getDisplayMetrics().heightPixels;
			int pixeldpi = Resources.getSystem().getDisplayMetrics().densityDpi;

			Log.i("SuperHexagon", "DisplayMetrics: w/h:" +width + "x" + height + " barHeight:" + heightBar + "x barWidth:" + widthBar + " bar:" + bar + " widthBar:" + barWidth + " densityDPI:"  +pixeldpi);


			glView.setWindowResize(widthBar, heightBar);
		} else {
			Log.e("OF", "onResume OFGLSurfaceView is null");
		}
		OFAndroidLifeCycle.glResume(mOFGlSurfaceContainer);

	}
	@Override
	protected void onPause() {
		Log.i("OF", "onPause");
		super.onPause();
		OFAndroidLifeCycle.glPause();
	}
	@Override
	protected void onDestroy() {
		Log.e("OF", "onDestroy");
		OFAndroidLifeCycle.glDestroy();
		if (displayManager!=null) {
			displayManager.unregisterDisplayListener(this);
		}

		super.onDestroy();
	}
	
	@Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
		Log.i("OF", "onKeyDown" + keyCode + " event:" + event.toString());
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
		Log.i("OF", "onKeyUp" + keyCode + " event:" + event.toString());

		if ((event.getSource() & InputDevice.SOURCE_GAMEPAD)
				== InputDevice.SOURCE_GAMEPAD) {
			int deviceId = event.getDeviceId();
			OFAndroid.keyUp(keyCode+400, event);
			return true;
		}
		else if (OFAndroid.keyUp(keyCode, event)) {
		    return true;
		} else {
		    return super.onKeyUp(keyCode, event);
		}
    }

	@Override
	public boolean dispatchGenericMotionEvent (MotionEvent event){
		Log.i("OF", "dispatchGenericMotionEvent" + " event:" + event.toString());
		if (event.isFromSource(InputDevice.SOURCE_CLASS_JOYSTICK)) {
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
		if (event.isFromSource(InputDevice.SOURCE_CLASS_POINTER)) {
			switch (event.getAction()) {
				case MotionEvent.ACTION_HOVER_MOVE:
					// process the mouse hover movement...
					return true;
				case MotionEvent.ACTION_SCROLL:
					// process the scroll wheel movement...
					return true;
			}
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

	@Override
	public boolean dispatchKeyEvent (KeyEvent event){
		int deviceId = event.getDeviceId();
		Log.i("OF", "dispatchKeyEvent" + " event:" + event.toString() + " deviceID:" + deviceId);

		if ((event.getSource() & InputDevice.SOURCE_DPAD) == InputDevice.SOURCE_DPAD ||
				(event.getSource() & InputDevice.SOURCE_GAMEPAD) == InputDevice.SOURCE_GAMEPAD ||
				(event.getSource() & InputDevice.SOURCE_CLASS_JOYSTICK) == InputDevice.SOURCE_CLASS_JOYSTICK ||
				(event.getSource() & InputDevice.SOURCE_CLASS_BUTTON) == InputDevice.SOURCE_CLASS_BUTTON ||
				(event.getSource() & InputDevice.SOURCE_DPAD) > InputDevice.SOURCE_DPAD - 13 && (event.getSource() & InputDevice.SOURCE_DPAD)  < InputDevice.SOURCE_GAMEPAD  + 500
		) {
			if(event.getAction() == KeyEvent.ACTION_DOWN)
				return OFAndroid.keyDown(event.getKeyCode()+400, event);
			if(event.getAction() == KeyEvent.ACTION_UP)
				return OFAndroid.keyUp(event.getKeyCode()+400, event);
			else
				return true;
		}  else if ((event.getSource() & InputDevice.SOURCE_TRACKBALL) == InputDevice.SOURCE_TRACKBALL) {
			return true;
		} else {
			if(event.getAction() == KeyEvent.ACTION_DOWN)
				return OFAndroid.keyDown(event.getKeyCode(), event);
			if(event.getAction() == KeyEvent.ACTION_UP)
				return OFAndroid.keyUp(event.getKeyCode(), event);
			else
				return super.dispatchKeyEvent(event);
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
