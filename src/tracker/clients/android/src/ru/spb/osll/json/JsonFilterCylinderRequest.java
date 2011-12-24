package ru.spb.osll.json;

public class JsonFilterCylinderRequest extends JsonFilterCircleRequest {

	public JsonFilterCylinderRequest(String authToken, double lat, double lon, double rad,
		double alt1, double alt2, String timeFrom, String timeTo, String serverUrl) {
		super(authToken, lat, lon, rad, timeFrom, timeTo, serverUrl);
		setAltitudeInterval(alt1, alt2);
	}
	
	@Override
	protected String getRequest() {
		return "/filterCylinder";
	}

}
