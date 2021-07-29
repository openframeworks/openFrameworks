package cc.openframeworks;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Display;
import android.view.Surface;
import android.view.SurfaceHolder;

import static android.opengl.EGL14.EGL_NO_CONTEXT;
import static android.view.Surface.FRAME_RATE_COMPATIBILITY_DEFAULT;

class OFGLSurfaceView extends GLSurfaceView {

    public OFGLSurfaceView(Context context) {
        super(context);
        setClientVersion();

        int width = getWidth();
        int height = getHeight();

//        if(width <= 0 || height <= 0 ) {
//            try {
//                DisplayMetrics displayMetrics = new DisplayMetrics();
//                Display display = ((Activity) getContext()).getWindowManager().getDefaultDisplay();
//                display.getRealMetrics(displayMetrics);
//                height = displayMetrics.heightPixels;
//                width = displayMetrics.widthPixels;
//            } catch (Exception exception){
//                Log.w("OF", "Could not get Window for Display ", exception);
//            }
//        }

        mRenderer = new OFAndroidWindow(width, height);
        getHolder().setFormat(PixelFormat.OPAQUE);
        OFEGLConfigChooser configChooser = getConfigChooser();
        setEGLConfigChooser(configChooser);
//        EGLint attribs[] = {OFEGLConfigChooser.EGL_GL_COLORSPACE_KHR,OFEGLConfigChooser.EGL_GL_COLORSPACE_BT2020_PQ_EXT, EGL_NONE };
//        EGLSurface eglSurface=EGL14.eglCreateWindowSurface(configChooser.displayUsed, configChooser,  context., attribs);

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


    @Deprecated
    public OFGLSurfaceView(Context context, AttributeSet attributes) {
        super(context,attributes);
        setClientVersion();
        mRenderer = new OFAndroidWindow(getWidth(), getHeight());
        getHolder().setFormat(PixelFormat.OPAQUE);
        OFEGLConfigChooser configChooser = getConfigChooser();
        setEGLConfigChooser(configChooser);
        setRenderer(mRenderer);
        setRenderMode(OFGLSurfaceView.RENDERMODE_CONTINUOUSLY);
        display = getDisplay();

        post(new Runnable() {
            @Override
            public void run() {
                int width = getWidth();
                int height = getHeight();

                mRenderer.setResolution(width, height, true);
            }
        });
    }

    @Override
    public void setRenderer(GLSurfaceView.Renderer renderer) {

        super.setRenderer(renderer);
    }

    public OFAndroidWindow getRenderer() {
        return mRenderer;
    }

    @Override
    public void setRenderMode(int renderMode) {

        super.setRenderMode(renderMode);
    }

    public void setFrameRate(float frameRate) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            mSurface.setFrameRate(frameRate,
                    FRAME_RATE_COMPATIBILITY_DEFAULT);
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
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            Log.i("OF","getConfigChooser::WideGamut Config Chooser");
            return new OFEGLConfigChooser(8, 8, 8, 8, 16, 0, 4, false);
        }
        Log.i("OF","getConfigChooser::sRGB Config");
        return new OFEGLConfigChooser(8, 8, 8, 8, 16, 0, 4, false);

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

        Log.i("OF","OFGLSurfaceView::onResume");
        if(mRenderer != null)
        {
            if(android.opengl.EGL14.eglGetCurrentContext() == EGL_NO_CONTEXT) {
                Log.i("OF", "OFGLSurfaceView::onResume however no CONTEXT");
                OFAndroid.render();
            }
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
        super.surfaceDestroyed(holder);
        mHolder = null;
        mSurface = null;
        OFAndroid.onSurfaceDestroyed();
        mRenderer.exit();
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Log.i("OF","surfaceCreated");
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
        if(holder != null && holder.getSurface() != null && holder.getSurface().isValid()) {
            mHolder = holder;
            mSurface = holder.getSurface();
        }
        if(mRenderer != null && mRenderer.isSetup()) {
            mRenderer.setResolution(w,h, true);
        }

    }


    boolean isSetup(){
        return mRenderer != null && mRenderer.isSetup();
    }

    void setWindowResize(int w, int h){
        if(mRenderer != null && mRenderer.isSetup()) {
            mRenderer.setResolution(w,h, true);
        }
    }

    private OFAndroidWindow mRenderer;
    private SurfaceHolder mHolder;
    private Surface mSurface;
    private Display display;
}
