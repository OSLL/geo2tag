package ru.spb.osll.web.server.json;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

import org.json.JSONException;
import org.json.JSONObject;

public class JsonLoginRequest{
	private String m_login;
	private String m_password;
	private String m_serverUrl;
	private String REQUEST = "/login";
	private String LOGIN = "login";
	private String PASSWORD = "password";
	
	public JsonLoginRequest(String login, String password, String serverUrl){
		m_serverUrl = serverUrl;
		m_login = login;
		m_password = password;
	}
	public String getServerUrl(){
		return m_serverUrl;
	}
	
	protected JSONObject doRequestInternal() throws JSONException, IOException,
			URISyntaxException {
		JSONObject jsonObject = new JSONObject();
		jsonObject.put(LOGIN, m_login);
		jsonObject.put(PASSWORD, m_password);
		return JsonBase.instance().doRequest(jsonObject, new URI(getServerUrl() + REQUEST));
		}
	
	public JSONObject doRequest(){
		JSONObject resultJSON = null; 
		try {
			resultJSON = doRequestInternal(); 
		} catch (Exception e) {
			
		}
		return resultJSON;
	}
}
