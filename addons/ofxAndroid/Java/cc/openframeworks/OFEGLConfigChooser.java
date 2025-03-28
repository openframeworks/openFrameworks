package cc.openframeworks;

import static android.opengl.EGL14.EGL_OPENGL_ES2_BIT;
import static android.opengl.EGL15.EGL_OPENGL_ES3_BIT;

import static javax.microedition.khronos.egl.EGL10.EGL_RENDERABLE_TYPE;

import android.opengl.GLSurfaceView;
import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLDisplay;
import android.util.Log;
import javax.microedition.khronos.egl.EGLContext;

import android.opengl.EGL14;
import android.os.Build;

import androidx.annotation.Keep;
import androidx.annotation.RequiresApi;

@Keep
class ContextFactory implements GLSurfaceView.EGLContextFactory {

    private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;

    public static int OPENGLES_VERSION = 2;

    public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig) {
        Log.w("OFContextFactory", "creating OpenGL ES context");
        //checkEglError("Before eglCreateContext", egl);
        int[] attrib_list = { EGL_CONTEXT_CLIENT_VERSION, OPENGLES_VERSION, EGL10.EGL_NONE };

        if(eglConfig == null) {
            Log.e("OFContextFactory", "eglConfig is null");
            return null;
        }
        try {
            EGLContext context = egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);
            return context;
        } catch(Exception ex) {
            Log.e("OFContextFactory", "egl.eglCreateContext error:" + ex.getMessage());
        }
        return null;
    }


    public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) {
        egl.eglDestroyContext(display, context);
    }


    private static void checkEglError(String prompt, EGL10 egl) {
        int error;
        while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS) {
            Log.e("OFContextFactory", String.format("%s: EGL error: 0x%x", prompt, error));
        }
    }
}


@Keep
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
        if(mSampleSize > 1) MSAA = true;
    }

    public static void setGLESVersion(int version) {
        GLES_VERSION = version;

        if (version == 1) {
            EGL_OPENGL_ES_BIT = 1;
            ContextFactory.OPENGLES_VERSION = 1;
        } else if (version == 2) {
            EGL_OPENGL_ES_BIT = 4;
            ContextFactory.OPENGLES_VERSION = 2;
        }
//        else if(version==3) {
//            EGL_OPENGL_ES_BIT=EGL_OPENGL_ES3_BIT;
//            ContextFactory.OPENGLES_VERSION = 3;
//    }
    }

    public void setWideGamutRGB(){
        mRedSize = 10;
        mGreenSize = 10;
        mBlueSize = 10;
        mAlphaSize = 2;
        mDepthSize = 8;
        mStencilSize = 1;
        mWideGamut = true;
    }

    public void setRGB(){
        mWideGamut = false;
    }

    public static int getGLESVersion(){
        return GLES_VERSION;
    }

    public int getSampleSize(){
        return mSampleSize;
    }

    public boolean IsAntiAliasing(){
        return mSampleSize > 1;
    }

    public boolean IsWideGamut(){ // es3 / vulkan
        return mWideGamut;
    }



    /* This EGL config specification is used to specify 1.x rendering.
     * We use a minimum size of 4 bits for red/green/blue, but will
     * perform actual matching in chooseConfig() below.
     */
    private static boolean DEBUG = true;
    private static boolean MSAA = false;
    private static int EGL_OPENGL_ES_BIT = 1;
    private static int GLES_VERSION = 1;


    public static final int EGL_GL_COLORSPACE_KHR = 0x309D;
    public static final int EGL_COLOR_COMPONENT_TYPE_EXT = 0x3339;
    public static final int EGL_COLOR_COMPONENT_TYPE_FLOAT_EXT = 0x333B;
    public static final int EGL_GL_COLORSPACE_BT2020_PQ_EXT = 0x3340;
    public static final int EGL_COVERAGE_BUFFERS_NV = 0x30E0;
    public static final int EGL_COVERAGE_SAMPLES_NV = 0x30E1;
    public static final int EGL_GL_COLORSPACE_DISPLAY_P3_PASSTHROUGH_EXT = 0x3490;

    public static final int EGL_RENDERBUFFER_SAMPLES_ANGLE = 0x8CAB;
    public static final int EGL_MAX_SAMPLES_ANGLE = 0x8D57;

//    private static int[] s_configAttribsMSAA_P3 =
//            {
//                    EGL10.EGL_RED_SIZE, 10,
//                    EGL10.EGL_GREEN_SIZE, 10,
//                    EGL10.EGL_BLUE_SIZE, 10,
//                    EGL10.EGL_DEPTH_SIZE, 8,
//                    EGL10.EGL_ALPHA_SIZE, 2,
//                    EGL_COLOR_COMPONENT_TYPE_EXT,
//                    // Requires that setEGLContextClientVersion(2) is called on the view.
//                    EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT /* EGL_OPENGL_ES2_BIT */,
//                    EGL10.EGL_SAMPLE_BUFFERS, 1 /* true */,
//                    EGL10.EGL_SAMPLES, 4,
//                    EGL_GL_COLORSPACE_KHR, EGL_GL_COLORSPACE_DISPLAY_P3_PASSTHROUGH_EXT,
//                    EGL10.EGL_NONE
//            };

    private static final int[] s_configAttribsMSAA16Angle =
            {
                    EGL10.EGL_RED_SIZE, 8,
                    EGL10.EGL_GREEN_SIZE, 8,
                    EGL10.EGL_BLUE_SIZE, 8,
                    EGL10.EGL_ALPHA_SIZE, 8,
                    EGL10.EGL_DEPTH_SIZE, 16,
                    // Requires that setEGLContextClientVersion(2) is called on the view.
                    EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT /* EGL_OPENGL_ES2_BIT */,
                    EGL_MAX_SAMPLES_ANGLE, 4,
                    EGL10.EGL_NONE
            };

    private static final int[] s_configAttribsMSAA16 =
            {
                    EGL10.EGL_RED_SIZE, 8,
                    EGL10.EGL_GREEN_SIZE, 8,
                    EGL10.EGL_BLUE_SIZE, 8,
                    EGL10.EGL_ALPHA_SIZE, 8,
                    EGL10.EGL_DEPTH_SIZE, 16,
                    // Requires that setEGLContextClientVersion(2) is called on the view.
                    EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT /* EGL_OPENGL_ES2_BIT */,
                    EGL10.EGL_SAMPLE_BUFFERS, 1 /* true */,
                    EGL10.EGL_SAMPLES, 16,
                    EGL10.EGL_NONE
            };


    private static final int[] s_configAttribsMSAA8 =
            {
                    EGL10.EGL_RED_SIZE, 8,
                    EGL10.EGL_GREEN_SIZE, 8,
                    EGL10.EGL_BLUE_SIZE, 8,
                    EGL10.EGL_ALPHA_SIZE, 8,
                    EGL10.EGL_DEPTH_SIZE, 24,
                    // Requires that setEGLContextClientVersion(2) is called on the view.
                    EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT /* EGL_OPENGL_ES2_BIT */,
                    EGL10.EGL_SAMPLE_BUFFERS, 1 /* true */,
                    EGL10.EGL_SAMPLES, 4,
                    EGL10.EGL_NONE
            };

    private static final int[] s_configAttribsMSAA =
            {
                    EGL10.EGL_RED_SIZE, 8,
                    EGL10.EGL_GREEN_SIZE, 8,
                    EGL10.EGL_BLUE_SIZE, 8,
                    EGL10.EGL_ALPHA_SIZE, 8,
                    EGL10.EGL_DEPTH_SIZE, 16,
                    // Requires that setEGLContextClientVersion(2) is called on the view.
                    EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT /* EGL_OPENGL_ES2_BIT */,
                    EGL10.EGL_SAMPLE_BUFFERS, 1 /* true */,
                    EGL10.EGL_SAMPLES, 4,
                    EGL10.EGL_NONE
            };

    private static final int[] s_configAttribsMSAA4 =
            {
                    EGL10.EGL_RED_SIZE, 8,
                    EGL10.EGL_GREEN_SIZE, 8,
                    EGL10.EGL_BLUE_SIZE, 8,
                    EGL10.EGL_ALPHA_SIZE, 8,
                    EGL10.EGL_DEPTH_SIZE, 16,
                    // Requires that setEGLContextClientVersion(2) is called on the view.
                    EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT /* EGL_OPENGL_ES2_BIT */,
                    EGL10.EGL_SAMPLE_BUFFERS, 1 /* true */,
                    EGL10.EGL_SAMPLES, 4,
                    EGL10.EGL_NONE
            };

    private static final int[] s_configAttribsMSAA2 =
            {
                    EGL10.EGL_RED_SIZE, 8,
                    EGL10.EGL_GREEN_SIZE, 8,
                    EGL10.EGL_BLUE_SIZE, 8,
                    EGL10.EGL_ALPHA_SIZE, 8,
                    EGL10.EGL_DEPTH_SIZE, 16,
                    // Requires that setEGLContextClientVersion(2) is called on the view.
                    EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT /* EGL_OPENGL_ES2_BIT */,
                    EGL10.EGL_SAMPLE_BUFFERS, 1 /* true */,
                    EGL10.EGL_SAMPLES, 2,
                    EGL10.EGL_NONE
            };

    private static final int[] s_configAttribsNoSample =
    {
            EGL10.EGL_RED_SIZE, 8,
            EGL10.EGL_GREEN_SIZE, 8,
            EGL10.EGL_BLUE_SIZE, 8,
            EGL10.EGL_ALPHA_SIZE, 8,
            EGL10.EGL_DEPTH_SIZE, 24,
            // Requires that setEGLContextClientVersion(2) is called on the view.
            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT /* EGL_OPENGL_ES2_BIT */,
            EGL10.EGL_NONE
    };

    private static final int[] s_configAttribsMSAAFallBack = {
            EGL10.EGL_RED_SIZE, 8,
            EGL10.EGL_GREEN_SIZE, 8,
            EGL10.EGL_BLUE_SIZE, 8,
            EGL10.EGL_ALPHA_SIZE, 8,
            EGL10.EGL_DEPTH_SIZE, 24,
            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT /* EGL_OPENGL_ES2_BIT */,
            EGL_COVERAGE_BUFFERS_NV, 1 /* true */,
            EGL_COVERAGE_SAMPLES_NV, 2,  // always 5 in practice on tegra 2
            EGL10.EGL_NONE
    };

    private static final int[] s_configAttribsMSAAFallBack2 = {
            EGL10.EGL_RED_SIZE, 8,
            EGL10.EGL_GREEN_SIZE, 8,
            EGL10.EGL_BLUE_SIZE, 8,
            EGL10.EGL_ALPHA_SIZE, 8,
            EGL10.EGL_DEPTH_SIZE, 16,
            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT /* EGL_OPENGL_ES2_BIT */,
            EGL_COVERAGE_BUFFERS_NV, 1 /* true */,
            EGL_COVERAGE_SAMPLES_NV, 5,  // always 5 in practice on tegra 2
            EGL10.EGL_NONE
    };

    private static final int[] s_configAttribsDefault8 = {
            EGL10.EGL_RED_SIZE, 8,
            EGL10.EGL_GREEN_SIZE, 8,
            EGL10.EGL_BLUE_SIZE, 8,
            EGL10.EGL_DEPTH_SIZE, 24,
            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT /* EGL_OPENGL_ES2_BIT */,
            EGL10.EGL_NONE
    };

    private static final int[] s_configAttribsDefault8Depth16 = {
            EGL10.EGL_RED_SIZE, 8,
            EGL10.EGL_GREEN_SIZE, 8,
            EGL10.EGL_BLUE_SIZE, 8,
            EGL10.EGL_DEPTH_SIZE, 16,
            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT /* EGL_OPENGL_ES2_BIT */,
            EGL10.EGL_NONE
    };

    private static final int[] s_configAttribsDefault8DepthNone = {
            EGL10.EGL_RED_SIZE, 8,
            EGL10.EGL_GREEN_SIZE, 8,
            EGL10.EGL_BLUE_SIZE, 8,
            EGL10.EGL_DEPTH_SIZE, 0,
            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT /* EGL_OPENGL_ES2_BIT */,
            EGL10.EGL_NONE
    };

    private static final int[] s_configAttribsDefault = {
            EGL10.EGL_RED_SIZE, 5,
            EGL10.EGL_GREEN_SIZE, 6,
            EGL10.EGL_BLUE_SIZE, 5,
            EGL10.EGL_DEPTH_SIZE, 0,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT /* EGL_OPENGL_ES2_BIT */,
            EGL10.EGL_NONE
    };

    private static final int[] s_configAttribsDefaultES = {
            EGL10.EGL_RED_SIZE, 5,
            EGL10.EGL_GREEN_SIZE, 6,
            EGL10.EGL_BLUE_SIZE, 5,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT /* EGL_OPENGL_ES2_BIT */,
            EGL10.EGL_NONE
    };


    public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {

        try {
            Log.i("OF", String.format("eglChooseConfig: DEVICE:" + android.os.Build.DEVICE + " MODEL:"+ Build.MODEL));
            if (android.os.Build.DEVICE.contains("cactus")) {
                OFAndroid.samples = 1;
            }
        } catch (Exception ex) {

        }

        /* Get the number of minimally matching EGL configurations
         */
        int[] num_config = new int[1];
        EGLConfig[] configs = null;
        num_config[0] = 0;

//        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O && mWideGamut == true) {
//            if (!egl.eglChooseConfig(display, s_configAttribsMSAA, null, 0,
//                    num_config)) {
//                Log.w("OF", String.format("s_configAttribsMSAA_P3 MSAA with P3 failed"));
//            }
//        }

        boolean workingConfig = false;
        boolean tryMSAA = OFAndroid.maxSamples > 1;
        int numConfigs = num_config[0];

        if (!workingConfig) {
            setRGB();
            if(tryMSAA && OFAndroid.maxSamples >= 8) {
                Log.i("OF", String.format("eglChooseConfig MSAAx8"));
                if (tryMSAA && !egl.eglChooseConfig(display, s_configAttribsMSAA8, null, 0,
                        num_config)) {
                    Log.w("OF", String.format("eglChooseConfig MSAAx8 failed"));
                }
                numConfigs = num_config[0];
                if (tryMSAA && OFAndroid.maxSamples >= 8 && numConfigs > 0) {
                    configs = new EGLConfig[numConfigs];
                    try {
                        if (egl.eglChooseConfig(display, s_configAttribsMSAA8, configs, numConfigs, num_config)) {
                            numConfigs = num_config[0];
                            Log.v("OF", String.format("eglChooseConfig MSAA8 Success"));
                            EGLConfig testConfig = chooseConfig(egl, display, configs, true);
                            if(numConfigs > 0 && configs != null && testConfig != null) {
                                workingConfig = true;
                            } else {
                                workingConfig = false;
                                numConfigs = 0;
                            }
                        } else {
                            workingConfig = false;
                            numConfigs = 0;
                        }

                    } catch (Exception ex) {
                        numConfigs = 0;
                        workingConfig = false;
                        Log.e("OF", String.format("eglChooseConfig MSAAx8 failed with EXCEPTION:") + ex.getMessage());
                    }
                }
            }
            if (numConfigs <= 0 || !workingConfig) {
                setRGB();
                configs = new EGLConfig[numConfigs];
                try {
                    if (tryMSAA && OFAndroid.maxSamples >=4 && !egl.eglChooseConfig(display, s_configAttribsMSAA4, null, 0,
                            num_config)) {
                        Log.w("OF", String.format("eglChooseConfig MSAA failed"));
                    }
                } catch (Exception ex) {
                    numConfigs = 0;
                    workingConfig = false;
                    Log.e("OF", String.format("eglChooseConfig MSAAx4 failed with EXCEPTION:") + ex.getMessage());
                }
                numConfigs = num_config[0];
                if(tryMSAA && OFAndroid.maxSamples >=4 && numConfigs > 0) {
                    configs = new EGLConfig[numConfigs];
                    try {
                        if (tryMSAA && OFAndroid.maxSamples >=4 && !egl.eglChooseConfig(display, s_configAttribsMSAA4, null, 0,
                                num_config)) {
                            Log.w("OF", String.format("eglChooseConfig MSAA failed"));
                        }
                    } catch (Exception ex) {
                        numConfigs = 0;
                        workingConfig = false;
                        Log.e("OF", String.format("eglChooseConfig MSAAx4 failed with EXCEPTION:") + ex.getMessage());
                    }
                    numConfigs = num_config[0];
                    configs = new EGLConfig[numConfigs];
                    try {
                        if(egl.eglChooseConfig(display, s_configAttribsMSAA4, configs, numConfigs, num_config)){
                            numConfigs = num_config[0];
                            Log.v("OF", String.format("eglChooseConfig MSAA Success"));
                            EGLConfig testConfig = chooseConfig(egl, display, configs, true);
                            if(numConfigs > 0 && configs != null && testConfig != null) {
                                workingConfig = true;
                            } else {
                                workingConfig = false;
                                numConfigs = 0;
                            }
                        } else {
                            workingConfig = false;
                            numConfigs = 0;
                        }
                    } catch (Exception ex) {
                        numConfigs = 0;
                        workingConfig = false;
                        Log.e("OF", String.format("eglChooseConfig MSAA failed with EXCEPTION:") + ex.getMessage());
                    }
                }
                if (numConfigs <= 0 || !workingConfig) {
                    configs = new EGLConfig[numConfigs];

                    numConfigs = num_config[0];
                    if(tryMSAA && numConfigs > 0) {
                        configs = new EGLConfig[numConfigs];
                        try {

                            if(egl.eglChooseConfig(display, s_configAttribsMSAAFallBack, configs, numConfigs, num_config)){
                                numConfigs = num_config[0];
                                Log.v("OF", String.format("eglChooseConfig MSAA Tegra Success"));
                                EGLConfig testConfig = chooseConfig(egl, display, configs, true);
                                if(numConfigs > 0 && configs != null && testConfig != null) {
                                    workingConfig = true;
                                } else {
                                    workingConfig = false;
                                    numConfigs = 0;
                                }
                            } else {
                                workingConfig = false;
                                numConfigs = 0;
                            }
                        } catch (Exception ex) {
                            numConfigs = 0;
                            workingConfig = false;
                            Log.e("OF", String.format("eglChooseConfig MSAA Fallback failed with EXCEPTION:") + ex.getMessage());
                        }
                    }
                    if (numConfigs <= 0 && !workingConfig) {
                        if(tryMSAA) Log.i("OF", String.format("try eglChooseConfig MSAAx4 TEGRA2"));
                        if (tryMSAA && !egl.eglChooseConfig(display, s_configAttribsMSAAFallBack2, null, 0,
                                num_config)) {
                            Log.w("OF", String.format("eglChooseConfig MSAA Fallback failed"));
                        }
                        numConfigs = num_config[0];
                        if(tryMSAA && numConfigs > 0) {
                            configs = new EGLConfig[numConfigs];
                            try {
                                if(egl.eglChooseConfig(display, s_configAttribsMSAAFallBack2, configs, numConfigs, num_config)){
                                    numConfigs = num_config[0];
                                    EGLConfig testConfig = chooseConfig(egl, display, configs, true);
                                    if(numConfigs > 0 && configs != null && testConfig != null) {
                                        workingConfig = true;
                                    } else {
                                        workingConfig = false;
                                        numConfigs = 0;
                                    }
                                } else {
                                    workingConfig = false;
                                    numConfigs = 0;
                                }
                            } catch (Exception ex) {
                                numConfigs = 0;
                                workingConfig = false;
                                Log.e("OF", String.format("eglChooseConfig MSAA Fallback2 failed with EXCEPTION:") + ex.getMessage());
                            }
                        }

                        if (numConfigs <= 0 || !workingConfig) {
                            mSampleSize = 0;
                            Log.i("OF", String.format("eglChooseConfig Default8"));
                            if (!egl.eglChooseConfig(display, s_configAttribsDefault8, null, 0,
                                    num_config)) {
                                Log.w("OF", String.format("s_configAttribsDefault8 Fallback failed"));
                            }
                            numConfigs = num_config[0];
                            if(numConfigs > 0) {
                                int numberConfigurations = numConfigs;
                                for(int i = 0; i<7; i++) {
                                    switch (i) {
                                        case 0:
                                            mSampleSize = 0;
                                            mAlphaSize = 8;
                                            break;
                                        case 1:
                                            mSampleSize = 0;
                                            mAlphaSize = 8;
                                            mStencilSize = 0;
                                            mDepthSize = 24;
                                            break;
                                        case 2:
                                            mBlueSize = 8;
                                            mRedSize = 8;
                                            mGreenSize = 8;
                                            mStencilSize = 0;
                                            mDepthSize = 16;
                                            break;
                                        case 3:
                                            mBlueSize = 8;
                                            mRedSize = 8;
                                            mGreenSize = 8;
                                            mStencilSize = 8;
                                            mDepthSize = 24;
                                            mAlphaSize = 8;
                                            mSampleSize = OFAndroid.samples;
                                            break;
                                        case 4:
                                            mBlueSize = 8;
                                            mRedSize = 8;
                                            mGreenSize = 8;
                                            mStencilSize = 8;
                                            mDepthSize = 16;
                                            mAlphaSize = 8;
                                            mSampleSize = OFAndroid.samples;
                                            break;
                                        case 5:
                                            mBlueSize = 8;
                                            mRedSize = 8;
                                            mGreenSize = 8;
                                            mStencilSize = 8;
                                            mDepthSize = 8;
                                            mSampleSize = 0;
                                            mAlphaSize = 0;
                                            break;
                                        case 6:
                                            mBlueSize = 8;
                                            mRedSize = 8;
                                            mGreenSize = 8;
                                            mStencilSize = 0;
                                            mDepthSize = 0;
                                            mSampleSize = 0;
                                            mAlphaSize = 0;
                                            break;
                                        case 7:
                                            mBlueSize = 5;
                                            mRedSize = 5;
                                            mGreenSize = 6;
                                            mStencilSize = 8;
                                            mDepthSize = 16;
                                            mAlphaSize = 0;
                                            mSampleSize = OFAndroid.samples;
                                            break;

                                    }
                                    configs = new EGLConfig[numberConfigurations];
                                    try {

                                        if (egl.eglChooseConfig(display, s_configAttribsDefault8, configs, numberConfigurations, num_config)) {
                                            numConfigs = num_config[0];
                                            EGLConfig testConfig = chooseConfig(egl, display, configs, true);
                                            if(numConfigs > 0 && configs != null && testConfig != null) {
                                                workingConfig = true;
                                                numConfigs = numberConfigurations;
                                                Log.i("OF", String.format("eglChooseConfig Default8 Found on iteration:" + i));
                                                break;
                                            } else {
                                                workingConfig = false;
                                                numConfigs = 0;
                                                Log.w("OF", String.format("eglChooseConfig Default8 failed to find iteration:" + i));
                                            }
                                        } else {
                                            workingConfig = false;
                                            numConfigs = 0;
                                            Log.w("OF", String.format("eglChooseConfig Default8 failed to find iteration:" + i));
                                        }
                                    } catch (Exception ex) {
                                        numConfigs = 0;
                                        workingConfig = false;
                                        Log.e("OF", String.format("eglChooseConfig Default8 failed with EXCEPTION:") + ex.getMessage() +" iteration:" + i);
                                    }
                                }
                            }
                            if (numConfigs <= 0  || !workingConfig) {
                                numConfigs = num_config[0];
                                if (numConfigs > 0) {
                                    configs = new EGLConfig[numConfigs];
                                    try {
                                        mDepthSize = 16;
                                        mAlphaSize = 0;
                                        if (egl.eglChooseConfig(display, s_configAttribsDefault8Depth16, configs, numConfigs, num_config)) {
                                            numConfigs = num_config[0];
                                            EGLConfig testConfig = chooseConfig(egl, display, configs, true);
                                            if(numConfigs > 0 && configs != null && testConfig != null) {
                                                workingConfig = true;
                                            } else {
                                                workingConfig = false;
                                                numConfigs = 0;
                                            }
                                        } else {
                                            workingConfig = false;
                                            numConfigs = 0;
                                        }
                                    } catch (Exception ex) {
                                        numConfigs = 0;
                                        workingConfig = false;
                                        Log.e("OF", String.format("eglChooseConfig Default8Depth16 failed with EXCEPTION:") + ex.getMessage());
                                    }
                                }
                            }
                            if (numConfigs <= 0 || !workingConfig) {
                                numConfigs = num_config[0];
                                if (numConfigs > 0) {
                                    configs = new EGLConfig[numConfigs];
                                    try {
                                        mDepthSize = 0;
                                        if (egl.eglChooseConfig(display, s_configAttribsDefault8DepthNone, configs, numConfigs, num_config)) {
                                            numConfigs = num_config[0];

                                            EGLConfig testConfig = chooseConfig(egl, display, configs, true);
                                            if(numConfigs > 0 && configs != null && testConfig != null) {
                                                workingConfig = true;
                                            } else {
                                                workingConfig = false;
                                                numConfigs = 0;
                                            }
                                        } else {
                                            workingConfig = false;
                                            numConfigs = 0;
                                        }
                                    } catch (Exception ex) {
                                        numConfigs = 0;
                                        workingConfig = false;
                                        Log.e("OF", String.format("eglChooseConfig Default8DepthNone failed with EXCEPTION:") + ex.getMessage());
                                    }
                                }
                            }
                            if (numConfigs <= 0 || !workingConfig) {
                                Log.i("OF", String.format("eglChooseConfig Default"));
                                try {
                                    mBlueSize = 5;
                                    mRedSize = 5;
                                    mGreenSize = 6;
                                    mStencilSize = 1;
                                    mDepthSize = 24;
                                    if (!egl.eglChooseConfig(display, s_configAttribsDefault, null, 0,
                                            num_config)) {
                                        Log.w("OF", String.format("eglChooseConfig Default failed"));
                                    }
                                } catch (Exception ex) {
                                    numConfigs = 0;
                                    workingConfig = false;
                                    Log.e("OF", String.format("eglChooseConfig s_configAttribsDefault failed with EXCEPTION:") + ex.getMessage());
                                }
                                numConfigs = num_config[0];
                                if(numConfigs > 0) {
                                    configs = new EGLConfig[numConfigs];
                                    try {
                                        mBlueSize = 5;
                                        mRedSize = 5;
                                        mGreenSize = 6;
                                        mStencilSize = 1;
                                        mDepthSize = 0;
                                        if(egl.eglChooseConfig(display, s_configAttribsDefault, configs, numConfigs, num_config)){
                                            numConfigs = num_config[0];
                                            EGLConfig testConfig = chooseConfig(egl, display, configs, true);
                                            if(numConfigs > 0 && configs != null && testConfig != null) {
                                                workingConfig = true;
                                            } else {
                                                workingConfig = false;
                                                numConfigs = 0;
                                            }
                                        } else {
                                            workingConfig = false;
                                            numConfigs = 0;
                                        }
                                    } catch (Exception ex) {
                                        numConfigs = 0;
                                        workingConfig = false;
                                        Log.e("OF", String.format("eglChooseConfig s_configAttribsDefault failed with EXCEPTION:") + ex.getMessage());
                                    }
                                }
                                if (numConfigs <= 0 || !workingConfig) {
                                    Log.i("OF", String.format("eglChooseConfig DefaultES"));
                                    try {
                                        if (!egl.eglChooseConfig(display, s_configAttribsDefaultES, null, 0,
                                                num_config)) {
                                            Log.w("OF", String.format("s_configAttribsDefaultES Default failed"));
                                        }
                                    } catch (Exception ex) {
                                        Log.e("OF", String.format("eglChooseConfig s_configAttribsDefaultES failed with EXCEPTION:") + ex.getMessage());
                                    }
                                    numConfigs = num_config[0];
                                    if (numConfigs <= 0) {
                                        Log.e("OF", String.format("eglChooseConfig No configs match configSpec"));
                                        throw new IllegalArgumentException("No configs match configSpec");
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        /* Allocate then read the array of minimally matching EGL configs
         */

//        if(configs == null) {
//            Log.w("OF", String.format("configs=null eglChooseConfig Default NO MSAA"));
//            try {
//                if (!egl.eglChooseConfig(display, s_configAttribsNoSample, null, 0,
//                        num_config)) {
//                    Log.w("OF", String.format("eglChooseConfig Default failed"));
//                }
//            } catch (Exception ex) {
//                Log.e("OF", String.format("eglChooseConfig s_configAttribsNoSample failed with EXCEPTION:") + ex.getMessage());
//            }
//            numConfigs = num_config[0];
//            if(numConfigs > 0) {
//                configs = new EGLConfig[numConfigs];
//                try {
//                    if(egl.eglChooseConfig(display, s_configAttribsNoSample, configs, numConfigs, num_config)){
//                        numConfigs = num_config[0];
//                        if(numConfigs > 0) workingConfig = true;
//                    }
//                } catch (Exception ex) {
//                    Log.e("OF", String.format("eglChooseConfig s_configAttribsNoSample failed with EXCEPTION:") + ex.getMessage());
//                }
//            }
//            if(configs != null) {
//                Log.i("OF", String.format("eglChooseConfig Default NO MSAA - worked"));
//            }
//        }


        if (DEBUG && configs != null) {
            printConfigs(egl, display, configs);
        }



        /* Now return the "best" one
         */

        EGLConfig finalConfig = null;
        try {
            finalConfig = chooseConfig(egl, display, configs, false);
        } catch(Exception exception)
        {
            Log.e("OF", String.format("finalConfig choose exception: ") + exception.getMessage());

        }

        mSampleSize = mFinalSampleSize;

        if(mFinalSampleSize != OFAndroid.samples) {
            Log.w("OF", String.format("MSAA FinalConfig Final Samples: %d - target:%d - saving max for device", mFinalSampleSize, mSampleSize));
            OFAndroid.samples = mFinalSampleSize;
            OFAndroid.savePreferences();
        }

        if (DEBUG && finalConfig != null) {
            printConfig(egl, display, finalConfig);
        }
        return finalConfig;

    }

    public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display,
                                  EGLConfig[] configs, boolean validate) {

        EGLConfig foundConfig = null;
        EGLConfig lastConfig = null;

        int foundMSAA = 0;
        int foundDepth = 0;
        int foundStencil = 0;

        String targetOutput = String.format("Config Row: r%d g:%d b%d a:%d d:%d s%d aa:%d", mRedSize, mGreenSize, mBlueSize, mAlphaSize, mDepthSize, mStencilSize, mSampleSize);


        for(EGLConfig config : configs) {

            //Log.i("OF", String.format("Loop Config Row:", config.toString()));
            // We want an *exact* match for red/green/blue/alpha
            int r = findConfigAttrib(egl, display, config,
                    EGL10.EGL_RED_SIZE, 0);
            int g = findConfigAttrib(egl, display, config,
                    EGL10.EGL_GREEN_SIZE, 0);
            int b = findConfigAttrib(egl, display, config,
                    EGL10.EGL_BLUE_SIZE, 0);
            int a = findConfigAttrib(egl, display, config,
                    EGL10.EGL_ALPHA_SIZE, 0);

            int depth = findConfigAttrib(egl, display, config,
                    EGL10.EGL_DEPTH_SIZE, 0);

            int stencil = findConfigAttrib(egl, display, config,
                    EGL10.EGL_STENCIL_SIZE, 0);

            int sampleBuffers = 0;
            int samples = 0;
            int configID = findConfigAttrib(egl, display, config,
                    EGL10.EGL_CONFIG_ID, 0);
            if(mSampleSize > 1) {
                 sampleBuffers = findConfigAttrib(egl, display, config,
                        EGL10.EGL_SAMPLE_BUFFERS, 0);
                 samples = findConfigAttrib(egl, display, config,
                        EGL10.EGL_SAMPLES, 0);

            }

            String output = String.format("Config Row: r%d g:%d b%d a:%d d:%d s%d aa:%d configID:%d", r, g, b, a, depth,stencil, samples, configID);


            // We need at least mDepthSize and mStencilSize bits
            if (depth < mDepthSize || stencil < mStencilSize) {
                if(!validate) Log.i("OF", String.format("Skipping Config Row: %s - target:depth:%d stencil:%d", output, mDepthSize, mStencilSize));
                continue;
            }
            else if (samples < mSampleSize) {
                if(!validate) Log.i("OF", String.format("Skipping Config Row: %s - target:samples:%d", output, mSampleSize));
                continue;
            }
            else if (samples > OFAndroid.maxSamples) {
                if(!validate) Log.i("OF", String.format("Skipping Config Row: %s - target:maxSamples:%d", output, OFAndroid.maxSamples));
                continue;
            }
            else if (r < mRedSize || g < mGreenSize ||  b < mBlueSize ) {
                if(!validate) Log.i("OF", String.format("Skipping Config Row: %s - target:r:%d g%d b%d", output, mRedSize, mGreenSize, mBlueSize));
                continue;
            }
            else if (a < mAlphaSize ) {
                if(!validate) Log.i("OF", String.format("Skipping Config Row: %s - target:alpha%d", output, mAlphaSize));
                continue;
            }
            else {
                if(!validate) Log.i("OF", String.format("Loop Config Row:", config.toString()));
            }

//            int gamut =  findConfigAttrib(egl, display, config,
//                    EGL_GL_COLORSPACE_KHR, 0);
//
//            if(gamut != 0) {
//                Log.w("OF", String.format("WideGamut: is ", gamut));
//            }
            if(OFAndroid.samples > 1) {
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                    int anglesamples = findConfigAttrib(egl, display, config,
                            EGL_MAX_SAMPLES_ANGLE, 0);
                    // check for ANGLE Lib EGL Samples
                    if (foundConfig == null && r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && depth == mDepthSize && stencil == mStencilSize && anglesamples == mSampleSize) {
                        if (foundConfig == null) {
                            foundConfig = config;
                            foundMSAA = anglesamples;
                            foundDepth = depth;
                            Log.i("OF", "MSAA - ANGLE MSAA - Found and Set:" + output);
                        } else {
                            foundConfig = config;
                            Log.i("OF", "Override Already Found and Set:" + output);
                        }
                    } else {
                        Log.v("OF", "No Angle Samples - Else Config: " + output);
                    }

                }
                int nvsamples = findConfigAttrib(egl, display, config,
                        EGL_COVERAGE_SAMPLES_NV, 0);

                if ((foundConfig == null || foundMSAA != 0 && foundMSAA != mSampleSize && nvsamples >= foundMSAA) && r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && depth == mDepthSize && stencil == mStencilSize && nvsamples == mSampleSize) {

                    foundConfig = config;
                    foundMSAA = nvsamples;
                    foundDepth = depth;
                    if(!validate) Log.i("OF", "MSAA - NV samples Found and Set:nvsamples:" + nvsamples + " output:" + output);

                } else {
                    if(!validate) Log.v("OF", "No MSAA - NV samples - Else Config: " + output);
                }

                if ((foundConfig == null || foundMSAA != 0 && foundMSAA != mSampleSize && nvsamples >= foundMSAA) && r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && depth == mDepthSize && stencil == mStencilSize && nvsamples == 5) {

                    foundConfig = config;
                    foundMSAA = nvsamples;
                    foundDepth = depth;
                    if(!validate) Log.i("OF", "MSAA - NV 5 samples Found and Set:nvsamples:" + nvsamples + " output:" + output);

                } else {
                    if(!validate) Log.v("OF", "No MSAA - NV samples - Else Config: " + output);
                }

                if ((foundConfig == null || foundMSAA != 0 && foundMSAA != mSampleSize && samples >= foundMSAA && samples >= mSampleSize) && r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && depth == mDepthSize && stencil == mStencilSize && samples == mSampleSize) {

                    foundConfig = config;
                    foundMSAA = samples;
                    foundDepth = depth;
                    if(!validate) Log.i("OF", "MSAA Found and Set:" + output);

                } else {
                    if(!validate) Log.v("OF", "NO MSAA - Else Config: " + output);
                }

                if ((foundConfig == null || foundMSAA != 0 && foundMSAA != mSampleSize && samples >= foundMSAA && samples < OFAndroid.maxSamples && mSampleSize >= samples) && r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && depth == mDepthSize && stencil == mStencilSize && samples == mSampleSize / 2) {

                    foundConfig = config;
                    foundMSAA = samples;
                    foundDepth = depth;
                    if(!validate) Log.i("OF", "MSAA/2 Found and Set:" + output);

                } else {
                    if(!validate) Log.v("OF", "NO MSAA/2 Else Config: " + output);
                }

                if((foundConfig == null || foundMSAA != 0 && foundMSAA != mSampleSize && samples >= foundMSAA && samples >= mSampleSize) && r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && depth == mDepthSize && stencil == mStencilSize && samples == mSampleSize / 4) {

                    foundConfig = config;
                    foundMSAA = samples;
                    foundDepth = depth;
                    if(!validate) Log.i("OF", "MSAA/4 Found and Set:" + output);

                } else {
                    if(!validate) Log.v("OF", "NO MSAA/4 Else Config: " + output);
                }


                if (foundConfig == null && r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && depth == mDepthSize && stencil == mStencilSize && mSampleSize <= 1 && samples <= 1) {
                    foundConfig = config;
                    foundMSAA = samples;
                    foundDepth = depth;
                    if(!validate) Log.i("OF", "Found and Set :" + output);

                } else {
                    if(!validate) Log.v("OF", "NO MSAA - other issue - Else Config: " + output);
                }
            } else {
                if ((foundConfig == null || foundMSAA != mSampleSize && samples == mSampleSize && samples <= OFAndroid.maxSamples) && r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && depth == mDepthSize && stencil == mStencilSize && samples == mSampleSize) {

                    foundConfig = config;
                    foundMSAA = samples;
                    foundDepth = depth;
                    if(!validate) Log.i("OF", "Config Found - NO MSAA and All other parameters are Set:" + output);

                } else {
                    if(!validate) Log.v("OF", "Not searching for MSAA - Else Config: " + output);
                }
            }

            if ( foundConfig == null && r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && samples != 0 && mSampleSize >= 1 && samples <= OFAndroid.maxSamples) {

                foundConfig = config;
                foundMSAA = samples;
                if(!validate) Log.i("OF", "MSAA Any Found and Set :Depth Size and Stencil any:" + output);

            } else {
                if(!validate) Log.v("OF", "Else Config: " + output);
            }

            if (foundConfig == null && r == mRedSize && g == mGreenSize && b == mBlueSize && depth == mDepthSize && samples == 1 && mSampleSize == 0) {

                foundConfig = config;
                foundMSAA = samples;
                if(!validate) Log.i("OF", "MSAA Off - Last Resort Found and Set:" + output);

            } else {
                if(!validate) Log.v("OF", "Else Config: " + output);
            }

        }
        if(foundConfig == null && lastConfig != null) {
            if(!validate) Log.v("OF", "Using Last Resort Config" + lastConfig.toString());
            foundConfig = lastConfig;
        } else if(foundConfig != null) {
            if(!validate)
                Log.v("OF", "Using Config: " + foundConfig.toString());
            else
                Log.v("OF", "Validated Config for FinalConfig: " + foundConfig.toString());

            if(foundMSAA == 1)
                mFinalSampleSize = mSampleSize;
            else
                mFinalSampleSize = foundMSAA;
            if(DEBUG) printConfig(egl, display, foundConfig);
        }
        if(validate && foundConfig == null) {
            Log.v("OF", "GL Config not found for: " + targetOutput);
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
        if(configs != null) {
            int numConfigs = configs.length;
            Log.w("OF", String.format("%d configurations", numConfigs));
            for (int i = 0; i < numConfigs; i++) {
                Log.w("OF", String.format("Configuration %d:\n", i));
                printConfig(egl, display, configs[i]);
            }
        } else {
            Log.w("OF", String.format("printConfigs - no configs - %d configurations", 0));
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
//                EGL10.EGL_LEVEL,
//                EGL10.EGL_MAX_PBUFFER_HEIGHT,
//                EGL10.EGL_MAX_PBUFFER_PIXELS,
//                EGL10.EGL_MAX_PBUFFER_WIDTH,
//                EGL10.EGL_NATIVE_RENDERABLE,
                EGL10.EGL_NATIVE_VISUAL_ID,
//                EGL10.EGL_NATIVE_VISUAL_TYPE,
//                0x3030, // EGL10.EGL_PRESERVED_RESOURCES,
                EGL10.EGL_SAMPLES,
                EGL10.EGL_SAMPLE_BUFFERS,
                EGL10.EGL_SURFACE_TYPE,
//                EGL10.EGL_TRANSPARENT_TYPE,
//                EGL10.EGL_TRANSPARENT_RED_VALUE,
//                EGL10.EGL_TRANSPARENT_GREEN_VALUE,
//                EGL10.EGL_TRANSPARENT_BLUE_VALUE,
//                0x3039, // EGL10.EGL_BIND_TO_TEXTURE_RGB,
//                0x303A, // EGL10.EGL_BIND_TO_TEXTURE_RGBA,
//                0x303B, // EGL10.EGL_MIN_SWAP_INTERVAL,
//                0x303C, // EGL10.EGL_MAX_SWAP_INTERVAL,
//                EGL10.EGL_LUMINANCE_SIZE,
//                EGL10.EGL_ALPHA_MASK_SIZE,
//                EGL10.EGL_COLOR_BUFFER_TYPE,
                EGL_RENDERABLE_TYPE,
//                0x3042, // EGL10.EGL_CONFORMANT
//                0x309D, // EGL_GL_COLORSPACE_KHR
//                0x3339, //  EGL_COLOR_COMPONENT_TYPE_EXT
//                0x333B, //EGL_COLOR_COMPONENT_TYPE_FLOAT_EXT
//                0x3340, // EGL_GL_COLORSPACE_BT2020_PQ_EXT
//                0x30E0, //EGL_COVERAGE_BUFFERS_NV
//                0x30E1, //EGL_COVERAGE_SAMPLES_NV
//                0x3490, //EGL_GL_COLORSPACE_DISPLAY_P3_PASSTHROUGH_EXT
//                EGL_COVERAGE_BUFFERS_NV, //
//                EGL_COVERAGE_SAMPLES_NV, // always 5 in practice on tegra 2
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
//                "EGL_LEVEL",
//                "EGL_MAX_PBUFFER_HEIGHT",
//                "EGL_MAX_PBUFFER_PIXELS",
//                "EGL_MAX_PBUFFER_WIDTH",
//                "EGL_NATIVE_RENDERABLE",
                "EGL_NATIVE_VISUAL_ID",
                "EGL_NATIVE_VISUAL_TYPE",
//                "EGL_PRESERVED_RESOURCES",
                "EGL_SAMPLES",
                "EGL_SAMPLE_BUFFERS",
                "EGL_SURFACE_TYPE",
//                "EGL_TRANSPARENT_TYPE",
//                "EGL_TRANSPARENT_RED_VALUE",
//                "EGL_TRANSPARENT_GREEN_VALUE",
//                "EGL_TRANSPARENT_BLUE_VALUE",
//                "EGL_BIND_TO_TEXTURE_RGB",
//                "EGL_BIND_TO_TEXTURE_RGBA",
//                "EGL_MIN_SWAP_INTERVAL",
//                "EGL_MAX_SWAP_INTERVAL",
//                "EGL_LUMINANCE_SIZE",
//                "EGL_ALPHA_MASK_SIZE",
//                "EGL_COLOR_BUFFER_TYPE",
                "EGL_RENDERABLE_TYPE",
                "EGL_CONFORMANT",
//                "EGL_GL_COLORSPACE_KHR",
//                "EGL_COLOR_COMPONENT_TYPE_EXT",
//                "EGL_COLOR_COMPONENT_TYPE_FLOAT_EXT",
//                "EGL_GL_COLORSPACE_BT2020_PQ_EXT",
//                "EGL_COVERAGE_BUFFERS_NV",
//                "EGL_COVERAGE_SAMPLES_NV",
//                "EGL_GL_COLORSPACE_DISPLAY_P3_PASSTHROUGH_EXT",
//                "EGL_COVERAGE_BUFFERS_NV",
//                "EGL_COVERAGE_SAMPLES_NV"
        };
        int[] value = new int[1];
        if(attributes != null) {
            for (int i = 0; i < attributes.length; i++) {
                int attribute = attributes[i];
                String name = names[i];
                if (egl.eglGetConfigAttrib(display, config, attribute, value)) {
                    if (value != null)
                        Log.w("OF", String.format("  %s: %d\n", name, value[0]));
                } else {
                    // Log.w(TAG, String.format("  %s: failed\n", name));
                    while (egl.eglGetError() != EGL10.EGL_SUCCESS);
                }
            }
        } else {
            while (egl.eglGetError() != EGL10.EGL_SUCCESS);
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

    protected int mFinalSampleSize = 1;
    protected boolean mWideGamut;
    private int[] mValue = new int[1];
}

