package ru.spb.osll.GDS.events;

import ru.spb.osll.GDS.GDSUtil;
import android.content.Context;
import android.content.Intent;

public class EventsManager  {
	
	public static final String LOG = "GDS_Events_service";
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
		i.putExtra(GDSUtil.AUTH_TOKEN, m_authToken);	
		c.startService(i);
	}
	
	public void stopEventsService(Context c) {
		c.stopService(new Intent(c, EventsService.class));
	}
	
	public boolean isEventsServieRunning(Context c) {
		return GDSUtil.isServiceRunning(c, EventsService.class.getName());
	}
	
	public void requestEvents(Context c) {
		Intent intent = new Intent(EventsService.InternalReceiver.ACTION);
		intent.putExtra(EventsService.InternalReceiver.TYPE_SIGNAL,
				EventsService.InternalReceiver.SIGNAL_SEND_MARKS);
		c.sendBroadcast(intent);
	}

}