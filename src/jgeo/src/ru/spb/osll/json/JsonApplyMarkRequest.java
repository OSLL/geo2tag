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

package ru.spb.osll.json;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

import org.json.JSONException;
import org.json.JSONObject;

import ru.spb.osll.log.Log;

import static ru.spb.osll.json.IRequest.IApplyMark.*; 

public class JsonApplyMarkRequest extends JsonBaseRequest {
	private String m_authToken;
	private String m_channel;
	private String m_title;
	private String m_link;
	private String m_description;
	private double m_latitude;
	private double m_longitude;
	private double m_altitude;
	private String m_time;
	private String m_serverUrl;
	
	public JsonApplyMarkRequest(String authToken, String channel, String title, String link,
			String description, double latitude, double longitude, double altitude, String time, String serverUrl){
		m_authToken = authToken;
		m_channel = channel;
		m_title = title;
		m_link = link;
		m_description = description;
		m_latitude = latitude;
		m_longitude = longitude;
		m_altitude = altitude;
		m_time = time;
		m_serverUrl = serverUrl;
	}
	
	@Override
	protected JSONObject doRequestInternal() throws JSONException, IOException,
			URISyntaxException {
		JSONObject jsonObject = new JSONObject();
		jsonObject.put("auth_token", m_authToken);
		jsonObject.put("channel", m_channel);
		jsonObject.put("title", m_title);
		jsonObject.put("link", m_link);
		jsonObject.put("description", m_description);
		jsonObject.put("latitude", m_latitude);
		jsonObject.put("longitude", m_longitude);
		jsonObject.put("altitude", m_altitude);
		jsonObject.put("time", m_time);
		Log.out.println(JSON_LOG, jsonObject.toString());
		return JsonBase.instance().doRequest(jsonObject, new URI(m_serverUrl + REQUEST)); // TODO
	}
}
