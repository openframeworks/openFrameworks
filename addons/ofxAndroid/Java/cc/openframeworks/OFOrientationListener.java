package cc.openframeworks;

import android.content.Context;
import android.util.Log;
import android.view.Display;
import android.view.OrientationEventListener;
import android.view.Surface;
import android.view.WindowManager;
import androidx.annotation.Keep;

@Keep
public class OFOrientationListener extends OrientationEventListener {
    OFOrientationListener(Context context){
        super(context);
    }

    private int lastOrientation;
    private boolean firstCheck = true;

    @Override
    public void enable() {
        checkOrientation(-1);
        super.enable();
    }

    @Override
    public void disable() {
        firstCheck = false;
        super.disable();
    }

    @Override
    public void onOrientationChanged(int orientation) {
        checkOrientation(orientation);
    }

    private void checkOrientation(int newOrientation) {

        try {

            WindowManager windowManager = (WindowManager) OFAndroidLifeCycle.getActivity().getSystemService(Context.WINDOW_SERVICE);
            Display display = windowManager.getDefaultDisplay();

            if (lastOrientation != display.getRotation() || firstCheck) {
                lastOrientation = display.getRotation();
                firstCheck = false;

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
                if(OFAndroidLifeCycle.coreLibraryLoaded && OFAndroidLifeCycle.appLibraryLoaded) {
                    OFAndroid.deviceOrientationChanged(ofOrientation);
                } else {
                    firstCheck = true;
                }
            }

        } catch (Exception ex) {

        }
    }
}