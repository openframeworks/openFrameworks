package cc.openframeworks;

import android.app.Activity;
import android.content.Context;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;

public class OFAndroidGPS extends OFAndroidObject implements LocationListener {
	OFAndroidGPS(Activity ofActivity){
		this.ofActivity = ofActivity;
	}
	
	void startGPS(){
		final OFAndroidGPS gps = this;
		ofActivity.runOnUiThread(new Runnable(){
			public void run(){
				// Acquire a reference to the system Location Manager
				LocationManager locationManager = (LocationManager) ofActivity.getSystemService(Context.LOCATION_SERVICE);
		
		
				// Register the listener with the Location Manager to receive location updates
				locationManager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER, 0, 0, gps);
				
				// Register the listener with the Location Manager to receive location updates
				locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 0, 0, gps);
				
				Location lastLocation = locationManager.getLastKnownLocation(LocationManager.GPS_PROVIDER);
				if(lastLocation!=null)
					onLocationChanged(lastLocation);
				started = true;
			}
		});
		
	}
	
	void stopGPS(){

		final OFAndroidGPS gps = this;
		ofActivity.runOnUiThread(new Runnable(){
			public void run(){
				// Acquire a reference to the system Location Manager
				LocationManager locationManager = (LocationManager) ofActivity.getSystemService(Context.LOCATION_SERVICE);
		
				locationManager.removeUpdates(gps);
				
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
	
	private Activity ofActivity;
	private boolean started;

	@Override
	protected void pause() {
		boolean wasstarted = started;
		stopGPS();
		started = wasstarted;
	}

	@Override
	protected void resume() {
		if(started)
			startGPS();
	}

	@Override
	protected void stop() {
		boolean wasstarted = started;
		stopGPS();
		started = wasstarted;
	}
}
