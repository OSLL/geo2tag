package ru.spb.osll.GDS.tracking;

import java.util.Date;

import org.json.JSONObject;

import ru.spb.osll.GDS.GDSUtil;
import ru.spb.osll.GDS.exception.ExceptionHandler;
import ru.spb.osll.GDS.preferences.Settings;
import ru.spb.osll.json.JsonApplyMarkRequest;
import ru.spb.osll.json.JsonBaseResponse;
import ru.spb.osll.json.IRequest.IResponse;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.IBinder;
import android.os.SystemClock;
import android.util.Log;

public class TrackingService extends Service {
	
	private String m_authToken = null;
	private String m_channel = null;
	private LocationManager m_locationManager;
	private boolean m_isDeviceReady = false;
	private Thread m_trackThread;
	private InternalReceiver m_internalReceiver = new InternalReceiver();
	private Settings m_settings;

	@Override
	public IBinder onBind(Intent arg0) {
		return null;
	}
	
	@Override
	public void onCreate() {
		if (GDSUtil.DEBUG) {
			Log.v(TrackingManager.LOG, "TrackingService create");
		}
		
		super.onCreate();
		Thread.setDefaultUncaughtExceptionHandler(new ExceptionHandler(this));

		m_settings = new Settings(this);
		m_locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
		m_locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 0, 0, m_locationListener);
		registerReceiver(m_internalReceiver, new IntentFilter(InternalReceiver.ACTION));
		getLocation();
	}

	@Override
	public void onStart(Intent intent, int startId) {
		if (GDSUtil.DEBUG) {
			Log.v(TrackingManager.LOG, "TrackingService start");
		}
		super.onStart(intent, startId);
		m_isDeviceReady = false; // TODO check device
		
		Bundle extras =intent.getExtras();
		if (extras != null) {
		    m_authToken = extras.getString(GDSUtil.AUTH_TOKEN);
		    m_channel = extras.getString(GDSUtil.CHANNEL); 
		}
		if (m_authToken == null || m_channel == null) {
			if (GDSUtil.DEBUG) {
				Log.v(TrackingManager.LOG, "problem with extracting data");
			}
			broadcastError("Failed to start tracking");
			stopSelf();
			return;
		}
		
		startTracking(); // TODO for testing on AVD
	}

	@Override
	public void onDestroy() {
		if (GDSUtil.DEBUG) {
			Log.v(TrackingManager.LOG, "TrackingService destroy");
		}
		super.onDestroy();
	
		stopTracking();		
		m_locationManager.removeUpdates(m_locationListener);
		unregisterReceiver(m_internalReceiver);
	}
	
	private LocationListener m_locationListener = new LocationListener() {
		public void onLocationChanged(Location location) {
			if (!m_isDeviceReady && location != null){
				m_isDeviceReady = true;
				onLocationDeviceStatusChanged(true);
			} else if (m_isDeviceReady && location == null) {
				m_isDeviceReady = false;
				onLocationDeviceStatusChanged(false);
			}
		}
		public void onStatusChanged(String provider, int status, Bundle extras) {}
		public void onProviderEnabled(String provider) {}
		public void onProviderDisabled(String provider) {}
	};
	
	public Location getLocation() {
		Location location = null;
		String provider = LocationManager.NETWORK_PROVIDER;
		LocationManager locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);

		if (locationManager != null) {
			if (locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER)) {
				provider = LocationManager.GPS_PROVIDER;
			}
			location = locationManager.getLastKnownLocation(provider);
		}
		return location;
	}
	
	protected void onLocationDeviceStatusChanged(boolean isReady) {
		if (GDSUtil.DEBUG) {
			Log.v(TrackingManager.LOG, "onLocationDeviceStatusChanged: " + isReady);
		}
		if (isReady) {
			startTracking();
		} else {
			stopTracking();
		}
	}
	
	protected void stopTracking(){
		if (m_trackThread != null){
			m_trackThread.interrupt();
		}
	}
	
	protected void startTracking(){
		if (m_trackThread != null){
			m_trackThread.interrupt();
		}
		m_trackThread = new Thread(new Runnable() {
			@Override
			public void run() {
				while (!Thread.currentThread().isInterrupted()){
					Location location = getLocation();
					if (GDSUtil.DEBUG) {
						Log.v(TrackingManager.LOG, "coords: " + location.getLatitude()
							+ ", " + location.getLongitude());
					}
					
					sendMark(location);
					
					SystemClock.sleep(m_settings.getTrackingPeriod() * 60 * 1000);
				}
			}
		});
		m_trackThread.start();
	}
	
	private void sendMark(Location location) {
		String serverUrl = m_settings.getServerUrl();
		JSONObject JSONResponse = null;
		for(int i = 0; i < GDSUtil.ATTEMPTS; i++){
			JSONResponse = new JsonApplyMarkRequest(m_authToken, m_channel, "gds tracker", "",
					"gds tracker", location.getLatitude(), location.getLongitude(), 0,
					GDSUtil.getUtcTime(new Date()), serverUrl).doRequest();
			if (JSONResponse != null) 
				break;
		}
		if (JSONResponse != null) {
			int errno = JsonBaseResponse.parseErrno(JSONResponse);
			if (errno == IResponse.geo2tagError.SUCCESS.ordinal()) {
				if (GDSUtil.DEBUG) {
					Log.v(TrackingManager.LOG, "Mark sent successfully");
				}
				broadcastMarkSent(location);
			} else {
				handleError(errno);
				return;
			}
		} else {
			if (GDSUtil.DEBUG) {
				Log.v(TrackingManager.LOG, "response failed");
			}
			broadcastError("Failed to send location");
			return;
		}
	}
	
	private void handleError(int errno) {
		if (errno < 0) {
			if (GDSUtil.DEBUG) {
				Log.v(TrackingManager.LOG, "bad response received");
			}
		} else if (errno >= IResponse.geo2tagError.values().length) {
			if (GDSUtil.DEBUG) {
				Log.v(TrackingManager.LOG, "unknown error");
			}
		} else if (errno > 0) {
			String error = IResponse.geo2tagError.values()[errno].name();
			if (GDSUtil.DEBUG) {
				Log.v(TrackingManager.LOG, "error: " + error);
			}
		}
		broadcastError("Failed to send location");
	}
	
	
	
	public class InternalReceiver extends BroadcastReceiver {
		public static final String ACTION 	= "osll.gds.tracking.internal";
		public static final String TYPE_SIGNAL	= "osll.gds.signal";

		public static final int SIGNAL_UPDATE_SETTINGS	= 0;
		public static final int SIGNAL_SEND_HISTORY		= 1;
		public static final int SIGNAL_SEND_COORDINATE	= 2;
		
		@Override
		public void onReceive(Context context, Intent intent) {
			int type = intent.getIntExtra(TYPE_SIGNAL, -1);
			switch (type) {
			case SIGNAL_UPDATE_SETTINGS:
				//refreshSCache();
				//onSettingUpdated();
				break;
			case SIGNAL_SEND_HISTORY:
				//refreshSCache();
				//sendHistory();
				break;
			case SIGNAL_SEND_COORDINATE:
				//refreshSCache();
				//sendLastCoordinate();
				break;
			}
		}
	}
	
	private void broadcastError(String error) {
		Intent intent = new Intent(TrackingReceiver.ACTION_TRACKING);
		intent.putExtra(TrackingReceiver.TYPE_OPERATION, TrackingReceiver.TYPE_ERROR);
		intent.putExtra(TrackingReceiver.ERROR, error);
		sendBroadcast(intent);
	}
	
	private void broadcastMarkSent(Location location) {
		Intent intent = new Intent(TrackingReceiver.ACTION_TRACKING);
		intent.putExtra(TrackingReceiver.TYPE_OPERATION, TrackingReceiver.TYPE_MARK_SENT);
		intent.putExtra(TrackingReceiver.LONLAT,  GDSUtil.convertLocation(location));
		sendBroadcast(intent);
	}

}
