package ru.spb.osll.airomo;

import java.util.Date;

import ru.spb.osll.ala.AlaReceiver;
import ru.spb.osll.objects.Mark;
import ru.spb.osll.preferences.Settings;
import ru.spb.osll.preferences.Settings.ITrackerNetSettings;
import ru.spb.osll.utils.TrackerUtil;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;
 
abstract class BaseAlaService extends Service {
	private LocationManager m_locationManager;
	
	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}
	
	@Override
	public void onCreate() {
		super.onCreate();
		Log.v(Ala.ALA_LOG, "BaseAlaService create");
		m_locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
		m_locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 0, 0, locationListener);
	}

	@Override
	public void onStart(Intent intent, int startId) {
		super.onStart(intent, startId);
		m_isDeviceReady = false;
		Log.v(Ala.ALA_LOG, "BaseAlaService start");
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		Log.v(Ala.ALA_LOG, "BaseAlaService destroy");
		m_isDeviceReady = false;
		m_locationManager.removeUpdates(locationListener);
	}
	
	protected abstract void onLocationDeviceStatusChanged(boolean isReady);
	
	private boolean m_isDeviceReady = false;
	private LocationListener locationListener = new LocationListener() {
		public void onLocationChanged(Location location) {
			if (!m_isDeviceReady && location != null){
				m_isDeviceReady = true;
				onLocationDeviceStatusChanged(true);
			} else if (m_isDeviceReady && location == null) {
				m_isDeviceReady = false;
				onLocationDeviceStatusChanged(false);
			}
		}
		public void onStatusChanged(String provider, int status, Bundle extras) {}
		public void onProviderEnabled(String provider) {}
		public void onProviderDisabled(String provider) {}
	};
	
	public Location getLocation() {
		Location location = null;
		String provider = LocationManager.NETWORK_PROVIDER;
		LocationManager locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);

		if (locationManager != null) {
			if (locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER)) {
				provider = LocationManager.GPS_PROVIDER;
			}
			location = locationManager.getLastKnownLocation(provider);
		}
		return location;
	}

	// ------------------------------------------------------------
	protected abstract void networkStatusChanged(boolean isOnline);

	private Boolean m_isOnline = false;
	private ConnectionChangeReceiver m_networkReceiver = new ConnectionChangeReceiver();
	private class ConnectionChangeReceiver extends BroadcastReceiver {
		@Override
		public void onReceive(Context context, Intent intent) {
			final boolean isOnline = TrackerUtil.isOnline(context);
			synchronized (m_isOnline){
				m_isOnline = isOnline;
			}
			networkStatusChanged(isOnline);
			Log.v(Ala.ALA_LOG, "ConnectionChangeReceiver " + isOnline);
		}
	}	

	protected abstract void gooffEvent();
	private ShutdownReceiver m_shutdownReceiver = new ShutdownReceiver();
	private class ShutdownReceiver extends BroadcastReceiver {
		@Override
		public void onReceive(Context context, Intent intent) {
			Log.v(Ala.ALA_LOG, "ShutdownReceiver ");
			gooffEvent();
		}
	}	

	// ------------------------------------------------------------
	private ConnectionData m_connectData;
	
	class ConnectionData {
		public String login = getPreference(ITrackerNetSettings.LOGIN, "");
		public String pass = getPreference(ITrackerNetSettings.PASSWORD, "");
		public String channel = getPreference(ITrackerNetSettings.CHANNEL, "");
		public String serverUrl = getPreference(ITrackerNetSettings.SERVER_URL, "");
	}

	Settings m_settings = new Settings(this);
	private String getPreference(String key, String defVal) {
		return m_settings.getPreferences().getString(key, defVal);
	}
	
	protected void refreshConnectionData(){
		m_connectData = new ConnectionData();
	}

	protected ConnectionData netData(){
		return m_connectData;
	}
	
	protected Mark constructDruftMark(){
		double latitude 	= 0.0;
		double longitude 	= 0.0;
	
		Location location = getLocation();
		if (location == null){
			//return; //FIXME
		} else {
			latitude = location.getLatitude();
			longitude = location.getLongitude();
		}
		Mark mark = new Mark();
		mark.setTitle("title");
		mark.setLink("unknown");
		mark.setDescription("this tag was generated automaticaly by tracker application");
		mark.setLatitude(latitude);
		mark.setLongitude(longitude);
		mark.setTime(TrackerUtil.getTime(new Date()));
		broadcastMark(mark); 
		return mark;
	}
	
	protected void completeMark(Mark mark, String authToken, String channel){
		mark.setAuthToken(authToken);
		mark.setChannel(channel);
	}
	
	protected boolean isOnline(){
		return false;		// TODO	
	}
	
	protected void onErrorOccured(String error){
		// TODO
	}
	
	// TODO move later
	private void broadcastMark(Mark mark){
		Intent intent = new Intent(AlaReceiver.ACTION_ALA);
		intent.putExtra(AlaReceiver.TYPE_OPERATION, AlaReceiver.TYPE_MARK);
		intent.putExtra(AlaReceiver.LONLAT, "set : " + TrackerUtil.convertLocation(mark));
		sendBroadcast(intent);
	}	

}
