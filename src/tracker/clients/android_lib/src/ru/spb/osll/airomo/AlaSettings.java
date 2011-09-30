package ru.spb.osll.airomo;

import ru.spb.osll.airomo.BaseAlaService.InternalReceiver;
import ru.spb.osll.airomo.settings.Settings;
import ru.spb.osll.airomo.settings.Settings.ITrackerNetSettings;
import android.content.Context;
import android.content.ContextWrapper;
import android.content.Intent;

class AlaSettings implements IsAlaSettings {
	private Settings m_settings; 
	private Context  m_context;
	
	public AlaSettings(Context c){
		m_settings = new Settings(c);
		m_context = c;
	}
	
	public void setLogin(String login){
		m_settings.setPreference(ITrackerNetSettings.LOGIN, login);	
		settingsSignal();
	}
	
	public void setPass(String pass){
		m_settings.setPreference(ITrackerNetSettings.PASSWORD, pass);	
		settingsSignal();
	}

	public void setServerUrl(String url){
		m_settings.setPreference(ITrackerNetSettings.SERVER_URL, url);
		settingsSignal();
	}
		
	public void setChannel(String channel){
		m_settings.setPreference(ITrackerNetSettings.CHANNEL, channel);
		settingsSignal();
	}
	
	public void setTrackInterval(int sec){
		m_settings.setPreference(ITrackerNetSettings.TIME_TICK, sec);
		settingsSignal();
	}
	
	public void setHistoryLimit(int size){
		m_settings.setPreference(ITrackerNetSettings.HISTORY_LIMIT, size);
		settingsSignal();
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
	
	public int getHistoryLimit(){
		return m_settings.getPreference(ITrackerNetSettings.HISTORY_LIMIT, 50);
	}
	
	private void settingsSignal(){
		Intent intent = new Intent(InternalReceiver.ACTION);
		intent.putExtra(InternalReceiver.TYPE_SIGNAL, InternalReceiver.SIGNAL_UPDATE_SETTINGS);
		((ContextWrapper) m_context).sendBroadcast(intent);
	}
}
