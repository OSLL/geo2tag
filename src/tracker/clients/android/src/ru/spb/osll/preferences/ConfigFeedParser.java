package ru.spb.osll.preferences;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import ru.spb.osll.preferences.Settings.ITrackerAppSettings;
import ru.spb.osll.preferences.Settings.ITrackerNetSettings;
import android.content.SharedPreferences.Editor;

public class ConfigFeedParser extends DefaultHandler implements IXMLSettrings {
	private Editor m_configEditor;
	private StringBuffer m_sb;
	
	public ConfigFeedParser(Editor configEditor) {
		m_configEditor = configEditor;
	}
	
	@Override
    public void startElement(String uri, String localName, String qName, Attributes atts) throws SAXException {
		super.startElement(uri, localName, qName, atts);
		m_sb = new StringBuffer();
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

interface IXMLSettrings{
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

