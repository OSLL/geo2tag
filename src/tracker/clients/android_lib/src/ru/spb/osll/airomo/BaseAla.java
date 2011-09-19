package ru.spb.osll.airomo;

import java.util.Date;
import java.util.LinkedList;
import java.util.List;

import ru.spb.osll.error.AlaError;
import ru.spb.osll.objects.Mark;
import ru.spb.osll.preferences.Settings;
import ru.spb.osll.preferences.Settings.ITrackerNetSettings;
import ru.spb.osll.services.LocationService;
import ru.spb.osll.utils.TrackerUtil;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.location.Location;
import android.util.Log;

abstract class BaseAla implements IsAla {
	public static final String ALA_LOG = "AlaLog";
	
	private List<NetworkListener> m_netListeners = new LinkedList<NetworkListener>();
	private List<GooffListener> m_gooffListeners = new LinkedList<GooffListener>();
	private List<ErrorListener> m_errorListeners = new LinkedList<ErrorListener>();
	private SharedPreferences m_preferences;
	private Editor m_preferencesEditor;

	private Boolean m_isOnline = false;
	private ConnectionData m_connectData;

	
	public BaseAla(Context c) {
		initSettings(c);
		refreshConnectionData();
		
		Log.v(ALA_LOG, "BaseAla was created...");
		c.startService(new Intent(c, LocationService.class));
		c.registerReceiver(m_networkReceiver, new IntentFilter("android.net.conn.CONNECTIVITY_CHANGE"));
		c.registerReceiver(m_shutdownReceiver, new IntentFilter("android.intent.action.BATTERY_LOW"));
	}

	public void onDestroy(Context c) {
		Log.v(ALA_LOG, "Base Ala - onDestroy...");
		c.stopService(new Intent(c, LocationService.class));
		c.unregisterReceiver(m_networkReceiver);
		c.unregisterReceiver(m_shutdownReceiver);
	}
	
	@Override
	protected void finalize() throws Throwable {
		super.finalize();
	}


	private void initSettings(Context c) {
		final Settings settings = new Settings(c);
		if (settings.isSettingsEmpty()) {
			settings.setDefaultSettrings();
		}
		m_preferences = settings.getPreferences();
		m_preferencesEditor = settings.getPreferencesEditor();
	}
	
	protected void setPreference(String key, int val) {
		m_preferencesEditor.putInt(key, val);
		m_preferencesEditor.commit();
	}

	protected int getPreference(String key, int defVal) {
		return m_preferences.getInt(key, defVal);
	}

	protected void setPreference(String key, String val) {
		m_preferencesEditor.putString(key, val);
		m_preferencesEditor.commit();
	}

	protected String getPreference(String key, String defVal) {
		return m_preferences.getString(key, defVal);
	}
	
	protected void completeMark(Mark mark, String authToken, String channel){
		mark.setAuthToken(authToken);
		mark.setChannel(channel);
	}

	protected Mark constructDruftMark(){
		double latitude 	= 0.0;
		double longitude 	= 0.0;
	
		Location location = LocationService.getLocation();
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
		return mark;
	}


	// ------------------- IsAla ---------------------
	@Override
	public void addGooffListener(GooffListener l) {
		m_gooffListeners.add(l);
	}

	@Override
	public void addOnNetworkListener(NetworkListener l) {
		m_netListeners.add(l);
	}

	@Override
	public void addErrorListener(ErrorListener l) {
		m_errorListeners.add(l);
	}

	private String m_lastError;
	@Override
	public void onErrorOccured(String error) {
		m_lastError = error;
		for(ErrorListener l : m_errorListeners){
			l.onError(new AlaError(error));
		}
	}

	@Override
	public String getLastError() {
		return m_lastError;
	}
	
	@Override
	public synchronized boolean isOnline() {
		return m_isOnline;
	}
	
	// ---------------- IsAla extension ------------------
	public void startLocationListener(){
		LocationService.startLocationListener();
	}

	public void stopLocationListener(){
		LocationService.stopLocationListener();
	}
	
	
	// ---------------- for lesteners ... ----------------
	protected abstract void networkStatusChanged(boolean isOnline);
	private ConnectionChangeReceiver m_networkReceiver = new ConnectionChangeReceiver();
	private class ConnectionChangeReceiver extends BroadcastReceiver {
		@Override
		public void onReceive(Context context, Intent intent) {
			final boolean isOnline = TrackerUtil.isOnline(context);
			synchronized (m_isOnline){
				m_isOnline = isOnline;
			}
			networkStatusChanged(isOnline);
			for(NetworkListener l : m_netListeners){
				l.networkChanged(isOnline);
			}
			Log.v(ALA_LOG, "ConnectionChangeReceiver " + isOnline);
		}
	}	

	protected abstract void gooffEvent();
	private ShutdownReceiver m_shutdownReceiver = new ShutdownReceiver();
	private class ShutdownReceiver extends BroadcastReceiver {
		@Override
		public void onReceive(Context context, Intent intent) {
			Log.v(ALA_LOG, "ShutdownReceiver ");
			gooffEvent();
			for(GooffListener l : m_gooffListeners){
				l.gooff();
			}
		}
	}	
	
	class ConnectionData {
		public String login = getPreference(ITrackerNetSettings.LOGIN, "");
		public String pass = getPreference(ITrackerNetSettings.PASSWORD, "");
		public String channel = getPreference(ITrackerNetSettings.CHANNEL, "");
		public String serverUrl = getPreference(ITrackerNetSettings.SERVER_URL, "");
	}
	
	protected void refreshConnectionData(){
		m_connectData = new ConnectionData();
	}

	protected ConnectionData netData(){
		return m_connectData;
	}
}
