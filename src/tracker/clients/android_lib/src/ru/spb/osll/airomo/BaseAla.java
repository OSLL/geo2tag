package ru.spb.osll.airomo;

import java.util.LinkedList;
import java.util.List;

import ru.spb.osll.error.AlaError;
import ru.spb.osll.preferences.Settings;
import ru.spb.osll.preferences.Settings.ITrackerNetSettings;
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
	private LocationState m_locationState;

	private Boolean m_isOnline = false;
	protected String m_serverUrl;
	
	public BaseAla(Context c) {
		m_locationState = new LocationState(c);
		initSettings(c);

		m_serverUrl = getPreference(ITrackerNetSettings.SERVER_URL, ""); 
		c.registerReceiver(new ConnectionChangeReceiver(), 
				new IntentFilter("android.net.conn.CONNECTIVITY_CHANGE"));
		c.registerReceiver(new ShutdownReceiver(), 
				new IntentFilter("android.intent.action.BATTERY_LOW"));
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
	
	protected Location getLocation() {
		return m_locationState.getLocation();
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
		m_locationState.startLocationListener();
	}

	public void stopLocationListener(){
		m_locationState.stopLocationListener();
	}
	
	protected abstract void networkStatusChanged(boolean isOnline);
	
	public class ConnectionChangeReceiver extends BroadcastReceiver {
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

	public class ShutdownReceiver extends BroadcastReceiver {
		@Override
		public void onReceive(Context context, Intent intent) {
			Log.v(ALA_LOG, "ShutdownReceiver ");
			sendLastCoordinate();
			sendHistory();
			for(GooffListener l : m_gooffListeners){
				l.gooff();
			}
		}
	}	

}
