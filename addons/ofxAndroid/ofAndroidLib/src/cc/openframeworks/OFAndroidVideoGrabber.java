package cc.openframeworks;

import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;

import android.graphics.PixelFormat;
import android.hardware.Camera;
import android.util.Log;

public class OFAndroidVideoGrabber extends OFAndroidObject implements Runnable, Camera.PreviewCallback {
	private Camera camera;
	private byte[] buffer;
	private int width, height, targetFps;
	private Thread thread;
	private int id;
	private static int nextId=0;
	public static Map<Integer,OFAndroidVideoGrabber> camera_instances = new HashMap<Integer,OFAndroidVideoGrabber>();
	private boolean initialized = false;
	private Method addBufferMethod;
	
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
		config.setPreviewSize(w, h);
		config.setPreviewFormat(PixelFormat.YCbCr_420_SP);
		config.setPreviewFrameRate(targetFps);
		camera.setParameters(config);
		
		config = camera.getParameters();
		width = config.getPreviewSize().width;
		height = config.getPreviewSize().height;
		targetFps = _targetFps;
		Log.i("OF","camera settings: " + width + "x" + height);
		buffer = new byte[width*height*2];
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
		}
	}
	
	@Override
	public void pause(){
		if(initialized){
			Log.i("OF","pausing camera preview");
			camera.stopPreview();
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
	
	public static native int newFrame(byte[] data, int width, int height);

	

}
