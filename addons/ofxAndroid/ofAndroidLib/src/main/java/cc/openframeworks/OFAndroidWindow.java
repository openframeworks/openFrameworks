package cc.openframeworks;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;
import javax.microedition.khronos.opengles.GL10;
//import javax.microedition.khronos.egl.EGL15;
//import javax.microedition.khronos.egl.EGL14;
import javax.microedition.khronos.egl.EGL11;
import javax.microedition.khronos.egl.EGL;

import android.app.Activity;
import android.content.Context;
import android.content.res.Configuration;
import android.graphics.ColorSpace;
import android.graphics.PixelFormat;
import android.opengl.EGL14;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.WindowManager;

import static android.opengl.EGL14.EGL_NO_CONTEXT;

class OFEGLConfigChooser implements GLSurfaceView.EGLConfigChooser {

	
    public OFEGLConfigChooser(int r, int g, int b, int a, int depth, int stencil, int samples, boolean wideGamut) {
        mRedSize = r;
        mGreenSize = g;
        mBlueSize = b;
        mAlphaSize = a;
        mDepthSize = depth;
        mStencilSize = stencil;
        mSampleSize = samples;
        mWideGamut = wideGamut;
    }
    
    public static void setGLESVersion(int version){
		GLES_VERSION = version; 
		
    	if(version==1) EGL_OPENGL_ES_BIT=1;
    	else EGL_OPENGL_ES_BIT=4;
    }

    public void setWideGamutRGB(){
        mRedSize = 10;
        mGreenSize = 10;
        mBlueSize = 10;
        mAlphaSize = 2;
        mDepthSize = 16;
        mStencilSize = 16;
        mSampleSize = 4;
        mWideGamut = true;
    }

    public void setRGB(){
        mRedSize = 8;
        mGreenSize = 8;
        mBlueSize = 8;
        mAlphaSize = 0;
        mDepthSize = 16;
        mStencilSize = 16;
        mSampleSize = 4;
        mWideGamut = true;
    }
    
    public static int getGLESVersion(){
    	return GLES_VERSION;
    }

    /* This EGL config specification is used to specify 1.x rendering.
     * We use a minimum size of 4 bits for red/green/blue, but will
     * perform actual matching in chooseConfig() below.
     */
    private static boolean DEBUG = false;
    private static int EGL_OPENGL_ES_BIT = 1;
    private static int GLES_VERSION = 1;


    public static final int EGL_GL_COLORSPACE_KHR = 0x309D;
    public static final int EGL_COLOR_COMPONENT_TYPE_EXT = 0x3339;
    public static final int EGL_COLOR_COMPONENT_TYPE_FLOAT_EXT = 0x333B;
    public static final int EGL_GL_COLORSPACE_BT2020_PQ_EXT = 0x3340;
    public static final int EGL_COVERAGE_BUFFERS_NV = 0x30E0;
    public static final int EGL_COVERAGE_SAMPLES_NV = 0x30E1;
    public static final int EGL_GL_COLORSPACE_DISPLAY_P3_PASSTHROUGH_EXT = 0x3490;

    private static int[] s_configAttribsMSAA_P3 =
            {
                    EGL14.EGL_RED_SIZE, 10,
                    EGL14.EGL_GREEN_SIZE, 10,
                    EGL14.EGL_BLUE_SIZE, 10,
                    EGL14.EGL_DEPTH_SIZE, 16,
                    EGL14.EGL_ALPHA_SIZE, 2,
                    EGL_COLOR_COMPONENT_TYPE_EXT,
                    // Requires that setEGLContextClientVersion(2) is called on the view.
                    EGL14.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT /* EGL_OPENGL_ES2_BIT */,
                    EGL14.EGL_SAMPLE_BUFFERS, 1 /* true */,
                    EGL14.EGL_SAMPLES, 4,
                    EGL_GL_COLORSPACE_KHR, EGL_GL_COLORSPACE_DISPLAY_P3_PASSTHROUGH_EXT,
                    EGL14.EGL_NONE
            };

    private static final int[] s_configAttribsMSAA =
    {
            EGL10.EGL_RED_SIZE, 8,
            EGL10.EGL_GREEN_SIZE, 8,
            EGL10.EGL_BLUE_SIZE, 8,
            EGL10.EGL_DEPTH_SIZE, 16,
            // Requires that setEGLContextClientVersion(2) is called on the view.
            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT /* EGL_OPENGL_ES2_BIT */,
            EGL10.EGL_SAMPLE_BUFFERS, 1 /* true */,
            EGL10.EGL_SAMPLES, 4,
            EGL10.EGL_NONE
    };

    private static final int[] s_configAttribsMSAAFallBack = {
            EGL10.EGL_RED_SIZE, 8,
            EGL10.EGL_GREEN_SIZE, 8,
            EGL10.EGL_BLUE_SIZE, 8,
            EGL10.EGL_DEPTH_SIZE, 16,
            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT /* EGL_OPENGL_ES2_BIT */,
            EGL_COVERAGE_BUFFERS_NV, 1 /* true */,
            EGL_COVERAGE_SAMPLES_NV, 2,  // always 5 in practice on tegra 2
            EGL10.EGL_NONE
    };
    private static final int[] s_configAttribsDefault = {
            EGL10.EGL_RED_SIZE, 5,
            EGL10.EGL_GREEN_SIZE, 6,
            EGL10.EGL_BLUE_SIZE, 5,
            EGL10.EGL_DEPTH_SIZE, 16,
            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT /* EGL_OPENGL_ES2_BIT */,
            EGL10.EGL_NONE
    };

    private static final int[] s_configAttribsDefaultES = {
            EGL10.EGL_RED_SIZE, 5,
            EGL10.EGL_GREEN_SIZE, 6,
            EGL10.EGL_BLUE_SIZE, 5,
//            EGL10.EGL_DEPTH_SIZE, 16,
            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT /* EGL_OPENGL_ES2_BIT */,
            EGL10.EGL_NONE
    };


    public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {

        /* Get the number of minimally matching EGL configurations
         */
        int[] num_config = new int[1];
        EGLConfig[] configs = null;
        num_config[0] = 0;

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O && mWideGamut == true) {
            if (!egl.eglChooseConfig(display, s_configAttribsMSAA_P3, null, 0,
                    num_config)) {
                Log.w("OF", String.format("s_configAttribsMSAA_P3 MSAA with P3 failed"));
            }
        }
        int numConfigs = num_config[0];
        if (numConfigs <= 0) {
            setRGB();
            if (!egl.eglChooseConfig(display, s_configAttribsMSAA, null, 0,
                    num_config)) {
                Log.w("OF", String.format("eglChooseConfig MSAA failed"));
            }
            numConfigs = num_config[0];
            if (numConfigs <= 0) {
                if (!egl.eglChooseConfig(display, s_configAttribsMSAAFallBack, null, 0,
                        num_config)) {
                    Log.w("OF", String.format("eglChooseConfig MSAA Fallback failed"));
                }
                numConfigs = num_config[0];
                if (numConfigs <= 0) {
                    if (!egl.eglChooseConfig(display, s_configAttribsDefault, null, 0,
                            num_config)) {
                        Log.w("OF", String.format("eglChooseConfig Default failed"));
                    }
                    numConfigs = num_config[0];
                    if (numConfigs <= 0) {
                        if (!egl.eglChooseConfig(display, s_configAttribsDefaultES, null, 0,
                                num_config)) {
                            Log.w("OF", String.format("s_configAttribsDefaultES Default failed"));
                        }
                        numConfigs = num_config[0];
                        if (numConfigs <= 0) {
                            throw new IllegalArgumentException("No configs match configSpec");
                        }
                    } else {
                        configs = new EGLConfig[numConfigs];
                        egl.eglChooseConfig(display, s_configAttribsDefault, configs, numConfigs, num_config);
                    }
                } else {
                    configs = new EGLConfig[numConfigs];
                    egl.eglChooseConfig(display, s_configAttribsMSAAFallBack, configs, numConfigs, num_config);
                }
            } else {
                configs = new EGLConfig[numConfigs];
                egl.eglChooseConfig(display, s_configAttribsMSAA, configs, numConfigs, num_config);
            }
        } else {
            configs = new EGLConfig[numConfigs];
            egl.eglChooseConfig(display, s_configAttribsMSAA_P3, configs, numConfigs, num_config);
        }
        /* Allocate then read the array of minimally matching EGL configs
         */


        if (DEBUG) {
             printConfigs(egl, display, configs);
        }
        /* Now return the "best" one
         */

        EGLConfig finalConfig =  chooseConfig(egl, display, configs);
        if (DEBUG) {
            printConfig(egl, display, finalConfig);
        }
        return finalConfig;

    }

    public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display,
            EGLConfig[] configs) {

        EGLConfig foundConfig = null;
        for(EGLConfig config : configs) {
            int d = findConfigAttrib(egl, display, config,
                    EGL10.EGL_DEPTH_SIZE, 0);
            int s = findConfigAttrib(egl, display, config,
                    EGL10.EGL_STENCIL_SIZE, 0);

            // We need at least mDepthSize and mStencilSize bits
            //if (d < mDepthSize || s < mStencilSize)
             //   continue;

            // We want an *exact* match for red/green/blue/alpha
            int r = findConfigAttrib(egl, display, config,
                    EGL10.EGL_RED_SIZE, 0);
            int g = findConfigAttrib(egl, display, config,
                        EGL10.EGL_GREEN_SIZE, 0);
            int b = findConfigAttrib(egl, display, config,
                        EGL10.EGL_BLUE_SIZE, 0);
            int a = findConfigAttrib(egl, display, config,
                    EGL10.EGL_ALPHA_SIZE, 0);

            int gamut =  findConfigAttrib(egl, display, config,
                    EGL_GL_COLORSPACE_KHR, 0);

            if(gamut != 0) {
                Log.w("OF", String.format("WideGamut: is ", gamut));
            }

            int sb = findConfigAttrib(egl, display, config,
                    EGL10.EGL_SAMPLE_BUFFERS, 1);
            int samples = findConfigAttrib(egl, display, config,
                    EGL10.EGL_SAMPLES, 0);

            if (r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && samples == mSampleSize) {
                Log.w("OF", String.format("WideGamut:enabled and Enabled MSAAx%d", mSampleSize));
                if(foundConfig == null)
                    foundConfig = config;
                else {
                    Log.i("OF", String.format("Already Found. New Row: Config Row: r%d g:%d b%d a:%d s:%d gam:%d", r,g,b,a,samples, gamut));
                }
            } else {
                Log.i("OF", String.format("Config Row: r%d g:%d b%d a:%d s:%d gam:%d", r,g,b,a,samples, gamut));
            }

            if (r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && samples == mSampleSize) {
                Log.w("OF", String.format("Enabled MSAAx%d", mSampleSize));
                if(foundConfig == null)
                    foundConfig = config;
                else {
                    Log.i("OF", String.format("Already Found. New Row: Config Row: r%d g:%d b%d a:%d s:%d gam:%d", r,g,b,a,samples, gamut));
                }
            }

            if (r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && samples == 2 && mSampleSize > 2) {
                Log.w("OF", String.format("Enabled MSAAx%d ", mSampleSize));
                if(foundConfig == null)
                    foundConfig = config;
                else {
                    Log.i("OF", String.format("Already Found. New Row: Config Row: r%d g:%d b%d a:%d s:%d gam:%d", r,g,b,a,samples, gamut));
                }
            }

            if (r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize) {
                if(foundConfig == null)
                    foundConfig = config;
                else {
                    Log.i("OF", String.format("Already Found. New Row: Config Row: r%d g:%d b%d a:%d s:%d gam:%d", r,g,b,a,samples, gamut));
                }
            }

        }
        return foundConfig;
    }

    private int findConfigAttrib(EGL10 egl, EGLDisplay display,
            EGLConfig config, int attribute, int defaultValue) {

        if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) {
            return mValue[0];
        }
        return defaultValue;
    }

    private void printConfigs(EGL10 egl, EGLDisplay display,
        EGLConfig[] configs) {
        int numConfigs = configs.length;
        Log.w("OF", String.format("%d configurations", numConfigs));
        for (int i = 0; i < numConfigs; i++) {
            Log.w("OF", String.format("Configuration %d:\n", i));
            printConfig(egl, display, configs[i]);
        }
    }

    private void printConfig(EGL10 egl, EGLDisplay display,
            EGLConfig config) {
        int[] attributes = {
                EGL10.EGL_BUFFER_SIZE,
                EGL10.EGL_ALPHA_SIZE,
                EGL10.EGL_BLUE_SIZE,
                EGL10.EGL_GREEN_SIZE,
                EGL10.EGL_RED_SIZE,
                EGL10.EGL_DEPTH_SIZE,
                EGL10.EGL_STENCIL_SIZE,
                EGL10.EGL_CONFIG_CAVEAT,
                EGL10.EGL_CONFIG_ID,
                EGL10.EGL_LEVEL,
                EGL10.EGL_MAX_PBUFFER_HEIGHT,
                EGL10.EGL_MAX_PBUFFER_PIXELS,
                EGL10.EGL_MAX_PBUFFER_WIDTH,
                EGL10.EGL_NATIVE_RENDERABLE,
                EGL10.EGL_NATIVE_VISUAL_ID,
                EGL10.EGL_NATIVE_VISUAL_TYPE,
                0x3030, // EGL10.EGL_PRESERVED_RESOURCES,
                EGL10.EGL_SAMPLES,
                EGL10.EGL_SAMPLE_BUFFERS,
                EGL10.EGL_SURFACE_TYPE,
                EGL10.EGL_TRANSPARENT_TYPE,
                EGL10.EGL_TRANSPARENT_RED_VALUE,
                EGL10.EGL_TRANSPARENT_GREEN_VALUE,
                EGL10.EGL_TRANSPARENT_BLUE_VALUE,
                0x3039, // EGL10.EGL_BIND_TO_TEXTURE_RGB,
                0x303A, // EGL10.EGL_BIND_TO_TEXTURE_RGBA,
                0x303B, // EGL10.EGL_MIN_SWAP_INTERVAL,
                0x303C, // EGL10.EGL_MAX_SWAP_INTERVAL,
                EGL10.EGL_LUMINANCE_SIZE,
                EGL10.EGL_ALPHA_MASK_SIZE,
                EGL10.EGL_COLOR_BUFFER_TYPE,
                EGL10.EGL_RENDERABLE_TYPE,
                0x3042, // EGL10.EGL_CONFORMANT
                0x309D, // EGL_GL_COLORSPACE_KHR
                0x3339, //  EGL_COLOR_COMPONENT_TYPE_EXT
                0x333B, //EGL_COLOR_COMPONENT_TYPE_FLOAT_EXT
                0x3340, // EGL_GL_COLORSPACE_BT2020_PQ_EXT
                0x30E0, //EGL_COVERAGE_BUFFERS_NV
                0x30E1, //EGL_COVERAGE_SAMPLES_NV
                0x3490, //EGL_GL_COLORSPACE_DISPLAY_P3_PASSTHROUGH_EXT
                EGL_COVERAGE_BUFFERS_NV, //
                EGL_COVERAGE_SAMPLES_NV, // always 5 in practice on tegra 2
        };
        String[] names = {
                "EGL_BUFFER_SIZE",
                "EGL_ALPHA_SIZE",
                "EGL_BLUE_SIZE",
                "EGL_GREEN_SIZE",
                "EGL_RED_SIZE",
                "EGL_DEPTH_SIZE",
                "EGL_STENCIL_SIZE",
                "EGL_CONFIG_CAVEAT",
                "EGL_CONFIG_ID",
                "EGL_LEVEL",
                "EGL_MAX_PBUFFER_HEIGHT",
                "EGL_MAX_PBUFFER_PIXELS",
                "EGL_MAX_PBUFFER_WIDTH",
                "EGL_NATIVE_RENDERABLE",
                "EGL_NATIVE_VISUAL_ID",
                "EGL_NATIVE_VISUAL_TYPE",
                "EGL_PRESERVED_RESOURCES",
                "EGL_SAMPLES",
                "EGL_SAMPLE_BUFFERS",
                "EGL_SURFACE_TYPE",
                "EGL_TRANSPARENT_TYPE",
                "EGL_TRANSPARENT_RED_VALUE",
                "EGL_TRANSPARENT_GREEN_VALUE",
                "EGL_TRANSPARENT_BLUE_VALUE",
                "EGL_BIND_TO_TEXTURE_RGB",
                "EGL_BIND_TO_TEXTURE_RGBA",
                "EGL_MIN_SWAP_INTERVAL",
                "EGL_MAX_SWAP_INTERVAL",
                "EGL_LUMINANCE_SIZE",
                "EGL_ALPHA_MASK_SIZE",
                "EGL_COLOR_BUFFER_TYPE",
                "EGL_RENDERABLE_TYPE",
                "EGL_CONFORMANT",
                "EGL_GL_COLORSPACE_KHR",
                "EGL_COLOR_COMPONENT_TYPE_EXT",
                "EGL_COLOR_COMPONENT_TYPE_FLOAT_EXT",
                "EGL_GL_COLORSPACE_BT2020_PQ_EXT",
                "EGL_COVERAGE_BUFFERS_NV",
               "EGL_COVERAGE_SAMPLES_NV",
                "EGL_GL_COLORSPACE_DISPLAY_P3_PASSTHROUGH_EXT",
                "EGL_COVERAGE_BUFFERS_NV",
                "EGL_COVERAGE_SAMPLES_NV"
        };
        int[] value = new int[1];
        for (int i = 0; i < attributes.length; i++) {
            int attribute = attributes[i];
            String name = names[i];
            if ( egl.eglGetConfigAttrib(display, config, attribute, value)) {
                Log.w("OF", String.format("  %s: %d\n", name, value[0]));
            } else {
                // Log.w(TAG, String.format("  %s: failed\n", name));
                while (egl.eglGetError() != EGL10.EGL_SUCCESS);
            }
        }
    }

    // Subclasses can adjust these values:
    protected int mRedSize;
    protected int mGreenSize;
    protected int mBlueSize;
    protected int mAlphaSize;
    protected int mDepthSize;
    protected int mStencilSize;
    protected int mSampleSize;
    protected boolean mWideGamut;
    private int[] mValue = new int[1];
}

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
    public OFGLSurfaceView(Context context,AttributeSet attributes) {
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
//        post(new Runnable() {
//            @Override
//            public void run() {
//                setBackgroundResource(0);
//            }
//        });
    }

    public OFEGLConfigChooser getConfigChooser() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                Log.i("OF","getConfigChooser::WideGamut Config Chooser");
                return new OFEGLConfigChooser(8, 8, 8, 0, 16, 0, 4, false);
        }
        Log.i("OF","getConfigChooser::sRGB Config");
        return new OFEGLConfigChooser(8, 8, 8, 0, 16, 0, 4, false);

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

//    @Override
//	public void surfaceDestroyed(SurfaceHolder holder) {
//        Log.i("OF","surfaceDestroyed");
//    	//super.surfaceDestroyed(holder);
//    	mHolder = null;
//    	//mSurface = null;
//		//OFAndroid.onSurfaceDestroyed();
//        //mRenderer.exit();
//	}

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

class OFAndroidWindow implements GLSurfaceView.Renderer {
	
	public OFAndroidWindow(int w, int h){
        setResolution(w,h, true);
	}

	public void setResolution(int w, int h, boolean updateSurface) {
        Log.i("OF","setResolution:width:" + w + " height:" + h);
        this.w = w;
        this.h = h;

        if(w != 0 && h != 0){
            resolutionSetup = true;
            if(updateSurface) onSurfaceChanged(w,h);
        }
    }

    public static void surfaceHasBeenDestroyed() {
        Log.i("OF","surfaceHasBeenDestroyed");
        OFAndroid.onSurfaceDestroyed();
        has_surface = false;
        exit();
    }
	
	@Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {


		Log.i("OF","onSurfaceCreated");
		// notify that old surface was destroyed
        boolean hasDestroyed = false;
		if(this.has_surface) {
            Log.i("OF","onSurfaceCreated - has_surface destroy");
            surfaceHasBeenDestroyed();
            hasDestroyed = true;
		}
		
		// notify that new surface was created
		this.has_surface = true;
		OFAndroid.onSurfaceCreated();
		Activity activity = OFAndroidLifeCycle.getActivity();
		if(OFActivity.class.isInstance(activity)) {
            ((OFActivity) activity).onGLSurfaceCreated();
        }
        else {
            try {
                Log.e("OF", "setup OFActivity.class.is NOT Instance(activity)" + activity.getLocalClassName());
            } catch (Exception e) {

            }
        }

		if(hasDestroyed) {
		    setup();
        }
		OFAndroid.updateRefreshRates();
		return;
    }
	
	@Override
    public void onSurfaceChanged(GL10 gl, int w, int h) {
	    onSurfaceChanged(w,h);
    }

    public void onSurfaceChanged(int w, int h) {
        Log.i("OF","onSurfaceChanged width:" + w + " height:" + h);
        setResolution(w,h, false);
        if(!setup && OFAndroid.unpackingDone){
            setup();
        } else if(!setup && !OFAndroid.unpackingDone) {
            Log.i("OF","onSurfaceChanged not setup however !OFAndroid.unpackingDone");
            setup();
        } else if(setup && OFAndroid.unpackingDone) {
            Log.i("OF","onSurfaceChanged setup already");

        }
        OFGestureListener.swipe_Min_Distance = (int)(Math.max(w, h)*.04);
        OFGestureListener.swipe_Max_Distance = (int)(Math.max(w, h)*.6);
        OFAndroid.resize(w, h);
    }
    
    private void setup(){
        Log.i("OF","setup");
        if(w <= 0 || h <= 0) {
            Log.e("OF","setup width or height is <=0. Will cause strange effects");
        }
    	OFAndroid.setup(w,h);
    	setup = true;
    	android.os.Process.setThreadPriority(8);
    	OFGestureListener.swipe_Min_Distance = (int)(Math.max(w, h)*.04);
    	OFGestureListener.swipe_Max_Distance = (int)(Math.max(w, h)*.6);
    	Activity activity = OFAndroidLifeCycle.getActivity();
		if(OFActivity.class.isInstance(activity)) {
            ((OFActivity) activity).onGLSurfaceCreated();
        }
		else {
		    try {
                Log.e("OF", "setup OFActivity.class.is NOT Instance(activity)" + activity.getLocalClassName());
            } catch (Exception e) {

            }
        }
    }
    
    public static void exit() {
    	setup = false;
    }



    @Override
    public void onDrawFrame(GL10 gl) {
            // Remove the initial background
            if (!initialRender) {
                initialRender = true;
                if( OFAndroidLifeCycle.getGLView() != null) OFAndroidLifeCycle.getGLView().setBackgroundResourceClear();
            }
    	if(setup && OFAndroid.unpackingDone){
    	    if(android.opengl.EGL14.eglGetCurrentContext() != EGL_NO_CONTEXT)
    		    OFAndroid.render();
    	    else {
                Log.e("OF", "ofAndroidWindow::onDrawFrame GLContext = EGL_NO_CONTEXT BAD. Restarting Window");
                setup = false;
                setup();
            }
    	}else if(!setup && OFAndroid.unpackingDone){
    		setup();
    	}else{
    		gl.glClear(GL10.GL_COLOR_BUFFER_BIT);
    		gl.glClearColor(0f, 0f, 0f, 1.0f);
    	}
    }
    
    public boolean isSetup(){
    	return setup;
    }

    public boolean isResolutionSetup(){
        return resolutionSetup;
    }


    private static boolean setup;
    private static boolean resolutionSetup;
    private static boolean initialRender;
    private int w,h;
    public static boolean has_surface = false;
}
