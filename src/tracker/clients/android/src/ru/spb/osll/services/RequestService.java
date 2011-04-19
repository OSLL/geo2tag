package ru.spb.osll.services;

import java.util.Date;

import org.json.JSONObject;

import ru.spb.osll.TrackerActivity;
import ru.spb.osll.TrackerActivity.TrackerReceiver;
import ru.spb.osll.json.JsonAddUserRequest;
import ru.spb.osll.json.JsonApplyChannelRequest;
import ru.spb.osll.json.JsonApplyMarkRequest;
import ru.spb.osll.json.JsonBase;
import ru.spb.osll.json.JsonLoginRequest;
import ru.spb.osll.json.IRequest.IResponse;
import ru.spb.osll.preferences.Settings;
import ru.spb.osll.preferences.Settings.ITrackerAppSettings;
import ru.spb.osll.preferences.Settings.ITrackerNetSettings;
import ru.spb.osll.utils.TrackerUtil;
import android.app.Service;
import android.content.Intent;
import android.content.SharedPreferences;
import android.location.Location;
import android.os.IBinder;
import android.util.Log;

public class RequestService extends Service {
	// get in settings
	private String m_login;
	private String m_password;
	private String m_channel;
	private String m_serverUrl;
	
	private String m_authToken;

	@Override
	public void onCreate() {
		super.onCreate();
		Log.v(TrackerActivity.LOG, "request service create");
	}

	@Override
	public void onStart(Intent intent, int startId) {
		super.onStart(intent, startId);
		Log.v(TrackerActivity.LOG, "request service start");

		getSettingsValues();
		setServiceStatus(true);
		new Thread(loginRunnable).start();
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		Log.v(TrackerActivity.LOG, "request service destroy");
		setServiceStatus(false);
	}

	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}

	private void getSettingsValues(){
		SharedPreferences settings = new Settings(this).getPreferences();
		m_login = settings.getString(ITrackerNetSettings.LOGIN, "");
		m_password = settings.getString(ITrackerNetSettings.PASSWORD, "");
		m_channel = settings.getString(ITrackerNetSettings.CHANNEL, "");
		//m_channelKey = settings.getString(ITrackerSettings.CHANNEL_KEY, "");
		m_serverUrl = settings.getString(ITrackerNetSettings.SERVER_URL, "");
	}
	
	private Runnable loginRunnable = new Runnable() {
		@Override
		public void run() {
			login();
		}
	};
	
	private final int ATTEMPT = 5;
	private void login(){
		JSONObject JSONResponse = null;
		for(int i = 0; i < ATTEMPT; i++){
			JSONResponse = new JsonLoginRequest(m_login, m_password, m_serverUrl).doRequest();
			if (JSONResponse != null) 
				break;
		}

		if (JSONResponse != null) {
			String status = JsonBase.getString(JSONResponse, IResponse.STATUS);
			String statusDescription = JsonBase.getString(JSONResponse, IResponse.STATUS_DESCRIPTION);
			m_authToken = JsonBase.getString(JSONResponse, IResponse.AUTH_TOKEN);

			showMess("login:" + status + ","  + statusDescription, false);
			if(status.equals(IResponse.OK_STATUS)){
				applyChannel();
			} else {
				addUser();
			}
		} else {
			onFail(TrackerUtil.MESS_FAIL_CONNECTION_TO_SERVER);
		}
	}
	
	private void addUser(){
		Log.v(TrackerActivity.LOG, "addUser()");
		
		JSONObject JSONResponse = null;
		for(int i = 0; i < ATTEMPT; i++){
			JSONResponse = new JsonAddUserRequest(m_login, m_password, m_serverUrl).doRequest();
			if (JSONResponse != null) 
				break;
		}

		if (JSONResponse != null) {
			String status = JsonBase.getString(JSONResponse, IResponse.STATUS);
			String statusDescription = JsonBase.getString(JSONResponse, IResponse.STATUS_DESCRIPTION);
			showMess("add user:" + status + ","  + statusDescription, false);
			if(status.equals(IResponse.OK_STATUS)){
				login();
			} else {
				onFail(TrackerUtil.MESS_FAIL_ADDUSER + statusDescription);
			}
		} else {
			onFail(TrackerUtil.MESS_FAIL_CONNECTION_TO_SERVER);
		}
	}

	private void applyChannel() {
		JSONObject JSONResponse = null;
		for (int i = 0; i < ATTEMPT; i++){
			JSONResponse = new JsonApplyChannelRequest(m_authToken, m_channel,
					"Geo2Tag tracker channel", "http://osll.spb.ru/", 3000, m_serverUrl)
					.doRequest();
			if(JSONResponse != null)
				break;
		}

		if(JSONResponse != null){
			String status = JsonBase.getString(JSONResponse, IResponse.STATUS);
			String statusDescription = JsonBase.getString(JSONResponse, IResponse.STATUS_DESCRIPTION);
			showMess("apply channel:" + status + ","  + statusDescription, false);	
			if (status.equals(IResponse.OK_STATUS) ||
					statusDescription.equals(IResponse.CHANNEL_EXTSTS)){
				new Thread(markRunnable).start();
			} else {
				onFail(TrackerUtil.MESS_FAIL_APPLY_CHANNEL + statusDescription);
			}
		} else {
			onFail(TrackerUtil.MESS_FAIL_CONNECTION_TO_SERVER);
		}
	}

	Runnable markRunnable = new Runnable() {
		@Override
		public void run() {
			int delay = new Settings(RequestService.this).
				getPreferences().getInt(ITrackerNetSettings.TIME_TICK, 5);
			while (isActive()){
				try {
					applyMark();
					Thread.sleep(delay*1000);
				} catch (InterruptedException e) {
					setServiceStatus(false);
				}
			}
		}
	};

	private void applyMark(){
		double latitude = 30.0;
		double longitude = 60.0;
	
		Location location = LocationService.getLocation(this);
		if (location == null){
			//return; //FIXME
		} else {
			latitude = location.getLatitude();
			longitude = location.getLongitude();
		}
		
		JSONObject JSONResponse;
		JSONResponse = new JsonApplyMarkRequest(
				m_authToken,
				m_channel,
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
			 	boolean isShowTick = Settings.getPreferences(this).getBoolean(ITrackerAppSettings.IS_SHOW_TICKS, false); 
				showMess(TrackerUtil.convertLocation(latitude, longitude), isShowTick);
			} else {
				showMess("apply mark:" + status + "," + statusDescription, false);
			}
		} 
	}
	
	private void showMess(String mess, boolean isShowToast){
		Log.v(TrackerActivity.LOG, mess);
		sendMess(TrackerReceiver.ID_APPEND_TO_LOG, mess);
		if (isShowToast)
			sendMess(TrackerReceiver.ID_SHOW_TOAST, mess);

//		TrackerUtil.appendToLogView(mess);
//		if (isShowToast)
//			TrackerUtil.showToast(mess);
	}
	
	private void onFail(String mess){
		String failMess = "FAIL: " + mess;
		Log.v(TrackerActivity.LOG, failMess);
		TrackerUtil.showToast(failMess);
		TrackerUtil.appendToLogView(failMess);
		TrackerUtil.disnotify(this);
		setServiceStatus(false);
	}

	private void sendMess(int operationId, String mess){
		Intent intent = new Intent(TrackerReceiver.ACTION_SHOW_MESS);
		intent.putExtra(TrackerReceiver.TYPE_OPEATION, operationId);
		intent.putExtra(TrackerReceiver.TYPE_MESS, mess);
		sendBroadcast(intent);
	}
	
	private static boolean TRAKER_STATUS = false;
	public synchronized static boolean isActive(){
		return TRAKER_STATUS;
	}
	public synchronized void setServiceStatus(boolean status){
		TRAKER_STATUS = status;
	}
}
