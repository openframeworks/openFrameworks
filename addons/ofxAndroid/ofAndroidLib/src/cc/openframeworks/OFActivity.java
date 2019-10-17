package cc.openframeworks;

import android.Manifest;
import android.app.Activity;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;

public abstract class OFActivity extends Activity{
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


	private boolean create_first = true;
	
	@Override
	protected void onCreate(Bundle arg0) {
		super.onCreate(arg0);
		OFAndroidLifeCycle.setActivity(this);
		OFAndroidLifeCycle.init();
		OFAndroidLifeCycle.glCreate();
		//create gesture listener
		//register the two events
		initView();
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
