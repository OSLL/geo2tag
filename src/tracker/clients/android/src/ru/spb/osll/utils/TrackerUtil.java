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
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;

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
	
	public static void showToast(final String mess){
		if (TrackerActivity.Instance != null){
			TrackerActivity.Instance.showToast(mess);
		}
	}

	public static void showToast(final int mess){
		if (TrackerActivity.Instance != null){
			TrackerActivity.Instance.showToast(mess);
		}
	}

	private static int lineCount = 0;
	public static void appendToLogView(final String mess){
		if (TrackerActivity.Instance != null){
			if (lineCount > 10){
				TrackerActivity.Instance.clearLogView();
				lineCount = 0;
			}
			TrackerActivity.Instance.appendToLogView(mess);
			lineCount++;
		}
	}
	
	
	//TODO	
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

}
