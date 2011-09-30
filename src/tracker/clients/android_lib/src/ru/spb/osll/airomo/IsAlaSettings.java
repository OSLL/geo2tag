package ru.spb.osll.airomo;

public interface IsAlaSettings {
	public void setLogin(String login);
	public void setPass(String pass);
	public void setServerUrl(String url);
	public void setChannel(String channel);
	public void setTrackInterval(int sec);
	public void setHistoryLimit(int size);
	
	public String getLogin();
	public String getPass();
	public String getServerUrl();
	public String getChannel();
	public int getTrackInterval();
	public int getHistoryLimit();
}