package ru.spb.osll.json;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

import static ru.spb.osll.json.IRequest.IAddUser.*; 

public class JsonAddUserRequest extends JsonBaseRequest {
	private String m_login;
	private String m_password;
	private String m_serverUrl;
	
	public JsonAddUserRequest(String login, String password, String serverUrl){
		m_login = login;
		m_password = password;
		m_serverUrl = serverUrl;
	}

	@Override
	protected JSONObject doRequestInternal() throws JSONException, IOException,
			URISyntaxException {
		JSONObject jsonObject = new JSONObject();
		jsonObject.put(LOGIN, m_login);
		jsonObject.put(PASSWORD, m_password);
		Log.v(JSON_LOG, jsonObject.toString());
		return JsonBase.instance().doRequest(jsonObject, new URI(m_serverUrl + REQUEST));
	}

}
