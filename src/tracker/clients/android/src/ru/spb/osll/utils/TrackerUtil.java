package ru.spb.osll.utils;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

public class TrackerUtil {
	private static DateFormat dateFormat = new SimpleDateFormat("dd MM yyyy HH:MM:ss.SSS");
	public static String getTime(Date date){
		return dateFormat.format(date);
	}
	
	//TODO
}
