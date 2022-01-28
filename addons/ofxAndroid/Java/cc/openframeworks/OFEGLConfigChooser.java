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

@Keep
class ContextFactory implements GLSurfaceView.EGLContextFactory {

    private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;

    public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig) {
        Log.w("OFContextFactory", "creating OpenGL ES 2.0 context");
        //checkEglError("Before eglCreateContext", egl);
        int[] attrib_list = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };
        try {
            EGLContext context = egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);
            //checkEglError("After eglCreateContext", egl);

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
        mDepthSize = 8;
        mStencilSize = 1;
        //mSampleSize = 8;
        mWideGamut = true;
    }

    public void setRGB(){
//        mRedSize = 8;
//      //  mGreenSize = 8;
//      //  mBlueSize = 8;
//       // mAlphaSize = 8;
//       // mDepthSize = 16;
//        mStencilSize = 16;
//        //mSampleSize = 4;
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
    private static boolean DEBUG = false;
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

    private static int[] s_configAttribsMSAA_P3 =
            {
                    EGL10.EGL_RED_SIZE, 10,
                    EGL10.EGL_GREEN_SIZE, 10,
                    EGL10.EGL_BLUE_SIZE, 10,
                    EGL10.EGL_DEPTH_SIZE, 8,
                    EGL10.EGL_ALPHA_SIZE, 2,
                    EGL_COLOR_COMPONENT_TYPE_EXT,
                    // Requires that setEGLContextClientVersion(2) is called on the view.
                    EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT /* EGL_OPENGL_ES2_BIT */,
                    EGL10.EGL_SAMPLE_BUFFERS, 1 /* true */,
                    EGL10.EGL_SAMPLES, 4,
                    EGL_GL_COLORSPACE_KHR, EGL_GL_COLORSPACE_DISPLAY_P3_PASSTHROUGH_EXT,
                    EGL10.EGL_NONE
            };

    private static final int[] s_configAttribsMSAA16Angle =
            {
                    EGL10.EGL_RED_SIZE, 8,
                    EGL10.EGL_GREEN_SIZE, 8,
                    EGL10.EGL_BLUE_SIZE, 8,
                    EGL10.EGL_ALPHA_SIZE, 8,
                    EGL10.EGL_DEPTH_SIZE, 16,
                    // Requires that setEGLContextClientVersion(2) is called on the view.
                    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT /* EGL_OPENGL_ES2_BIT */,
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
                    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT /* EGL_OPENGL_ES2_BIT */,
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
                    EGL10.EGL_DEPTH_SIZE, 16,
                    // Requires that setEGLContextClientVersion(2) is called on the view.
                    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT /* EGL_OPENGL_ES2_BIT */,
                    EGL10.EGL_SAMPLE_BUFFERS, 1 /* true */,
                    EGL10.EGL_SAMPLES, 8,
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
                    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT /* EGL_OPENGL_ES2_BIT */,
                    EGL10.EGL_SAMPLE_BUFFERS, 1 /* true */,
                    EGL10.EGL_SAMPLES, 4,
                    EGL10.EGL_NONE
            };

    private static final int[] s_configAttribsMSAAFallBack = {
            EGL10.EGL_RED_SIZE, 8,
            EGL10.EGL_GREEN_SIZE, 8,
            EGL10.EGL_BLUE_SIZE, 8,
            EGL10.EGL_ALPHA_SIZE, 8,
            EGL10.EGL_DEPTH_SIZE, 16,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT /* EGL_OPENGL_ES2_BIT */,
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
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT /* EGL_OPENGL_ES2_BIT */,
            EGL_COVERAGE_BUFFERS_NV, 1 /* true */,
            EGL_COVERAGE_SAMPLES_NV, 5,  // always 5 in practice on tegra 2
            EGL10.EGL_NONE
    };

    private static final int[] s_configAttribsDefault8 = {
            EGL10.EGL_RED_SIZE, 8,
            EGL10.EGL_GREEN_SIZE, 8,
            EGL10.EGL_BLUE_SIZE, 8,
            EGL10.EGL_DEPTH_SIZE, 16,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT /* EGL_OPENGL_ES2_BIT */,
            EGL10.EGL_NONE
    };

    private static final int[] s_configAttribsDefault = {
            EGL10.EGL_RED_SIZE, 5,
            EGL10.EGL_GREEN_SIZE, 6,
            EGL10.EGL_BLUE_SIZE, 5,
            EGL10.EGL_DEPTH_SIZE, 16,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT /* EGL_OPENGL_ES2_BIT */,
            EGL10.EGL_NONE
    };

    private static final int[] s_configAttribsDefaultES = {
            EGL10.EGL_RED_SIZE, 5,
            EGL10.EGL_GREEN_SIZE, 6,
            EGL10.EGL_BLUE_SIZE, 5,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT /* EGL_OPENGL_ES2_BIT */,
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
        boolean tryMSAA = true;
        if(OFAndroid.samples == 1 ||
                OFAndroid.height != 0 && OFAndroid.height <= 720 || // if height < 720 super old hardware
                Build.VERSION.SDK_INT < Build.VERSION_CODES.M // Disable for old API's
           ) {
            tryMSAA = false;
            OFAndroid.samples = 1;
            Log.i("OF", String.format("eglChooseConfig MSAA Disabled: height:") + OFAndroid.height);

        } else {
            Log.i("OF", String.format("eglChooseConfig MSAA Enabled: height:") + OFAndroid.height);
        }
        int numConfigs = num_config[0];
//        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
//            if (!egl.eglChooseConfig(display, s_configAttribsMSAA16Angle, null, 0,
//                    num_config)) {
//                Log.w("OF", String.format("s_configAttribsMSAA16Angle MSAA with Angle Libs failed"));
//            }  else {
//                numConfigs = num_config[0];
//                if(numConfigs > 0) {
//                    configs = new EGLConfig[numConfigs];
//                    egl.eglChooseConfig(display, s_configAttribsMSAA16, configs, numConfigs, num_config);
//
//                } else {
//                    Log.w("OF", String.format("s_configAttribsMSAA16Angle MSAA with Angle Libs failed"));
//                }
//            }
//        }

        if (numConfigs <= 0  && !workingConfig) {
            setRGB();
            if(tryMSAA) Log.i("OF", String.format("eglChooseConfig MSAAx8"));
            if (tryMSAA && !egl.eglChooseConfig(display, s_configAttribsMSAA8, null, 0,
                    num_config)) {
                Log.w("OF", String.format("eglChooseConfig MSAAx8 failed"));
            }
            numConfigs = num_config[0];
            if(tryMSAA && numConfigs > 0) {
                configs = new EGLConfig[numConfigs];
                try {
                    if (egl.eglChooseConfig(display, s_configAttribsMSAA8, configs, numConfigs, num_config)) {
                        numConfigs = num_config[0];
                        Log.v("OF", String.format("eglChooseConfig MSAA8 Success"));
                        if (numConfigs > 0) workingConfig = true;
                    }

                } catch (Exception ex) {
                    Log.e("OF", String.format("eglChooseConfig MSAAx8 failed with EXCEPTION:") + ex.getMessage());
                }
            }
            if (numConfigs <= 0 && !workingConfig) {
                setRGB();

                configs = new EGLConfig[numConfigs];
                try {
                    if (tryMSAA && !egl.eglChooseConfig(display, s_configAttribsMSAA, null, 0,
                            num_config)) {
                        Log.w("OF", String.format("eglChooseConfig MSAA failed"));
                    }
                } catch (Exception ex) {
                    Log.e("OF", String.format("eglChooseConfig MSAAx4 failed with EXCEPTION:") + ex.getMessage());
                }
                numConfigs = num_config[0];
                if(tryMSAA && numConfigs > 0) {
                    configs = new EGLConfig[numConfigs];
                    try {

                        if(egl.eglChooseConfig(display, s_configAttribsMSAA, configs, numConfigs, num_config)){
                            numConfigs = num_config[0];
                            Log.v("OF", String.format("eglChooseConfig MSAA Success"));
                            if(numConfigs > 0) workingConfig = true;
                        }
                    } catch (Exception ex) {
                        Log.e("OF", String.format("eglChooseConfig MSAA failed with EXCEPTION:") + ex.getMessage());
                    }
                }
                if (numConfigs <= 0 && !workingConfig) {
                    configs = new EGLConfig[numConfigs];

                    numConfigs = num_config[0];
                    if(tryMSAA && numConfigs > 0) {
                        configs = new EGLConfig[numConfigs];
                        try {
                            if(egl.eglChooseConfig(display, s_configAttribsMSAAFallBack, configs, numConfigs, num_config)){
                                numConfigs = num_config[0];
                                Log.v("OF", String.format("eglChooseConfig MSAA Tegra Success"));
                                if(numConfigs > 0) workingConfig = true;
                            }
                        } catch (Exception ex) {
                            Log.e("OF", String.format("eglChooseConfig MSAA Fallback failed with EXCEPTION:") + ex.getMessage());
                        }
                    }
                    if (numConfigs <= 0 && !workingConfig) {
                        if(tryMSAA) Log.i("OF", String.format("eglChooseConfig MSAAx4 TEGRA2"));
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
                                    if(numConfigs > 0) workingConfig = true;
                                }
                            } catch (Exception ex) {
                                Log.e("OF", String.format("eglChooseConfig MSAA Fallback failed with EXCEPTION:") + ex.getMessage());
                            }
                        }
                        if (numConfigs <= 0 && !workingConfig) {
                            Log.i("OF", String.format("eglChooseConfig Default8"));
                            if (!egl.eglChooseConfig(display, s_configAttribsDefault8, null, 0,
                                    num_config)) {
                                Log.w("OF", String.format("s_configAttribsDefault8 Fallback failed"));
                            }
                            numConfigs = num_config[0];
                            if(numConfigs > 0) {
                                configs = new EGLConfig[numConfigs];
                                try {
                                    if(egl.eglChooseConfig(display, s_configAttribsDefault8, configs, numConfigs, num_config)){
                                        numConfigs = num_config[0];
                                        if(numConfigs > 0) workingConfig = true;
                                    }
                                } catch (Exception ex) {
                                    Log.e("OF", String.format("eglChooseConfig Default8 failed with EXCEPTION:") + ex.getMessage());
                                }
                            }
                            if (numConfigs <= 0  && !workingConfig) {
                                Log.i("OF", String.format("eglChooseConfig Default"));
                                try {
                                    if (!egl.eglChooseConfig(display, s_configAttribsDefault, null, 0,
                                            num_config)) {
                                        Log.w("OF", String.format("eglChooseConfig Default failed"));
                                    }
                                } catch (Exception ex) {
                                    Log.e("OF", String.format("eglChooseConfig s_configAttribsDefault failed with EXCEPTION:") + ex.getMessage());
                                }
                                numConfigs = num_config[0];
                                if(numConfigs > 0) {
                                    configs = new EGLConfig[numConfigs];
                                    try {
                                        if(egl.eglChooseConfig(display, s_configAttribsDefault, configs, numConfigs, num_config)){
                                            numConfigs = num_config[0];
                                            if(numConfigs > 0) workingConfig = true;
                                        }
                                    } catch (Exception ex) {
                                        Log.e("OF", String.format("eglChooseConfig s_configAttribsDefault failed with EXCEPTION:") + ex.getMessage());
                                    }
                                }
                                if (numConfigs <= 0 && !workingConfig) {
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




        if (DEBUG) {
            printConfigs(egl, display, configs);
        }
        /* Now return the "best" one
         */

        EGLConfig finalConfig = null;
        try {
            finalConfig = chooseConfig(egl, display, configs);
        } catch(Exception exception)
        {
            Log.e("OF", String.format("finalConfig choose exception: ") + exception.getMessage());

        }

        int samples = 1;
        try {
            samples = findConfigAttrib(egl, display, finalConfig,
                    EGL10.EGL_SAMPLES, 1);
        } catch(Exception exception)
        {

        }


        if(samples == 0) samples = 1;
        mSampleSize = samples;

        if (DEBUG) {
            printConfig(egl, display, finalConfig);
        }
        return finalConfig;

    }

    public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display,
                                  EGLConfig[] configs) {

        EGLConfig foundConfig = null;
        for(EGLConfig config : configs) {

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

            int depth = findConfigAttrib(egl, display, config,
                    EGL10.EGL_DEPTH_SIZE, 0);

            int stencil = findConfigAttrib(egl, display, config,
                    EGL10.EGL_STENCIL_SIZE, 0);

//            int gamut =  findConfigAttrib(egl, display, config,
//                    EGL_GL_COLORSPACE_KHR, 0);
//
//            if(gamut != 0) {
//                Log.w("OF", String.format("WideGamut: is ", gamut));
//            }

            int sampleBuffers = findConfigAttrib(egl, display, config,
                    EGL10.EGL_SAMPLE_BUFFERS, 1);
            int samples = findConfigAttrib(egl, display, config,
                    EGL10.EGL_SAMPLES, 0);
            String output = String.format("Config Row: r%d g:%d b%d a:%d d:%d s%d aa:%d sbuffer:%d", r, g, b, a, depth,stencil, samples, sampleBuffers);

            if (OFAndroid.samples > 1 && Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                int anglesamples = findConfigAttrib(egl, display, config,
                        EGL_MAX_SAMPLES_ANGLE, 0);
                // check for ANGLE Lib EGL Samples
                if (foundConfig == null && r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && depth == mDepthSize && stencil == mStencilSize && anglesamples == mSampleSize) {
                    if (foundConfig == null) {
                        foundConfig = config;
                        Log.i("NV samples OF", "Found and Set:" + output);
                    } else {
                        foundConfig = config;
                        Log.i("OF", "Override Already Found and Set:" + output);
                    }
                } else {
                    Log.v("OF", "Else Config: " + output);
                }
                if (foundConfig == null && r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && depth == mDepthSize && stencil == mStencilSize && anglesamples == mSampleSize/2) {
                    if (foundConfig == null) {
                        foundConfig = config;
                        Log.i("NV samples OF", "Found and Set:" + output);
                    } else {
                        foundConfig = config;
                        Log.i("OF", "Override Already Found and Set:" + output);
                    }
                } else {
                    Log.v("OF", "Else Config: " + output);
                }
                if (foundConfig == null && r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && depth == mDepthSize && stencil == mStencilSize && anglesamples == mSampleSize/4) {
                    if (foundConfig == null) {
                        foundConfig = config;
                        Log.i("NV samples OF", "Found and Set:" + output);
                    } else {
                        foundConfig = config;
                        Log.i("OF", "Override Already Found and Set:" + output);
                    }
                } else {
                    Log.v("OF", "Else Config: " + output);
                }


            }
            int nvsamples = findConfigAttrib(egl, display, config,
                    EGL_COVERAGE_SAMPLES_NV, 0);

            if (OFAndroid.samples > 1 && foundConfig == null &&  r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && depth == mDepthSize && stencil == mStencilSize && nvsamples == 5) {
                if (foundConfig == null) {
                    foundConfig = config;
                    Log.i("NV samples OF", "Found and Set:" + output);
                }
                else {
                    foundConfig = config;
                    Log.i("OF", "Override Already Found and Set:" + output);
                }
            } else {
                Log.v("OF", "Else Config: " + output);
            }

            if (OFAndroid.samples > 1 && foundConfig == null && r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && depth == mDepthSize && stencil == mStencilSize && samples == mSampleSize) {
                if (foundConfig == null) {
                    foundConfig = config;
                    mSampleSize = samples;
                    Log.i("OF", "MSAA Found and Set:" + output);
                }
                else {
                    foundConfig = config;
                    mSampleSize = samples;
                    Log.i("OF", "Override Already Found and Set:" + output);
                }
            } else {
                Log.v("OF", "Else Config: " + output);
            }

            if (OFAndroid.samples > 1 && foundConfig == null && r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && depth == mDepthSize && stencil == mStencilSize && samples == mSampleSize/2) {
                if (foundConfig == null) {
                    foundConfig = config;
                    mSampleSize = samples;
                    Log.i("OF", "MSAA/2 Found and Set:" + output);
                }
                else {
                    Log.i("OF", "Override Already Found and Set:" + output);
                }
            } else {
                Log.v("OF", "Else Config: " + output);
            }

            if (OFAndroid.samples > 1 && foundConfig == null &&  r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && depth == mDepthSize && stencil == mStencilSize && samples == mSampleSize/4) {

                foundConfig = config;
                mSampleSize = samples;
                Log.i("OF", "MSAA/4 Found and Set:" + output);

            } else {
                Log.v("OF", "Else Config: " + output);
            }

            if (OFAndroid.samples > 1 &&  foundConfig == null && r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && depth == mDepthSize && stencil == mStencilSize && samples == 2 && mSampleSize > 2) {
                foundConfig = config;
                mSampleSize = samples;
                Log.i("OF", "MSAAx2 Found and Set:" + output);

            } else {
                 Log.v("OF", "Else Config: " + output);
            }

            if (OFAndroid.samples > 1 && foundConfig == null && r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && depth == mDepthSize && stencil == mStencilSize && samples == 1 && mSampleSize >= 1) {

                foundConfig = config;
                mSampleSize = 1;
                Log.i("OF", "Found and Set :" + output);

            } else {
                Log.v("OF", "Else Config: " + output);
            }

            if ( foundConfig == null && r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && samples == 1 && mSampleSize >= 1) {

                foundConfig = config;
                mSampleSize = 1;
                Log.i("OF", "MSAA Any Found and Set :Depth Size and Stencil any:" + output);

            } else {
                Log.v("OF", "Else Config: " + output);
            }

            if (foundConfig == null && r == mRedSize && g == mGreenSize && b == mBlueSize && 16 == mDepthSize && mSampleSize >= 1) {

                foundConfig = config;
                mSampleSize = 1;
                Log.i("OF", "MSAA Any - Depth 16 - Last Resort Found and Set:" + output);

            } else {
                Log.v("OF", "Else Config: " + output);
            }

            if (foundConfig == null && r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && mSampleSize >= 1) {

                foundConfig = config;
                mSampleSize = 1;
                Log.i("OF", "Last Resort Found and Set:" + output);

            } else {
                Log.v("OF", "Else Config: " + output);
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
                EGL_RENDERABLE_TYPE,
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

