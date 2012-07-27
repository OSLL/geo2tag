/*
 * Copyright 2011-2012 OSLL
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

package ru.spb.osll.web.server;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;

public class SettingsStorage {
	
	private static SettingsStorage m_instance;
	
	private String m_filename;
	private Map<Object, Object> m_internalSettings;
	
	private SettingsStorage() {
		m_filename = DefaultValues.SETTINGS_STORAGE_FILENAME;
		initInternalSettings();
	}
	
	public static SettingsStorage getInstance() {
	    if (m_instance == null) {	
	    	m_instance = new SettingsStorage();
	    }
	    return m_instance;
	}

	public void setFilename(String filename) {
		m_filename = filename;
	}
	
	public String getFilename() {
		return m_filename;
	}
	
	public Map<Object, Object> getInternalSettings() {
		return m_internalSettings;
	}
	
	public Object getValue(Object key) {
		Properties defaultSettings = new Properties();
		defaultSettings.put(DefaultValues.SERVER_URL, DefaultValues.DEFAULT_SERVER_URL_VALUE);
		
		Properties settings = new Properties(defaultSettings);
		FileInputStream is;
		try {
			is = new FileInputStream(m_filename);
			settings.load(is);
			return settings.get(key);
		} catch (IOException e) {
			e.printStackTrace();
			return m_internalSettings.get(key);
		}
	}
	
	private void initInternalSettings() {
		m_internalSettings = new HashMap<Object, Object>();
		m_internalSettings.put(DefaultValues.SERVER_URL, DefaultValues.DEFAULT_SERVER_URL_VALUE);
	}
}
