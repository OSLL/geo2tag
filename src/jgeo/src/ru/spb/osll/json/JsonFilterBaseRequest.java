/*
 * Copyright 2010-2011  Vasily Romanikhin  bac1ca89@gmail.com
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

import static ru.spb.osll.json.IRequest.IBaseFilter.*;

abstract class JsonFilterBaseRequest extends JsonBaseRequest {
	
	private String m_authToken;
	private String m_timeFrom;
	private String m_timeTo;
	
	private double m_altitude1;
	private double m_altitude2;
	private boolean m_is3d = false;
	
	public JsonFilterBaseRequest(String authToken, String timeFrom, String timeTo, String serverUrl) {
		setServerUrl(serverUrl);
		m_authToken = authToken;
		m_timeFrom = timeFrom;
		m_timeTo = timeTo;
	}
	
	public JsonFilterBaseRequest(String authToken, String timeFrom, String timeTo, double alt1, double alt2, String serverUrl) {
		this(authToken, timeFrom, timeTo, serverUrl);
		m_is3d = true;
		m_altitude1 = alt1;
		m_altitude2 = alt2;
	}

	public void setAltitudeInterval(double alt1, double alt2) {
		m_is3d = true;
		m_altitude1 = alt1;
		m_altitude2 = alt2;
	}
	
	protected JSONObject getJsonObject() throws JSONException {
		JSONObject jsonObject = new JSONObject();
		jsonObject.put(AUTH_TOKEN, m_authToken);
		jsonObject.put(TIME_FROM, m_timeFrom);
		jsonObject.put(TIME_TO, m_timeTo);
		if (m_is3d) {
			JSONObject altObject = new JSONObject();
			altObject.put(ALT1, m_altitude1);
			altObject.put(ALT2, m_altitude2);
			jsonObject.put(ALT_SHIFT, altObject);
		}
		return jsonObject;
	}
	
	protected abstract String getRequest();
	
	@Override
	protected JSONObject doRequestInternal() throws JSONException, IOException,
		URISyntaxException {
		JSONObject jsonObject = getJsonObject();
		Log.out.println(JSON_LOG, jsonObject.toString());
		return JsonBase.instance().doRequest(jsonObject, new URI(getServerUrl() + getRequest()));
	}
	
}
