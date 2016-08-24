package cc.openframeworks;

import java.util.List;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.util.Log;
import android.view.Display;
import android.view.Surface;
import android.view.WindowManager;

public class OFAndroidAccelerometer extends OFAndroidObject {
	private SensorManager sensorManager;
    private Sensor accelerometer;
    
    OFAndroidAccelerometer(SensorManager sensorManager){
        this.sensorManager = sensorManager;
        List<Sensor> sensors = sensorManager.getSensorList(Sensor.TYPE_ACCELEROMETER);
        if(sensors.size() > 0)
        {
        	accelerometer = sensors.get(0);
        	sensorManager.registerListener(sensorListener, accelerometer, SensorManager.SENSOR_DELAY_GAME);   
        	Log.v("OF", "accelerometer set up correctly");
        }else{
        	Log.e("OF","no accelerometer available");
        }
    }

	@Override
	protected void appPause() {
		sensorManager.unregisterListener(sensorListener);
		
	}

	@Override
	protected void appResume() {
		sensorManager.registerListener(sensorListener, accelerometer, SensorManager.SENSOR_DELAY_GAME);   
	}

	@Override
	protected void appStop() {
		sensorManager.unregisterListener(sensorListener);
	}
	
	
	private final SensorEventListener sensorListener = new SensorEventListener() {
		public void onSensorChanged(SensorEvent event) {
	    	WindowManager windowManager = (WindowManager)OFAndroidLifeCycle.getActivity().getSystemService(Context.WINDOW_SERVICE);
	    	Display display = windowManager.getDefaultDisplay();
	    	
	    	float x, y;
	    	switch (display.getRotation()) {
	    	case Surface.ROTATION_90:
	    		x = -event.values[1];
	    		y = event.values[0];
	    		break;
	    	case Surface.ROTATION_180:
	    		x = event.values[0];
	    		y = -event.values[1];
	    		break;
	    	case Surface.ROTATION_270:
	    		x = event.values[1];
	    		y = -event.values[0];
	    		break;
	    	case Surface.ROTATION_0:
	    	default:
	    		x = event.values[0];
	    		y = event.values[1];
	    		break;
	    	}
	    	
			updateAccelerometer(x, y,
			          event.values[2]);
		}
		 
		public void onAccuracyChanged(Sensor sensor, int accuracy) {}
	};

	public static native void updateAccelerometer(float x, float y, float z);
    
}
