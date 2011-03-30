package ru.spb.osll.json;

public interface IRequest {
	public String BASE_URL = "http://178.252.121.244:8080/service"; // FIXME

	public interface ILoginRequest{
		String REQUEST = BASE_URL + "?query=login";
		
		String LOGIN = "login";
		String PASSWORD = "password";

		String STATUS = "status";
		String STATUS_DESCRIPTION = "status_description";
		String AUTH_TOKEN = "auth_token";
	}

}
