package ru.spb.osll.airomo;

import ru.spb.osll.TrackerActivity;
import android.content.Context;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.util.Log;

public class LocationState {
	
	private LocationManager m_locationManager;
	private boolean isWorking = false;
	private Context m_context;			// FIXME Context!!!
	
	public LocationState(Context context) {
		Log.v(TrackerActivity.LOG, "LocationState create");
		m_context = context;
		m_locationManager = (LocationManager) context.getSystemService(Context.LOCATION_SERVICE);
	}

	public void startLocationListener(){
		if (! isWorking) {
			isWorking = true;
			m_locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 0, 0, locationListener);
		}
	}
	
	public void stopLocationListener(){
		if (isWorking) {
			isWorking = false;
			m_locationManager.removeUpdates(locationListener);
		}
	}
	
	public Location getLocation(){
		return getLocation(m_context);
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
