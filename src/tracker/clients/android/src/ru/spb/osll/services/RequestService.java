package ru.spb.osll.services;

import java.io.IOException;
import java.net.URISyntaxException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

import org.json.JSONException;
import org.json.JSONObject;

import ru.spb.osll.R;
import ru.spb.osll.TrackerActivity;
import ru.spb.osll.json.JsonAddUserRequest;
import ru.spb.osll.json.JsonApplyChannelRequest;
import ru.spb.osll.json.JsonApplyMarkRequest;
import ru.spb.osll.json.JsonBase;
import ru.spb.osll.json.JsonLoginRequest;
import ru.spb.osll.json.JsonRequest;
import ru.spb.osll.json.IRequest.IResponse;
import ru.spb.osll.preferences.Settings;
import ru.spb.osll.preferences.Settings.ITrackerSettings;
import ru.spb.osll.utils.TrackerUtil;
import android.app.Service;
import android.content.Intent;
import android.content.SharedPreferences;
import android.location.Location;
import android.os.IBinder;
import android.util.Log;

public class RequestService extends Service {
	
	public static String SERVER; // FIXME bad smells in code
	private String m_authToken;

	// get in settings
	private String m_login;
	private String m_password;
	private String m_channel;
	private String m_channelKey;
	private String m_serverUrl;
	
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
		
		SERVER = new Settings(this).getPreferences().getString(ITrackerSettings.SERVER_URL, ""); // TODO bad smells in code
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
		m_login = settings.getString(ITrackerSettings.LOGIN, "");
		m_password = settings.getString(ITrackerSettings.PASSWORD, "");
		m_channel = settings.getString(ITrackerSettings.CHANNEL, "");
		m_channelKey = settings.getString(ITrackerSettings.CHANNEL_KEY, "");
		m_serverUrl = settings.getString(ITrackerSettings.SERVER_URL, "");
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
			JSONResponse = new JsonLoginRequest(m_login, m_password).doRequest();
			if (JSONResponse != null) 
				break;
		}

		if (JSONResponse != null) {
			String status = JsonBase.getString(JSONResponse, IResponse.STATUS);
			String statusDescription = JsonBase.getString(JSONResponse, IResponse.STATUS_DESCRIPTION);
			m_authToken = JsonBase.getString(JSONResponse, IResponse.AUTH_TOKEN);
			if(status.equals(IResponse.OK_STATUS)){
				Log.v(TrackerActivity.LOG, "login status : " + statusDescription);
				TrackerUtil.appendToLogView("login status : " + status);
				TrackerUtil.appendToLogView("login statusDescription : " + statusDescription);
				applyChannel();
			} else {
				Log.v(TrackerActivity.LOG, "login status : " + statusDescription);
				TrackerUtil.appendToLogView("login status : " + status);
				TrackerUtil.appendToLogView("login statusDescription : " + statusDescription);
				addUser();
			}
		} else {
			Log.v(TrackerActivity.LOG, "login fail");
			TrackerUtil.showToast("login fail");
		}
	}
	
	private void addUser(){
		Log.v(TrackerActivity.LOG, "addUser()");
		
		JSONObject JSONResponse = null;
		for(int i = 0; i < ATTEMPT; i++){
			JSONResponse = new JsonAddUserRequest(m_login, m_password).doRequest();
			if (JSONResponse != null) 
				break;
		}

		if (JSONResponse != null) {
			String status = JsonBase.getString(JSONResponse, IResponse.STATUS);
			String statusDescription = JsonBase.getString(JSONResponse, IResponse.STATUS_DESCRIPTION);
			if(status.equals(IResponse.OK_STATUS)){
				Log.v(TrackerActivity.LOG, "addUser status : " + statusDescription);
				TrackerUtil.appendToLogView("addUser status : " + status);
				TrackerUtil.appendToLogView("addUser statusDescription : " + statusDescription);
				login();
			} else {
				Log.v(TrackerActivity.LOG, "addUser status : " + statusDescription);
				TrackerUtil.appendToLogView("addUser status : " + status);
				TrackerUtil.appendToLogView("addUser statusDescription : " + statusDescription);
			}
		} else {
			Log.v(TrackerActivity.LOG, "addUser fail");
		}
	}

	// TODO
	private void applyChannel() {
		Log.v(TrackerActivity.LOG, "applyChannel()");
				
		JSONObject JSONResponse = null;
		for (int i = 0; i < ATTEMPT; i++){
			JSONResponse = new JsonApplyChannelRequest(m_authToken, m_channel,
					"Geo2Tag tracker channel", "http://osll.spb.ru/", 3000)
					.doRequest();
			if(JSONResponse != null)
				break;
		}

		if(JSONResponse != null){
			Log.v(TrackerActivity.LOG, "JSonResponse = " + JSONResponse.toString());
			
			String status = JsonBase.getString(JSONResponse, IResponse.STATUS);
			String statusDescription = JsonBase.getString(JSONResponse, IResponse.STATUS_DESCRIPTION);
			if (status.equals(IResponse.OK_STATUS) ||
					statusDescription.equals(IResponse.CHANNEL_EXTSTS)){
				Log.v(TrackerActivity.LOG, "apply channel status : " + statusDescription);
				TrackerUtil.appendToLogView("apply channel status status : " + status);
				TrackerUtil.appendToLogView("apply channel status statusDescription : " + statusDescription);
				new Thread(markRunnable).start();
			} else {
				TrackerUtil.appendToLogView("apply channel status status : " + status);
				TrackerUtil.appendToLogView("apply channel status statusDescription : " + statusDescription);
			}
		} else {
			Log.v(TrackerActivity.LOG, "apply channel fail");
			// TODO apply channel fail
		}
	}

	
	private static final int DELAY = 2500; 
	Runnable markRunnable = new Runnable() {
		@Override
		public void run() {
			while (isActive()){
				try {
					applyMark();
					Thread.sleep(DELAY);
				} catch (InterruptedException e) {
					setServiceStatus(false);
				}
			}
		}
	};

	private void applyMark(){
		Location location = LocationService.getLocation(this);
		// FIXME
//		if (location == null)
//			return;
//		Log.v(TrackerActivity.LOG, "location " + location);
				
		JSONObject JSONResponse;
		JSONResponse = new JsonApplyMarkRequest(
				m_authToken,
				m_channel,
				"title",
				"unknown",
				"this tag was generated automaticaly by tracker application",
				/*location.getLatitude()*/ 30.0,	// FIXME
				/*location.getLongitude()*/60.0,	// FIXME
				TrackerUtil.getTime(new Date())).doRequest();
		if (JSONResponse != null){
			String status = JsonBase.getString(JSONResponse, IResponse.STATUS);
			String statusDescription = JsonBase.getString(JSONResponse, IResponse.STATUS_DESCRIPTION);
			Log.v(TrackerActivity.LOG, "apply mark status : " + statusDescription);

			TrackerUtil.appendToLogView("apply mark status : " + status);
			TrackerUtil.appendToLogView("apply mark status statusDescription : " + statusDescription);
		}
	}
	
	private static boolean TRAKER_STATUS = false;
	public synchronized static boolean isActive(){
		return TRAKER_STATUS;
	}
	public synchronized void setServiceStatus(boolean status){
		TRAKER_STATUS = status;
	}

}
