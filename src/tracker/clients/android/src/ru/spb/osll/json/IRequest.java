package ru.spb.osll.json;

public interface IRequest {
	public String BASE_URL = "http://tracklife.ru/service"; // FIXME

	public interface ILogin{
		String REQUEST = "?query=login";
		
		String LOGIN = "login";
		String PASSWORD = "password";
	}

	public interface IAddUser{
		String REQUEST = "?query=addUser";
		
		String LOGIN = "login";
		String PASSWORD = "password";
	}

	public interface IApplyChannel{
		String REQUEST = "?query=addChannel";
		
		String AUTH_TOKEN = "auth_token";
		String NAME = "name";
		String DESCRIPTION = "description";
		String URL = "url";
		String ACTIVE_RADIUS = "activeRadius";
	}

	public interface IApplyMark{
		String REQUEST = "?query=apply";

		String AUTH_TOKEN = "auth_token";
		String CHANNEL = "channel";
		String TITLE = "title";
		String LINK = "link";
		String DESCRIPTION = "description";
		String LATITUDE = "latitude";
		String LONGITUDE = "longitude";
		String TIME = "time";
	}
	
	public interface IResponse{
		String STATUS = "status";
		String STATUS_DESCRIPTION = "status_description";
		String AUTH_TOKEN = "auth_token";
		
		
		
		String OK_STATUS = "Ok";
		String ERROR_STATUS = "Error";

		String USER_EXTSTS = "User exists!";
		String CHANNEL_EXTSTS = "Channel exists!";
	}
}
