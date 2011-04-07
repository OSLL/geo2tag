package ru.spb.osll.services;

import org.json.JSONObject;

import ru.spb.osll.TrackerActivity;
import ru.spb.osll.json.JsonApplyChannelRequest;
import ru.spb.osll.json.JsonApplyMarkRequest;
import ru.spb.osll.json.JsonBase;
import ru.spb.osll.json.JsonLoginRequest;
import ru.spb.osll.json.IRequest.IResponse;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;
import android.widget.Toast;

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
		Log.v(TrackerActivity.LOG, "service create");
	}

	@Override
	public void onStart(Intent intent, int startId) {
		super.onStart(intent, startId);
		Log.v(TrackerActivity.LOG, "service start");
		
		setServiceStatus(true);
		//login();

		new Thread(r).run();
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		Log.v(TrackerActivity.LOG, "service destroy");
		setServiceStatus(false);
	}

	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}

	private void login(){
		JSONObject JSONResponse = null;
		for(int i = 0; i < 10; i++){
			JSONResponse = new JsonLoginRequest("Mark", "test").doRequest();
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
			// TODO login fail
		}
	}

	// TODO
	private void applyChannel(String authToken) {
		JSONObject JSONResponse = null;
		
		for (int i = 0; i < 10; i++){
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
				//applyMarkRunnable(); //TODO
			} 
			
		} else {
			// TODO apply channel fail
		}
	}

	
	private static final int DELAY = 5000; 
		Runnable r = new Runnable() {
			@Override
			public void run() {
				while (isActive()){
					try {
//						applyMark();
						Log.v(TrackerActivity.LOG, "apply mark status : ");
						Thread.sleep(DELAY);
					} catch (InterruptedException e) {
						setServiceStatus(false);
					}
				}
			}
		};
		
	
	private void applyMark(){
		JSONObject JSONResponse;
		while (RequestService.isActive()) {
			JSONResponse = new JsonApplyMarkRequest(
					"KKKKKKKKKK",
					"My channel",
					"title",
					"unknown",
					"this tag was generated automaticaly by tracker application",
					60.0, 30.0, "04 03 2011 15:33:47.630").doRequest();
			if (JSONResponse != null){
				String status = JsonBase.getString(JSONResponse, IResponse.STATUS);
				String statusDescription = JsonBase.getString(JSONResponse, IResponse.STATUS_DESCRIPTION);
				Log.v(TrackerActivity.LOG, "apply mark status : " + statusDescription);
			}
		}
	}
	
}
