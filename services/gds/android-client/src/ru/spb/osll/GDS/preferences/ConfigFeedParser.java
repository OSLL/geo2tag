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

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

public class ConfigFeedParser extends DefaultHandler implements IXMLSettings {
	private StringBuffer m_sb;
	private Settings m_settings;
	
	public ConfigFeedParser(Settings settings) {
		m_settings = settings;
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

		} else if (localName.equals(login)){
			//m_settings.setLogin(m_sb.toString());
    	} else if (localName.equals(password)){
    		//m_settings.setPassword(m_sb.toString());
    	} else if (localName.equals(remember)){
    		m_settings.setRememberMe(Boolean.parseBoolean(m_sb.toString()));
    	} else if (localName.equals(channel)){
 
    	} else if (localName.equals(key)){

    	} else if (localName.equals(server_url)){
    		m_settings.setServerUrl(m_sb.toString());
    	} else if (localName.equals(time_tick)){
    		//m_configEditor.putInt(IGDSSettings.TIME_TICK, Integer.parseInt(m_sb.toString()));
    	} else if (localName.equals(show_tick)){
    		//boolean val = m_sb.toString().equals("true") ? true : false; 
    		//m_configEditor.putBoolean(IGDSSettings.IS_SHOW_TICKS, val);
    	} else if (localName.equals(hide_app)){
    		//boolean val = m_sb.toString().equals("true") ? true : false; 
    		//m_configEditor.putBoolean(IGDSSettings.IS_HIDE_APP, val);
    	} 
	}

    @Override
    public void characters(char ch[], int start, int length) {
        String str = new String(ch, start, length);
        m_sb.append(str);
    }
}

interface IXMLSettings{
	String config = "config";
	String login = "login";
	String password = "password";
	String remember = "remember";
	String channel = "channel";
	String key = "key";
	String server_url = "server_url";
	String time_tick = "time_tick";
	String show_tick = "show_tick";
	String hide_app = "hide_app";
}

