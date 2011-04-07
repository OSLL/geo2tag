package ru.spb.osll.services;

import ru.spb.osll.TrackerActivity;
import android.app.Service;
import android.content.Intent;
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
		Log.v(TrackerActivity.LOG, "service create");
	}

	@Override
	public void onStart(Intent intent, int startId) {
		super.onStart(intent, startId);
		Log.v(TrackerActivity.LOG, "service start");
		
		setServiceStatus(true);
		new Thread(raRunnable).start();
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

	Runnable raRunnable = new Runnable() {
		@Override
		public void run() {
			while (isActive()){
				try {
					Log.v(TrackerActivity.LOG, "service tick");
					Thread.sleep(1000);
				} catch (InterruptedException e) {
					setServiceStatus(false);
				}
			}
		}
	}; 
	
}
