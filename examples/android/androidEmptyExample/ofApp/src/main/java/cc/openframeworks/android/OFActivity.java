package cc.openframeworks.android;

import android.app.Activity;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.WindowManager;

import androidx.core.view.ViewCompat;
import androidx.core.view.WindowCompat;
import androidx.core.view.WindowInsetsCompat;
import androidx.core.view.WindowInsetsControllerCompat;

import cc.openframeworks.OFAndroid;
import cc.openframeworks.OFAndroidController;
import cc.openframeworks.OFAndroidLifeCycle;
import cc.openframeworks.OFAndroidLifeCycleHelper;

import com.getkeepsafe.relinker.ReLinker;



public class OFActivity extends cc.openframeworks.OFActivity {

    private static final String appName = "ofapp"; // modify this to target appName (ofApp etc)
    private static final String LOG_TAG = appName + "::OFActivity";

    private ReLinker.Logger logcatLogger = new ReLinker.Logger() {
        @Override
        public void log(String message) {
            Log.d("ReLinker", message);
        }
    };
    private OFActivity thisActivity;


    // Extremely important
    public OFActivity() {
        OFAndroidLifeCycle.coreLibraryLoaded = true;

        OFAndroid.maxSamples = 4;
        OFAndroid.maximumFrameRate = 144;

        thisActivity = this;
        ReLinker.log(logcatLogger)
                .force()
                .recursively()
                .loadLibrary(this, appName, new ReLinker.LoadListener() {
                    @Override
                    public void success() {
                        Log.i(LOG_TAG, "loadLibrary success");
                        OFAndroidLifeCycle.appLibraryLoaded = true;
                        Setup(); // very important - this will in turn call main
                    }

                    @Override
                    public void failure(Throwable t) {
                        /* Boo */
                        Log.i(LOG_TAG, "loadLibrary failure" + t.getMessage());
                    }
                });
    }

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        Log.i(LOG_TAG, "onCreate");
        super.onCreate(savedInstanceState);

        setFullscreen();
        hideSystemBars();
    }

    @Override
    public void onStart() {
        super.onStart();

    }

    @Override
    public void onDetachedFromWindow() {

    }

    // Menus
    // http://developer.android.com/guide/topics/ui/menus.html
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Create settings menu options from here, one by one or infalting an xml
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // This passes the menu option string to OF
        // you can add additional behavior from java modifying this method
        // but keep the call to OFAndroid so OF is notified of menu events
        if(OFAndroid.menuItemSelected(item.getItemId())){

            return true;
        }
        return super.onOptionsItemSelected(item);
    }


    @Override
    public boolean onPrepareOptionsMenu (Menu menu){
        // This method is called every time the menu is opened
        //  you can add or remove menu options from here
        return  super.onPrepareOptionsMenu(menu);
    }

    public void onRestore() {

        if (!OFAndroidLifeCycle.appLibraryLoaded) return;
    }

    private void hideSystemBars() {
        WindowInsetsControllerCompat windowInsetsController =
                ViewCompat.getWindowInsetsController(getWindow().getDecorView());
        if (windowInsetsController == null) {
            return;
        }
        // Configure the behavior of the hidden system bars
        windowInsetsController.setSystemBarsBehavior(
                WindowInsetsControllerCompat.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE
        );
        // Hide both the status bar and the navigation bar
        windowInsetsController.hide(WindowInsetsCompat.Type.systemBars());
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);

        if (hasFocus) {
            if (Build.VERSION.SDK_INT < Build.VERSION_CODES.KITKAT) {

                setFullscreen();
            } else {
                setFullscreen();
            }
        }	else {

        }
    }

    private void handleException(Exception e, String details) {
        Log.e(LOG_TAG, "Exception:", e);

    }

    public void setFullscreen() {
        try {
            if (Build.VERSION.SDK_INT < Build.VERSION_CODES.KITKAT) {
                // No sticky immersive mode for devices pre-kitkat
                getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                        WindowManager.LayoutParams.FLAG_FULLSCREEN);
            } else {
                View decorView = getWindow().getDecorView();
//				int uiOptions = View.SYSTEM_UI_FLAG_FULLSCREEN;
                int uiOptions = View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
                decorView.setSystemUiVisibility(uiOptions);
            }

            WindowCompat.setDecorFitsSystemWindows(getWindow(), false);

        } catch (Exception ex) {
            handleException(ex, "setFullscreen exception");
        }

        try {
            View decorView = getWindow().getDecorView();
            int[] locations = new int[2];
            decorView.getLocationInWindow(locations);
            int[] locations2 = new int[2];
            decorView.getLocationOnScreen(locations2);

        } catch (Exception ex) {
            handleException(ex, "setFullscreen exception");
        }

        WindowInsetsControllerCompat windowInsetsController =
                ViewCompat.getWindowInsetsController(getWindow().getDecorView());
        if (windowInsetsController == null) {
            return;
        }
        // Hide both the status bar and the navigation bar
        windowInsetsController.hide(WindowInsetsCompat.Type.systemBars());
        windowInsetsController.hide(WindowInsetsCompat.Type.navigationBars());
        windowInsetsController.hide(WindowInsetsCompat.Type.statusBars());
    }


}

