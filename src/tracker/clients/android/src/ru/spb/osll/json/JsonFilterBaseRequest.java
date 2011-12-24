package ru.spb.osll.json;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

import static ru.spb.osll.json.IRequest.IBaseFilter.*;

abstract class JsonFilterBaseRequest extends JsonBaseRequest {
	
	private String m_authToken;
	private String m_timeFrom;
	private String m_timeTo;
	
	private double m_altitude1;
	private double m_altitude2;
	private boolean m_is3d = false;
	
	public JsonFilterBaseRequest(String authToken, String timeFrom, String timeTo) {
		m_authToken = authToken;
		m_timeFrom = timeFrom;
		m_timeTo = timeTo;
	}
	
	public JsonFilterBaseRequest(String authToken, String timeFrom, String timeTo, double alt1, double alt2) {
		this(authToken, timeFrom, timeTo);
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
		Log.v(JSON_LOG, jsonObject.toString());
		return JsonBase.instance().doRequest(jsonObject, new URI(getServerUrl() + getRequest()));
	}
	
}
