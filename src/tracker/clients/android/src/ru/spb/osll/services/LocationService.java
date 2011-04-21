package ru.spb.osll.services;

import ru.spb.osll.TrackerActivity;
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
	private LocationManager myLocationManager;
	
	@Override
	public void onCreate() {
		super.onCreate();
		Log.v(TrackerActivity.LOG, "location service create");
	
		myLocationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
		myLocationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 0, 0, locationListener);
	}

	@Override
	public void onStart(Intent intent, int startId) {
		super.onStart(intent, startId);
		Log.v(TrackerActivity.LOG, "location service start");
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		Log.v(TrackerActivity.LOG, "location service destroy");
		myLocationManager.removeUpdates(locationListener);
	}

	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}

	private LocationListener locationListener = new LocationListener() {
		public void onLocationChanged(Location location) {
			if (location != null){
				Log.v(TrackerActivity.LOG, "onLocationChanged  " + location.toString());
			}
		}

		public void onStatusChanged(String provider, int status, Bundle extras) {
			Log.v(TrackerActivity.LOG, "onStatusChanged " + provider + " " + status + " " + extras);
		}

		public void onProviderEnabled(String provider) {
			//Log.v(TrackerActivity.LOG, "onProviderEnabled " + provider);
		}

		public void onProviderDisabled(String provider) {
			//Log.v(TrackerActivity.LOG, "onProviderDisabled " + provider);
		}
	};

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
