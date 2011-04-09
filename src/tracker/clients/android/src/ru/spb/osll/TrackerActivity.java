package ru.spb.osll;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

import ru.spb.osll.preferences.Settings;
import ru.spb.osll.preferences.SettingsActivity;
import ru.spb.osll.services.LocationService;
import ru.spb.osll.services.RequestService;
import android.app.Activity;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class TrackerActivity extends Activity {
	public static String LOG = "Tracker";
	private static final String SD_CARD_PATH = "/sdcard";	// FIXME hardcoding style; use later Environment.getExternalStorageState()
	private Logger m_loggerLocationListner = new Logger(SD_CARD_PATH + "/loggerLocationListner.txt");
//	private Logger m_loggerGetLocation = new Logger(SD_CARD_PATH + "/loggerGetLocation.txt");
	
	TextView logView;
	public static TrackerActivity Instance;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		logView = (TextView) findViewById(R.id.TextField);
			
		initialization();
		Instance = this;
		
		
		Settings settings = new Settings(this);
		if (settings.isSettingsEmpty()){
			settings.setDefaultSettrings();
		}
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		m_loggerLocationListner.destroy();
	}

	// ----------------------------------------------------------------
	private void initialization(){
		Log.v(LOG, "TrackerActivity - initialization");
		
		final Button btnService = (Button) findViewById(R.id.TestButton);
		if (RequestService.isActive()){
			btnService.setText("STOP TRACKER");
		} else {
			btnService.setText("START TRACKER");
		}
		
		btnService.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
            	startTracker();
            }
        });

		final Button settingsBtn= (Button) findViewById(R.id.settingsButton);
		settingsBtn.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				startActivity(new Intent(TrackerActivity.this, SettingsActivity.class));
			}
		});
		
	}
	
	private static final int ID = 1;
	private void startTracker(){
		try {
			if (RequestService.isActive()){
				stopService(new Intent(this, RequestService.class));
				stopService(new Intent(this, LocationService.class));
				
				NotificationManager nm = (NotificationManager)getSystemService(Context.NOTIFICATION_SERVICE);
				nm.cancel(ID);
			} else if (isOnline()){
				startService(new Intent(this, RequestService.class));
				startService(new Intent(this, LocationService.class));
				
				//notify("geo2tag tracker", "geo2tag tracker service");
				notify2();
			}
		} catch (Exception e){
			logView.append("\n" + e.getMessage());
		}
	}
	
	private void notify(String text, String details)
	{
		NotificationManager nm = (NotificationManager)getSystemService(Context.NOTIFICATION_SERVICE);

		Intent notificationIntent = new Intent(this, TrackerActivity.class);
//	    notificationIntent.setAction(Intent.ACTION_MAIN);
//	    notificationIntent.addCategory(Intent.CATEGORY_LAUNCHER);

	    PendingIntent contentIntent = PendingIntent.getActivity(this, 0, notificationIntent, 0); 
	    Notification note = new Notification(R.drawable.icon, text,System.currentTimeMillis()); 

	    note.setLatestEventInfo(this, text, details, contentIntent);
	    note.flags = Notification.FLAG_AUTO_CANCEL;
	    
	    nm.notify(ID, note);
	}
	
	private void notify2(){
		NotificationManager nm = (NotificationManager)getSystemService(Context.NOTIFICATION_SERVICE);

		Notification notification = new Notification(R.drawable.icon, "NewMaps",System.currentTimeMillis());
		String expandedTitle = "New Map Service";
		String expandedText = "New Map Service is running";
		Intent i = new Intent(this, TrackerActivity.class);
		PendingIntent launchIntent = PendingIntent.getActivity(getApplicationContext(), 0, i, 0);
		notification.setLatestEventInfo(getApplicationContext(), expandedTitle, expandedText, launchIntent);
		nm.notify(ID,notification);
	}
	
	// ------------------------------------------------------------------
	
	private class Logger {
		private PrintWriter m_logger;
		DateFormat m_dateFormat = new SimpleDateFormat("yyyy/dd/MM/HH/mm/ss");
		
		private Logger(String logPath){
			try {
				m_logger = new PrintWriter(new OutputStreamWriter(new FileOutputStream(logPath, true), "UTF-8"));
				m_logger.println(m_dateFormat.format(new Date()) + " \t -------------- START LOG -------------");
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
		public void println(String str){
			m_logger.println(m_dateFormat.format(new Date()) + "\t\t:" + str);
		}

		public void destroy(){
			if (m_logger != null)
				m_logger.close();
		}
	}
	
	private boolean isOnline() {
	    ConnectivityManager cm = (ConnectivityManager)getSystemService(Context.CONNECTIVITY_SERVICE);
	    NetworkInfo nInfo = cm.getActiveNetworkInfo();
	    if (nInfo != null && nInfo.isConnected()) {
	    	showToast("ONLINE");
	    	Log.v(LOG, "ONLINE");
	        return true;
	    }
	    else {
	    	showToast("OFFLINE");
	        Log.v(LOG, "OFFLINE");
	        return false;
	    }
	}

	public void showToast(final String mess){
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				Toast.makeText(TrackerActivity.this, mess, Toast.LENGTH_SHORT).show();				
			}
		});
	}

//	public void showToast(final String mess){
//		Toast.makeText(TrackerActivity.this, mess, Toast.LENGTH_SHORT).show();				
//	}
}
