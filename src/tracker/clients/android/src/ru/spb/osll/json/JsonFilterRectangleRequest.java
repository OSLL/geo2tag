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

import org.json.JSONException;
import org.json.JSONObject;

public class JsonFilterRectangleRequest extends JsonFilterBaseRequest {

	public double m_lat1;
	public double m_lat2;
	public double m_lon1;
	public double m_lon2;

	public JsonFilterRectangleRequest(String authToken, double lat1, double lat2, 
		double lon1, double lon2,  String timeFrom, String timeTo, String serverUrl) {
		super(authToken, timeFrom, timeTo, serverUrl);
		m_lat1 = lat1;
		m_lat2 = lat2;
		m_lon1 = lon1;
		m_lon2 = lon2;
	}
	
	@Override
	protected JSONObject getJsonObject() throws JSONException {
		JSONObject jsonObject = super.getJsonObject();
		
		JSONObject latShift = new JSONObject();
		latShift.put("latitude1", m_lat1);
		latShift.put("latitude2", m_lat2);
		
		JSONObject lonShift = new JSONObject();
		lonShift.put("longitude1", m_lon1);
		lonShift.put("longitude2", m_lon2);

		jsonObject.put("latitude_shift", latShift);
		jsonObject.put("longitude_shift", lonShift);
		return jsonObject;
	}
	
	@Override
	protected String getRequest() {
		return "/filterRectangle";
	}

}
