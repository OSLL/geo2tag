package ru.spb.osll.preferences;

import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.Attributes;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.DefaultHandler;

import ru.spb.osll.R;
import ru.spb.osll.TrackerActivity;
import ru.spb.osll.json.IRequest;
import android.R.string;
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
	
	// TODO
	public void setDefaultSettrings(){
		
		
		Editor editor = getPreferencesEditor();

		editor.putString(ITrackerNetSettings.LOGIN, "Mark");	
		editor.putString(ITrackerNetSettings.PASSWORD, "test");
		editor.putString(ITrackerNetSettings.CHANNEL, "Test channel");
		editor.putString(ITrackerNetSettings.CHANNEL_KEY, "");
		editor.putString(ITrackerNetSettings.SERVER_URL, IRequest.BASE_URL);
		editor.putInt(ITrackerNetSettings.TIME_TICK, 5);
		
		editor.putBoolean(ITrackerAppSettings.IS_SHOW_TICKS, false);
		editor.putBoolean(ITrackerAppSettings.IS_HIDE_APP, true);
		
		editor.putBoolean(SETTINGS_STATUS, false);
		editor.commit();
	}
	
	public void examineXMLFile() {
		try {
			InputSource is = 
				new InputSource(m_context.getResources().openRawResource(R.raw.tracker));

			// create the factory
			SAXParserFactory factory = SAXParserFactory.newInstance();

			// create a parser
			SAXParser parser = factory.newSAXParser();

			// create the reader (scanner)
			XMLReader xmlreader = parser.getXMLReader();

			// instantiate our handler
			
			// assign our handler
//			Config config = new Config();
			xmlreader.setContentHandler(new ConfigFeedHandler());

			// perform the synchronous parse
			xmlreader.parse(is);

//			Log.v(TrackerActivity.LOG, config.toString());
			
			// should be done... let's display our results
//			tvData.setText(tfh.getResults());
		} catch (Exception e) {
//			tvData.setText(e.getMessage());
		}

	}

	
	public static SharedPreferences getPreferences(Context c){
		return new Settings(c).getPreferences();
	}
	
	public interface ITrackerNetSettings{
		String TRACKER_SETTINGS = "tracker_settings";
		
		String LOGIN = "login";
		String PASSWORD = "password";
		String CHANNEL = "channel";
		String CHANNEL_KEY = "channel_key";
	   	String SERVER_URL = "server_url";
	   	String TIME_TICK = "time_request";
		String RADIUS = "radius";
		String URI = "uri";
	}
	
	public interface ITrackerAppSettings{
		String IS_SHOW_TICKS = "is_show_ticks";
		String IS_HIDE_APP = "is_hide_app";
	}
	
//	private class Config{
//		public String login;
//		public String password;
//		public String channel;
//		public String channel_key;
//
//		@Override
//		public String toString() {
//			super.toString();
//			return login + " " + password + " " + channel + " " + channel_key;
//		}
//	}
	
	private class ConfigFeedHandler extends DefaultHandler implements IXMLSettrings {
		private Editor m_configEditor;
		private StringBuffer m_sb;
		
		@Override
	    public void startElement(String uri, String localName, String qName, Attributes atts) throws SAXException {
			super.startElement(uri, localName, qName, atts);
			m_sb = new StringBuffer();
	    	if (localName.equals(config)){
	    		 m_configEditor = getPreferencesEditor();
	    	}
	    }

		@Override
		public void endElement(String uri, String localName, String qName) throws SAXException {
			super.endElement(uri, localName, qName);
			if (localName.equals(config)){
				m_configEditor.commit();
			} else if (localName.equals(login)){
	    		m_configEditor.putString(ITrackerNetSettings.LOGIN, m_sb.toString());
	    	} else if (localName.equals(password)){
	    		m_configEditor.putString(ITrackerNetSettings.PASSWORD, m_sb.toString());
	    	} else if (localName.equals(channel)){
	    		m_configEditor.putString(ITrackerNetSettings.CHANNEL, m_sb.toString());
	    	} else if (localName.equals(key)){
	    		m_configEditor.putString(ITrackerNetSettings.CHANNEL_KEY, m_sb.toString());
	    	} else if (localName.equals(server_url)){
	    		m_configEditor.putString(ITrackerNetSettings.SERVER_URL, m_sb.toString());
	    	} else if (localName.equals(time_tick)){
	    		m_configEditor.putInt(ITrackerNetSettings.TIME_TICK, Integer.parseInt(m_sb.toString()));
	    	} else if (localName.equals(show_tick)){
	    		boolean val = m_sb.toString().equals("true") ? true : false; 
	    		m_configEditor.putBoolean(ITrackerAppSettings.IS_SHOW_TICKS, val);
	    	} else if (localName.equals(hide_app)){
	    		boolean val = m_sb.toString().equals("true") ? true : false; 
	    		m_configEditor.putBoolean(ITrackerAppSettings.IS_HIDE_APP, val);
	    	} 
		}

	    @Override
	    public void characters(char ch[], int start, int length) {
	        String str = new String(ch, start, length);
	        m_sb.append(str);
	    }
	}
	
	public interface IXMLSettrings{
		String config = "config";
		String login = "login";
		String password = "password";
		String channel = "channel";
		String key = "key";
		String server_url = "server_url";
		String time_tick = "time_tick";
		String show_tick = "show_tick";
		String hide_app = "hide_app";
	}
	
}