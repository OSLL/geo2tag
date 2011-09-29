package ru.spb.osll.airomo;

import org.json.JSONObject;

import ru.spb.osll.json.JsonApplyChannelRequest;
import ru.spb.osll.json.JsonApplyMarkRequest;
import ru.spb.osll.json.JsonBase;
import ru.spb.osll.json.JsonLoginRequest;
import ru.spb.osll.json.IRequest.IResponse;
import ru.spb.osll.objects.Mark;
import ru.spb.osll.utils.TrackerUtil;
import android.content.Intent;
import android.os.SystemClock;
import android.util.Log;

public class AlaService extends BaseAlaService {
	private Buffer<Mark> m_history;

	private Thread m_trackThread;
	private Thread m_historyThread;
	private String m_authTokenCache = null;
	private boolean m_isChanAvailableCache = false;

	private void dropCache(){
		m_authTokenCache = null;
		m_isChanAvailableCache = false;
	}
	
	private synchronized Buffer<Mark> getHistory(){
		return m_history;
	}
	
	@Override
	public void onCreate() {
		super.onCreate();
		m_history = new Buffer<Mark>(sCache().historyLimit); 
	}

	@Override
	public void onStart(Intent intent, int startId) {
		super.onStart(intent, startId);
		//startTracking(); // TODO for testing on AVD
	}
	
	@Override
	public void onDestroy() {
		stopTracking();
		stopSendHistory();
		super.onDestroy();
	}

	@Override
	protected void networkStatusChanged(boolean isOnline) {
		Log.v(Ala.ALA_LOG, "networkStatusChanged: " + isOnline);
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
	
	@Override
	protected void onLocationDeviceStatusChanged(boolean isReady) {
		Log.v(Ala.ALA_LOG, "onLocationDeviceStatusChanged: " + isReady);
		if (isReady) {
			startTracking();
		} else {
			stopTracking();
		}
	}
	
	@Override
	protected void onSettingUpdated() {
		getHistory().setBufferSize(sCache().historyLimit);
	}
	
	@Override
	protected void stopTracking(){
		if (m_trackThread != null){
			m_trackThread.interrupt();
		}
	}
	
	@Override
	protected void startTracking(){
		if (m_trackThread != null){
			m_trackThread.interrupt();
		}
		m_trackThread = new Thread(new Runnable() {
			@Override
			public void run() {
				refreshSCache();
				while (!Thread.currentThread().isInterrupted()){
					final Mark mark = constructDruftMark();
					if (isOnline()){
						Log.v(Ala.ALA_LOG, "send mark to server...");
						sendMark(mark);
					} else {
						Log.v(Ala.ALA_LOG, "add mark to history...");
						getHistory().add(mark); 			
					}
					SystemClock.sleep(sCache().trackInterval * 1000);
				}
			}
		});
		m_trackThread.start();
	}

	private void stopSendHistory(){
		if (m_historyThread != null){
			m_historyThread.interrupt();
		}
	}
	
	@Override
	public void sendHistory() {
		Log.v(Ala.ALA_LOG, "CALLED: sendHistory");
		if  (m_historyThread != null){
			m_historyThread.interrupt();
		}
		m_historyThread = new Thread(new Runnable() {
			@Override
			public void run() {
				refreshSCache();
				while(isOnline() && getHistory().hasElements()){
					Log.v(Ala.ALA_LOG, "sending history...");
					final Mark mark = getHistory().getFirst();
					if (sendMark(mark)){
						Log.v(Ala.ALA_LOG, "sending history: mark was sent...");
						getHistory().removeFirst();
					}
					SystemClock.sleep(333);
				}
			}
		});
		m_historyThread.start();
	}	
	
	@Override
	public void sendLastCoordinate() {
		Log.v(Ala.ALA_LOG, "CALLED: sendLastCoordinate");
		new Thread(new Runnable() {
			@Override
			public void run() {
				refreshSCache();
				final Mark mark = constructDruftMark();
				sendMark(mark);
			}
		}).start();
	}
	
	
	
	
	private boolean sendMark(Mark mark){
		if(!m_isChanAvailableCache){
			m_authTokenCache = auth(sCache().login, sCache().pass);
			m_isChanAvailableCache = applyChannel(m_authTokenCache, sCache().channel);
		}
		if (m_isChanAvailableCache){
			completeMark(mark, m_authTokenCache, sCache().channel);
			return applyMark(mark);
		}
		return false;
	}

	private String auth(String login, String pass) {
		String authToken = null;
		final String serverUrl = sCache().serverUrl;
		JSONObject JSONResponse = new JsonLoginRequest(login, pass, serverUrl).doRequest();
		if (JSONResponse != null) {
			authToken = JsonBase.getString(JSONResponse, IResponse.AUTH_TOKEN);
		} else {
			onErrorOccured(TrackerUtil.MESS_FAIL_CONNECTION_TO_SERVER); 
		}
		return authToken;
	}
	
	private boolean applyChannel(String authToken, String channel){
		Log.v(Ala.ALA_LOG, "applayChannel");
		JSONObject JSONResponse = new JsonApplyChannelRequest(authToken, channel,
				"Geo2Tag tracker channel", "http://osll.spb.ru/", 3000, sCache().serverUrl)
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
		JSONObject JSONResponse = new JsonApplyMarkRequest(mark, sCache().serverUrl).doRequest();
		if (JSONResponse != null){
			String status = JsonBase.getString(JSONResponse, IResponse.STATUS);
			String statusDescription = JsonBase.getString(JSONResponse, IResponse.STATUS_DESCRIPTION);
			success = status.equals(IResponse.OK_STATUS);
			if (success){
				Log.v(Ala.ALA_LOG, "sent " + TrackerUtil.convertLocation(mark));
			} else {
				onErrorOccured("apply mark:" + status + "," + statusDescription);
			}
		} 
		return success;
	}
	
}
