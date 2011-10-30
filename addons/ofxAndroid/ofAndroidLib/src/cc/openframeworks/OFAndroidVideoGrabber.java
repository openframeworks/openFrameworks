package cc.openframeworks;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;

import android.content.Context;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.hardware.Camera.Size;
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
	
	void setDeviceID(int id){
		deviceID = id;
	}
	
	void initGrabber(int w, int h, int _targetFps){
		if(deviceID==-1)
			camera = Camera.open();
		else{			
			try {
				int numCameras = (Integer) Camera.class.getMethod("getNumberOfCameras").invoke(null);
				Class<?> cameraInfoClass = Class.forName("android.hardware.Camera$CameraInfo");
				Object cameraInfo = null;
				Field field = null;
		        if ( cameraInfoClass != null ) {
		            cameraInfo = cameraInfoClass.newInstance();
		        }
		        if ( cameraInfo != null ) {
		            field = cameraInfo.getClass().getField( "facing" );
		        }
				Method getCameraInfoMethod = Camera.class.getMethod( "getCameraInfo", Integer.TYPE, cameraInfoClass );
				for(int i=0;i<numCameras;i++){
					getCameraInfoMethod.invoke( null, i, cameraInfo );
	                int facing = field.getInt( cameraInfo );
	                Log.v("OF","Camera " + i + " facing: " + facing);
				}
				camera = (Camera) Camera.class.getMethod("open", Integer.TYPE).invoke(null, deviceID);
			} catch (Exception e) {
				// TODO Auto-generated catch block
				Log.e("OF","Error trying to open specific camera, trying default",e);
				camera = Camera.open();
			} 
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
		config.setPreviewSize(w, h);
		config.setPreviewFormat(ImageFormat.NV21);
		try{
			camera.setParameters(config);
		}catch(Exception e){
			Log.e("OF","couldn init camera", e);
		}

		config = camera.getParameters();
		width = config.getPreviewSize().width;
		height = config.getPreviewSize().height;
		if(width!=w || height!=h)  Log.w("OF","camera size different than asked for, resizing (this can slow the app)");
		
		
		if(_targetFps!=-1){
			config = camera.getParameters();
			config.setPreviewFrameRate(_targetFps);
			try{
				camera.setParameters(config);
			}catch(Exception e){
				Log.e("OF","couldn init camera", e);
			}
		}
		
		targetFps = _targetFps;
		Log.i("OF","camera settings: " + width + "x" + height);
		
		buffer = new byte[width*height*2];
		
		orientationListener = new OrientationListener(OFAndroid.getContext());
		orientationListener.enable();
		
		thread = new Thread(this);
		thread.start();
		initialized = true;
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
			Log.i("OF","stopping camera");
			camera.stopPreview();
			previewStarted = false;
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
	public void appPause(){
		appStop();
			
	}
	
	@Override
	public void appResume(){
		if(initialized){
			initGrabber(width,height,targetFps);
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
		try{
			camera.startPreview();
			previewStarted = true;
		} catch (Exception e) {
			Log.e("OF","error starting preview",e);
		}
	}

	private class OrientationListener extends OrientationEventListener{

		public OrientationListener(Context context) {
			super(context);
			// TODO Auto-generated constructor stub
		}

		@Override
		public void onOrientationChanged(int orientation) {
			if (orientation == ORIENTATION_UNKNOWN) return;
			try{
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
			}catch(Exception e){
				
			}
		}
		
	}
	
	public static native int newFrame(byte[] data, int width, int height);
	
	

	private Camera camera;
	private int deviceID = -1;
	private byte[] buffer;
	private int width, height, targetFps;
	private Thread thread;
	private int id;
	private static int nextId=0;
	public static Map<Integer,OFAndroidVideoGrabber> camera_instances = new HashMap<Integer,OFAndroidVideoGrabber>();
	private boolean initialized = false;
	private boolean previewStarted = false;
	private Method addBufferMethod;
	private OrientationListener orientationListener;
	

}
