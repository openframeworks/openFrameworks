package cc.openframeworks;

import cc.openframeworks.OFAndroidLifeCycle.ILifeCycleCallback;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;

public abstract class OFActivity extends Activity implements ILifeCycleCallback{
	public void onGLSurfaceCreated(){}
	public void onLoadPercent(float percent){}
	public void onUnpackingResourcesDone(){}
	
	//gesture handler member
	
	private OFGLSurfaceView mGLView = null;
	
	public void initView(){  
		String packageName = this.getPackageName();
        try {
        	Log.v("OF","trying to find class: "+packageName+".R$layout");
			Class<?> layout = Class.forName(packageName+".R$layout");
			View view = this.getLayoutInflater().inflate(layout.getField("main_layout").getInt(null),null);
			this.setContentView(view);
			
			Class<?> id = Class.forName(packageName+".R$id");
			mGLView= (OFGLSurfaceView)this.findViewById(id.getField("of_gl_surface").getInt(null));
			
		} catch (Exception e) {
			Log.e("OF", "couldn't create view from layout falling back to GL only",e);
			mGLView = new OFGLSurfaceView(this);
	        this.setContentView(mGLView);
		}
	}
	
	public OFGLSurfaceView getGLContentView() {
        return mGLView;
	}
	
	@Override
	protected void onCreate(Bundle arg0) {
		// TODO Auto-generated method stub
		super.onCreate(arg0);
		//create gesture listener
		//register the two events
		OFAndroid.initOFAndroid(this.getPackageName(), this);
		initView();
		mGLView.setVisibility(View.INVISIBLE);
		OFAndroidLifeCycle.init(this, this);
		OFAndroidLifeCycle.onCreate(this);
	}
	
	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		OFAndroidLifeCycle.onResume();
		super.onResume();
	}
	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		OFAndroidLifeCycle.onPause();
		super.onPause();
	}
	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		OFAndroidLifeCycle.onDestroy();
		super.onDestroy();
	}
	
	
	@Override
	public void callbackInit() {
		// TODO Auto-generated method stub
		
	}
	
	@Override
	public void callbackCreated() {
		// TODO Auto-generated method stub
		
	}
	
	@Override
	public void callbackResumed() {
		// TODO Auto-generated method stub
		mGLView.setVisibility(View.VISIBLE);
	}
	
	@Override
	public void callbackPaused() {
		// TODO Auto-generated method stub
		
	}
	
	@Override
	public void callbackDestroed() {
		// TODO Auto-generated method stub
		
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
