package ru.spb.osll.json;

public interface IRequest {
	public String BASE_URL = "http://178.252.121.244:8080/service"; // FIXME

	public interface ILogin{
		String REQUEST = BASE_URL + "?query=login";
		
		String LOGIN = "login";
		String PASSWORD = "password";

		String STATUS = "status";
		String STATUS_DESCRIPTION = "status_description";
		String AUTH_TOKEN = "auth_token";
	}
	
	public interface IApplyChannel{
		String REQUEST = BASE_URL + "?query=addChannel";
		
		String AUTH_TOKEN = "auth_token";
		String NAME = "name";
		String DESCRIPTION = "description";
		String URL = "url";
		String ACTIVE_RADIUS = "activeRadius";

        String STATUS = "status";
        String STATUS_DESCRIPTION = "status_description";
	}

	public interface IApplyMark{
		String REQUEST = BASE_URL + "?query=apply";

		String AUTH_TOKEN = "auth_token";
		String CHANNEL = "channel";
		String TITLE = "title";
		String LINK = "link";
		String DESCRIPTION = "description";
		String LATITUDE = "latitude";
		String LONGITUDE = "longitude";
		String TIME = "time";
		
        String STATUS = "status";
        String STATUS_DESCRIPTION = "status_description";
	}

}
