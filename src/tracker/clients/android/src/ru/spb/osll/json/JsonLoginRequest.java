package ru.spb.osll.json;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

import org.json.JSONException;
import org.json.JSONObject;

public class JsonLoginRequest implements IRequest.ILoginRequest {
	
	private String m_login;
	private String m_password;
	
	public JsonLoginRequest(String login, String password){
		m_login = login;
		m_password = password;
	}
	
	public JSONObject doRequest(){
		JSONObject resultJSON = null; 
		try {
			JSONObject jsonObject = new JSONObject();
			jsonObject.put(LOGIN, m_login);
			jsonObject.put(PASSWORD, m_password);
			resultJSON = JsonBase.instance().doRequest(jsonObject, new URI(REQUEST));
		} catch (JSONException e) {
			// TODO Auto-generated catch block
		} catch (IOException e) {
			// TODO: handle exception
		} catch (URISyntaxException e) {
			// TODO Auto-generated catch block
		}
		return resultJSON;
	}

}

