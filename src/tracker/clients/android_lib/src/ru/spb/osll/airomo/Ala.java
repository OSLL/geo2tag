package ru.spb.osll.airomo;

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
	public void onDestroy(Context c) {
		super.onDestroy(c);
		dropCache();
		if (m_trackThread != null){
			m_trackThread.interrupt();
		}
		if (m_historyThread != null){
			m_historyThread.interrupt();
		}
		stopLocationListener();
	}

	private synchronized Buffer<Mark> getHistory(){
		return m_history;
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
		doTracking();
	}

	@Override
	public void stopTrack() {
		m_isWorking = false;
		dropCache();
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
		getHistory().setBufferSize(maxMarks);
		setPreference(ITrackerNetSettings.HISTORY_LIMIT, maxMarks);
	}

	@Override
	public int getHistoryLimit() {
		return getPreference(ITrackerNetSettings.HISTORY_LIMIT, 50);
	}
	
	@Override
	protected void networkStatusChanged(boolean isOnline) {
		if(isOnline){
			sendHistory();
		} else {
			dropCache();			
		}
	}

	@Override
	protected void gooffEvent() {
		sendLastCoordinate();
		sendHistory();
	}
	
	private String m_authTokenCache = null;
	private boolean m_isChanAvailableCache = false;
	private void dropCache(){
		m_authTokenCache = null;
		m_isChanAvailableCache = false;
	}
	
	private Thread m_historyThread;
	@Override
	public void sendHistory() {
		if  (m_historyThread != null){
			m_historyThread.interrupt();
		}
		m_historyThread = new Thread(new Runnable() {
			@Override
			public void run() {
				final String login = getPreference(ITrackerNetSettings.LOGIN, "");
				final String pass = getPreference(ITrackerNetSettings.PASSWORD, "");
				final String channel = getPreference(ITrackerNetSettings.CHANNEL, "");
				try {
					while(isOnline() && getHistory().hasElements()){
						Log.v(ALA_LOG, "sending history...");
						if (m_authTokenCache == null){
							m_authTokenCache = auth(login, pass);
							m_isChanAvailableCache = applyChannel(m_authTokenCache, channel);
						}
						if (m_isChanAvailableCache){
							final Mark mark = getHistory().getFirst();
							completeMark(mark, m_authTokenCache, channel);
							if ( applyMark(mark) ){
								getHistory().removeFirst();
							}
						}
						Thread.sleep(333);
					}
				} catch (InterruptedException e) {
					Log.v(ALA_LOG, "m_historyThread is interrupted");
				}
			}
		});
		m_historyThread.start();
	}

	@Override
	public void sendLastCoordinate() {
		new Thread(new Runnable() {
			@Override
			public void run() {
				final String login =  getPreference(ITrackerNetSettings.LOGIN, "");
				final String pass = getPreference(ITrackerNetSettings.PASSWORD, "");
				final String channel = getPreference(ITrackerNetSettings.CHANNEL, "");
				if (m_authTokenCache == null){
					m_authTokenCache = auth(login, pass);
					m_isChanAvailableCache = applyChannel(m_authTokenCache, channel);
				}
				if (m_isChanAvailableCache){
					Mark mark = constructDruftMark();
					completeMark(mark, m_authTokenCache, channel);
					applyMark(mark);
				}
			}
		}).start();
	}
	
	public List<Mark> getAllMarks(){
		return getHistory().getBufferData();
	}
	

	private Thread m_trackThread;
	private void doTracking(){
		if (m_trackThread != null){
			m_trackThread.interrupt();
		}
		m_trackThread = new Thread(new Runnable() {
			@Override
			public void run() {
				final String login = getPreference(ITrackerNetSettings.LOGIN, "");
				final String pass = getPreference(ITrackerNetSettings.PASSWORD, "");
				final String channel = getPreference(ITrackerNetSettings.CHANNEL, "");
				try {
					while (!Thread.currentThread().isInterrupted() && m_isWorking){
						final Mark mark = constructDruftMark();
						if (isOnline()){
							if (m_authTokenCache == null){
								m_authTokenCache = auth(login, pass);
								m_isChanAvailableCache = applyChannel(m_authTokenCache, channel);
							}
							if (m_isChanAvailableCache){
								completeMark(mark, m_authTokenCache, channel);
								Log.v(ALA_LOG, "track...");
								applyMark(mark);
							}
						} else {
							Log.v(ALA_LOG, "add mark to history");
							getHistory().add(mark);
						}
						Thread.sleep(m_trackInterval * 1000);
					}
				} catch (InterruptedException e) {
					//m_isWorking = false;
				}
			}
		});
		m_trackThread.start();
	}
	
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
}
