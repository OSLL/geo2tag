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

package ru.spb.osll.web.server.json;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

import org.json.JSONException;
import org.json.JSONObject;

public class JsonFilterRectangleRequest {

	public double m_lat1;
	public double m_lat2;
	public double m_lon1;
	public double m_lon2;
	private String m_serverUrl;
	private String REQUEST = "/filterRectangle";
	private String LATITUDE1 = "latitude1";
	private String LATITUDE2 = "latitude2";
	private String LONGITUDE1 = "longitude1";
	private String LONGITUDE2 = "longitude2";
	private String LATITUDE_SHIFT = "latitude_shift";
	private String LONGITUDE_SHIFT = "longitude_shift";

	public JsonFilterRectangleRequest(String authToken, String timeFrom,
			String timeTo, double lat1, double lat2, double lon1, double lon2,
			String serverUrl) {
		m_serverUrl = serverUrl;
		m_lat1 = lat1;
		m_lat2 = lat2;
		m_lon1 = lon1;
		m_lon2 = lon2;
	}

	public String getServerUrl() {
		return m_serverUrl;
	}

	protected JSONObject getJsonObject() throws JSONException {
		JSONObject jsonObject = new JSONObject();

		JSONObject latShift = new JSONObject();
		latShift.put(LATITUDE1, m_lat1);
		latShift.put(LATITUDE2, m_lat2);

		JSONObject lonShift = new JSONObject();
		lonShift.put(LONGITUDE1, m_lon1);
		lonShift.put(LONGITUDE2, m_lon2);

		jsonObject.put(LATITUDE_SHIFT, latShift);
		jsonObject.put(LONGITUDE_SHIFT, lonShift);
		return jsonObject;
	}

	protected JSONObject doRequestInternal() throws JSONException, IOException,
			URISyntaxException {
		JSONObject jsonObject = getJsonObject();
		return JsonBase.instance().doRequest(jsonObject,
				new URI(getServerUrl() + REQUEST));
	}

	public JSONObject doRequest() {
		JSONObject resultJSON = null;
		try {
			resultJSON = doRequestInternal();
		} catch (Exception e) {

		}
		return resultJSON;
	}

}