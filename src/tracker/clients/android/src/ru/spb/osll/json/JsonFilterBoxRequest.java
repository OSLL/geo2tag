package ru.spb.osll.json;

public class JsonFilterBoxRequest extends JsonFilterRectangleRequest {

	public JsonFilterBoxRequest(String authToken, double lat1, double lat2, double lon1, 
			double lon2, double alt1, double alt2, String timeFrom, String timeTo, String serverUrl) {
		super(authToken, lat1, lat2, lon1, lon2, timeFrom, timeTo, serverUrl);
		setAltitudeInterval(alt1, alt2);
	}
	
	@Override
	protected String getRequest() {
		return "/filterBox";
	}

}
