package ru.spb.osll.services;

import ru.spb.osll.TrackerActivity;
import ru.spb.osll.airomo.Ala;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;

public class LocationService extends Service {
	private LocationManager m_locationManager;
	private static LocationService instance; 
	
	@Override
	public void onCreate() {
		super.onCreate();
		Log.v(Ala.ALA_LOG, "location service create");
		m_locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
	}

	@Override
	public void onStart(Intent intent, int startId) {
		super.onStart(intent, startId);
		Log.v(Ala.ALA_LOG, "location service start");
		instance = this;
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		Log.v(TrackerActivity.LOG, "location service destroy");
		m_locationManager.removeUpdates(locationListener);
	}
	
	public void startLocationListenerInternal(){
		m_locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 0, 0, locationListener);
	}

	public void stopLocationListenerInternal (){
		m_locationManager.removeUpdates(locationListener);
	}

	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}

	private LocationListener locationListener = new LocationListener() {
		public void onLocationChanged(Location location) {
			//if (location != null){
			//	Log.v(TrackerActivity.LOG, "onLocationChanged  " + location.toString());
			//}
		}

		public void onStatusChanged(String provider, int status, Bundle extras) {
			//Log.v(TrackerActivity.LOG, "onStatusChanged " + provider + " " + status + " " + extras);
		}

		public void onProviderEnabled(String provider) {
			//Log.v(TrackerActivity.LOG, "onProviderEnabled " + provider);
		}

		public void onProviderDisabled(String provider) {
			//Log.v(TrackerActivity.LOG, "onProviderDisabled " + provider);
		}
	};

	public static boolean startLocationListener(){
		if(instance != null){
			instance.startLocationListenerInternal();
			return true;
		}
		return false;
	}

	public static boolean stopLocationListener(){
		if(instance != null){
			instance.stopLocationListenerInternal();
			return true;
		}
		return false;
	}
	
	public static Location getLocation(){
		if(instance != null){
			return getLocation(instance);
		} else {
			return null;
		}
	}
	
	public static Location getLocation(Context context) {
		Location location = null;
		String provider = LocationManager.NETWORK_PROVIDER;
		LocationManager locationManager = (LocationManager) context.getSystemService(Context.LOCATION_SERVICE);

		if (locationManager != null) {
			if (locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER)) {
				provider = LocationManager.GPS_PROVIDER;
			}
			location = locationManager.getLastKnownLocation(provider);
		}
		return location;
	}
	
}
