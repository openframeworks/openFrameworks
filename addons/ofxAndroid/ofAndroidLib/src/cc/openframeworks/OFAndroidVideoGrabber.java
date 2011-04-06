package cc.openframeworks;

import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;

import android.content.Context;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.util.Log;
import android.view.OrientationEventListener;

public class OFAndroidVideoGrabber extends OFAndroidObject implements Runnable, Camera.PreviewCallback {
	
	
	public OFAndroidVideoGrabber(){
		id=nextId++;
		camera_instances.put(id, this);
	}
	
	public int getId(){
		return id;
	}
	
	public static OFAndroidVideoGrabber getInstance(int id){
		return camera_instances.get(id);
	}
	
	
	void initGrabber(int w, int h, int _targetFps){
		camera = Camera.open();
		Camera.Parameters config = camera.getParameters();
		Log.i("OF", "Grabber default format: " + config.getPreviewFormat());
		Log.i("OF", "Grabber default preview size: " + config.getPreviewSize().width + "," + config.getPreviewSize().height);
		config.setPreviewSize(w, h);
		config.setPreviewFormat(ImageFormat.NV21);
		config.setPreviewFrameRate(targetFps);
		camera.setParameters(config);
		
		config = camera.getParameters();
		width = config.getPreviewSize().width;
		height = config.getPreviewSize().height;
		targetFps = _targetFps;
		Log.i("OF","camera settings: " + width + "x" + height);
		if(width!=w || height!=h)  Log.w("OF","camera size different than asked for, resizing (this can slow the app)");
		buffer = new byte[width*height*2];
		
		orientationListener = new OrientationListener(OFAndroid.getContext());
		orientationListener.enable();
		
		thread = new Thread(this);
		thread.start();
		initialized = true;
	}
	
	
	
	@Override
	public void stop(){
		if(initialized){
			Log.i("OF","stopping camera");
			camera.stopPreview();
			try {
				thread.join();
			} catch (InterruptedException e) {
				Log.e("OF", "problem trying to close camera thread", e);
			}
			camera.release();
			orientationListener.disable();
		}
	}
	
	@Override
	public void pause(){
		if(initialized){
			Log.i("OF","pausing camera preview");
			camera.stopPreview();
			orientationListener.disable();
		}
			
	}
	
	@Override
	public void resume(){
		if(initialized){
			switch(state){
			case Paused:
				Log.i("OF","camera paused, resuming");
				camera.startPreview();
				break;
			case Stopped:
				Log.i("OF","camera stopped, reinitializing");
				initGrabber(width,height,targetFps);
				break;
			}
			orientationListener.enable();
		}
	}
	
	public void onPreviewFrame(byte[] data, Camera camera) {
		//Log.i("OF","video buffer length: " + data.length);
		//Log.i("OF", "size: " + camera.getParameters().getPreviewSize().width + "x" + camera.getParameters().getPreviewSize().height);
		//Log.i("OF", "format " + camera.getParameters().getPreviewFormat());
		newFrame(data, width, height);
		
		if(addBufferMethod!=null){
			try {
				addBufferMethod.invoke(camera, buffer);
			} catch (Exception e) {
				Log.e("OF","error adding buffer",e);
			} 
		}
		//camera.addCallbackBuffer(data);
		
	}

	public void run() {
		thread.setPriority(Thread.MAX_PRIORITY);
		try {
			addBufferMethod = Camera.class.getMethod("addCallbackBuffer", byte[].class);
			addBufferMethod.invoke(camera, buffer);
			Camera.class.getMethod("setPreviewCallbackWithBuffer", Camera.PreviewCallback.class).invoke(camera, this);
			Log.i("OF","setting camera callback with buffer");
		} catch (SecurityException e) {
			Log.e("OF","security exception, check permissions to acces to the camera",e);
		} catch (NoSuchMethodException e) {
			try {
				Camera.class.getMethod("setPreviewCallback", Camera.PreviewCallback.class).invoke(camera, this);
				Log.i("OF","setting camera callback without buffer");
			} catch (SecurityException e1) {
				Log.e("OF","security exception, check permissions to acces to the camera",e1);
			} catch (Exception e1) {
				Log.e("OF","cannot create callback, the camera can only be used from api v7",e1);
			} 
		} catch (Exception e) {
			Log.e("OF","error adding callback",e);
		}
		
		//camera.addCallbackBuffer(buffer);
		//camera.setPreviewCallbackWithBuffer(this);
		camera.startPreview();
	}

	private class OrientationListener extends OrientationEventListener{

		public OrientationListener(Context context) {
			super(context);
			// TODO Auto-generated constructor stub
		}

		@Override
		public void onOrientationChanged(int orientation) {
			if (orientation == ORIENTATION_UNKNOWN) return;
			Camera.Parameters config = camera.getParameters();
			/*Camera.CameraInfo info =
			        new Camera.CameraInfo();*/
			//Camera.getCameraInfo(camera, info);
			orientation = (orientation + 45) / 90 * 90;
			int rotation = orientation % 360;
			//if (info.facing == CameraInfo.CAMERA_FACING_FRONT) {
			    //rotation = (info.orientation - orientation + 360) % 360;
			/*} else {  // back-facing camera
			    rotation = (info.orientation + orientation) % 360;
			}*/
			config.setRotation(rotation);
			camera.setParameters(config);
		}
		
	}
	
	public static native int newFrame(byte[] data, int width, int height);
	
	

	private Camera camera;
	private byte[] buffer;
	private int width, height, targetFps;
	private Thread thread;
	private int id;
	private static int nextId=0;
	public static Map<Integer,OFAndroidVideoGrabber> camera_instances = new HashMap<Integer,OFAndroidVideoGrabber>();
	private boolean initialized = false;
	private Method addBufferMethod;
	private OrientationListener orientationListener;
	

}
