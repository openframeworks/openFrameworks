package cc.openframeworks;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;

public class OFAndroidGPS extends OFAndroidObject implements LocationListener, SensorEventListener {
	
	private static OFAndroidGPS m_instance = null;
	
	private boolean m_gpsStarted;
	private boolean m_compassStarted;
	
	float[] m_currentGravityData;
	float[] m_currentGeomagneticData;

	
	private static OFAndroidGPS getInstance()
	{
		if(m_instance == null)
			m_instance = new OFAndroidGPS();
		
		return m_instance;
	}
	
	void startGPS(){
		
		OFAndroidObject.activity.runOnUiThread(new Runnable(){
			public void run(){
				// Acquire a reference to the system Location Manager
				LocationManager locationManager = (LocationManager) OFAndroidObject.activity.getSystemService(Context.LOCATION_SERVICE);
				
				// Register the listener with the Location Manager to receive location updates
				locationManager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER, 0, 0, OFAndroidGPS.this);
				
				// Register the listener with the Location Manager to receive location updates
				locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 0, 0, OFAndroidGPS.this);
				
				Location lastLocation = locationManager.getLastKnownLocation(LocationManager.GPS_PROVIDER);
				if(lastLocation!=null)
					onLocationChanged(lastLocation);
				m_gpsStarted = true;
			}
		});
	}
	
	void stopGPS(){

		OFAndroidObject.activity.runOnUiThread(new Runnable(){
			public void run(){
				// Acquire a reference to the system Location Manager
				LocationManager locationManager = (LocationManager) OFAndroidObject.activity.getSystemService(Context.LOCATION_SERVICE);
		
				locationManager.removeUpdates(OFAndroidGPS.this);
				
				m_gpsStarted = false;
			}
		});
	}
	
	void startCompass()
	{
		OFAndroidObject.activity.runOnUiThread(new Runnable(){
			public void run(){
				
				SensorManager sensorManager = (SensorManager) OFAndroidObject.activity.getSystemService(Context.SENSOR_SERVICE);
				
				sensorManager.registerListener(
						OFAndroidGPS.this, 
						sensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD),
						SensorManager.SENSOR_DELAY_UI);
				
				sensorManager.registerListener(
						OFAndroidGPS.this, 
						sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), 
						SensorManager.SENSOR_DELAY_UI);
				
				sensorManager.registerListener(
						OFAndroidGPS.this, 
						sensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE),
						SensorManager.SENSOR_DELAY_UI);
				
				m_compassStarted = true;
			}
		});
	}
	
	void stopCompass()
	{
		OFAndroidObject.activity.runOnUiThread(new Runnable(){
			public void run(){

				SensorManager sensorManager = (SensorManager) OFAndroidObject.activity.getSystemService(Context.SENSOR_SERVICE);
				
				sensorManager.unregisterListener(OFAndroidGPS.this);

				m_compassStarted = false;
			}
		});
	}

	@Override
    public void onLocationChanged(Location location) {
      // Called when a new location is found by the network location provider.
      //makeUseOfNewLocation(location);
    	locationChanged(location.getAltitude(),location.getLatitude(),location.getLongitude(),location.getSpeed(),location.getBearing());
    }

	@Override
	public void onSensorChanged(SensorEvent event) {

		if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER)
			m_currentGravityData = event.values;
		
		if (event.sensor.getType() == Sensor.TYPE_MAGNETIC_FIELD)
			m_currentGeomagneticData = event.values;
		
		if (m_currentGravityData != null && m_currentGeomagneticData != null) {
			float R[] = new float[9];
			float I[] = new float[9];
			boolean success = SensorManager.getRotationMatrix(R, I, m_currentGravityData, m_currentGeomagneticData);
			
			if (success) {
				float orientation[] = new float[3];
				SensorManager.getOrientation(R, orientation);
				
				headingChanged(360 - orientation[0] * 180 / Math.PI); // orientation contains: azimut, pitch and roll
			}
		}
	}
	
	@Override
	public void onAccuracyChanged(Sensor arg0, int arg1) {
		// TODO Auto-generated method stub
		
	}


    public void onProviderEnabled(String provider) {}

    public void onProviderDisabled(String provider) {}

	public void onStatusChanged(String provider, int status, Bundle extras) {
		// TODO Auto-generated method stub
		
	}
	
	
	public static native void locationChanged(double altitude, double latitude, double longitude, float speed, float heading);
	
	public static native void headingChanged(double heading);

	@Override
	protected void appPause() {
		boolean gpsStarted = m_gpsStarted;
		stopGPS();
		m_gpsStarted = gpsStarted;
		
		boolean compassStarted = m_compassStarted;
		stopCompass();
		m_compassStarted = compassStarted;
	}

	@Override
	protected void appResume() {
		if(m_gpsStarted)
			startGPS();
		
		if (m_compassStarted)
			startCompass();
	}

	@Override
	protected void appStop() {
		boolean gpsStarted = m_gpsStarted;
		stopGPS();
		m_gpsStarted = gpsStarted;
		
		boolean compassStarted = m_compassStarted;
		stopCompass();
		m_compassStarted = compassStarted;
	}
}
