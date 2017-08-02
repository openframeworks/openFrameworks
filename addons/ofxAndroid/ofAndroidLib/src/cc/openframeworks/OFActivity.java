package cc.openframeworks;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.content.Context;

public abstract class OFActivity extends Activity{
	public void onGLSurfaceCreated(){}
	public void onLoadPercent(float percent){}
	public void onUnpackingResourcesDone(){}
	
	//gesture handler member
	private ViewGroup mOFGlSurfaceContainer;
	
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


	
	@Override
	protected void onCreate(Bundle arg0) {
		// TODO Auto-generated method stub
		super.onCreate(arg0);
		OFAndroidLifeCycle.setActivity(this);
		OFAndroidLifeCycle.init();
		OFAndroidLifeCycle.glCreate();
		//create gesture listener
		//register the two events
		initView();
	}
	
	@Override
	protected void onResume() {
		OFAndroidLifeCycle.setActivity(this);
		OFAndroidLifeCycle.glResume(mOFGlSurfaceContainer);
		super.onResume();
	}
	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		OFAndroidLifeCycle.glPause();
		super.onPause();
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
