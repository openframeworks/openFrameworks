package cc.openframeworks;

import java.util.List;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.util.Log;
import android.view.Display;
import android.view.WindowManager;

public class OFAndroidMagneticField extends OFAndroidObject implements OFAndroidAccelerometerListener  {
	
	private SensorManager sensorManager;
    private Sensor magnetField;
    
    private float[] valuesMagneticField;
    private boolean hasCompass;
    
    private float[] matrixR;
    private float[] outR;
    private float[] valuesOrientation;
    private float[] valuesAccelerometer;
        
    OFAndroidMagneticField(SensorManager sensorManager){
        this.sensorManager = sensorManager;
        hasCompass = false;
        List<Sensor> sensors = sensorManager.getSensorList(Sensor.TYPE_MAGNETIC_FIELD);
        if(sensors.size() > 0)
        {
        	magnetField = sensors.get(0);
        	sensorManager.registerListener(magnetListener, magnetField, SensorManager.SENSOR_DELAY_GAME);
        	Log.v("OF", "magnetic field sensor set up correctly");
            valuesMagneticField = new float[3];
            hasCompass = true;
            
            matrixR = new float[9];
            outR = new float[9];
            valuesOrientation = new float[3];
            valuesAccelerometer = new float[3];
       
        }else{
        	Log.e("OF","no magnetic field sensor available");
        }
    }

    public boolean hasCompass() {
    	return hasCompass;
    }
	
	public float getOrientation() {
		return valuesMagneticField[0];
	}
	
	public float getTrueOrientation() {
		// distance from magnetic north;
		return valuesOrientation[0];
	}

	@Override 
	public void updateAccelerometerValues(float x, float y, float z) {
		
		valuesAccelerometer[0] = x;
		valuesAccelerometer[1] = y;
		valuesAccelerometer[2] = z;
		
    }

	private SensorEventListener magnetListener = new SensorEventListener() {
	    
	    public void onSensorChanged(SensorEvent event) {
	    	
	    	for(int i =0; i < 3; i++){
    		    valuesMagneticField[i] = event.values[i];
	    	}	
	 
	    	// Load the rotation matrix into matrixR.
	    	boolean success = SensorManager.getRotationMatrix(
	   		       matrixR,
	   		       null,
	   		       valuesAccelerometer,
	   		       valuesMagneticField);
	    	  
	   		if(success){
	   				
	   				// This is regarding landscape orientation (needs testing)
	   				WindowManager windowManager = (WindowManager)OFAndroid.getContext().getSystemService(Context.WINDOW_SERVICE);
	   				Display display = windowManager.getDefaultDisplay();
	   				int rotation = display.getRotation();
	   					
	   				if (rotation == 1) {
	   					SensorManager.remapCoordinateSystem(matrixR, SensorManager.AXIS_X, SensorManager.AXIS_MINUS_Z, outR);
	   				} else {
	   					SensorManager.remapCoordinateSystem(matrixR, SensorManager.AXIS_Y, SensorManager.AXIS_MINUS_Z, outR);
	   				}
	   			
	   			   // Get the orientation values.
	   			   SensorManager.getOrientation(matrixR, valuesOrientation);
	   			   
	   			  /* [0] : yaw (azimuth), rotation around z axis
	   			   * [1] : pitch, rotation around x axis
	   			   * [2] : roll, rotation around y axis */
	   			  
	   			  valuesOrientation[0] = (float)Math.toDegrees (valuesOrientation[0]);
	   			  valuesOrientation[1] = (float)Math.toDegrees (valuesOrientation[1]); 
	   			  valuesOrientation[2] = (float)Math.toDegrees (valuesOrientation[2]);
	   			
	   			  valuesOrientation[0] = (valuesOrientation[0] + 360) % 360;
	   			 
	   		}
	   		
	    }
	    
	    public void onAccuracyChanged(Sensor sensor, int accuracy) {
	        // do things if you're interested in accuracy changes
	    }
	
	};

	@Override
	protected void appPause() {
		sensorManager.unregisterListener(magnetListener);
		
	}

	@Override
	protected void appResume() {
		sensorManager.registerListener(magnetListener, magnetField, SensorManager.SENSOR_DELAY_GAME);   
	}

	@Override
	protected void appStop() {
		sensorManager.unregisterListener(magnetListener);
	}
    
}
