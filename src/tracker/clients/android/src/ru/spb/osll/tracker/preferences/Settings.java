/*
 * Copyright 2010-2012  Vasily Romanikhin  bac1ca89@gmail.com
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */

/*! ---------------------------------------------------------------
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

package ru.spb.osll.tracker.preferences;

import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.InputSource;
import org.xml.sax.XMLReader;

import ru.spb.osll.tracker.R;
import ru.spb.osll.tracker.TrackerActivity;
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

	
	public static SharedPreferences getPreferences(Context c){
		return new Settings(c).getPreferences();
	}
	
	public interface ITrackerNetSettings{
		String TRACKER_SETTINGS = "tracker_settings";
		
		String LOGIN          = "login";
		String PASSWORD       = "password";
		String CHANNEL        = "channel";
	   	String SERVER_URL     = "server_url";
	   	String TIME_TICK      = "time_request";
		String RADIUS         = "radius";
		String URI            = "uri";
	}
	
	public interface ITrackerAppSettings{
		String IS_SHOW_TICKS  = "is_show_ticks";
		String IS_HIDE_APP    = "is_hide_app";
	}
		
}