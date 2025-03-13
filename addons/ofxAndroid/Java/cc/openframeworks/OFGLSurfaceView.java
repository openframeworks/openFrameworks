package cc.openframeworks;

import android.app.Activity;
import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.InputDevice;
import android.view.InputEvent;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceControl;
import android.view.SurfaceHolder;
import android.view.View;

import androidx.annotation.Keep;

import static android.opengl.EGL14.EGL_NO_CONTEXT;
import static android.view.Surface.CHANGE_FRAME_RATE_ALWAYS;
import static android.view.Surface.FRAME_RATE_COMPATIBILITY_DEFAULT;

@Keep
class OFGLSurfaceView extends GLSurfaceView implements View.OnFocusChangeListener {

    ContextFactory factory;

    public OFGLSurfaceView(Context context) {
        super(context);
        Log.i("OF","OFGLSurfaceView():" + context.toString());
        init(false, 24, 8);
        setFocusable(true);
        setFocusableInTouchMode(true);
        requestFocus();
    }

    public OFGLSurfaceView(Context context, boolean translucent, int depth, int stencil) {
        super(context);
        Log.i("OF","OFGLSurfaceView():" + context.toString());
        init(translucent, depth, stencil);
    }

    private void init(boolean translucent, int depth, int stencil) {

        Log.i("OF","OFGLSurfaceView():init translucent:" + translucent + " | depth:" + depth + " | stencil:" + stencil + " | msaa:" + OFAndroid.samples + "| Max FPS:" + OFAndroid.maximumFrameRate);
        if (translucent) {
            this.getHolder().setFormat(PixelFormat.TRANSLUCENT);
        } else {
            this.getHolder().setFormat(PixelFormat.OPAQUE);
        }

        try {
                factory = new ContextFactory();
                setEGLContextFactory(factory);
        } catch (Exception exception){
                Log.w("OF", "Could not set ContextFactory ", exception);
        }
        setClientVersion();
        int width = getWidth();
        int height = getHeight();

        if(width <= 0 || height <= 0 ) {
            try {
                DisplayMetrics displayMetrics = new DisplayMetrics();
                Display display = ((Activity) getContext()).getWindowManager().getDefaultDisplay();
                display.getRealMetrics(displayMetrics);
                height = displayMetrics.heightPixels;
                width = displayMetrics.widthPixels;
            } catch (Exception exception){
                Log.w("OF", "Could not get Window for Display ", exception);
            }
        }
        
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.M) {
            OFAndroid.samples=1; // force no AA old devices
            depth = 24;
        }
        if(OFAndroid.maxSamples <= OFAndroid.samples)
            OFAndroid.samples = OFAndroid.maxSamples;

        OFEGLConfigChooser configChooser = getConfigChooser(8, 8, 8, 8, depth, 8, OFAndroid.samples, false);

        try {
            setEGLConfigChooser(configChooser);
        } catch (Exception ex) {
            Log.e("OF", "setEGLConfigChooser failed 8,8,8,8,depth,1 ", ex);
            try {
                configChooser = getConfigChooser(8, 8, 8, 8, 16, 1, OFAndroid.samples, false);
                setEGLConfigChooser(configChooser);
            } catch (Exception exOne) {
                Log.e("OF", "setEGLConfigChooser failed exOne: Trying 5, 6, 5, 1, 16, 1 ", ex);
                try {
                    configChooser = getConfigChooser(5, 6, 5, 1, 16, 1, OFAndroid.samples, false);
                    setEGLConfigChooser(configChooser);
                } catch (Exception exceptionTwo) {
                   try {
                       Log.e("OF", "setEGLConfigChooser failed exOne: Trying 10, 10, 10, 10, 16, 1 ", exceptionTwo);
                       configChooser = getConfigChooser(10, 10, 10, 10, 16, 1, OFAndroid.samples, false);
                       setEGLConfigChooser(configChooser);

                    } catch (Exception exceptionThree) {
                       configChooser = getConfigChooser(8, 8, 8, 1, 16, 1, 0, false);
                       setEGLConfigChooser(configChooser);
                    }
                }
            }
        }

        mRenderer = new OFAndroidWindow(width, height);
        OFAndroid.samples = configChooser.getSampleSize();
        if(OFAndroid.samples > OFAndroid.maxSamples)
            OFAndroid.samples = OFAndroid.maxSamples;
        OFAndroid.setSampleSize(OFAndroid.samples);
        setRenderer(mRenderer);
        setRenderMode(OFGLSurfaceView.RENDERMODE_CONTINUOUSLY);
        display = getDisplay();
        post(new Runnable() {
            @Override
            public void run() {

                mRenderer.setResolution(getWidth(), getHeight(), true);

            }
        });
    }

    @Override
    public boolean onCapturedPointerEvent(MotionEvent motionEvent) {
        // Get the coordinates required by your app
        float verticalOffset = motionEvent.getY();
        // Use the coordinates to update your view and return true if the event was
        // successfully processed
        return true;
    }

    @Override
    public void setRenderer(GLSurfaceView.Renderer renderer) {
        Log.i("OF","setRenderer:" + renderer.toString());
        super.setRenderer(renderer);
    }

    public OFAndroidWindow getRenderer() {
        return mRenderer;
    }

    @Override
    public void setRenderMode(int renderMode) {

        super.setRenderMode(renderMode);
    }

    @Override
    public void onFocusChange(View v, boolean hasFocus) {
        if(v != null)
            Log.i("OF","view onFocusChange:" + v.toString() + " hasFocus:" + hasFocus);
        if(doNotDraw) return;
        requestFocus();
    }

    public void setFrameRate(float frameRate) {
//        if(doNotDraw) return;

        //if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.) {
            Log.i("OF","setFrameRate:" + frameRate);
            try {
                if(mSurface != null && mSurface.isValid()) {
                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
                        mSurface.setFrameRate(frameRate,
                                FRAME_RATE_COMPATIBILITY_DEFAULT, CHANGE_FRAME_RATE_ALWAYS);
                    } else {

                        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
                            mSurface.setFrameRate(frameRate,
                                    FRAME_RATE_COMPATIBILITY_DEFAULT);
                        }
                    }
                    // https://developer.android.com/reference/android/view/Surface#CHANGE_FRAME_RATE_ALWAYS
                } else {
                    Log.i("OF","setFrameRate called and no Surface");
                }
            } catch (Exception ex) {
                Log.e("OF", "setFrameRate Exception:", ex);
            }
       // }

        // SurfaceControl Fallback for OnePlus Phones
        try {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
                SurfaceControl sc = this.getSurfaceControl();
                SurfaceControl.Transaction transaction = null;
                transaction = new SurfaceControl.Transaction();
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
                    if(transaction != null && sc != null && sc.isValid()) {
                        transaction.setFrameRate(sc, frameRate, Surface.FRAME_RATE_COMPATIBILITY_FIXED_SOURCE);
                        transaction.apply();
                    }
                }
            }
        } catch (Exception ex) {
            Log.e("OF", "SurfaceControl setFrameRate Exception:", ex);
        }

    }

    public void OnResume() {
        if(doNotDraw) return;
        if(mRenderer != null && mRenderer.isSetup()) {

            int width = getWidth();
            int height = getHeight();

            Log.i("OF","OnResume w:" + width + " h:" + height);
            mRenderer.setResolution(width, height, true);
        } else{
            Log.i("OF","OnResume no mRenderer:" + (mRenderer!=null &&mRenderer.isSetup()));
        }
    }

    @Override
    public void setDebugFlags(int debugFlags) {
        super.setDebugFlags(debugFlags);
    }

    @Override
    public void setPreserveEGLContextOnPause(boolean preserveOnPause) {
        super.setPreserveEGLContextOnPause(preserveOnPause);
    }

    @Override
    public void setEGLContextFactory(EGLContextFactory factory) {
        super.setEGLContextFactory(factory);
        Log.i("OF","setEGLContextFactory");
    }

    @Override
    public void setEGLWindowSurfaceFactory(EGLWindowSurfaceFactory factory) {
        super.setEGLWindowSurfaceFactory(factory);
        Log.i("OF","setEGLWindowSurfaceFactory");
    }

    @Override
    public void setEGLConfigChooser(EGLConfigChooser configChooser) {
        super.setEGLConfigChooser(configChooser);
        Log.i("OF","setEGLConfigChooser");
    }

    @Override
    public void setEGLContextClientVersion(int version) {
        super.setEGLContextClientVersion(version);
        Log.i("OF","setEGLContextClientVersion:" + version);
    }

    public void setBackgroundResourceClear() {
        post(new Runnable() {
            @Override
            public void run() {
                setBackgroundColor(0);
            }
        });
    }

    public OFEGLConfigChooser getConfigChooser() {
        return getConfigChooser(8,8,8,8,8,1,OFAndroid.samples,false);
    }

    public OFEGLConfigChooser getConfigChooser(int r, int g, int b, int a, int depth, int stencil, int samples, boolean gamut) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            Log.i("OF","getConfigChooser::WideGamut Config Chooser");
            return new OFEGLConfigChooser(r, g, b, a, depth, stencil, samples, gamut);
        }
        Log.i("OF","getConfigChooser::sRGB Config");
        return new OFEGLConfigChooser(r, g, b, a, depth, stencil, samples, false);
    }

    public void setClientVersion() {
        if(OFEGLConfigChooser.getGLESVersion()>=2){
            Log.i("OF","SetClientVersion::setClientVersion setEGLContextClientVersion " + OFEGLConfigChooser.getGLESVersion());
            setEGLContextClientVersion(OFEGLConfigChooser.getGLESVersion());
        } else {
            Log.i("OF","OFGLSurfaceView::setClientVersion setEGLContextClientVersion " + OFEGLConfigChooser.getGLESVersion());
        }
    }



    @Override
    public void onPause() {
        /*
         * We call a "pause" function in our Renderer class, which tells it to save state and
         * go to sleep.  Because it's running in the Renderer thread, we call it through
         * queueEvent(), which doesn't wait for the code to actually execute.  In theory the
         * application could be killed shortly after we return from here, which would be bad if
         * it happened while the Renderer thread was still saving off important state.  We need
         * to wait for it to finish.
         */

        //setRenderMode(OFGLSurfaceView.RENDERMODE_WHEN_DIRTY);
        
        super.onPause();

        //Log.d(TAG, "asking renderer to pause");
//        queueEvent(new Runnable() {
//            @Override public void run() {
//                if(mRenderer != null)
//                mRenderer.onViewPause(syncObj);
//            }});

        //Log.d(TAG, "renderer pause complete");
    }
   

    @Override
    public void onResume() {
        if(doNotDraw) return;
        Log.i("OF","OFGLSurfaceView::onResume");
        if(mRenderer != null)
        {
            if(android.opengl.EGL14.eglGetCurrentContext() == EGL_NO_CONTEXT) {
                Log.w("OF", "OFGLSurfaceView::onResume however no CONTEXT");
                // surfaceDestroyed(this.mHolder);

                display = getDisplay();
                getHolder().setFormat(PixelFormat.OPAQUE);
                
                setRenderMode(OFGLSurfaceView.RENDERMODE_CONTINUOUSLY);

                post(new Runnable() {
                    @Override
                    public void run() {
                        int width = getWidth();
                        int height = getHeight();
                        if (mRenderer != null){
                            if(OFAndroid.maxSamples <= OFAndroid.samples)
                                OFAndroid.samples = OFAndroid.maxSamples;
                            OFAndroid.setSampleSize(OFAndroid.samples);
                            mRenderer.setResolution(width, height, true);
                        }
                    }
                });
            }
        } else if(mRenderer == null) {
            Log.e("OF", "OFGLSurfaceView::onResume however mRenderer is NULL");
            surfaceDestroyed(this.mHolder);

            mRenderer = new OFAndroidWindow(getWidth(), getHeight());
            getHolder().setFormat(PixelFormat.OPAQUE);
            if(OFAndroid.maxSamples <= OFAndroid.samples)
                OFAndroid.samples = OFAndroid.maxSamples;
            OFAndroid.setSampleSize(OFAndroid.samples);

            setRenderer(mRenderer);
            setRenderMode(OFGLSurfaceView.RENDERMODE_CONTINUOUSLY);
            display = getDisplay();
        }
        super.onResume();

    }

    @Override
    public void queueEvent(Runnable r) {
        super.queueEvent(r);
    }

// NOTE - The following has been removed because it is not a good way to determine that the opengl context was destroyed with its resources.
//        The Android SurfaceView source code is a bit confusing  - there are many times when some kind of surface is beign destroyed (eg. during window resize),
//            so it is not that surprising, that not every surfaceDestoyed callback means what we might think it means.
//        We don't need this callback that much anyways, the renderer does not call render callbacks when gl context is invalid, so the OFAndroidWindow.onSurfaceCreated callback should be enought for us to make things work.

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.i("OF","surfaceDestroyed");
        if(holder != null)
            super.surfaceDestroyed(holder);

//        mHolder = null;
//        mSurface = null;
        //OFAndroid.onSurfaceDestroyed();
        //mRenderer.exit();
        //mRenderer = null;
    }

    @Override
    public void finalize() {
        Log.i("OF","finalize");
    }

    public void setDoNotDraw() {
        Log.i("OF","setDoNotDraw");
        doNotDraw = true;
        if(mRenderer != null) {
            setRenderMode(OFGLSurfaceView.RENDERMODE_WHEN_DIRTY);
            mRenderer.setDoNotDraw();
        }
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Log.i("OF","surfaceCreated");
        if(doNotDraw) return;
        if(holder != null && holder.getSurface() != null && holder.getSurface().isValid()) {
            mHolder = holder;
            mSurface = holder.getSurface();
        }
        super.surfaceCreated(holder);
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
        Log.i("OF","surfaceChanged format:" + format + " w:" + w + " h:" + h);
        super.surfaceChanged(holder, format, w, h);
        if(doNotDraw) return;
        if(holder != null && holder.getSurface() != null && holder.getSurface().isValid()) {
            mHolder = holder;
            mSurface = holder.getSurface();
        }
        if(mRenderer != null) {
            mRenderer.setResolution(w,h, true);
        }
    }

    boolean isSetup(){
        return mRenderer != null && mRenderer.isSetup();
    }

    void setWindowResize(int w, int h){
        if(doNotDraw) return;
        Log.i("OF","setWindowResize mRenderer:" + " w:" + w + " h:" + h  );
        if(mRenderer != null && mRenderer.isSetup()) {
            mRenderer.setResolution(w,h, true);
        } else if(mRenderer != null) {
            mRenderer.setResolution(w,h, true);
        }
    }
    private boolean doNotDraw = false;
    private OFAndroidWindow mRenderer;
    private SurfaceHolder mHolder;
    private Surface mSurface;
    private Display display;

//    @Override
//    public boolean onKeyDown(int keyCode, KeyEvent event) {
//        return true;
//    }
//    @Override
//    public boolean onKeyUp(int keyCode, KeyEvent event) {
//        return true;
//    }
//    @Override
//    public boolean onGenericMotionEvent(MotionEvent event) {
//        return true;
//    }
//    @Override
//    public boolean onKeyMultiple(int keyCode, int count, KeyEvent event) {
//        return true;
//    }


    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        Log.i("OF", "onKeyDown" + keyCode + " event:" + event.toString());
        if(doNotDraw) return true;
        if ((event.getSource() & InputDevice.SOURCE_GAMEPAD)
                == InputDevice.SOURCE_GAMEPAD) {
            int deviceId = event.getDeviceId();
            OFAndroid.keyDown(keyCode+400, event);
            return true;
        }
        else if (OFAndroid.keyDown(keyCode, event)) {
            return true;
        } else {
            return super.onKeyDown(keyCode, event);
        }
    }

    @Override
    public boolean onKeyMultiple(int keyCode, int count, KeyEvent event) {
        Log.i("OF", "onKeyDown" + keyCode + " event:" + event.toString());
        if(doNotDraw) return true;
        int keyOffset = 0;
        if ((event.getSource() & InputDevice.SOURCE_GAMEPAD)
                == InputDevice.SOURCE_GAMEPAD) {
            keyOffset = 400;
        }
        int deviceId = event.getDeviceId();
        if(event.getAction() == KeyEvent.ACTION_DOWN)
            return OFAndroid.keyDown(event.getKeyCode()+keyOffset, event);
        else if(event.getAction() == KeyEvent.ACTION_UP)
            return OFAndroid.keyUp(event.getKeyCode()+keyOffset, event);
        else
            return true;

    }


    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        if(doNotDraw) return true;
        Log.i("OF", "onKeyUp" + keyCode + " event:" + event.toString());

        if ((event.getSource() & InputDevice.SOURCE_GAMEPAD)
                == InputDevice.SOURCE_GAMEPAD) {
            int deviceId = event.getDeviceId();
            return OFAndroid.keyUp(keyCode+400, event);
        }
        else if (OFAndroid.keyUp(keyCode, event)) {
            return true;
        } else {
            return super.onKeyUp(keyCode, event);
        }
    }

	@Override
	public boolean onGenericMotionEvent(MotionEvent event) {
		if(doNotDraw) return true;
		return OFAndroidController.genericMotionEvent(event);
	}
	


    


	
}
