package ru.spb.osll.airomo;

import java.util.Date;

import org.json.JSONObject;

import ru.spb.osll.TrackerActivity;
import ru.spb.osll.json.IRequest.IResponse;
import ru.spb.osll.json.JsonApplyChannelRequest;
import ru.spb.osll.json.JsonApplyMarkRequest;
import ru.spb.osll.json.JsonBase;
import ru.spb.osll.json.JsonLoginRequest;
import ru.spb.osll.preferences.Settings.ITrackerNetSettings;
import ru.spb.osll.utils.TrackerUtil;
import android.content.Context;
import android.location.Location;
import android.util.Log;

public class Ala extends BaseAla {
	private boolean m_isWorking = false;
	private int m_trackInterval;
	private int  m_historyLimit;
	
	public Ala(Context c) {
		super(c);
		m_trackInterval = getTrackInterval();
	}
	
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
		m_historyLimit = maxMarks;
		setPreference(ITrackerNetSettings.HISTORY_LIMIT, maxMarks);
	}

	@Override
	public int getHistoryLimit() {
		return getPreference(ITrackerNetSettings.HISTORY_LIMIT, 50);
	}

	@Override
	public void sendHistory() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void sendLastCoordinate() {
		// TODO Auto-generated method stub
		
	}

	// TODO
	private Runnable m_doTracking = new Runnable() {
		@Override
		public void run(){
			Log.v(TrackerActivity.LOG, "m_doTracking");
			if (isOnline()){
				final String login  =  getPreference(ITrackerNetSettings.LOGIN, "");
				final String pass = getPreference(ITrackerNetSettings.PASSWORD, "");
				final String authToken = auth(login, pass);
				Log.v(TrackerActivity.LOG, "authToken : " + authToken);
				final String channel = getPreference(ITrackerNetSettings.CHANNEL, "");
				if (applyChannel(authToken, channel)){
					Log.v(TrackerActivity.LOG, "lol " + m_isWorking);
					int delay = getPreference(ITrackerNetSettings.TIME_TICK, 5);
					while(m_isWorking){
						try {
							applyMark(authToken, channel);
							Thread.sleep(delay*1000);
						} catch (InterruptedException e) {
							m_isWorking = false;
						}
					}
				}
			}
		}
	};
	

	private boolean applyChannel(String authToken, String channel){
		Log.v(TrackerActivity.LOG, "appayChannel ");
		
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
	
	private void applyMark(String authToken, String channel){
		double latitude = 30.0;
		double longitude = 60.0;
	
		Location location = getLocation();
		if (location == null){
			//return; //FIXME
		} else {
			latitude = location.getLatitude();
			longitude = location.getLongitude();
		}
		
		JSONObject JSONResponse;
		JSONResponse = new JsonApplyMarkRequest(
				authToken,
				channel,
				"title",
				"unknown",
				"this tag was generated automaticaly by tracker application",
				latitude,	
				longitude,	
				TrackerUtil.getTime(new Date()), 
				m_serverUrl).doRequest();
		if (JSONResponse != null){
			String status = JsonBase.getString(JSONResponse, IResponse.STATUS);
			String statusDescription = JsonBase.getString(JSONResponse, IResponse.STATUS_DESCRIPTION);
			if (status.equals(IResponse.OK_STATUS)){
				Log.v(TrackerActivity.LOG, "sent " + TrackerUtil.convertLocation(latitude, longitude));
				//onErrorOccured(TrackerUtil.convertLocation(latitude, longitude));
			} else {
				onErrorOccured("apply mark:" + status + "," + statusDescription);
			}
		} 
	}

}
