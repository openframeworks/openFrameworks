package cc.openframeworks.androidNativeActivityExample;

import cc.openframeworks.OFAndroidLifeCycle;
import cc.openframeworks.OFAndroidObject;
import cc.openframeworks.androidNativeActivityExample.R.id;
import cc.openframeworks.androidNativeActivityExample.R.layout;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;

public class FirstActivity extends Activity implements OFAndroidLifeCycle.ILifeCycleCallback
{

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(layout.first_activity);
		OFAndroidObject.setActivity(this);
		OFAndroidLifeCycle.init(this, this);
		findViewById(id.btnOpenActivity).setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(FirstActivity.this, OFActivity.class);
				startActivity(intent);
			}
		});
	}

	@Override
	public void callbackInit() {
		Log.d("FirstActivity", "ofAndroidApplicationInit() (in main.cpp) finished running");
	}

	@Override
	public void callbackCreated() {
		Log.d("FirstActivity", "ofAndroidActivityInit() (in main.cpp) finished running");
		
	}

	@Override
	public void callbackResumed() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void callbackPaused() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void callbackDestroed() {
		// TODO Auto-generated method stub
		
	}
	
}
