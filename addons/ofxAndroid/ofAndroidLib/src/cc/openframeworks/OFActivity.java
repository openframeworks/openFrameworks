package cc.openframeworks;

import android.app.Activity;

public abstract class OFActivity extends Activity {
	public void onGLSurfaceCreated(){}
	public void onLoadPercent(float percent){}
	public void onUnpackingResourcesDone(){}
}
