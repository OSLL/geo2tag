package ru.spb.osll.preferences;

import ru.spb.osll.json.IRequest;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;

public class Settings {
	private Context m_context;
	private static final String SETTINGS_STATUS = "settings_status"; 
	
	public Settings(Context context){
		m_context = context;
	}
	
	public SharedPreferences getPreferences(){
		return m_context.getSharedPreferences(ITrackerNetSettings.TRACKER_SETTINGS, 0);
	}
	
	public Editor getPreferencesEditor(){
		return getPreferences().edit();		
	}	
	
	public boolean isSettingsEmpty(){
		return getPreferences().getBoolean(SETTINGS_STATUS, true);
	}
	
	// TODO
	public void setDefaultSettrings(){
		Editor editor = getPreferencesEditor();

		editor.putString(ITrackerNetSettings.LOGIN, "Mark");	
		editor.putString(ITrackerNetSettings.PASSWORD, "test");
		editor.putString(ITrackerNetSettings.CHANNEL, "Test channel");
		editor.putString(ITrackerNetSettings.CHANNEL_KEY, "");
		editor.putString(ITrackerNetSettings.SERVER_URL, IRequest.BASE_URL);
		editor.putInt(ITrackerNetSettings.TIME_TICK, 5);
		
		editor.putBoolean(ITrackerAppSettings.IS_SHOW_TICKS, false);
		editor.putBoolean(ITrackerAppSettings.IS_HIDE_APP, true);
		
		editor.putBoolean(SETTINGS_STATUS, false);
		editor.commit();
	}
	
	public static SharedPreferences getPreferences(Context c){
		return new Settings(c).getPreferences();
	}
	
	public interface ITrackerNetSettings{
		String TRACKER_SETTINGS = "tracker_settings";
		
		String LOGIN = "login";
		String PASSWORD = "password";
		String CHANNEL = "channel";
		String CHANNEL_KEY = "channel_key";
	   	String SERVER_URL = "server_url";
	   	String TIME_TICK = "time_request";
		String RADIUS = "radius";
		String URI = "uri";
	}
	
	public interface ITrackerAppSettings{
		String IS_SHOW_TICKS = "is_show_ticks";
		String IS_HIDE_APP = "is_hide_app";
	}
}