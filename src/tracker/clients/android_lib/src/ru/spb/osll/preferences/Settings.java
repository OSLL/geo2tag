package ru.spb.osll.preferences;

import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.InputSource;
import org.xml.sax.XMLReader;

import ru.spb.osll.R;
import ru.spb.osll.TrackerActivity;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.util.Log;

public class Settings {
	private Context m_context;
	private static final String SETTINGS_STATUS = "settings_status"; 
	
	public Settings(Context context){
		m_context = context;
	}
	
	public SharedPreferences getPreferences(){
		return m_context.getSharedPreferences(ITrackerNetSettings.TRACKER_SETTINGS, 0);
	}
	
	public Editor getPreferencesEditor(){
		return getPreferences().edit();		
	}	
	
	public boolean isSettingsEmpty(){
		return getPreferences().getBoolean(SETTINGS_STATUS, true);
	}
	
	public void setDefaultSettrings(){
		Editor editor = getPreferencesEditor();
		loadFromXMLFile(editor);
		editor.putBoolean(SETTINGS_STATUS, false);
		editor.commit();
	}
	
	private void loadFromXMLFile(Editor configEditor) {
		try {
			InputSource is = 
				new InputSource(m_context.getResources().openRawResource(R.raw.tracker));
			SAXParserFactory factory = SAXParserFactory.newInstance();
			SAXParser parser = factory.newSAXParser();
			XMLReader xmlreader = parser.getXMLReader();
			xmlreader.setContentHandler(new ConfigFeedParser(configEditor));
			xmlreader.parse(is);
		} catch (Exception e) {
			Log.v(TrackerActivity.LOG, "loadFromXMLFile - exception: " + e.getMessage());
		}
	}
	
	public static boolean getPreference(Context c, String key, boolean defVal){
		return new Settings(c).getPreferences().getBoolean(key, defVal);
	}
	
	public interface ITrackerNetSettings{
		String TRACKER_SETTINGS = "tracker_settings";
		
		String LOGIN 			= "login";
		String PASSWORD 		= "password";
		String CHANNEL 			= "channel";
		String CHANNEL_KEY 		= "channel_key";
	   	String SERVER_URL 		= "server_url";
	   	String TIME_TICK 		= "time_request";
	   	String HISTORY_LIMIT 	= "history_limit";
		String RADIUS 			= "radius";
		String URI 				= "uri";
	}
	
	public interface ITrackerAppSettings{
		String IS_SHOW_TICKS = "is_show_ticks";
		String IS_HIDE_APP = "is_hide_app";
	}

}