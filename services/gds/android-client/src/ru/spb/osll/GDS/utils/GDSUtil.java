package ru.spb.osll.GDS.utils;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import ru.spb.osll.objects.Mark;
import android.app.ActivityManager;
import android.app.ActivityManager.RunningServiceInfo;
import android.content.Context;
import android.location.Location;

public class GDSUtil {
	private static DateFormat dateFormat = new SimpleDateFormat("dd MM yyyy HH:MM:ss.SSS");
	public static String getTime(Date date){
		return dateFormat.format(date);
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
		strBuffer.append("lat: ").append(Location.convert(lat, Location.FORMAT_MINUTES)).
		append("  lon: ").append(Location.convert(lon, Location.FORMAT_MINUTES)).
		append("  alt: ").append(Location.convert(alt, Location.FORMAT_DEGREES));
		return strBuffer.toString();
	}

}
