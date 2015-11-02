package cc.openframeworks;

import android.content.Context;
import android.util.Log;
import android.view.Display;
import android.view.OrientationEventListener;
import android.view.Surface;
import android.view.WindowManager;

public class OFOrientationListener extends OrientationEventListener {
	OFOrientationListener(Context context){
		super(context);
	}

	private int lastOrientation;

	@Override
	public void onOrientationChanged(int orientation) {
		WindowManager windowManager = (WindowManager)OFAndroid.getContext().getSystemService(Context.WINDOW_SERVICE);
		Display display = windowManager.getDefaultDisplay();

		if(lastOrientation != display.getRotation()){
			lastOrientation = display.getRotation();

			int ofOrientation;
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
			OFAndroid.deviceOrientationChanged(ofOrientation);
		}
	}
}
