package cc.openframeworks;

import android.content.Context;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;

public class OFAndroidGPS extends OFAndroidObject implements LocationListener {
	
	private static OFAndroidGPS m_instance = null;
	
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
				started = true;
			}
		});
	}
	
	void stopGPS(){

		OFAndroidObject.activity.runOnUiThread(new Runnable(){
			public void run(){
				// Acquire a reference to the system Location Manager
				LocationManager locationManager = (LocationManager) OFAndroidObject.activity.getSystemService(Context.LOCATION_SERVICE);
		
				locationManager.removeUpdates(OFAndroidGPS.this);
				
				started = false;
			}
		});
	}
	

    public void onLocationChanged(Location location) {
      // Called when a new location is found by the network location provider.
      //makeUseOfNewLocation(location);
    	locationChanged(location.getAltitude(),location.getLatitude(),location.getLongitude(),location.getSpeed(),location.getBearing());
    }


    public void onProviderEnabled(String provider) {}

    public void onProviderDisabled(String provider) {}

	public void onStatusChanged(String provider, int status, Bundle extras) {
		// TODO Auto-generated method stub
		
	}
	
	
	public static native void locationChanged(double altitude, double latitude, double longitude, float speed, float bearing);
	
	private boolean started;

	@Override
	protected void appPause() {
		boolean wasstarted = started;
		stopGPS();
		started = wasstarted;
	}

	@Override
	protected void appResume() {
		if(started)
			startGPS();
	}

	@Override
	protected void appStop() {
		boolean wasstarted = started;
		stopGPS();
		started = wasstarted;
	}
}
