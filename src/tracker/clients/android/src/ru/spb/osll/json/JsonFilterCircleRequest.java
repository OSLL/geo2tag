package ru.spb.osll.json;

import org.json.JSONException;
import org.json.JSONObject;

public class JsonFilterCircleRequest extends JsonFilterBaseRequest {

	public double m_lat;
	public double m_lon;
	public double m_rad;
	
	public JsonFilterCircleRequest(String authToken, double lat, 
		double lon, double rad, String timeFrom, String timeTo, String serverUrl){
		super(authToken, timeFrom, timeTo);
		setServerUrl(serverUrl);
		m_lat = lat;
		m_lon = lon;
		m_rad = rad;
	}
	
	@Override
	protected JSONObject getJsonObject() throws JSONException {
		JSONObject jsonObject = super.getJsonObject();
		jsonObject.put("latitude", m_lat);
		jsonObject.put("longitude", m_lon);
		jsonObject.put("radius", m_rad);
		return jsonObject;
	}

	@Override
	protected String getRequest() {
		return "/filterCircle";
	}

}
