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

	boolean m_didReportLocation;
	
	private static OFAndroidGPS getInstance()
	{
		if(m_instance == null)
			m_instance = new OFAndroidGPS();
		
		return m_instance;
	}
	
	void startGPS(){
		
		OFAndroidObject.activity.runOnUiThread(new Runnable(){
			public void run(){
				
				m_didReportLocation = false;
				
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
						sensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR),
						200); //200ms
				
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
		
		if (location.getProvider().equals(LocationManager.GPS_PROVIDER) || !m_didReportLocation) {
			locationChanged(location.getAltitude(),location.getLatitude(),location.getLongitude(),location.getSpeed(),location.getBearing());
		
			m_didReportLocation = true;
		}
    }

	@Override
	public void onSensorChanged(SensorEvent event) {
  
		float[] rotationMatrix = new float[9];

		if (event.values.length > 4) {

			// On some Samsung devices SensorManager.getRotationMatrixFromVector
			// appears to throw an exception if rotation vector has length > 4.
			// For the purposes of this class the first 4 values of the
			// rotation vector are sufficient (see crbug.com/335298 for details).

			float[] truncatedValues = new float[4];

			System.arraycopy(event.values, 0, truncatedValues, 0, 4);

			SensorManager.getRotationMatrixFromVector(rotationMatrix, truncatedValues);
		}
		else {

			SensorManager.getRotationMatrixFromVector(rotationMatrix, event.values);
		}
		
		SensorManager.remapCoordinateSystem(rotationMatrix, SensorManager.AXIS_X, SensorManager.AXIS_Y, rotationMatrix);

		float[] orientation = new float[3];
		
		SensorManager.getOrientation(rotationMatrix, orientation);
	
		headingChanged(360 - Math.toDegrees(orientation[0]));
		
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
