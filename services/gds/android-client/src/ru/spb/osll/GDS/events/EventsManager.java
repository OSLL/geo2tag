package ru.spb.osll.GDS.events;

import ru.spb.osll.GDS.utils.GDSUtil;
import android.content.Context;
import android.content.Intent;

public class EventsManager  {
	
	public static final String LOG = "GDS_Events_service";
	public static final String AUTH_TOKEN = "auth_token";
	public static final String EVENTS_CHANNEL = "Events";
	public static final int INTERVAL = 15;
	public static final int RADIUS = 30;
	public static final int RELEVANT_PERIOD_IN_HOURS = 240;
	
	private String m_authToken;
	
	public EventsManager() {
		m_authToken = "";
	}
	
	public EventsManager(String authToken) {
		m_authToken = authToken;
	}
	
	public void setData(String authToken) {
		m_authToken = authToken;
	}
	
	public void startEventsService(Context c) {
		Intent i = new Intent(c, EventsService.class);
		i.putExtra(AUTH_TOKEN, m_authToken);	
		c.startService(i);
	}
	
	public void stopEventsService(Context c) {
		c.stopService(new Intent(c, EventsService.class));
	}
	
	public boolean isEventsServieRunning(Context c) {
		return GDSUtil.isServiceRunning(c, EventsService.class.getName());
	}

}