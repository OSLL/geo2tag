package ru.spb.osll.utils;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

import ru.spb.osll.R;
import ru.spb.osll.TrackerActivity;
import ru.spb.osll.objects.Mark;
import android.app.ActivityManager;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.ActivityManager.RunningServiceInfo;
import android.content.Context;
import android.content.Intent;
import android.location.Location;
import android.location.LocationManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.provider.Settings;

public class TrackerUtil {
	private static DateFormat dateFormat = new SimpleDateFormat("dd MM yyyy HH:MM:ss.SSS");
	public static String getTime(Date date){
		return dateFormat.format(date);
	}
	
	public static final int TRACKER_NOTIFY_ID = 1;
	public static void notify(Context c){
		NotificationManager nm = (NotificationManager)c.getSystemService(Context.NOTIFICATION_SERVICE);
		String title = "Geo2Tag Traker service";
		String expandedText = "Geo2Tag Traker service is running";
		Notification notification = new Notification(R.drawable.icon, title, System.currentTimeMillis());
		Intent i = new Intent(c, TrackerActivity.class);
		PendingIntent launchIntent = PendingIntent.getActivity(c, 0, i, 0);
		notification.setLatestEventInfo(c, title, expandedText, launchIntent);
		nm.notify(TRACKER_NOTIFY_ID,notification);
	}	
		
	public static void disnotify(Context c){
		NotificationManager nm = (NotificationManager)c.getSystemService(Context.NOTIFICATION_SERVICE);
		nm.cancel(TRACKER_NOTIFY_ID);
	}	

	public static String convertLocation(Mark mark){
		return convertLocation(mark.getLatitude(), mark.getLongitude());
	}
	
	public static String convertLocation(Location loc){
		return convertLocation(loc.getLatitude(), loc.getLongitude());
	}
	
	public static String convertLocation(double latitude, double longitude){
		StringBuffer strBuffer = new StringBuffer();
		strBuffer.append("lat: ").append(Location.convert(latitude, Location.FORMAT_MINUTES)).
		append("  lon: ").append(Location.convert(longitude, Location.FORMAT_MINUTES));	
		return strBuffer.toString();
	}
	
	public static void hideApplication(Context c){
		Intent intent = new Intent(Intent.ACTION_MAIN);
		intent.addCategory(Intent.CATEGORY_HOME);
		c.startActivity(intent);
	}
	
	public static boolean isOnline(Context c) {
	    ConnectivityManager cm = (ConnectivityManager)c.getSystemService(Context.CONNECTIVITY_SERVICE);
	    NetworkInfo nInfo = cm.getActiveNetworkInfo();
	    if (nInfo != null && nInfo.isConnected()) {
	        return true;
	    }
	    else {
	        return false;
	    }
	}
	
	@SuppressWarnings("static-access")
	public static boolean isServiceRunning(Context c, String serviceName){
	    ActivityManager manager = (ActivityManager) c.getSystemService(c.ACTIVITY_SERVICE);
	    for (RunningServiceInfo service : manager.getRunningServices(Integer.MAX_VALUE)) {
	        if (serviceName.equals(service.service.getClassName())) {
	            return true;
	        }
	    }
	    return false;
	}
	
	public static void openGPSSettings(Context c){
		c.startActivity(new Intent( Settings.ACTION_LOCATION_SOURCE_SETTINGS )); 
	}
	
	@SuppressWarnings("static-access")
	public static boolean isGPSEnabled(Context c){
		LocationManager locationManager = (LocationManager) c.getSystemService(c.LOCATION_SERVICE); 
		return locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER ); 
	}
	
	public class Logger {
		private PrintWriter m_logger;
		
		public Logger(String logPath){
			try {
				m_logger = new PrintWriter(new OutputStreamWriter(new FileOutputStream(logPath, true), "UTF-8"));
				m_logger.println(dateFormat.format(new Date()) + " \t -------------- START LOG -------------");
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
		public void println(String str){
			m_logger.println(dateFormat.format(new Date()) + "\t\t:" + str);
		}

		public void destroy(){
			if (m_logger != null)
				m_logger.close();
		}
	}

	// messages
	public static final String MESS_FAIL_CONNECTION = "Connection fail...";
	public static final String MESS_FAIL_CONNECTION_TO_SERVER = "Fail connection to server...";
	public static final String MESS_FAIL_ADDUSER = "add user fail:"; 
	public static final String MESS_FAIL_APPLY_CHANNEL = "apply channel fail:";
	public static final String MESS_FAIL_APPLY_MARK = "apply mark fail:";
	
	public static final String MESS_TRACKER_ALREADY_RUNNING = "Tracker is already running...";
	public static final String MESS_TRACKER_START = "Tracker is running...";
	public static final String MESS_TRACKER_STOP = "Tracker stopped...";
	
	public static final String MESS_SETTINGS_SAVED = "Settings saved!";
	public static final String MESS_SETTINGS_NOT_AVAILABLE = "Settings are not available! Stop the tracker...";
}
