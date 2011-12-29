package ru.spb.osll.json;

public class JsonFilterFenceRequest extends JsonFilterPolygonRequest {

	public JsonFilterFenceRequest(String authToken, double alt1, double alt2, String timeFrom, String timeTo, String serverUrl) {
		super(authToken, timeFrom, timeTo, serverUrl);
		setAltitudeInterval(alt1, alt2);
	}

	@Override
	protected String getRequest() {
		return "/filterFence";
	}
}
