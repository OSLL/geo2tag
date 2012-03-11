package ru.spb.osll.GDS.events;

import java.util.Calendar;
import java.util.Date;
import java.util.List;

import org.json.JSONObject;

import ru.spb.osll.GDS.LocationService;
import ru.spb.osll.GDS.exception.ExceptionHandler;
import ru.spb.osll.GDS.preferences.Settings;
import ru.spb.osll.GDS.preferences.Settings.IGDSSettings;
import ru.spb.osll.GDS.utils.GDSUtil;
import ru.spb.osll.json.IRequest.IResponse;
import ru.spb.osll.json.JsonFilterCircleRequest;
import ru.spb.osll.json.JsonFilterResponse;
import ru.spb.osll.objects.Channel;
import ru.spb.osll.objects.Mark;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.location.Location;
import android.os.Bundle;
import android.os.IBinder;
import android.os.SystemClock;
import android.util.Log;

public class EventsService extends Service {
	
	private String m_authToken = null;
	private Thread m_eventsThread;
	private InternalReceiver m_internalReceiver = new InternalReceiver();
	private Settings m_settings;

	@Override
	public IBinder onBind(Intent arg0) {
		return null;
	}
	
	@Override
	public void onCreate() {
		Log.v(EventsManager.LOG, "EventsService create");
		
		super.onCreate();
		Thread.setDefaultUncaughtExceptionHandler(new ExceptionHandler(this));

		m_settings = new Settings(this);
		registerReceiver(m_internalReceiver, new IntentFilter(InternalReceiver.ACTION));
		LocationService.getLocation(this);
	}

	@Override
	public void onStart(Intent intent, int startId) {
		Log.v(EventsManager.LOG, "EventsService start");
		super.onStart(intent, startId);
		
		Bundle extras =intent.getExtras();
		if (extras != null) {
		    m_authToken = extras.getString(EventsManager.AUTH_TOKEN);
		}
		if (m_authToken == null) {
			Log.v(EventsManager.LOG, "problem with extracting data");
			broadcastError("Failed to start tracking");
			stopSelf();
			return;
		}
		
		startEventsThread();
	}

	@Override
	public void onDestroy() {
		Log.v(EventsManager.LOG, "EventsService destroy");
		super.onDestroy();
		stopEventsTread();
		unregisterReceiver(m_internalReceiver);
	}
	
	protected void stopEventsTread(){
		if (m_eventsThread != null){
			m_eventsThread.interrupt();
		}
	}
	
	protected void startEventsThread(){
		if (m_eventsThread != null){
			m_eventsThread.interrupt();
		}
		m_eventsThread = new Thread(new Runnable() {
			@Override
			public void run() {
				while (!Thread.currentThread().isInterrupted()){
					Location location = LocationService.getLocation(EventsService.this);
					if (location == null) {
						Log.v(EventsManager.LOG, "can't get location");
					} else {
						Log.v(EventsManager.LOG, "coords: " + location.getLatitude()
								+ ", " + location.getLongitude());
						requestEvents(location);
					}
					
					SystemClock.sleep(EventsManager.INTERVAL * 1000);
				}
			}
		});
		m_eventsThread.start();
	}
	
	private void requestEvents(Location location) {
		String serverUrl = m_settings.getServerUrl();
		JSONObject JSONResponse = null;
		for(int i = 0; i < IGDSSettings.ATTEMPTS; i++){
			Calendar calendar = Calendar.getInstance();
			calendar.setTime(new Date());
			calendar.add(Calendar.YEAR, 1); // just in case
			String timeTo = GDSUtil.getTime(calendar.getTime());
			calendar.add(Calendar.YEAR, -1); // return current date
			calendar.add(Calendar.HOUR, - EventsManager.RELEVANT_PERIOD_IN_HOURS);
			String timeFrom = GDSUtil.getTime(calendar.getTime());
			JSONResponse = new JsonFilterCircleRequest(m_authToken,
					location.getLatitude(), location.getLongitude(),
					EventsManager.RADIUS, timeFrom, timeTo,
					serverUrl).doRequest();
			if (JSONResponse != null) 
				break;
		}
		if (JSONResponse != null) {
			int errno = JsonFilterResponse.parseErrno(JSONResponse);
			if (errno == IResponse.geo2tagError.SUCCESS.ordinal()) {
				Log.v(EventsManager.LOG, "Events received successfully");
				JsonFilterResponse response = new JsonFilterResponse();
				response.parseJson(JSONResponse);
				List<Channel> channels = response.getChannelsData();
				for (Channel channel : channels) {
					if (channel.getName().compareTo("Events") == 0) {
						broadcastEvents(channel);
					}
				}
			} else {
				handleError(errno);
				return;
			}
		} else {
			Log.v(EventsManager.LOG, "response failed");
			broadcastError("Failed to send location");
			return;
		}
	}
	
	private void handleError(int errno) {
		if (errno < 0) {
			Log.v(EventsManager.LOG, "bad response received");
		} else if (errno >= IResponse.geo2tagError.values().length) {
			Log.v(EventsManager.LOG, "unknown error");
		} else if (errno > 0) {
			String error = IResponse.geo2tagError.values()[errno].name();
			Log.v(EventsManager.LOG, "error: " + error);
		}
		broadcastError("Failed to send location");
	}
	
	
	
	public class InternalReceiver extends BroadcastReceiver {
		public static final String ACTION 	= "osll.gds.events.internal";
		public static final String TYPE_SIGNAL	= "osll.gds.signal";

		public static final int SIGNAL_UPDATE_SETTINGS	= 0;
		
		@Override
		public void onReceive(Context context, Intent intent) {
			int type = intent.getIntExtra(TYPE_SIGNAL, -1);
			switch (type) {
			case SIGNAL_UPDATE_SETTINGS:
				//refreshSCache();
				//onSettingUpdated();
				break;
			}
		}
	}
	
	private void broadcastError(String error) {
		Intent intent = new Intent(EventsReceiver.ACTION_EVENTS);
		intent.putExtra(EventsReceiver.TYPE_OPERATION, EventsReceiver.TYPE_ERROR);
		intent.putExtra(EventsReceiver.ERROR, error);
		sendBroadcast(intent);
	}
	
	private void broadcastEvents(Channel eventsChannel) {
		Intent intent = new Intent(EventsReceiver.ACTION_EVENTS);
		intent.putExtra(EventsReceiver.TYPE_OPERATION, EventsReceiver.TYPE_EVENTS);
		Mark[] marks = eventsChannel.getMarks().toArray(new Mark[0]);
		intent.putExtra(EventsReceiver.EVENTS, marks);
		sendBroadcast(intent);
	}

}