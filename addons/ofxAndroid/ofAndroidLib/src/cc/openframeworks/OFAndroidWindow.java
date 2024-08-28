package cc.openframeworks;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;

class OFEGLConfigChooser implements GLSurfaceView.EGLConfigChooser {
	
    public OFEGLConfigChooser(int r, int g, int b, int a, int depth, int stencil, int samples) {
        mRedSize = r;
        mGreenSize = g;
        mBlueSize = b;
        mAlphaSize = a;
        mDepthSize = depth;
        mStencilSize = stencil;
        mSampleSize = samples;
    }
    
    public static void setGLESVersion(int version){
		GLES_VERSION = version; 
		
    	if(version==1) EGL_OPENGL_ES_BIT=1;
    	else EGL_OPENGL_ES_BIT=4;
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
    private static int[] s_configAttribsMSAA =
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

    private static final int EGL_COVERAGE_BUFFERS_NV = 0x30E0;
    private static final int EGL_COVERAGE_SAMPLES_NV = 0x30E1;
    private static int[] s_configAttribsMSAAFallBack = {
            EGL10.EGL_RED_SIZE, 8,
            EGL10.EGL_GREEN_SIZE, 8,
            EGL10.EGL_BLUE_SIZE, 8,
            EGL10.EGL_DEPTH_SIZE, 16,
            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT /* EGL_OPENGL_ES2_BIT */,
            EGL_COVERAGE_BUFFERS_NV, 1 /* true */,
            EGL_COVERAGE_SAMPLES_NV, 2,  // always 5 in practice on tegra 2
            EGL10.EGL_NONE
    };
    private static int[] s_configAttribsDefault = {
            EGL10.EGL_RED_SIZE, 5,
            EGL10.EGL_GREEN_SIZE, 6,
            EGL10.EGL_BLUE_SIZE, 5,
            EGL10.EGL_DEPTH_SIZE, 16,
            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT /* EGL_OPENGL_ES2_BIT */,
            EGL10.EGL_NONE
    };

    private static int[] s_configAttribsDefaultES = {
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

        if (!egl.eglChooseConfig(display, s_configAttribsMSAA, null, 0,
                num_config)) {
            Log.w("OF", String.format("eglChooseConfig MSAA failed"));
        }
        int numConfigs = num_config[0];
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
        for(EGLConfig config : configs) {
            int d = findConfigAttrib(egl, display, config,
                    EGL10.EGL_DEPTH_SIZE, 0);
            int s = findConfigAttrib(egl, display, config,
                    EGL10.EGL_STENCIL_SIZE, 0);

            // We need at least mDepthSize and mStencilSize bits
            if (d < mDepthSize || s < mStencilSize)
                continue;

            // We want an *exact* match for red/green/blue/alpha
            int r = findConfigAttrib(egl, display, config,
                    EGL10.EGL_RED_SIZE, 0);
            int g = findConfigAttrib(egl, display, config,
                        EGL10.EGL_GREEN_SIZE, 0);
            int b = findConfigAttrib(egl, display, config,
                        EGL10.EGL_BLUE_SIZE, 0);
            int a = findConfigAttrib(egl, display, config,
                    EGL10.EGL_ALPHA_SIZE, 0);

            int sb = findConfigAttrib(egl, display, config,
                    EGL10.EGL_SAMPLE_BUFFERS, 1);
            int samples = findConfigAttrib(egl, display, config,
                    EGL10.EGL_SAMPLES, 0);

            if (r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && samples == mSampleSize) {
                Log.w("OF", String.format("Enabled MSAAx%d", mSampleSize));
                return config;
            }

            if (r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && samples == 2 && mSampleSize > 2) {
                Log.w("OF", String.format("Enabled MSAAx%d ", mSampleSize));
                return config;
            }

            if (r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize)
                return config;
        }
        return null;
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
                0x3042 // EGL10.EGL_CONFORMANT
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
                "EGL_CONFORMANT"
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
    private int[] mValue = new int[1];
}

class OFGLSurfaceView extends GLSurfaceView{
	public OFGLSurfaceView(Context context) {
        super(context);
        mRenderer = new OFAndroidWindow(getWidth(),getHeight());
        if(OFEGLConfigChooser.getGLESVersion()>=2){
        	setEGLContextClientVersion(OFEGLConfigChooser.getGLESVersion());
        }
        getHolder().setFormat( PixelFormat.OPAQUE );
        OFEGLConfigChooser configChooser = new OFEGLConfigChooser(8,8,8,0,16,0, 4);
        setEGLConfigChooser(configChooser);
        setRenderer(mRenderer);
    }
	
	@Deprecated
	public OFGLSurfaceView(Context context,AttributeSet attributes) {
        super(context,attributes);
        mRenderer = new OFAndroidWindow(getWidth(),getHeight());
        if(OFEGLConfigChooser.getGLESVersion()>=2){
        	setEGLContextClientVersion(OFEGLConfigChooser.getGLESVersion());
        }
        getHolder().setFormat( PixelFormat.OPAQUE );
        OFEGLConfigChooser configChooser = new OFEGLConfigChooser(8,8,8,0,16,0, 4);
        setEGLConfigChooser(configChooser);
        setRenderer(mRenderer);
    }

// NOTE - The following has been removed because it is not a good way to determine that the opengl context was destroyed with its resources.
//        The Android SurfaceView source code is a bit confusing  - there are many times when some kind of surface is beign destroyed (eg. during window resize),
//            so it is not that surprising, that not every surfaceDestoyed callback means what we might think it means.
//        We don't need this callback that much anyways, the renderer does not call render callbacks when gl context is invalid, so the OFAndroidWindow.onSurfaceCreated callback should be enought for us to make things work.

    @Override
	public void surfaceDestroyed(SurfaceHolder holder) {
    	super.surfaceDestroyed(holder);
		OFAndroid.onSurfaceDestroyed();
	}

    
    boolean isSetup(){
    	return mRenderer.isSetup();
    }
    
    private OFAndroidWindow mRenderer;
}

class OFAndroidWindow implements GLSurfaceView.Renderer {
	
	public OFAndroidWindow(int w, int h){ 
		this.w = w;
		this.h = h;
	}
	
	@Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		Log.i("OF","onSurfaceCreated");
		// notify that old surface was destroyed
		if(this.has_surface) {
			OFAndroid.onSurfaceDestroyed();
			this.has_surface = false;
		}
		
		// notify that new surface was created
		this.has_surface = true;
		OFAndroid.onSurfaceCreated();
		Activity activity = OFAndroidLifeCycle.getActivity();
		if(OFActivity.class.isInstance(activity))
			((OFActivity)activity).onGLSurfaceCreated();
		
		return;
    }
	
	@Override
    public void onSurfaceChanged(GL10 gl, int w, int h) {
		this.w = w;
		this.h = h;
    	if(!setup && OFAndroid.unpackingDone){
        	setup();
    	}
    	OFGestureListener.swipe_Min_Distance = (int)(Math.max(w, h)*.04);
    	OFGestureListener.swipe_Max_Distance = (int)(Math.max(w, h)*.6);
    	OFAndroid.resize(w, h);
    }
    
    private void setup(){
    	OFAndroid.setup(w,h);
    	setup = true;
    	android.os.Process.setThreadPriority(8);
    	OFGestureListener.swipe_Min_Distance = (int)(Math.max(w, h)*.04);
    	OFGestureListener.swipe_Max_Distance = (int)(Math.max(w, h)*.6);
    	Activity activity = OFAndroidLifeCycle.getActivity();
		if(OFActivity.class.isInstance(activity))
			((OFActivity)activity).onGLSurfaceCreated();
    }
    
    public static void exit() {
    	setup = false;
    }
    
    @Override
    public void onDrawFrame(GL10 gl) {
    	if(setup && OFAndroid.unpackingDone){
    		OFAndroid.render();
    	}else if(!setup && OFAndroid.unpackingDone){
    		setup();
    	}else{
    		gl.glClear(GL10.GL_COLOR_BUFFER_BIT);
    		gl.glClearColor(.5f, .5f, .5f, 1.f);	
    	}
    }
    
    public boolean isSetup(){
    	return setup;
    }

    private static boolean setup;
    private int w,h;
    private boolean has_surface = false;
}
