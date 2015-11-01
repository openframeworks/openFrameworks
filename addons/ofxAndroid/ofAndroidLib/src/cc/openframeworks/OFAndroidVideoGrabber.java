package cc.openframeworks;

import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.locks.ReentrantLock;


import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.ImageFormat;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.util.Log;
import android.view.OrientationEventListener;

public class OFAndroidVideoGrabber extends OFAndroidObject implements Runnable, Camera.PreviewCallback {
	
	
	public OFAndroidVideoGrabber(){
		instanceId=nextId++;
		camera_instances.put(instanceId, this);
		threadLock = new ReentrantLock();
	}
	
	public int getId(){
		return instanceId;
	}
	
	public void setTexture(int texID){
        try {
            // Create surface texture instance
            surfaceTexture = new SurfaceTexture(texID);

            // set texture as preview surface for camera
            camera.setPreviewTexture(surfaceTexture);
        } catch (Exception e1) {
            Log.e("OF","Error initializing gl surface",e1);
        }
	}

    public int getNumCameras(){
        return Camera.getNumberOfCameras();
    }

    public int getCameraFacing(int facing){
        int numCameras = Camera.getNumberOfCameras();
        Camera.CameraInfo cameraInfo = new Camera.CameraInfo();
        for(int i=0;i<numCameras;i++){
            Camera.getCameraInfo(i, cameraInfo);
            int _facing = cameraInfo.facing;
            if(_facing == facing){
                return i;
            }
        }
        return -1;
    }

    public void setDeviceID(int _deviceId){
        deviceID = _deviceId;

        if(initialized){
            stopGrabber();

            initGrabber(width, height, targetFps, texID);
        }
    }

	public void initGrabber(int w, int h, int _targetFps, int _texID){
        if(camera != null){
            camera.release();
        }

		if(deviceID==-1)
            deviceID = getCameraFacing(0);

        try {
            camera = Camera.open(deviceID);
        } catch (Exception e) {
            Log.e("OF","Error trying to open specific camera, trying default",e);
            camera = Camera.open();
        }

        if(_texID != -1) {
            texID = _texID;
            setTexture(texID);
        }

		Camera.Parameters config = camera.getParameters();
		
		Log.i("OF","Grabber supported sizes");
		for(Size s : config.getSupportedPreviewSizes()){
			Log.i("OF",s.width + " " + s.height);
		}
		
		Log.i("OF","Grabber supported formats");
		for(Integer i : config.getSupportedPreviewFormats()){
			Log.i("OF",i.toString());
		}
		
		Log.i("OF","Grabber supported fps");
		for(Integer i : config.getSupportedPreviewFrameRates()){
			Log.i("OF",i.toString());
		}

		Log.i("OF", "Grabber default format: " + config.getPreviewFormat());
		Log.i("OF", "Grabber default preview size: " + config.getPreviewSize().width + "," + config.getPreviewSize().height);
		config.setPictureSize(w, h);
		config.setPreviewSize(w, h);
		config.setPreviewFormat(ImageFormat.NV21);
        try{
			Method setRecordingHint = config.getClass().getMethod("setRecordingHint",boolean.class);
			setRecordingHint.invoke(config, true);
        }catch(Exception e){
        	Log.i("OF","couldn't set recording hint");
        }
		try{
			camera.setParameters(config);
		}catch(Exception e){
			Log.e("OF","couldn init camera", e);
		}

		config = camera.getParameters();
		width = config.getPreviewSize().width;
		height = config.getPreviewSize().height;
		if(width!=w || height!=h)  Log.w("OF","camera size different than asked for, resizing (this can slow the app)");
		
        targetFps = _targetFps;

        // If target fps is not defined, then take the maximum fps
        if(targetFps == -1) {
            for(Integer i : config.getSupportedPreviewFrameRates()){
                if(targetFps < i) {
                    targetFps = i;
                }
            }
        }

        Log.i("OF", "Grabber fps: " + targetFps);
        config = camera.getParameters();
        config.setPreviewFrameRate(targetFps);
        try{
            camera.setParameters(config);
        } catch(Exception e){
            Log.e("OF","couldn init camera", e);
        }

		Log.i("OF","camera settings: " + width + "x" + height);

        int bufferSize = width * height;
        if(buffer == null || buffer.length != bufferSize) {
            // it actually needs (width*height) * 3/2
            bufferSize = bufferSize * ImageFormat.getBitsPerPixel(config.getPreviewFormat()) / 8;
            buffer[0] = new byte[bufferSize];
            buffer[1] = new byte[bufferSize];
        }

        // Get camera info
        Camera.CameraInfo info = new Camera.CameraInfo();
        Camera.getCameraInfo(deviceID, info);

        cameraFacing = info.facing;
        cameraOrientation = info.orientation;

		//orientationListener = new OrientationListener(OFAndroid.getContext());
		//orientationListener.enable();
		
		thread = new Thread(this);
		thread.start();
		initialized = true;
	}

    public void stopGrabber(){
        if(initialized){
            Log.i("OF", "stopping camera");
            camera.stopPreview();
            previewStarted = false;
            try {
                thread.join();
            } catch (InterruptedException e) {
                Log.e("OF", "problem trying to close camera thread", e);
            }
            camera.setPreviewCallback(null);
            try {
                camera.setPreviewTexture(surfaceTexture);
            } catch (Exception e) {
            }
            camera.release();
            //orientationListener.disable();
            initialized = false;
        }
    }
	
	public void update(){
        try {
            surfaceTexture.updateTexImage();
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

       try {
            // Add the other buffer to the callback queue to indicate we are ready for new frame
            camera.addCallbackBuffer(buffer[bufferFlipFlip?0:1]);
            bufferFlipFlip = !bufferFlipFlip;
        } catch (Exception e) {
            Log.e("OF", "error adding buffer", e);
        }
	}

	// Getting the current frame data as byte array
	// Notice: this locks the thread, you need to  call releaseFrameData() when done
	public byte[] getFrameData(){
		threadLock.lock();
		return buffer[bufferFlipFlip?0:1];
	}

	public void releaseFrameData(){
		threadLock.unlock();
	}

	public void getTextureMatrix(float[] mtx) {
		try {
			if(surfaceTexture != null) surfaceTexture.getTransformMatrix(mtx);

		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

    public int getCameraOrientation(){
        return cameraOrientation;
    }

    public int getFacingOfCamera(int _deviceID){
        if(_deviceID == -1) _deviceID = deviceID;

        Camera.CameraInfo info = new Camera.CameraInfo();
        Camera.getCameraInfo(_deviceID, info);
        return info.facing;
    }

	public boolean setAutoFocus(boolean autofocus){
		
		if(initialized){
			while(initialized && !previewStarted){
				try {
					Thread.sleep(10);
				} catch (InterruptedException e) {
				}
			}
		}
		try{
			if(autofocus){
				camera.autoFocus(autoFocusCB);
			}else{
				camera.cancelAutoFocus();
			}
		}catch(Exception e){
			Log.e("OF","couldn't (de)activate autofocus", e);
			return false;
		}
		return true;
	}
	
	private class AutoFocusCB implements Camera.AutoFocusCallback{

		public void onAutoFocus(boolean success, Camera camera) {
			// TODO Auto-generated method stub
			
		}
		
	}
	
	private AutoFocusCB autoFocusCB = new AutoFocusCB();
	
	@Override
	public void appStop(){
		if(initialized){
			stopGrabber();
		}
	}
	
	@Override
	public void appPause(){
		appStop();
			
	}
	
	@Override
	public void appResume(){
		if(initialized){
			//orientationListener.enable();
		}
	}
	
	public void onPreviewFrame(byte[] data, Camera camera) {
		//Log.i("OF","video buffer length: " + data.length);
		//Log.i("OF", "size: " + camera.getParameters().getPreviewSize().width + "x" + camera.getParameters().getPreviewSize().height);
		//Log.i("OF", "format " + camera.getParameters().getPreviewFormat());
		threadLock.lock();

		// Tell the of app that a new frame has arrived
		newFrame(width, height, instanceId);

		/*try {
            camera.addCallbackBuffer(buffer[0]);
        } catch (Exception e) {
            Log.e("OF","error adding buffer",e);
        }*/

		threadLock.unlock();
	}

	public void run() {
		thread.setPriority(Thread.MAX_PRIORITY);
		try {
            // Add both callback buffers to the queue
			camera.addCallbackBuffer(buffer[0]);
            //camera.addCallbackBuffer(buffer[1]);
            camera.setPreviewCallbackWithBuffer(this);
            //camera.setPreviewCallback(this);

			Log.i("OF","setting camera callback with buffer");
		} catch (SecurityException e) {
			Log.e("OF","security exception, check permissions in your AndroidManifest to access to the camera",e);
		} catch (Exception e) {
			Log.e("OF","error adding callback",e);
		}
		
		try{
			camera.startPreview();
			previewStarted = true;
		} catch (Exception e) {
			Log.e("OF","error starting preview",e);
		}
	}

    // Currently not used
    private class OrientationListener extends OrientationEventListener{
		private int rotation = -1;

		public OrientationListener(Context context) {
			super(context);
		}

		@Override
		public void onOrientationChanged(int orientation) {
			if (orientation == ORIENTATION_UNKNOWN) return;
			try{
				orientation = (orientation + 45) / 90 * 90;
				int newRotation = orientation % 360;

				if (newRotation != rotation) {
					rotation = newRotation;
					Camera.Parameters config = camera.getParameters();
					config.setRotation(rotation);
					camera.setParameters(config);
				}
			} catch(Exception e) {
				
			}
		}
		
	}
	
	public static native int newFrame(int width, int height, int cameraId);
	
	

	private Camera camera;
	private int deviceID = -1;
	private byte[][] buffer = {null, null};
    private boolean bufferFlipFlip = false;
	private int width, height, targetFps;
	private int texID;
    private int cameraOrientation;
    private int cameraFacing;
	private Thread thread;
	private ReentrantLock threadLock;
	private int instanceId;
	private static int nextId=0;
	public static Map<Integer,OFAndroidVideoGrabber> camera_instances = new HashMap<Integer,OFAndroidVideoGrabber>();
	//private static OFCameraSurface cameraSurface = null;
	//private static ViewGroup rootViewGroup = null;
	private boolean initialized = false;
	private boolean previewStarted = false;
	private OrientationListener orientationListener;
	SurfaceTexture surfaceTexture;
	

}
