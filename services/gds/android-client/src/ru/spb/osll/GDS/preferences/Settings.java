/*
 * Copyright 2010-2011  Vasily Romanikhin  bac1ca89@gmail.com
 * Copyright 2012  Ivan Bezyazychnyy  ivan.bezyazychnyy@gmail.com
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

package ru.spb.osll.GDS.preferences;

import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.InputSource;
import org.xml.sax.XMLReader;

import ru.spb.osll.GDS.R;
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
		return m_context.getSharedPreferences(IGDSSettings.GDS_SETTINGS, 0);
	}
	
	public Editor getPreferencesEditor(){
		return getPreferences().edit();		
	}	
	
	public boolean isSettingsEmpty(){
		return getPreferences().getBoolean(SETTINGS_STATUS, true);
	}
	
	public void setDefaultSettings(){
		Editor editor = getPreferencesEditor();
		loadFromXMLFile(editor);
		editor.putBoolean(SETTINGS_STATUS, false);
		editor.commit();
	}
	
	private void loadFromXMLFile(Editor configEditor) {
		try {
			InputSource is = new InputSource(
					m_context.getResources().openRawResource(R.raw.gds_client));
			SAXParserFactory factory = SAXParserFactory.newInstance();
			SAXParser parser = factory.newSAXParser();
			XMLReader xmlreader = parser.getXMLReader();
			xmlreader.setContentHandler(new ConfigFeedParser(configEditor));
			xmlreader.parse(is);
		} catch (Exception e) {
			Log.v(m_context.getString(R.string.app_name),
					"loadFromXMLFile - exception: " + e.getMessage());
		}
	}
	
	public String getLogin() {
		SharedPreferences prefs = m_context.getSharedPreferences(IGDSSettings.GDS_SETTINGS, 0);
	    return prefs.getString(IGDSSettings.LOGIN, "");
	}
	
	public void setLogin(String login) {
		SharedPreferences prefs = m_context.getSharedPreferences(IGDSSettings.GDS_SETTINGS, 0);
	    prefs.edit().putString(IGDSSettings.LOGIN, login).commit();
	}
	
	public String getPassword() {
		SharedPreferences prefs = m_context.getSharedPreferences(IGDSSettings.GDS_SETTINGS, 0);
	    return prefs.getString(IGDSSettings.LOGIN, "");
	}
	
	public void setPassword(String login) {
		SharedPreferences prefs = m_context.getSharedPreferences(IGDSSettings.GDS_SETTINGS, 0);
	    prefs.edit().putString(IGDSSettings.LOGIN, login).commit();
	}

	
	public static SharedPreferences getPreferences(Context c){
		return new Settings(c).getPreferences();
	}
	
	public interface IGDSSettings{
		String GDS_SETTINGS = "gds_settings";
		
		String LOG = "GeoDoctorSearch";
		int ATTEMPTS = 5;
		
		String LOGIN = "login";
		String PASSWORD = "password";
		String REMEMBER = "remember";
		String CHANNEL = "channel";
		String CHANNEL_KEY = "channel_key";
	   	String SERVER_URL = "server_url";
	   	String TIME_TICK = "time_request";
		String RADIUS = "radius";
		String URI = "uri";
		String IS_SHOW_TICKS = "is_show_ticks";
		String IS_HIDE_APP = "is_hide_app";
	}
		
}