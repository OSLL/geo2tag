package ru.spb.osll.GDS;

import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.TimeZone;

import ru.spb.osll.objects.Mark;
import android.app.ActivityManager;
import android.app.ActivityManager.RunningServiceInfo;
import android.content.Context;
import android.location.Location;
import android.util.Log;

public class GDSUtil {
	
	private GDSUtil() {}
	
	public static final boolean DEBUG = true;
	public static final String LOG = "GeoDoctorSearch";
	public static final int ATTEMPTS = 5;
	public static final boolean NOT_RECEIVE_OWN_EVENTS = false;
	
	public static final String AUTH_TOKEN = "auth_token";
	public static final String LOGIN = "login";
	public static final String CHANNEL = "channel";
	
	public static final String EVENTS_CHANNEL = "Events";
	public static final int EVENTS_INTERVAL = 15;
	public static final int EVENTS_RADIUS = 3;
	public static final int RELEVANT_PERIOD_IN_HOURS = 240;
	public static final int TRACKING_INTERVAL = 1;
	
	private static final String DATE_FORMAT = "dd MM yyyy HH:mm:ss.SSS";
	private static final DateFormat dateFormat = new SimpleDateFormat(DATE_FORMAT);
	private static final DateFormat dateUtcFormat;
	static {
		dateUtcFormat = new SimpleDateFormat(DATE_FORMAT);
		dateUtcFormat.setTimeZone(TimeZone.getTimeZone("UTC"));
	}
	
	public static String getTime(Date date) {
		return dateFormat.format(date);
	}
	
	public static String getUtcTime(Date date) {
		return dateUtcFormat.format(date);
	}
	
	public static String getTimeFromUtcString(String time) {
		try {
			Date date = dateUtcFormat.parse(time);
			return date.toString();
		} catch (ParseException e) {
			if (DEBUG) {
				Log.v(LOG, "can't parse UTC time " + time);
			}
			return time;
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
	
	public static String convertLocation(Mark mark){
		return convertLocation(mark.getLatitude(), mark.getLongitude(), 0.0);
	}
	
	public static String convertLocation(Location loc){
		double alt = loc.hasAltitude() ? loc.getAltitude() : 0.0;
		return convertLocation(loc.getLatitude(), loc.getLongitude(), alt);
	}
	
	public static String convertLocation(double lat, double lon, double alt){
		StringBuffer strBuffer = new StringBuffer();
		strBuffer.append("lat: ").append(String.valueOf(lat)).
			append("  lon: ").append(String.valueOf(lon));
			// append("  alt: ").append(String.valueOf(alt));
		return strBuffer.toString();
	}

}
