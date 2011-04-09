package ru.spb.osll.preferences;

import ru.spb.osll.TrackerActivity;
import ru.spb.osll.json.IRequest;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.util.Log;

public class Settings {
	private Context m_context;
	private static final String SETTINGS_STATUS = "settings_status"; 
	
	public Settings(Context context){
		m_context = context;
	}
	
	public SharedPreferences getPreferences(){
		return m_context.getSharedPreferences(ITrackerSettings.TRAKER_SETTINGS, 0);
	}
	
	public Editor getPreferencesEditor(){
		return getPreferences().edit();		
	}	
	
	public boolean isSettingsEmpty(){
		return getPreferences().getBoolean(SETTINGS_STATUS, true);
	}
	
	// TODO
	public void setDefaultSettrings(){
		Log.v(TrackerActivity.LOG, "setDefaultSettrings()");
		
		Editor editor = getPreferencesEditor();
		editor.putString(ITrackerSettings.LOGIN, "Mark");	
		editor.putString(ITrackerSettings.PASSWORD, "test");
		editor.putString(ITrackerSettings.CHANNEL, "Test channel");
		editor.putString(ITrackerSettings.CHANNEL_KEY, "");
		editor.putString(ITrackerSettings.SERVER_URL, IRequest.BASE_URL);
		
		editor.putBoolean(SETTINGS_STATUS, false);
		editor.commit();
	}
	
	public interface ITrackerSettings{
		String TRAKER_SETTINGS = "tracker_settings";
		
		String LOGIN = "login";
		String PASSWORD = "password";
		String CHANNEL = "channel";
		String CHANNEL_KEY = "channel_key";
	   	String SERVER_URL = "server_url";
	   	String TIME_TICK = "time_request";
		String RADIUS = "radius";
		String URI = "uri";
	}
}