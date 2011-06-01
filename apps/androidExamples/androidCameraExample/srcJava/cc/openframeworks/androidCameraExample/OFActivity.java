package cc.openframeworks.androidCameraExample;

import android.app.Activity;
import android.os.Bundle;
import cc.openframeworks.OFAndroid;


public class OFActivity extends Activity{

	@Override
    public void onCreate(Bundle savedInstanceState)
    { 
        super.onCreate(savedInstanceState);
        String packageName = getPackageName();

        ofApp = new OFAndroid(packageName,this);
    }
	
	@Override
	protected void onStart(){
		super.onStart();
		ofApp.start();
	}
	
	@Override
	protected void onRestart(){
		super.onRestart();
		ofApp.restart();
	}
	
    @Override
    protected void onPause() {
        super.onPause(); 
        ofApp.pause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        ofApp.resume();
    }
    
    @Override
    protected void onStop() {
        super.onStop();
        ofApp.stop();
    }
    
    @Override
    protected void onDestroy(){
		super.onDestroy();
		ofApp.destroy();
    }
    
	OFAndroid ofApp;
}



