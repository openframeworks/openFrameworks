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
import android.opengl.GLES10;
import android.opengl.GLES20;
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

class OFAndroidWindow implements GLSurfaceView.Renderer {

	public OFAndroidWindow(int w, int h){
        if( OFActivity.LOG_ENGINE) Log.i("OF","OFAndroidWindow():width:" + w + " height:" + h);
	    setResolution(w,h, true);
	}

	public void setResolution(int w, int h, boolean updateSurface) {
        if( OFActivity.LOG_ENGINE) Log.i("OFAndroidWindow","setResolution:width:" + w + " height:" + h + "update:" + updateSurface);
        if(w == this.w && h == this.h && updateSurface == false) return;



        this.w = w;
        this.h = h;

        if(w != 0 && h != 0 && setup == true){
            resolutionSetup = true;
            if(updateSurface) onSurfaceChanged(w,h);
        }
    }

    public void surfaceHasBeenDestroyed() {
        if( OFActivity.LOG_ENGINE) Log.i("OF","surfaceHasBeenDestroyed");
        OFAndroid.onSurfaceDestroyed();
        has_surface = false;
        exit();
    }

	@Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

        if( OFActivity.LOG_ENGINE) Log.i("OF","onSurfaceCreated");
		// notify that old surface was destroyed
        boolean hasDestroyed = false;
		if(this.has_surface) {
            if( OFActivity.LOG_ENGINE) Log.i("OF","onSurfaceCreated - has_surface destroy");
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
            if( OFActivity.LOG_ENGINE) Log.i("OF", "hasDestroyed calling setup");
            setup();
        }
		OFAndroid.updateRefreshRates();
        gl.glClearColor(0f, 0f, 0, 1);
		return;
    }
    
	@Override
    public void onSurfaceChanged(GL10 gl, int w, int h) {
	    onSurfaceChanged(w,h);
    }

    public void onSurfaceChanged(int w, int h) {

        if( OFActivity.LOG_ENGINE) Log.i("OF","onSurfaceChanged width:" + w + " height:" + h);
        if(doNotDraw == true) return; // fix for strange case
        if(firstFrameDrawn == false) {
            Log.i("OF","onSurfaceChanged firstFrameDrawn not drawn");
            //return;
        }
        setResolution(w,h, false);
        if(!setup && OFAndroid.unpackingDone && firstFrameDrawn){
            if( OFActivity.LOG_ENGINE) Log.i("OF","onSurfaceChanged && OFAndroid.unpackingDone");
            setup();
        } else if(!setup && !OFAndroid.unpackingDone && firstFrameDrawn) {
            if( OFActivity.LOG_ENGINE) Log.i("OF","onSurfaceChanged not setup however !OFAndroid.unpackingDone");
            setup();
        } else if(setup && OFAndroid.unpackingDone) {
            if( OFActivity.LOG_ENGINE) Log.i("OF","onSurfaceChanged setup already");
        }
        OFGestureListener.swipe_Min_Distance = (int)(Math.max(w, h)*.04);
        OFGestureListener.swipe_Max_Distance = (int)(Math.max(w, h)*.6);
        GLES10.glViewport(0, 0, w, h);
        OFAndroid.resize(w, h);
    }

    private void setup(){
	    boolean isContext = true;
        if(android.opengl.EGL14.eglGetCurrentContext() == EGL_NO_CONTEXT) {
            isContext = false;
        }

        if(doNotDraw == true) return; // fix for strange case
        if( OFActivity.LOG_ENGINE) Log.i("OF","OFAndroidWindow::setup context:" + isContext);
        if(w <= 0 || h <= 0) {
            Log.e("OF","setup width or height is <=0. Will cause strange effects");
        }
        if(isContext == true) {
            OFAndroid.setup(w, h);
            setup = true;
        }
        try {
            android.os.Process.setThreadPriority(8);
            OFGestureListener.swipe_Min_Distance = (int)(Math.max(w, h)*.04);
            OFGestureListener.swipe_Max_Distance = (int)(Math.max(w, h)*.6);

        } catch (Exception e) {
            Log.e("OF", "setup OFAndroidWindow setup OFGestureListener:Exception:" + e.getMessage());
        }

        Activity activity = OFAndroidLifeCycle.getActivity();
		if(activity != null) {
            try {
                ((OFActivity) activity).onGLSurfaceCreated();
            } catch (Exception e) {
                Log.e("OF", "setup OFAndroidWindow setup OFGestureListener:Exception:" + e.getMessage());
            }
        }
		else {
		    try {
                Log.e("OF", "setup OFActivity.class.is NOT Instance(activity)" + activity.getLocalClassName());
            } catch (Exception e) {

            }
        }

        OFAndroidLifeCycle.HasSetup();

    }

    public void exit() {
	    setup = false;
    }

    public void setDoNotDraw() {
	    doNotDraw = true;
    }

    private int drawFrame = 0;
    @Override
    public void onDrawFrame(GL10 gl) {
            drawFrame++;
            //Log.i("OF","onDrawFrame" + drawFrame);
            // Remove the initial background
            if (!initialRender) {
                initialRender = true;
                drawClear = true;
                if( OFActivity.LOG_ENGINE) Log.i("OFAndroidWindow", "onDrawFrame initialRenderFrame!");
                if( OFAndroidLifeCycle.getGLView() != null)
                    OFAndroidLifeCycle.getGLView().setBackgroundResourceClear();
            } else {
                if(isWindowReady == false)
                    if(OFAndroid.isWindowReady()) isWindowReady = true;
            }
            if(setup && OFAndroid.unpackingDone && !doNotDraw && isWindowReady){
                if(android.opengl.EGL14.eglGetCurrentContext() != EGL_NO_CONTEXT) {

                    if(firstFrameDrawn == false) {
                        firstFrameDrawn = true;
                        drawClear = false;
                        OFAndroidLifeCycle.glStart();
                        OFAndroidLifeCycle.SetFirstFrameDrawn();
                       // gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
                        //gl.glClearColor(0f, 0f, 0f, 0.0f);
                        if( OFActivity.LOG_ENGINE) Log.i("OFAndroidWindow", "onDrawFrame setup and unpacking done SetFirstFrameDrawn");
                    }
                    OFAndroid.render();
                }
                else {
                    Log.e("OFAndroidWindow", "ofAndroidWindow::onDrawFrame GLContext = EGL_NO_CONTEXT BAD. Restarting Window");
                    setup = false;
                    setup();
                    drawClear = true;
                }
            }else if(doNotDraw){
                drawClear = false;
                Log.i("OFAndroidWindow", "onDrawFrame DoNotDraw");
            } else if(isWindowReady == false) {
                Log.w("OFAndroidWindow", "ofAndroidWindow::onDrawFrame isWindowReady:" + isWindowReady);
                drawClear = true;
            } else if(!setup && OFAndroid.unpackingDone){
                Log.i("OFAndroidWindow", "onDrawFrame !setup and unpacking done");
                setup();
                drawClear = true;
            }else{
                drawClear = true;
                Log.e("OFAndroidWindow", "onDrawFrame  draw clear");
            }
            if(drawClear) {
                Log.e("OFAndroidWindow", "onDrawFrame  draw clear");
                gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
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

    private boolean setup = false;
    private boolean isWindowReady = false;
    private boolean doNotDraw = false;
    private boolean resolutionSetup = false;
    private boolean initialRender = false;
    private boolean firstFrameDrawn = false;
    private int w,h = 0;
    public  boolean has_surface = false;

    private boolean drawClear = false;
}
