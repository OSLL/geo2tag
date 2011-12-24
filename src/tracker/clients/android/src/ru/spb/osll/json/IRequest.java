package ru.spb.osll.json;

public interface IRequest {

	public interface ICommon {
		String ERRNO = "errno";
	}
	
	public interface ILogin{
		String REQUEST = "/login";
		
		String LOGIN = "login";
		String PASSWORD = "password";
	}

	public interface IAddUser{
		String REQUEST = "/addUser";
		
		String LOGIN = "login";
		String PASSWORD = "password";
	}

	public interface IApplyChannel{
		String REQUEST = "/addChannel";
		
		String AUTH_TOKEN = "auth_token";
		String NAME = "name";
		String DESCRIPTION = "description";
		String URL = "url";
		String ACTIVE_RADIUS = "activeRadius";
	}

	public interface IApplyMark{
		String REQUEST = "/apply";

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

		String USER_EXTSTS = "Username already exists!";
		String CHANNEL_EXTSTS = "Channel exists!";
	}
	
	public interface IBaseFilter{
		String AUTH_TOKEN 		= "auth_token";
		String TIME_FROM 		= "time_from";
		String TIME_TO 			= "time_to";
		
		String ALT_SHIFT 		= "altitude_shift";
		String ALT1 			= "altitude1";
		String ALT2 			= "altitude2";		
	}
	
	public interface ICircleFilter extends ICommon {
		String REQUEST 			= "/filterCircle";
		
		String LATITUDE 		= "latitude";
		String LONGITUDE 		= "longitude";
		String RADIUS 			= "radius";
	}
}
