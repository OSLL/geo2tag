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
