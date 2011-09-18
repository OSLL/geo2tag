package ru.spb.osll.airomo;

import java.util.Date;
import java.util.List;

import org.json.JSONObject;

import ru.spb.osll.json.IRequest.IResponse;
import ru.spb.osll.json.JsonApplyChannelRequest;
import ru.spb.osll.json.JsonApplyMarkRequest;
import ru.spb.osll.json.JsonBase;
import ru.spb.osll.json.JsonLoginRequest;
import ru.spb.osll.objects.Mark;
import ru.spb.osll.preferences.Settings.ITrackerNetSettings;
import ru.spb.osll.utils.TrackerUtil;
import android.content.Context;
import android.location.Location;
import android.util.Log;

public class Ala extends BaseAla {
	private boolean m_isWorking = false;
	private int m_trackInterval;
	private Buffer<Mark> m_history;
	
	public Ala(Context c) {
		super(c);
		m_trackInterval = getTrackInterval();
		m_history = new Buffer<Mark>(getHistoryLimit());
	}
	
	@Override
	public void setUserData(String login, String pass){
		setPreference(ITrackerNetSettings.LOGIN, login);
		setPreference(ITrackerNetSettings.PASSWORD, pass);
	}

	@Override
	public String auth(String login, String pass) {
		String authToken = null;
		JSONObject JSONResponse = new JsonLoginRequest(login, pass, m_serverUrl).doRequest();
		if (JSONResponse != null) {
//			String status = JsonBase.getString(JSONResponse, IResponse.STATUS);
//			String statusDescription = JsonBase.getString(JSONResponse, IResponse.STATUS_DESCRIPTION);
			authToken = JsonBase.getString(JSONResponse, IResponse.AUTH_TOKEN);
		} else {
			onErrorOccured(TrackerUtil.MESS_FAIL_CONNECTION_TO_SERVER);
		}
		return authToken;
	}

	@Override
	public void startTrack() {
		m_isWorking = true;
		startLocationListener();
		new Thread(m_doTracking).start();
	}

	@Override
	public void stopTrack() {
		m_isWorking = false;
		stopLocationListener();
	}

	@Override
	public boolean isTracking() {
		return m_isWorking;
	}

	@Override
	public void setTrackInterval(int sec) {
		m_trackInterval = sec;
		setPreference(ITrackerNetSettings.TIME_TICK, sec);
	}

	@Override
	public int getTrackInterval() {
		return getPreference(ITrackerNetSettings.TIME_TICK, 5);
	}

	@Override
	public void setHistoryLimit(int maxMarks) {
		m_history.setBufferSize(maxMarks);
		setPreference(ITrackerNetSettings.HISTORY_LIMIT, maxMarks);
	}

	@Override
	public int getHistoryLimit() {
		return getPreference(ITrackerNetSettings.HISTORY_LIMIT, 50);
	}

	
	@Override
	protected void networkStatusChanged(boolean isOnline) {
		if(!isOnline){
			dropCache();
		} else {
			sendHistory();
		}
	}

	private String authTokenCache = null;
	private boolean isChanAvailableCache = false;
	private void dropCache(){
		authTokenCache = null;
		isChanAvailableCache = false;
	}
	
	@Override
	public void sendHistory() {
		new Thread(new Runnable() {
			@Override
			public void run() {
				final String login    =  getPreference(ITrackerNetSettings.LOGIN, "");
				final String pass   = getPreference(ITrackerNetSettings.PASSWORD, "");
				final String channel = getPreference(ITrackerNetSettings.CHANNEL, "");
				while(isOnline() && m_history.hasElements()){
					Log.v(ALA_LOG, "sending history...");
					if (authTokenCache == null){
						authTokenCache = auth(login, pass);
						isChanAvailableCache = applyChannel(authTokenCache, channel);
					}
					final Mark mark = m_history.getFirst();
					if (mark == null){
						Log.v(ALA_LOG, "sending history end 1 - mark == null");
					}
					completeMark(mark, authTokenCache, channel);
					if ( applyMark(mark) ){
						m_history.removeFirst();
					}
					final int delay = 1000;
					try {
						Thread.sleep(delay);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
		}).start();
	}

	@Override
	public void sendLastCoordinate() {
		new Thread(new Runnable() {
			@Override
			public void run() {
				final String login    =  getPreference(ITrackerNetSettings.LOGIN, "");
				final String pass   = getPreference(ITrackerNetSettings.PASSWORD, "");
				final String channel = getPreference(ITrackerNetSettings.CHANNEL, "");
				if (authTokenCache == null){
					authTokenCache = auth(login, pass);
					isChanAvailableCache = applyChannel(authTokenCache, channel);
				}
				if (isChanAvailableCache){
					Mark mark = constructDruftMark();
					completeMark(mark, authTokenCache, channel);
					applyMark(mark);
				}
			}
		}).start();
	}
	
	public List<Mark> getAllMarks(){
		return m_history.getBufferData();
	}


	private Runnable m_doTracking = new Runnable() {
		@Override
		public void run(){
			final String login    =  getPreference(ITrackerNetSettings.LOGIN, "");
			final String pass   = getPreference(ITrackerNetSettings.PASSWORD, "");
			final String channel = getPreference(ITrackerNetSettings.CHANNEL, "");
			
			while (m_isWorking){
				final Mark mark = constructDruftMark();
				if (isOnline()){
					if (authTokenCache == null){
						authTokenCache = auth(login, pass);
						isChanAvailableCache = applyChannel(authTokenCache, channel);
					}
					if (isChanAvailableCache){
						completeMark(mark, authTokenCache, channel);
						applyMark(mark);
					}
				} else {
					Log.v(ALA_LOG, "add mark to history");
					dropCache();
					m_history.add(mark);
				}
				try {
					Thread.sleep(m_trackInterval * 1000);
				} catch (InterruptedException e) {
					m_isWorking = false;
				}
			}
		}
	};
	
	private boolean applyChannel(String authToken, String channel){
		Log.v(ALA_LOG, "applayChannel");
		JSONObject JSONResponse = new JsonApplyChannelRequest(authToken, channel,
				"Geo2Tag tracker channel", "http://osll.spb.ru/", 3000, m_serverUrl)
				.doRequest();
		
		boolean success = false;
		if(JSONResponse != null){
			String status = JsonBase.getString(JSONResponse, IResponse.STATUS);
			String statusDescription = JsonBase.getString(JSONResponse, IResponse.STATUS_DESCRIPTION);
			success = status.equals(IResponse.OK_STATUS) || statusDescription.equals(IResponse.CHANNEL_EXTSTS); 
			if (!success){
				onErrorOccured(TrackerUtil.MESS_FAIL_APPLY_CHANNEL + statusDescription);
			}
		} else {
			onErrorOccured(TrackerUtil.MESS_FAIL_CONNECTION_TO_SERVER);
		}
		return success;
	}
	
	private boolean applyMark(Mark mark){
		boolean success = false;
		JSONObject JSONResponse = new JsonApplyMarkRequest(mark, m_serverUrl).doRequest();
		if (JSONResponse != null){
			String status = JsonBase.getString(JSONResponse, IResponse.STATUS);
			String statusDescription = JsonBase.getString(JSONResponse, IResponse.STATUS_DESCRIPTION);
			success = status.equals(IResponse.OK_STATUS);
			if (success){
				Log.v(ALA_LOG, "sent " + TrackerUtil.convertLocation(mark));
			} else {
				onErrorOccured("apply mark:" + status + "," + statusDescription);
			}
		} 
		return success;
	}
	
	private void completeMark(Mark mark, String authToken, String channel){
		mark.setAuthToken(authToken);
		mark.setChannel(channel);
	}

	private Mark constructDruftMark(){
		double latitude 	= 0.0;
		double longitude 	= 0.0;
	
		Location location = getLocation();
		if (location == null){
			//return; //FIXME
		} else {
			latitude = location.getLatitude();
			longitude = location.getLongitude();
		}
		Mark mark = new Mark();
		mark.setTitle("title");
		mark.setLink("unknown");
		mark.setDescription("this tag was generated automaticaly by tracker application");
		mark.setLatitude(latitude);
		mark.setLongitude(longitude);
		mark.setTime(TrackerUtil.getTime(new Date()));
		return mark;
	}
	
}
