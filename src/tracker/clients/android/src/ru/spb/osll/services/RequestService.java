package ru.spb.osll.services;

import org.json.JSONObject;

import ru.spb.osll.TrackerActivity;
import ru.spb.osll.json.JsonApplyChannelRequest;
import ru.spb.osll.json.JsonApplyMarkRequest;
import ru.spb.osll.json.JsonBase;
import ru.spb.osll.json.JsonLoginRequest;
import ru.spb.osll.json.IRequest.IResponse;
import ru.spb.osll.preferences.Settings;
import ru.spb.osll.preferences.Settings.ITrackerSettings;
import android.app.Service;
import android.content.Intent;
import android.location.Location;
import android.os.IBinder;
import android.util.Log;

public class RequestService extends Service {
	private static boolean TRAKER_STATUS = false;
	public synchronized static boolean isActive(){
		return TRAKER_STATUS;
	}
	public synchronized void setServiceStatus(boolean status){
		TRAKER_STATUS = status;
	}
	
	@Override
	public void onCreate() {
		super.onCreate();
		Log.v(TrackerActivity.LOG, "request service create");
	}

	@Override
	public void onStart(Intent intent, int startId) {
		super.onStart(intent, startId);
		Log.v(TrackerActivity.LOG, "request service start");

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

	private Runnable loginRunnable = new Runnable() {
		@Override
		public void run() {
			login();
		}
	};
	
	private final int ATTEMPT = 5;
	private void login(){
		Settings settings = new Settings(this);
		String login = settings.getPreferences().getString(ITrackerSettings.LOGIN, "Makr");
		String password = settings.getPreferences().getString(ITrackerSettings.PASSWORD, "test");
		
		JSONObject JSONResponse = null;
		for(int i = 0; i < ATTEMPT; i++){
			JSONResponse = new JsonLoginRequest(login, password).doRequest();
			if (JSONResponse != null) 
				break;
		}

		if (JSONResponse != null) {
			String status = JsonBase.getString(JSONResponse, IResponse.STATUS);
			String statusDescription = JsonBase.getString(JSONResponse, IResponse.STATUS_DESCRIPTION);
			String authToken = JsonBase.getString(JSONResponse, IResponse.AUTH_TOKEN);
			if(status.equals(IResponse.OK_STATUS)){
				Log.v(TrackerActivity.LOG, "login status : " + statusDescription);
				applyChannel(authToken);
			} else {
				Log.v(TrackerActivity.LOG, "login status : " + statusDescription);
			}
		} else {
			Log.v(TrackerActivity.LOG, "login fail");
			onFail("login fail");
		}
	}

	// TODO
	private void applyChannel(String authToken) {
		JSONObject JSONResponse = null;
		
		for (int i = 0; i < ATTEMPT; i++){
			JSONResponse = new JsonApplyChannelRequest("KKKKKKKKKK", "Test channel",
					"my new super chanel", "http://osll.spb.ru/", 3000)
					.doRequest();
			if(JSONResponse != null)
				break;
		}

		if(JSONResponse != null){
			String status = JsonBase.getString(JSONResponse, IResponse.STATUS);
			String statusDescription = JsonBase.getString(JSONResponse, IResponse.STATUS_DESCRIPTION);
			if (status.equals(IResponse.OK_STATUS) ||
					statusDescription.equals(IResponse.CHANNEL_EXTSTS)){
				Log.v(TrackerActivity.LOG, "apply channel status : " + statusDescription);

				new Thread(markRunnable).start();
			} 
			
		} else {
			// TODO apply channel fail
		}
	}

	
	private static final int DELAY = 5000; 
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
		if (location == null)
			return;
		Log.v(TrackerActivity.LOG, "location " + location);
		
		JSONObject JSONResponse;
		JSONResponse = new JsonApplyMarkRequest(
				"KKKKKKKKKK",
				"My channel",
				"title",
				"unknown",
				"this tag was generated automaticaly by tracker application",
				location.getLatitude(), location.getLongitude(), "04 03 2011 15:33:47.630").doRequest();
		if (JSONResponse != null){
			String status = JsonBase.getString(JSONResponse, IResponse.STATUS);
			String statusDescription = JsonBase.getString(JSONResponse, IResponse.STATUS_DESCRIPTION);
			Log.v(TrackerActivity.LOG, "apply mark status : " + statusDescription);
		}
	}
	
	private void onFail(String mess){
		if (TrackerActivity.Instance != null){
			TrackerActivity.Instance.showToast("login fail");
		}
	}
	
}
