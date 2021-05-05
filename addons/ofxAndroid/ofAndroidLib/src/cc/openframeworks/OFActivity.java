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
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;

public abstract class OFActivity extends Activity implements DisplayManager.DisplayListener{

	private DisplayManager displayManager;
	private Display display;
	private Display presentationDisplay;

	private float currentRefreshRate;
	private float highestRefreshRate;

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
						Log.i("OF", "Display Mode Supported:" +  mode);
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
		OFAndroid.deviceHighestRefreshRate((int)highestRefreshRate);
		OFAndroid.deviceRefreshRateChanged((int)currentRefreshRate);
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

				OFAndroid.deviceHighestRefreshRate((int)highestRefreshRate);
				OFAndroid.deviceRefreshRateChanged((int)currentRefreshRate);
			}
		} catch(Exception exception) {
			Log.w("OF", "Could not get Window for Display ", exception);
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

			Log.i("SuperHexagon", "DisplayMetrics: w/h:" +width + "x" + height + " barHeight:" + heightBar + "x barWidth:" + widthBar + " bar:" + bar + " widthBar:" + barWidth + " densityDPI:"  +pixeldpi);


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
		super.onStart();
		OFAndroidLifeCycle.glStart();
	}
	
	@Override
	protected void onStop() {
		super.onStop();
		OFAndroidLifeCycle.glStop();
	}
	
	@Override
	protected void onRestart() {
		super.onRestart();
		OFAndroidLifeCycle.glRestart();
	}
	
	@Override
	protected void onResume() {
		super.onResume();
		OFAndroidLifeCycle.setActivity(this);
		if(OFAndroidLifeCycle.isInit() && mOFGlSurfaceContainer == null) {
			Log.e("OF", "onResume mOFGlSurfaceContainer is null");
		}
		//resetView();
		if(OFAndroidLifeCycle.isInit() && OFAndroidLifeCycle.getGLView() == null) {
			Log.e("OF", "onResume getGLView is null - glCreateSurface");
			OFAndroid.setupGL(OFAndroid.eglVersion, true);

		}
		OFAndroidLifeCycle.glResume(mOFGlSurfaceContainer);
	}
	@Override
	protected void onPause() {
		super.onPause();
		OFAndroidLifeCycle.glPause();
	}
	@Override
	protected void onDestroy() {
		OFAndroidLifeCycle.glDestroy();
		if (displayManager!=null) {
			displayManager.unregisterDisplayListener(this);
		}

		super.onDestroy();
	}
	
	@Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
		if (OFAndroid.keyDown(keyCode, event)) {
		    return true;
		} else {
		    return super.onKeyDown(keyCode, event);
		}
    }
    
    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
		if (OFAndroid.keyUp(keyCode, event)) {
		    return true;
		} else {
		    return super.onKeyUp(keyCode, event);
		}
    }
}
