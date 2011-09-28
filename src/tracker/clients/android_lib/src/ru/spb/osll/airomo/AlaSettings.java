package ru.spb.osll.airomo;

import ru.spb.osll.preferences.Settings;
import ru.spb.osll.preferences.Settings.ITrackerNetSettings;
import android.content.Context;

public class AlaSettings {
	private Settings m_settings; 
	
	public AlaSettings(Context c){
		m_settings = new Settings(c);
	}
	
	public void setLogin(String login){
		m_settings.setPreference(ITrackerNetSettings.LOGIN, login);	
	}
	
	public void setPass(String pass){
		m_settings.setPreference(ITrackerNetSettings.PASSWORD, pass);	
	}

	public void setServerUrl(String url){
		m_settings.setPreference(ITrackerNetSettings.SERVER_URL, url);
	}
		
	public void setChannel(String channel){
		m_settings.setPreference(ITrackerNetSettings.CHANNEL, channel);
	}
	
	public void setTrackInterval(int sec){
		m_settings.setPreference(ITrackerNetSettings.TIME_TICK, sec);
	}
	
	public void setHistorySize(int size){
		m_settings.setPreference(ITrackerNetSettings.HISTORY_LIMIT, size);
	}
	
	public String getLogin(){
		return m_settings.getPreference(ITrackerNetSettings.LOGIN, "?????");
	}
	
	public String getPass(){
		return m_settings.getPreference(ITrackerNetSettings.PASSWORD, "?????");
	}

	public String getServerUrl(){
		return m_settings.getPreference(ITrackerNetSettings.SERVER_URL, "?????");
	}
		
	public String getChannel(){
		return m_settings.getPreference(ITrackerNetSettings.CHANNEL, "?????");
	}
	
	public int getTrackInterval(){
		return m_settings.getPreference(ITrackerNetSettings.TIME_TICK, 5);
	}
	
	public int getHistorySize(){
		return m_settings.getPreference(ITrackerNetSettings.HISTORY_LIMIT, 50);
	}
}
