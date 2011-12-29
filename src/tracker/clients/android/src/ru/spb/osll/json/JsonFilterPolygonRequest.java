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

import java.util.ArrayList;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import ru.spb.osll.objects.GeoPoint;

public class JsonFilterPolygonRequest extends JsonFilterBaseRequest {
	
	private List<GeoPoint> m_points = new ArrayList<GeoPoint>(); 
	
	public JsonFilterPolygonRequest(String authToken, String timeFrom, String timeTo, String serverUrl){
		super(authToken, timeFrom, timeTo, serverUrl);
	}
	
	public void addPoint(double lat, double lon) {
		m_points.add(new GeoPoint(lat, lon));
	}

	@Override
	protected JSONObject getJsonObject() throws JSONException {
		JSONObject jsonObject = super.getJsonObject();
		JSONArray jpoints = new JSONArray();
		for (int i = 0; i < m_points.size(); i++) {
			JSONObject jpoint = new JSONObject();
			jpoint.put("number", i);
			jpoint.put("latitude", m_points.get(i).x());
			jpoint.put("longitude", m_points.get(i).y());
			jpoints.put(jpoint);
		}		
		jsonObject.put("polygon", jpoints);
		return jsonObject;
	}
	
	@Override
	protected String getRequest() {
		return "/filterPolygon";
	}

	
}
