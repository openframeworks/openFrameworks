package cc.openframeworks;

import java.util.List;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.util.Log;

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
			updateAccelerometer(event.values[0],
			          event.values[1],
			          event.values[2]);
		}
		 
		public void onAccuracyChanged(Sensor sensor, int accuracy) {}
	};

	public static native void updateAccelerometer(float x, float y, float z);
    
}
