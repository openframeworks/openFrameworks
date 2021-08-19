package cc.openframeworks.android;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import cc.openframeworks.OFAndroid;

import cc.openframeworks.OFAndroid;
import cc.openframeworks.OFAndroidLifeCycle;
import cc.openframeworks.OFAndroidLifeCycleHelper;


public class OFActivity extends cc.openframeworks.OFActivity{

    static { // this
        System.loadLibrary("ofApp");
        try{
            if(OFAndroidLifeCycle.coreLibraryLoaded == false) {
                Log.i("ofApp", "Loading openFrameworksAndroid Core");

                System.loadLibrary("openFrameworksAndroid");
                OFAndroidLifeCycle.coreLibraryLoaded = true;
            } else {
                Log.i("ofApp", "openFrameworksAndroid Core Already Loaded");
            }

        } catch(Throwable ex2){
            Log.i("ofApp","Failed to Load openFrameworksAndroid Exception:", ex2);;
        }

    }

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
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
}

