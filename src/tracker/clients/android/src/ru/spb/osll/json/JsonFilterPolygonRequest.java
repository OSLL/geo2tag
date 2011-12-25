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
